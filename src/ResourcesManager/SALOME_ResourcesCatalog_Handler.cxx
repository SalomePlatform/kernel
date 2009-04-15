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
//  SALOME ResourcesCatalog : implementation of catalog resources parsing (SALOME_ModuleCatalog.idl)
//  File   : SALOME_ResourcesCatalog_Handler.cxx
//  Author : Estelle Deville
//  Module : SALOME
//$Header$
//
#include "SALOME_ResourcesCatalog_Handler.hxx"
#include "Basics_Utils.hxx"
#include <iostream>
#include <map>

using namespace std;

//=============================================================================
/*!
 *  Constructor
 *  \param listOfResources: map of ParserResourcesType to fill when parsing
 */ 
//=============================================================================

SALOME_ResourcesCatalog_Handler::
SALOME_ResourcesCatalog_Handler(MapOfParserResourcesType& resources_list,
				MapOfParserResourcesType& resources_batch_list):
    _resources_list(resources_list),
    _resources_batch_list(resources_batch_list)
{
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
  test_components = "component";
  test_component_name = "name";
  test_os = "OS";
  test_mem_in_mb = "memInMB";
  test_cpu_freq_mhz = "CPUFreqMHz";
  test_nb_of_nodes = "nbOfNodes";
  test_nb_of_proc_per_node = "nbOfProcPerNode";
  test_batch_queue = "batchQueue";
  test_user_commands = "userCommands";
}

//=============================================================================
/*!
 *  Destructor
 */ 
//=============================================================================

