//  SALOME Session : implementation of Session.idl
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
//  File   : SALOME_Session_Server.cxx
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

#include "Utils_ORB_INIT.hxx"
#include "Utils_SINGLETON.hxx"
#include "SALOME_NamingService.hxx"
#include <iostream>
#include <unistd.h>

#include <qthread.h> 
#include <qapplication.h>
#include <qlabel.h>
#include <qwaitcondition.h>

#include "SALOMEGUI_Application.h"
#include "QAD.h"
#include "QAD_MessageBox.h"
#include "QAD_Application.h"
#include "QAD_Settings.h"
#include "QAD_Config.h"
#include "QAD_Tools.h"
#include "QAD_ResourceMgr.h"
#include "Utils_SALOME_Exception.hxx"
#include "Utils_CorbaException.hxx"
#include "SALOMEGUI_QtCatchCorbaException.hxx"
#include "SALOME_Event.hxx"

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Session)
#include CORBA_SERVER_HEADER(SALOMEDS)

#include "utilities.h"
#include "LocalTraceCollector.hxx"

#include "SALOME_Session_i.hxx"

#include "Session_ServerLauncher.hxx"
using namespace std;

extern "C" int HandleSignals(QApplication *theQApplication);

/*! - read arguments, define list of server to launch with their arguments.
 *  - wait for naming service
 *  - create and run a thread for launch of all servers
 *  
 */

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

using namespace std;

#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif

int main(int argc, char **argv)
{
  SALOME_Event::GetSessionThread();
  ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance() ;
  int orbArgc = 1;
  CORBA::ORB_var &orb = init( orbArgc , argv ) ;
  LocalTraceCollector *myThreadTrace = LocalTraceCollector::instance(orb);
  try
    {
      CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
      PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);

      PortableServer::POAManager_var pman = poa->the_POAManager() ;
      pman->activate() ;
      INFOS("pman->activate()");

      SALOME_NamingService *_NS = new SALOME_NamingService(orb);

      // CORBA Servant Launcher
      QMutex _GUIMutex ;
      QWaitCondition _ServerLaunch;
      _GUIMutex.lock();     // to block Launch server thread until wait(mutex)

      Session_ServerLauncher* myServerLauncher
	= new Session_ServerLauncher(argc, argv, orb, poa, &_GUIMutex, &_ServerLaunch);
      myServerLauncher->start();

      QApplication *_qappl = 0;

      // Allow multiple activation/deactivation of GUI
      while ( 1 ) {
	MESSAGE("waiting wakeAll()");
	_ServerLaunch.wait(&_GUIMutex); // to be reseased by Launch server thread when ready:
	// atomic operation lock - unlock on mutex
	// unlock mutex: serverThread runs, calls  _ServerLaunch->wakeAll()
	// this thread wakes up, and lock mutex

	INFOS("Session activated, Launch IAPP...");

	int qappArgc = 1;
	if ( !_qappl )
	  _qappl = new QApplication(qappArgc, argv );

	INFOS("creation QApplication");
	_GUIMutex.unlock();

	QAD_ASSERT ( QObject::connect(_qappl, SIGNAL(lastWindowClosed()), _qappl, SLOT(quit()) ) );
	SALOMEGUI_Application* _mw = new SALOMEGUI_Application ( "MDTV-Standard", "HDF", "hdf" );
	INFOS("creation SALOMEGUI_Application");
	
	SCRUTE(_NS);
	if ( !SALOMEGUI_Application::addToDesktop ( _mw, _NS ) ) {
	  QAD_MessageBox::error1 ( 0,
				   QObject::tr("ERR_ERROR"), 
				   QObject::tr("ERR_APP_INITFAILED"),
				   QObject::tr("BUT_OK") );
	}
	else {
	  QFileInfo prgInfo(argv[0]);
	  QDir prgDir(prgInfo.dirPath(true));
	  QAD_CONFIG->setPrgDir(prgDir);        // CWD is program directory
	  QAD_CONFIG->createConfigFile(false);  // Create config file
	                                        // ~/.tr(MEN_APPNAME)/tr(MEN_APPNAME).conf if there's none
	  QAD_CONFIG->readConfigFile();         // Read config file
	  
	  _qappl->setPalette( QAD_Application::getPalette() ); 

	  /* Run 'SALOMEGUI' application */
	  QAD_Application::run();

	  // T2.12 - catch exceptions thrown on attempts to modified a locked study
	  while (1) {
	    try 
	      {
		MESSAGE("run(): starting the main event loop");
		int _ret = HandleSignals(_qappl);
		break;
	      }
	    catch (SALOME::SALOME_Exception& e)
	      {
		QtCatchCorbaException(e);
	      }
	    catch(SALOMEDS::StudyBuilder::LockProtection&)
	      {
		INFOS("run(): An attempt to modify a locked study has not been handled by QAD_Operation");
		QApplication::restoreOverrideCursor();
		QAD_MessageBox::warn1 ( (QWidget*)QAD_Application::getDesktop(),
				       QObject::tr("WRN_WARNING"), 
				       QObject::tr("WRN_STUDY_LOCKED"),
				       QObject::tr("BUT_OK") );
	      }
	    catch (const CORBA::Exception& e)
	      {
		CORBA::Any tmp;
		tmp<<= e;
		CORBA::TypeCode_var tc = tmp.type();
		const char *p = tc->name();
		INFOS ("run(): CORBA exception of the kind : "<<p<< " is caught");

		QApplication::restoreOverrideCursor();
		QAD_MessageBox::error1 ( (QWidget*)QAD_Application::getDesktop(),
					QObject::tr("ERR_ERROR"), 
					QObject::tr("ERR_APP_EXCEPTION")
					+ QObject::tr(" CORBA exception ") + QObject::tr(p),
					QObject::tr("BUT_OK") );
	      }
	    catch(std::exception& e)
	      {
		INFOS("run(): An exception has been caught");
		QApplication::restoreOverrideCursor();
		QAD_MessageBox::error1 ( (QWidget*)QAD_Application::getDesktop(),
					QObject::tr("ERR_ERROR"), 
					QObject::tr("ERR_APP_EXCEPTION")+ "\n" +QObject::tr(e.what()),
					QObject::tr("BUT_OK") );
	      }
	    catch(...)
	      {
		INFOS("run(): An exception has been caught");
		QApplication::restoreOverrideCursor();
		QAD_MessageBox::error1 ( (QWidget*)QAD_Application::getDesktop(),
					QObject::tr("ERR_ERROR"), 
					QObject::tr("ERR_APP_EXCEPTION"),
					QObject::tr("BUT_OK") );
	      }
	  }
	  QString confMsg = "Settings create " 
	    + QAD_CONFIG->getConfigDir().absPath() + "/" + QObject::tr("MEN_APPNAME") + ".conf";
	  MESSAGE (confMsg);
	  QAD_CONFIG->createConfigFile(true);
	}

	// Prepare _GUIMutex for a new GUI activation
	_GUIMutex.lock(); 
      }

      //orb->shutdown(0);
      myServerLauncher->KillAll();
    }
  catch (SALOME_Exception& e)
    {
      INFOS("run(): SALOME::SALOME_Exception is caught: "<<e.what());
    }
  catch (CORBA::SystemException& e)
    {
      INFOS("Caught CORBA::SystemException.");
    }
  catch (CORBA::Exception& e)
    {
      INFOS("Caught CORBA::Exception.");
      CORBA::Any tmp;
      tmp<<= e;
      CORBA::TypeCode_var tc = tmp.type();
      const char *p = tc->name();
      INFOS ("run(): CORBA exception of the kind : "<<p<< " is caught");
    }
  catch(std::exception& e)
    {
      INFOS("run(): An exception has been caught: " <<e.what());
    }
  catch (...)
    {
      INFOS("Caught unknown exception.");
    }
  MESSAGE("End of SALOME_Session_Server");
  delete myThreadTrace;
  return 0 ;
}
