using namespace std;
//=============================================================================
// File      : SALOME_Session_Server.cxx
// Created   : mar jun 19 13:26:24 CEST 2001
// Author    : Paul RASCLE, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

/*! \file SALOME_Session_Server.cxx
 */

using namespace std;
# include "Utils_ORB_INIT.hxx"
# include "Utils_SINGLETON.hxx"

#include <iostream.h>
#include <unistd.h>

// #include <qapplication.h>
// #include "Qtappl_example.hxx"

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Session)

#include "utilities.h"

#include "SALOME_Session_i.hxx"

//! CORBA server for SALOME Session
/*!
 *  SALOME_Session Server launches a SALOME session servant.
 *  The servant registers to the Naming Service.
 *  See SALOME_Session.idl for interface specification.
 *
 *  Main services offered by the servant are:
 *   - launch GUI
 *   - stop Session (must be idle)
 *   - get session state
 */

int main(int argc, char **argv)
{

  try
    {
    ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance() ;
    ASSERT(SINGLETON_<ORB_INIT>::IsAlreadyExisting()) ;
    CORBA::ORB_var &orb = init( argc , argv ) ;

    CORBA::Object_var obj =orb->resolve_initial_references("RootPOA") ;
    //MESSAGE("orb->resolve_initial_references('Root_POA')")
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj) ;
    //MESSAGE("POA::_narrow(obj)")

    // servant

    SALOME_Session_i * mySALOME_Session = new SALOME_Session_i(argc, argv, orb, poa) ;
    PortableServer::ObjectId_var mySALOME_Sessionid = poa->activate_object(mySALOME_Session) ;
    MESSAGE("poa->activate_object(mySALOME_Session)")

    obj = mySALOME_Session->_this() ;
    CORBA::String_var sior(orb->object_to_string(obj)) ;
    // MESSAGE("sior(orb->object-to-string(obj))")
    // cerr << "'" << (char*) sior << "'" << endl ;

    mySALOME_Session->NSregister();

    mySALOME_Session->_remove_ref() ;
    //MESSAGE("mySALOME_Session->_remove_ref()")

    PortableServer::POAManager_var pman = poa->the_POAManager() ;
    pman->activate() ;
    MESSAGE("pman->activate()")

    orb->run() ;
    orb->destroy() ;
    }
    catch (CORBA::SystemException&)
    {
      INFOS("Caught CORBA::SystemException.")
    }
    catch (CORBA::Exception&)
    {
      INFOS("Caught CORBA::Exception.")
    }
    catch (...)
    {
      INFOS("Caught unknown exception.")
    }
  return 0 ;
}
