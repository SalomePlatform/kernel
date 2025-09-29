// Copyright (C) 2007-2025  CEA, EDF, OPEN CASCADE
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
// Author : Anthony GEAY (EDF R&D)

#include "SALOMESDS_DataScopeServer.hxx"
#include "SALOMESDS_DataServerManager.hxx"
#include "SALOMESDS_PickelizedPyObjRdOnlyServer.hxx"
#include "SALOMESDS_PickelizedPyObjRdExtServer.hxx"
#include "SALOMESDS_PickelizedPyObjRdWrServer.hxx"
#include "SALOMESDS_PickelizedPyObjRdExtInitServer.hxx"
#include "SALOMESDS_TrustTransaction.hxx"
#include "SALOMESDS_KeyWaiter.hxx"
#include "SALOMESDS_Transaction.hxx"
#include "SALOME_NamingService_Abstract.hxx"
#include "SALOMESDS_Exception.hxx"

#include <sstream>
#include <iterator>
#include <algorithm>

// agy : awful, to be factorized with ContainerManager.
#ifndef WIN32
#include <unistd.h>
#else
#include <process.h>
#define getpid _getpid
#endif

using namespace SALOMESDS;

std::size_t DataScopeServerBase::COUNTER=0;

void DataScopeKiller::shutdown()
{
  _orb->shutdown(0);
}

RequestSwitcher::RequestSwitcher(CORBA::ORB_ptr orb, DataScopeServerTransaction *ds):RequestSwitcherBase(orb),_ds(ds)
{
}

SALOME_CMOD::StringVec *RequestSwitcher::listVars()
{
  return _ds->listVars();
}

SALOME_CMOD::ByteVec *RequestSwitcher::fetchSerializedContent(const char *varName)
{
  return _ds->fetchSerializedContent(varName);
}

void RequestSwitcher::fetchAndGetAccessOfVar(const char *varName, CORBA::String_out access, SALOME_CMOD::ByteVec_out data)
{
  return _ds->fetchAndGetAccessOfVar(varName,access,data);
}

DataScopeServerBase::DataScopeServerBase(const SALOME_CPythonHelper *pyHelper, CORBA::ORB_ptr orb, SALOME_CMOD::DataScopeKiller_var killer, const std::string& scopeName, SALOME_NamingService_Container_Abstract *ns):_ns(ns),_pyHelper(pyHelper),_orb(CORBA::ORB::_duplicate(orb)),_name(scopeName),_killer(killer)
{
}

DataScopeServerBase::DataScopeServerBase(const DataScopeServerBase& other):omniServant(other),ServantBase(other),_ns(other._ns->clone()),_pyHelper(other._pyHelper),_name(other._name),_vars(other._vars),_killer(other._killer)
{
}

DataScopeServerBase::~DataScopeServerBase()
{
  for(std::list< std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > >::const_iterator it=_vars.begin();it!=_vars.end();it++)
    {
       BasicDataServer *obj((*it).second);
       if(obj)
         {
           obj->decrRef();
         }
    }
  delete _ns;
}

/*!
 * Called remotely -> to protect against throw
 */
void DataScopeServerBase::ping()
{
}

/*!
 * Called remotely -> to protect against throw
 */
char *DataScopeServerBase::getScopeName()
{
  return CORBA::string_dup(_name.c_str());
}

/*!
 * Called remotely -> to protect against throw
 */
SALOME_CMOD::StringVec *DataScopeServerBase::listVars()
{
  SALOME_CMOD::StringVec *ret(new SALOME_CMOD::StringVec);
  std::size_t sz(_vars.size());
  ret->length((CORBA::ULong)sz); //!< TODO: size_t to CORBA::ULong
  std::list< std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > >::iterator it(_vars.begin());
  for(std::size_t i=0;i<sz;it++,i++)
    {
      BasicDataServer *obj((*it).second);
      std::string name(obj->getVarNameCpp());
      (*ret)[(CORBA::ULong)i]=CORBA::string_dup(name.c_str()); //!< TODO: size_t to CORBA::ULong
    }
  return ret;
}

CORBA::Boolean DataScopeServerBase::existVar(const char *varName)
{
  std::list< std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > >::const_iterator it(_vars.begin());
  for(;it!=_vars.end();it++)
    if((*it).second->getVarNameCpp()==varName)
      return true;
  return false;
}

SALOME_CMOD::BasicDataServer_ptr DataScopeServerBase::retrieveVarInternal(const char *varName)
{
  std::list< std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > >::const_iterator it0(retrieveVarInternal3(varName));
  return SALOME_CMOD::BasicDataServer::_duplicate((*it0).first);
}

BasicDataServer *DataScopeServerBase::retrieveVarInternal2(const std::string& varName)
{
  std::list< std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > >::const_iterator it0(retrieveVarInternal3(varName));
  return (*it0).second;
}

void DataScopeServerBase::deleteVar(const char *varName)
{
  std::string varNameCpp(varName);
  std::vector<std::string> allNames(getAllVarNames());
  std::vector<std::string>::iterator it(std::find(allNames.begin(),allNames.end(),varNameCpp));
  if(it==allNames.end())
    {
      std::ostringstream oss; oss << "DataScopeServerBase::deleteVar : name \"" << varNameCpp << "\" does not exists ! Possibilities are :";
      std::copy(allNames.begin(),allNames.end(),std::ostream_iterator<std::string>(oss,", "));
      throw Exception(oss.str());
    }
  std::size_t pos(std::distance(allNames.begin(),it));
  std::list< std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > >::iterator it0(_vars.begin());
  for(std::size_t ii=0;ii<pos;ii++,it0++);
  (*it0).second->decrRef();
  _vars.erase(it0);
}

