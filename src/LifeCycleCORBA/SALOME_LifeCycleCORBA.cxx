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

#include <ServiceUnreachable.hxx>

#include "SALOME_LifeCycleCORBA.hxx"
#include CORBA_CLIENT_HEADER(SALOME_ModuleCatalog)
#include "SALOME_ContainerManager.hxx"
#include "SALOME_Component_i.hxx"
#include "SALOME_NamingService.hxx"
using namespace std;

SALOME_LifeCycleCORBA::SALOME_LifeCycleCORBA(SALOME_NamingService *ns)
{
  _NS = ns;
  //add try catch
  CORBA::Object_var obj=_NS->Resolve(SALOME_ContainerManager::_ContainerManagerNameInNS);
  _ContManager=Engines::ContainerManager::_narrow(obj);
}

SALOME_LifeCycleCORBA::~SALOME_LifeCycleCORBA()
{
}

string SALOME_LifeCycleCORBA::ContainerName(
                                         const char * aComputerContainer ,
                                         string * theComputer ,
                                         string * theContainer ) {
  char * ContainerName = new char [ strlen( aComputerContainer ) + 1 ] ;
  strcpy( ContainerName , aComputerContainer ) ;
  string theComputerContainer("/Containers/");
  char * slash = strchr( ContainerName , '/' ) ;
  if ( !slash ) {
    *theComputer = GetHostname() ;
    theComputerContainer += *theComputer ;
    theComputerContainer += "/" ;
    *theContainer = ContainerName ;
    theComputerContainer += *theContainer ;
  }
  else {
    slash[ 0 ] = '\0' ;
    slash += 1 ;
    *theContainer = slash ;
    if ( !strcmp( ContainerName , "localhost" ) ) {
      *theComputer = GetHostname() ;
    }
    else {
      *theComputer = ContainerName ;
    }
    theComputerContainer += *theComputer ;
    theComputerContainer += "/" ;
    theComputerContainer += *theContainer ;
  }
  delete [] ContainerName;
  return theComputerContainer ;
}

string SALOME_LifeCycleCORBA::ComputerPath(
                                         const char * theComputer ) {
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

Engines::Component_ptr SALOME_LifeCycleCORBA::FindOrLoad_Component
                                  (const char *containerName,
				   const char *componentName)
{
  char *stContainer=strdup(containerName);
  string st2Container(stContainer);
  int rg=st2Container.find("/");
  if(rg<0) {
    //containerName doesn't contain "/" => Local container
    free(stContainer);
    Engines::MachineList_var listOfMachine=new Engines::MachineList;
    listOfMachine->length(1);
    listOfMachine[0]=CORBA::string_dup(GetHostname().c_str());
    Engines::Component_ptr ret=FindComponent(containerName,componentName,listOfMachine.in());
    if(CORBA::is_nil(ret))
      return LoadComponent(containerName,componentName,listOfMachine);
    else
      return ret;
  }
  else {
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

Engines::Component_ptr SALOME_LifeCycleCORBA::FindOrLoad_Component(const Engines::MachineParameters& params,
								   const char *componentName)
{
  Engines::MachineList_var listOfMachine=_ContManager->GetFittingResources(params,componentName);
  Engines::Component_ptr ret=FindComponent(params.container_name,componentName,listOfMachine);
  if(CORBA::is_nil(ret))
    return LoadComponent(params.container_name,componentName,listOfMachine);
  else
    return ret;
}

Engines::Component_ptr SALOME_LifeCycleCORBA::FindComponent(const char *containerName,
								 const char *componentName,
								 const Engines::MachineList& listOfMachines)
{
  if(containerName[0]!='\0')
    {
      Engines::MachineList_var machinesOK=new Engines::MachineList;
      unsigned int lghtOfmachinesOK=0;
      machinesOK->length(listOfMachines.length());
      for(unsigned int i=0;i<listOfMachines.length();i++)
	{
	  const char *currentMachine=listOfMachines[i];
	  string componentNameForNS=Engines_Component_i::BuildComponentNameForNS(componentName,containerName,currentMachine);
	  CORBA::Object_var obj = _NS->Resolve(componentNameForNS.c_str());
	  if(!CORBA::is_nil(obj))
	    {
	      machinesOK[lghtOfmachinesOK++]=CORBA::string_dup(currentMachine);
	    }
	}
      if(lghtOfmachinesOK!=0)
	{
	  machinesOK->length(lghtOfmachinesOK);
	  CORBA::String_var bestMachine=_ContManager->FindBest(machinesOK);
	  string componentNameForNS=Engines_Component_i::BuildComponentNameForNS(componentName,containerName,bestMachine);
	  CORBA::Object_var obj=_NS->Resolve(componentNameForNS.c_str());
	  return Engines::Component::_narrow(obj);
	}
      else
	return Engines::Component::_nil();
    }
  else
    {
      //user specified no container name so trying to find a component in the best machine among listOfMachines
      CORBA::String_var bestMachine=_ContManager->FindBest(listOfMachines);
      //Normally look at all containers launched on bestMachine to see if componentName is already launched on one of them. To do..
      string componentNameForNS=Engines_Component_i::BuildComponentNameForNS(componentName,containerName,bestMachine);
      CORBA::Object_var obj = _NS->Resolve(componentNameForNS.c_str());
      return Engines::Component::_narrow(obj);
    }
}

Engines::Component_ptr SALOME_LifeCycleCORBA::LoadComponent(const char *containerName, const char *componentName, const Engines::MachineList& listOfMachines)
{
  Engines::Container_var cont=_ContManager->FindOrStartContainer(containerName,listOfMachines);
  string implementation=Engines_Component_i::GetDynLibraryName(componentName);
  return cont->load_impl(componentName, implementation.c_str());
}
