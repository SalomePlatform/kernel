//  SALOME MPIContainer : implemenation of container based on MPI libraries
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
//  File   : MPIContainer_i.hxx
//  Module : SALOME

#ifndef _SALOME_PCONTAINER_
#define _SALOME_PCONTAINER_ 

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(MPIContainer)
#include "SALOME_Container_i.hxx"
#include "SALOME_NamingService.hxx"
#include "Utils_SINGLETON.hxx"
#include "OpUtil.hxx"
#include "MPIObject_i.hxx"

class MPIContainer_i : public POA_Engines::MPIContainer,
		     public Engines_Container_i,
		     public MPIObject_i
{

 public:
  // Constructor
  MPIContainer_i( int nbproc, int numproc,
		  CORBA::ORB_ptr orb,
		  PortableServer::POA_ptr poa,
		  char * containerName);
  // Destructor
  ~MPIContainer_i();

  // Load a component
  Engines::Component_ptr load_impl(const char* nameToRegister,
				   const char* componentName);

  // Unload a component
  void remove_impl(Engines::Component_ptr component_i);
  void finalize_removal();

};
#endif