SALOME_ResourcesCatalog_Handler::~SALOME_ResourcesCatalog_Handler()
{
  //  cout << "SALOME_ResourcesCatalog_Handler destruction") << endl;
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
            {
	      xmlChar* hostname = xmlGetProp(aCurNode, (const xmlChar*)test_hostname);
	      _resource.DataForSort._hostName = (const char*)hostname;
	      _resource.HostName = (const char*)hostname;
              xmlFree(hostname);
            }
	  else
	    break;

	  if (xmlHasProp(aCurNode, (const xmlChar*)test_alias))
            {
	      xmlChar* alias = xmlGetProp(aCurNode, (const xmlChar*)test_alias);
	      _resource.Alias = (const char*)alias;
              xmlFree(alias);
            }
	  else
	    _resource.Alias = "";

          if (xmlHasProp(aCurNode, (const xmlChar*)test_batch_queue))
            {
              xmlChar* batch_queue = xmlGetProp(aCurNode, (const xmlChar*)test_batch_queue);
              _resource.batchQueue = (const char*)batch_queue;
              xmlFree(batch_queue);
            }
          else
            _resource.batchQueue = "";

          if (xmlHasProp(aCurNode, (const xmlChar*)test_user_commands))
            {
              xmlChar* user_commands= xmlGetProp(aCurNode, (const xmlChar*)test_user_commands);
              _resource.userCommands = (const char*)user_commands;
              xmlFree(user_commands);
            }
          else
            _resource.userCommands = "";
          
          if (xmlHasProp(aCurNode, (const xmlChar*)test_protocol))
            {
              xmlChar* protocol= xmlGetProp(aCurNode, (const xmlChar*)test_protocol);
              switch ( protocol[0])
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
              xmlFree(protocol);
            }
          else
            _resource.Protocol = rsh;
	  
	  if (xmlHasProp(aCurNode, (const xmlChar*)test_mode))
            {
	      xmlChar* mode=xmlGetProp(aCurNode, (const xmlChar*)test_mode);
	      switch ( mode[0] )
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
              xmlFree(mode);
            }
          else
            _resource.Mode = interactive;

	  if (xmlHasProp(aCurNode, (const xmlChar*)test_batch))
            {
              xmlChar* batch = xmlGetProp(aCurNode, (const xmlChar*)test_batch);
              std::string aBatch = (const char*)batch;
              xmlFree(batch);
              if (aBatch == "pbs")
                _resource.Batch = pbs;
              else if  (aBatch == "lsf")
                _resource.Batch = lsf;
              else if  (aBatch == "sge")
                _resource.Batch = sge;
              else
                _resource.Batch = none;
            }

	  if (xmlHasProp(aCurNode, (const xmlChar*)test_mpi))
            {
              xmlChar* mpi = xmlGetProp(aCurNode, (const xmlChar*)test_mpi);
              std::string anMpi = (const char*)mpi;
              xmlFree(mpi);
              if (anMpi == "lam")
                _resource.mpi = lam;
              else if (anMpi == "mpich1")
                _resource.mpi = mpich1;
              else if (anMpi == "mpich2")
                _resource.mpi = mpich2;
              else if (anMpi == "openmpi")
                _resource.mpi = openmpi;
              else if  (anMpi == "slurm")
                _resource.mpi = slurm;
              else if  (anMpi == "prun")
                _resource.mpi = prun;
              else
                _resource.mpi = nompi;
            }

	  if (xmlHasProp(aCurNode, (const xmlChar*)test_user_name))
            {
	      xmlChar* user_name= xmlGetProp(aCurNode, (const xmlChar*)test_user_name);
	      _resource.UserName = (const char*)user_name;
              xmlFree(user_name);
            }
	  
	  if (xmlHasProp(aCurNode, (const xmlChar*)test_appli_path))
            {
	      xmlChar* appli_path = xmlGetProp(aCurNode, (const xmlChar*)test_appli_path);
	      _resource.AppliPath = (const char*)appli_path;
              xmlFree(appli_path);
            }
	  
	  if (xmlHasProp(aCurNode, (const xmlChar*)test_os))
            {
	      xmlChar* os = xmlGetProp(aCurNode, (const xmlChar*)test_os);
	      _resource.OS = (const char*)os;
              xmlFree(os);
            }
	  
	  if (xmlHasProp(aCurNode, (const xmlChar*)test_mem_in_mb))
            {
	      xmlChar* mem_in_mb = xmlGetProp(aCurNode, (const xmlChar*)test_mem_in_mb);
	      _resource.DataForSort._memInMB = atoi((const char*)mem_in_mb);
              xmlFree(mem_in_mb);
            }
	  
	  if (xmlHasProp(aCurNode, (const xmlChar*)test_cpu_freq_mhz))
            {
	      xmlChar* cpu_freq_mhz = xmlGetProp(aCurNode, (const xmlChar*)test_cpu_freq_mhz);
	      _resource.DataForSort._CPUFreqMHz = atoi((const char*)cpu_freq_mhz);
              xmlFree(cpu_freq_mhz);
            }
	  
	  if (xmlHasProp(aCurNode, (const xmlChar*)test_nb_of_nodes))
            {
	      xmlChar* nb_of_nodes = xmlGetProp(aCurNode, (const xmlChar*)test_nb_of_nodes);
	      _resource.DataForSort._nbOfNodes = atoi((const char*)nb_of_nodes);
              xmlFree(nb_of_nodes);
            }
	  
	  if (xmlHasProp(aCurNode, (const xmlChar*)test_nb_of_proc_per_node))
            {
	      xmlChar* nb_of_proc_per_node = xmlGetProp(aCurNode, (const xmlChar*)test_nb_of_proc_per_node);
	      _resource.DataForSort._nbOfProcPerNode = atoi((const char*)nb_of_proc_per_node);
              xmlFree(nb_of_proc_per_node);
            }
	  
	  // Process components
	  xmlNodePtr aCurSubNode = aCurNode->xmlChildrenNode;
	  while(aCurSubNode != NULL)
	    {
	      if ( !xmlStrcmp(aCurSubNode->name, (const xmlChar*)test_components) )
		{
                  //If a component is given, it is in a module with the same name
                  //except if the module name is given
		  if (xmlHasProp(aCurSubNode, (const xmlChar*)test_component_name)) 
		    {
		      xmlChar* component_name = xmlGetProp(aCurSubNode, (const xmlChar*)test_component_name);
		      std::string aComponentName = (const char*)component_name;
                      _resource.ComponentsList.push_back(aComponentName);
		      if (xmlHasProp(aCurSubNode, (const xmlChar*)test_module_name)) 
                        {
		          xmlChar* module_name = xmlGetProp(aCurSubNode, (const xmlChar*)test_module_name);
		          std::string aModuleName = (const char*)module_name;
                          _resource.ModulesList.push_back(aModuleName);
                          xmlFree(module_name);
                        }
                      else
                        _resource.ModulesList.push_back(aComponentName);
                      xmlFree(component_name);
		    }
		}
              else if ( !xmlStrcmp(aCurSubNode->name, (const xmlChar*)test_modules) )
		{
                  // If a module is given, we create an entry in componentsList and modulesList
                  // with the same name (module == component)
		  if (xmlHasProp(aCurSubNode, (const xmlChar*)test_module_name)) 
		    {
		      xmlChar* component_name = xmlGetProp(aCurSubNode, (const xmlChar*)test_module_name);
		      std::string aComponentName = (const char*)component_name;
                      _resource.ComponentsList.push_back(aComponentName);
                      _resource.ModulesList.push_back(aComponentName);
                      xmlFree(component_name);
		    }
		}
	      aCurSubNode = aCurSubNode->next;
	    }
	 
	  // There is two lists
	  // _resources_list for interactive resources
	  // _resources_batch_list for batch resources
	  // This choice is done with Mode parameter
	  if (_resource.Mode == interactive)
	  {
	    int aNbNodes = _resource.DataForSort._nbOfNodes;
	    if( aNbNodes > 1 ){
	      string clusterNode = _resource.DataForSort._hostName ;
	      for( int i=0; i < aNbNodes; i++ ){
		char inode[64];
		inode[0] = '\0' ;
		sprintf(inode,"%s%d",clusterNode.c_str(),i+1);
		std::string nodeName(inode);
		_resource.DataForSort._hostName = nodeName ;
		_resource.HostName = nodeName ;
		_resources_list[nodeName] = _resource;
	      }
            }
            else
              {
                if(_resource.HostName == "localhost")
                  {
                    _resource.HostName = Kernel_Utils::GetHostname();
                    _resource.DataForSort._hostName = Kernel_Utils::GetHostname();
                    _resources_list[Kernel_Utils::GetHostname()] = _resource;
                  }
                else
                  _resources_list[_resource.HostName] = _resource;
              }
          }
          else
            _resources_batch_list[_resource.HostName] = _resource;
        }
      aCurNode = aCurNode->next;
    }

