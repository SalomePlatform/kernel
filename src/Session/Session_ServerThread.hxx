//  SALOME Session : implementation of Session_ServerThread.hxx
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
//  File   : Session_ServerThread.hxx
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

#ifndef _SESSION_SERVERTHREAD_HXX_
#define _SESSION_SERVERTHREAD_HXX_

#include <CORBA.h> 
#include <string>

#include "SALOME_NamingService.hxx"
#include <qthread.h>

using namespace std;

class Session_ServerThread
{
public:
  static const int NB_SRV_TYP;
  static const char* _serverTypes[];

  Session_ServerThread();
  Session_ServerThread(int argc,
		       char ** argv, 
		       CORBA::ORB_ptr orb, 
		       PortableServer::POA_ptr poa,
		       QMutex *GUIMutex);
  virtual ~Session_ServerThread();
  void Init();
protected:
  void ActivateModuleCatalog(int argc,
			     char ** argv);
  void ActivateSALOMEDS(int argc,
			char ** argv);
  void ActivateRegistry(int argc,
			char ** argv);
  void ActivateContainer(int argc,
			char ** argv);
  virtual void ActivateSession(int argc,
			char ** argv);

protected:
  int _argc;
  char ** _argv;
  int _servType;
  CORBA::ORB_var _orb;
  PortableServer::POA_var _root_poa;
  QMutex* _GUIMutex;
  SALOME_NamingService *_NS;
};

class Session_SessionThread : public Session_ServerThread
{
public:
  Session_SessionThread() {}
  Session_SessionThread(int argc,
		       char** argv, 
		       CORBA::ORB_ptr orb, 
		       PortableServer::POA_ptr poa,
		       QMutex* GUIMutex,
                       QWaitCondition* GUILauncher);
  virtual ~Session_SessionThread();  

protected:
  virtual void ActivateSession(int argc,
			char ** argv);
private:
  QWaitCondition* _GUILauncher;
};

#endif

