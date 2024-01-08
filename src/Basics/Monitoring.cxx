// Copyright (C) 2023  CEA/DEN, EDF R&D
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

#include "Monitoring.hxx"

#include "baseutilities.h"

#include <cstdint>
#include <cmath>
#include <vector>
#include <numeric>
#include <iomanip>
#include <iostream>

#include <sstream>
#include <array>
#include <stdexcept>
#include <thread>
#include <stdio.h>
#include <chrono>
#include <fstream>

#ifndef WIN32
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <signal.h>
#endif

static std::string _out_filename;
#ifndef WIN32
static pid_t pid_of_subprocess = 0;
#endif

#ifndef WIN32
static void LaunchMonitoringLinux(const std::string& pyScriptToEvaluate, const std::string& outFileName)
{
  constexpr char PYTHON_EXEC[] = "python3";
  pid_t pid = fork();
  if (pid == -1)
  {
    throw std::runtime_error("LaunchMonitoring : Error at creation of sub process !");
  }
  else if( pid == 0)
  {
    execlp(PYTHON_EXEC,PYTHON_EXEC,pyScriptToEvaluate.c_str(),nullptr);
    char buff[1024];
    std::ostringstream oss; oss << "LaunchMonitoring : Error during exe : " << strerror_r(errno, buff, sizeof(buff));
    throw std::runtime_error( oss.str() );
  }
  else
  {
    pid_of_subprocess = pid;
  }
}
#endif

void SALOME::LaunchMonitoring(const std::string& pyScriptToEvaluate, const std::string& outFileName)
{
  _out_filename = outFileName;
#ifndef WIN32
  LaunchMonitoringLinux(pyScriptToEvaluate,outFileName);
#else
  throw std::runtime_error("LaunchMonitoring not implemented for Windows !");
#endif
}

std::vector<double> SALOME::ReadFloatsInFile(const std::string& fileName)
{
  std::ifstream inputFile( fileName );

  if(!inputFile.is_open())
  {
    std::ostringstream oss; oss << "Impossible to open file \"" << _out_filename<< "\" !";
    throw std::runtime_error( oss.str() );
  }
  std::vector<double> ret;
  std::string line;
  try
  {
      while (std::getline(inputFile, line))
      {
        std::istringstream iss(line);
        double floatValue;
        if( !(iss >> floatValue) )
          throw std::invalid_argument("Conversion into FP failed !");
        if( !iss.eof() )
          throw std::invalid_argument("Conversion into FP failed !");
        ret.push_back(floatValue);
      }
      inputFile.close();
  }
  catch (const std::exception& e)
  {
  }
  return ret;
}

#ifndef WIN32
static std::vector<double> StopMonitoringLinux()
{
  kill( pid_of_subprocess, SIGTERM );
  std::vector<double> ret;
  try
  {
    ret = SALOME::ReadFloatsInFile( _out_filename );
  }
  catch(std::exception& e) { }
  pid_of_subprocess = 0;
  _out_filename.clear();
  return ret;
}
#endif

std::vector<double> SALOME::StopMonitoring()
{
#ifndef WIN32
  return StopMonitoringLinux();
#else
  throw std::runtime_error("StopMonitoring not implemented for Windows !");
#endif
}
