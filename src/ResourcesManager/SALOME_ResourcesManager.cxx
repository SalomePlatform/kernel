//  Copyright (C) 2007-2010  CEA/DEN, EDF R&D, OPEN CASCADE
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

#include "SALOME_ResourcesManager.hxx" 
#include "Utils_ExceptHandlers.hxx"
#include "Utils_CorbaException.hxx"
#include "OpUtil.hxx"

#include <stdlib.h>
#include <stdio.h>
#ifndef WIN32
#include <unistd.h>
#else
#include <io.h>
#include <process.h>
#endif
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <map>
#include <list>

#include <sys/types.h>
#include <sys/stat.h>
#include "utilities.h"

#define MAX_SIZE_FOR_HOSTNAME 256;

const char *SALOME_ResourcesManager::_ResourcesManagerNameInNS = "/ResourcesManager";

//=============================================================================
/*!
 * just for test
 */ 
//=============================================================================

SALOME_ResourcesManager::
SALOME_ResourcesManager(CORBA::ORB_ptr orb, 
                        PortableServer::POA_var poa, 
                        SALOME_NamingService *ns,
                        const char *xmlFilePath) : _rm(xmlFilePath)
{
  MESSAGE("SALOME_ResourcesManager constructor");
  _NS = ns;
  _orb = CORBA::ORB::_duplicate(orb) ;
  _poa = PortableServer::POA::_duplicate(poa) ;
  PortableServer::ObjectId_var id = _poa->activate_object(this);
  CORBA::Object_var obj = _poa->id_to_reference(id);
  Engines::ResourcesManager_var refContMan = Engines::ResourcesManager::_narrow(obj);
  _NS->Register(refContMan,_ResourcesManagerNameInNS);
  MESSAGE("SALOME_ResourcesManager constructor end");
}

//=============================================================================
/*!
 *  Standard constructor, parse resource file.
 *  - if ${APPLI} exists in environment,
 *    look for ${HOME}/${APPLI}/CatalogResources.xml
 *  - else look for default:
 *    ${KERNEL_ROOT_DIR}/share/salome/resources/kernel/CatalogResources.xml
 *  - parse XML resource file.
 */ 
//=============================================================================

SALOME_ResourcesManager::SALOME_ResourcesManager(CORBA::ORB_ptr orb, 
                                                 PortableServer::POA_var poa, 
                                                 SALOME_NamingService *ns) : _rm()
{
  MESSAGE("SALOME_ResourcesManager constructor");
  _NS = ns;
  _orb = CORBA::ORB::_duplicate(orb) ;
  _poa = PortableServer::POA::_duplicate(poa) ;
  PortableServer::ObjectId_var id = _poa->activate_object(this);
  CORBA::Object_var obj = _poa->id_to_reference(id);
  Engines::ResourcesManager_var refContMan = Engines::ResourcesManager::_narrow(obj);
  _NS->Register(refContMan,_ResourcesManagerNameInNS);

  MESSAGE("SALOME_ResourcesManager constructor end");
}

//=============================================================================
/*!
 *  Standard Destructor
 */ 
//=============================================================================

SALOME_ResourcesManager::~SALOME_ResourcesManager()
{
  MESSAGE("SALOME_ResourcesManager destructor");
}


//=============================================================================
/*! CORBA method:
 *  shutdown all the containers, then the ContainerManager servant
 */
//=============================================================================

void SALOME_ResourcesManager::Shutdown()
{
  MESSAGE("Shutdown");
  _NS->Destroy_Name(_ResourcesManagerNameInNS);
  PortableServer::ObjectId_var oid = _poa->servant_to_id(this);
  _poa->deactivate_object(oid);
}

//=============================================================================
//! get the name of resources fitting the specified constraints (params)
/*!
 *  If hostname specified, check it is local or known in resources catalog.
 *
 *  Else
 *  - select first machines with corresponding OS (all machines if
 *    parameter OS empty),
 *  - then select the sublist of machines on which the component is known
 *    (if the result is empty, that probably means that the inventory of
 *    components is probably not done, so give complete list from previous step)
 */ 
//=============================================================================