CORBA::Boolean DataScopeServerBase::shutdownIfNotHostedByDSM(SALOME_CMOD::DataScopeKiller_out killer)
{
  CORBA::Object_var obj(_ns->Resolve(DataServerManager::NAME_IN_NS));
  SALOME_CMOD::DataServerManager_var dsm(SALOME_CMOD::DataServerManager::_narrow(obj));
  if(CORBA::is_nil(dsm))
    throw Exception("Unable to reach in the NS the unique DataServerManager instance of the Session !");
  // destroy ref in the naming service
  std::string fullScopeName(SALOMESDS::DataServerManager::CreateAbsNameInNSFromScopeName(_name));
  _ns->Destroy_Name(fullScopeName.c_str());
  // establish if dsm and this shared the same POA. If yes dsm and this are collocated !
  PortableServer::ServantBase *ret(0);
  try
    {
      ret=_poa->reference_to_servant(dsm);
    }
  catch(...) { ret=0; }
  //
  if(!ret)
    {
      enforcedRelease();
      killer=SALOME_CMOD::DataScopeKiller::_duplicate(_killer);
      return true;
    }
  else
    {
      ret->_remove_ref();
      enforcedRelease();
      killer=SALOME_CMOD::DataScopeKiller::_duplicate(_killer);
      return false;
    }
}

SALOME_CMOD::ByteVec *DataScopeServerBase::fetchSerializedContent(const char *varName)
{
  BasicDataServer *var(retrieveVarInternal2(varName));
  PickelizedPyObjServer *varc(dynamic_cast<PickelizedPyObjServer *>(var));
  if(!varc)
    {
      std::ostringstream oss; oss << "DataScopeServerBase::fetchSerializedContent : var \"" << varName << "\" exists but it is not serialized !";
      throw Exception(oss.str());
    }
  return varc->fetchSerializedContent();
}

SALOME_CMOD::SeqOfByteVec *DataScopeServerBase::getAllKeysOfVarWithTypeDict(const char *varName)
{
  BasicDataServer *var(retrieveVarInternal2(varName));
  PickelizedPyObjServer *varc(dynamic_cast<PickelizedPyObjServer *>(var));
  if(!varc)
    {
      std::ostringstream oss; oss << "DataScopeServerBase::getAllKeysOfVarWithTypeDict : var \"" << varName << "\" exists but it is not serialized !";
      throw Exception(oss.str());
    }
  if(!varc->isDict())
    {
      std::ostringstream oss; oss << "DataScopeServerBase::getAllKeysOfVarWithTypeDict : var \"" << varName << "\" exists but it is not a PyDict !";
      throw Exception(oss.str());
    }
  PyObject *keys(PyDict_Keys(varc->getPyObj()));
  if(!PyList_Check(keys))
    {
      std::ostringstream oss; oss << "DataScopeServerBase::getAllKeysOfVarWithTypeDict : var \"" << varName << "\" has keys but not of type list !";
      throw Exception(oss.str());
    }
  Py_ssize_t sz(PyList_Size(keys));
  SALOME_CMOD::SeqOfByteVec *ret(new SALOME_CMOD::SeqOfByteVec);
  ret->length((CORBA::ULong)sz); //!< TODO: convert Py_ssize_t in CORBA::ULong
  for(Py_ssize_t i=0;i<sz;i++)
    {
      PyObject *item(PyList_GetItem(keys,i));
      Py_XINCREF(item);
      std::string pickel(varc->pickelize(item));//item consumed
      PickelizedPyObjServer::FromCppToByteSeq(pickel,(*ret)[(CORBA::ULong)i]); //!< TODO: convert Py_ssize_t in CORBA::ULong
    }
  Py_XDECREF(keys);
  return ret;
}

SALOME_CMOD::ByteVec *DataScopeServerBase::getValueOfVarWithTypeDict(const char *varName, const SALOME_CMOD::ByteVec& constKey)
{
  BasicDataServer *var(retrieveVarInternal2(varName));
  PickelizedPyObjServer *varc(dynamic_cast<PickelizedPyObjServer *>(var));
  if(!varc)
    {
      std::ostringstream oss; oss << "DataScopeServerBase::getValueOfVarWithTypeDict : var \"" << varName << "\" exists but it is not serialized !";
      throw Exception(oss.str());
    }
  if(!varc->isDict())
    {
      std::ostringstream oss; oss << "DataScopeServerBase::getValueOfVarWithTypeDict : var \"" << varName << "\" exists but it is not a PyDict !";
      throw Exception(oss.str());
    }
  //
  std::string keyCpp;
  PickelizedPyObjServer::FromByteSeqToCpp(constKey,keyCpp);
  SALOME::AutoPyRef key(PickelizedPyObjServer::GetPyObjFromPickled(keyCpp,this));
  PyObject *value(PyDict_GetItem(varc->getPyObj(),key.get()));//borrowed
  if(!value)
    {
      std::ostringstream oss; oss << "DataScopeServerBase::getValueOfVarWithTypeDict : var \"" << varName << "\" seems to not have key specified !";
      throw Exception(oss.str());
    }
  Py_XINCREF(value);
  std::string ret(PickelizedPyObjServer::Pickelize(value,this));//value is consumed
  return PickelizedPyObjServer::FromCppToByteSeq(ret);
}

