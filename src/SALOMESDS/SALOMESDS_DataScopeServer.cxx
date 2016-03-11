// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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
#include "SALOME_NamingService.hxx"
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
  Py_Finalize();
  _orb->shutdown(0);
}

DataScopeServerBase::DataScopeServerBase(CORBA::ORB_ptr orb, SALOME::DataScopeKiller_var killer, const std::string& scopeName):_globals(0),_locals(0),_pickler(0),_orb(CORBA::ORB::_duplicate(orb)),_name(scopeName),_killer(killer)
{
}

DataScopeServerBase::DataScopeServerBase(const DataScopeServerBase& other):omniServant(other),ServantBase(other),_globals(0),_locals(0),_pickler(0),_name(other._name),_vars(other._vars),_killer(other._killer)
{
}

DataScopeServerBase::~DataScopeServerBase()
{
  // _globals is borrowed ref -> do nothing
  Py_XDECREF(_locals);
  Py_XDECREF(_pickler);
  for(std::list< std::pair< SALOME::BasicDataServer_var, BasicDataServer * > >::const_iterator it=_vars.begin();it!=_vars.end();it++)
    {
       BasicDataServer *obj((*it).second);
       if(obj)
         {
           obj->decrRef();
         }
    }
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
SALOME::StringVec *DataScopeServerBase::listVars()
{
  SALOME::StringVec *ret(new SALOME::StringVec);
  std::size_t sz(_vars.size());
  ret->length(sz);
  std::list< std::pair< SALOME::BasicDataServer_var, BasicDataServer * > >::iterator it(_vars.begin());
  for(std::size_t i=0;i<sz;it++,i++)
    {
      BasicDataServer *obj((*it).second);
      std::string name(obj->getVarNameCpp());
      (*ret)[i]=CORBA::string_dup(name.c_str());
    }
  return ret;
}

CORBA::Boolean DataScopeServerBase::existVar(const char *varName)
{
  std::list< std::pair< SALOME::BasicDataServer_var, BasicDataServer * > >::const_iterator it(_vars.begin());
  for(;it!=_vars.end();it++)
    if((*it).second->getVarNameCpp()==varName)
      return true;
  return false;
}

SALOME::BasicDataServer_ptr DataScopeServerBase::retrieveVarInternal(const char *varName)
{
  std::list< std::pair< SALOME::BasicDataServer_var, BasicDataServer * > >::const_iterator it0(retrieveVarInternal3(varName));
  return SALOME::BasicDataServer::_duplicate((*it0).first);
}

BasicDataServer *DataScopeServerBase::retrieveVarInternal2(const std::string& varName)
{
  std::list< std::pair< SALOME::BasicDataServer_var, BasicDataServer * > >::const_iterator it0(retrieveVarInternal3(varName));
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
  std::list< std::pair< SALOME::BasicDataServer_var, BasicDataServer * > >::iterator it0(_vars.begin());
  for(std::size_t ii=0;ii<pos;ii++,it0++);
  (*it0).second->decrRef();
  _vars.erase(it0);
}

CORBA::Boolean DataScopeServerBase::shutdownIfNotHostedByDSM(SALOME::DataScopeKiller_out killer)
{
  SALOME_NamingService ns(_orb);
  CORBA::Object_var obj(ns.Resolve(DataServerManager::NAME_IN_NS));
  SALOME::DataServerManager_var dsm(SALOME::DataServerManager::_narrow(obj));
  if(CORBA::is_nil(dsm))
    throw Exception("Unable to reach in the NS the unique DataServerManager instance of the Session !");
  // destroy ref in the naming service
  std::string fullScopeName(SALOMESDS::DataServerManager::CreateAbsNameInNSFromScopeName(_name));
  ns.Destroy_Name(fullScopeName.c_str());
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
      killer=SALOME::DataScopeKiller::_duplicate(_killer);
      return true;
    }
  else
    {
      ret->_remove_ref();
      enforcedRelease();
      killer=SALOME::DataScopeKiller::_duplicate(_killer);
      return false;
    }
}

SALOME::ByteVec *DataScopeServerBase::fetchSerializedContent(const char *varName)
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

