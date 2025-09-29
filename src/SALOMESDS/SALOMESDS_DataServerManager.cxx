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

#include "SALOMESDS_DataServerManager.hxx"
#include "SALOMESDS_Exception.hxx"

#include "SALOME_ContainerManager.hxx"
#include "SALOME_NamingService.hxx"
#include "SALOME_Embedded_NamingService.hxx"
#include "KernelBasis.hxx"

#include <sstream>
#include <algorithm>

using namespace SALOMESDS;

const char DataServerManager::NAME_IN_NS[]="/DataServerManager";

const char DataServerManager::DFT_SCOPE_NAME_IN_NS[]="Default";

SALOME_CMOD::StringVec *RequestSwitcherDSM::listScopes()
{
  return _dsm->listScopes_unsafe();
}

SALOME_CMOD::DataScopeServerTransaction_ptr RequestSwitcherDSM::giveADataScopeTransactionCalled(const char *scopeName, CORBA::Boolean& isCreated)
{
  return _dsm->giveADataScopeTransactionCalled_unsafe(scopeName,isCreated);
}

void RequestSwitcherDSM::holdRequests()
{
  _dsm->holdRequests();
}

void RequestSwitcherDSM::activeRequests()
{
  _dsm->activeRequests();
}


DataServerManager::DataServerManager(const SALOME_CPythonHelper *pyHelper, CORBA::ORB_ptr orb, PortableServer::POA_ptr poa, SALOME_NamingService_Abstract *ns):_orb(CORBA::ORB::_duplicate(orb))
{
  _ns = ns==nullptr?new SALOME_NamingService(_orb):ns;
  DataScopeServer *dftScope(new DataScopeServer(pyHelper,orb,SALOME_CMOD::DataScopeKiller::_nil(),DFT_SCOPE_NAME_IN_NS,_ns->cloneCoVar()));//_remove_ref will be call by DataScopeServer::shutdownIfNotHostedByDSM
  PortableServer::POAManager_var pman(poa->the_POAManager());
  CORBA::PolicyList policies;
  policies.length(1);
  PortableServer::ThreadPolicy_var threadPol(poa->create_thread_policy(PortableServer::SINGLE_THREAD_MODEL));
  policies[0]=PortableServer::ThreadPolicy::_duplicate(threadPol);
  PortableServer::POA_var safePoa = poa->create_POA("SingleThPOA4SDS",pman,policies);
  _poa = poa;
  threadPol->destroy();
  // activate this to be ready to be usable from NS.
  PortableServer::ObjectId_var id(_poa->activate_object(this));
  CORBA::Object_var obj(_poa->id_to_reference(id));
  SALOME_CMOD::DataServerManager_var obj2(SALOME_CMOD::DataServerManager::_narrow(obj));
  // publish Data server manager in NS
  _ns->Register(obj2,NAME_IN_NS);
  // the default DataScopeServer object is the only one hosted by the current process
  dftScope->setPOA(_poa);
  obj=dftScope->activate();
  SALOME_CMOD::DataScopeServer_var dftScopePtr(SALOME_CMOD::DataScopeServer::_narrow(obj));
  dftScope->registerInNS(dftScopePtr);// agy : Very important ! invoke this method BEFORE activation ! Because this method initializes Python !
}

DataServerManager::~DataServerManager()
{
  //_ns is owned by DataServerManager
  delete _ns;
}

SALOME_CMOD::StringVec * DataServerManager::listScopes_unsafe()
{
  std::vector<std::string> scopes(listOfScopesCpp());
  SALOME_CMOD::StringVec *ret(new SALOME_CMOD::StringVec);
  std::size_t sz(scopes.size());
  ret->length((CORBA::ULong)sz); //!< TODO: size_t to CORBA::ULong
  for(std::size_t i=0;i<sz;i++)
    (*ret)[(CORBA::ULong)i]=CORBA::string_dup(scopes[i].c_str()); //!< TODO: size_t to CORBA::ULong
  return ret;
}

SALOME_CMOD::StringVec *DataServerManager::listScopes()
{
  const std::lock_guard<std::mutex> lock(_mutex);
  return listScopes_unsafe();
}

