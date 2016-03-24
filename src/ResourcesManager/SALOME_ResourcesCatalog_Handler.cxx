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
//  File   : SALOME_ResourcesCatalog_Handler.cxx
//  Author : Estelle Deville
//  Module : SALOME
//$Header$
//
#include "SALOME_ResourcesCatalog_Handler.hxx"
#include "Basics_Utils.hxx"
#include <iostream>
#include <sstream>
#include <map>

using namespace std;

//=============================================================================
/*!
 *  Constructor
 *  \param listOfResources: map of ParserResourcesType to fill when parsing
 */ 
//=============================================================================

SALOME_ResourcesCatalog_Handler::
SALOME_ResourcesCatalog_Handler(MapOfParserResourcesType& resources_list): _resources_list(resources_list)
{
  //XML tags initialisation
  test_machine = "machine";
  test_cluster = "cluster";
  test_name = "name";
  test_hostname = "hostname";
  test_type = "type";
  test_protocol = "protocol";
  test_cluster_internal_protocol = "iprotocol";
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
  test_nb_of_proc = "nbOfProc";
  test_nb_of_proc_per_node = "nbOfProcPerNode";
  test_batch_queue = "batchQueue";
  test_user_commands = "userCommands";
  test_use = "use";
  test_members = "members";
  test_is_cluster_head = "isClusterHead";
  test_working_directory = "workingDirectory";
  test_can_launch_batch_jobs = "canLaunchBatchJobs";
  test_can_run_containers = "canRunContainers";
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
    // Declaration of a single machine or a frontal node for a cluster managed by a batch manager
    if (!xmlStrcmp(aCurNode->name,(const xmlChar*)test_machine))
    {
      ParserResourcesType resource;
      bool Ok = ProcessMachine(aCurNode, resource);
      if (Ok)
      {
        // Adding a resource
        if(resource.HostName == "localhost")
        {
          resource.HostName = Kernel_Utils::GetHostname();
        }
        std::map<std::string, ParserResourcesType>::const_iterator iter = _resources_list.find(resource.Name);
        if (iter != _resources_list.end())
          RES_INFOS("Warning resource " << resource.Name << " already added, keep last resource found !");
        _resources_list[resource.Name] = resource;
      }
    }
    // Declaration of a cluster
    // Here, a cluster is NOT the frontal node of a cluster managed by a batch manager (classical
    // usage of a cluster). It is a group of machines intended to be used for a parallel container.
    // The methods ProcessCluster and ProcessMember are only used in the context of parallel
    // containers. They are not used in classical Salome usage scenarios.
    if (!xmlStrcmp(aCurNode->name,(const xmlChar*)test_cluster))
    {
      ParserResourcesType resource;
      if(ProcessCluster(aCurNode, resource))
      {
        std::map<std::string, ParserResourcesType>::const_iterator iter = _resources_list.find(resource.Name);
        if (iter != _resources_list.end())
          RES_INFOS("Warning resource " << resource.Name << " already added, keep last resource found !");
        _resources_list[resource.Name] = resource;
      }
    }
    aCurNode = aCurNode->next;
  }

#ifdef _DEBUG_
  for (std::map<std::string, ParserResourcesType>::const_iterator iter = _resources_list.begin();
       iter != _resources_list.end();
       iter++)
  {
    std::cerr << "************************************************" << std::endl;
    std::cerr << "Resource " << (*iter).first << " found:" << std::endl;
    std::cerr << (*iter).second;
    std::cerr << "************************************************" << std::endl;
  }
#endif
}