Engines::ResourceList *
SALOME_ResourcesManager::GetFittingResources(const Engines::ResourceParameters& params)
{
  MESSAGE("ResourcesManager::GetFittingResources");
  Engines::ResourceList * ret = new Engines::ResourceList;

  // CORBA -> C++
  resourceParams p;
  p.name = params.name;
  p.hostname = params.hostname;
  p.OS = params.OS;
  p.nb_proc = params.nb_proc;
  p.nb_node = params.nb_node;
  p.nb_proc_per_node = params.nb_proc_per_node;
  p.cpu_clock = params.cpu_clock;
  p.mem_mb = params.mem_mb;
  for(unsigned int i=0; i<params.componentList.length(); i++)
    p.componentList.push_back(std::string(params.componentList[i]));
  for(unsigned int i=0; i<params.resList.length(); i++)
    p.resourceList.push_back(std::string(params.resList[i]));
  
  try
  {
    // Call C++ ResourceManager
    std::vector <std::string> vec = _rm.GetFittingResources(p);

    // C++ -> CORBA
    ret->length(vec.size());
    for(unsigned int i=0;i<vec.size();i++)
      (*ret)[i] = (vec[i]).c_str();
  }
  catch(const ResourcesException &ex)
  {
    INFOS("Caught exception in GetFittingResources C++:  " << ex.msg);
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(),SALOME::BAD_PARAM);
  }  

  return ret;
}

//=============================================================================
/*!
 *  dynamically obtains the first machines
 */ 
//=============================================================================

char *
SALOME_ResourcesManager::FindFirst(const Engines::ResourceList& listOfResources)
{
  // CORBA -> C++
  std::vector<std::string> rl;
  for(unsigned int i=0; i<listOfResources.length(); i++)
    rl.push_back(std::string(listOfResources[i]));

  return CORBA::string_dup(_rm.Find("first", rl).c_str());
}

char *
SALOME_ResourcesManager::Find(const char* policy, const Engines::ResourceList& listOfResources)
{
  // CORBA -> C++
  std::vector<std::string> rl;
  for(unsigned int i=0; i<listOfResources.length(); i++)
    rl.push_back(std::string(listOfResources[i]));

  return CORBA::string_dup(_rm.Find(policy, rl).c_str());
}

Engines::ResourceDefinition* 
SALOME_ResourcesManager::GetResourceDefinition(const char * name)
{
  ParserResourcesType resource = _rm.GetResourcesDescr(name);
  Engines::ResourceDefinition *p_ptr = new Engines::ResourceDefinition;

  p_ptr->name = CORBA::string_dup(resource.Name.c_str());
  p_ptr->hostname = CORBA::string_dup(resource.HostName.c_str());
  if( resource.Protocol == rsh )
    p_ptr->protocol = "rsh";
  else if( resource.Protocol == ssh )
    p_ptr->protocol = "ssh";
  if( resource.ClusterInternalProtocol == rsh )
    p_ptr->iprotocol = "rsh";
  else if( resource.ClusterInternalProtocol == ssh )
    p_ptr->iprotocol = "ssh";
  p_ptr->username = CORBA::string_dup(resource.UserName.c_str());
  p_ptr->applipath = CORBA::string_dup(resource.AppliPath.c_str());
  p_ptr->componentList.length(resource.ComponentsList.size());
  for(unsigned int i=0;i<resource.ComponentsList.size();i++)
    p_ptr->componentList[i] = CORBA::string_dup(resource.ComponentsList[i].c_str());
  p_ptr->OS = CORBA::string_dup(resource.OS.c_str());
  p_ptr->mem_mb = resource.DataForSort._memInMB;
  p_ptr->cpu_clock = resource.DataForSort._CPUFreqMHz;
  p_ptr->nb_proc_per_node = resource.DataForSort._nbOfProcPerNode;
  p_ptr->nb_node = resource.DataForSort._nbOfNodes;

  if( resource.mpi == lam )
    p_ptr->mpiImpl = "lam";
  else if( resource.mpi == mpich1 )
    p_ptr->mpiImpl = "mpich1";
  else if( resource.mpi == mpich2 )
    p_ptr->mpiImpl = "mpich2";
  else if( resource.mpi == openmpi )
    p_ptr->mpiImpl = "openmpi";
  else if( resource.mpi == slurm )
    p_ptr->mpiImpl = "slurm";
  else if( resource.mpi == prun )
    p_ptr->mpiImpl = "prun";

  if( resource.Batch == pbs )
    p_ptr->batch = "pbs";
  else if( resource.Batch == lsf )
    p_ptr->batch = "lsf";
  else if( resource.Batch == sge )
    p_ptr->batch = "sge";
  else if( resource.Batch == ccc )
    p_ptr->batch = "ccc";
  else if( resource.Batch == ssh_batch )
    p_ptr->batch = "ssh";

  return p_ptr;
}

