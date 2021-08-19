// Copyright (C) 2019-2021  CEA/DEN, EDF R&D, OPEN CASCADE
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

#pragma once

#include "SALOME_Launcher_defs.hxx"

#include <SALOMEconfig.h>

#include CORBA_SERVER_HEADER(SALOME_ExternalServerLauncher)

#include <vector>
#include <string>

class SALOME_NamingService_Abstract;
class SALOME_CPythonHelper;

class SALOMELAUNCHER_EXPORT SALOME_ExternalServerLauncher : public POA_SALOME::ExternalServerLauncher
{
 public:
  SALOME_ExternalServerLauncher(const SALOME_CPythonHelper *pyHelper, CORBA::ORB_ptr orb, PortableServer::POA_var poa, SALOME_NamingService_Abstract *ns = nullptr);
  virtual ~SALOME_ExternalServerLauncher();
 public:
  SALOME::ExternalServerHandler_ptr launchServer(const char *server_name, const char *working_dir, const SALOME::CmdList& command_list ) override;
  void registerToKill(const char *server_name, CORBA::Long PID) override;
  void cleanServersInNS() override;
  void shutdownServers() override;
  SALOME::StringVec *listServersInNS() override;
  SALOME::ExternalServerHandler_ptr retrieveServerRefGivenNSEntry( const char *ns_entry ) override;
  char *gethostname() override;
  SALOME::ByteVec *fetchContentOfFileAndRm(const char *file_name) override;
  const SALOME_CPythonHelper *getPyHelper() const { return _pyHelper; }
 private:
  static std::string CreateAbsNameInNSFromServerName(const std::string& scopeName);
  static std::vector<std::string> ListOfExternalServersCpp(SALOME_NamingService_Abstract *ns);
  static bool IsAliveAndKicking(SALOME::ExternalServerHandler_ptr server);
  static bool IsAliveAndKicking(SALOME_NamingService_Abstract *ns, const std::string& serverName);
  static SALOME::ExternalServerHandler_var GetServerHandlerGivenName(SALOME_NamingService_Abstract *ns, const std::string& serverName);
 private:
  const SALOME_CPythonHelper *_pyHelper = nullptr;
  SALOME_NamingService_Abstract *_NS = nullptr;
  PortableServer::POA_var _poa;
  static unsigned CNT;
  std::vector<long> _list_of_pids_to_kill;
 public:
  static const char NAME_IN_NS[];
};
