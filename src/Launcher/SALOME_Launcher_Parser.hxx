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

//  SALOME ResourcesCatalog : implementation of catalog resources parsing (SALOME_ModuleCatalog.idl)
//  File   : SALOME_Launcher_Parser.hxx
//  Author : Bernard Secher
//  Module : SALOME
//$Header$
//
#ifndef SALOME_LAUNCHER_PARSER
#define SALOME_LAUNCHER_PARSER

#include <string>
#include <vector>
#include <map>
#include <iostream>

struct MachineParameters
  {
    std::string EnvFile;
    std::string WorkDirectory;
    void Print() const;
    void Clear();
  };

struct ParserLauncherType
  {
    std::map<std::string,MachineParameters> MachinesList;
    std::string RefDirectory;
    long NbOfProcesses;
    std::vector<std::string> InputFile;
    std::vector<std::string> OutputFile;
    std::string Command;

    void Print() const;
    void Clear();
  };

#endif //SALOME_LAUNCHER_PARSER