void DataScopeServerBase::takeANap(CORBA::Double napDurationInSec)
{
  if(napDurationInSec<0.)
    throw Exception("DataScopeServerBase::takeANap : negative value !");
#ifndef WIN32
  struct timespec req,rem;
  long nbSec((long)napDurationInSec);
  double remainTime(napDurationInSec-(double)nbSec);
  req.tv_sec=nbSec;
  req.tv_nsec=(long)(remainTime*1000000.);
  int ret(nanosleep(&req,&rem));
  if(ret!=0)
    throw Exception("DataScopeServerBase::takeANap : nap not finished as expected !");
#else
  throw Exception("DataScopeServerBase::takeANap : not implemented for Windows !");
#endif
}

void DataScopeServerBase::registerToSalomePiDict() const
{
  _pyHelper->registerToSalomePiDict("SALOME_DataScopeServerBase",getpid());
}

void DataScopeServerBase::setPOA(PortableServer::POA_var poa)
{
  _poa=poa;
}

void DataScopeServerBase::registerInNS(SALOME_CMOD::DataScopeServerBase_ptr ptr)
{
  std::string fullScopeName(SALOMESDS::DataServerManager::CreateAbsNameInNSFromScopeName(_name));
  _ns->Register(ptr,fullScopeName.c_str());
}

std::string DataScopeServerBase::BuildTmpVarNameFrom(const std::string& varName)
{
  std::ostringstream oss;
  oss << varName << "@" << COUNTER++;
  return oss.str();
}

std::vector< std::string > DataScopeServerBase::getAllVarNames() const
{
  std::size_t sz(_vars.size());
  std::vector<std::string> ret(sz);
  std::list< std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > >::const_iterator it(_vars.begin());
  for(std::size_t i=0;i<sz;it++,i++)
    ret[i]=(*it).second->getVarNameCpp();
  return ret;
}

bool DataScopeServerBase::isExistingVar(const std::string& varName) const
{
  std::vector<std::string> allNames(getAllVarNames());
  std::vector<std::string>::iterator it(std::find(allNames.begin(),allNames.end(),varName));
  return it!=allNames.end();
}

void DataScopeServerBase::checkNotAlreadyExistingVar(const std::string& varName) const
{
  if(isExistingVar(varName))
    {
      std::ostringstream oss; oss << "DataScopeServerBase::checkNotAlreadyExistingVar : name \"" << varName << "\" already exists !";
      throw Exception(oss.str());
    }
}

void DataScopeServerBase::checkExistingVar(const std::string& varName) const
{
  std::vector<std::string> allNames(getAllVarNames());
  std::vector<std::string>::iterator it(std::find(allNames.begin(),allNames.end(),varName));
  if(it==allNames.end())
    {
      std::ostringstream oss; oss << "DataScopeServerBase::checkExistingVar : name \"" << varName << "\" does not exist !";
      throw Exception(oss.str());
    }
}

PickelizedPyObjServer *DataScopeServerBase::checkVarExistingAndDict(const std::string& varName)
{
  checkExistingVar(varName);
  BasicDataServer *var(retrieveVarInternal2(varName.c_str()));
  PickelizedPyObjServer *ret(dynamic_cast<PickelizedPyObjServer *>(var));
  if(!ret)
    {
      std::ostringstream oss; oss << "TransactionAddKeyValueHard::prepareRollBackInCaseOfFailure : var \"" << varName << "\"exists but it is not serialized !";
      throw Exception(oss.str());
    }
  if(!ret->isDict())
    {
      std::ostringstream oss; oss << "TransactionAddKeyValueHard::prepareRollBackInCaseOfFailure : var \"" << varName << "\"exists but it is not a Dict !";
      throw Exception(oss.str());
    }
  return ret;
}

void DataScopeServerBase::moveStatusOfVarFromRdWrToRdOnly(const std::string& varName)
{
  std::list< std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > >::iterator it(retrieveVarInternal4(varName));
  std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * >& p(*it);
  PickelizedPyObjRdWrServer *varc(dynamic_cast<PickelizedPyObjRdWrServer *>(p.second));
  if(!varc)
    throw Exception("DataScopeServerBase::moveStatusOfVarFromRdWrToRdOnly : var is not a RdWr !");
  PyObject *pyobj(varc->getPyObj()); Py_XINCREF(pyobj);
  PickelizedPyObjRdOnlyServer *newVar(new PickelizedPyObjRdOnlyServer(this,varName,pyobj));
  CORBA::Object_var obj(newVar->activate());
  SALOME_CMOD::BasicDataServer_var obj2(SALOME_CMOD::BasicDataServer::_narrow(obj));
  p.first=obj2; p.second=newVar;
  varc->decrRef();
}

void DataScopeServerBase::moveStatusOfVarFromRdOnlyToRdWr(const std::string& varName)
{
  std::list< std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > >::iterator it(retrieveVarInternal4(varName));
  std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * >& p(*it);
  PickelizedPyObjRdOnlyServer *varc(dynamic_cast<PickelizedPyObjRdOnlyServer *>(p.second));
  if(!varc)
    throw Exception("DataScopeServerBase::moveStatusOfVarFromRdOnlyToRdWr : var is not a RdWr !");
  PyObject *pyobj(varc->getPyObj()); Py_XINCREF(pyobj);
  PickelizedPyObjRdWrServer *newVar(new PickelizedPyObjRdWrServer(this,varName,pyobj));
  CORBA::Object_var obj(newVar->activate());
  SALOME_CMOD::BasicDataServer_var obj2(SALOME_CMOD::BasicDataServer::_narrow(obj));
  p.first=obj2; p.second=newVar;
  varc->decrRef();
}

