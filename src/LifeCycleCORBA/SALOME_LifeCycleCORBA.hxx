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
//  File   : SALOME_LifeCycleCORBA.hxx
//  Author : Paul RASCLE, EDF - MARC TAJCHMAN, CEA
//  Module : SALOME
//  $Header$

#ifndef _SALOME_LIFECYCLECORBA_HXX_
#define _SALOME_LIFECYCLECORBA_HXX_

#include <stdlib.h>
#include <unistd.h>
#include <string>

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_ContainerManager)
#include CORBA_CLIENT_HEADER(SALOME_Component)

class SALOME_NamingService;

class SALOME_LifeCycleCORBA
{
public:
  SALOME_LifeCycleCORBA(SALOME_NamingService *ns);
  virtual ~SALOME_LifeCycleCORBA();
  Engines::Container_ptr FindContainer(const char *containerName); // for supervision
  Engines::Component_ptr FindOrLoad_Component(const Engines::MachineParameters& params,
					      const char *componentName);
  Engines::Component_ptr FindOrLoad_Component(const char *containerName,
					      const char *componentName);
  bool isKnownComponentClass(const char *componentName);
protected:
  //! Establish if a component called "componentName" in a container called "containerName" exists among the list of resources
  //! in "listOfMachines". This method uses Naming Service to find the component.
  Engines::Component_ptr FindComponent(const char *containerName,
					    const char *componentName,
					    const Engines::MachineList& listOfMachines);

  Engines::Component_ptr LoadComponent(const char *containerName, const char *componentName, const Engines::MachineList& listOfMachines);

  SALOME_NamingService *_NS;
  Engines::ContainerManager_var _ContManager;
  
  //private:
  std::string ContainerName( const char * aComputerContainer ,
			     std::string * theComputer ,
			     std::string * theContainer ) ;
  std::string ComputerPath( const char * theComputer ) ;
} ;

#endif
