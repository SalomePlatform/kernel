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
#include "NamingService_WaitForServerReadiness.hxx"
#include "OpUtil.hxx"
#include "Utils_ORB_INIT.hxx"
#include "Utils_SINGLETON.hxx"
#include "Utils_SALOME_Exception.hxx"
#include "Utils_CommException.hxx"
#include "SALOMETraceCollector.hxx"
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

Engines::TestComponent_ptr create_instance(Engines::Container_ptr iGenFact,
					   string componenttName)
{
  bool isLib =
    iGenFact->load_component_Library(componenttName.c_str());
  //    iGenFact->load_component_Library("SalomeTestComponent");
  ASSERT(isLib);
  CORBA::Object_var obj =
    //    iGenFact->create_component_instance("SalomeTestComponent",
    iGenFact->create_component_instance(componenttName.c_str(),
					0);
  Engines::TestComponent_var anInstance = Engines::TestComponent::_narrow(obj);
  MESSAGE("create anInstance");
  SCRUTE(anInstance->instanceName());
  return anInstance._retn();
}

int main (int argc, char * argv[])
{
  // Initializing omniORB
  ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance() ;
  CORBA::ORB_var &orb = init( argc , argv ) ;
  SALOMETraceCollector *myThreadTrace = SALOMETraceCollector::instance(orb);

  try
    {
      SALOME_NamingService _NS(orb) ;
      string containerName = "/Containers/" ;
      string hostName = GetHostname();
      containerName += hostName + "/FactoryServer";
      NamingService_WaitForServerReadiness(&_NS,containerName);

      CORBA::Object_var obj = _NS.Resolve(containerName.c_str()) ;
      Engines::Container_var iGenFact = Engines::Container::_narrow(obj);

      int nbInstances = 5;

      vector<Engines::TestComponent_var> instances(nbInstances);
    
      MESSAGE("------------------------------- create instances ");
      for (int iter = 0; iter < nbInstances ; iter++)
	{
	  instances[iter] = create_instance(iGenFact,"SalomeTestComponent");
	}

      MESSAGE("------------------------------ set env instances ");
      for (int iter = 0; iter < nbInstances ; iter++)
	{
	  Engines::TestComponent_var anInstance = instances[iter];
	  SCRUTE(anInstance->instanceName());
	  Engines::FieldsDict_var dico = new Engines::FieldsDict;
	  dico->length(3);
	  dico[0].key=CORBA::string_dup("key_0");
	  dico[0].value <<="value_0";
	  dico[1].key=CORBA::string_dup("key_1");
	  dico[1].value <<=(CORBA::UShort)72;
	  dico[2].key=CORBA::string_dup("key_2");
	  dico[2].value <<=(CORBA::ULong)iter;
	  anInstance->setProperties(dico);
	  MESSAGE("Coucou " << anInstance->Coucou(iter));
	  anInstance->Setenv();
	}

      MESSAGE("---------------------------------- get instances ");
      for (int iter = 0; iter < nbInstances ; iter++)
	{
	  Engines::TestComponent_var anInstance = instances[iter];
	  SCRUTE(anInstance->instanceName());
	  Engines::FieldsDict_var dico2 =  anInstance->getProperties();
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
	}

      MESSAGE("------------------------------- remove instances ");
      for (int iter = 0; iter < nbInstances ; iter++)
	{
	  Engines::TestComponent_var anInstance = instances[iter];
	  SCRUTE(anInstance->instanceName());
	  iGenFact->remove_impl(anInstance) ;
	  //iGenFact->finalize_removal() ; // unpredictable results ...
	} 
      MESSAGE("------------------------------- PYTHON ");
      {
// 	bool isLib =
// 	  iGenFact->load_component_Library("SALOME_TestComponentPy");
// 	ASSERT(isLib);
// 	CORBA::Object_var obj =
// 	  iGenFact->create_component_instance("SALOME_TestComponentPy",
// 					      0);
// 	Engines::TestComponent_var anInstance =
// 	  Engines::TestComponent::_narrow(obj);
// 	MESSAGE("create anInstance");
// 	SCRUTE(anInstance->instanceName());
      MESSAGE("------------------------------- create instances ");
      for (int iter = 0; iter < nbInstances ; iter++)
	{
	  instances[iter] = create_instance(iGenFact,"SALOME_TestComponentPy");
	}

      MESSAGE("---------------------------------- get instances ");
      for (int iter = 0; iter < nbInstances ; iter++)
	{
	  Engines::TestComponent_var anInstance = instances[iter];
	  SCRUTE(anInstance->instanceName());
	  MESSAGE("Coucou " << anInstance->Coucou(iter));
	}
      }
   
      // Clean-up.
      iGenFact->finalize_removal() ;
      orb->shutdown(0);
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

  delete myThreadTrace;
  return 0;
}