void 
SALOME_ResourcesManager::AddResource(const Engines::ResourceDefinition& new_resource,
				     CORBA::Boolean write,
				     const char * xml_file)
{
  ParserResourcesType resource;
  resource.Name = new_resource.name.in();
  resource.HostName = new_resource.hostname.in();
  resource.OS = new_resource.OS.in();
  resource.AppliPath = new_resource.applipath.in();
  resource.DataForSort._memInMB = new_resource.mem_mb;
  resource.DataForSort._CPUFreqMHz = new_resource.cpu_clock;
  resource.DataForSort._nbOfNodes = new_resource.nb_node;
  resource.DataForSort._nbOfProcPerNode = new_resource.nb_proc_per_node;
  resource.UserName = new_resource.username.in();

  std::string aBatch = new_resource.batch.in();
  if (aBatch == "pbs")
    resource.Batch = pbs;
  else if  (aBatch == "lsf")
    resource.Batch = lsf;
  else if  (aBatch == "sge")
    resource.Batch = sge;
  else if  (aBatch == "ccc")
    resource.Batch = ccc;
  else if  (aBatch == "ssh_batch")
    resource.Batch = ssh_batch;
  else if (aBatch == "")
    resource.Batch = none;
  else {
    INFOS("Bad Batch definition in AddResource: " << aBatch);
    std::string message("Bad Batch definition in AddResource: ");
    message += aBatch;
    THROW_SALOME_CORBA_EXCEPTION(message.c_str(),SALOME::BAD_PARAM);
  }

  std::string anMpi = new_resource.mpiImpl.in();
  if (anMpi == "lam")
    resource.mpi = lam;
  else if (anMpi == "mpich1")
    resource.mpi = mpich1;
  else if (anMpi == "mpich2")
    resource.mpi = mpich2;
  else if (anMpi == "openmpi")
    resource.mpi = openmpi;
  else if  (anMpi == "slurm")
    resource.mpi = slurm;
  else if  (anMpi == "prun")
    resource.mpi = prun;
  else if (anMpi == "")
    resource.mpi = nompi;
  else {
    INFOS("Bad MPI definition in AddResource: " << anMpi);
    std::string message("Bad MPI definition in AddResource: ");
    message += anMpi;
    THROW_SALOME_CORBA_EXCEPTION(message.c_str(),SALOME::BAD_PARAM);
  }

  std::string mode_str = new_resource.mode.in();
  if (mode_str == "interactive")
    resource.Mode = interactive;
  else if (mode_str == "batch")
    resource.Mode = batch;
  else if (mode_str == "")
    resource.Mode = interactive;
  else {
    INFOS("Bad mode definition in AddResource: " << mode_str);
    std::string message("Bad mode definition in AddResource: ");
    message += mode_str;
    THROW_SALOME_CORBA_EXCEPTION(message.c_str(),SALOME::BAD_PARAM);
  }
  
  std::string protocol = new_resource.protocol.in();
  if (protocol == "rsh")
    resource.Protocol = rsh;
  else if (protocol == "ssh")
    resource.Protocol = ssh;
  else if (protocol == "")
    resource.Protocol = rsh;
  else {
    INFOS("Bad protocol definition in AddResource: " << protocol);
    std::string message("Bad protocol definition in AddResource: ");
    message += protocol;
    THROW_SALOME_CORBA_EXCEPTION(message.c_str(),SALOME::BAD_PARAM);
  }

  std::string iprotocol = new_resource.iprotocol.in();
  if (iprotocol == "rsh")
    resource.ClusterInternalProtocol = rsh;
  else if (iprotocol == "ssh")
    resource.ClusterInternalProtocol = ssh;
  else if (iprotocol == "")
    resource.ClusterInternalProtocol = rsh;
  else {
    INFOS("Bad iprotocol definition in AddResource: " << iprotocol);
    std::string message("Bad iprotocol definition in AddResource: ");
    message += iprotocol;
    THROW_SALOME_CORBA_EXCEPTION(message.c_str(),SALOME::BAD_PARAM);
  }

  for (CORBA::ULong i = 0; i < new_resource.componentList.length(); i++)
    resource.ComponentsList.push_back(new_resource.componentList[i].in());

  _rm.AddResourceInCatalog(resource);

  if (write)
  {
    _rm.WriteInXmlFile(std::string(xml_file));
    _rm.ParseXmlFiles();
  }
}

void 
SALOME_ResourcesManager::RemoveResource(const char * resource_name,
					CORBA::Boolean write,
					const char * xml_file)
{
  _rm.DeleteResourceInCatalog(resource_name);
  if (write)
  {
    _rm.WriteInXmlFile(std::string(xml_file));
    _rm.ParseXmlFiles();
  }
}