void DataScopeServerBase::moveStatusOfVarFromRdExtOrRdExtInitToRdExtInit(const std::string& varName)
{
  std::list< std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > >::iterator it(retrieveVarInternal4(varName));
  std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * >& p(*it);
  PickelizedPyObjRdExtServer *varc0(dynamic_cast<PickelizedPyObjRdExtServer *>(p.second));
  PickelizedPyObjRdExtInitServer *varc1(dynamic_cast<PickelizedPyObjRdExtInitServer *>(p.second));
  if(!varc0 && !varc1)
    throw Exception("DataScopeServerBase::moveStatusOfVarFromRdExtOrRdExtInitToRdExtInit : var is neither RdExt nor RdExtInit !");
  if(varc0)
    {
      PickelizedPyObjRdExtInitServer *newVar(varc0->buildInitInstanceFrom(varName));
      newVar->incrNbClients();
      CORBA::Object_var obj(newVar->activate());
      SALOME_CMOD::BasicDataServer_var obj2(SALOME_CMOD::BasicDataServer::_narrow(obj));
      p.first=obj2; p.second=newVar;
      varc0->decrRef();
    }
  else
    varc1->incrNbClients();
}

void DataScopeServerBase::moveStatusOfVarFromRdExtOrRdExtInitToRdExt(const std::string& varName)
{
  std::list< std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > >::iterator it(retrieveVarInternal4(varName));
  std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * >& p(*it);
  PickelizedPyObjRdExtInitServer *varc0(dynamic_cast<PickelizedPyObjRdExtInitServer *>(p.second));
  PickelizedPyObjRdExtServer *varc1(dynamic_cast<PickelizedPyObjRdExtServer *>(p.second));
  if(!varc0 && !varc1)
    throw Exception("DataScopeServerBase::moveStatusOfVarFromRdExtOrRdExtInitToRdExt : var is not a RdExtInit !");
  if(varc0)
    {
      if(varc0->decrNbClients())
        {
          PickelizedPyObjRdExtServer *newVar(varc0->buildStdInstanceFrom(varName));
          CORBA::Object_var obj(newVar->activate());
          SALOME_CMOD::BasicDataServer_var obj2(SALOME_CMOD::BasicDataServer::_narrow(obj));
          p.first=obj2; p.second=newVar;
          varc0->decrRef();
        }
    }
}

std::list< std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > >::const_iterator DataScopeServerBase::retrieveVarInternal3(const std::string& varName) const
{
  std::vector<std::string> allNames(getAllVarNames());
  std::vector<std::string>::iterator it(std::find(allNames.begin(),allNames.end(),varName));
  if(it==allNames.end())
    {
      std::ostringstream oss; oss << "DataScopeServerBase::retrieveVarInternal3 : name \"" << varName << "\" does not exists ! Possibilities are :";
      std::copy(allNames.begin(),allNames.end(),std::ostream_iterator<std::string>(oss,", "));
      throw Exception(oss.str());
    }
  std::size_t pos(std::distance(allNames.begin(),it));
  std::list< std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > >::const_iterator it0(_vars.begin());
  for(std::size_t i=0;i<pos;i++,it0++);
  return it0;
}

std::list< std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > >::iterator DataScopeServerBase::retrieveVarInternal4(const std::string& varName)
{
  std::vector<std::string> allNames(getAllVarNames());
  std::vector<std::string>::iterator it(std::find(allNames.begin(),allNames.end(),varName));
  if(it==allNames.end())
    {
      std::ostringstream oss; oss << "DataScopeServerBase::retrieveVarInternal4 : name \"" << varName << "\" does not exists ! Possibilities are :";
      std::copy(allNames.begin(),allNames.end(),std::ostream_iterator<std::string>(oss,", "));
      throw Exception(oss.str());
    }
  std::size_t pos(std::distance(allNames.begin(),it));
  std::list< std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > >::iterator it0(_vars.begin());
  for(std::size_t i=0;i<pos;i++,it0++);
  return it0;
}

///////

DataScopeServer::DataScopeServer(const SALOME_CPythonHelper *pyHelper, CORBA::ORB_ptr orb, SALOME_CMOD::DataScopeKiller_var killer, const std::string& scopeName, SALOME_NamingService_Container_Abstract *ns):DataScopeServerBase(pyHelper,orb,killer,scopeName,ns)
{
}

DataScopeServer::DataScopeServer(const DataScopeServer& other):omniServant(other),ServantBase(other),DataScopeServerBase(other)
{
}

SALOME_CMOD::PickelizedPyObjRdOnlyServer_ptr DataScopeServer::createRdOnlyVar(const char *varName, const SALOME_CMOD::ByteVec& constValue)
{
  std::string varNameCpp(varName);
  checkNotAlreadyExistingVar(varNameCpp);
  PickelizedPyObjRdOnlyServer *tmp(new PickelizedPyObjRdOnlyServer(this,varNameCpp,constValue));
  CORBA::Object_var ret(tmp->activate());
  std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > p(SALOME_CMOD::BasicDataServer::_narrow(ret),tmp);
  _vars.push_back(p);
  return SALOME_CMOD::PickelizedPyObjRdOnlyServer::_narrow(ret);
}

SALOME_CMOD::PickelizedPyObjRdExtServer_ptr DataScopeServer::createRdExtVar(const char *varName, const SALOME_CMOD::ByteVec& constValue)
{
  std::string varNameCpp(varName);
  checkNotAlreadyExistingVar(varNameCpp);
  PickelizedPyObjRdExtServer *tmp(new PickelizedPyObjRdExtServer(this,varNameCpp,constValue));
  CORBA::Object_var ret(tmp->activate());
  std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > p(SALOME_CMOD::BasicDataServer::_narrow(ret),tmp);
  _vars.push_back(p);
  return SALOME_CMOD::PickelizedPyObjRdExtServer::_narrow(ret);
}

