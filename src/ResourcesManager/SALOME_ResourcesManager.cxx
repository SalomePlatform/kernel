// Copyright (C) 2007-2025  CEA, EDF, OPEN CASCADE
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

using namespace std;

const char *SALOME_ResourcesManager::_ResourcesManagerNameInNS = "/ResourcesManager";

//=============================================================================
/*!
 * just for test
 */ 
//=============================================================================

SALOME_ResourcesManager::SALOME_ResourcesManager(CORBA::ORB_ptr orb,
                                                 PortableServer::POA_var poa,
                                                 SALOME_NamingService_Abstract *ns,
                                                 const char *xmlFilePath)
: _rm(new ResourcesManager_cpp(xmlFilePath))
{
  MESSAGE("SALOME_ResourcesManager constructor");
  _NS = ns;
  _orb = CORBA::ORB::_duplicate(orb) ;
  //
  PortableServer::POAManager_var pman = poa->the_POAManager();
  CORBA::PolicyList policies;
  policies.length(1);
  PortableServer::ThreadPolicy_var threadPol(poa->create_thread_policy(PortableServer::SINGLE_THREAD_MODEL));
  policies[0] = PortableServer::ThreadPolicy::_duplicate(threadPol);
  _poa = poa->create_POA("SingleThreadPOA",pman,policies);
  threadPol->destroy();
  //
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
                                                 SALOME_NamingService_Abstract *ns) : _rm(new ResourcesManager_cpp())
{
  _NS = ns;
  _orb = CORBA::ORB::_duplicate(orb) ;
  //
  PortableServer::POAManager_var pman = poa->the_POAManager();
  CORBA::PolicyList policies;
  policies.length(1);
  PortableServer::ThreadPolicy_var threadPol(poa->create_thread_policy(PortableServer::SINGLE_THREAD_MODEL));
  policies[0] = PortableServer::ThreadPolicy::_duplicate(threadPol);
  _poa = poa->create_POA("SingleThreadPOA",pman,policies);
  threadPol->destroy();
  //
  PortableServer::ObjectId_var id = _poa->activate_object(this);
  CORBA::Object_var obj = _poa->id_to_reference(id);
  Engines::ResourcesManager_var refContMan = Engines::ResourcesManager::_narrow(obj);
  if(_NS)
    _NS->Register(refContMan,_ResourcesManagerNameInNS);
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
  if(!_NS)
    return ;
  _NS->Destroy_Name(_ResourcesManagerNameInNS);
  PortableServer::ObjectId_var oid = _poa->servant_to_id(this);
  _poa->deactivate_object(oid);
}

/*!
 * Return list of resources available (regarding content of CatalogResources.xml) but select only those with canRunContainers attribute set to true.
 * And for each resource the number of proc available of it.
 * 
 * \sa SALOME_ResourcesManager::ListAllResourcesInCatalog, SALOME_ResourcesManager::ListAllResourceEntriesInCatalog
 */
void SALOME_ResourcesManager::ListAllAvailableResources(Engines::ResourceList_out machines, Engines::IntegerList_out nbProcsOfMachines)
{
  const MapOfParserResourcesType& zeList(_rm->GetList());
  std::vector<std::string> ret0;
  std::vector<int> ret1;
  for(MapOfParserResourcesType::const_iterator it=zeList.begin();it!=zeList.end();it++)
  {
    const ParserResourcesType& elt((*it).second);
    if(elt.can_run_containers)
    {
      ret0.push_back(elt.HostName);
      ret1.push_back(elt.DataForSort._nbOfNodes*elt.DataForSort._nbOfProcPerNode);
    }
  }
  machines=new Engines::ResourceList;
  nbProcsOfMachines=new Engines::IntegerList;
  std::size_t sz(ret0.size());
  machines->length((CORBA::ULong)sz); nbProcsOfMachines->length((CORBA::ULong)sz);
  for(std::size_t j=0;j<sz;j++)
    {
      (*machines)[(CORBA::ULong)j]=CORBA::string_dup(ret0[j].c_str());
      (*nbProcsOfMachines)[(CORBA::ULong)j]=ret1[j];
    }
}

/*!
 * Return list of resources available (regarding content of CatalogResources.xml) whatever canRunContainers attribute value.
 * 
 * \sa SALOME_ResourcesManager::ListAllAvailableResources, SALOME_ResourcesManager::ListAllResourceEntriesInCatalog
 */
Engines::ResourceList *SALOME_ResourcesManager::ListAllResourcesInCatalog()
{
  const MapOfParserResourcesType& zeList(_rm->GetList());
  auto sz = zeList.size();
  Engines::ResourceList *ret(new Engines::ResourceList);
  ret->length( sz );
  CORBA::ULong i(0);
  for(auto it : zeList)
  {
    (*ret)[i++] = CORBA::string_dup( it.second.HostName.c_str() );
  }
  return ret;
}

/*!
 * Return list of resources entries available. Useful to scan remotely the content of the playground
 */
Engines::ResourceList *SALOME_ResourcesManager::ListAllResourceEntriesInCatalog()
{
  const MapOfParserResourcesType& zeList(_rm->GetList());
  auto sz = zeList.size();
  Engines::ResourceList *ret(new Engines::ResourceList);
  ret->length( sz );
  CORBA::ULong i(0);
  for(auto it : zeList)
  {
    (*ret)[i++] = CORBA::string_dup( it.first.c_str() );
  }
  return ret;
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
  //MESSAGE("ResourcesManager::GetFittingResources");
  Engines::ResourceList_var ret;

  // CORBA -> C++
  resourceParams p = resourceParameters_CORBAtoCPP(params);

  try
  {
    // Call C++ ResourceManager
    std::vector <std::string> vec = _rm->GetFittingResources(p);

    // C++ -> CORBA
    ret = resourceList_CPPtoCORBA(vec);
  }
  catch(const ResourcesException &ex)
  {
    INFOS("Caught exception in GetFittingResources C++:  " << ex.msg);
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(),SALOME_CMOD::BAD_PARAM);
  }  

  return ret._retn();
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
  std::vector<std::string> rl = resourceList_CORBAtoCPP(listOfResources);

  return CORBA::string_dup(_rm->Find("first", rl).c_str());
}

