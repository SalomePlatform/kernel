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
//  File   : SALOME_Session_Server.cxx
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

# include "Utils_ORB_INIT.hxx"
# include "Utils_SINGLETON.hxx"

#include <iostream>
#include <unistd.h>
using namespace std;

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Session)

#include "utilities.h"

#include "SALOME_Session_i.hxx"

//! CORBA server for SALOME Session
/*!
 *  SALOME_Session Server launches a SALOME session servant.
 *  The servant registers to the Naming Service.
 *  See SALOME_Session.idl for interface specification.
 *
 *  Main services offered by the servant are:
 *   - launch GUI
 *   - stop Session (must be idle)
 *   - get session state
 */

int main(int argc, char **argv)
{

  try
    {
    ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance() ;
    ASSERT(SINGLETON_<ORB_INIT>::IsAlreadyExisting()) ;
    CORBA::ORB_var &orb = init( argc , argv ) ;

    CORBA::Object_var obj =orb->resolve_initial_references("RootPOA") ;
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj) ;

    // servant
    SALOME_Session_i * mySALOME_Session = new SALOME_Session_i(argc, argv, orb, poa) ;
    PortableServer::ObjectId_var mySALOME_Sessionid = poa->activate_object(mySALOME_Session) ;
    MESSAGE("poa->activate_object(mySALOME_Session)")

    obj = mySALOME_Session->_this() ;
    CORBA::String_var sior(orb->object_to_string(obj)) ;

    mySALOME_Session->NSregister();

    mySALOME_Session->_remove_ref() ;

    PortableServer::POAManager_var pman = poa->the_POAManager() ;
    pman->activate() ;
    MESSAGE("pman->activate()")

    orb->run() ;
    orb->destroy() ;
    }
    catch (CORBA::SystemException&)
    {
      INFOS("Caught CORBA::SystemException.")
    }
    catch (CORBA::Exception&)
    {
      INFOS("Caught CORBA::Exception.")
    }
    catch (...)
    {
      INFOS("Caught unknown exception.")
    }
  return 0 ;
}
