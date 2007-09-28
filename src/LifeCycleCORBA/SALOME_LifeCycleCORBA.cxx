//  SALOME LifeCycleCORBA : implementation of containers and engines life cycle both in Python and C++
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
//  File   : SALOME_LifeCycleCORBA.cxx
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <time.h>
#ifndef WNT
  #include <sys/time.h>
#endif

#include "OpUtil.hxx"
#include "utilities.h"

#include <ServiceUnreachable.hxx>

#include "SALOME_LifeCycleCORBA.hxx"
#ifndef WNT
#include CORBA_CLIENT_HEADER(SALOME_ModuleCatalog)
#else
#include "SALOME_ModuleCatalog.hh"
#endif
#include "SALOME_ContainerManager.hxx"
#include "SALOME_Component_i.hxx"
#include "SALOME_NamingService.hxx"

using namespace std;

IncompatibleComponent::IncompatibleComponent( void ):
  SALOME_Exception( "IncompatibleComponent" )
{
}

IncompatibleComponent::IncompatibleComponent(const IncompatibleComponent &ex):
  SALOME_Exception( ex ) 
{
}

//=============================================================================
/*! 
 *  Constructor
 */
//=============================================================================

SALOME_LifeCycleCORBA::SALOME_LifeCycleCORBA(SALOME_NamingService *ns)
{
  // be sure to have an instance of traceCollector, when used via SWIG
  // in a Python module
  int argc = 0;
  char *xargv = "";
  char **argv = &xargv;
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
  //  LocalTraceCollector *myThreadTrace = SALOMETraceCollector::instance(orb);
  if (!ns)
    {
      _NS = new SALOME_NamingService(orb);
    }
  else _NS = ns;
  //add try catch
  _NS->Change_Directory("/"); // mpv 250105: current directory may be not root 
                              // (in SALOMEDS for an example)
  // not enough: set a current directory in naming service is not thread safe
  // if naming service instance is shared among several threads...
  // ==> allways use absolute path and dot rely on current directory!

  CORBA::Object_var obj =
    _NS->Resolve(SALOME_ContainerManager::_ContainerManagerNameInNS);
  ASSERT( !CORBA::is_nil(obj));
  _ContManager=Engines::ContainerManager::_narrow(obj);
}

//=============================================================================
/*! 
 *  Destructor
 */
//=============================================================================

SALOME_LifeCycleCORBA::~SALOME_LifeCycleCORBA()
{
}

//=============================================================================
/*! Public - 
 *  Find and aready existing and registered component instance.
 *  \param params         machine parameters like type or name...
 *  \param componentName  the name of component class
 *  \param studyId        default = 0  : multistudy instance
 *  \return a CORBA reference of the component instance, or _nil if not found
 */
//=============================================================================

Engines::Component_ptr
SALOME_LifeCycleCORBA::FindComponent(const Engines::MachineParameters& params,
				     const char *componentName,
				     int studyId)
{
  if (! isKnownComponentClass(componentName))
    return Engines::Component::_nil();

  Engines::MachineList_var listOfMachines =
    _ContManager->GetFittingResources(params, componentName);

  Engines::Component_var compo = _FindComponent(params,
						componentName,
						studyId,
						listOfMachines);

  return compo._retn();
}

//=============================================================================
/*! Public - 
 *  Load a component instance on a container defined by machine parameters
 *  \param params         machine parameters like type or name...
 *  \param componentName  the name of component class
 *  \param studyId        default = 0  : multistudy instance
 *  \return a CORBA reference of the component instance, or _nil if problem
 */
//=============================================================================

Engines::Component_ptr
SALOME_LifeCycleCORBA::LoadComponent(const Engines::MachineParameters& params,
				     const char *componentName,
				     int studyId)
{
  // --- Check if Component Name is known in ModuleCatalog

  if (! isKnownComponentClass(componentName))
    return Engines::Component::_nil();

  Engines::MachineList_var listOfMachines =
    _ContManager->GetFittingResources(params, componentName);

  Engines::Component_var compo = _LoadComponent(params,
						componentName,
						studyId,
						listOfMachines);

  return compo._retn();
}

//=============================================================================
/*! Public - 
 *  Find and aready existing and registered component instance or load a new
 *  component instance on a container defined by machine parameters.
 *  \param params         machine parameters like type or name...
 *  \param componentName  the name of component class
 *  \param studyId        default = 0  : multistudy instance
 *  \return a CORBA reference of the component instance, or _nil if problem
 */
//=============================================================================

Engines::Component_ptr
SALOME_LifeCycleCORBA::
FindOrLoad_Component(const Engines::MachineParameters& params,
		     const char *componentName,
		     int studyId)
{
  // --- Check if Component Name is known in ModuleCatalog

  if (! isKnownComponentClass(componentName))
    return Engines::Component::_nil();

  Engines::MachineList_var listOfMachines =
    _ContManager->GetFittingResources(params,componentName);

  Engines::Component_var compo = _FindComponent(params,
						componentName,
						studyId,
						listOfMachines);

  if(CORBA::is_nil(compo))
    compo = _LoadComponent(params,
			   componentName,
			   studyId,
			   listOfMachines);

  return compo._retn();
}