SALOME_CMOD::PickelizedPyObjRdWrServer_ptr DataScopeServer::createRdWrVar(const char *typeName, const char *varName)
{
  std::string varNameCpp(varName),typeNameCpp(typeName);
  checkNotAlreadyExistingVar(varNameCpp);
  PickelizedPyObjRdWrServer *tmp(new PickelizedPyObjRdWrServer(this,typeNameCpp,varNameCpp));
  CORBA::Object_var ret(tmp->activate());
  std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > p(SALOME_CMOD::BasicDataServer::_narrow(ret),tmp);
  _vars.push_back(p);
  return SALOME_CMOD::PickelizedPyObjRdWrServer::_narrow(ret);
}

DataScopeServer::~DataScopeServer()
{
}

////////

DataScopeServerTransaction::DataScopeServerTransaction(const SALOME_CPythonHelper *pyHelper, CORBA::ORB_ptr orb, SALOME_CMOD::DataScopeKiller_var killer, const std::string& scopeName, SALOME_NamingService_Container_Abstract *ns):DataScopeServerBase(pyHelper,orb,killer,scopeName,ns)
{
  CORBA::Object_var obj(_orb->resolve_initial_references("RootPOA"));
  PortableServer::POA_var poa(PortableServer::POA::_narrow(obj));
  //
  PortableServer::POAManager_var mgr(poa->the_POAManager());
  CORBA::PolicyList policies;
  policies.length(1);
  PortableServer::ThreadPolicy_var threadPol(poa->create_thread_policy(PortableServer::ORB_CTRL_MODEL));
  policies[0]=PortableServer::ThreadPolicy::_duplicate(threadPol);
  PortableServer::POA_var poa2(poa->create_POA("POAForWaiters",mgr,policies));
  threadPol->destroy();
  //
  _poa_for_key_waiter=poa2;
}

DataScopeServerTransaction::DataScopeServerTransaction(const DataScopeServerTransaction& other):omniServant(other),ServantBase(other),DataScopeServerBase(other),_poa_for_key_waiter(other.getPOA4KeyWaiter())
{
}

char *DataScopeServerTransaction::getAccessOfVar(const char *varName)
{
  std::string varNameCpp(varName);
  checkExistingVar(varNameCpp);
  BasicDataServer *var(retrieveVarInternal2(varName));
  if(!var)
    throw Exception("DataScopeServerTransaction::getAccessOfVar : variable is NULL !");
  PickelizedPyObjServer *varc(dynamic_cast<PickelizedPyObjServer *>(var));
  if(!varc)
    throw Exception("DataScopeServerTransaction::getAccessOfVar : variable is not of type PickelizedPyObjServer !");
  std::string ret(varc->getAccessStr());
  return CORBA::string_dup(ret.c_str());
}

/*!
 * This method is here to retrieve atomically accessStr and picklization.
 */
void DataScopeServerTransaction::fetchAndGetAccessOfVar(const char *varName, CORBA::String_out access, SALOME_CMOD::ByteVec_out data)
{
  access=getAccessOfVar(varName);
  data=fetchSerializedContent(varName);
}

void DataScopeServerTransaction::createRdOnlyVarInternal(const std::string& varName, const SALOME_CMOD::ByteVec& constValue)
{
  checkNotAlreadyExistingVar(varName);
  PickelizedPyObjRdOnlyServer *tmp(new PickelizedPyObjRdOnlyServer(this,varName,constValue));
  CORBA::Object_var ret(tmp->activate());
  std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > p(SALOME_CMOD::BasicDataServer::_narrow(ret),tmp);
  _vars.push_back(p);
}

void DataScopeServerTransaction::createRdExtVarInternal(const std::string& varName, const SALOME_CMOD::ByteVec& constValue)
{
  checkNotAlreadyExistingVar(varName);
  PickelizedPyObjRdExtServer *tmp(new PickelizedPyObjRdExtServer(this,varName,constValue));
  CORBA::Object_var ret(tmp->activate());
  std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > p(SALOME_CMOD::BasicDataServer::_narrow(ret),tmp);
  _vars.push_back(p);
}

void DataScopeServerTransaction::createRdExtVarFreeStyleInternal(const std::string& varName, const SALOME_CMOD::ByteVec& constValue, std::string&& compareFuncContent, SALOME::AutoPyRef&& compareFunc)
{
  if(!isExistingVar(varName))
    {
      PickelizedPyObjRdExtFreeStyleServer *tmp(new PickelizedPyObjRdExtFreeStyleServer(this,varName,constValue,std::move(compareFuncContent),std::move(compareFunc)));
      CORBA::Object_var ret(tmp->activate());
      std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > p(SALOME_CMOD::BasicDataServer::_narrow(ret),tmp);
      _vars.push_back(p);
    }
  else
    {
      BasicDataServer *ds(retrieveVarInternal2(varName));
      if(!ds)
        {
          std::ostringstream oss;
          oss << "DataScopeServerTransaction::createRdExtVarFreeStyleInternal : internal error 1 for varname \"" << varName << "\"!";
          throw Exception(oss.str());
        }
      Sha1Keeper *ds2(dynamic_cast<Sha1Keeper *>(ds));
      if(!ds2)
        {
          std::ostringstream oss;
          oss << "DataScopeServerTransaction::createRdExtVarFreeStyleInternal : varname \"" << varName << "\" already exists with a non Sha1Keeper type !";
          throw Exception(oss.str());
        }
      PickelizedPyObjServer *ds3(dynamic_cast<PickelizedPyObjServer *>(ds));
      if(!ds3)
        {
          std::ostringstream oss;
          oss << "DataScopeServerTransaction::createRdExtVarFreeStyleInternal : varname \"" << varName << "\" already exists with a non PickelizedPyObjServer type !";
          throw Exception(oss.str());
        }
      std::vector<unsigned char> constValueAsCpp;
      Transaction::FromByteSeqToVB(constValue,constValueAsCpp);
      SALOME::AutoPyRef newObj(PickelizedPyObjServer::GetPyObjFromPickled(constValueAsCpp,this));
      if(newObj.isNull())
        {
          std::ostringstream oss;
          oss << "DataScopeServerTransaction::createRdExtVarFreeStyleInternal : varname \"" << varName << "\" already exists but input pickelized object is not loadable !";
          throw Exception(oss.str());
        }
      ds2->checkSame(varName,compareFuncContent,ds3->getPyObj(),newObj);
    }
}

