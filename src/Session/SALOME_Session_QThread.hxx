//=============================================================================
// File      : SALOME_Session_QThread.hxx
// Created   : mar jun 19 14:38:31 CEST 2001
// Author    : Paul RASCLE, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

#ifndef _SALOME_SESSION_QTHREAD_HXX_
#define _SALOME_SESSION_QTHREAD_HXX_

#include <qthread.h> 
class QApplication;

class SALOMEGUI_Application;
class SALOME_NamingService;


//! Qt thread class for the GUI
class SALOME_Session_QThread: public QThread
{

public:
  SALOME_Session_QThread(int argc, char ** argv) ;

  //! launch the Qt main window of the GUI on a separate thread
  virtual void run() ;

  //! set the naming service
  void setNamingService(SALOME_NamingService *ns);

protected:
  int _argc ;
  char ** _argv;
  int _ret ;
  SALOME_NamingService *_NS;
  QApplication *_qappl ;

  SALOMEGUI_Application* _mw ;
} ;

#endif
