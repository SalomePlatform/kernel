// Copyright (C) 2009-2021  CEA/DEN, EDF R&D, OPEN CASCADE
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
#include "Launcher_Job_PythonSALOME.hxx"
#include <sys/stat.h>

const char Launcher::Job_PythonSALOME::TYPE_NAME[] = "python_salome";

Launcher::Job_PythonSALOME::Job_PythonSALOME()
{
  _job_type = Launcher::Job_PythonSALOME::TYPE_NAME;
}

Launcher::Job_PythonSALOME::~Job_PythonSALOME() {}

void
Launcher::Job_PythonSALOME::setJobFile(const std::string & job_file)
{
  Launcher::Job::setJobFile(job_file);
}

void
Launcher::Job_PythonSALOME::addJobTypeSpecificScript(std::ofstream & launch_script_stream)
{
  struct stat statbuf;
  if(stat(getenv("APPLI"), &statbuf) ==0 &&  S_ISREG(statbuf.st_mode))
      // case where AppliPath hold a salome launcher file
      launch_script_stream << _resource_definition.AppliPath << " shell -p \"$appli_port\" python " << _job_file_name_complete << " > logs/python_" << _launch_date << ".log 2>&1" << std::endl;
  else
      launch_script_stream << _resource_definition.AppliPath << "/salome shell -p \"$appli_port\" python " << _job_file_name_complete << " > logs/python_" << _launch_date << ".log 2>&1" << std::endl;
}