SALOME::SeqOfByteVec *DataScopeServerBase::getAllKeysOfVarWithTypeDict(const char *varName)
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
  SALOME::SeqOfByteVec *ret(new SALOME::SeqOfByteVec);
  ret->length(sz);
  for(Py_ssize_t i=0;i<sz;i++)
    {
      PyObject *item(PyList_GetItem(keys,i));
      Py_XINCREF(item);
      std::string pickel(varc->pickelize(item));//item consumed
      PickelizedPyObjServer::FromCppToByteSeq(pickel,(*ret)[i]);
    }
  Py_XDECREF(keys);
  return ret;
}

void DataScopeServerBase::initializePython(int argc, char *argv[])
{
  Py_Initialize();
  PyEval_InitThreads();
  PySys_SetArgv(argc,argv);
  PyObject *mainmod(PyImport_AddModule("__main__"));
  _globals=PyModule_GetDict(mainmod);
  if(PyDict_GetItemString(_globals, "__builtins__") == NULL)
    {
      PyObject *bimod(PyImport_ImportModule("__builtin__"));
      if (bimod == NULL || PyDict_SetItemString(_globals, "__builtins__", bimod) != 0)
        Py_FatalError("can't add __builtins__ to __main__");
      Py_XDECREF(bimod);
    }
  _locals=PyDict_New();
  PyObject *tmp(PyList_New(0));
  _pickler=PyImport_ImportModuleLevel(const_cast<char *>("cPickle"),_globals,_locals,tmp,-1);
}

void DataScopeServerBase::registerToSalomePiDict() const
{
  PyObject *mod(PyImport_ImportModule("addToKillList"));//new value
  if(!mod)
    return;
  PyObject *meth(PyObject_GetAttrString(mod,"addToKillList"));//new value
  if(!meth)
    { Py_XDECREF(mod); return ; }
  PyObject *args(PyTuple_New(2));
  PyTuple_SetItem(args,0,PyInt_FromLong(getpid()));
  PyTuple_SetItem(args,1,PyString_FromString("SALOME_DataScopeServerBase"));
  PyObject *res(PyObject_CallObject(meth,args));
  Py_XDECREF(args);
  Py_XDECREF(res);
  Py_XDECREF(meth);
  Py_XDECREF(mod);
}

void DataScopeServerBase::setPOA(PortableServer::POA_var poa)
{
  _poa=poa;
}

void DataScopeServerBase::registerInNS(SALOME::DataScopeServerBase_ptr ptr)
{
  std::string fullScopeName(SALOMESDS::DataServerManager::CreateAbsNameInNSFromScopeName(_name));
  SALOME_NamingService ns(_orb);
  ns.Register(ptr,fullScopeName.c_str());
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
  std::list< std::pair< SALOME::BasicDataServer_var, BasicDataServer * > >::const_iterator it(_vars.begin());
  for(std::size_t i=0;i<sz;it++,i++)
    ret[i]=(*it).second->getVarNameCpp();
  return ret;
}

void DataScopeServerBase::checkNotAlreadyExistingVar(const std::string& varName) const
{
  std::vector<std::string> allNames(getAllVarNames());
  std::vector<std::string>::iterator it(std::find(allNames.begin(),allNames.end(),varName));
  if(it!=allNames.end())
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
  std::list< std::pair< SALOME::BasicDataServer_var, BasicDataServer * > >::iterator it(retrieveVarInternal4(varName));
  std::pair< SALOME::BasicDataServer_var, BasicDataServer * >& p(*it);
  PickelizedPyObjRdWrServer *varc(dynamic_cast<PickelizedPyObjRdWrServer *>(p.second));
  if(!varc)
    throw Exception("DataScopeServerBase::moveStatusOfVarFromRdWrToRdOnly : var is not a RdWr !");
  PyObject *pyobj(varc->getPyObj()); Py_XINCREF(pyobj);
  PickelizedPyObjRdOnlyServer *newVar(new PickelizedPyObjRdOnlyServer(this,varName,pyobj));
  CORBA::Object_var obj(newVar->activate());
  SALOME::BasicDataServer_var obj2(SALOME::BasicDataServer::_narrow(obj));
  p.first=obj2; p.second=newVar;
  varc->decrRef();
}

