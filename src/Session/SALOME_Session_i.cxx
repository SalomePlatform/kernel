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
//  File   : SALOME_Session_i.cxx
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

#include "utilities.h"

#include "SALOME_Session_i.hxx"
#include "SALOME_NamingService.hxx"
#include "SALOME_Event.hxx"

#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include <qapplication.h>

// Open CASCADE Includes
#include <OSD_SharedLibrary.hxx>
#include <OSD_LoadMode.hxx>
#include <OSD_Function.hxx>
using namespace std;

//=============================================================================
/*! SALOME_Session_i
 *  constructor
 */ 
//=============================================================================

SALOME_Session_i::SALOME_Session_i(int argc, 
				   char ** argv, 
				   CORBA::ORB_ptr orb, 
				   PortableServer::POA_ptr poa, 
				   QMutex* GUIMutex,
				   QWaitCondition* GUILauncher)
{
  _argc = argc ;
  _argv = argv ;
  _isGUI = FALSE ;
  _runningStudies= 0 ;
  _orb = CORBA::ORB::_duplicate(orb) ;
  _poa = PortableServer::POA::_duplicate(poa) ;
  _GUIMutex = GUIMutex;
  _GUILauncher = GUILauncher;
  //MESSAGE("constructor end");
}

//=============================================================================
/*! GetVisuComponent
 *  returns Visu component
 */ 
//=============================================================================

Engines::Component_ptr SALOME_Session_i::GetVisuComponent()
{
  //MESSAGE("SALOME_Session_i::GetVisuGen");
  typedef Engines::Component_ptr TGetImpl(CORBA::ORB_ptr,
					 PortableServer::POA_ptr,
					 SALOME_NamingService*,QMutex*);
  OSD_SharedLibrary  aSharedLibrary("libVISUEngineImpl.so");
  if(aSharedLibrary.DlOpen(OSD_RTLD_LAZY))
    if(OSD_Function anOSDFun = aSharedLibrary.DlSymb("GetImpl"))
      return ((TGetImpl (*)) anOSDFun)(_orb,_poa,_NS,_GUIMutex);
  return Engines::Component::_nil();
}

//=============================================================================
/*! ~SALOME_Session_i
 *  destructor
 */ 
//=============================================================================

SALOME_Session_i::~SALOME_Session_i()
{
  //MESSAGE("destructor end"); 
}

//=============================================================================
/*! NSregister
 *  tries to find the Corba Naming Service and to register the session,
 *  gives naming service interface to _IAPPThread
 */ 
//=============================================================================

void SALOME_Session_i::NSregister()
{
  SALOME::Session_ptr pSession = SALOME::Session::_narrow(_this());
  try
    {
      _NS = new SALOME_NamingService(_orb);
      _NS->Register(pSession, "/Kernel/Session");
    }
  catch (ServiceUnreachable&)
    {
      INFOS("Caught exception: Naming Service Unreachable");
      exit(1) ;
    }
  catch (...)
    {
      INFOS("Caught unknown exception from Naming Service");
    }
  //MESSAGE("Session registered in Naming Service"); 
}

//=============================================================================
/*! GetInterface
 *  Launches the GUI if there is none.
 *  The Corba method is oneway (corba client does'nt wait for GUI completion)
 */ 
//=============================================================================

void SALOME_Session_i::GetInterface()
{
  if( !QAD_Application::getDesktop() ) {
    _GUILauncher->wakeAll();
    MESSAGE("SALOME_Session_i::GetInterface() called, starting GUI...")
  }
}

//=============================================================================
/*! StopSession
 *  Kills the session if there are no active studies nore GUI
 */ 
//=============================================================================
class CloseEvent : public SALOME_Event
{
public:
  virtual void Execute() {
    if ( QAD_Application::getDesktop() )
      QAD_Application::getDesktop()->closeDesktop( true );
  }
};

void SALOME_Session_i::StopSession()
{
  ProcessVoidEvent( new CloseEvent() );
}
 
//=============================================================================
/*! StatSession
 *  Send a SALOME::StatSession structure (see idl) to the client
 *  (number of running studies and presence of GUI)
 */ 
//=============================================================================

class QtLock
{
public:
  QtLock() { if ( qApp ) qApp->lock(); }
  ~QtLock() { if ( qApp ) qApp->unlock(); }
};


SALOME::StatSession SALOME_Session_i::GetStatSession()
{
  // update Session state
  _GUIMutex->lock();    

  _runningStudies = 0;
  {
    QtLock lock;
    _isGUI = QAD_Application::getDesktop();
    if ( _isGUI && QAD_Application::getDesktop()->getActiveApp() )
      _runningStudies = QAD_Application::getDesktop()->getActiveApp()->getStudies().count();
  }

  _GUIMutex->unlock();

  // getting stat info
  SALOME::StatSession_var myStats = new SALOME::StatSession ;
  if (_runningStudies)
    myStats->state = SALOME::running ;
  else
    myStats->state = SALOME::asleep ;
  myStats->runningStudies = _runningStudies ;
  myStats->activeGUI = _isGUI ;
  return myStats._retn() ;
}

CORBA::Long SALOME_Session_i::GetActiveStudyId()
{
  long aStudyId=-1;
  if( QAD_Application::getDesktop() && QAD_Application::getDesktop()->getActiveStudy()) {
    aStudyId = QAD_Application::getDesktop()->getActiveStudy()->getStudyId();
  }
  return aStudyId;
}
