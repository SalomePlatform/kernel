//  SALOME ResourcesCatalog : implementation of catalog resources parsing (SALOME_ModuleCatalog.idl)
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : SALOME_ResourcesCatalog_Handler.cxx
//  Author : Estelle Deville
//  Module : SALOME
//$Header$

#include "SALOME_ResourcesCatalog_Handler.hxx"
#include <iostream>
#include <map>
#include "utilities.h"

using namespace std;

#ifdef _DEBUG_
static int MYDEBUG = 1;
#else
static int MYDEBUG = 0;
#endif

//=============================================================================
/*!
 *  Constructor
 *  \param listOfResources: map of ParserResourcesType to fill when parsing
 */ 
//=============================================================================

SALOME_ResourcesCatalog_Handler::
SALOME_ResourcesCatalog_Handler(MapOfParserResourcesType& listOfResources):
    _resources_list(listOfResources)
{
  MESSAGE("SALOME_ResourcesCatalog_Handler creation");
  //XML tags initialisation
  test_machine = "machine";
  test_resources = "resources";

  test_hostname = "hostname";
  test_alias = "alias";
  test_protocol = "protocol";
  test_mode = "mode";
  test_batch = "batch";
  test_mpi = "mpi";
  test_user_name = "userName";
  test_appli_path = "appliPath";
  test_modules = "modules";
  test_module_name = "moduleName";
  test_os = "OS";
  test_mem_in_mb = "memInMB";
  test_cpu_freq_mhz = "CPUFreqMHz";
  test_nb_of_nodes = "nbOfNodes";
  test_nb_of_proc_per_node = "nbOfProcPerNode";
}

//=============================================================================
/*!
 *  Destructor
 */ 
//=============================================================================

SALOME_ResourcesCatalog_Handler::~SALOME_ResourcesCatalog_Handler()
{
  //  MESSAGE("SALOME_ResourcesCatalog_Handler destruction");
}

//=============================================================================
/*!
 *  Retrieves DS after the file parse.
 */ 
//=============================================================================

const MapOfParserResourcesType&
SALOME_ResourcesCatalog_Handler::GetResourcesAfterParsing() const
{
  return _resources_list;
}

//=============================================================================
/*!
 *  Processes XML document and fills the list of resources
 */ 
//=============================================================================

