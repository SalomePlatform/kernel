//  SALOME Session : implementation of Session_ServerThread.cxx
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
//  File   : Session_ServerThread.cxx
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

// #include <SALOMEconfig.h>
// #include CORBA_SERVER_HEADER(SALOME_Session)
// #include CORBA_SERVER_HEADER(SALOMEDS)

#include "Session_ServerThread.hxx"

#include "SALOME_Container_i.hxx"
#include "SALOMEDS_StudyManager_i.hxx"
#include "SALOME_ModuleCatalog_impl.hxx"
#include "RegistryService.hxx"
#include "SALOME_Session_i.hxx"

#include "Utils_ORB_INIT.hxx"
#include "Utils_SINGLETON.hxx"
#include "Utils_SALOME_Exception.hxx"
#include "OpUtil.hxx"
#include "utilities.h"

#include <cstdlib>
#include <ctime>

using namespace std;

const int Session_ServerThread::NB_SRV_TYP = 5;
const char* Session_ServerThread::_serverTypes[NB_SRV_TYP] = {"Container",
							      "ModuleCatalog",
							      "Registry",
							      "SALOMEDS",
							      "Session"};

//=============================================================================
/*! 
 * Wait until the given server is ready i.e. is name is found in namingService.
 * Try 40 times, with 250 ms sleep between each try.
 * If Logger is used for traces, it must be ready before this call, because
 * SALOME_NamingService client uses SALOME traces. So, Logger readiness must be
 * checked in Launch script before execution of WaitForServerReadiness.
 */
//=============================================================================

void WaitForServerReadiness(SALOME_NamingService* NS, string serverName)
{
  long TIMESleep = 250000000; // 250 ms.
  int NumberOfTries = 40;     // total wait = 10 s.
  int found = 0;

  timespec ts_req;
  ts_req.tv_nsec=TIMESleep;
  ts_req.tv_sec=0;
  timespec ts_rem;
  ts_rem.tv_nsec=0;
  ts_rem.tv_sec=0;

  for (int itry=0; itry < NumberOfTries; itry++)
    {
      try
	{
	  if (serverName.length() == 0)
	    {
	      string curdir = NS->Current_Directory(); // to wait for naming service
	      found = 1;
	      break; // naming service found
	    }
	  else
	    {
	      CORBA::Object_ptr obj = NS->Resolve(serverName.c_str());
	      if (! CORBA::is_nil(obj))
		{
		  found =1;
		  break; // server found, no more try to do
		}
	      MESSAGE("Server "<< serverName <<" not yet ready, waiting...");
	      int a = nanosleep(&ts_req,&ts_rem); // wait before retry
	    }
	}
      catch( ServiceUnreachable& )
	{
	  MESSAGE("CORBA::COMM_FAILURE: Naming Service not yet ready, waiting...");
	  int a = nanosleep(&ts_req,&ts_rem); // wait before retry
	}
    }
  if (!found)
    {
    INFOS("Server "<< serverName <<" not found, abort...");
    exit(EXIT_FAILURE);
    }
}

//=============================================================================
/*! 
 *  default constructor not for use
 */
//=============================================================================

Session_ServerThread::Session_ServerThread()
{
  ASSERT(0); // must not be called
}

//=============================================================================
/*! 
 *  constructor
 */
//=============================================================================

Session_ServerThread::Session_ServerThread(int argc,
					   char ** argv, 
					   CORBA::ORB_ptr orb, 
					   PortableServer::POA_ptr poa,
					   QMutex *GUIMutex)
{
  MESSAGE("Session_ServerThread Constructor " << argv[0]);
  _argc = argc;
  _argv = argv;
  _orb = CORBA::ORB::_duplicate(orb);
  _root_poa = PortableServer::POA::_duplicate(poa);
  _GUIMutex = GUIMutex;
  _servType =-1;
  _NS = new SALOME_NamingService(_orb); // one instance per server to limit
                                        // multi thread coherence problems
}

//=============================================================================
/*! 
 *  destructor 
 */
//=============================================================================

Session_ServerThread::~Session_ServerThread()
{
  MESSAGE("~Session_ServerThread "<< _argv[0]);
}

//=============================================================================
/*! 
 *  run the thread : activate one servant, the servant type is given by
 *  argument _argv[0]
 */
//=============================================================================

