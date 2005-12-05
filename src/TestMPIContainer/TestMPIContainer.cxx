// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/
//
// using namespace std;
//=============================================================================
// File      : TestMPIContainer.cxx
// Created   : mer jui 4 13:11:27 CEST 2003
// Author    : Bernard SECHER, 2003
// Project   : SALOME
// Copyright : CEA 2003
// $Header$
//=============================================================================

#include "utilities.h"
#include <iostream>
#include <unistd.h>
#include <string>
#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_MPIContainer)
#include CORBA_CLIENT_HEADER(SALOME_TestMPIComponent)

# include "Utils_ORB_INIT.hxx"
# include "Utils_SINGLETON.hxx"
#include "SALOME_NamingService.hxx"
#include "OpUtil.hxx"
#include "SALOMETraceCollector.hxx"
using namespace std;

int main (int argc, char * argv[])
{

  // Initializing omniORB
  ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance() ;
  CORBA::ORB_var &orb = init( argc , argv ) ;
  //  SALOMETraceCollector *myThreadTrace = SALOMETraceCollector::instance(orb);
    
  BEGIN_OF(argv[0])
  try{

    int status;

    if( argc != 3 || strcmp(argv[1],"-np") ){
      cout << "Usage: TestMPIContainer -np nbproc" << endl;
      exit(0);
    }

    // Obtain a reference to the root POA
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA") ;
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj) ;

    // Use Name Service to find container
    SALOME_NamingService NS(orb) ;
    string containerName = "/Containers/" ;
    string hostName = GetHostname();
    containerName += hostName + "/MPIFactoryServer_" + argv[2];

    string dirn(getenv("KERNEL_ROOT_DIR"));
    dirn += "/lib/salome/libSalomeTestMPIComponentEngine.so";
    
    // Try to resolve MPI Container
    obj = NS.Resolve(containerName.c_str()) ;
    Engines::MPIContainer_var iGenFact = Engines::MPIContainer::_narrow(obj);

    if(CORBA::is_nil(iGenFact)){

      // Launch MPI Container
      string cmd("mpirun -np ");
      cmd += argv[2];
      cmd += " ";
      cmd += getenv("KERNEL_ROOT_DIR");
      cmd += "/bin/salome/SALOME_MPIContainer MPIFactoryServer_";
      cmd += argv[2];
      cmd += " &";
      MESSAGE(cmd);
      status = system(cmd.c_str());
      if (status == -1) {
	INFOS("TestMPIContainer launch MPI Container failed (system command status -1)") ;
      }
      else if (status == 217) {
	INFOS("TestMPIContainer launch MPI Container failed (system command status 217)") ;
      }
    
      // Try to resolve MPI Container
      int it = 0;
      do{
	sleep(1);
	obj = NS.Resolve(containerName.c_str()) ;
	iGenFact = Engines::MPIContainer::_narrow(obj);
	MESSAGE("Waiting for MPI Container " << containerName << " : it = " << it );
      }while( CORBA::is_nil(iGenFact) && (it++<15) );

    }

    if(CORBA::is_nil(iGenFact)){
      MESSAGE("launching container failed");
      exit(1);
    }


    Engines::TestMPIComponent_var m1;

//     for (int iter = 0; iter < 1 ; iter++){
//       MESSAGE("----------------------------------------------------" << iter);   
    obj = iGenFact->load_impl("TestMPIComponent",dirn.c_str());

    INFOS("Get Handle on MPI Component");
    m1 = Engines::TestMPIComponent::_narrow(obj);
    if(CORBA::is_nil(m1)){
      INFOS("getting handle on MPI component failed");
    }
    else{ 
      m1->Coucou(1L);
// // 	sleep(5);
      INFOS("Unload MPI Component");
      iGenFact->remove_impl(m1) ;
    }
//     }
//     // Clean-up.
// //     iGenFact->finalize_removal() ;
// //     sleep(5);
    iGenFact->Shutdown();
//     INFOS("shut down corba server for Test MPI Container");
//     orb->destroy();
  }
  catch(CORBA::COMM_FAILURE& ex) {
    INFOS("Caught system exception COMM_FAILURE -- unable to contact the object.");
  }
  catch(CORBA::SystemException&) {
    INFOS("Caught a CORBA::SystemException.");
  }
  catch(CORBA::Exception&) {
    INFOS("Caught CORBA::Exception.");
  }
  catch(...) {
    INFOS("Caught unknown exception.");
  }

  END_OF(argv[0]);
  //  delete myThreadTrace;
  return 0 ;
}
