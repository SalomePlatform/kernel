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
#include CORBA_SERVER_HEADER(SALOME_MPIContainer)
#include "SALOME_Container_i.hxx"
#include "MPIObject_i.hxx"

class Engines_MPIContainer_i : public POA_Engines::MPIContainer,
			       public Engines_Container_i,
			       public MPIObject_i
{

 public:
  // Constructor
  Engines_MPIContainer_i( int nbproc, int numproc,
			  CORBA::ORB_ptr orb,
			  PortableServer::POA_ptr poa,
			  char * containerName,
			  int argc, char *argv[]);
  Engines_MPIContainer_i(int nbproc, int numproc);
  // Destructor
  ~Engines_MPIContainer_i();

  // shutdown corba server
  void Shutdown();

  // Load a component library
  // synchronous version for process 0
  bool load_component_Library(const char* componentName);
  // asynchronous version for other process
  void Asload_component_Library(const char* componentName);

  // Create an instance of component
  // synchronous version for process 0
  Engines::Component_ptr
  create_component_instance( const char* componentName,
			     CORBA::Long studyId); // 0 for multiStudy
  // asynchronous version for other process
  void Ascreate_component_instance( const char* componentName,
				  CORBA::Long studyId); // 0 for multiStudy

  // Load a component in current MPI container
  // synchronous version for process 0
  Engines::Component_ptr load_impl(const char* nameToRegister,
				   const char* componentName);
  // asynchronous version for other process
  void Asload_impl(const char* nameToRegister, const char* componentName);

  // Unload a component from current MPI container
  // synchronous version for process 0
  void remove_impl(Engines::Component_ptr component_i);
  // asynchronous version for other process
  void Asremove_impl(Engines::Component_ptr component_i);

  // synchronous version for process 0
  void finalize_removal();
  // asynchronous version for other process
  void Asfinalize_removal();

 private:
  // local version to not duplicate code 
  // called by synchronous and asynchronous version
  bool Lload_component_Library(const char* componentName);
  Engines::Component_ptr
  Lcreate_component_instance( const char* componentName,
			      CORBA::Long studyId); // 0 for multiStudy
  Engines::Component_ptr Lload_impl(const char* nameToRegister,
				    const char* componentName);
  void Lremove_impl(Engines::Component_ptr component_i);
  void Lfinalize_removal();

  Engines::Component_ptr
  createMPIInstance(std::string genericRegisterName,
		    void *handle,
		    int studyId);

};
#endif
