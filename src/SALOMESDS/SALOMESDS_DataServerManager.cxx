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

#include "SALOMESDS_DataServerManager.hxx"
#include "SALOMESDS_Exception.hxx"

#include "SALOME_ContainerManager.hxx"
#include "SALOME_NamingService.hxx"

#include <sstream>
#include <algorithm>

using namespace SALOMESDS;

const char DataServerManager::NAME_IN_NS[]="/DataServerManager";

const char DataServerManager::DFT_SCOPE_NAME_IN_NS[]="Default";

DataServerManager::DataServerManager(int argc, char *argv[], CORBA::ORB_ptr orb, PortableServer::POA_ptr poa):_orb(CORBA::ORB::_duplicate(orb))
{
  DataScopeServer *dftScope(new DataScopeServer(orb,SALOME::DataScopeKiller::_nil(),DFT_SCOPE_NAME_IN_NS));//_remove_ref will be call by DataScopeServer::shutdownIfNotHostedByDSM
  PortableServer::POAManager_var pman(poa->the_POAManager());
  CORBA::PolicyList policies;
  policies.length(1);
  PortableServer::ThreadPolicy_var threadPol(poa->create_thread_policy(PortableServer::SINGLE_THREAD_MODEL));
  policies[0]=PortableServer::ThreadPolicy::_duplicate(threadPol);
  _poa=poa->create_POA("SingleThPOA4SDS",pman,policies);
  threadPol->destroy();
  //
  dftScope->initializePython(argc,argv);// agy : Very important ! invoke this method BEFORE activation !
  // activate this to be ready to be usable from NS.
  PortableServer::ObjectId_var id(_poa->activate_object(this));
  CORBA::Object_var obj(_poa->id_to_reference(id));
  SALOME::DataServerManager_var obj2(SALOME::DataServerManager::_narrow(obj));
  // publish Data server manager in NS
  SALOME_NamingService ns(orb);
  ns.Register(obj2,NAME_IN_NS);
  // the default DataScopeServer object is the only one hosted by the current process
  dftScope->setPOA(_poa);
  obj=dftScope->activate();
  SALOME::DataScopeServer_var dftScopePtr(SALOME::DataScopeServer::_narrow(obj));
  dftScope->registerInNS(dftScopePtr);// agy : Very important ! invoke this method BEFORE activation ! Because this method initializes Python !
}

SALOME::StringVec *DataServerManager::listScopes()
{
  std::vector<std::string> scopes(listOfScopesCpp());
  SALOME::StringVec *ret(new SALOME::StringVec);
  std::size_t sz(scopes.size());
  ret->length(sz);
  for(std::size_t i=0;i<sz;i++)
    (*ret)[i]=CORBA::string_dup(scopes[i].c_str());
  return ret;
}

SALOME::StringVec *DataServerManager::listAliveAndKickingScopes()
{
  std::vector<std::string> scopes(listOfScopesCpp());
  std::size_t sz(scopes.size());
  std::vector<std::string> retCpp; retCpp.reserve(sz);
  for(std::size_t i=0;i<sz;i++)
    {
      if(isAliveAndKicking(scopes[i].c_str()))
        retCpp.push_back(scopes[i]);
    }
  //
  SALOME::StringVec *ret(new SALOME::StringVec);
  sz=retCpp.size();
  ret->length(sz);
  for(std::size_t i=0;i<sz;i++)
    (*ret)[i]=CORBA::string_dup(retCpp[i].c_str());
  return ret;
}

SALOME::DataScopeServer_ptr DataServerManager::getDefaultScope()
{
  SALOME::DataScopeServerBase_var ret(retriveDataScope(DFT_SCOPE_NAME_IN_NS));
  if(CORBA::is_nil(ret))
    return SALOME::DataScopeServer::_narrow(ret);
  SALOME::DataScopeServer_ptr ret2(SALOME::DataScopeServer::_narrow(ret));
  if(CORBA::is_nil(ret2))
    throw Exception("DataServerManager::getDefaultScope : exists but has not expected sub type !");
  return ret2;
}

CORBA::Boolean DataServerManager::isAliveAndKicking(const char *scopeName)
{
  SALOME::DataScopeServerBase_var scopePtr(getScopePtrGivenName(scopeName));
  return IsAliveAndKicking(scopePtr);
}

template<class T>
typename T::PtrType CreateDataScope(const std::string& scopeName, const std::vector<std::string>& scopes, SALOME_NamingService& ns)
{
  int isTransactionInt(T::IsTransaction);
  if(std::find(scopes.begin(),scopes.end(),scopeName)!=scopes.end())
    {
      std::ostringstream oss; oss << "DataServerManager::createDataScope : scope name \"" << scopeName << "\" already exists !";
      throw Exception(oss.str());
    }
  //
  std::string fullScopeName(DataServerManager::CreateAbsNameInNSFromScopeName(scopeName));
  std::ostringstream oss; oss << "SALOME_DataScopeServer" << " " << scopeName << " " << isTransactionInt << " ";
  SALOME_ContainerManager::AddOmninamesParams(oss,&ns);
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
typename T::PtrType GiveADataScopeCalled(const std::string& scopeName, const std::vector<std::string>& scopes, SALOME_NamingService& ns, CORBA::Boolean& isCreated)
{
  if(std::find(scopes.begin(),scopes.end(),scopeName)==scopes.end())
    {
      isCreated=true;
      return CreateDataScope<T>(scopeName,scopes,ns);
    }
  else
    {
      SALOME::DataScopeServerBase_var ret(SALOMESDS::DataServerManager::GetScopePtrGivenName(scopeName,scopes,ns));
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
          return CreateDataScope<T>(scopeName,scopes,ns);
        }
    }
}