void DataScopeServerTransaction::createRdExtInitVarInternal(const std::string& varName, const SALOME_CMOD::ByteVec& constValue)
{
  checkNotAlreadyExistingVar(varName);
  PickelizedPyObjRdExtInitServer *tmp(new PickelizedPyObjRdExtInitServer(this,varName,constValue));
  CORBA::Object_var ret(tmp->activate());
  std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > p(SALOME_CMOD::BasicDataServer::_narrow(ret),tmp);
  _vars.push_back(p);
}

void DataScopeServerTransaction::createRdWrVarInternal(const std::string& varName, const SALOME_CMOD::ByteVec& constValue)
{
  checkNotAlreadyExistingVar(varName);
  PickelizedPyObjRdWrServer *tmp(new PickelizedPyObjRdWrServer(this,varName,constValue));
  CORBA::Object_var ret(tmp->activate());
  std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > p(SALOME_CMOD::BasicDataServer::_narrow(ret),tmp);
  _vars.push_back(p);
}

SALOME_CMOD::Transaction_ptr DataScopeServerTransaction::createRdOnlyVarTransac(const char *varName, const SALOME_CMOD::ByteVec& constValue)
{
  checkNotAlreadyExistingVar(varName);
  TransactionRdOnlyVarCreate *ret(new TransactionRdOnlyVarCreate(this,varName,constValue));
  CORBA::Object_var obj(ret->activate());
  return SALOME_CMOD::Transaction::_narrow(obj);
}

SALOME_CMOD::Transaction_ptr DataScopeServerTransaction::createRdExtVarTransac(const char *varName, const SALOME_CMOD::ByteVec& constValue)
{
  checkNotAlreadyExistingVar(varName);
  TransactionRdExtVarCreate *ret(new TransactionRdExtVarCreate(this,varName,constValue));
  CORBA::Object_var obj(ret->activate());
  return SALOME_CMOD::Transaction::_narrow(obj);
}

SALOME_CMOD::Transaction_ptr DataScopeServerTransaction::createRdExtVarFreeStyleTransac(const char *varName, const SALOME_CMOD::ByteVec& constValue, const char *compareFuncContent)
{// no check on varName done here. Will be done on perform
  TransactionRdExtVarFreeStyleCreate *ret(new TransactionRdExtVarFreeStyleCreate(this,varName,constValue,compareFuncContent));
  CORBA::Object_var obj(ret->activate());
  return SALOME_CMOD::Transaction::_narrow(obj);
}

SALOME_CMOD::Transaction_ptr DataScopeServerTransaction::createRdExtInitVarTransac(const char *varName, const SALOME_CMOD::ByteVec& constValue)
{
  checkNotAlreadyExistingVar(varName);
  TransactionRdExtInitVarCreate *ret(new TransactionRdExtInitVarCreate(this,varName,constValue));
  CORBA::Object_var obj(ret->activate());
  return SALOME_CMOD::Transaction::_narrow(obj);
}

SALOME_CMOD::Transaction_ptr DataScopeServerTransaction::createRdWrVarTransac(const char *varName, const SALOME_CMOD::ByteVec& constValue)
{
  checkNotAlreadyExistingVar(varName);
  TransactionRdWrVarCreate *ret(new TransactionRdWrVarCreate(this,varName,constValue));
  CORBA::Object_var obj(ret->activate());
  return SALOME_CMOD::Transaction::_narrow(obj);
}

void DataScopeServerTransaction::addWaitKey(KeyWaiter *kw)
{
  if(!kw)
    throw Exception("DataScopeServerTransaction::addWaitKey : NULL input object !");
  _waiting_keys.push_back(kw);
}

void DataScopeServerTransaction::pingKey(PyObject *keyObj)
{
  std::size_t ii(0);
  // this part does nothing except to be sure that in notify key all will be OK.
  PyObject *args(PyTuple_New(1));
  PyTuple_SetItem(args,0,keyObj); Py_XINCREF(keyObj);
  for(std::list< KeyWaiter *>::iterator it=_waiting_keys.begin();it!=_waiting_keys.end();it++,ii++)
    {
      PyObject *waitKey((*it)->getKeyPyObj());
      PyObject *meth(PyObject_GetAttrString(waitKey,"__eq__"));
      if(!meth)
      {
    	  std::ostringstream oss; oss << "DataScopeServerTransaction::pingKey : for object id #" << ii << " no __eq__ in pyobj !";
    	  throw Exception(oss.str());
      }
      PyObject *res(PyObject_CallObject(meth,args));
      Py_XDECREF(meth);
      if(res==NULL)
        {
          std::ostringstream oss; oss << "DataScopeServerTransaction::pingKey : for object id #" << ii << " error during cmp(k,wk[i]) !";
          throw Exception(oss.str());
        }
      PyBool_Check(res);
      if(PyErr_Occurred())
        {
          std::ostringstream oss; oss << "DataScopeServerTransaction::pingKey : for object id #" << ii << " error during interpretation of cmp(k,wk[i]) !";
          throw Exception(oss.str());
        }
      Py_XDECREF(res);
    }
  Py_XDECREF(args);
}