bool
SALOME_ResourcesCatalog_Handler::ProcessCluster(xmlNodePtr cluster_descr, ParserResourcesType & resource)
{
  // Ajout d'un cluster
  // hostname, use et nbOfProc sont obligatoires
  if (xmlHasProp(cluster_descr, (const xmlChar*)test_hostname))
  {
    xmlChar* hostname = xmlGetProp(cluster_descr, (const xmlChar*)test_hostname);
    resource.HostName = (const char*)hostname;
    xmlFree(hostname);
  }
  else
  {
    std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessCluster : !!! Warning !!! found a cluster without a hostname" << std::endl;
    std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessCluster : !!! Warning !!! this cluster will not be added" << std::endl;
    return false;
  }

  if (xmlHasProp(cluster_descr, (const xmlChar*)test_name))
  {
    xmlChar* name = xmlGetProp(cluster_descr, (const xmlChar*)test_name);
    resource.Name = (const char*)name;
    resource.DataForSort._Name = (const char*)name;
    xmlFree(name);
  }
  else
  {
    resource.Name = resource.HostName;
    resource.DataForSort._Name = resource.HostName;
    std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessCluster : !!! Warning !!! No Name found use Hostname for resource: " << resource.Name << std::endl;
  }

  if (xmlHasProp(cluster_descr, (const xmlChar*)test_use))
  {
    xmlChar* use = xmlGetProp(cluster_descr, (const xmlChar*)test_use);
    resource.use = (const char*)use;
    xmlFree(use);
  }
  else
  {
    std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessCluster : !!! Warning !!! found a cluster without a use" << std::endl;
    std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessCluster : !!! Warning !!! this cluster will not be added" << std::endl;
    return false;
  }

  if (xmlHasProp(cluster_descr, (const xmlChar*)test_nb_of_proc))
  {
    xmlChar* nb_of_proc = xmlGetProp(cluster_descr, (const xmlChar*)test_nb_of_proc);
    resource.nbOfProc = atoi((const char*)nb_of_proc);
    xmlFree(nb_of_proc);
  }
  else
  {
    std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessCluster : !!! Warning !!! found a cluster without a nbOfProc" << std::endl;
    std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessCluster : !!! Warning !!! this cluster will not be added" << std::endl;
    return false;
  }

  if (xmlHasProp(cluster_descr, (const xmlChar*)test_mpi))
  {
    xmlChar* mpi = xmlGetProp(cluster_descr, (const xmlChar*)test_mpi);
    std::string anMpi = (const char*)mpi;
    xmlFree(mpi);
    resource.setMpiImplTypeStr(anMpi);
  }

  // Parsing des membres du cluster 
  xmlNodePtr aCurSubNode = cluster_descr->xmlChildrenNode;
  while(aCurSubNode != NULL)
  {
    if (!xmlStrcmp(aCurSubNode->name, (const xmlChar*)test_members))
    {
       xmlNodePtr members = aCurSubNode->xmlChildrenNode;
       while (members != NULL)
       {
         // Process members
         if (!xmlStrcmp(members->name, (const xmlChar*)test_machine))
         {
           ParserResourcesType new_member;
           if (ProcessMember(members, new_member))
             resource.ClusterMembersList.push_back(new_member);
         }
         members = members->next;
       }
    }
    aCurSubNode = aCurSubNode->next;
  }

  // Test: Il faut au moins un membre pour que le cluster soit correct !
  if (resource.ClusterMembersList.empty())
  {
    std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessCluster : !!! Warning !!! found a cluster without a member" << std::endl;
    std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessCluster : !!! Warning !!! this cluster will not be added" << std::endl;
    return false;
  }
  return true;
}