char *
SALOME_ResourcesManager::Find(const char* policy, const Engines::ResourceList& listOfResources)
{
  // CORBA -> C++
  std::vector<std::string> rl = resourceList_CORBAtoCPP(listOfResources);

  return CORBA::string_dup(_rm->Find(policy, rl).c_str());
}

Engines::ResourceDefinition*
SALOME_ResourcesManager::GetResourceDefinition(const char * name)
{
  Engines::ResourceDefinition_var resDef;
  try {
    ParserResourcesType resource = _rm->GetResourcesDescr(name);
    resDef = resourceDefinition_CPPtoCORBA(resource);
  } catch (const exception & ex) {
    INFOS("Caught exception in GetResourceDefinition: " << ex.what());
    THROW_SALOME_CORBA_EXCEPTION(ex.what(), SALOME_CMOD::BAD_PARAM);
  }

  return resDef._retn();
}

void 
SALOME_ResourcesManager::AddResource(const Engines::ResourceDefinition& new_resource,
                                     CORBA::Boolean write,
                                     const char * xml_file)
{
  try
  {
    ParserResourcesType resource = resourceDefinition_CORBAtoCPP(new_resource);
    _rm->AddResourceInCatalog(resource);

    if (write)
    {
      _rm->WriteInXmlFile(std::string(xml_file));
      _rm->ParseXmlFiles();
    }
  }
  catch (const SALOME_Exception & e)
  {
    INFOS("Error in AddResourceInCatalog: " << e);
    THROW_SALOME_CORBA_EXCEPTION(e.what(), SALOME_CMOD::BAD_PARAM);
  }
  catch (const ResourcesException & e)
  {
    INFOS("Error in AddResourceInCatalog: " << e.msg);
    THROW_SALOME_CORBA_EXCEPTION(e.msg.c_str(), SALOME_CMOD::BAD_PARAM);
  }
}

void
SALOME_ResourcesManager::RemoveResource(const char * resource_name,
                                        CORBA::Boolean write,
                                        const char * xml_file)
{
  try
  {
    _rm->DeleteResourceInCatalog(resource_name);
  }
  catch (const SALOME_Exception & e)
  {
    INFOS("Error in DeleteResourceInCatalog: " << e);
    THROW_SALOME_CORBA_EXCEPTION(e.what(), SALOME_CMOD::BAD_PARAM);
  }

  if (write)
  {
    _rm->WriteInXmlFile(std::string(xml_file));
    _rm->ParseXmlFiles();
  }
}

char *
SALOME_ResourcesManager::getMachineFile(const char * resource_name,
                                        CORBA::Long nb_procs, 
                                        const char * parallelLib)
{
  std::string machine_file_name("");

  if (std::string(parallelLib) == "Dummy")
  {
    MESSAGE("[getMachineFile] parallelLib is Dummy");
    MapOfParserResourcesType resourcesList = _rm->GetList();
    if (resourcesList.find(std::string(resource_name)) != resourcesList.end())
    {
      ParserResourcesType resource = resourcesList[std::string(resource_name)];

      // Check if resource is cluster or not
      if (resource.ClusterMembersList.empty())
      {
        //It is not a cluster so we create a cluster with one machine
        ParserResourcesType fake_node;
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
      std::list<ParserResourcesType>::iterator cluster_it =
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
  else if (std::string(parallelLib) == "Mpi")
  {
    MESSAGE("[getMachineFile] parallelLib is Mpi");

    MapOfParserResourcesType resourcesList = _rm->GetList();
    if (resourcesList.find(std::string(resource_name)) != resourcesList.end())
    {
      ParserResourcesType resource = resourcesList[std::string(resource_name)];
      // Check if resource is cluster or not
      if (resource.ClusterMembersList.empty())
      {
        //It is not a cluster so we create a cluster with one machine
        ParserResourcesType fake_node;
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
        std::list<ParserResourcesType>::iterator cluster_it =
          resource.ClusterMembersList.begin();
        while (cluster_it != resource.ClusterMembersList.end())
        {
          unsigned int number_of_proc = (*cluster_it).DataForSort._nbOfNodes * 
            (*cluster_it).DataForSort._nbOfProcPerNode;
          machine_file << (*cluster_it).HostName << " cpu=" << number_of_proc << std::endl;
          cluster_it++;
        }
      }
      else if ((resource.mpi == openmpi) || (resource.mpi == ompi))
      {
        // Creating machine file
        machine_file_name = tmpnam(NULL);
        std::ofstream machine_file(machine_file_name.c_str(), std::ios_base::out);

        // We add all cluster machines to the file
        std::list<ParserResourcesType>::iterator cluster_it =
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
        INFOS("[getMachineFile] Error resource_name MPI implementation not currently handled for " << resource_name);
    }
    else
      INFOS("[getMachineFile] Error resource_name not found in resourcesList -> " << resource_name);
  }
  else
    INFOS("[getMachineFile] Error parallelLib is not handled -> " << parallelLib);

  return CORBA::string_dup(machine_file_name.c_str());
}