void Session_ServerThread::Init()
{
  MESSAGE("Session_ServerThread::Init "<< _argv[0]); 

  for (int i=0; i<_argc; i++) SCRUTE(_argv[i]);
  for (int i=0; i<NB_SRV_TYP; i++)
    if (strcmp(_argv[0],_serverTypes[i])==0)
      {
	_servType = i;
	MESSAGE("Server Thread type : "<<_serverTypes[i]);
	switch (_servType)
	  {
	  case 0:  // Container
	    {
	      WaitForServerReadiness(_NS,"/Registry");
	      ActivateContainer(_argc, _argv);
	      break;
	    }
	  case 1:  // ModuleCatalog
	    {
	      WaitForServerReadiness(_NS,"/Registry");
	      ActivateModuleCatalog(_argc, _argv);
	      break;
	    }
	  case 2:  // Registry
	    {
	      WaitForServerReadiness(_NS,"");
	      ActivateRegistry(_argc, _argv);
	      break;
	    }
	  case 3:  // SALOMEDS
	    {
	      WaitForServerReadiness(_NS,"/Kernel/ModulCatalog");
	      ActivateSALOMEDS(_argc, _argv);
	      break;
	    }
	  case 4:  // Session
	    {
	      WaitForServerReadiness(_NS,"/myStudyManager");
	      string containerName = "/Containers/";
	      containerName = containerName + GetHostname();
	      containerName = containerName + "/FactoryServer";
	      WaitForServerReadiness(_NS,containerName);
	      ActivateSession(_argc, _argv);
	      break;
	    }
	  default:
	    {
	      ASSERT(0);
	      break;
	    }
	  }
      }
}

//=============================================================================
/*! 
 *  
 */
//=============================================================================

void Session_ServerThread::ActivateModuleCatalog(int argc,
						 char ** argv)
{
  try
    {
      INFOS("ModuleCatalog thread started");
      // allocation on heap to allow destruction by POA

      SALOME_ModuleCatalogImpl* Catalogue_i
	= new SALOME_ModuleCatalogImpl(argc, argv);

      // Tell the POA that the objects are ready to accept requests.

      _root_poa->activate_object (Catalogue_i);

      CORBA::Object_ptr myCata = Catalogue_i->_this();
      _NS->Register(myCata ,"/Kernel/ModulCatalog");
    }
  catch(CORBA::SystemException&)
    {
      INFOS( "Caught CORBA::SystemException." );
    }
  catch(CORBA::Exception&)
    {
      INFOS( "Caught CORBA::Exception." );
    }
  catch(omniORB::fatalException& fe)
    {
      INFOS( "Caught omniORB::fatalException:" );
      INFOS( "  file: " << fe.file() );
      INFOS( "  line: " << fe.line() );
      INFOS( "  mesg: " << fe.errmsg() );
    }
  catch(...) 
    {
      INFOS( "Caught unknown exception." );
    }
}

//=============================================================================
/*! 
 *  
 */
//=============================================================================

void Session_ServerThread::ActivateSALOMEDS(int argc,
					    char ** argv)
{
  try
    {
      INFOS("SALOMEDS thread started");
      // We allocate the objects on the heap.  Since these are reference
      // counted objects, they will be deleted by the POA when they are no
      // longer needed.    

      SALOMEDS_StudyManager_i * myStudyManager_i
	= new  SALOMEDS_StudyManager_i(_orb);
      
      // Activate the objects.  This tells the POA that the objects are
      // ready to accept requests.

      PortableServer::ObjectId_var myStudyManager_iid
	= _root_poa->activate_object(myStudyManager_i);
      myStudyManager_i->register_name("/myStudyManager");
    }
  catch(CORBA::SystemException&)
    {
      INFOS( "Caught CORBA::SystemException." );
    }
  catch(CORBA::Exception&)
    {
      INFOS( "Caught CORBA::Exception." );
    }
  catch(omniORB::fatalException& fe)
    {
      INFOS( "Caught omniORB::fatalException:" );
      INFOS( "  file: " << fe.file() );
      INFOS( "  line: " << fe.line() );
      INFOS( "  mesg: " << fe.errmsg() );
    }
  catch(...) 
    {
      INFOS( "Caught unknown exception." );
    }
}

//=============================================================================
/*! 
 *  
 */
//=============================================================================

