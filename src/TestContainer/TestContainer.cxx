//  SALOME TestContainer : test of container creation and its life cycle
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
//  File   : TestContainer.cxx
//  Author : Paul RASCLE, EDF - MARC TAJCHMAN, CEA
//  Module : SALOME
//  $Header$

using namespace std;
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
      containerName += hostName + "/FactoryServer";

      obj = _NS.Resolve(containerName.c_str()) ;
      Engines::Container_var iGenFact = Engines::Container::_narrow(obj);

      Engines::TestComponent_var m1;
    
      for (int iter = 0; iter < 3 ; iter++)
	{
	  INFOS("----------------------------------------------------" << iter);   
          string dirn = getenv("SALOME_ROOT_DIR");
          dirn += "/lib/salome/libSalomeTestComponentEngine.so";
          obj = iGenFact->load_impl("SalomeTestComponent",dirn.c_str());
	  m1 = Engines::TestComponent::_narrow(obj);
	  INFOS("recup m1");
	  SCRUTE(m1->instanceName());
	  INFOS("Coucou " << m1->Coucou(1L));
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

