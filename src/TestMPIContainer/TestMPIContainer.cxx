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
using namespace std;

int main (int argc, char * argv[])
{

  try{

    int status;

    MESSAGE("COUCOU");
    if( argc != 3 || strcmp(argv[1],"-np") ){
      cout << "Usage: TestMPIContainer -np nbproc" << endl;
      exit(0);
    }

    // Initializing omniORB
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    
    // Obtain a reference to the root POA
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA") ;
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj) ;

    // Use Name Service to find container
    SALOME_NamingService NS(orb) ;
    string containerName = "/Containers/" ;
    string hostName = GetHostname();
    containerName += hostName + "/MPIFactoryServer_" + argv[2];

    MESSAGE(containerName);

    string dirn(getenv("KERNEL_ROOT_DIR"));
    dirn += "/lib/salome/libSalomeTestMPIComponentEngine.so";
    MESSAGE(dirn.c_str());
    
    // Try to resolve MPI Container
    obj = NS.Resolve(containerName.c_str()) ;
    Engines::Container_var iGenFact = Engines::Container::_narrow(obj);

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
	iGenFact = Engines::Container::_narrow(obj);
	MESSAGE("Waiting for MPI Container " << containerName << " : it = " << it );
      }while( CORBA::is_nil(iGenFact) && (it++<15) );

    }

    if(CORBA::is_nil(iGenFact)){
      MESSAGE("echec recuperation poignee container");
      exit(1);
    }

    Engines::TestMPIComponent_var m1;

    for (int iter = 0; iter < 1 ; iter++){
      MESSAGE("----------------------------------------------------" << iter);   
      obj = iGenFact->load_impl("TestMPIComponent",dirn.c_str());

      m1 = Engines::TestMPIComponent::_narrow(obj);
      if(CORBA::is_nil(m1)){
	INFOS("echec recuperation poignee composant");
      }
      else{
      
	INFOS("recup m1");
	SCRUTE(m1->instanceName());
	INFOS("Lancement de coucou");
	m1->Coucou(1L);
	INFOS("On remove le composant");
	iGenFact->remove_impl(m1) ;
	sleep(5);
      }
    }
    // Clean-up.
    iGenFact->finalize_removal() ;
    orb->destroy();
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

  return 0;
}