bool
SALOME_ResourcesCatalog_Handler::ProcessMember(xmlNodePtr member_descr, ParserResourcesType & resource)
{
  if (xmlHasProp(member_descr, (const xmlChar*)test_hostname))
  {
    xmlChar* hostname = xmlGetProp(member_descr, (const xmlChar*)test_hostname);
    resource.HostName = (const char*)hostname;
    xmlFree(hostname);
  }
  else
  {
    std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessMember : Warning found a machine without a hostname" << std::endl;
    std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessMember : Warning this machine will not be added" << std::endl;
    return false;
  }

  if (xmlHasProp(member_descr, (const xmlChar*)test_protocol))
  {
    xmlChar* protocol= xmlGetProp(member_descr, (const xmlChar*)test_protocol);
    try
    {
      resource.setAccessProtocolTypeStr((const char *)protocol);
    }
    catch (const ResourcesException & e)
    {
      std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessMember : Warning found a machine with a bad protocol" << std::endl;
      std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessMember : Warning this machine will not be added" << std::endl;
      return false;
    }
    xmlFree(protocol);
  }
  else
  {
    std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessMember : Warning found a machine without a protocol" << std::endl;
    std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessMember : Warning this machine will not be added" << std::endl;
    return false;
  }

  if (xmlHasProp(member_descr, (const xmlChar*)test_cluster_internal_protocol))
  {
    xmlChar* iprotocol= xmlGetProp(member_descr, (const xmlChar*)test_cluster_internal_protocol);
    try
    {
      resource.setClusterInternalProtocolStr((const char *)iprotocol);
    }
    catch (const ResourcesException & e)
    {
      std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessMember : Warning found a machine with a bad protocol" << std::endl;
      std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessMember : Warning this machine will not be added" << std::endl;
      return false;
    }
    xmlFree(iprotocol);
  }
  else
  {
    std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessMember : Warning found a machine without a protocol" << std::endl;
    std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessMember : Warning this machine will not be added" << std::endl;
    return false;
  }

  if (xmlHasProp(member_descr, (const xmlChar*)test_user_name))
  {
    xmlChar* user_name= xmlGetProp(member_descr, (const xmlChar*)test_user_name);
    resource.UserName = (const char*)user_name;
    xmlFree(user_name);
  }
  else
  {
    std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessMember : Warning found a machine without a user name" << std::endl;
    std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessMember : Warning this machine will not be added" << std::endl;
    return false;
  }

  if (xmlHasProp(member_descr, (const xmlChar*)test_nb_of_nodes))
  {
    xmlChar* nb_of_nodes = xmlGetProp(member_descr, (const xmlChar*)test_nb_of_nodes);
    resource.DataForSort._nbOfNodes = atoi((const char*)nb_of_nodes);
    xmlFree(nb_of_nodes);
  }
  else
  {
    std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessMember : Warning found a machine without a nbOfNodes" << std::endl;
    std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessMember : Warning this machine will not be added" << std::endl;
    return false;
  }

  if (xmlHasProp(member_descr, (const xmlChar*)test_nb_of_proc_per_node))
  {
    xmlChar* nb_of_proc_per_node = xmlGetProp(member_descr, (const xmlChar*)test_nb_of_proc_per_node);
    resource.DataForSort._nbOfProcPerNode = atoi((const char*)nb_of_proc_per_node);
    xmlFree(nb_of_proc_per_node);
  }
  else
  {
    std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessMember : Warning found a machine without a nbOfProcPerNode" << std::endl;
    std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessMember : Warning this machine will not be added" << std::endl;
    return false;
  }

  if (xmlHasProp(member_descr, (const xmlChar*)test_appli_path))
  {
    xmlChar* appli_path = xmlGetProp(member_descr, (const xmlChar*)test_appli_path);
    resource.AppliPath = (const char*)appli_path;
    xmlFree(appli_path);
  }
  else
  {
    std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessMember : Warning found a machine without a AppliPath" << std::endl;
    std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessMember : Warning this machine will not be added" << std::endl;
    return false;
  }
  return true;
}

