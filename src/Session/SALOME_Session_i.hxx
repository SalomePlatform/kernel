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
//  File   : SALOME_Session_i.hxx
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

#ifndef _SALOME_SESSION_I_HXX_
#define _SALOME_SESSION_I_HXX_

#include <qthread.h> 

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Component)
#include CORBA_SERVER_HEADER(SALOME_Session)
class SALOME_NamingService;
class SALOME_Session_QThread;

class SALOME_Session_i:  public virtual POA_SALOME::Session,
		         public virtual PortableServer::RefCountServantBase
{
public:
  SALOME_Session_i(int argc, char ** argv, CORBA::ORB_ptr orb, PortableServer::POA_ptr poa, QMutex* GUIMutex);
  ~SALOME_Session_i();

  //! Launch Graphical User Interface
  void GetInterface();

  //! Return VISU component
  Engines::Component_ptr GetVisuComponent();

  //! Stop the Session (must be idle): kill servant & server
  void StopSession();

  //! Get session state
  SALOME::StatSession GetStatSession();

  //! Register the servant to Naming Service
  void NSregister();
  
  void ping(){};

protected:

  //! Naming service interface
  SALOME_NamingService *_NS;

  int _argc ;
  char **_argv;
  CORBA::Boolean _isGUI ;
  QMutex* _GUIMutex ;
  int _runningStudies ;
  CORBA::ORB_var _orb;
  PortableServer::POA_var _poa;
};

#endif

