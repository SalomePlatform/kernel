//  SALOME ModuleCatalog : implementation of ModuleCatalog server which parsers xml description of modules
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
//  File   : SALOME_ModuleCatalog_Server.cxx
//  Module : SALOME

using namespace std;
/* $Header$ */
#include <iostream.h>
#include "SALOME_NamingService.hxx"
#include "SALOME_ModuleCatalog_impl.hxx"
#include "utilities.h"

//#define CHECKTIME
#ifdef CHECKTIME
#include <Utils_Timer.hxx>
#endif

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

  SALOME_ModuleCatalogImpl* Catalogue_i = new SALOME_ModuleCatalogImpl(argc, argv);
  poa->activate_object (Catalogue_i);

  mgr->activate();

  
  CORBA::Object_ptr myCata = Catalogue_i->_this();

  // initialise Naming Service
  SALOME_NamingService *_NS;
  _NS = new SALOME_NamingService(orb);
  // register Catalog in Naming Service
  _NS->Register(myCata ,"/Kernel/ModulCatalog");

  MESSAGE("Running CatalogServer.");

#ifdef CHECKTIME
  Utils_Timer timer;
  timer.Start();
  timer.Stop();
  MESSAGE("SALOME_Registry_Server.cxx - orb->run()");
  timer.ShowAbsolute();
#endif
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
