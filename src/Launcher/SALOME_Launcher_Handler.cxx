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
//  File   : SALOME_Laucher_Handler.cxx
//  Author : Bernard Secher
//  Module : SALOME
//  $Header$
//
#include "SALOME_Launcher_Handler.hxx"
#include <iostream>
#include <map>

//=============================================================================
/*!
 *  Constructor
 *  \param listOfResources: map of ParserResourcesType to fill when parsing
 */ 
//=============================================================================

SALOME_Launcher_Handler::SALOME_Launcher_Handler(ParserLauncherType& launch):
    _launch(launch)
{
  //XML tags initialisation
  test_machine_list = "machine-list";
  test_main = "main";

  test_machine = "machine";
  test_env_file = "env-file";
  test_work_directory = "work-directory";
  test_ref_directory = "ref-directory";
  test_nb_processes = "nb-processes";
  test_input_file = "input-file";
  test_output_file = "output-file";
  test_command = "command";

}

//=============================================================================
/*!
 *  Destructor
 */ 
//=============================================================================

SALOME_Launcher_Handler::~SALOME_Launcher_Handler()
{
  //  cout << "SALOME_Launcher_Handler destruction") << endl;
}

//=============================================================================
/*!
 *  Retrieves DS after the file parse.
 */ 
//=============================================================================

const ParserLauncherType&
SALOME_Launcher_Handler::GetLauncherAfterParsing() const
{
  return _launch;
}

//=============================================================================
/*!
 *  Processes XML document and fills the list of resources
 */ 
//=============================================================================

void SALOME_Launcher_Handler::ProcessXmlDocument(xmlDocPtr theDoc)
{
//   if (MYDEBUG) cout << "Begin parse document" << endl;

  // Empty private elements
  _launch.Clear();

  // Get the document root node
  xmlNodePtr aCurNode = xmlDocGetRootElement(theDoc);

  aCurNode = aCurNode->xmlChildrenNode;
  
  // Processing the document nodes
  while(aCurNode != NULL){
    if ( !xmlStrcmp(aCurNode->name,(const xmlChar*)test_machine_list) ){
      xmlNodePtr aCurNode2 = aCurNode->xmlChildrenNode;
      while(aCurNode2 != NULL){
        if ( !xmlStrcmp(aCurNode2->name,(const xmlChar*)test_machine) ){
          _machp.Clear();
          xmlChar* name = xmlNodeGetContent(aCurNode2);
          std::string clusterName = (const char*)name;
          xmlFree(name);
        
          if (xmlHasProp(aCurNode2, (const xmlChar*)test_env_file)){
            xmlChar* envfile = xmlGetProp(aCurNode2, (const xmlChar*)test_env_file);
            _machp.EnvFile = (const char*)envfile;
            xmlFree(envfile);
          }

          if (xmlHasProp(aCurNode2, (const xmlChar*)test_work_directory)){
            xmlChar* workdirectory = xmlGetProp(aCurNode2, (const xmlChar*)test_work_directory);
            _machp.WorkDirectory = (const char*)workdirectory;
            xmlFree(workdirectory);
          }
          _launch.MachinesList[clusterName]=_machp ;
        }
        aCurNode2 = aCurNode2->next;
      }
    }

    if ( !xmlStrcmp(aCurNode->name,(const xmlChar*)test_ref_directory) ){
      xmlChar* refdirectory = xmlNodeGetContent(aCurNode);
      _launch.RefDirectory = (const char*)refdirectory;
      xmlFree(refdirectory);
    }
          
    if ( !xmlStrcmp(aCurNode->name,(const xmlChar*)test_nb_processes) ){
      xmlChar* nbofprocesses = xmlNodeGetContent(aCurNode);
      _launch.NbOfProcesses = atoi((const char*)nbofprocesses);
      xmlFree(nbofprocesses);
    }
          
    if ( !xmlStrcmp(aCurNode->name,(const xmlChar*)test_input_file) ){
      xmlChar* inputfile = xmlNodeGetContent(aCurNode);
      _launch.InputFile.push_back((const char*)inputfile);
      xmlFree(inputfile);
    }
          
    if ( !xmlStrcmp(aCurNode->name,(const xmlChar*)test_output_file) ){
      xmlChar* outputfile = xmlNodeGetContent(aCurNode);
      _launch.OutputFile.push_back((const char*)outputfile);
      xmlFree(outputfile);
    }
          
    if ( !xmlStrcmp(aCurNode->name,(const xmlChar*)test_command) ){
      xmlChar* command = xmlNodeGetContent(aCurNode);
      _launch.Command = (const char*)command;
      xmlFree(command);
    }
          
    aCurNode = aCurNode->next;
  }

}
