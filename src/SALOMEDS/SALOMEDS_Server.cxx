//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
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
//  File   : SALOMEDS_Server.cxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

using namespace std;
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include "SALOMEDS_StudyManager_i.hxx"
#include "utilities.h"

//#define CHECKTIME
#ifdef CHECKTIME
#include <Utils_Timer.hxx>
#endif

// extern "C"
// { // for ccmalloc memory debug
// void ccmalloc_static_initialization(void);
// }

///////////////////////////// MAIN ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
//   ccmalloc_static_initialization();

  MESSAGE("SALOMEDS_Server - main");
  try {
    // Initialise the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, "omniORB3");
    omniORB::MaxMessageSize(100 * 1024 * 1024);
    
    // Obtain a reference to the root POA.
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);


    // We allocate the objects on the heap.  Since these are reference
    // counted objects, they will be deleted by the POA when they are no
    // longer needed.    
    SALOMEDS_StudyManager_i * myStudyManager_i = new  SALOMEDS_StudyManager_i(orb);
 
    // Activate the objects.  This tells the POA that the objects are
    // ready to accept requests.
    PortableServer::ObjectId_var myStudyManager_iid = poa->activate_object(myStudyManager_i);
    myStudyManager_i->register_name("/myStudyManager");
       
    // Obtain a POAManager, and tell the POA to start accepting
    // requests on its objects.
    PortableServer::POAManager_var pman = poa->the_POAManager();
    pman->activate();

#ifdef CHECKTIME
    Utils_Timer timer;
    timer.Start();
    timer.Stop();
    MESSAGE("SALOME_Registry_Server.cxx - orb->run()");
    timer.ShowAbsolute();
#endif
    orb->run();
    orb->destroy();
  }
  catch(CORBA::SystemException&) {
    MESSAGE( "Caught CORBA::SystemException." )
  }
  catch(CORBA::Exception&) {
    MESSAGE( "Caught CORBA::Exception." )
  }
  catch(omniORB::fatalException& fe) {
    MESSAGE( "Caught omniORB::fatalException:" )
    MESSAGE( "  file: " << fe.file() )
    MESSAGE( "  line: " << fe.line() )
    MESSAGE( "  mesg: " << fe.errmsg() )
  }
  catch(...) {
    MESSAGE( "Caught unknown exception." )
  }
  return 0;
}
