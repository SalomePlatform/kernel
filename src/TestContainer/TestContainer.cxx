//  SALOME TestContainer : test of container creation and its life cycle
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
//  File   : TestContainer.cxx
//  Author : Paul RASCLE, EDF - MARC TAJCHMAN, CEA
//  Module : SALOME
//  $Header$

#include "utilities.h"
#include <iostream>
#include <unistd.h>
#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_Component)
#include CORBA_CLIENT_HEADER(SALOME_TestComponent)

#include "SALOME_NamingService.hxx"
#include "OpUtil.hxx"
#include "Utils_ORB_INIT.hxx"
#include "Utils_SINGLETON.hxx"
#include "Utils_SALOME_Exception.hxx"
#include "Utils_CommException.hxx"
using namespace std;

static ostream& operator<<(ostream& os, const CORBA::Exception& e)
{
  CORBA::Any tmp;
  tmp<<= e;
  CORBA::TypeCode_var tc = tmp.type();
  const char *p = tc->name();
  os<<"Test blocking exception was catch of the kind : ";
  if ( *p != '\0' ) {
    os<<p;
  } 
  else  { 
    os << tc->id();
  }
  
  return os;
}


int main (int argc, char * argv[])
{

  try
    {
      // Initializing omniORB
      CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    
      // use IOR to find container
      //if (argc != 2) { return 1; }
      //CORBA::Object_var obj = orb->string_to_object(argv[1]);
      //Engines::Container_var iGenFact = Engines::Container::_narrow(obj);

      // Obtain a reference to the root POA
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
      int TEST_CONTAINER=0;
      const char * Env = getenv("USE_LOGGER"); 
      int EnvL =0;
      if ((Env!=NULL) && (strlen(Env)))
	EnvL=1;
      CosNaming::Name name;
      name.length(1);
      name[0].id=CORBA::string_dup("Logger");    
      PortableServer::POAManager_var manager; 
      for (int i = 1; i<=NumberOfTries; i++)
	{
	  if (i!=1) 
	    a=nanosleep(&ts_req,&ts_rem);
	  try
	    { 
	      obj = orb->resolve_initial_references("RootPOA");
	      if(!CORBA::is_nil(obj))
		poa = PortableServer::POA::_narrow(obj);
	      if(!CORBA::is_nil(poa))
		manager = poa->the_POAManager();
	      if(!CORBA::is_nil(orb)) 
		theObj = orb->resolve_initial_references("NameService");
	      if (!CORBA::is_nil(theObj))
		inc = CosNaming::NamingContext::_narrow(theObj);
	    }
	  catch( CORBA::COMM_FAILURE& )
	    {
	      INFOS( "Test Container: CORBA::COMM_FAILURE: Unable to contact the Naming Service" )
		}
	  if(!CORBA::is_nil(inc))
	    {
	      MESSAGE( "Test Container: Naming Service was found" )
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
			  {
			    INFOS( "Test Container: Logger Server wasn't found" );
			  }
			catch(...)
			  {
			    INFOS( "Test Container: Unknown exception" );
			  }
			if (!CORBA::is_nil(object))
			  {
			    MESSAGE( "Test Container: Loger Server was found" );
			    TEST_CONTAINER=1;
			    break;
			  }
		      }
		  }
	    }
	  if ((TEST_CONTAINER==1)||((EnvL==0)&&(!CORBA::is_nil(inc))))
            break;
	}

      // Use Name Service to find container
      SALOME_NamingService _NS(orb) ;
      string containerName = "/Containers/" ;
      string hostName = GetHostname();
      containerName += hostName + "/FactoryServer";

      obj = _NS.Resolve(containerName.c_str()) ;
      Engines::Container_var iGenFact = Engines::Container::_narrow(obj);

      Engines::TestComponent_var m1;
    
      for (int iter = 0; iter < 3 ; iter++)
	{
	  MESSAGE("----------------------------------------------------" << iter);   
          string dirn = getenv("KERNEL_ROOT_DIR");
          dirn += "/lib/salome/libSalomeTestComponentEngine.so";
          obj = iGenFact->load_impl("SalomeTestComponent",dirn.c_str());
	  m1 = Engines::TestComponent::_narrow(obj);
	  MESSAGE("recup m1");
	  SCRUTE(m1->instanceName());

	  Engines::FieldsDict_var dico = new Engines::FieldsDict;
	  dico->length(3);
	  dico[0].key=CORBA::string_dup("key_0");
	  dico[0].value <<="value_0";
	  dico[1].key=CORBA::string_dup("key_1");
	  dico[1].value <<=(CORBA::UShort)72;
	  dico[2].key=CORBA::string_dup("key_2");
	  dico[2].value <<="value_2";
	  m1->setProperties(dico);

	  MESSAGE("Coucou " << m1->Coucou(1L));

	  m1->Setenv();

	  Engines::FieldsDict_var dico2 =  m1->getProperties();
	  for (CORBA::ULong i=0; i<dico2->length(); i++)
	    {
	      MESSAGE("dico2["<<i<<"].key="<<dico2[i].key);
	      MESSAGE("dico2["<<i<<"].value type ="<<dico2[i].value.type()->kind());
	      if (dico2[i].value.type()->kind() == CORBA::tk_string)
		{
		  const char* value;
		  dico2[i].value >>= value;
		  MESSAGE("dico2["<<i<<"].value="<<value);
		}
	    }

	  iGenFact->remove_impl(m1) ;
	  //iGenFact->finalize_removal() ; // unpredictable results ...
          sleep(5);
	}    
      // Clean-up.
      iGenFact->finalize_removal() ;
      orb->destroy();
    }
  catch(CORBA::COMM_FAILURE& ex) {
    INFOS("Caught system exception COMM_FAILURE -- unable to contact the object.")
      }
  catch(CORBA::SystemException& e) {
    INFOS("Caught a CORBA::SystemException."<<e)
      }
  catch(CORBA::Exception& e) {
    INFOS("Caught CORBA::Exception."<<e)
      }
  catch(...) {
    INFOS("Caught unknown exception.")
      }

  return 0;
}