SALOME_CMOD::StringVec *DataServerManager::listAliveAndKickingScopes()
{
  const std::lock_guard<std::mutex> lock(_mutex);
  std::vector<std::string> scopes(listOfScopesCpp());
  std::size_t sz(scopes.size());
  std::vector<std::string> retCpp; retCpp.reserve(sz);
  for(std::size_t i=0;i<sz;i++)
    {
      if(isAliveAndKicking_unsafe(scopes[i].c_str()))
        retCpp.push_back(scopes[i]);
    }
  //
  SALOME_CMOD::StringVec *ret(new SALOME_CMOD::StringVec);
  sz=retCpp.size();
  ret->length((CORBA::ULong)sz); //!< TODO: size_t to CORBA::ULong
  for(std::size_t i=0;i<sz;i++)
    (*ret)[(CORBA::ULong)i]=CORBA::string_dup(retCpp[i].c_str()); //!< TODO: size_t to CORBA::ULong
  return ret;
}

SALOME_CMOD::DataScopeServer_ptr DataServerManager::getDefaultScope()
{
  const std::lock_guard<std::mutex> lock(_mutex);
  SALOME_CMOD::DataScopeServerBase_var ret(retriveDataScope_unsafe(DFT_SCOPE_NAME_IN_NS));
  if(CORBA::is_nil(ret))
    return SALOME_CMOD::DataScopeServer::_narrow(ret);
  SALOME_CMOD::DataScopeServer_ptr ret2(SALOME_CMOD::DataScopeServer::_narrow(ret));
  if(CORBA::is_nil(ret2))
    throw Exception("DataServerManager::getDefaultScope : exists but has not expected sub type !");
  return ret2;
}

CORBA::Boolean DataServerManager::isAliveAndKicking(const char *scopeName)
{
  const std::lock_guard<std::mutex> lock(_mutex);
  return isAliveAndKicking_unsafe(scopeName);
}

CORBA::Boolean DataServerManager::isAliveAndKicking_unsafe(const char *scopeName)
{
  SALOME_CMOD::DataScopeServerBase_var scopePtr(getScopePtrGivenName(scopeName));
  return IsAliveAndKicking(scopePtr);
}

template<class T>
typename T::PtrType CreateDataScope(CORBA::ORB_ptr orb, const std::string& scopeName, const std::vector<std::string>& scopes, SALOME_NamingService_Abstract& ns)
{
  int isTransactionInt(T::IsTransaction);
  if(std::find(scopes.begin(),scopes.end(),scopeName)!=scopes.end())
    {
      std::ostringstream oss; oss << "DataServerManager::createDataScope : scope name \"" << scopeName << "\" already exists !";
      throw Exception(oss.str());
    }
  //
  std::string fullScopeName(DataServerManager::CreateAbsNameInNSFromScopeName(scopeName));
  std::ostringstream oss; 
  if(!getSSLMode())
  {
    oss << "SALOME_DataScopeServer" << " " << scopeName << " " << isTransactionInt << " ";
    SALOME_ContainerManager::AddOmninamesParams(oss,&ns);
  }
  else
  {
     oss << "SALOME_DataScopeServer_SSL" << " " << scopeName << " " << isTransactionInt << " ";
     Engines::EmbeddedNamingService_var emb = GetEmbeddedNamingService();
     CORBA::String_var ior = orb->object_to_string(emb);
     oss << ior.in();
  }
  std::string command(oss.str());
  SALOME_ContainerManager::MakeTheCommandToBeLaunchedASync(command);
  int status(SALOME_ContainerManager::SystemThreadSafe(command.c_str()));
  if(status!=0)
    {
      std::ostringstream oss2; oss2 << "CreateDataScope : Fail to launch \"" << command << "\" ! Return code was : " << status << " !";
      throw Exception(oss2.str());
    }
  int count(SALOME_ContainerManager::GetTimeOutToLoaunchServer());
  typename T::VarType ret(T::nil());
  while (CORBA::is_nil(ret) && count)
    {
      SALOME_ContainerManager::SleepInSecond(1);
      count--;
      CORBA::Object_var obj(ns.Resolve(fullScopeName.c_str()));
      ret=T::narrow(obj);
    }
  return T::duplicate(ret);
}

