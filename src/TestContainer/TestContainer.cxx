using namespace std;
//=============================================================================
// File      : TestContainer.cxx
// Created   : jeu jui 12 13:11:27 CEST 2001
// Author    : Paul RASCLE, EDF - MARC TAJCHMAN, CEA
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

#include "utilities.h"
#include <iostream>
#include <unistd.h>
#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_Component)
#include CORBA_CLIENT_HEADER(SALOME_TestComponent)

#include "SALOME_NamingService.hxx"
#include "OpUtil.hxx"

int main (int argc, char * argv[])
{

  try
    {
      // Initializing omniORB
      CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    
      // use IOR to find container
      //if (argc != 2) { return 1; }
      //CORBA::Object_var obj = orb->string_to_object(argv[1]);
      //Engines::Container_var iGenFact = Engines::Container::_narrow(obj);

      // Obtain a reference to the root POA
      CORBA::Object_var obj = orb->resolve_initial_references("RootPOA") ;
      PortableServer::POA_var poa = PortableServer::POA::_narrow(obj) ;
    
      // Use Name Service to find container
      SALOME_NamingService _NS(orb) ;
      string containerName = "/Containers/" ;
      string hostName = GetHostname();
      containerName += hostName;

      obj = _NS.Resolve(containerName.c_str()) ;
      Engines::Container_var iGenFact = Engines::Container::_narrow(obj);

      Engines::TestComponent_var m1;
    
      for (int iter = 0; iter < 3 ; iter++)
	{
	  MESSAGE("----------------------------------------------------" << iter);   
    
	  obj = iGenFact->load_impl("SalomeTestComponent",
				    "../lib/libSalomeTestComponentEngine.so");
	  m1 = Engines::TestComponent::_narrow(obj);
	  MESSAGE("recup m1");
	  SCRUTE(m1->instanceName());
	  MESSAGE("Coucou " << m1->Coucou(1L));
	  iGenFact->remove_impl(m1) ;
	  //iGenFact->finalize_removal() ; // unpredictable results ...
          sleep(5);
	}    
      // Clean-up.
      iGenFact->finalize_removal() ;
      orb->destroy();
    }
  catch(CORBA::COMM_FAILURE& ex) {
    INFOS("Caught system exception COMM_FAILURE -- unable to contact the object.")
      }
  catch(CORBA::SystemException&) {
    INFOS("Caught a CORBA::SystemException.")
      }
  catch(CORBA::Exception&) {
    INFOS("Caught CORBA::Exception.")
      }
  catch(...) {
    INFOS("Caught unknown exception.")
      }

  return 0;
}

