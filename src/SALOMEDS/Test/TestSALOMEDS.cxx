// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
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

// --- include all SALOMEDS Test from basics until the present directory

#include "SALOMELocalTraceTest.hxx"
#include "SALOMEDSImplTest.hxx"
#include "SALOMEDSTest.hxx"

#include <TCollection_AsciiString.hxx>

// --- Registers the fixture into the 'registry'

CPPUNIT_TEST_SUITE_REGISTRATION( SALOMEDSTest );
CPPUNIT_TEST_SUITE_REGISTRATION( SALOMEDSTest_Embedded );

#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TextTestProgressListener.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestRunner.h>
#include <stdexcept>

#include <iostream>
#include <fstream>

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include "utilities.h"
#include "Utils_SINGLETON.hxx"
#include "Utils_ORB_INIT.hxx"
#include "OpUtil.hxx"
#include "SALOME_NamingService.hxx"
#include "NamingService_WaitForServerReadiness.hxx"
#include "SALOMEDS_StudyManager_i.hxx"

// ============================================================================
/*!
 *  Main program source for Unit Tests with cppunit package does not depend
 *  on actual tests, so we use the same for all partial unit tests.
 */
// ============================================================================

int main(int argc, char* argv[])
{
  // --- Run Salome without GUI

  setenv("SALOME_trace", "file:./traceUnitTest.log", 1); // 1: overwrite

  system("runSalome -t &");

  // --- Wait till SALOMEDS server is launched
  
  char hostname[511];
  int size;
  gethostname(hostname, size);
  char* chr_port = getenv("SALOMEDS_UNITTESTS_PORT");
  string port;
  if(chr_port) port = chr_port;
  if(port.empty()) port = "2810";
  string cfg_file = string(getenv("HOME"))+"/.omniORB_"+string(hostname)+"_"+port+".cfg";
  setenv("OMNIORB_CONFIG", cfg_file.c_str(), 1);

  ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance() ;
  ASSERT(SINGLETON_<ORB_INIT>::IsAlreadyExisting());
  CORBA::ORB_var orb = init(argc , argv ) ;

  sleep(15);

  string host; // = GetHostname();
  char* wait_Superv = getenv("SALOMEDS_UNITTESTS_WAIT_SUPERVISOR");
  if(wait_Superv) host = GetHostname(); 

  SALOME_NamingService NS(orb);
  if(host.empty())
    NamingService_WaitForServerReadiness(&NS, "/myStudyManager");
  else {
    string serverName = "/Containers/"+host+"/SuperVisionContainer";
    NamingService_WaitForServerReadiness(&NS, serverName);
  }

  CORBA::Object_var obj = NS.Resolve( "/myStudyManager" );
  if(CORBA::is_nil(obj)) {
     system("killSalome.py");
     return 1;
  }

  //Set up the environement for Embedded case
  string kernel_root = getenv("KERNEL_ROOT_DIR");
  CPPUNIT_ASSERT(!kernel_root.empty());
  kernel_root+="/share/salome/resources/kernel";
  setenv("CSF_PluginDefaults", kernel_root.c_str(), 1);
  setenv("CSF_SALOMEDS_ResourcesDefaults", kernel_root.c_str(), 1);

  CORBA::Object_var poaObj = orb->resolve_initial_references("RootPOA");
  if(!CORBA::is_nil(poaObj)) {
		PortableServer::POA_var poa = PortableServer::POA::_narrow(poaObj);

    SALOMEDS_StudyManager_i * aStudyManager_i = new  SALOMEDS_StudyManager_i(orb, poa);
    // Activate the objects.  This tells the POA that the objects are ready to accept requests.
    PortableServer::ObjectId_var aStudyManager_iid =  poa->activate_object(aStudyManager_i);
    aStudyManager_i->register_name("/myStudyManager_embedded");
      
    // Obtain a POAManager, and tell the POA to start accepting
    // requests on its objects.
    PortableServer::POAManager_var pman = poa->the_POAManager();
    pman->activate();
  }
 

  // --- Create the event manager and test controller
  CPPUNIT_NS::TestResult controller;

  // ---  Add a listener that colllects test result
  CPPUNIT_NS::TestResultCollector result;
  controller.addListener( &result );        

  // ---  Add a listener that print dots as test run.
#ifdef WIN32
  CPPUNIT_NS::TextTestProgressListener progress;
#else
  CPPUNIT_NS::BriefTestProgressListener progress;
#endif
  controller.addListener( &progress );      

  // ---  Get the top level suite from the registry

  CPPUNIT_NS::Test *suite =
    CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest();

  // ---  Adds the test to the list of test to run

  CPPUNIT_NS::TestRunner runner;
  runner.addTest( suite );
  runner.run( controller);

  // ---  Print test in a compiler compatible format.

  std::ofstream testFile;
  testFile.open("UnitTestsResult", std::ios::out |  std::ios::trunc);
  //CPPUNIT_NS::CompilerOutputter outputter( &result, std::cerr );
  CPPUNIT_NS::CompilerOutputter outputter( &result, testFile );
  outputter.write(); 

  // ---  Run the tests.

  bool wasSucessful = result.wasSuccessful();
  testFile.close();

  // --- Kill all created Salome process

  system("killSalome.py");

  // ---  Return error code 1 if the one of test failed.

  return wasSucessful ? 0 : 1;
}
