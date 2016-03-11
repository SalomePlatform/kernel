// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

#include "SALOME_Launcher_Parser.hxx"
#include <iostream>
#include <sstream>

#define NULL_VALUE 0

void MachineParameters::Clear()
{
  EnvFile = "";
  WorkDirectory = "";
}

void MachineParameters::Print() const
{
  std::ostringstream oss;
  oss << " EnvFile: " <<  EnvFile 
      << " WorkDirectory: " <<  WorkDirectory << std::endl;

  std::cout << oss.str();
}

void ParserLauncherType::Print() const
{
  std::ostringstream oss;
  oss << std::endl <<
    "RefDirectory: " << RefDirectory << std::endl << 
    "NbOfProcesses: " << NbOfProcesses << std::endl <<
    "InputFile: ";
  for(int i=0; i <InputFile.size();i++)
    oss << InputFile[i] << " ";
  oss << std::endl << "OutputFile: ";
  for(int i=0; i <OutputFile.size();i++)
    oss << OutputFile[i] << " ";
  oss << std::endl <<
    "Command: " << Command << std::endl <<
    "Machines: " << std::endl;

  std::cout << oss.str();

  std::map < std::string, MachineParameters >::const_iterator it;
  for(it=MachinesList.begin();it!=MachinesList.end();it++){
    std::cout << "  " << it->first;
    it->second.Print();
  }

}

void ParserLauncherType::Clear()
{
  MachinesList.clear();
  RefDirectory = "";
  NbOfProcesses = 1;
  InputFile.clear();
  OutputFile.clear();
  Command = "";
}
