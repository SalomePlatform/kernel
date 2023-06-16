// Copyright (C) 2009-2023  CEA/DEN, EDF R&D, OPEN CASCADE
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
#include "Launcher_Job_Command.hxx"
#include "Basics_DirUtils.hxx"

#ifdef WITH_LIBBATCH
#include <libbatch/Constants.hxx>
#endif

#ifdef WIN32
#include <io.h>
#define _chmod chmod
#include <process.h>
#endif

#include <sstream>

const char Launcher::Job_Command::TYPE_NAME[] = "command";

Launcher::Job_Command::Job_Command()
{
  _job_type = Launcher::Job_Command::TYPE_NAME;
}

Launcher::Job_Command::~Job_Command() {}

void
Launcher::Job_Command::update_job()
{
#ifdef WITH_LIBBATCH
  Batch::Parametre params = common_job_params();
  params[Batch::EXECUTABLE] = buildCommandScript(params, _launch_date);
  _batch_job->setParametre(params);
#endif
}

#ifdef WITH_LIBBATCH
std::string
Launcher::Job_Command::buildCommandScript(Batch::Parametre params, std::string launch_date)
{
  // parameters
  std::string work_directory = params[Batch::WORKDIR].str();

  // File name
  std::string launch_date_port_file = launch_date;
  std::ostringstream str_pid;
#ifdef WIN32
  str_pid << _getpid();
#else
  str_pid << ::getpid();
#endif
  std::string launch_script = Kernel_Utils::GetTmpDir() + "runCommand_"
                              + _job_file_name + "_" + launch_date + "-"
                              + str_pid.str();
#ifndef WIN32
  launch_script += ".sh";
#else
  launch_script += ".bat";
#endif
  std::ofstream launch_script_stream;
  launch_script_stream.open(launch_script.c_str(), std::ofstream::out);

  // Script
#ifndef WIN32
  launch_script_stream << "#!/bin/sh -f" << std::endl;
  launch_script_stream << "cd " << work_directory << std::endl;
  // remove the exit code from any previous execution
  launch_script_stream << "rm -f logs/exit_code.log" << std::endl;
  launch_script_stream << "export PYTHONPATH=" << work_directory << ":$PYTHONPATH" << std::endl;
  launch_script_stream << "export PATH=" << work_directory << ":$PATH" << std::endl;
  if (_env_file != "")
  {
    std::string::size_type last = _env_file.find_last_of("/");
    launch_script_stream << ". ./" << _env_file.substr(last+1) << std::endl;
  }
#else
  launch_script_stream << "echo OFF" << std::endl;
  launch_script_stream << "cd " << work_directory << std::endl;
  launch_script_stream << "set PYTHONPATH=" << work_directory << ";%PYTHONPATH%" << std::endl;
  launch_script_stream << "set PATH=" << work_directory << ";%PATH%" << std::endl;
  if (_env_file != "")
  {
    std::string::size_type last = _env_file.find_last_of("\\");
    launch_script_stream << "call " << _env_file.substr(last+1) << std::endl;
  }
#endif
  launch_script_stream << runCommandString() << std::endl;
#ifndef WIN32
  // log the exit code
  launch_script_stream << "echo $? > logs/exit_code.log" << std::endl;
#endif

  // Return
  launch_script_stream.flush();
  launch_script_stream.close();
  chmod(launch_script.c_str(), 0x1ED);
  chmod(_job_file.c_str(), 0x1ED);
  return launch_script;
}

std::string Launcher::Job_Command::runCommandString()
{
  std::ostringstream result;
#ifndef WIN32
  result << "./" << _job_file_name_complete;
#else
  result << "call " << _job_file_name_complete;
#endif
  return result.str();
}
#endif
