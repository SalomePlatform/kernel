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
#include CORBA_CLIENT_HEADER(SALOME_ModuleCatalog)
#include "SALOME_ContainerManager.hxx"
#include "SALOME_Component_i.hxx"
#include "SALOME_NamingService.hxx"
using namespace std;

//=============================================================================
/*! 
 *  Constructor
 */
//=============================================================================

SALOME_LifeCycleCORBA::SALOME_LifeCycleCORBA(SALOME_NamingService *ns)
{
  _NS = ns;
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
 *  Look for a fully qualified container name in Naming Service
 *  \param containerName name of the container to find.
 *         if containerName begins with "/Containers/" it's used as it is.
 *            We assume string = "/Containers/machine/aContainerName"
 *         else
 *            we assume string = "aContainerName" or "machine/aContainerName".
 *            string "/Containers/machine/aContainerName" is rebuilt.
 *            if machine = "localhost", machine replaced by GetHostname()
 *  \return Container CORBA reference or _nil if not found in Naming Service
 */
//=============================================================================

Engines::Container_ptr
SALOME_LifeCycleCORBA::FindContainer(const char *containerName)
{
  ASSERT(_NS != NULL);
  string cont ;
  if ( strncmp( containerName , "/Containers/" , 12 ) )
    { // Compatibility ...
      string theComputer ;
      string theContainer ;
      cont = ContainerName( containerName , &theComputer , &theContainer ) ;
    }
  else
    {
      cont = containerName ;
    }
  try
    {
      SCRUTE( cont );
      CORBA::Object_var obj = _NS->Resolve( cont.c_str() );
      if( !CORBA::is_nil( obj ) )
	{
	  return Engines::Container::_narrow( obj ) ;
	}
    }
  catch (ServiceUnreachable&)
    {
      INFOS("Caught exception: Naming Service Unreachable");
    }
  catch (...)
    {
      INFOS("Caught unknown exception.");
    }
  return Engines::Container::_nil();
}

//=============================================================================
/*! Public - 
 *  Find and aready existing and registered component instance.
 *  \param params         machine parameters like type or name...
 *  \param componentName  the name of component class
 *  \param studyId        default = 0  : multistudy instance
 *  \param instanceName   default = "" : to retrieve a specific instance
 *  \return a CORBA reference of the component instance, or _nil if not found
 */
//=============================================================================

Engines::Component_ptr
SALOME_LifeCycleCORBA::FindComponent(const Engines::MachineParameters& params,
				     const char *componentName,
				     int studyId,
				     const char *instanceName)
{
  ASSERT(0);
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
  ASSERT(0);
}

//=============================================================================
/*! Public - 
 *  Find and aready existing and registered component instance or load a new
 *  component instance on a container defined by machine parameters
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
  if (! isKnownComponentClass(componentName))
    return Engines::Component::_nil();

  Engines::MachineList_var listOfMachine =
    _ContManager->GetFittingResources(params,componentName);
  Engines::Component_ptr ret=
    FindComponent(params.container_name,componentName,listOfMachine);
  if(CORBA::is_nil(ret))
    return LoadComponent(params.container_name,componentName,listOfMachine);
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
 *     (not the same rules as FindContainer() method base on protected method
 *      ContainerName() -- MUST BE CORRECTED --)
 *  \param componentName  the name of component class
 *  \return a CORBA reference of the component instance, or _nil if problem
 */
//=============================================================================

Engines::Component_ptr
SALOME_LifeCycleCORBA::FindOrLoad_Component(const char *containerName,
					    const char *componentName)
{
  if (!isKnownComponentClass(componentName)) return Engines::Component::_nil();

  char *stContainer=strdup(containerName);
  string st2Container(stContainer);
  int rg=st2Container.find("/");
  if(rg>=0)
    {
      stContainer[rg]='\0';
      if(strcmp(stContainer,"localhost")==0)
	{
	  Engines::Component_ptr ret=FindOrLoad_Component(stContainer+rg+1,
							  componentName);
	  free(stContainer);
	  return ret;
	}
      else ASSERT(0); // no return in that case...
    }
  if(rg<0)
    {
      //containerName doesn't contain "/" => Local container
      free(stContainer);
      Engines::MachineList_var listOfMachine=new Engines::MachineList;
      listOfMachine->length(1);
      listOfMachine[0]=CORBA::string_dup(GetHostname().c_str());
      Engines::Component_ptr ret = FindComponent(containerName,
						 componentName,
						 listOfMachine.in());
      if(CORBA::is_nil(ret))
	return LoadComponent(containerName,componentName,listOfMachine);
      else
	return ret;
    }
  else 
    {
      //containerName contains "/" => Remote container
      stContainer[rg]='\0';
      Engines::MachineParameters_var params=new Engines::MachineParameters;
      params->container_name=CORBA::string_dup(stContainer+rg+1);
      params->hostname=CORBA::string_dup(stContainer);
      params->OS=CORBA::string_dup("LINUX");
      free(stContainer);
      return FindOrLoad_Component(params,componentName);
    }
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

//=============================================================================
/*! Protected -
 *  
 */
//=============================================================================

Engines::Component_ptr
SALOME_LifeCycleCORBA::FindComponent(const char *containerName,
				     const char *componentName,
				     const Engines::MachineList& listOfMachines)
{
  if (! isKnownComponentClass(componentName)) return Engines::Component::_nil();
  if(containerName[0]!='\0')
    {
      Engines::MachineList_var machinesOK=new Engines::MachineList;
      unsigned int lghtOfmachinesOK=0;
      machinesOK->length(listOfMachines.length());
      for(unsigned int i=0;i<listOfMachines.length();i++)
	{
	  const char *currentMachine=listOfMachines[i];
	  string componentNameForNS = 
	    Engines_Component_i::BuildComponentNameForNS(componentName,
							 containerName,
							 currentMachine);
	  SCRUTE(componentNameForNS);
	  CORBA::Object_var obj=_NS->ResolveFirst(componentNameForNS.c_str());
	  if(!CORBA::is_nil(obj))
	    {
	      machinesOK[lghtOfmachinesOK++]=CORBA::string_dup(currentMachine);
	    }
	}
      if(lghtOfmachinesOK!=0)
	{
	  machinesOK->length(lghtOfmachinesOK);
	  CORBA::String_var bestMachine=_ContManager->FindBest(machinesOK);
	  string componentNameForNS =
	    Engines_Component_i::BuildComponentNameForNS(componentName,
							 containerName,
							 bestMachine);
	  SCRUTE(componentNameForNS);
	  CORBA::Object_var obj=_NS->ResolveFirst(componentNameForNS.c_str());
	  return Engines::Component::_narrow(obj);
	}
      else
	return Engines::Component::_nil();
    }
  else
    {
      //user specified no container name so trying to find a component in
      //the best machine among listOfMachines
      CORBA::String_var bestMachine=_ContManager->FindBest(listOfMachines);
      //Normally look at all containers launched on bestMachine to see if
      //componentName is already launched on one of them. To do..
      string componentNameForNS =
	Engines_Component_i::BuildComponentNameForNS(componentName,
						     containerName,
						     bestMachine);
      SCRUTE(componentNameForNS);
      CORBA::Object_var obj = _NS->ResolveFirst(componentNameForNS.c_str());
      return Engines::Component::_narrow(obj);
    }
}

//=============================================================================
/*! Protected -
 *
 */
//=============================================================================

Engines::Component_ptr
SALOME_LifeCycleCORBA::LoadComponent(const char *containerName,
				     const char *componentName,
				     const Engines::MachineList& listOfMachines)
{
  Engines::Container_var cont=_ContManager->FindOrStartContainer(containerName,
								 listOfMachines);
  //string implementation=Engines_Component_i::GetDynLibraryName(componentName);
  //return cont->load_impl(componentName, implementation.c_str());
  return cont->load_impl(componentName,"");
}


//=============================================================================
/*! Protected -
 *
 */
//=============================================================================

Engines::Container_ptr
SALOME_LifeCycleCORBA::FindOrStartContainer(const string aComputerContainer ,
					    const string theComputer ,
					    const string theContainer )
{
  SCRUTE( aComputerContainer ) ;
  SCRUTE( theComputer ) ;
  SCRUTE( theContainer ) ;

  Engines::Container_var aContainer = FindContainer(aComputerContainer.c_str());

  if ( !CORBA::is_nil( aContainer ) )
    {
      return aContainer ;
    }

  Engines::Container_var aFactoryServer ;

  bool pyCont = false ;
  int len = theContainer.length() ;
  if ( !strcmp( &theContainer.c_str()[len-2] , "Py" ) )
    {
      pyCont = true ;
    }

  string addr=_NS->getIORaddr();
  string CMD="SALOME_Container";
  if ( pyCont ) {
    CMD="SALOME_ContainerPy.py";
  }
  CMD=CMD + " " + theContainer;
  CMD=CMD + " -ORBInitRef NameService="+addr;

  /*
   *  Get the appropriate launcher and ask to launch
   */
  PyObject * launcher=getLauncher((char *)theComputer.c_str());
  Launcher_Slaunch(launcher,(char *)theComputer.c_str(),(char *)CMD.c_str());
  /*
   *  Wait until the container is registered in Naming Service
   */
  int count = 5 ;
  while ( CORBA::is_nil( aFactoryServer ) && count )
    {
      sleep( 1 ) ;
      count-- ;
      if ( count != 10 )
	MESSAGE( count << ". Waiting for FactoryServer on " << theComputer)
	  aFactoryServer = FindContainer( aComputerContainer.c_str() ) ;
    }
  if ( !CORBA::is_nil( aFactoryServer ) )
    {
      return aFactoryServer;
    }
  MESSAGE("SALOME_LifeCycleCORBA::StartOrFindContainer rsh failed") ;
  return Engines::Container::_nil();
}

//=============================================================================
/*! Protected -
 *  \param aComputerContainer container name under one of the forms:
 *           - 1 aContainer
 *           - 2 machine/aContainer
 *  \param theComputer  return computer name:
 *           - 1 machine = GetHostname() 
 *           - 2 machine (localhost replaced by GetHostName())
 *  \param theContainer return container name:
 *           - 1 aContainer 
 *           - 2 aContainer
 *  \return /Containers/machine/aContainer
 */
//=============================================================================

string SALOME_LifeCycleCORBA::ContainerName(const char *aComputerContainer ,
					    string * theComputer ,
					    string * theContainer )
{
  char * ContainerName = new char [ strlen( aComputerContainer ) + 1 ] ;
  strcpy( ContainerName , aComputerContainer ) ;
  string theComputerContainer("/Containers/");
  char *slash = strchr( ContainerName , '/' ) ; // first occurence of '/'
  if ( !slash )    // no '/', only the name, without path
    {
      *theComputer = GetHostname() ;
      theComputerContainer += *theComputer ;
      theComputerContainer += "/" ;
      *theContainer = ContainerName ;
      theComputerContainer += *theContainer ;
    }
  else
    {
      ASSERT( slash != aComputerContainer); // "/something..." not OK
      slash[ 0 ] = '\0' ;
      slash += 1 ;
      *theContainer = slash ;
      if ( !strcmp( ContainerName , "localhost" ) )
	{
	  *theComputer = GetHostname() ;
	}
      else
	{
	  *theComputer = ContainerName ;
	}
      theComputerContainer += *theComputer ;
      theComputerContainer += "/" ;
      theComputerContainer += *theContainer ;
    }
  delete [] ContainerName;
  return theComputerContainer ;
}

//=============================================================================
/*! Protected -
 *
 */
//=============================================================================

string SALOME_LifeCycleCORBA::ComputerPath(const char * theComputer ) 
{
  CORBA::String_var path;
  CORBA::Object_var obj = _NS->Resolve("/Kernel/ModulCatalog");
  SALOME_ModuleCatalog::ModuleCatalog_var Catalog = 
	             SALOME_ModuleCatalog::ModuleCatalog::_narrow(obj) ;
  try {
    path = Catalog->GetPathPrefix( theComputer );
  }
  catch (SALOME_ModuleCatalog::NotFound&) {
    INFOS("GetPathPrefix(" << theComputer << ") not found!");
    path = "" ;
  }
  SCRUTE( path ) ;
  return CORBA::string_dup( path ) ;
}