template<class T>
typename T::PtrType GiveADataScopeCalled(CORBA::ORB_ptr orb, const std::string& scopeName, const std::vector<std::string>& scopes, SALOME_NamingService_Abstract& ns, CORBA::Boolean& isCreated)
{
  if(std::find(scopes.begin(),scopes.end(),scopeName)==scopes.end())
    {
      isCreated=true;
      return CreateDataScope<T>(orb,scopeName,scopes,ns);
    }
  else
    {
      SALOME_CMOD::DataScopeServerBase_var ret(SALOMESDS::DataServerManager::GetScopePtrGivenName(scopeName,scopes,&ns));
      if(SALOMESDS::DataServerManager::IsAliveAndKicking(ret))
        {
          isCreated=false;
          typename T::PtrType ret2(T::narrow(ret));
          if(CORBA::is_nil(ret))
            return ret2;
          if(CORBA::is_nil(ret2))
            {
              std::ostringstream oss; oss << "DataServerManager::giveADataScopeCalled : scope \"" << scopeName << "\" exists but with invalid type !";
              throw Exception(oss.str());
            }
          return ret2;
        }
      else
        {
          std::string fullScopeName(SALOMESDS::DataServerManager::CreateAbsNameInNSFromScopeName(scopeName));
          ns.Destroy_Name(fullScopeName.c_str());
          isCreated=true;
          return CreateDataScope<T>(orb,scopeName,scopes,ns);
        }
    }
}

class NormalFunctor
{
public:
  typedef SALOME_CMOD::DataScopeServer_ptr PtrType;
  typedef SALOME_CMOD::DataScopeServer_var VarType;
  typedef SALOME_CMOD::DataScopeServer TheType;
  static const bool IsTransaction=false;
  static PtrType nil() { return SALOME_CMOD::DataScopeServer::_nil(); }
  static PtrType narrow(CORBA::Object_ptr obj) { return SALOME_CMOD::DataScopeServer::_narrow(obj); }
  static PtrType duplicate(PtrType obj) { return SALOME_CMOD::DataScopeServer::_duplicate(obj); }
};

class TransactionFunctor
{
public:
  typedef SALOME_CMOD::DataScopeServerTransaction_ptr PtrType;
  typedef SALOME_CMOD::DataScopeServerTransaction_var VarType;
  typedef SALOME_CMOD::DataScopeServerTransaction TheType;
  static const bool IsTransaction=true;
  static PtrType nil() { return SALOME_CMOD::DataScopeServerTransaction::_nil(); }
  static PtrType narrow(CORBA::Object_ptr obj) { return SALOME_CMOD::DataScopeServerTransaction::_narrow(obj); }
  static PtrType duplicate(PtrType obj) { return SALOME_CMOD::DataScopeServerTransaction::_duplicate(obj); }
};

SALOME_CMOD::DataScopeServer_ptr DataServerManager::createDataScope(const char *scopeName)
{
  const std::lock_guard<std::mutex> lock(_mutex);
  return CreateDataScope<NormalFunctor>(_orb,scopeName,listOfScopesCpp(),*_ns);
}

SALOME_CMOD::DataScopeServer_ptr DataServerManager::giveADataScopeCalled(const char *scopeName, CORBA::Boolean& isCreated)
{
  const std::lock_guard<std::mutex> lock(_mutex);
  return GiveADataScopeCalled<NormalFunctor>(_orb,scopeName,listOfScopesCpp(),*_ns,isCreated);
}

SALOME_CMOD::DataScopeServerTransaction_ptr DataServerManager::createDataScopeTransaction(const char *scopeName)
{
  const std::lock_guard<std::mutex> lock(_mutex);
  return CreateDataScope<TransactionFunctor>(_orb,scopeName,listOfScopesCpp(),*_ns);
}

SALOME_CMOD::DataScopeServerTransaction_ptr
DataServerManager::giveADataScopeTransactionCalled_unsafe(
                              const char *scopeName, CORBA::Boolean& isCreated)
{
  return GiveADataScopeCalled<TransactionFunctor>(_orb,scopeName,listOfScopesCpp(),*_ns,isCreated);
}


SALOME_CMOD::DataScopeServerTransaction_ptr DataServerManager::giveADataScopeTransactionCalled(const char *scopeName, CORBA::Boolean& isCreated)
{
  const std::lock_guard<std::mutex> lock(_mutex);
  return giveADataScopeTransactionCalled_unsafe(scopeName, isCreated);
}

SALOME_CMOD::DataScopeServerBase_ptr DataServerManager::retriveDataScope(const char *scopeName)
{
  const std::lock_guard<std::mutex> lock(_mutex);
  return retriveDataScope_unsafe(scopeName);
}

