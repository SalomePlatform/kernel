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
//  File   : SALOME_Launcher_Handler.hxx
//  Author : Bernard SECHER
//  Module : SALOME
//$Header$
//
#ifndef SALOME_LAUNCHER_HANDLER
#define SALOME_LAUNCHER_HANDLER

#include "SALOME_Launcher_Parser.hxx"

#include <string>
#include <vector>

#include <libxml/parser.h>

class SALOME_Launcher_Handler
{
  
public :
  SALOME_Launcher_Handler(ParserLauncherType& launch);

  virtual ~SALOME_Launcher_Handler();

  const ParserLauncherType& GetLauncherAfterParsing() const;

  void ProcessXmlDocument(xmlDocPtr theDoc);
  
private :
  ParserLauncherType& _launch;
  MachineParameters _machp;

  const char *test_machine_list;
  const char *test_main;

  const char *test_machine;
  const char *test_env_file;
  const char *test_work_directory;
  const char *test_ref_directory;
  const char *test_nb_processes;
  const char *test_input_file;
  const char *test_output_file;
  const char *test_command;

};

#endif // SALOME_LAUNCHER_HANDLER
