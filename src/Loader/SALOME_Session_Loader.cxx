//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOME_Session_loader.cxx
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

using namespace std;
/*! \file SALOME_Session_loader.cxx
 */

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_Session)

#include "Utils_ORB_INIT.hxx"
#include "Utils_SINGLETON.hxx"
#include "SALOME_NamingService.hxx"
#include "utilities.h"

//! CORBA client for SALOME Session server : launch GUI
/*!
 *  SALOME_Session Loader launches a SALOME GUI on the SALOME session servant process
 */

#include <qapplication.h>
#include "InquireServersQThread.h"

int main(int argc, char **argv)
{
//VRV: T2.4 - Trace management improvement
  QApplication myQApp(argc, argv) ;
  InquireServersGUI myIS;
  myQApp.setMainWidget(&myIS);
  ASSERT(QObject::connect(&myQApp, SIGNAL(lastWindowClosed()), &myQApp, SLOT(quit()) ) );
  myIS.show();
  myQApp.exec();

  if (myIS.getExitStatus())
    exit(1);
//VRV: T2.4 - Trace management improvement
  if (myIS.withGUI()) {
    try
      {
	CORBA::ORB_ptr orb = CORBA::ORB_init(argc,argv) ;
	
	SALOME_NamingService &NS = *SINGLETON_<SALOME_NamingService>::Instance() ;
	ASSERT(SINGLETON_<SALOME_NamingService>::IsAlreadyExisting()) ;
	NS.init_orb( orb ) ;
	
	CORBA::Object_var obj = NS.Resolve("/Kernel/Session");
	
	SALOME::Session_var session = SALOME::Session::_narrow(obj) ;
	ASSERT(! CORBA::is_nil(session));
	MESSAGE("SALOME::Session::_narrow(obj)");
	INFOS("Corba initialisation, Distant server");
	
	// -------------------------------------------------------------
	
	session->GetInterface() ;

	// -------------------------------------------------------------
	
	orb->destroy() ;
      }
    catch (ServiceUnreachable&)
      {
	INFOS("Caught exception: Naming Service Unreachable");
      }
    catch (CORBA::COMM_FAILURE&)
      {
	INFOS("Caught CORBA::SystemException CommFailure.");
      }
    catch (CORBA::SystemException&)
      {
	INFOS("Caught CORBA::SystemException.");
      }
    catch (CORBA::Exception&)
      {
	INFOS("Caught CORBA::Exception.");
      }
    catch (...)
      {
	INFOS("Caught unknown exception.");
      }
    return 0 ;
  }
}


