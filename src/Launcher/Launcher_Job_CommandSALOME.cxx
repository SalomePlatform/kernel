// Copyright (C) 2009-2024  CEA, EDF, OPEN CASCADE
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

// Author: André RIBES - EDF R&D
//
#include "Launcher_Job_CommandSALOME.hxx"

#ifdef WITH_LIBBATCH
#include <libbatch/Constants.hxx>
#endif

#include <sstream>
#include <sys/stat.h>

const char Launcher::Job_CommandSALOME::TYPE_NAME[] = "command_salome";

Launcher::Job_CommandSALOME::Job_CommandSALOME()
{
  _job_type = Launcher::Job_CommandSALOME::TYPE_NAME;
}

Launcher::Job_CommandSALOME::~Job_CommandSALOME() {}


#ifdef WITH_LIBBATCH

std::string Launcher::Job_CommandSALOME::runCommandString()
{
  std::ostringstream result;
  struct stat statbuf;
  if(stat(getenv("APPLI"), &statbuf) ==0 &&  S_ISREG(statbuf.st_mode))
      // case of a salome launcher file
      result << _resource_definition.AppliPath
             << " shell ./"
             << _job_file_name_complete;
  else
      // case of a salome appli dir
      result << _resource_definition.AppliPath
             << "/salome shell ./"
             << _job_file_name_complete;
  return result.str();
}
#endif
