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
//  File   : SALOME_Container.cxx
//  Author : Paul RASCLE, EDF - MARC TAJCHMAN, CEA
//  Module : SALOME
//  $Header$

#include <iostream>
#include <string>
#include <stdio.h>

//#include "Utils_ORB_INIT.hxx"
//#include "Utils_SINGLETON.hxx"
#include <unistd.h>
#include "SALOME_NamingService.hxx"
#include "SALOME_Container_i.hxx"
#include "utilities.h"
#include "SALOMETraceCollector.hxx"
#include "OpUtil.hxx"

#ifdef CHECKTIME
#include <Utils_Timer.hxx>
#endif

#ifdef HAVE_MPI2
#include <mpi.h>
#endif

#include "Container_init_python.hxx"

using namespace std;

extern "C" void HandleServerSideSignals(CORBA::ORB_ptr theORB);

int main(int argc, char* argv[])
{
#ifdef HAVE_MPI2
  MPI_Init(&argc,&argv);
#endif
  // Initialise the ORB.
  CORBA::ORB_var orb = CORBA::ORB_init( argc , argv ) ;
  SALOMETraceCollector *myThreadTrace = SALOMETraceCollector::instance(orb);
  INFOS_COMPILATION;
  BEGIN_OF(argv[0]);

  ASSERT(argc > 1);
  SCRUTE(argv[1]);
  bool isSupervContainer = false;
  if (strcmp(argv[1],"SuperVisionContainer") == 0) isSupervContainer = true;

  if (!isSupervContainer)
    {
      int _argc = 1;
      char* _argv[] = {""};
      KERNEL_PYTHON::init_python(argc,argv);
    }
  else
    {
      Py_Initialize() ;
      PySys_SetArgv( argc , argv ) ;
    }

  try{
    // Obtain a reference to the root POA.
    // obtain the root poa manager
    //
    long TIMESleep = 500000000;
    int NumberOfTries = 40;
    int a;
    timespec ts_req;
    ts_req.tv_nsec=TIMESleep;
    ts_req.tv_sec=0;
    timespec ts_rem;
    ts_rem.tv_nsec=0;
    ts_rem.tv_sec=0;
    CosNaming::NamingContext_var inc;
    PortableServer::POA_var root_poa;
    CORBA::Object_var theObj;
    CORBA::Object_var obj;
    CORBA::Object_var object;
    //SALOME_NamingService &naming = *SINGLETON_<SALOME_NamingService>::Instance() ;
    int CONTAINER=0;
    const char * Env = getenv("USE_LOGGER");
    int EnvL =0;
    if(Env != NULL && strlen(Env))
      EnvL=1;
    
    CosNaming::Name name;
    name.length(1);
    name[0].id=CORBA::string_dup("Logger");    
    PortableServer::POAManager_var pman; 
    for(int i = 1; i <= NumberOfTries; i++){
      if(i != 1) 
	a=nanosleep(&ts_req,&ts_rem);
      try{ 
	obj = orb->resolve_initial_references("RootPOA");
	if(!CORBA::is_nil(obj))
	  root_poa = PortableServer::POA::_narrow(obj);
	if(!CORBA::is_nil(root_poa))
	  pman = root_poa->the_POAManager();
	if(!CORBA::is_nil(orb)) 
	  theObj = orb->resolve_initial_references("NameService");
	if (!CORBA::is_nil(theObj))
	  inc = CosNaming::NamingContext::_narrow(theObj);
      }catch(CORBA::SystemException&){
	MESSAGE( "Container: CORBA::SystemException: Unable to contact the Naming Service" );
      }
      if(!CORBA::is_nil(inc)){
	MESSAGE( "Container: Naming Service was found" );
	if(EnvL == 1){
	  for(int j = 1; j <= NumberOfTries; j++){
	    if(j != 1) 
	      a=nanosleep(&ts_req, &ts_rem);
	    try{
	      object = inc->resolve(name);
	    }catch(CosNaming::NamingContext::NotFound){
	      MESSAGE( "Container: Logger Server wasn't found" );
	    }catch(...){
	      MESSAGE( "Container: Unknown exception" );
	    }
	    if(!CORBA::is_nil(object)){
	      MESSAGE( "Container: Logger Server was found" );
	      CONTAINER = 1;
	      break;
	    }
	  }
	}
      }
      if(CONTAINER == 1 || (EnvL == 0 && !CORBA::is_nil(inc)))
	break;
    }
    
    char *containerName = "";
    if(argc > 1){
      containerName = argv[1] ;
    }
    
    Engines_Container_i * myContainer 
      = new Engines_Container_i(orb, root_poa, containerName , argc , argv );

    pman->activate();
    
#ifdef CHECKTIME
    Utils_Timer timer;
    timer.Start();
    timer.Stop();
    MESSAGE("SALOME_Registry_Server.cxx - orb->run()");
    timer.ShowAbsolute();
#endif
    
    HandleServerSideSignals(orb);
  }catch(CORBA::SystemException&){
    INFOS("Caught CORBA::SystemException.");
  }catch(PortableServer::POA::WrongPolicy&){
    INFOS("Caught CORBA::WrongPolicyException.");
  }catch(PortableServer::POA::ServantAlreadyActive&){
    INFOS("Caught CORBA::ServantAlreadyActiveException");
  }catch(CORBA::Exception&){
    INFOS("Caught CORBA::Exception.");
  }catch(std::exception& exc){
    INFOS("Caught std::exception - "<<exc.what()); 
  }catch(...){
    INFOS("Caught unknown exception.");
  }
#ifdef HAVE_MPI2
  MPI_Finalize();
#endif
  END_OF(argv[0]);
  delete myThreadTrace;
  return 0 ;
}