//=============================================================================
/*! Public - 
 *  Find and aready existing and registered component instance or load a new
 *  component instance on a container defined by name
 *  \param containerName  the name of container, under one of the forms
 *           - 1 aContainer (local container)
 *           - 2 machine/aContainer (container on hostname = machine)
 *  \param componentName  the name of component class
 *  \return a CORBA reference of the component instance, or _nil if problem
 */
//=============================================================================

Engines::Component_ptr
SALOME_LifeCycleCORBA::FindOrLoad_Component(const char *containerName,
					    const char *componentName)
{
  // --- Check if Component Name is known in ModuleCatalog

  if (! isKnownComponentClass(componentName))
    return Engines::Component::_nil();

  // --- Check if containerName contains machine name (if yes: rg>0)

  char *stContainer=strdup(containerName);
  string st2Container(stContainer);
  int rg=st2Container.find("/");

  Engines::MachineParameters_var params=new Engines::MachineParameters;
  preSet(params);
  if (rg<0)
    {
      // containerName doesn't contain "/" => Local container
      params->container_name=CORBA::string_dup(stContainer);
      params->hostname=CORBA::string_dup(GetHostname().c_str());
    }
  else 
    {
      stContainer[rg]='\0';
      params->container_name=CORBA::string_dup(stContainer+rg+1);
      params->hostname=CORBA::string_dup(stContainer);
    }
  params->isMPI = false;
  SCRUTE(params->container_name);
//   SCRUTE(params->hostname);
//   SCRUTE(params->OS);
//   SCRUTE(params->mem_mb);
//   SCRUTE(params->cpu_clock);
//   SCRUTE(params->nb_proc_per_node);
//   SCRUTE(params->nb_node);
//   SCRUTE(params->isMPI);
  free(stContainer);
  return FindOrLoad_Component(params,componentName);
  
}

//=============================================================================
/*! Public -
 *  Check if the component class is known in module catalog
 *  \param componentName  the name of component class
 *  \return true if found, false otherwise
 */
//=============================================================================

bool SALOME_LifeCycleCORBA::isKnownComponentClass(const char *componentName)
{

  try
    {
      CORBA::Object_var obj = _NS->Resolve("/Kernel/ModulCatalog");
      SALOME_ModuleCatalog::ModuleCatalog_var Catalog = 
	SALOME_ModuleCatalog::ModuleCatalog::_narrow(obj) ;
      ASSERT(! CORBA::is_nil(Catalog));
      SALOME_ModuleCatalog::Acomponent_var compoInfo = 
	Catalog->GetComponent(componentName);
      if (CORBA::is_nil (compoInfo)) 
	{
	  INFOS("Catalog Error: Component not found in the catalog" );
          INFOS( componentName );	  
	  return false;
	}
      else return true;
    }
  catch (ServiceUnreachable&)
    {
      INFOS("Caught exception: Naming Service Unreachable");
    }
  catch (...)
    {
      INFOS("Caught unknown exception.");
    }
  return false;
}

//=============================================================================
/*! Public -
 *  Not so complex... useful ?
 */
//=============================================================================

bool 
SALOME_LifeCycleCORBA::isMpiContainer(const Engines::MachineParameters& params)
  throw(IncompatibleComponent)
{
  if( params.isMPI )
    return true;
  else
    return false;
}


//=============================================================================
/*! Public -
 *  Pre initialisation of a given Engines::MachineParameters with default
 *  values.
 *  - container_name = ""  : not relevant
 *  - hostname = ""        : not relevant
 *  - OS = ""              : not relevant
 *  - mem_mb = 0           : not relevant
 *  - cpu_clock = 0        : not relevant
 *  - nb_proc_per_node = 0 : not relevant
 *  - nb_node = 0          : not relevant
 *  - isMPI = false        : standard components
 */
//=============================================================================

void SALOME_LifeCycleCORBA::preSet( Engines::MachineParameters& params)
{
  params.container_name = "";
  params.hostname = "";
  params.OS = "";
  params.mem_mb = 0;
  params.cpu_clock = 0;
  params.nb_proc_per_node = 0;
  params.nb_node = 0;
  params.isMPI = false;

  params.parallelLib = "";
  params.nb_component_nodes = 0;
}

//=============================================================================
/*! Public -
 *  \return a number of processors not 0, only for MPI containers
 */
//=============================================================================

int SALOME_LifeCycleCORBA::NbProc(const Engines::MachineParameters& params)
{
  if( !isMpiContainer(params) )
    return 0;
  else if( (params.nb_node <= 0) && (params.nb_proc_per_node <= 0) )
    return 1;
  else if( params.nb_node == 0 )
    return params.nb_proc_per_node;
  else if( params.nb_proc_per_node == 0 )
    return params.nb_node;
  else
    return params.nb_node * params.nb_proc_per_node;
}

//=============================================================================
/*! Public -
 *  \return the container Manager
 */
//=============================================================================

Engines::ContainerManager_ptr SALOME_LifeCycleCORBA::getContainerManager()
{
 Engines::ContainerManager_var contManager =
   Engines::ContainerManager::_duplicate(_ContManager);
 return contManager._retn();
}