void DataScopeServerBase::moveStatusOfVarFromRdOnlyToRdWr(const std::string& varName)
{
  std::list< std::pair< SALOME::BasicDataServer_var, BasicDataServer * > >::iterator it(retrieveVarInternal4(varName));
  std::pair< SALOME::BasicDataServer_var, BasicDataServer * >& p(*it);
  PickelizedPyObjRdOnlyServer *varc(dynamic_cast<PickelizedPyObjRdOnlyServer *>(p.second));
  if(!varc)
    throw Exception("DataScopeServerBase::moveStatusOfVarFromRdOnlyToRdWr : var is not a RdWr !");
  PyObject *pyobj(varc->getPyObj()); Py_XINCREF(pyobj);
  PickelizedPyObjRdWrServer *newVar(new PickelizedPyObjRdWrServer(this,varName,pyobj));
  CORBA::Object_var obj(newVar->activate());
  SALOME::BasicDataServer_var obj2(SALOME::BasicDataServer::_narrow(obj));
  p.first=obj2; p.second=newVar;
  varc->decrRef();
}

void DataScopeServerBase::moveStatusOfVarFromRdExtOrRdExtInitToRdExtInit(const std::string& varName)
{
  std::list< std::pair< SALOME::BasicDataServer_var, BasicDataServer * > >::iterator it(retrieveVarInternal4(varName));
  std::pair< SALOME::BasicDataServer_var, BasicDataServer * >& p(*it);
  PickelizedPyObjRdExtServer *varc0(dynamic_cast<PickelizedPyObjRdExtServer *>(p.second));
  PickelizedPyObjRdExtInitServer *varc1(dynamic_cast<PickelizedPyObjRdExtInitServer *>(p.second));
  if(!varc0 && !varc1)
    throw Exception("DataScopeServerBase::moveStatusOfVarFromRdExtOrRdExtInitToRdExtInit : var is neither RdExt nor RdExtInit !");
  if(varc0)
    {
      PyObject *pyobj(varc0->getPyObj()); Py_XINCREF(pyobj);
      PickelizedPyObjRdExtInitServer *newVar(new PickelizedPyObjRdExtInitServer(this,varName,pyobj));
      newVar->incrNbClients();
      CORBA::Object_var obj(newVar->activate());
      SALOME::BasicDataServer_var obj2(SALOME::BasicDataServer::_narrow(obj));
      p.first=obj2; p.second=newVar;
      varc0->decrRef();
    }
  else
    varc1->incrNbClients();
}

void DataScopeServerBase::moveStatusOfVarFromRdExtOrRdExtInitToRdExt(const std::string& varName)
{
  std::list< std::pair< SALOME::BasicDataServer_var, BasicDataServer * > >::iterator it(retrieveVarInternal4(varName));
  std::pair< SALOME::BasicDataServer_var, BasicDataServer * >& p(*it);
  PickelizedPyObjRdExtInitServer *varc0(dynamic_cast<PickelizedPyObjRdExtInitServer *>(p.second));
  PickelizedPyObjRdExtServer *varc1(dynamic_cast<PickelizedPyObjRdExtServer *>(p.second));
  if(!varc0 && !varc1)
    throw Exception("DataScopeServerBase::moveStatusOfVarFromRdExtOrRdExtInitToRdExt : var is not a RdExtInit !");
  if(varc0)
    {
      if(varc0->decrNbClients())
        {
          PyObject *pyobj(varc0->getPyObj()); Py_XINCREF(pyobj);
          PickelizedPyObjRdExtServer *newVar(new PickelizedPyObjRdExtServer(this,varName,pyobj));
          CORBA::Object_var obj(newVar->activate());
          SALOME::BasicDataServer_var obj2(SALOME::BasicDataServer::_narrow(obj));
          p.first=obj2; p.second=newVar;
          varc0->decrRef();
        }
    }
}

std::list< std::pair< SALOME::BasicDataServer_var, BasicDataServer * > >::const_iterator DataScopeServerBase::retrieveVarInternal3(const std::string& varName) const
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
  std::list< std::pair< SALOME::BasicDataServer_var, BasicDataServer * > >::const_iterator it0(_vars.begin());
  for(std::size_t i=0;i<pos;i++,it0++);
  return it0;
}

std::list< std::pair< SALOME::BasicDataServer_var, BasicDataServer * > >::iterator DataScopeServerBase::retrieveVarInternal4(const std::string& varName)
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
  std::list< std::pair< SALOME::BasicDataServer_var, BasicDataServer * > >::iterator it0(_vars.begin());
  for(std::size_t i=0;i<pos;i++,it0++);
  return it0;
}