class NormalFunctor
{
public:
  typedef SALOME::DataScopeServer_ptr PtrType;
  typedef SALOME::DataScopeServer_var VarType;
  typedef SALOME::DataScopeServer TheType;
  static const bool IsTransaction=false;
  static PtrType nil() { return SALOME::DataScopeServer::_nil(); }
  static PtrType narrow(CORBA::Object_ptr obj) { return SALOME::DataScopeServer::_narrow(obj); }
  static PtrType duplicate(PtrType obj) { return SALOME::DataScopeServer::_duplicate(obj); }
};

class TransactionFunctor
{
public:
  typedef SALOME::DataScopeServerTransaction_ptr PtrType;
  typedef SALOME::DataScopeServerTransaction_var VarType;
  typedef SALOME::DataScopeServerTransaction TheType;
  static const bool IsTransaction=true;
  static PtrType nil() { return SALOME::DataScopeServerTransaction::_nil(); }
  static PtrType narrow(CORBA::Object_ptr obj) { return SALOME::DataScopeServerTransaction::_narrow(obj); }
  static PtrType duplicate(PtrType obj) { return SALOME::DataScopeServerTransaction::_duplicate(obj); }
};

SALOME::DataScopeServer_ptr DataServerManager::createDataScope(const char *scopeName)
{
  SALOME_NamingService ns(_orb);
  return CreateDataScope<NormalFunctor>(scopeName,listOfScopesCpp(),ns);
}

SALOME::DataScopeServer_ptr DataServerManager::giveADataScopeCalled(const char *scopeName, CORBA::Boolean& isCreated)
{
  SALOME_NamingService ns(_orb);
  return GiveADataScopeCalled<NormalFunctor>(scopeName,listOfScopesCpp(),ns,isCreated);
}

SALOME::DataScopeServerTransaction_ptr DataServerManager::createDataScopeTransaction(const char *scopeName)
{
  SALOME_NamingService ns(_orb);
  return CreateDataScope<TransactionFunctor>(scopeName,listOfScopesCpp(),ns);
}

SALOME::DataScopeServerTransaction_ptr DataServerManager::giveADataScopeTransactionCalled(const char *scopeName, CORBA::Boolean& isCreated)
{
  SALOME_NamingService ns(_orb);
  return GiveADataScopeCalled<TransactionFunctor>(scopeName,listOfScopesCpp(),ns,isCreated);
}

SALOME::DataScopeServerBase_ptr DataServerManager::retriveDataScope(const char *scopeName)
{
  SALOME::DataScopeServerBase_var ret(getScopePtrGivenName(scopeName));
  return SALOME::DataScopeServerBase::_duplicate(ret);
}

void DataServerManager::removeDataScope(const char *scopeName)
{
  SALOME::DataScopeServerBase_var scs(getScopePtrGivenName(scopeName));
  SALOME::DataScopeKiller_ptr killer;
  if(scs->shutdownIfNotHostedByDSM(killer))
    killer->shutdown();
  CORBA::release(killer);
}

void DataServerManager::cleanScopesInNS()
{
  SALOME_NamingService ns(_orb);
  std::vector<std::string> scopes(listOfScopesCpp());
  for(std::vector<std::string>::const_iterator it=scopes.begin();it!=scopes.end();it++)
    {
      if(!isAliveAndKicking((*it).c_str()))
        {
          std::string fullScopeName(SALOMESDS::DataServerManager::CreateAbsNameInNSFromScopeName(*it));
          ns.Destroy_Name(fullScopeName.c_str());
        }
    }
}

void DataServerManager::shutdownScopes()
{
  std::vector<std::string> scopeNames(listOfScopesCpp());
  for(std::vector<std::string>::const_iterator it=scopeNames.begin();it!=scopeNames.end();it++)
    {
      SALOME::DataScopeServerBase_var scope(getScopePtrGivenName(*it));
      SALOME::DataScopeKiller_ptr killer;
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

CORBA::Boolean DataServerManager::IsAliveAndKicking(SALOME::DataScopeServerBase_ptr scopePtr)
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
  SALOME_NamingService ns(_orb);
  ns.Change_Directory(NAME_IN_NS);
  std::vector<std::string> ret(ns.list_directory());
  return ret;
}

SALOME::DataScopeServerBase_var DataServerManager::GetScopePtrGivenName(const std::string& scopeName, const std::vector<std::string>& scopes, SALOME_NamingService& ns)
{
  if(std::find(scopes.begin(),scopes.end(),scopeName)==scopes.end())
    {
      std::ostringstream oss; oss << "DataServerManager::getScopePtrGivenName : scope name \"" << scopeName << "\" does not exist !";
      throw Exception(oss.str());
    }
  std::string fullScopeName(CreateAbsNameInNSFromScopeName(scopeName));
  CORBA::Object_var obj(ns.Resolve(fullScopeName.c_str()));
  SALOME::DataScopeServerBase_var ret(SALOME::DataScopeServerBase::_narrow(obj));
  return ret;
}

SALOME::DataScopeServerBase_var DataServerManager::getScopePtrGivenName(const std::string& scopeName)
{
  SALOME_NamingService ns(_orb);
  return GetScopePtrGivenName(scopeName,listOfScopesCpp(),ns);
}