void DataScopeServerTransaction::notifyKey(const std::string& varName, PyObject *keyObj, PyObject *valueObj)
{
  std::size_t ii(0);
  PyObject *args(PyTuple_New(1));
  PyTuple_SetItem(args,0,keyObj); Py_XINCREF(keyObj);
  std::list< KeyWaiter *> newList,listOfEltToWakeUp;
  for(std::list< KeyWaiter *>::iterator it=_waiting_keys.begin();it!=_waiting_keys.end();it++,ii++)
    {
      if((*it)->getVarName()!=varName)
        {
          newList.push_back(*it);
          continue;
        }
      PyObject *waitKey((*it)->getKeyPyObj());
      PyObject *meth(PyObject_GetAttrString(waitKey,"__eq__"));
      if(!meth)
      {
    	  std::ostringstream oss; oss << "DataScopeServerTransaction::pingKey : for object id #" << ii << " no __eq__ in pyobj !";
    	  throw Exception(oss.str());
      }
      PyObject *res(PyObject_CallObject(meth,args));
      Py_XDECREF(meth);
      if(!PyBool_Check(res))
      {
    	  std::ostringstream oss; oss << "DataScopeServerTransaction::pingKey : for object id #" << ii << " no __eq__ in pyobj !";
    	  throw Exception(oss.str());
      }
      if(PyErr_Occurred())
        {
          std::ostringstream oss; oss << "DataScopeServerTransaction::notifyKey : MAIN INTERNAL ERROR ! for object id #" << ii << " error during interpretation of cmp(k,wk[i]) !";
          throw Exception(oss.str());
        }
      if(res==Py_True)
        listOfEltToWakeUp.push_back(*it);
      else
        newList.push_back(*it);
      Py_XDECREF(res);
    }
  Py_XDECREF(args);
  for(std::list< KeyWaiter *>::iterator it=listOfEltToWakeUp.begin();it!=listOfEltToWakeUp.end();it++)
    (*it)->valueJustCome(valueObj);
  for(std::list< KeyWaiter *>::iterator it=listOfEltToWakeUp.begin();it!=listOfEltToWakeUp.end();it++)
    (*it)->go();
  _waiting_keys=newList;
}

SALOME_CMOD::Transaction_ptr DataScopeServerTransaction::addKeyValueInVarHard(const char *varName, const SALOME_CMOD::ByteVec& key, const SALOME_CMOD::ByteVec& value)
{
  checkVarExistingAndDict(varName);
  TransactionAddKeyValueHard *ret(new TransactionAddKeyValueHard(this,varName,key,value));
  CORBA::Object_var obj(ret->activate());
  return SALOME_CMOD::Transaction::_narrow(obj);
}

SALOME_CMOD::Transaction_ptr DataScopeServerTransaction::addKeyValueInVarErrorIfAlreadyExisting(const char *varName, const SALOME_CMOD::ByteVec& key, const SALOME_CMOD::ByteVec& value)
{
  checkVarExistingAndDict(varName);
  TransactionAddKeyValueErrorIfAlreadyExisting *ret(new TransactionAddKeyValueErrorIfAlreadyExisting(this,varName,key,value));
  CORBA::Object_var obj(ret->activate());
  return SALOME_CMOD::Transaction::_narrow(obj);
}

SALOME_CMOD::TransactionMultiKeyAddSession_ptr DataScopeServerTransaction::addMultiKeyValueSession(const char *varName)
{
  checkVarExistingAndDict(varName);
  TransactionMultiKeyAddSession *ret(new TransactionMultiKeyAddSession(this,varName));
  CORBA::Object_var obj(ret->activate());
  return SALOME_CMOD::TransactionMultiKeyAddSession::_narrow(obj);
}

SALOME_CMOD::Transaction_ptr DataScopeServerTransaction::removeKeyInVarErrorIfNotAlreadyExisting(const char *varName, const SALOME_CMOD::ByteVec& key)
{
  checkVarExistingAndDict(varName);
  TransactionRemoveKeyInVarErrorIfNotAlreadyExisting *ret(new TransactionRemoveKeyInVarErrorIfNotAlreadyExisting(this,varName,key));
  CORBA::Object_var obj(ret->activate());
  return SALOME_CMOD::Transaction::_narrow(obj);
}

SALOME_CMOD::TransactionRdWrAccess_ptr DataScopeServerTransaction::createWorkingVarTransac(const char *varName, const SALOME_CMOD::ByteVec& constValue)
{
  std::string varNameCpp(varName);
  checkNotAlreadyExistingVar(varName);
  PickelizedPyObjRdWrServer *tmp(new PickelizedPyObjRdWrServer(this,varNameCpp,constValue));
  CORBA::Object_var obj(tmp->activate());
  std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > p(SALOME_CMOD::BasicDataServer::_narrow(obj),tmp);
  _vars.push_back(p);
  //
  TransactionMorphRdWrIntoRdOnly *ret(new TransactionMorphRdWrIntoRdOnly(this,varName));
  CORBA::Object_var obj2(ret->activate());
  return SALOME_CMOD::TransactionRdWrAccess::_narrow(obj2);
}

