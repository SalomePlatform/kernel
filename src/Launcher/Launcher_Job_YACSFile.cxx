//  Copyright (C) 2009 CEA/DEN, EDF R&D
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
// Author: André RIBES - EDF R&D

#include "Launcher_Job_YACSFile.hxx"


Launcher::Job_YACSFile::Job_YACSFile() {}

Launcher::Job_YACSFile::~Job_YACSFile() {}

void 
Launcher::Job_YACSFile::setJobFile(const std::string & job_file)
{
  Launcher::Job::setJobFile(job_file);
}

void
Launcher::Job_YACSFile::addJobTypeSpecificScript(std::ofstream & launch_script_stream)
{
  launch_script_stream << _resource_definition.AppliPath << "/runSession -p $appli_port driver " << _job_file_name_complete << " > logs/yacs_" << _launch_date << ".log 2>&1" << std::endl;
}
