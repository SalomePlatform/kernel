// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//  File   : SALOMEDS_Server.cxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$
//
#include "utilities.h"
#include "Utils_SINGLETON.hxx"

#include "SALOME_NamingService.hxx"
#include "SALOMEDS_StudyManager_i.hxx"

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

#ifdef CHECKTIME
#include <Utils_Timer.hxx>
#endif

// extern "C"
// { // for ccmalloc memory debug
// void ccmalloc_static_initialization(void);
// }

///////////////////////////// MAIN ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
  //   ccmalloc_static_initialization();

  MESSAGE("SALOMEDS_Server - main");
  try 
    {
      // Initialise the ORB.
#if OMNIORB_VERSION >= 4
      CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "omniORB4" ) ;
#else
      CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "omniORB3" );
#endif      
      // Obtain a reference to the root POA.
      long TIMESleep = 500000000;
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
      int SALOMEDS=0;
      const char * Env = getenv("USE_LOGGER");
      int EnvL =0;
      if ((Env!=NULL) && (strlen(Env)))
        EnvL=1;
      CosNaming::Name name;
      name.length(1);
      name[0].id=CORBA::string_dup("Logger");    
      PortableServer::POAManager_var pman; 
      for (int i = 1; i<=NumberOfTries; i++)
        {
          if (i!=1) 
#ifndef WIN32
            a=nanosleep(&ts_req,&ts_rem);
#else
                Sleep(TIMESleep/1000000);
#endif
          try
            { 
              obj = orb->resolve_initial_references("RootPOA");
              if(!CORBA::is_nil(obj))
                poa = PortableServer::POA::_narrow(obj);
              if(!CORBA::is_nil(poa))
                pman = poa->the_POAManager();
              if(!CORBA::is_nil(orb)) 
                theObj = orb->resolve_initial_references("NameService"); 
              if (!CORBA::is_nil(theObj)){
                inc = CosNaming::NamingContext::_narrow(theObj);
                if(!CORBA::is_nil(inc))
                  {
                    MESSAGE( "SalomeDS Server: Naming Service was found" );
                    if(EnvL==1)
                      {
                        CORBA::ORB_var orb1 = CORBA::ORB_init(argc,argv) ;
                        SALOME_NamingService &NS = *SINGLETON_<SALOME_NamingService>::Instance() ;
                        NS.init_orb( orb1 ) ;
                        for(int j=1; j<=NumberOfTries; j++)
                          {
                            if (j!=1) 
#ifndef WIN32
                              a=nanosleep(&ts_req, &ts_rem);
#else
                              Sleep(TIMESleep/1000000);
#endif
                            try
                              {
                                object = inc->resolve(name);
                              }
                            catch(CosNaming::NamingContext::NotFound)
                              { 
                                MESSAGE( "SalomeDS Server: Logger Server wasn't found" ); }

                            catch(...)
                              {
                                MESSAGE( "SalomeDS Server: Unknown exception" );
                              }
                            if (!CORBA::is_nil(object))
                              {
                                MESSAGE( "SalomeDS Server: Logger Server was found" );
                                SALOMEDS=1;
                                break;
                              }
                          }
                      }
                  }
              }
        
            }
          catch( const SALOME_Exception &ex )
            {
              MESSAGE( "Communication Error : " << ex.what() );
              return EXIT_FAILURE ;
            }
          if ((SALOMEDS==1)||((EnvL==0)&&(!CORBA::is_nil(inc))))
            break;
        }
    
      // We allocate the objects on the heap.  Since these are reference
      // counted objects, they will be deleted by the POA when they are no
      // longer needed.    
      SALOMEDS_StudyManager_i * myStudyManager_i = new  SALOMEDS_StudyManager_i(orb,poa);
 
      // Activate the objects.  This tells the POA that the objects are
      // ready to accept requests.
      PortableServer::ObjectId_var myStudyManager_iid = poa->activate_object(myStudyManager_i);
      myStudyManager_i->register_name("/myStudyManager");
      myStudyManager_i->_remove_ref();
       
      // Obtain a POAManager, and tell the POA to start accepting
      // requests on its objects.
      pman = poa->the_POAManager();
      pman->activate();

#ifdef CHECKTIME
      Utils_Timer timer;
      timer.Start();
      timer.Stop();
      MESSAGE("SALOME_Registry_Server.cxx - orb->run()");
      timer.ShowAbsolute();
#endif
      orb->run();
      MESSAGE( "end of SALOME_DS server" );
      orb->destroy();
    }
  catch(CORBA::SystemException&)
    {
      MESSAGE( "Caught CORBA::SystemException." );
    }
  catch(CORBA::Exception&)
    {
      MESSAGE( "Caught CORBA::Exception." );
    }
  catch(omniORB::fatalException& fe)
    {
      MESSAGE( "Caught omniORB::fatalException:" );
      MESSAGE( "  file: " << fe.file() );
      MESSAGE( "  line: " << fe.line() );
      MESSAGE( "  mesg: " << fe.errmsg() );
    }
  catch(...) 
    {
      MESSAGE( "Caught unknown exception." );
    }
  return 0;
}