SALOME_CMOD::Transaction_ptr DataScopeServerTransaction::killVarTransac(const char *varName)
{
  std::string varNameCpp(varName);
  checkExistingVar(varNameCpp);
  //
  TransactionKillVar *ret(new TransactionKillVar(this,varName));
  CORBA::Object_var obj2(ret->activate());
  return SALOME_CMOD::Transaction::_narrow(obj2);
}

SALOME_CMOD::KeyWaiter_ptr DataScopeServerTransaction::waitForKeyInVar(const char *varName, const SALOME_CMOD::ByteVec& keyVal)
{
  PickelizedPyObjServer *pickelObj(checkVarExistingAndDict(varName));
  KeyWaiter *ret(new KeyWaiter(pickelObj,keyVal));
  CORBA::Object_var obj(ret->activate());//KeyWaiter instance activated inside a multithread POA contrary to all of objects in SALOMESDS in single thread !
  return SALOME_CMOD::KeyWaiter::_narrow(obj);
}

SALOME_CMOD::KeyWaiter_ptr DataScopeServerTransaction::waitForKeyInVarAndKillIt(const char *varName, const SALOME_CMOD::ByteVec& keyVal, SALOME_CMOD::Transaction_out transac)
{
  PickelizedPyObjServer *pickelObj(checkVarExistingAndDict(varName));
  KeyWaiter *ret0(new KeyWaiter(pickelObj,keyVal));
  CORBA::Object_var obj(ret0->activate());//KeyWaiter instance activated inside a multithread POA contrary to all of objects in SALOMESDS in single thread !
  //
  TransactionRemoveKeyInVarErrorIfNotAlreadyExisting *ret1(new TransactionRemoveKeyInVarErrorIfNotAlreadyExisting(this,varName,keyVal));
  CORBA::Object_var obj2(ret1->activate());
  transac=SALOME_CMOD::Transaction::_narrow(obj2);
  //
  return SALOME_CMOD::KeyWaiter::_narrow(obj);
}

SALOME_CMOD::ByteVec *DataScopeServerTransaction::waitForMonoThrRev(SALOME_CMOD::KeyWaiter_ptr kw)
{
  PortableServer::ServantBase *ret(0);
  try
    {
      ret=_poa_for_key_waiter->reference_to_servant(kw);// Warning ref cnt of ret has been incremented !
    }
  catch(...) { ret=0; }
  KeyWaiter *retc(dynamic_cast<KeyWaiter *>(ret));
  if(!retc)
    throw Exception("DataScopeServerTransaction::invokeMonoThr : internal error 1 !");
  retc->_remove_ref();// restore the counter after _poa_for_key_waiter->reference_to_servant(kw)
  SALOME_CMOD::ByteVec *zeRet(retc->waitForMonoThr());
  retc->enforcedRelease();
  return zeRet;
}

SALOME_CMOD::ByteVec *DataScopeServerTransaction::waitForAndKill(SALOME_CMOD::KeyWaiter_ptr kw)
{
  PortableServer::ServantBase *ret(0);
  try
    {
      ret=_poa_for_key_waiter->reference_to_servant(kw);// Warning ref cnt of ret has been incremented !
    }
  catch(...) { ret=0; }
  KeyWaiter *retc(dynamic_cast<KeyWaiter *>(ret));
  if(!retc)
    throw Exception("DataScopeServerTransaction::invokeMonoThr : internal error 1 !");
  retc->_remove_ref();// restore the counter after _poa_for_key_waiter->reference_to_servant(kw)
  SALOME_CMOD::ByteVec *zeRet(retc->waitForAndKill());
  retc->enforcedRelease();
  return zeRet;
}

void DataScopeServerTransaction::atomicApply(const SALOME_CMOD::ListOfTransaction& transactions)
{
  std::size_t sz(transactions.length());
  if(sz==0)
    return ;
  std::vector< AutoServantPtr<Transaction> > transactionsCpp(sz);
  for(std::size_t i=0;i<sz;i++)
    {
      PortableServer::ServantBase *eltBase(0);
      Transaction *elt(0);
      try
        {
          eltBase=_poa->reference_to_servant(transactions[(CORBA::ULong)i]); //!< TODO: size_t to CORBA::ULong
          elt=dynamic_cast<Transaction *>(eltBase);
        }
      catch(...)
        {
          std::ostringstream oss; oss << "DataScopeServerTransaction::atomicApply : the elt #" << i << " is invalid !";
          throw Exception(oss.str());
        }
      if(!elt)
        {
          std::ostringstream oss; oss << "DataScopeServerTransaction::atomicApply : the elt #" << i << " is null ?";
          throw Exception(oss.str());
        }
      elt->_remove_ref();
      transactionsCpp[i]=elt;
    }
  {// important do not merge loops ! 
    std::vector<TrustTransaction> transactions2(sz);
    bool mustRollback(true);
    for(std::size_t i=0;i<sz;i++)
      transactions2[i].setTransaction(transactionsCpp[i],&mustRollback);
    for(std::size_t i=0;i<sz;i++)
      transactions2[i].operate();
    mustRollback=false;
  }
  for(std::size_t i=0;i<sz;i++)
    transactionsCpp[i]->notify();
}

DataScopeServerTransaction::~DataScopeServerTransaction()
{
}

SALOME_CMOD::RequestSwitcher_ptr DataScopeServerTransaction::getRequestSwitcher()
{
  if(_rs.isNull())
    {
      _rs=new RequestSwitcher(_orb,this);
    }
  CORBA::Object_var obj(_rs->activate());
  return SALOME_CMOD::RequestSwitcher::_narrow(obj);
}
