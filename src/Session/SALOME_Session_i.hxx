//=============================================================================
// File      : SALOME_Session_i.hxx
// Created   : mar jun 19 13:42:55 CEST 2001
// Author    : Paul RASCLE, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

#ifndef _SALOME_SESSION_I_HXX_
#define _SALOME_SESSION_I_HXX_

#include <qthread.h> 

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Component)
//***//#include CORBA_SERVER_HEADER(VISU_Gen)
#include CORBA_SERVER_HEADER(SALOME_Session)
class SALOME_NamingService;
class SALOME_Session_QThread;

//! CORBA SALOME Session Servant

class SALOME_Session_i:  public POA_SALOME::Session,
		         public PortableServer::RefCountServantBase
{
public:
  SALOME_Session_i(int argc, char ** argv, CORBA::ORB_ptr orb, PortableServer::POA_ptr poa) ;
  ~SALOME_Session_i() ;

  //! Launch Graphical User Interface
  void GetInterface();
  //***//VISU::VISU_Gen_ptr GetVisuGen();
  Engines::Component_ptr GetVisuComponent();

  //! Stop the Session (must be idle): kill servant & server
  void StopSession();

  //! Get session state
  SALOME::StatSession GetStatSession();

  //! Register the servant to Naming Service
  void NSregister();
  
  void ping(){};

protected:
  //! A QT Thread for the GUI
  SALOME_Session_QThread *_IAPPThread ;

  //! Naming service interface
  SALOME_NamingService *_NS;

  int _argc ;
  char **_argv;
  CORBA::Boolean _isGUI ;
  QMutex _GUIMutex ;
  int _runningStudies ;
  CORBA::ORB_var _orb;
  PortableServer::POA_var _poa;
};

#endif