std::string 
SALOME_ResourcesManager::getMachineFile(std::string resource_name, 
                                        CORBA::Long nb_procs, 
                                        std::string parallelLib)
{
  std::string machine_file_name("");

  if (parallelLib == "Dummy")
  {
    MESSAGE("[getMachineFile] parallelLib is Dummy");
    MapOfParserResourcesType resourcesList = _rm.GetList();
    if (resourcesList.find(resource_name) != resourcesList.end())
    {
      ParserResourcesType resource = resourcesList[resource_name];

      // Check if resource is cluster or not
      if (resource.ClusterMembersList.empty())
      {
        //It is not a cluster so we create a cluster with one machine
        ParserResourcesClusterMembersType fake_node;
        fake_node.HostName = resource.HostName;
        fake_node.Protocol = resource.Protocol;
        fake_node.ClusterInternalProtocol = resource.ClusterInternalProtocol;
        fake_node.UserName = resource.UserName;
        fake_node.AppliPath = resource.AppliPath;
        fake_node.DataForSort = resource.DataForSort;

        resource.ClusterMembersList.push_front(fake_node);
      }

      // Creating list of machines for creating the machine file
      std::list<std::string> list_of_machines;
      std::list<ParserResourcesClusterMembersType>::iterator cluster_it = 
        resource.ClusterMembersList.begin();
      while (cluster_it != resource.ClusterMembersList.end())
      {
        // For each member of the cluster we add a nbOfNodes * nbOfProcPerNode in the list
        unsigned int number_of_proc = (*cluster_it).DataForSort._nbOfNodes * 
                                      (*cluster_it).DataForSort._nbOfProcPerNode;
        for (unsigned int i = 0; i < number_of_proc; i++)
          list_of_machines.push_back((*cluster_it).HostName);
        cluster_it++;
      }

      // Creating machine file
      machine_file_name = tmpnam(NULL);
	  std::ofstream machine_file(machine_file_name.c_str(), std::ios_base::out);

      CORBA::Long machine_number = 0;
      std::list<std::string>::iterator it = list_of_machines.begin();
      while (machine_number != nb_procs)
      {
        // Adding a new node to the machine file
        machine_file << *it << std::endl;

        // counting...
        it++;
        if (it == list_of_machines.end())
          it = list_of_machines.begin();
        machine_number++;
      }
    }
    else
      INFOS("[getMachineFile] Error resource_name not found in resourcesList -> " << resource_name);
  }
  else if (parallelLib == "Mpi")
  {
    MESSAGE("[getMachineFile] parallelLib is Mpi");

    MapOfParserResourcesType resourcesList = _rm.GetList();
    if (resourcesList.find(resource_name) != resourcesList.end())
    {
      ParserResourcesType resource = resourcesList[resource_name];
      // Check if resource is cluster or not
      if (resource.ClusterMembersList.empty())
      {
        //It is not a cluster so we create a cluster with one machine
        ParserResourcesClusterMembersType fake_node;
        fake_node.HostName = resource.HostName;
        fake_node.Protocol = resource.Protocol;
        fake_node.ClusterInternalProtocol = resource.ClusterInternalProtocol;
        fake_node.UserName = resource.UserName;
        fake_node.AppliPath = resource.AppliPath;
        fake_node.DataForSort = resource.DataForSort;

        resource.ClusterMembersList.push_front(fake_node);
      }

      // Choose mpi implementation -> each MPI implementation has is own machinefile...
      if (resource.mpi == lam)
      {
        // Creating machine file
        machine_file_name = tmpnam(NULL);
        std::ofstream machine_file(machine_file_name.c_str(), std::ios_base::out);

        // We add all cluster machines to the file
        std::list<ParserResourcesClusterMembersType>::iterator cluster_it = 
          resource.ClusterMembersList.begin();
        while (cluster_it != resource.ClusterMembersList.end())
        {
          unsigned int number_of_proc = (*cluster_it).DataForSort._nbOfNodes * 
            (*cluster_it).DataForSort._nbOfProcPerNode;
          machine_file << (*cluster_it).HostName << " cpu=" << number_of_proc << std::endl;
          cluster_it++;
        }
      }
      else if (resource.mpi == openmpi)
      {
	// Creating machine file
	machine_file_name = tmpnam(NULL);
	std::ofstream machine_file(machine_file_name.c_str(), std::ios_base::out);

	// We add all cluster machines to the file
	std::list<ParserResourcesClusterMembersType>::iterator cluster_it = 
	  resource.ClusterMembersList.begin();
	while (cluster_it != resource.ClusterMembersList.end())
	{
	  unsigned int number_of_proc = (*cluster_it).DataForSort._nbOfNodes * 
	    (*cluster_it).DataForSort._nbOfProcPerNode;
	  machine_file << (*cluster_it).HostName << " slots=" << number_of_proc << std::endl;
	  cluster_it++;
	}
      }
      else if (resource.mpi == nompi)
      {
        INFOS("[getMachineFile] Error resource_name MPI implementation was defined for " << resource_name);
      }
      else
        INFOS("[getMachineFile] Error resource_name MPI implementation not currenly handled for " << resource_name);
    }
    else
      INFOS("[getMachineFile] Error resource_name not found in resourcesList -> " << resource_name);
  }
  else
    INFOS("[getMachineFile] Error parallelLib is not handled -> " << parallelLib);

  return machine_file_name;
}