#ifdef _DEBUG_
    for (map<string, ParserResourcesType>::const_iterator iter =
	   _resources_list.begin();
	 iter != _resources_list.end();
	 iter++)
      {
	std::cerr << (*iter).first << std::endl;
	std::cerr << (*iter).second.HostName << std::endl;
	std::cerr << (*iter).second.Alias << std::endl;
	std::cerr << (*iter).second.UserName << std::endl;
	std::cerr << (*iter).second.AppliPath << std::endl;
	std::cerr << (*iter).second.OS << std::endl;
	std::cerr << (*iter).second.Protocol << std::endl;
	std::cerr << (*iter).second.Mode << std::endl;
      }
#endif

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
      xmlNewProp(node, BAD_CAST test_hostname, BAD_CAST (*iter).second.HostName.c_str());
      xmlNewProp(node, BAD_CAST test_alias, BAD_CAST (*iter).second.Alias.c_str());
      xmlNewProp(node, BAD_CAST test_batch_queue, BAD_CAST (*iter).second.batchQueue.c_str());
      xmlNewProp(node, BAD_CAST test_user_commands, BAD_CAST (*iter).second.userCommands.c_str());
  
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
	case sge:
	  xmlNewProp(node, BAD_CAST test_batch, BAD_CAST "sge");
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
	case slurm:
	  xmlNewProp(node, BAD_CAST test_mpi, BAD_CAST "slurm");
          break;
	case prun:
	  xmlNewProp(node, BAD_CAST test_mpi, BAD_CAST "prun");
          break;
        default:
	  xmlNewProp(node, BAD_CAST test_mpi, BAD_CAST "");
        }

      xmlNewProp(node, BAD_CAST test_user_name, BAD_CAST (*iter).second.UserName.c_str());

     for (vector<string>::const_iterator iter2 =
             (*iter).second.ComponentsList.begin();
           iter2 != (*iter).second.ComponentsList.end();
           iter2++)
        {
	  node1 = xmlNewChild(node, NULL, BAD_CAST test_components, NULL);
	  xmlNewProp(node1, BAD_CAST test_component_name, BAD_CAST (*iter2).c_str());
        }

      xmlNewProp(node, BAD_CAST test_os, BAD_CAST (*iter).second.OS.c_str());
      xmlNewProp(node, BAD_CAST test_mem_in_mb, BAD_CAST sprintf(string_buf, "%u", (*iter).second.DataForSort._memInMB));
      xmlNewProp(node, BAD_CAST test_cpu_freq_mhz, BAD_CAST sprintf(string_buf, "%u", (*iter).second.DataForSort._CPUFreqMHz));
      xmlNewProp(node, BAD_CAST test_nb_of_nodes, BAD_CAST sprintf(string_buf, "%u", (*iter).second.DataForSort._nbOfNodes));
      xmlNewProp(node, BAD_CAST test_nb_of_proc_per_node, BAD_CAST sprintf(string_buf, "%u", (*iter).second.DataForSort._nbOfProcPerNode));
    }
  for (map<string, ParserResourcesType>::iterator iter =
         _resources_batch_list.begin();
       iter != _resources_batch_list.end();
       iter++)
    {
      node = xmlNewChild(root_node, NULL, BAD_CAST test_machine, NULL);
      xmlNewProp(node, BAD_CAST test_hostname, BAD_CAST (*iter).second.HostName.c_str());
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
	case sge:
	  xmlNewProp(node, BAD_CAST test_batch, BAD_CAST "sge");
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
	case slurm:
	  xmlNewProp(node, BAD_CAST test_mpi, BAD_CAST "slurm");
          break;
	case prun:
	  xmlNewProp(node, BAD_CAST test_mpi, BAD_CAST "prun");
          break;
        default:
	  xmlNewProp(node, BAD_CAST test_mpi, BAD_CAST "");
        }

      xmlNewProp(node, BAD_CAST test_user_name, BAD_CAST (*iter).second.UserName.c_str());

     for (vector<string>::const_iterator iter2 =
             (*iter).second.ComponentsList.begin();
           iter2 != (*iter).second.ComponentsList.end();
           iter2++)
        {
	  node1 = xmlNewChild(node, NULL, BAD_CAST test_components, NULL);
	  xmlNewProp(node1, BAD_CAST test_component_name, BAD_CAST (*iter2).c_str());
        }

      xmlNewProp(node, BAD_CAST test_os, BAD_CAST (*iter).second.OS.c_str());
      xmlNewProp(node, BAD_CAST test_mem_in_mb, BAD_CAST sprintf(string_buf, "%u", (*iter).second.DataForSort._memInMB));
      xmlNewProp(node, BAD_CAST test_cpu_freq_mhz, BAD_CAST sprintf(string_buf, "%u", (*iter).second.DataForSort._CPUFreqMHz));
      xmlNewProp(node, BAD_CAST test_nb_of_nodes, BAD_CAST sprintf(string_buf, "%u", (*iter).second.DataForSort._nbOfNodes));
      xmlNewProp(node, BAD_CAST test_nb_of_proc_per_node, BAD_CAST sprintf(string_buf, "%u", (*iter).second.DataForSort._nbOfProcPerNode));
    }
}
