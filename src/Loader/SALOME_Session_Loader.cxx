using namespace std;
//=============================================================================
// File      : SALOME_Session_Loader.cxx
// Created   : jeu jun 21 07:37:59 CEST 2001
// Author    : Paul RASCLE, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

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
//VRVcd: T2.4 - Trace management improvement
  return 0 ;
}


