// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
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
#ifndef WNT
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
  Engines::SalomeLauncher_var refContMan =
    Engines::SalomeLauncher::_narrow(obj);

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
  //_remove_ref();
}

//=============================================================================
/*!
 *  get the list of name of ressources fitting for the specified module.
 *  If hostname specified, check it is local or known in resources catalog.
 *
 *  Else
 *  - select first machines with corresponding OS (all machines if
 *    parameter OS empty),
 *  - then select the sublist of machines on witch the module is known
 *    (if the result is empty, that probably means that the inventory of
 *    modules is probably not done, so give complete list from previous step)
 */ 
//=============================================================================

Engines::MachineList *
SALOME_ResourcesManager::GetFittingResources(const Engines::MachineParameters& params,
					     const Engines::CompoList& componentList)
{
//   MESSAGE("ResourcesManager::GetFittingResources");
  machineParams p;
  p.hostname = params.hostname;
  p.OS = params.OS;
  p.nb_node = params.nb_node;
  p.nb_proc_per_node = params.nb_proc_per_node;
  p.cpu_clock = params.cpu_clock;
  p.mem_mb = params.mem_mb;

  vector<string> cl;
  for(int i=0;i<componentList.length();i++)
    cl.push_back(string(componentList[i]));
  
  Engines::MachineList *ret=new Engines::MachineList;
  try{
      vector <std::string> vec = _rm.GetFittingResources(p,cl);
      ret->length(vec.size());
      for(int i=0;i<vec.size();i++)
	(*ret)[i] = (vec[i]).c_str();
  }
  catch(const ResourcesException &ex){
    INFOS("Caught exception.");
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
SALOME_ResourcesManager::FindFirst(const Engines::MachineList& listOfMachines)
{
  vector<string> ml;
  for(int i=0;i<listOfMachines.length();i++)
    ml.push_back(string(listOfMachines[i]));

  return CORBA::string_dup(_rm.FindFirst(ml).c_str());
}

Engines::MachineParameters* SALOME_ResourcesManager::GetMachineParameters(const char *hostname)
{
  ParserResourcesType resource = _rm.GetResourcesList(string(hostname));
  Engines::MachineParameters *p_ptr = new Engines::MachineParameters;
  p_ptr->container_name = CORBA::string_dup("");
  p_ptr->hostname = CORBA::string_dup(resource.HostName.c_str());
  p_ptr->alias = CORBA::string_dup(resource.Alias.c_str());
  if( resource.Protocol == rsh )
    p_ptr->protocol = "rsh";
  else if( resource.Protocol == ssh )
    p_ptr->protocol = "ssh";
  p_ptr->username = CORBA::string_dup(resource.UserName.c_str());
  p_ptr->applipath = CORBA::string_dup(resource.AppliPath.c_str());
  p_ptr->modList.length(resource.ModulesList.size());
  for(int i=0;i<resource.ModulesList.size();i++)
    p_ptr->modList[i] = CORBA::string_dup(resource.ModulesList[i].c_str());
  p_ptr->OS = CORBA::string_dup(resource.OS.c_str());
  p_ptr->mem_mb = resource.DataForSort._memInMB;
  p_ptr->cpu_clock = resource.DataForSort._CPUFreqMHz;
  p_ptr->nb_proc_per_node = resource.DataForSort._nbOfProcPerNode;
  p_ptr->nb_node = resource.DataForSort._nbOfNodes;
  if( resource.mpi == indif )
    p_ptr->mpiImpl = "indif";
  else if( resource.mpi == lam )
    p_ptr->mpiImpl = "lam";
  else if( resource.mpi == mpich1 )
    p_ptr->mpiImpl = "mpich1";
  else if( resource.mpi == mpich2 )
    p_ptr->mpiImpl = "mpich2";
  else if( resource.mpi == openmpi )
    p_ptr->mpiImpl = "openmpi";
  if( resource.Batch == pbs )
    p_ptr->batch = "pbs";
  else if( resource.Batch == lsf )
    p_ptr->batch = "lsf";
  else if( resource.Batch == slurm )
    p_ptr->batch = "slurm";

  return p_ptr;
}

