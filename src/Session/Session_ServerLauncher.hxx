//  SALOME Session : implementation of Session_ServerLauncher.hxx
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
//  File   : Session_ServerLauncher.hxx
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

#ifndef _SESSION_SERVERLAUNCHER_HXX_
#define _SESSION_SERVERLAUNCHER_HXX_

#include "Session_ServerThread.hxx"

#include <CORBA.h> 
#include <list>
#include <vector>
#include <string>
#include <qthread.h>
#include <qwaitcondition.h>


class ServArg
  {
  public:
    int _servType;
    int _firstArg;
    int _lastArg;
    inline ServArg(int servType=0, int firstArg=0, int lastArg=0);
};

inline ServArg::ServArg(int servType, int firstArg, int lastArg):
  _servType(servType),_firstArg(firstArg),_lastArg(lastArg)
{}

class Session_ServerLauncher: public QThread
{
public:
  Session_ServerLauncher();
  Session_ServerLauncher(int argc,
			 char ** argv, 
			 CORBA::ORB_ptr orb, 
			 PortableServer::POA_ptr poa,
			 QMutex *GUIMutex,
			 QWaitCondition *ServerLaunch);
  virtual ~Session_ServerLauncher();
  void run();
  void KillAll();

protected:
  void CheckArgs();
  void ActivateAll();

private:
  int _argc;
  char ** _argv;
  CORBA::ORB_var _orb;
  PortableServer::POA_var _root_poa;
  QMutex* _GUIMutex;
  QWaitCondition *_ServerLaunch;
  std::list<ServArg> _argServToLaunch;
  std::vector<std::string> _argCopy;
  std::list<Session_ServerThread*> _serverThreads;
};

#endif