///////

DataScopeServer::DataScopeServer(CORBA::ORB_ptr orb, SALOME::DataScopeKiller_var killer, const std::string& scopeName):DataScopeServerBase(orb,killer,scopeName)
{
}

DataScopeServer::DataScopeServer(const DataScopeServer& other):omniServant(other),ServantBase(other),DataScopeServerBase(other)
{
}

SALOME::PickelizedPyObjRdOnlyServer_ptr DataScopeServer::createRdOnlyVar(const char *varName, const SALOME::ByteVec& constValue)
{
  std::string varNameCpp(varName);
  checkNotAlreadyExistingVar(varNameCpp);
  PickelizedPyObjRdOnlyServer *tmp(new PickelizedPyObjRdOnlyServer(this,varNameCpp,constValue));
  CORBA::Object_var ret(tmp->activate());
  std::pair< SALOME::BasicDataServer_var, BasicDataServer * > p(SALOME::BasicDataServer::_narrow(ret),tmp);
  _vars.push_back(p);
  return SALOME::PickelizedPyObjRdOnlyServer::_narrow(ret);
}

SALOME::PickelizedPyObjRdExtServer_ptr DataScopeServer::createRdExtVar(const char *varName, const SALOME::ByteVec& constValue)
{
  std::string varNameCpp(varName);
  checkNotAlreadyExistingVar(varNameCpp);
  PickelizedPyObjRdExtServer *tmp(new PickelizedPyObjRdExtServer(this,varNameCpp,constValue));
  CORBA::Object_var ret(tmp->activate());
  std::pair< SALOME::BasicDataServer_var, BasicDataServer * > p(SALOME::BasicDataServer::_narrow(ret),tmp);
  _vars.push_back(p);
  return SALOME::PickelizedPyObjRdExtServer::_narrow(ret);
}

SALOME::PickelizedPyObjRdWrServer_ptr DataScopeServer::createRdWrVar(const char *typeName, const char *varName)
{
  std::string varNameCpp(varName),typeNameCpp(typeName);
  checkNotAlreadyExistingVar(varNameCpp);
  PickelizedPyObjRdWrServer *tmp(new PickelizedPyObjRdWrServer(this,typeNameCpp,varNameCpp));
  CORBA::Object_var ret(tmp->activate());
  std::pair< SALOME::BasicDataServer_var, BasicDataServer * > p(SALOME::BasicDataServer::_narrow(ret),tmp);
  _vars.push_back(p);
  return SALOME::PickelizedPyObjRdWrServer::_narrow(ret);
}

DataScopeServer::~DataScopeServer()
{
}

////////

DataScopeServerTransaction::DataScopeServerTransaction(CORBA::ORB_ptr orb, SALOME::DataScopeKiller_var killer, const std::string& scopeName):DataScopeServerBase(orb,killer,scopeName)
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
void DataScopeServerTransaction::fetchAndGetAccessOfVar(const char *varName, CORBA::String_out access, SALOME::ByteVec_out data)
{
  access=getAccessOfVar(varName);
  data=fetchSerializedContent(varName);
}

void DataScopeServerTransaction::createRdOnlyVarInternal(const std::string& varName, const SALOME::ByteVec& constValue)
{
  checkNotAlreadyExistingVar(varName);
  PickelizedPyObjRdOnlyServer *tmp(new PickelizedPyObjRdOnlyServer(this,varName,constValue));
  CORBA::Object_var ret(tmp->activate());
  std::pair< SALOME::BasicDataServer_var, BasicDataServer * > p(SALOME::BasicDataServer::_narrow(ret),tmp);
  _vars.push_back(p);
}

void DataScopeServerTransaction::createRdExtVarInternal(const std::string& varName, const SALOME::ByteVec& constValue)
{
  checkNotAlreadyExistingVar(varName);
  PickelizedPyObjRdExtServer *tmp(new PickelizedPyObjRdExtServer(this,varName,constValue));
  CORBA::Object_var ret(tmp->activate());
  std::pair< SALOME::BasicDataServer_var, BasicDataServer * > p(SALOME::BasicDataServer::_narrow(ret),tmp);
  _vars.push_back(p);
}

