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
//  File   : SALOME_Session_QThread.hxx
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

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
