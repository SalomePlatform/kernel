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
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
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

#include "OpUtil.hxx"
#include "utilities.h"
#include "Launchers.hxx"

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

IncompatibleComponent::IncompatibleComponent( void ): SALOME_Exception( "IncompatibleComponent" )
{
	;
}
IncompatibleComponent::IncompatibleComponent( const IncompatibleComponent &ex  ): SALOME_Exception( ex ) 
{
	;
}

//=============================================================================
/*! 
 *  Constructor
 */
//=============================================================================

SALOME_LifeCycleCORBA::SALOME_LifeCycleCORBA(SALOME_NamingService *ns)
{
  if (!ns)
    {
      int argc = 0;
      char *xargv = "";
      char **argv = &xargv;
      CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
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

Engines::Component_ptr
SALOME_LifeCycleCORBA::FindOrLoad_Component(const Engines::MachineParameters& params, 
                                            const char *componentName,
                                            int studyId)
{
  if (! isKnownComponentClass(componentName)) return Engines::Component::_nil();

  Engines::MachineList_var listOfMachine=_ContManager->GetFittingResources(params,componentName);
  Engines::Component_ptr ret=FindComponent(params,componentName,listOfMachine);
  if(CORBA::is_nil(ret))
    return LoadComponent(params,componentName,listOfMachine);
  else
    return ret;
}

//=============================================================================
/*! Public - 
 *  Find and aready existing and registered component instance or load a new
 *  component instance on a container defined by name
 *  \param containerName  the name of container, under one of the forms
 *           - 1 localhost/aContainer
 *           - 2 aContainer
 *           - 3 /machine/aContainer
 *     (not the same rules as FindContainer() method based on protected method
 *      ContainerName() -- MUST BE CORRECTED --)
 *  \param componentName  the name of component class
 *  \return a CORBA reference of the component instance, or _nil if problem
 */
//=============================================================================

Engines::Component_ptr
SALOME_LifeCycleCORBA::FindOrLoad_Component(const char *containerName,
					    const char *componentName)
{
  // Check if Component Name is known
  if (! isKnownComponentClass(componentName)) return Engines::Component::_nil();

  // Chack if containerName contains machine name (if yes: rg>0)
  char *stContainer=strdup(containerName);
  string st2Container(stContainer);
  int rg=st2Container.find("/");

  Engines::MachineParameters_var params=new Engines::MachineParameters;
  if(rg<0) {
    params->container_name=CORBA::string_dup(stContainer);
    params->hostname=CORBA::string_dup(GetHostname().c_str());
  }
  else {
    stContainer[rg]='\0';
    params->container_name=CORBA::string_dup(stContainer+rg+1);
    params->hostname=CORBA::string_dup(stContainer);
  }
  params->isMPI = false;
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
      SALOME_ModuleCatalog::Acomponent_ptr compoInfo = 
	Catalog->GetComponent(componentName);
      if (CORBA::is_nil (compoInfo)) 
	{
	  INFOS("Catalog Error : Component not found in the catalog");
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

bool SALOME_LifeCycleCORBA::isMpiContainer(const Engines::MachineParameters& params) throw(IncompatibleComponent)
{
//   MESSAGE(params.container_name)
//   if( strstr(params.container_name,"MPI") ){
//     if( !params.isMPI ){
//       cerr << "IncompatibleComponent" << endl;
//       throw IncompatibleComponent();
//     }
//     return true;
//   }
//   else{
//     if( params.isMPI ){
//       cerr << "IncompatibleComponent" << endl;
//       throw IncompatibleComponent();
//     }
//     return false;
//   }
  if( params.isMPI )
    return true;
  else
    return false;
}

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

Engines::Component_ptr
SALOME_LifeCycleCORBA::FindComponent(const Engines::MachineParameters& params, 
                                     const char *componentName, 
                                     const Engines::MachineList& listOfMachines)
{
  const char *containerName = params.container_name;
  int nbproc = NbProc(params);

  if (! isKnownComponentClass(componentName)) return Engines::Component::_nil();

  // find list of machines which have component
  Engines::MachineList_var machinesOK=new Engines::MachineList;
  unsigned int lghtOfmachinesOK=0;
  machinesOK->length(listOfMachines.length());
  for(unsigned int i=0;i<listOfMachines.length();i++) {
    const char *currentMachine=listOfMachines[i];
    
    CORBA::Object_var obj = _NS->ResolveComponent(currentMachine,containerName,componentName,nbproc);
    if(!CORBA::is_nil(obj))
      machinesOK[lghtOfmachinesOK++]=CORBA::string_dup(currentMachine);
  }

  // find the best machine among the list which have component
  if(lghtOfmachinesOK!=0) {
    machinesOK->length(lghtOfmachinesOK);
    CORBA::String_var bestMachine=_ContManager->FindBest(machinesOK);
    CORBA::Object_var obj = _NS->ResolveComponent(bestMachine,containerName,componentName,nbproc);
    return Engines::Component::_narrow(obj);
  }
  else
    return Engines::Component::_nil();
}

Engines::Component_ptr 
SALOME_LifeCycleCORBA::LoadComponent(const Engines::MachineParameters& params, 
                                     const char *componentName, 
                                     const Engines::MachineList& listOfMachines)
{
  Engines::Container_var cont=_ContManager->FindOrStartContainer(params,listOfMachines);
  if (CORBA::is_nil(cont)) return Engines::Component::_nil();

  bool isLoadable = cont->load_component_Library(componentName);
  if (!isLoadable) return Engines::Component::_nil();

  Engines::Component_var myInstance =
    cont->create_component_instance(componentName, 0);
  return myInstance._retn();
}














