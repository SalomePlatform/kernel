using namespace std;
//  File      : SALOMEDS_Server.cxx
//  Created   : Wed Nov 28 16:26:15 2001
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header$

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include "SALOMEDS_StudyManager_i.hxx"
#include "utilities.h"

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

