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

#include "Utils_ORB_INIT.hxx"
#include "Utils_SINGLETON.hxx"
#include "SALOME_NamingService.hxx"
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

      //
      long TIMESleep = 250000000;
      int NumberOfTries = 40;
      int a;
      timespec ts_req;
      ts_req.tv_nsec=TIMESleep;
      ts_req.tv_sec=0;
      timespec ts_rem;
      ts_rem.tv_nsec=0;
      ts_rem.tv_sec=0;
      CosNaming::NamingContext_var inc;
      PortableServer::POA_var poa;
      CORBA::Object_var theObj;
      CORBA::Object_var obj;
      CORBA::Object_var object;
      SALOME_NamingService &naming = *SINGLETON_<SALOME_NamingService>::Instance() ;
      int SESSION=0;
      const char * Env = getenv("USE_LOGGER"); 
      int EnvL =0;
      if ((Env!=NULL) && (strlen(Env)))
	EnvL=1;
      CosNaming::Name name;
      name.length(1);
      name[0].id=CORBA::string_dup("Logger");    
      PortableServer::POAManager_var pman; 
      for (int i = 1; i<=NumberOfTries; i++){
	if (i!=1) 
	  a=nanosleep(&ts_req,&ts_rem);
	try
	  { 
	    obj = orb->resolve_initial_references("RootPOA");
            if(!CORBA::is_nil(obj))
	      poa = PortableServer::POA::_narrow(obj);
            if(!CORBA::is_nil(poa))
	      pman = poa->the_POAManager();
	    if(!CORBA::is_nil(orb)) 
	      theObj = orb->resolve_initial_references("NameService");
	    if (!CORBA::is_nil(theObj))
	      inc = CosNaming::NamingContext::_narrow(theObj);
	  }
	catch( CORBA::COMM_FAILURE& )
	  {
	    MESSAGE( "Session Server: CORBA::COMM_FAILURE: Unable to contact the Naming Service" );
	  }
	if(!CORBA::is_nil(inc))
	  {
	    MESSAGE( "Session Server: Naming Service was found" );
	    if(EnvL==1)
	      {
		for(int j=1; j<=NumberOfTries; j++)
		  {
		    if (j!=1) 
		      a=nanosleep(&ts_req, &ts_rem);
		    try
		      {
			object = inc->resolve(name);
		      }
		    catch(CosNaming::NamingContext::NotFound)
		      { MESSAGE( "Session Server: Logger Server wasn't found" );
		      }
		    catch(...)
		      {
			MESSAGE( "Session Server: Unknown exception" ); 
		      }
		    if (!CORBA::is_nil(object))
		      {
			MESSAGE( "Session Server: Loger Server was found" );
			SESSION=1;
			break;
		      }
		  }
	      }
	  }
	if ((SESSION==1)||((EnvL==0)&&(!CORBA::is_nil(inc))))
	  break;
      }
   
      // servant
      SALOME_Session_i * mySALOME_Session = new SALOME_Session_i(argc, argv, orb, poa) ;
      PortableServer::ObjectId_var mySALOME_Sessionid = poa->activate_object(mySALOME_Session) ;
      MESSAGE("poa->activate_object(mySALOME_Session)")

	obj = mySALOME_Session->_this() ;
      CORBA::String_var sior(orb->object_to_string(obj)) ;

      mySALOME_Session->NSregister();

      mySALOME_Session->_remove_ref() ;

      //DECOMMENT PortableServer::POAManager_var pman = poa->the_POAManager() ;
      pman->activate() ;
      MESSAGE("pman->activate()")

	orb->run() ;
      orb->destroy() ;
    }
  catch (CORBA::SystemException&)
    {
      INFOS("Caught CORBA::SystemException.");
    }
  catch (CORBA::Exception&)
    {
      INFOS("Caught CORBA::Exception.");
    }
  catch (...)
    {
      INFOS("Caught unknown exception.");
    }
  return 0 ;
}
