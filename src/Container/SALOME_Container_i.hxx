//  SALOME Container : implementation of container and engine for Kernel
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
//  File   : SALOME_Container_i.hxx
//  Author : Paul RASCLE, EDF - MARC TAJCHMAN, CEA
//  Module : SALOME
//  $Header$

#ifndef _SALOME_CONTAINER_I_HXX_
#define _SALOME_CONTAINER_I_HXX_

#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Component)
#include <omnithread.h>
#include <map>
#include <string>

class SALOME_NamingService;

class Engines_Container_i: public POA_Engines::Container,
                public PortableServer::RefCountServantBase
{
public:
  Engines_Container_i();
  Engines_Container_i(CORBA::ORB_ptr orb, 
		      PortableServer::POA_ptr poa,
		      char * containerName ,
                      int argc, char* argv[],
		      bool regist = true,
		      bool activ = true);
  virtual ~Engines_Container_i();

  // Launch a new container from the current container
  Engines::Container_ptr start_impl(const char* ContainerName);

  // Load component in current container
  Engines::Component_ptr load_impl(const char* nameToRegister,
				   const char* componentName);

  // Unload component from current container
  void remove_impl(Engines::Component_ptr component_i);
  void finalize_removal();

  char* name();
  char* machineName();
  void ping();

  // Kill current container
  bool Kill_impl() ;
  char* getHostName();
  long getPID();

protected:

  SALOME_NamingService *_NS ;
  std::string _library_path;
  std::string _containerName;
  CORBA::ORB_var _orb;
  PortableServer::POA_var _poa;
  PortableServer::ObjectId * _id ;
  int _numInstance ;
  std::map<std::string, void *> handle_map ;
  std::map<std::string, void *> remove_map ;
  omni_mutex _numInstanceMutex ; // if several threads on the same object

  //private: 

  int   _argc ;
  char** _argv ;
  long _pid;

};

#endif