bool
SALOME_ResourcesCatalog_Handler::ProcessMachine(xmlNodePtr machine_descr, ParserResourcesType & resource)
{
  if (xmlHasProp(machine_descr, (const xmlChar*)test_hostname))
  {
    xmlChar* hostname = xmlGetProp(machine_descr, (const xmlChar*)test_hostname);
    resource.HostName = (const char*)hostname;
    xmlFree(hostname);
  }
  else
  {
    std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessMachine : Warning found a machine without a hostname" << std::endl;
    std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessMachine : Warning this machine will not be added" << std::endl;
    return false;
  }

  if (xmlHasProp(machine_descr, (const xmlChar*)test_name))
  {
    xmlChar* name = xmlGetProp(machine_descr, (const xmlChar*)test_name);
    resource.Name = (const char*)name;
    resource.DataForSort._Name = (const char*)name;
    xmlFree(name);
  }
  else
  {
    resource.Name = resource.HostName;
    resource.DataForSort._Name = resource.HostName;
    std::cerr << "SALOME_ResourcesCatalog_Handler::ProcessMachine : !!! Warning !!! No Name found use Hostname for resource: " << resource.Name << std::endl;
  }

  // This block is for compatibility with files created in Salome 6.
  // It can safely be removed in Salome 8.
  if (xmlHasProp(machine_descr, (const xmlChar*)test_mode))
  {
    cerr << "Warning: parameter \"" << test_mode << "\" defined for resource \"" <<
            resource.Name << "\" is deprecated. It will be replaced when your resource " <<
            "file is saved." << endl;
    xmlChar* mode=xmlGetProp(machine_descr, (const xmlChar*)test_mode);
    switch ( mode[0] )
    {
      case 'i':
        resource.can_run_containers = true;
        break;
      case 'b':
        resource.can_launch_batch_jobs = true;
        break;
      default:
        break;
    }
    xmlFree(mode);
  }

  if (xmlHasProp(machine_descr, (const xmlChar*)test_is_cluster_head))
  {
    cerr << "Warning: parameter \"" << test_is_cluster_head << "\" defined for resource \"" <<
            resource.Name << "\" is deprecated. It will be replaced when your resource " <<
            "file is saved." << endl;
    xmlChar* is_cluster_head = xmlGetProp(machine_descr, (const xmlChar*)test_is_cluster_head);
    std::string str_ich = (const char*)is_cluster_head;
    if (str_ich == "true")
    {
      resource.type = cluster;
      resource.can_launch_batch_jobs = true;
      resource.can_run_containers = false;
    }
    else
    {
      resource.type = single_machine;
      resource.can_run_containers = true;
    }
    xmlFree(is_cluster_head);
  }
  // End of compatibility block

  if (xmlHasProp(machine_descr, (const xmlChar*)test_type))
  {
    xmlChar* type = xmlGetProp(machine_descr, (const xmlChar*)test_type);
    try
    {
      resource.setResourceTypeStr((const char*)type);
    }
    catch (const ResourcesException & e)
    {
      cerr << "Warning, invalid type \"" << (const char*)type << "\" for resource \"" <<
              resource.Name << "\", using default value \"" << resource.getResourceTypeStr() <<
              "\"" << endl;
    }
    xmlFree(type);
  }
  else
  {
    cerr << "Warning, no type found for resource \"" << resource.Name <<
            "\", using default value \"" << resource.getResourceTypeStr() << "\"" << endl;
  }

  if (xmlHasProp(machine_descr, (const xmlChar*)test_batch_queue))
  {
    xmlChar* batch_queue = xmlGetProp(machine_descr, (const xmlChar*)test_batch_queue);
    resource.batchQueue = (const char*)batch_queue;
    xmlFree(batch_queue);
  }

  if (xmlHasProp(machine_descr, (const xmlChar*)test_user_commands))
  {
    xmlChar* user_commands= xmlGetProp(machine_descr, (const xmlChar*)test_user_commands);
    resource.userCommands = (const char*)user_commands;
    xmlFree(user_commands);
  }

  if (xmlHasProp(machine_descr, (const xmlChar*)test_protocol))
  {
    xmlChar* protocol= xmlGetProp(machine_descr, (const xmlChar*)test_protocol);
    try
    {
      resource.setAccessProtocolTypeStr((const char *)protocol);
    }
    catch (const ResourcesException & e)
    {
      cerr << "Warning, invalid protocol \"" << (const char*)protocol << "\" for resource \"" <<
              resource.Name << "\", using default value \"" <<
              resource.getAccessProtocolTypeStr() << "\"" << endl;
    }
    xmlFree(protocol);
  }

  if (xmlHasProp(machine_descr, (const xmlChar*)test_cluster_internal_protocol))
  {
    xmlChar* iprotocol= xmlGetProp(machine_descr, (const xmlChar*)test_cluster_internal_protocol);
    try
    {
      resource.setClusterInternalProtocolStr((const char *)iprotocol);
    }
    catch (const ResourcesException & e)
    {
      cerr << "Warning, invalid internal protocol \"" << (const char*)iprotocol <<
              "\" for resource \"" << resource.Name << "\", using default value \"" <<
              resource.getClusterInternalProtocolStr() << "\"" << endl;
    }
    xmlFree(iprotocol);
  }
  else
    resource.ClusterInternalProtocol = resource.Protocol;

  if (xmlHasProp(machine_descr, (const xmlChar*)test_batch))
  {
    xmlChar* batch = xmlGetProp(machine_descr, (const xmlChar*)test_batch);
    try
    {
      resource.setBatchTypeStr((const char *)batch);
    }
    catch (const ResourcesException & e)
    {
      cerr << "Warning, invalid batch manager \"" << (const char*)batch <<
              "\" for resource \"" << resource.Name << "\", using default value \"" <<
              resource.getBatchTypeStr() << "\"" << endl;
    }
    xmlFree(batch);
  }

  if (xmlHasProp(machine_descr, (const xmlChar*)test_mpi))
  {
    xmlChar* mpi = xmlGetProp(machine_descr, (const xmlChar*)test_mpi);
    try
    {
      resource.setMpiImplTypeStr((const char *)mpi);
    }
    catch (const ResourcesException & e)
    {
      cerr << "Warning, invalid MPI implementation \"" << (const char*)mpi <<
              "\" for resource \"" << resource.Name << "\", using default value \"" <<
              resource.getMpiImplTypeStr() << "\"" << endl;
    }
    xmlFree(mpi);
  }

  if (xmlHasProp(machine_descr, (const xmlChar*)test_user_name))
  {
    xmlChar* user_name= xmlGetProp(machine_descr, (const xmlChar*)test_user_name);
    resource.UserName = (const char*)user_name;
    xmlFree(user_name);
  }

  if (xmlHasProp(machine_descr, (const xmlChar*)test_appli_path))
  {
    xmlChar* appli_path = xmlGetProp(machine_descr, (const xmlChar*)test_appli_path);
    resource.AppliPath = (const char*)appli_path;
    xmlFree(appli_path);
  }

  if (xmlHasProp(machine_descr, (const xmlChar*)test_os))
  {
    xmlChar* os = xmlGetProp(machine_descr, (const xmlChar*)test_os);
    resource.OS = (const char*)os;
    xmlFree(os);
  }

  if (xmlHasProp(machine_descr, (const xmlChar*)test_mem_in_mb))
  {
    xmlChar* mem_in_mb = xmlGetProp(machine_descr, (const xmlChar*)test_mem_in_mb);
    resource.DataForSort._memInMB = atoi((const char*)mem_in_mb);
    xmlFree(mem_in_mb);
  }

  if (xmlHasProp(machine_descr, (const xmlChar*)test_cpu_freq_mhz))
  {
    xmlChar* cpu_freq_mhz = xmlGetProp(machine_descr, (const xmlChar*)test_cpu_freq_mhz);
    resource.DataForSort._CPUFreqMHz = atoi((const char*)cpu_freq_mhz);
    xmlFree(cpu_freq_mhz);
  }

  if (xmlHasProp(machine_descr, (const xmlChar*)test_nb_of_nodes))
  {
    xmlChar* nb_of_nodes = xmlGetProp(machine_descr, (const xmlChar*)test_nb_of_nodes);
    resource.DataForSort._nbOfNodes = atoi((const char*)nb_of_nodes);
    xmlFree(nb_of_nodes);
  }

  if (xmlHasProp(machine_descr, (const xmlChar*)test_nb_of_proc_per_node))
  {
    xmlChar* nb_of_proc_per_node = xmlGetProp(machine_descr, (const xmlChar*)test_nb_of_proc_per_node);
    resource.DataForSort._nbOfProcPerNode = atoi((const char*)nb_of_proc_per_node);
    xmlFree(nb_of_proc_per_node);
  }

  if (xmlHasProp(machine_descr, (const xmlChar*)test_can_launch_batch_jobs))
  {
    xmlChar* can_launch_batch_jobs = xmlGetProp(machine_descr, (const xmlChar*)test_can_launch_batch_jobs);
    try
    {
      resource.setCanLaunchBatchJobsStr((const char *)can_launch_batch_jobs);
    }
    catch (const ResourcesException & e)
    {
      cerr << "Warning, invalid can_launch_batch_jobs parameter value \"" <<
              (const char*)can_launch_batch_jobs << "\" for resource \"" << resource.Name <<
              "\", using default value \"" << resource.getCanLaunchBatchJobsStr() << "\"" << endl;
    }
    xmlFree(can_launch_batch_jobs);
  }

  if (xmlHasProp(machine_descr, (const xmlChar*)test_can_run_containers))
  {
    xmlChar* can_run_containers = xmlGetProp(machine_descr, (const xmlChar*)test_can_run_containers);
    try
    {
      resource.setCanRunContainersStr((const char *)can_run_containers);
    }
    catch (const ResourcesException & e)
    {
      cerr << "Warning, invalid can_run_containers parameter value \"" <<
              (const char*)can_run_containers << "\" for resource \"" << resource.Name <<
              "\", using default value \"" << resource.getCanRunContainersStr() << "\"" << endl;
    }
    xmlFree(can_run_containers);
  }

  if (xmlHasProp(machine_descr, (const xmlChar*)test_working_directory))
  {
    xmlChar* working_directory = xmlGetProp(machine_descr, (const xmlChar*)test_working_directory);
    resource.working_directory = (const char*)working_directory;
    xmlFree(working_directory);
  }

  // Process children nodes
  xmlNodePtr aCurSubNode = machine_descr->xmlChildrenNode;
  while(aCurSubNode != NULL)
  {
    // Process components
    if ( !xmlStrcmp(aCurSubNode->name, (const xmlChar*)test_components) )
    {
      //If a component is given, it is in a module with the same name
      //except if the module name is given
      if (xmlHasProp(aCurSubNode, (const xmlChar*)test_component_name)) 
      {
        xmlChar* component_name = xmlGetProp(aCurSubNode, (const xmlChar*)test_component_name);
        std::string aComponentName = (const char*)component_name;
        resource.ComponentsList.push_back(aComponentName);
        if (xmlHasProp(aCurSubNode, (const xmlChar*)test_module_name)) 
        {
          xmlChar* module_name = xmlGetProp(aCurSubNode, (const xmlChar*)test_module_name);
          std::string aModuleName = (const char*)module_name;
          resource.ModulesList.push_back(aModuleName);
          xmlFree(module_name);
        }
        else
          resource.ModulesList.push_back(aComponentName);
        xmlFree(component_name);
      }
    }
    // Process modules
    else if ( !xmlStrcmp(aCurSubNode->name, (const xmlChar*)test_modules) )
    {
      // If a module is given, we create an entry in componentsList and modulesList
      // with the same name (module == component)
      if (xmlHasProp(aCurSubNode, (const xmlChar*)test_module_name)) 
      {
        xmlChar* component_name = xmlGetProp(aCurSubNode, (const xmlChar*)test_module_name);
        std::string aComponentName = (const char*)component_name;
        resource.ComponentsList.push_back(aComponentName);
        resource.ModulesList.push_back(aComponentName);
        xmlFree(component_name);
      }
    }
    aCurSubNode = aCurSubNode->next;
  }
  return true;
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

  root_node = xmlNewNode(NULL, BAD_CAST "resources");
  xmlDocSetRootElement(theDoc, root_node);
    
  std::map<std::string, ParserResourcesType>::iterator iter = _resources_list.begin();
  for (; iter != _resources_list.end(); iter++)
  {
    node = xmlNewChild(root_node, NULL, BAD_CAST test_machine, NULL);
    RES_MESSAGE("Add resource name = " << (*iter).second.Name.c_str());
    xmlNewProp(node, BAD_CAST test_name, BAD_CAST (*iter).second.Name.c_str());
    xmlNewProp(node, BAD_CAST test_hostname, BAD_CAST (*iter).second.HostName.c_str());
    xmlNewProp(node, BAD_CAST test_type, BAD_CAST (*iter).second.getResourceTypeStr().c_str());
    xmlNewProp(node, BAD_CAST test_appli_path, BAD_CAST (*iter).second.AppliPath.c_str());
    xmlNewProp(node, BAD_CAST test_batch_queue, BAD_CAST (*iter).second.batchQueue.c_str());
    xmlNewProp(node, BAD_CAST test_user_commands, BAD_CAST (*iter).second.userCommands.c_str());
    xmlNewProp(node, BAD_CAST test_protocol, BAD_CAST (*iter).second.getAccessProtocolTypeStr().c_str());
    xmlNewProp(node, BAD_CAST test_cluster_internal_protocol,
               BAD_CAST (*iter).second.getClusterInternalProtocolStr().c_str());
    xmlNewProp(node, BAD_CAST test_working_directory, BAD_CAST (*iter).second.working_directory.c_str());
    xmlNewProp(node, BAD_CAST test_can_launch_batch_jobs,
               BAD_CAST (*iter).second.getCanLaunchBatchJobsStr().c_str());
    xmlNewProp(node, BAD_CAST test_can_run_containers,
               BAD_CAST (*iter).second.getCanRunContainersStr().c_str());
    xmlNewProp(node, BAD_CAST test_batch, BAD_CAST (*iter).second.getBatchTypeStr().c_str());
    xmlNewProp(node, BAD_CAST test_mpi, BAD_CAST (*iter).second.getMpiImplTypeStr().c_str());
    xmlNewProp(node, BAD_CAST test_user_name, BAD_CAST (*iter).second.UserName.c_str());

    std::vector<std::string>::const_iterator iter2 = (*iter).second.ComponentsList.begin();
    for(;iter2 != (*iter).second.ComponentsList.end(); iter2++)
    {
      node1 = xmlNewChild(node, NULL, BAD_CAST test_components, NULL);
      xmlNewProp(node1, BAD_CAST test_component_name, BAD_CAST (*iter2).c_str());
    }

    xmlNewProp(node, BAD_CAST test_os, BAD_CAST (*iter).second.OS.c_str());
    std::ostringstream mem_stream;
    mem_stream << (*iter).second.DataForSort._memInMB;
    xmlNewProp(node, BAD_CAST test_mem_in_mb, BAD_CAST mem_stream.str().c_str());
    std::ostringstream cpu_stream;
    cpu_stream << (*iter).second.DataForSort._CPUFreqMHz;
    xmlNewProp(node, BAD_CAST test_cpu_freq_mhz, BAD_CAST cpu_stream.str().c_str());
    std::ostringstream nb_nodes_stream;
    nb_nodes_stream << (*iter).second.DataForSort._nbOfNodes;
    xmlNewProp(node, BAD_CAST test_nb_of_nodes, BAD_CAST nb_nodes_stream.str().c_str());
    std::ostringstream nb_proc_per_nodes_stream;
    nb_proc_per_nodes_stream << (*iter).second.DataForSort._nbOfProcPerNode;
    xmlNewProp(node, BAD_CAST test_nb_of_proc_per_node, BAD_CAST nb_proc_per_nodes_stream.str().c_str());
  }
}