void DataScopeServerTransaction::createRdExtInitVarInternal(const std::string& varName, const SALOME::ByteVec& constValue)
{
  checkNotAlreadyExistingVar(varName);
  PickelizedPyObjRdExtInitServer *tmp(new PickelizedPyObjRdExtInitServer(this,varName,constValue));
  CORBA::Object_var ret(tmp->activate());
  std::pair< SALOME::BasicDataServer_var, BasicDataServer * > p(SALOME::BasicDataServer::_narrow(ret),tmp);
  _vars.push_back(p);
}

void DataScopeServerTransaction::createRdWrVarInternal(const std::string& varName, const SALOME::ByteVec& constValue)
{
  checkNotAlreadyExistingVar(varName);
  PickelizedPyObjRdWrServer *tmp(new PickelizedPyObjRdWrServer(this,varName,constValue));
  CORBA::Object_var ret(tmp->activate());
  std::pair< SALOME::BasicDataServer_var, BasicDataServer * > p(SALOME::BasicDataServer::_narrow(ret),tmp);
  _vars.push_back(p);
}

SALOME::Transaction_ptr DataScopeServerTransaction::createRdOnlyVarTransac(const char *varName, const SALOME::ByteVec& constValue)
{
  checkNotAlreadyExistingVar(varName);
  TransactionRdOnlyVarCreate *ret(new TransactionRdOnlyVarCreate(this,varName,constValue));
  CORBA::Object_var obj(ret->activate());
  return SALOME::Transaction::_narrow(obj);
}

SALOME::Transaction_ptr DataScopeServerTransaction::createRdExtVarTransac(const char *varName, const SALOME::ByteVec& constValue)
{
  checkNotAlreadyExistingVar(varName);
  TransactionRdExtVarCreate *ret(new TransactionRdExtVarCreate(this,varName,constValue));
  CORBA::Object_var obj(ret->activate());
  return SALOME::Transaction::_narrow(obj);
}

SALOME::Transaction_ptr DataScopeServerTransaction::createRdExtInitVarTransac(const char *varName, const SALOME::ByteVec& constValue)
{
  checkNotAlreadyExistingVar(varName);
  TransactionRdExtInitVarCreate *ret(new TransactionRdExtInitVarCreate(this,varName,constValue));
  CORBA::Object_var obj(ret->activate());
  return SALOME::Transaction::_narrow(obj);
}

SALOME::Transaction_ptr DataScopeServerTransaction::createRdWrVarTransac(const char *varName, const SALOME::ByteVec& constValue)
{
  checkNotAlreadyExistingVar(varName);
  TransactionRdWrVarCreate *ret(new TransactionRdWrVarCreate(this,varName,constValue));
  CORBA::Object_var obj(ret->activate());
  return SALOME::Transaction::_narrow(obj);
}

void DataScopeServerTransaction::addWaitKey(KeyWaiter *kw)
{
  if(!kw)
    throw Exception("DataScopeServerTransaction::addWaitKey : NULL input object !");
  _waiting_keys.push_back(kw);
}

void DataScopeServerTransaction::pingKey(PyObject *keyObj)
{
  PyObject *cmpObj(getPyCmpFunc());
  if(!keyObj)
    throw Exception("ataScopeServerTransaction::pingKey : Key Object is NULL !");
  PyObject *args(PyTuple_New(2));
  PyTuple_SetItem(args,0,keyObj); Py_XINCREF(keyObj);
  std::size_t ii(0);
  // this part does nothing except to be sure that in notify key all will be OK.
  for(std::list< KeyWaiter *>::iterator it=_waiting_keys.begin();it!=_waiting_keys.end();it++,ii++)
    {
      PyObject *waitKey((*it)->getKeyPyObj());
      PyTuple_SetItem(args,1,waitKey); Py_XINCREF(waitKey);
      PyObject *res(PyObject_CallObject(cmpObj,args));
      if(res==NULL)
        {
          std::ostringstream oss; oss << "DataScopeServerTransaction::pingKey : for object id #" << ii << " error during cmp(k,wk[i]) !";
          throw Exception(oss.str());
        }
      PyInt_AsLong(res);
      if(PyErr_Occurred())
        {
          std::ostringstream oss; oss << "DataScopeServerTransaction::pingKey : for object id #" << ii << " error during interpretation of cmp(k,wk[i]) !";
          throw Exception(oss.str());
        }
      Py_XDECREF(res);
    }
}

