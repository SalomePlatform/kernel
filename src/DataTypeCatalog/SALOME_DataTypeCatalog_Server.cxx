using namespace std;
// File: SALOME_DataTypeCatalog_Server.cxx
// Created: Mon Aug 08 2001
// Author: Estelle Deville
// Project: SALOME
// Copyright : CEA/DEN/DMSS/LGLS
// $Header$
#include <iostream.h>
#include "SALOME_NamingService.hxx"
#include "SALOME_DataTypeCatalog_impl.hxx"
#include "utilities.h"

int main(int argc,char **argv)
{
  try {
  CosNaming::NamingContext_var _rootContext, catalogContext;

  // initialize the ORB

  CORBA::ORB_ptr orb = CORBA::ORB_init (argc, argv);

  // initialize POA

  CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");

  PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
  PortableServer::POAManager_var mgr = poa->the_POAManager(); 

  // Active catalog

  SALOME_DataTypeCatalogImpl* Catalogue_i = new SALOME_DataTypeCatalogImpl(argc, argv);
  poa->activate_object (Catalogue_i);

  mgr->activate();

  
  CORBA::Object_ptr myCata = Catalogue_i->_this();

  // initialise Naming Service
  SALOME_NamingService *_NS;
  _NS = new SALOME_NamingService(orb);
  // register Catalog in Naming Service
  _NS->Register(myCata ,"/Kernel/DataTypeCatalog");

  MESSAGE("Running DataType Catalog Server.")

  orb->run();
 
  poa->destroy(1,1);
 
  }
    catch(CORBA::SystemException&) {
    INFOS("Caught CORBA::SystemException.")
  }
    catch(CORBA::Exception&) {
    INFOS("Caught CORBA::Exception.")
  }

  return 0;
}
