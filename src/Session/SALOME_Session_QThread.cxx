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
//  File   : SALOME_Session_QThread.cxx
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

using namespace std;
#include <qapplication.h>
#include <qlabel.h>

#include "utilities.h"
#include "SALOME_Session_QThread.hxx"
#include "SALOME_NamingService.hxx"
#include "SALOMEGUI_Application.h"
#include "QAD.h"
#include "QAD_MessageBox.h"
#include "QAD_Application.h"
#include "QAD_Settings.h"
#include "QAD_Config.h"
#include "QAD_Tools.h"
// only to use findFile method : perhaps put it in QAD_Tools ???
#include "QAD_ResourceMgr.h"
#include <qlabel.h>

#include "Utils_CatchSignals.h"
#include "Utils_CorbaException.hxx"
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

//=============================================================================
/*! SALOME_Session_QThread
 *  constructor
 */
//=============================================================================

SALOME_Session_QThread::SALOME_Session_QThread(int argc, char ** argv) : QThread()
{
  _qappl = 0 ;
  _mw = 0 ;
  _argc = argc ;
  _argv = argv ;
  _NS = 0 ;
} ;


//=============================================================================
/*! run
 *  Creates one QApplication for the all the session life.
 *  Executes the Qt main window of the GUI on a separate thread
 */
//=============================================================================
void SALOME_Session_QThread::run()
{
  if (_qappl == 0)
    {
      _qappl = new QApplication(_argc, _argv );
      MESSAGE("creation QApplication");
    }

  QAD_ASSERT ( QObject::connect(_qappl, SIGNAL(lastWindowClosed()), _qappl, SLOT(quit()) ) );
  _mw = new SALOMEGUI_Application ( "MDTV-Standard", "HDF", "hdf" );
  MESSAGE("creation SALOMEGUI_Application");
  
  if ( !SALOMEGUI_Application::addToDesktop ( _mw, _NS ) )
    {
      QAD_MessageBox::error1 ( 0,
			       QObject::tr("ERR_ERROR"), 
			       QObject::tr("ERR_APP_INITFAILED"),
			       QObject::tr("BUT_OK") );
    }
  else
    {
      
      QFileInfo prgInfo(_argv[0]);
      QDir prgDir(prgInfo.dirPath(true));
      QAD_CONFIG->setPrgDir(prgDir);        // CWD is program directory
      QAD_CONFIG->createConfigFile(false);  // Create config file ~/.tr(MEN_APPNAME)/tr(MEN_APPNAME).conf if there's none
      QAD_CONFIG->readConfigFile();         // Read config file

      _qappl->setPalette( QAD_Application::getPalette() ); 

      Utils_CatchSignals aCatch;
      aCatch.Activate();
      
      /* Run 'SALOMEGUI' application */
      QAD_Application::run();
     
      // T2.12 - catch exceptions thrown on attempts to modified a locked study
      while (1) {
	try {
	  MESSAGE("run(): starting the main event loop");
	  _ret = _qappl->exec();
	  break;
	}
	catch (SALOME::SALOME_Exception& e) {
	  INFOS("run(): SALOME::SALOME_Exception is caught");
	  QApplication::restoreOverrideCursor();
	  QAD_MessageBox::error1 ( (QWidget*)QAD_Application::getDesktop(),
				  QObject::tr("ERR_ERROR"), 
				  QObject::tr("ERR_APP_EXCEPTION")+ QObject::tr(" SALOME::SALOME_Exception is caught") ,
				  QObject::tr("BUT_OK") );
	  
	}
	catch(SALOMEDS::StudyBuilder::LockProtection&) {
	  INFOS("run(): An attempt to modify a locked study has not been handled by QAD_Operation");
	  QApplication::restoreOverrideCursor();
	  QAD_MessageBox::warn1 ( (QWidget*)QAD_Application::getDesktop(),
				 QObject::tr("WRN_WARNING"), 
				 QObject::tr("WRN_STUDY_LOCKED"),
				 QObject::tr("BUT_OK") );
	}
	catch (const CORBA::Exception& e) {
	  CORBA::Any tmp;
	  tmp<<= e;
	  CORBA::TypeCode_var tc = tmp.type();
	  const char *p = tc->name();
	  INFOS ("run(): CORBA exception of the kind : "<<p<< " is caught");

	  QApplication::restoreOverrideCursor();
	  QAD_MessageBox::error1 ( (QWidget*)QAD_Application::getDesktop(),
			       QObject::tr("ERR_ERROR"), 
			       QObject::tr("ERR_APP_EXCEPTION")+ QObject::tr(" CORBA exception ") + QObject::tr(p),
			       QObject::tr("BUT_OK") );
	}
	catch(std::exception& e) {
	  INFOS("run(): An exception has been caught");
	  QApplication::restoreOverrideCursor();
	  QAD_MessageBox::error1 ( (QWidget*)QAD_Application::getDesktop(),
			       QObject::tr("ERR_ERROR"), 
			       QObject::tr("ERR_APP_EXCEPTION")+ " " +QObject::tr(e.what()),
			       QObject::tr("BUT_OK") );
	}
	catch(...) {
	  INFOS("run(): An exception has been caught")
	  QApplication::restoreOverrideCursor();
	  QAD_MessageBox::error1 ( (QWidget*)QAD_Application::getDesktop(),
			       QObject::tr("ERR_ERROR"), 
			       QObject::tr("ERR_APP_EXCEPTION"),
			       QObject::tr("BUT_OK") );
	}
      }
      
      aCatch.Deactivate();
      QString confMsg = "Settings create $HOME/." + QObject::tr("MEN_APPNAME") + "/" + QObject::tr("MEN_APPNAME") + ".conf";
      MESSAGE (confMsg )
      QAD_CONFIG->createConfigFile(true);
    }
} ;

//=============================================================================
/*! setNamingService
 *  gives the naming service interface 
 */
//=============================================================================
void SALOME_Session_QThread::setNamingService(SALOME_NamingService *ns)
{
  _NS = ns;
}