SALOME_CMOD::DataScopeServerBase_ptr DataServerManager::retriveDataScope_unsafe(const char *scopeName)
{
  SALOME_CMOD::DataScopeServerBase_var ret(getScopePtrGivenName(scopeName));
  return SALOME_CMOD::DataScopeServerBase::_duplicate(ret);
}

void DataServerManager::removeDataScope(const char *scopeName)
{
  const std::lock_guard<std::mutex> lock(_mutex);
  SALOME_CMOD::DataScopeServerBase_var scs(getScopePtrGivenName(scopeName));
  SALOME_CMOD::DataScopeKiller_ptr killer;
  if(scs->shutdownIfNotHostedByDSM(killer))
    killer->shutdown();
  CORBA::release(killer);
}

void DataServerManager::cleanScopesInNS()
{
  const std::lock_guard<std::mutex> lock(_mutex);
  std::vector<std::string> scopes(listOfScopesCpp());
  for(std::vector<std::string>::const_iterator it=scopes.begin();it!=scopes.end();it++)
    {
      if(!isAliveAndKicking_unsafe((*it).c_str()))
        {
          std::string fullScopeName(SALOMESDS::DataServerManager::CreateAbsNameInNSFromScopeName(*it));
          _ns->Destroy_Name(fullScopeName.c_str());
        }
    }
}

void DataServerManager::shutdownScopes()
{
  const std::lock_guard<std::mutex> lock(_mutex);
  std::vector<std::string> scopeNames(listOfScopesCpp());
  for(std::vector<std::string>::const_iterator it=scopeNames.begin();it!=scopeNames.end();it++)
    {
      SALOME_CMOD::DataScopeServerBase_var scope(getScopePtrGivenName(*it));
      SALOME_CMOD::DataScopeKiller_ptr killer;
      if(scope->shutdownIfNotHostedByDSM(killer))
        killer->shutdown();
      CORBA::release(killer);
    }
}

std::string DataServerManager::CreateAbsNameInNSFromScopeName(const std::string& scopeName)
{
  std::ostringstream oss; oss << NAME_IN_NS << "/" << scopeName;
  return oss.str();
}

CORBA::Boolean DataServerManager::IsAliveAndKicking(SALOME_CMOD::DataScopeServerBase_ptr scopePtr)
{
  CORBA::Boolean ret(true);
  try
    {
      scopePtr->ping();
    }
  catch(...)
    { ret=false; }
  return ret;
}

std::vector<std::string> DataServerManager::listOfScopesCpp()
{
  _ns->Change_Directory(NAME_IN_NS);
  std::vector<std::string> ret(_ns->list_directory());
  return ret;
}

SALOME_CMOD::DataScopeServerBase_var DataServerManager::GetScopePtrGivenName(const std::string& scopeName, const std::vector<std::string>& scopes, SALOME_NamingService_Abstract *ns)
{
  if(std::find(scopes.begin(),scopes.end(),scopeName)==scopes.end())
    {
      std::ostringstream oss; oss << "DataServerManager::getScopePtrGivenName : scope name \"" << scopeName << "\" does not exist !";
      throw Exception(oss.str());
    }
  std::string fullScopeName(CreateAbsNameInNSFromScopeName(scopeName));
  CORBA::Object_var obj(ns->Resolve(fullScopeName.c_str()));
  SALOME_CMOD::DataScopeServerBase_var ret(SALOME_CMOD::DataScopeServerBase::_narrow(obj));
  return ret;
}

SALOME_CMOD::DataScopeServerBase_var DataServerManager::getScopePtrGivenName(const std::string& scopeName)
{
  return GetScopePtrGivenName(scopeName,listOfScopesCpp(),_ns);
}

SALOME_CMOD::RequestSwitcherDSM_ptr DataServerManager::getRequestSwitcher()
{
  const std::lock_guard<std::mutex> lock(_mutex);
  if(_rs.isNull())
    {
      _rs=new RequestSwitcherDSM(_orb,this);
    }
  CORBA::Object_var obj(_rs->activate());
  return SALOME_CMOD::RequestSwitcherDSM::_narrow(obj);
}

void DataServerManager::holdRequests()
{
  _mutex.lock();
}

void DataServerManager::activeRequests()
{
  _mutex.unlock();
}