void SALOME_ResourcesCatalog_Handler::ProcessXmlDocument(xmlDocPtr theDoc)
{
  if (MYDEBUG) MESSAGE("Begin parse document");

  // Empty private elements
  _resources_list.clear();

  // Get the document root node
  xmlNodePtr aCurNode = xmlDocGetRootElement(theDoc);

  aCurNode = aCurNode->xmlChildrenNode;
  
  // Processing the document nodes
  while(aCurNode != NULL)
    {
      if ( !xmlStrcmp(aCurNode->name,(const xmlChar*)test_machine) )
	{
          _resource.Clear();
	  if (xmlHasProp(aCurNode, (const xmlChar*)test_hostname))
	    _resource.DataForSort._hostName = (const char*)xmlGetProp(aCurNode, (const xmlChar*)test_hostname);
	  else
	    break;

	  if (xmlHasProp(aCurNode, (const xmlChar*)test_alias))
	    _resource.Alias = (const char*)xmlGetProp(aCurNode, (const xmlChar*)test_alias);
	  else
	    _resource.Alias = "";

	  switch ( ((const char*)xmlGetProp(aCurNode, (const xmlChar*)test_protocol))[0] )
	    {
	    case 'r':
	      _resource.Protocol = rsh;
	      break;
	    case 's':
	      _resource.Protocol = ssh;
	      break;
	    default:
	      // If it'not in all theses cases, the protocol is affected to rsh
	      _resource.Protocol = rsh;
	      break;
	    }
	  
	  switch ( ((const char*)xmlGetProp(aCurNode, (const xmlChar*)test_mode))[0] )
            {
            case 'i':
              _resource.Mode = interactive;
              break;
	    case 'b':
              _resource.Mode = batch;
              break;
            default:
              // If it'not in all theses cases, the mode is affected to interactive
              _resource.Mode = interactive;
              break;
            }

	  if (xmlHasProp(aCurNode, (const xmlChar*)test_batch))
            {
              std::string aBatch = (const char*)xmlGetProp(aCurNode, (const xmlChar*)test_batch);
              if (aBatch == "pbs")
                _resource.Batch = pbs;
              else if  (aBatch == "lsf")
                _resource.Batch = lsf;
              else if  (aBatch == "slurm")
                _resource.Batch = slurm;
              else
                _resource.Batch = none;
            }

	  if (xmlHasProp(aCurNode, (const xmlChar*)test_mpi))
            {
              std::string anMpi = (const char*)xmlGetProp(aCurNode, (const xmlChar*)test_mpi);
              if (anMpi == "lam")
                _resource.mpi = lam;
              else if (anMpi == "mpich1")
                _resource.mpi = mpich1;
              else if (anMpi == "mpich2")
                _resource.mpi = mpich2;
              else if (anMpi == "openmpi")
                _resource.mpi = openmpi;
              else
                _resource.mpi = indif;
            }

	  if (xmlHasProp(aCurNode, (const xmlChar*)test_user_name))
	    _resource.UserName = (const char*)xmlGetProp(aCurNode, (const xmlChar*)test_user_name);
	  
	  if (xmlHasProp(aCurNode, (const xmlChar*)test_appli_path))
	    _resource.AppliPath = (const char*)xmlGetProp(aCurNode, (const xmlChar*)test_appli_path);
	  
	  if (xmlHasProp(aCurNode, (const xmlChar*)test_os))
	    _resource.OS = (const char*)xmlGetProp(aCurNode, (const xmlChar*)test_os);
	  
	  if (xmlHasProp(aCurNode, (const xmlChar*)test_mem_in_mb))
	    _resource.DataForSort._memInMB = atoi((const char*)xmlGetProp(aCurNode, (const xmlChar*)test_mem_in_mb));
	  
	  if (xmlHasProp(aCurNode, (const xmlChar*)test_cpu_freq_mhz))
	    _resource.DataForSort._CPUFreqMHz = atoi((const char*)xmlGetProp(aCurNode, (const xmlChar*)test_cpu_freq_mhz));
	  
	  if (xmlHasProp(aCurNode, (const xmlChar*)test_nb_of_nodes))
	    _resource.DataForSort._nbOfNodes = atoi((const char*)xmlGetProp(aCurNode, (const xmlChar*)test_nb_of_nodes));
	  
	  if (xmlHasProp(aCurNode, (const xmlChar*)test_nb_of_proc_per_node))
	    _resource.DataForSort._nbOfProcPerNode = atoi((const char*)xmlGetProp(aCurNode, (const xmlChar*)test_nb_of_proc_per_node));
	  
	  // Process modules
	  xmlNodePtr aCurSubNode = aCurNode->xmlChildrenNode;
	  while(aCurSubNode != NULL)
	    {
	      if ( !xmlStrcmp(aCurSubNode->name, (const xmlChar*)test_modules) )
		{
		  if (xmlHasProp(aCurSubNode, (const xmlChar*)test_module_name)) 
		    {
		      std::string aModuleName = (const char*)xmlGetProp(aCurSubNode, (const xmlChar*)test_module_name);
                      _resource.ModulesList.push_back(aModuleName);
		    }
		}
	      aCurSubNode = aCurSubNode->next;
	    }
	  
	  int aNbNodes = _resource.DataForSort._nbOfNodes;
	  if( aNbNodes > 1 ){
	    string clusterNode = _resource.DataForSort._hostName ;
	    for( int i=0; i < aNbNodes; i++ ){
	      char inode[64];
	      inode[0] = '\0' ;
	      sprintf(inode,"%s%d",clusterNode.c_str(),i+1);
	      std::string nodeName(inode);
	      _resource.DataForSort._hostName = nodeName ;
	      _resources_list[nodeName] = _resource;
	    }
	  }
	  else
	    _resources_list[_resource.DataForSort._hostName] = _resource;
	}
      
      aCurNode = aCurNode->next;
    }

  // For debug only
  if (MYDEBUG)
    {
      for (map<string, ParserResourcesType>::const_iterator iter =
	     _resources_list.begin();
	   iter != _resources_list.end();
	   iter++)
	{
	  SCRUTE((*iter).second.Alias);
	  SCRUTE((*iter).second.UserName);
	  SCRUTE((*iter).second.AppliPath);
	  SCRUTE((*iter).second.OS);
	  SCRUTE((*iter).second.Protocol);
	  SCRUTE((*iter).second.Mode);
	}
      
      MESSAGE("This is the end of document");
    }
}


