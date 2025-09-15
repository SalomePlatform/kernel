// Copyright (C) 2019-2025  CEA, EDF, OPEN CASCADE
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

#include <string>

class SALOME_NamingService_Abstract;
class SALOME_ExternalServerLauncher;
class SALOME_CPythonHelper;

class SALOMELAUNCHER_EXPORT SALOME_ExternalServerHandler : public POA_SALOME_CMOD::ExternalServerHandler
{
 public:
  SALOME_ExternalServerHandler(SALOME_ExternalServerLauncher *boss, const std::string& name, SALOME_NamingService_Abstract *ns, long pid);
  virtual ~SALOME_ExternalServerHandler();
  void registerToKill(const SALOME_CPythonHelper *pyHelper) const;
  static void KillPID(long pid);
 public:
  CORBA::Long getPID() override;
  char *getName() override;
  void killMe() override;
  void ping() override;
  void shutdown() override;
  SALOME_CMOD::LongVec *listOfChildrenPID() override;
 private:
  std::string _name;
  long _pid;
  SALOME_NamingService_Abstract *_NS;
  SALOME_ExternalServerLauncher *_boss;
  static unsigned CNT;
};