void Session_ServerThread::ActivateRegistry(int argc,
					    char ** argv)
{
  INFOS("Registry thread started");
  SCRUTE(argc); 
  if( argc<3 )
    {
      INFOS("you must provide the Salome session name when you call SALOME_Registry_Server");
      throw CommException("you must provide the Salome session name when you call SALOME_Registry_Server");
    }
  const char *ptrSessionName=0;

  int k=0 ;
  for ( k=1 ; k<argc ; k++ )
    {
      if( strcmp(argv[k],"--salome_session")==0 )
	{
	  ptrSessionName=argv[k+1];
	  break;
	}
    }
  ASSERT(ptrSessionName) ;
  ASSERT(strlen( ptrSessionName )>0);
  const char *registryName = "Registry";
  Registry::Components_var varComponents;
  try
    {
      RegistryService *ptrRegistry = SINGLETON_<RegistryService>::Instance();
      ptrRegistry->SessionName( ptrSessionName );
      varComponents = ptrRegistry->_this();
      // The RegistryService must not already exist.
	    
      try
	{
	  CORBA::Object_var pipo = _NS->Resolve( registryName );
	  if (CORBA::is_nil(pipo) )  throw ServiceUnreachable();
	  INFOS("RegistryService servant already existing" );
	  ASSERT(0);
	}
      catch( const ServiceUnreachable &ex )
	{
	}
      catch( const CORBA::Exception &exx )
	{
	}
      string absoluteName = string("/") + registryName;
      _NS->Register( varComponents , absoluteName.c_str() );
      MESSAGE("On attend les requetes des clients");
    }
  catch( const SALOME_Exception &ex )
    {
      INFOS( "Communication Error : " << ex.what() );
      ASSERT(0);
    }
}

//=============================================================================
/*! 
 *  
 */
//=============================================================================

void Session_ServerThread::ActivateContainer(int argc,
					     char ** argv)
{
  try
    {
      INFOS("Container thread started");

      // get or create the child POA

      PortableServer::POA_var factory_poa;
      try
	{
	  factory_poa = _root_poa->find_POA("factory_poa",0);
	  // 0 = no activation (already done if exists)
	}
      catch (PortableServer::POA::AdapterNonExistent&)
	{
	  INFOS("factory_poa does not exists, create...");
	  // define policy objects     
	  PortableServer::ImplicitActivationPolicy_var implicitActivation =
	    _root_poa->create_implicit_activation_policy(
				PortableServer::NO_IMPLICIT_ACTIVATION);
	  // default = NO_IMPLICIT_ACTIVATION
	  PortableServer::ThreadPolicy_var threadPolicy =
	    _root_poa->create_thread_policy(PortableServer::ORB_CTRL_MODEL);
	  // default = ORB_CTRL_MODEL, other choice SINGLE_THREAD_MODEL
      
	  // create policy list
	  CORBA::PolicyList policyList;
	  policyList.length(2);
	  policyList[0] = PortableServer::ImplicitActivationPolicy::
	    _duplicate(implicitActivation);
	  policyList[1] = PortableServer::ThreadPolicy::
	    _duplicate(threadPolicy);
      
	  PortableServer::POAManager_var nil_mgr
	    = PortableServer::POAManager::_nil();
	  factory_poa = _root_poa->create_POA("factory_poa",
					      nil_mgr,
					      policyList);
	  //with nil_mgr instead of pman,
	  //a new POA manager is created with the new POA
      
	  // destroy policy objects
	  implicitActivation->destroy();
	  threadPolicy->destroy();

	  // obtain the factory poa manager
	  PortableServer::POAManager_var pmanfac = factory_poa->the_POAManager();
	  pmanfac->activate();
	  MESSAGE("pmanfac->activate()");
	}
      
      char *containerName = "";
      if (argc >1) 
	{
	  containerName = argv[1];
	}
      
      Engines_Container_i * myContainer 
	= new Engines_Container_i(_orb, factory_poa, containerName , argc , argv );
    }
  catch(CORBA::SystemException&)
    {
      INFOS("Caught CORBA::SystemException.");
    }
  catch(PortableServer::POA::WrongPolicy&)
    {
      INFOS("Caught CORBA::WrongPolicyException.");
    }
  catch(PortableServer::POA::ServantAlreadyActive&)
    {
      INFOS("Caught CORBA::ServantAlreadyActiveException");
    }
  catch(CORBA::Exception&)
    {
      INFOS("Caught CORBA::Exception.");
    }
  catch(...)
    {
      INFOS("Caught unknown exception.");
    }
}

//=============================================================================
/*! 
 *  
 */
//=============================================================================

void Session_ServerThread::ActivateSession(int argc,
					   char ** argv)
{
    try
      {
	INFOS("Session thread started");
	SALOME_Session_i * mySALOME_Session
	  = new SALOME_Session_i(argc, argv, _orb, _root_poa, _GUIMutex) ;
	PortableServer::ObjectId_var mySALOME_Sessionid
	  = _root_poa->activate_object(mySALOME_Session);
	INFOS("poa->activate_object(mySALOME_Session)");
      
	CORBA::Object_var obj = mySALOME_Session->_this();
	CORBA::String_var sior(_orb->object_to_string(obj));
      
	mySALOME_Session->NSregister();
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
}
