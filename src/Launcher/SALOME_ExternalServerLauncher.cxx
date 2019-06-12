// Copyright (C) 2019  CEA/DEN, EDF R&D, OPEN CASCADE
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

#include "SALOME_ExternalServerLauncher.hxx"
#include "SALOME_ExternalServerHandler.hxx"
#include "SALOME_NamingService.hxx"
#include "SALOME_LauncherException.hxx"
#include "SALOME_ContainerManager.hxx"
#include "SALOME_CPythonHelper.hxx"

#include CORBA_CLIENT_HEADER(SALOME_ExternalServerLauncher)

#include <sstream>
#include <algorithm>

constexpr char NAME_IN_NS[]="/ExternalServers";

unsigned SALOME_ExternalServerLauncher::CNT = 0;

SALOME_ExternalServerLauncher::SALOME_ExternalServerLauncher(const SALOME_CPythonHelper *pyHelper, CORBA::ORB_ptr orb, PortableServer::POA_var poa):_pyHelper(pyHelper),_poa(poa)
{
  _NS = new SALOME_NamingService(orb);
  PortableServer::ObjectId_var id(_poa->activate_object(this));
  CORBA::Object_var obj(_poa->id_to_reference(id));
  SALOME::ExternalServerLauncher_var refPtr(SALOME::ExternalServerLauncher::_narrow(obj));
  _NS->Register(refPtr,NAME_IN_NS);
}

SALOME_ExternalServerLauncher::~SALOME_ExternalServerLauncher()
{
  delete _NS;
}

SALOME::ExternalServerHandler_ptr SALOME_ExternalServerLauncher::launchServer(const char *server_name, const SALOME::CmdList& command_list )
{
  std::vector<std::string> servers(ListOfExternalServersCpp(_NS));
  if(std::find(servers.begin(),servers.end(),server_name)!=servers.end())
    {
      std::ostringstream oss2; oss2 << "SALOME_ExternalServerLauncher::launchServer : Server \""<< server_name << "\" already exists !";
      throw SALOME_LauncherException(oss2.str());
    }
  std::vector<std::string> cmd(command_list.length());
  const char *toto(command_list[0]);
  for(auto i=0;i<command_list.length();i++)
    cmd[i] = command_list[i];
  long pid(0);
  try
    {
      pid = SALOME_ContainerManager::SystemWithPIDThreadSafe(cmd) ;
    }
  catch(SALOME_Exception& e)
    {
      std::ostringstream oss2; oss2 << "SALOME_ExternalServerLauncher::launchServer : Fail to launch subprocess ! Reason is : " << e.what() << " !";
      throw SALOME_LauncherException(oss2.str());
    }
  SALOME_ExternalServerHandler *retServ(new SALOME_ExternalServerHandler(this,server_name,_NS,pid));
  retServ->registerToKill(_pyHelper);
  PortableServer::ObjectId_var id(_poa->activate_object(retServ));
  CORBA::Object_var obj(_poa->id_to_reference(id));
  std::string fullServerName(CreateAbsNameInNSFromServerName(server_name));
  SALOME::ExternalServerHandler_ptr ret(SALOME::ExternalServerHandler::_narrow(obj));
  _NS->Register(ret,fullServerName.c_str());
  return ret;
}

void SALOME_ExternalServerLauncher::registerToKill(const char *server_name, CORBA::Long PID)
{
  std::ostringstream oss;
  oss << "Custom_"<< server_name << "_" << CNT++;
  _pyHelper->registerToSalomePiDict(oss.str(),PID);
}

void SALOME_ExternalServerLauncher::cleanServersInNS()
{
  std::vector<std::string> servers(ListOfExternalServersCpp(_NS));
  for(std::vector<std::string>::const_iterator it=servers.begin();it!=servers.end();it++)
    {
      if(!IsAliveAndKicking(_NS,(*it).c_str()))
        {
          std::string fullServerName(CreateAbsNameInNSFromServerName(*it));
          _NS->Destroy_Name(fullServerName.c_str());
        }
    }
}

void SALOME_ExternalServerLauncher::shutdownServers()
{
  std::vector<std::string> lioes(ListOfExternalServersCpp(_NS));
  for(auto servName : lioes)
    {
      SALOME::ExternalServerHandler_var proc(GetServerHandlerGivenName(_NS,servName));
      PortableServer::ServantBase *procServ(_poa->reference_to_servant(proc));
      SALOME_ExternalServerHandler *procServC(dynamic_cast<SALOME_ExternalServerHandler *>(procServ));
      try
        {
          procServC->killMe();
        }
      catch(...)
        { }
    }
  cleanServersInNS();
}

SALOME::StringVec *SALOME_ExternalServerLauncher::listServersInNS()
{
  SALOME::StringVec *ret(new SALOME::StringVec);
  std::vector<std::string> loes(ListOfExternalServersCpp(_NS));
  std::size_t sz(loes.size());
  ret->length(sz);
  for(auto i = 0; i<sz ; i++)
    {
      (*ret)[i]=CORBA::string_dup(loes[i].c_str());
    }
  return ret;
}

SALOME::ExternalServerHandler_ptr SALOME_ExternalServerLauncher::retrieveServerRefGivenNSEntry( const char *ns_entry )
{
  SALOME::ExternalServerHandler_var ret(GetServerHandlerGivenName(_NS,ns_entry));
  return ret._retn();
}

char *SALOME_ExternalServerLauncher::gethostname()
{
  std::string ret(_pyHelper->evalS("socket.gethostname()"));
  return CORBA::string_dup(ret.c_str());
}

std::vector<std::string> SALOME_ExternalServerLauncher::ListOfExternalServersCpp(SALOME_NamingService *ns)
{
  ns->Change_Directory(NAME_IN_NS);
  std::vector<std::string> ret(ns->list_directory());
  return ret;
}

std::string SALOME_ExternalServerLauncher::CreateAbsNameInNSFromServerName(const std::string& scopeName)
{
  std::ostringstream oss; oss << NAME_IN_NS << "/" << scopeName;
  return oss.str();
}

bool SALOME_ExternalServerLauncher::IsAliveAndKicking(SALOME::ExternalServerHandler_ptr server)
{
  bool ret(true);
  try
    {
      server->ping();
    }
  catch(...)
    { ret=false; }
  return ret;
}

bool SALOME_ExternalServerLauncher::IsAliveAndKicking(SALOME_NamingService *ns, const std::string& serverName)
{
  SALOME::ExternalServerHandler_var pt(GetServerHandlerGivenName(ns, serverName));
  if( CORBA::is_nil(pt) )
    return false;
  return IsAliveAndKicking(pt);
}

SALOME::ExternalServerHandler_var SALOME_ExternalServerLauncher::GetServerHandlerGivenName(SALOME_NamingService *ns, const std::string& serverName)
{
  std::vector<std::string> serverNames(ListOfExternalServersCpp(ns));
  if(std::find(serverNames.begin(),serverNames.end(),serverName)==serverNames.end())
    {
      std::ostringstream oss; oss << "SALOME_ExternalServerLauncher::GetServerHandlerGivenName : scope name \"" << serverName << "\" does not exist !";
      throw SALOME_LauncherException(oss.str());
    }
  std::string fullServerName(CreateAbsNameInNSFromServerName(serverName));
  CORBA::Object_var obj(ns->Resolve(fullServerName.c_str()));
  SALOME::ExternalServerHandler_var ret(SALOME::ExternalServerHandler::_narrow(obj));
  return ret;
}