void DataScopeServerTransaction::notifyKey(const std::string& varName, PyObject *keyObj, PyObject *valueObj)
{
  PyObject *cmpObj(getPyCmpFunc());
  if(!keyObj)
    throw Exception("DataScopeServerTransaction::notifyKey : MAIN INTERNAL ERROR ! Key Object is NULL !");
  PyObject *args(PyTuple_New(2));
  PyTuple_SetItem(args,0,keyObj); Py_XINCREF(keyObj);
  std::size_t ii(0);
  std::list< KeyWaiter *> newList,listOfEltToWakeUp;
  for(std::list< KeyWaiter *>::iterator it=_waiting_keys.begin();it!=_waiting_keys.end();it++,ii++)
    {
      if((*it)->getVarName()!=varName)
        {
          newList.push_back(*it);
          continue;
        }
      PyObject *waitKey((*it)->getKeyPyObj());
      PyTuple_SetItem(args,1,waitKey); Py_XINCREF(waitKey);
      PyObject *res(PyObject_CallObject(cmpObj,args));
      if(res==NULL)
        {
          std::ostringstream oss; oss << "DataScopeServerTransaction::notifyKey : MAIN INTERNAL ERROR ! for object id #" << ii << " error during cmp(k,wk[i]) !";
          throw Exception(oss.str());
        }
      long resCpp(PyInt_AsLong(res));
      if(PyErr_Occurred())
        {
          std::ostringstream oss; oss << "DataScopeServerTransaction::notifyKey : MAIN INTERNAL ERROR ! for object id #" << ii << " error during interpretation of cmp(k,wk[i]) !";
          throw Exception(oss.str());
        }
      Py_XDECREF(res);
      if(resCpp==0)
        listOfEltToWakeUp.push_back(*it);
      else
        newList.push_back(*it);
    }
  for(std::list< KeyWaiter *>::iterator it=listOfEltToWakeUp.begin();it!=listOfEltToWakeUp.end();it++)
    (*it)->valueJustCome(valueObj);
  for(std::list< KeyWaiter *>::iterator it=listOfEltToWakeUp.begin();it!=listOfEltToWakeUp.end();it++)
    (*it)->go();
  _waiting_keys=newList;
}

SALOME::Transaction_ptr DataScopeServerTransaction::addKeyValueInVarHard(const char *varName, const SALOME::ByteVec& key, const SALOME::ByteVec& value)
{
  checkVarExistingAndDict(varName);
  TransactionAddKeyValueHard *ret(new TransactionAddKeyValueHard(this,varName,key,value));
  CORBA::Object_var obj(ret->activate());
  return SALOME::Transaction::_narrow(obj);
}

SALOME::Transaction_ptr DataScopeServerTransaction::addKeyValueInVarErrorIfAlreadyExisting(const char *varName, const SALOME::ByteVec& key, const SALOME::ByteVec& value)
{
  checkVarExistingAndDict(varName);
  TransactionAddKeyValueErrorIfAlreadyExisting *ret(new TransactionAddKeyValueErrorIfAlreadyExisting(this,varName,key,value));
  CORBA::Object_var obj(ret->activate());
  return SALOME::Transaction::_narrow(obj);
}

SALOME::TransactionMultiKeyAddSession_ptr DataScopeServerTransaction::addMultiKeyValueSession(const char *varName)
{
  checkVarExistingAndDict(varName);
  TransactionMultiKeyAddSession *ret(new TransactionMultiKeyAddSession(this,varName));
  CORBA::Object_var obj(ret->activate());
  return SALOME::TransactionMultiKeyAddSession::_narrow(obj);
}

SALOME::Transaction_ptr DataScopeServerTransaction::removeKeyInVarErrorIfNotAlreadyExisting(const char *varName, const SALOME::ByteVec& key)
{
  checkVarExistingAndDict(varName);
  TransactionRemoveKeyInVarErrorIfNotAlreadyExisting *ret(new TransactionRemoveKeyInVarErrorIfNotAlreadyExisting(this,varName,key));
  CORBA::Object_var obj(ret->activate());
  return SALOME::Transaction::_narrow(obj);
}

