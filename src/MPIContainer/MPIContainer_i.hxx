// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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
//  File   : MPIContainer_i.hxx
//  Module : SALOME

#ifndef _SALOME_PCONTAINER_
#define _SALOME_PCONTAINER_ 

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_MPIContainer)
#include "SALOME_Container_i.hxx"
#include "MPIObject_i.hxx"

void * th_loadcomponentlibrary(void *st);
void * th_createcomponentinstance(void *st);
void * th_loadimpl(void *st);
void * th_removeimpl(void *st);
void * th_finalizeremoval(void *st);

typedef struct {
  int ip;
  Engines::IORTab* tior;
  std::string compoName;
  std::string nameToRegister;
  long studyId;
  Engines::EngineComponent_ptr cptr;
} thread_st;

class Engines_MPIContainer_i : public POA_Engines::MPIContainer,
                               public Engines_Container_i,
                               public MPIObject_i
{

 public:
  // Constructor
  Engines_MPIContainer_i( CORBA::ORB_ptr orb,
                          PortableServer::POA_ptr poa,
                          char * containerName,
                          int argc, char *argv[]);
  Engines_MPIContainer_i();
  // Destructor
  ~Engines_MPIContainer_i();

  // shutdown corba server
  void Shutdown();

  // Load a component library
  // synchronous version for process 0
  bool load_component_Library(const char* componentName, CORBA::String_out reason);

  // Create an instance of component
  // synchronous version for process 0
  virtual Engines::EngineComponent_ptr
  create_component_instance_env( const char* componentName,
                                 CORBA::Long studyId,          // 0 for multiStudy
                                 const Engines::FieldsDict& env,
                                 CORBA::String_out reason);

  // Load a component in current MPI container
  // synchronous version for process 0
  Engines::EngineComponent_ptr load_impl(const char* nameToRegister,
                                   const char* componentName);

  // Unload a component from current MPI container
  // synchronous version for process 0
  void remove_impl(Engines::EngineComponent_ptr component_i);

  // synchronous version for process 0
  void finalize_removal();

 private:
  bool Lload_component_Library(const char* componentName);
  Engines::EngineComponent_ptr
  Lcreate_component_instance( const char* componentName,
                              CORBA::Long studyId); // 0 for multiStudy
  Engines::EngineComponent_ptr Lload_impl(const char* nameToRegister,
                                    const char* componentName);

  Engines::EngineComponent_ptr
  createMPIInstance(std::string genericRegisterName,
                    void *handle,
                    int studyId);

};
#endif
