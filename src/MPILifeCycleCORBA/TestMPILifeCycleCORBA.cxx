// using namespace std;
//=============================================================================
// File      : TestMPILifeCycleCORBA.cxx
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
#include "SALOME_MPILifeCycleCORBA.hxx"
#include "OpUtil.hxx"

int main (int argc, char * argv[])
{

  try{
    // Initializing omniORB
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    
    // Obtain a reference to the root POA
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA") ;
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj) ;
    
    // Use Name Service to find container
    SALOME_NamingService NS(orb);
    
    SALOME_MPILifeCycleCORBA LCC(&NS);
    
    Engines::Component_var comp = LCC.FindOrLoad_MPIComponent("MPIFactoryServer","TestMPIComponent",2);
    
    Engines::TestMPIComponent_var m1 = Engines::TestMPIComponent::_narrow(comp);
    if(CORBA::is_nil(m1)){
      INFOS("echec recuperation poignee composant");
    }
    else{
      
      INFOS("Lancement de coucou");
      m1->Coucou(1L);
    }

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