//=============================================================================
/*! Protected -
 *  Find and aready existing and registered component instance.
 *  \param params         machine parameters like type or name...
 *  \param componentName  the name of component class
 *  \param studyId        default = 0  : multistudy instance
 *  \param listOfMachines list of machine address
 *  \return a CORBA reference of the component instance, or _nil if not found
 * - build a list of machines on which an instance of the component is running,
 * - find the best machine among the list
 */
//=============================================================================

Engines::Component_ptr
SALOME_LifeCycleCORBA::
_FindComponent(const Engines::MachineParameters& params,
	       const char *componentName,
	       int studyId,
	       const Engines::MachineList& listOfMachines)
{
  // --- build the list of machines on which the component is already running

  const char *containerName = params.container_name;
  int nbproc = NbProc(params);
//   MESSAGE("_FindComponent, required " << containerName <<
// 	  " " << componentName << " " << nbproc);

  Engines::MachineList_var machinesOK = new Engines::MachineList;

  unsigned int lghtOfmachinesOK = 0;
  machinesOK->length(listOfMachines.length());

  for(unsigned int i=0; i<listOfMachines.length(); i++)
    {
      const char *currentMachine=listOfMachines[i];
//       MESSAGE("_FindComponent, look at " << currentMachine);
      CORBA::Object_var obj = _NS->ResolveComponent(currentMachine,
						    containerName,
						    componentName,
						    nbproc);
      if (!CORBA::is_nil(obj))
	machinesOK[lghtOfmachinesOK++] = CORBA::string_dup(currentMachine);
    }

  // --- find the best machine among the list

  if(lghtOfmachinesOK != 0)
    {
      machinesOK->length(lghtOfmachinesOK);
      CORBA::String_var bestMachine = _ContManager->FindFirst(machinesOK);
      CORBA::Object_var obj = _NS->ResolveComponent(bestMachine,
						    containerName,
						    componentName,
						    nbproc);
      return Engines::Component::_narrow(obj);
    }
  else
    return Engines::Component::_nil();
}

//=============================================================================
/*! Protected -
 *  Load a component instance.
 *  \param params         machine parameters like type or name...
 *  \param componentName  the name of component class
 *  \param studyId        default = 0  : multistudy instance
 *  \param listOfMachines list of machine address
 *  \return a CORBA reference of the component instance, or _nil if problem
 *  - Finds a container in the list of machine or start one.
 *  - Try to load the component library in the container,
 *  - then create an instance of the component.
 */
//=============================================================================

Engines::Component_ptr 
SALOME_LifeCycleCORBA::
_LoadComponent(const Engines::MachineParameters& params, 
	      const char *componentName,
	      int studyId,
	      const Engines::MachineList& listOfMachines)
{
  const char *containerName = params.container_name;
  int nbproc = NbProc(params);

  MESSAGE("_LoadComponent, required " << containerName <<
	  " " << componentName << " " << nbproc);

  Engines::Container_var cont =
    _ContManager->FindOrStartContainer(params,
				       listOfMachines);
  if (CORBA::is_nil(cont)) return Engines::Component::_nil();

  bool isLoadable = cont->load_component_Library(componentName);
  if (!isLoadable) return Engines::Component::_nil();

  Engines::Component_var myInstance =
    cont->create_component_instance(componentName, studyId);
  return myInstance._retn();
}

Engines::Component_ptr
SALOME_LifeCycleCORBA::Load_ParallelComponent(const Engines::MachineParameters& params,
                                              const char *componentName,
                                              int studyId)
{
  MESSAGE("Entering LoadParallelComponent");

/*MESSAGE("Parameters : ");
  MESSAGE("Container name : " << params.container_name);
  MESSAGE("Number of component nodes : " << params.nb_component_nodes);
  MESSAGE("Component Name : " << componentName);*/

  MESSAGE("Building a list of machines");
  Engines::MachineList_var listOfMachines = _ContManager->GetFittingResources(params, componentName);
  if (listOfMachines->length() == 0)
  {
    INFOS("No matching machines founded !");
    return Engines::Component::_nil();
  }

  MESSAGE("Starting Parallel Container");
  Engines::Container_var cont = _ContManager->FindOrStartParallelContainer(params, listOfMachines);
  if (CORBA::is_nil(cont)) {
    INFOS("FindOrStartParallelContainer() returns a NULL container !");
    return Engines::Component::_nil();
  }

  MESSAGE("Loading component library");
  bool isLoadable = cont->load_component_Library(componentName);
  if (!isLoadable) {
    INFOS(componentName <<" library is not loadable !");
    return Engines::Component::_nil();
  }

  MESSAGE("Creating component instance");
  // @PARALLEL@ permits to identify that the component requested
  // is a parallel component.
  string name = string(componentName) + string("@PARALLEL@");
  Engines::Component_var myInstance = cont->create_component_instance(name.c_str(), studyId);
  if (CORBA::is_nil(myInstance))
    INFOS("create_component_instance returns a NULL component !");
  return myInstance._retn();
}

