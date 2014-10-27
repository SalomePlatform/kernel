// Copyright (C) 2007-2014  CEA/DEN, EDF R&D, OPEN CASCADE
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
  DataScopeServer *dftScope(new DataScopeServer(orb,DFT_SCOPE_NAME_IN_NS));//_remove_ref will be call by DataScopeServer::shutdownIfNotHostedByDSM
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
  id=_poa->activate_object(dftScope);
  obj=_poa->id_to_reference(id);
  SALOME::DataScopeServer_var dftScopePtr(SALOME::DataScopeServer::_narrow(obj));
  dftScope->setPOAAndRegister(_poa,dftScopePtr);// agy : Very important ! invoke this method BEFORE activation ! Because this method initializes Python !
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
  return retriveDataScope(DFT_SCOPE_NAME_IN_NS);
}

CORBA::Boolean DataServerManager::isAliveAndKicking(const char *scopeName)
{
  SALOME::DataScopeServer_var scopePtr(getScopePtrGivenName(scopeName));
  CORBA::Boolean ret(true);
  try
    {
      scopePtr->ping();
    }
  catch(...)
    { ret=false; }
  return ret;
}

SALOME::DataScopeServer_ptr DataServerManager::createDataScope(const char *scopeName)
{
  std::string scopeNameCpp(scopeName);
  std::vector<std::string> scopes(listOfScopesCpp());
  if(std::find(scopes.begin(),scopes.end(),scopeNameCpp)!=scopes.end())
    {
      std::ostringstream oss; oss << "DataServerManager::createDataScope : scope name \"" << scopeName << "\" already exists !";
      throw Exception(oss.str());
    }
  //
  SALOME_NamingService ns(_orb);
  std::string fullScopeName(CreateAbsNameInNSFromScopeName(scopeName));
  std::ostringstream oss; oss << "SALOME_DataScopeServer" << " " << scopeName << " ";
  SALOME_ContainerManager::AddOmninamesParams(oss,&ns);
  std::string command(oss.str());
  SALOME_ContainerManager::MakeTheCommandToBeLaunchedASync(command);
  int status(SALOME_ContainerManager::SystemThreadSafe(command.c_str()));
  int count(SALOME_ContainerManager::GetTimeOutToLoaunchServer());
  SALOME::DataScopeServer_var ret(SALOME::DataScopeServer::_nil());
  while (CORBA::is_nil(ret) && count)
    {
      SALOME_ContainerManager::SleepInSecond(1);
      count--;
      CORBA::Object_var obj(ns.Resolve(fullScopeName.c_str()));
      ret=SALOME::DataScopeServer::_narrow(obj);
    }
  return SALOME::DataScopeServer::_duplicate(ret);
}

SALOME::DataScopeServer_ptr DataServerManager::giveADataScopeCalled(const char *scopeName, CORBA::Boolean& isCreated)
{
  std::string scopeNameCpp(scopeName);
  std::vector<std::string> scopes(listOfScopesCpp());
  if(std::find(scopes.begin(),scopes.end(),scopeNameCpp)==scopes.end())
    {
      isCreated=true;
      return createDataScope(scopeName);
    }
  else
    {
      if(isAliveAndKicking(scopeName))
        {
          isCreated=false;
          return retriveDataScope(scopeName);
        }
      else
        {
          SALOME_NamingService ns(_orb);
          std::string fullScopeName(SALOMESDS::DataServerManager::CreateAbsNameInNSFromScopeName(scopeNameCpp));
          ns.Destroy_Name(fullScopeName.c_str());
          isCreated=true;
          return createDataScope(scopeName);
        }
    }
}

SALOME::DataScopeServer_ptr DataServerManager::retriveDataScope(const char *scopeName)
{
  SALOME::DataScopeServer_var ret(getScopePtrGivenName(scopeName));
  return SALOME::DataScopeServer::_duplicate(ret);
}

void DataServerManager::removeDataScope(const char *scopeName)
{
  SALOME::DataScopeServer_var scs(getScopePtrGivenName(scopeName));
  scs->shutdownIfNotHostedByDSM();
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
    getScopePtrGivenName(*it)->shutdownIfNotHostedByDSM();
}

std::string DataServerManager::CreateAbsNameInNSFromScopeName(const std::string& scopeName)
{
  std::ostringstream oss; oss << NAME_IN_NS << "/" << scopeName;
  return oss.str();
}

std::vector<std::string> DataServerManager::listOfScopesCpp()
{
  SALOME_NamingService ns(_orb);
  ns.Change_Directory(NAME_IN_NS);
  std::vector<std::string> ret(ns.list_directory());
  return ret;
}

SALOME::DataScopeServer_var DataServerManager::getScopePtrGivenName(const std::string& scopeName)
{
  std::vector<std::string> scopes(listOfScopesCpp());
  std::size_t sz(scopes.size());
  if(std::find(scopes.begin(),scopes.end(),scopeName)==scopes.end())
    {
      std::ostringstream oss; oss << "DataServerManager::getScopePtrGivenName : scope name \"" << scopeName << "\" does not exist !";
      throw Exception(oss.str());
    }
  SALOME_NamingService ns(_orb);
  std::string fullScopeName(CreateAbsNameInNSFromScopeName(scopeName));
  CORBA::Object_var obj(ns.Resolve(fullScopeName.c_str()));
  SALOME::DataScopeServer_var ret(SALOME::DataScopeServer::_narrow(obj));
  return ret;
}