//=============================================================================
/*!
 *  Fill the document tree in xml file, used to write in an xml file.
 *  \param theDoc document to fill.
 */ 
//=============================================================================

void SALOME_ResourcesCatalog_Handler::PrepareDocToXmlFile(xmlDocPtr theDoc)
{
  // Node pointers
  xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;
  char string_buf[80];

  root_node = xmlNewNode(NULL, BAD_CAST "resources");
  xmlDocSetRootElement(theDoc, root_node);
    
  for (map<string, ParserResourcesType>::iterator iter =
         _resources_list.begin();
       iter != _resources_list.end();
       iter++)
    {
      node = xmlNewChild(root_node, NULL, BAD_CAST test_machine, NULL);
      xmlNewProp(node, BAD_CAST test_hostname, BAD_CAST (*iter).first.c_str());
      xmlNewProp(node, BAD_CAST test_alias, BAD_CAST (*iter).second.Alias.c_str());
      
      switch ((*iter).second.Protocol)
        {
        case rsh:
	  xmlNewProp(node, BAD_CAST test_protocol, BAD_CAST "rsh");
          break;
        case ssh:
	  xmlNewProp(node, BAD_CAST test_protocol, BAD_CAST "ssh");
	  break;
        default:
	  xmlNewProp(node, BAD_CAST test_protocol, BAD_CAST "rsh");
	}

      switch ((*iter).second.Mode)
        {
	case interactive:
	  xmlNewProp(node, BAD_CAST test_mode, BAD_CAST "interactive");
          break;
        case batch:
	  xmlNewProp(node, BAD_CAST test_mode, BAD_CAST "batch");
          break;
        default:
	  xmlNewProp(node, BAD_CAST test_mode, BAD_CAST "interactive");
        }

      switch ((*iter).second.Batch)
        {
	case pbs:
	  xmlNewProp(node, BAD_CAST test_batch, BAD_CAST "pbs");
          break;
	case lsf:
	  xmlNewProp(node, BAD_CAST test_batch, BAD_CAST "lsf");
          break;
	case slurm:
	  xmlNewProp(node, BAD_CAST test_batch, BAD_CAST "slurm");
          break;
        default:
	  xmlNewProp(node, BAD_CAST test_batch, BAD_CAST "");
        }

      switch ((*iter).second.mpi)
        {
	case lam:
	  xmlNewProp(node, BAD_CAST test_mpi, BAD_CAST "lam");
          break;
	case mpich1:
	  xmlNewProp(node, BAD_CAST test_mpi, BAD_CAST "mpich1");
          break;
	case mpich2:
	  xmlNewProp(node, BAD_CAST test_mpi, BAD_CAST "mpich2");
          break;
	case openmpi:
	  xmlNewProp(node, BAD_CAST test_mpi, BAD_CAST "openmpi");
          break;
        default:
	  xmlNewProp(node, BAD_CAST test_mpi, BAD_CAST "");
        }

      xmlNewProp(node, BAD_CAST test_user_name, BAD_CAST (*iter).second.UserName.c_str());

     for (vector<string>::const_iterator iter2 =
             (*iter).second.ModulesList.begin();
           iter2 != (*iter).second.ModulesList.end();
           iter2++)
        {
	  node1 = xmlNewChild(node, NULL, BAD_CAST test_modules, NULL);
	  xmlNewProp(node1, BAD_CAST test_module_name, BAD_CAST (*iter2).c_str());
        }

      xmlNewProp(node, BAD_CAST test_os, BAD_CAST (*iter).second.OS.c_str());
      xmlNewProp(node, BAD_CAST test_mem_in_mb, BAD_CAST sprintf(string_buf, "%u", (*iter).second.DataForSort._memInMB));
      xmlNewProp(node, BAD_CAST test_cpu_freq_mhz, BAD_CAST sprintf(string_buf, "%u", (*iter).second.DataForSort._CPUFreqMHz));
      xmlNewProp(node, BAD_CAST test_nb_of_nodes, BAD_CAST sprintf(string_buf, "%u", (*iter).second.DataForSort._nbOfNodes));
      xmlNewProp(node, BAD_CAST test_nb_of_proc_per_node, BAD_CAST sprintf(string_buf, "%u", (*iter).second.DataForSort._nbOfProcPerNode));
    }
}
