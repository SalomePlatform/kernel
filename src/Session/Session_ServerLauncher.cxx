//  SALOME Session : implementation of Session_ServerLauncher.cxx
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
//  File   : Session_ServerLauncher.xx
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

#include "Session_ServerLauncher.hxx"

#include "Utils_SALOME_Exception.hxx"
#include "utilities.h"
using namespace std;

//=============================================================================
/*! 
 *  default constructor not for use
 */
//=============================================================================

Session_ServerLauncher::Session_ServerLauncher()
{
  ASSERT(0); // must not be called
}

//=============================================================================
/*! 
 *  constructor
 */
//=============================================================================

Session_ServerLauncher::Session_ServerLauncher(int argc,
					       char ** argv, 
					       CORBA::ORB_ptr orb, 
					       PortableServer::POA_ptr poa,
					       QMutex *GUIMutex,
					       QWaitCondition *ServerLaunch)
{
  _argc = argc;
  _argv = argv;
  _orb = CORBA::ORB::_duplicate(orb);
  _root_poa = PortableServer::POA::_duplicate(poa);
  _GUIMutex = GUIMutex;
  _ServerLaunch = ServerLaunch;
}

//=============================================================================
/*! 
 *  destructor
 */
//=============================================================================

Session_ServerLauncher::~Session_ServerLauncher()
{
}

//=============================================================================
/*! 
 *  Check args and activate servers
 */
//=============================================================================

void Session_ServerLauncher::run()
{
  //MESSAGE("Session_ServerLauncher::run");
  _GUIMutex->lock(); // lock released by calling thread when ready: wait(mutex)
  //MESSAGE("Server Launcher thread free to go...");
   _GUIMutex->unlock();

  CheckArgs();
  ActivateAll();

  _orb->run();       // this thread wait, during omniORB process events
}

//=============================================================================
/*! 
 *  controls and dispatchs arguments given with command
 */
//=============================================================================

void Session_ServerLauncher::CheckArgs()
{
  int argState = 0;
  ServArg aServArg(0,0,0);
  _argCopy.reserve(_argc);
  for (int iarg=0; iarg <_argc; iarg++)
    {
      SCRUTE(iarg);
      SCRUTE(_argv[iarg]);
      _argCopy.push_back(_argv[iarg]);
      switch (argState)
	{
	case 0: // looking for "--with"
	  {
	    if (strcmp(_argv[iarg],"--with")==0)
	      argState = 1;
	    break;
	  }
	case 1: // looking for server type
	  {
	    for (int i=0; i<Session_ServerThread::NB_SRV_TYP; i++)
		if (strcmp(_argv[iarg],Session_ServerThread::_serverTypes[i])==0)
		  {
		    aServArg._servType = i;
		    argState = 2;
		    break;
		  }
	    break;
	  }
	case 2: // looking for "("
	  {
	    if (strcmp(_argv[iarg],"(")!=0)
	      {
		INFOS("parenthesis '(' is required here...");
		for (int i=0; i<iarg; i++)
		  cerr << _argv[i] << " ";
		cerr << endl;
		throw SALOME_Exception(LOCALIZED("Error in command arguments, missing prenthesis"));
	      } 
	    else
	      {
		aServArg._firstArg=iarg+1;    // arg after '('
		argState = 3;
	      }
	    break;
	  }
	case 3: // looking for arguments
	  {
	    if (strcmp(_argv[iarg],")")==0)   // end of arguments = ')'
	      {
		aServArg._lastArg=iarg-1;     // arg before ')'
		MESSAGE("server : "<< Session_ServerThread::_serverTypes[aServArg._servType]);
		for (int i=aServArg._firstArg; i<=aServArg._lastArg; i++)
		  MESSAGE("  arg : " << _argCopy[i]);
		_argServToLaunch.push_back(aServArg);
		argState = 0;
	      } 
	    break;
	  }
	default:
	  {
	    ASSERT(0);
	    break;
	  }
	}
    }
  if (argState == 1)
    throw SALOME_Exception(LOCALIZED("Error in command arguments, missing server type"));
  if (argState == 2)
    throw SALOME_Exception(LOCALIZED("Error in command arguments, missing parenthesis '('"));
  if (argState == 3)
    throw SALOME_Exception(LOCALIZED("Error in command arguments, missing parenthesis ')'"));
}

//=============================================================================
/*! 
 *  
 */
//=============================================================================

void Session_ServerLauncher::ActivateAll()
{

  list<ServArg>::iterator itServ;
  for (itServ = _argServToLaunch.begin(); itServ !=_argServToLaunch.end(); itServ++)
    {
      int argc = 2 + (*itServ)._lastArg - (*itServ)._firstArg;
      char** argv = new char*[argc+1];
      argv[argc]=0; // for Engines_Container_i constructor...
      int servType = (*itServ)._servType;
      argv[0]=strdup(Session_ServerThread::_serverTypes[servType]);
      if (argc>1)
	{
	  for (int i=0; i<argc-1; i++)
	    //argv[i+1] = _argCopy[(*itServ)._firstArg + i].c_str();
	    argv[i+1] = _argv[(*itServ)._firstArg + i];
	}
      Session_ServerThread* aServerThread
	= new Session_ServerThread(argc, argv, _orb,_root_poa,_GUIMutex);
      _serverThreads.push_front(aServerThread);

      aServerThread->Init();
    }

  // Always launch Session Server

  int argc=1;
  char **argv = new char*[argc];
  argv[0] = "Session";
  Session_SessionThread* aServerThread
    = new Session_SessionThread(argc, argv, _orb,_root_poa,_GUIMutex,_ServerLaunch);
  _serverThreads.push_front(aServerThread);

  aServerThread->Init();
}

//=============================================================================
/*! 
 *  Destruction des classes serveur dans l'ordre inverse de creation
 */
//=============================================================================

void Session_ServerLauncher::KillAll()
{
  MESSAGE("Session_ServerLauncher::KillAll()");
  list<Session_ServerThread*>::reverse_iterator itServ;
  for (itServ = _serverThreads.rbegin(); itServ !=_serverThreads.rend(); itServ++)
    {
      delete (*itServ);
    }
}
