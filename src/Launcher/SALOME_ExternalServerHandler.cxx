// Copyright (C) 2019-2022  CEA/DEN, EDF R&D, OPEN CASCADE
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

#include "SALOME_ExternalServerHandler.hxx"
#include "SALOME_ExternalServerLauncher.hxx"
#include "SALOME_NamingService.hxx"
#include "SALOME_LauncherException.hxx"
#include "SALOME_ContainerManager.hxx"
#include "SALOME_CPythonHelper.hxx"

#include <sstream>
#include <fstream>
#include <algorithm>

#ifndef WIN32
#include <sys/types.h>
#include <signal.h>
#endif

unsigned SALOME_ExternalServerHandler::CNT = 0;

SALOME_ExternalServerHandler::SALOME_ExternalServerHandler(SALOME_ExternalServerLauncher *boss, const std::string& name, SALOME_NamingService_Abstract *ns, long pid):_name(name),_pid(pid),_NS(ns),_boss(boss)
{
}

SALOME_ExternalServerHandler::~SALOME_ExternalServerHandler()
{
}

void SALOME_ExternalServerHandler::registerToKill(const SALOME_CPythonHelper *pyHelper) const
{
  std::ostringstream oss;
  oss << _name << "_" << CNT++;
  pyHelper->registerToSalomePiDict(oss.str(),_pid);
}

CORBA::Long SALOME_ExternalServerHandler::getPID()
{
  return _pid;
}

char *SALOME_ExternalServerHandler::getName()
{
  return CORBA::string_dup(_name.c_str());
}

void SALOME_ExternalServerHandler::KillPID(long pid)
{
#ifndef WIN32
  kill(pid,SIGTERM);//SIGTERM is emitted not SIGKILL to give _pid process a chance to trap it.
#endif
}

void SALOME_ExternalServerHandler::killMe()
{
  KillPID(_pid);
}

void SALOME_ExternalServerHandler::ping()
{
#ifndef WIN32
  if( kill(_pid,0) != 0 )
    {
      std::ostringstream oss2; oss2 << "SALOME_ExternalServerHandler::ping : Fail to ping server " << _name << "\" with pid = " << _pid << " !";
      throw SALOME_LauncherException(oss2.str());
    }
  // check for a non zombie process
  std::ostringstream statusFile;
  statusFile << "/proc/" << _pid << "/status";
  std::ifstream ifs(statusFile.str());
  if(!ifs.good())
    {
      std::ostringstream oss2; oss2 << "SALOME_ExternalServerHandler::ping : Fail to ping server " << _name << "\" with pid = " << _pid << " during access of status file !";
      throw SALOME_LauncherException(oss2.str());
    }
  constexpr char PAT[]="State:";
  while(ifs.good())
    {
      std::string line;
      std::getline(ifs,line);
      if(line.substr(0,strlen(PAT))==PAT)
        {
          std::string part2(line.substr(strlen(PAT)));
          std::size_t pos(part2.find_first_not_of(" \t"));
          if(pos==std::string::npos)
            return ;
          char state(part2[pos]);
          if(state!='Z')
            return ;
          std::ostringstream oss2; oss2 << "SALOME_ExternalServerHandler::ping : server " << _name << "\" with pid = " << _pid << " has been detected as a Zombie !";
          throw SALOME_LauncherException(oss2.str());
        }
    }
#endif
}

void SALOME_ExternalServerHandler::shutdown()
{
  killMe();
  _boss->cleanServersInNS();
}

SALOME::LongVec *SALOME_ExternalServerHandler::listOfChildrenPID()
{
  SALOME::LongVec *ret(new SALOME::LongVec);
  const SALOME_CPythonHelper *pyh(_boss->getPyHelper());
#ifndef WIN32
  std::ostringstream oss;
  oss << "[int(elt) for elt in sp.check_output([\"ps\",\"-o\",\"pid=\",\"--ppid\",\"" << _pid << "\"]).split()]";
  std::vector<long> pids(pyh->evalVL(oss.str()));
  std::size_t sz(pids.size());
  ret->length(sz);
  for(size_t i=0;i<sz;i++)
    (*ret)[i] = pids[i];
#endif
  return ret;
}
