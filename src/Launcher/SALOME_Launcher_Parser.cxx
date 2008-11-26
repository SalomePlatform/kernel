//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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
#include "SALOME_Launcher_Parser.hxx"
#include <iostream>
#include <sstream>

#define NULL_VALUE 0

using namespace std;

void MachineParameters::Clear()
{
  EnvFile = "";
  WorkDirectory = "";
}

void MachineParameters::Print() const
{
  ostringstream oss;
  oss << " EnvFile: " <<  EnvFile 
      << " WorkDirectory: " <<  WorkDirectory << endl;

  cout << oss.str();
}

void ParserLauncherType::Print() const
{
  ostringstream oss;
  oss << endl <<
    "RefDirectory: " << RefDirectory << endl << 
    "NbOfProcesses: " << NbOfProcesses << endl <<
    "InputFile: ";
  for(int i=0; i <InputFile.size();i++)
    oss << InputFile[i] << " ";
  oss << endl << "OutputFile: ";
  for(int i=0; i <OutputFile.size();i++)
    oss << OutputFile[i] << " ";
  oss << endl <<
    "Command: " << Command << endl <<
    "Machines: " << endl;

  cout << oss.str();

  std::map < string, MachineParameters >::const_iterator it;
  for(it=MachinesList.begin();it!=MachinesList.end();it++){
    cout << "  " << it->first;
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