SALOME::TransactionRdWrAccess_ptr DataScopeServerTransaction::createWorkingVarTransac(const char *varName, const SALOME::ByteVec& constValue)
{
  std::string varNameCpp(varName);
  checkNotAlreadyExistingVar(varName);
  PickelizedPyObjRdWrServer *tmp(new PickelizedPyObjRdWrServer(this,varNameCpp,constValue));
  CORBA::Object_var obj(tmp->activate());
  std::pair< SALOME::BasicDataServer_var, BasicDataServer * > p(SALOME::BasicDataServer::_narrow(obj),tmp);
  _vars.push_back(p);
  //
  TransactionMorphRdWrIntoRdOnly *ret(new TransactionMorphRdWrIntoRdOnly(this,varName));
  CORBA::Object_var obj2(ret->activate());
  return SALOME::TransactionRdWrAccess::_narrow(obj2);
}

SALOME::Transaction_ptr DataScopeServerTransaction::killVarTransac(const char *varName)
{
  std::string varNameCpp(varName);
  checkExistingVar(varNameCpp);
  //
  TransactionKillVar *ret(new TransactionKillVar(this,varName));
  CORBA::Object_var obj2(ret->activate());
  return SALOME::Transaction::_narrow(obj2);
}

SALOME::KeyWaiter_ptr DataScopeServerTransaction::waitForKeyInVar(const char *varName, const SALOME::ByteVec& keyVal)
{
  PickelizedPyObjServer *pickelObj(checkVarExistingAndDict(varName));
  KeyWaiter *ret(new KeyWaiter(pickelObj,keyVal));
  CORBA::Object_var obj(ret->activate());//KeyWaiter instance activated inside a multithread POA contrary to all of objects in SALOMESDS in single thread !
  return SALOME::KeyWaiter::_narrow(obj);
}

SALOME::KeyWaiter_ptr DataScopeServerTransaction::waitForKeyInVarAndKillIt(const char *varName, const SALOME::ByteVec& keyVal, SALOME::Transaction_out transac)
{
  PickelizedPyObjServer *pickelObj(checkVarExistingAndDict(varName));
  KeyWaiter *ret0(new KeyWaiter(pickelObj,keyVal));
  CORBA::Object_var obj(ret0->activate());//KeyWaiter instance activated inside a multithread POA contrary to all of objects in SALOMESDS in single thread !
  //
  TransactionRemoveKeyInVarErrorIfNotAlreadyExisting *ret1(new TransactionRemoveKeyInVarErrorIfNotAlreadyExisting(this,varName,keyVal));
  CORBA::Object_var obj2(ret1->activate());
  transac=SALOME::Transaction::_narrow(obj2);
  //
  return SALOME::KeyWaiter::_narrow(obj);
}

SALOME::ByteVec *DataScopeServerTransaction::waitForMonoThrRev(SALOME::KeyWaiter_ptr kw)
{
  PortableServer::ServantBase *ret(0);
  try
    {
      ret=_poa_for_key_waiter->reference_to_servant(kw);
    }
  catch(...) { ret=0; }
  KeyWaiter *retc(dynamic_cast<KeyWaiter *>(ret));
  if(!retc)
    throw Exception("DataScopeServerTransaction::invokeMonoThr : internal error 1 !");
  retc->_remove_ref();
  return retc->waitForMonoThr();
}

void DataScopeServerTransaction::atomicApply(const SALOME::ListOfTransaction& transactions)
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
          eltBase=_poa->reference_to_servant(transactions[i]);
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

/*!
 * Returns borrowed reference.
 */
PyObject *DataScopeServerTransaction::getPyCmpFunc()
{
  PyObject *builtins(PyDict_GetItemString(_globals,"__builtins__"));//borrowed
  if(builtins==NULL)
    throw Exception("Fail to find reference to builtins !");
  PyObject *builtins2(PyModule_GetDict(builtins));//borrowed
  if(builtins2==NULL)
    throw Exception("Fail to invoke __dict__ on builtins !");
  PyObject *cmpObj(PyDict_GetItemString(builtins2,"cmp"));
  if(cmpObj==NULL)
    throw Exception("Fail to find cmp in __builtins__ !");
  return cmpObj;
}

DataScopeServerTransaction::~DataScopeServerTransaction()
{
}

