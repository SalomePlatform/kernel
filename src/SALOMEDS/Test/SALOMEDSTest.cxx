// Copyright (C) 2006  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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

#include "SALOMEDSTest.hxx"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include "utilities.h"
#include "Utils_SALOME_Exception.hxx"
#include "Utils_ORB_INIT.hxx"
#include "Utils_SINGLETON.hxx"
#include "OpUtil.hxx"

#include "SALOMEDSClient.hxx"
#include "SALOMEDSClient_ClientFactory.hxx"

#include <TCollection_AsciiString.hxx>

using namespace std;

#define PT_INTEGER 0
#define PT_REAL    1
#define PT_BOOLEAN 2
#define PT_STRING  3
#define PT_REALARRAY 4
#define PT_INTARRAY  5
#define PT_STRARRAY  6


#define TRACEFILE "/tmp/traceUnitTest.log"

// ============================================================================
/*!
 * Set up the environment
 */
// ============================================================================

void SALOMEDSTest::setUp()
{
  TCollection_AsciiString kernel(getenv("KERNEL_ROOT_DIR"));
  TCollection_AsciiString subPath("/share/salome/resources/kernel");
  TCollection_AsciiString csf_var = (kernel+subPath);
  setenv("CSF_PluginDefaults", csf_var.ToCString(), 0);
  setenv("CSF_SALOMEDS_ResourcesDefaults", csf_var.ToCString(), 0);

  // --- trace on file
  char *theFileName = TRACEFILE;

  string s = "file:";
  s += theFileName;
  CPPUNIT_ASSERT(! setenv("SALOME_trace",s.c_str(),1)); // 1: overwrite

  ofstream traceFile;
  traceFile.open(theFileName, ios::out | ios::app);
  CPPUNIT_ASSERT(traceFile); // file created empty, then closed
  traceFile.close();

  LocalTraceBufferPool* bp1 = LocalTraceBufferPool::instance();
  CPPUNIT_ASSERT(bp1);

  // --- Get or initialize the orb

  int _argc = 1;
  char* _argv[] = {""};
  ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance() ;
  ASSERT(SINGLETON_<ORB_INIT>::IsAlreadyExisting());
  _orb = init(_argc , _argv ) ;

  sleep(2);

  // --- Create a SALOME_NamingService instance

  _NS = new SALOME_NamingService;

  sleep(2);

  _NS->init_orb(_orb) ;
}

// ============================================================================
/*!
 *  - delete trace classes
 */
// ============================================================================

void 
SALOMEDSTest::tearDown()
{
  LocalTraceBufferPool* bp1 = LocalTraceBufferPool::instance();
  CPPUNIT_ASSERT(bp1);
  bp1->deleteInstance(bp1);

  delete _NS;
}

// ============================================================================
/*!
 * Check AttributeParameter
 */
// ============================================================================
void SALOMEDSTest::testAttributeParameter()
{
  CORBA::Object_var obj = _orb->resolve_initial_references( "RootPOA" );
  PortableServer::POA_var poa = PortableServer::POA::_narrow( obj );

  PortableServer::POAManager_var pman = poa->the_POAManager();
  pman->activate() ;

  _PTR(StudyManager) sm = ClientFactory::createStudyManager(_orb, poa);

  CPPUNIT_ASSERT(sm);

  _PTR(Study) study = sm->NewStudy("Test");

  CPPUNIT_ASSERT(study);

  _PTR(AttributeParameter) _ap = study->GetCommonParameters("TestComp", 0);

  CPPUNIT_ASSERT(_ap);

  _ap->SetInt("IntValue", 1);
  CPPUNIT_ASSERT(_ap->IsSet("IntValue", PT_INTEGER));
  CPPUNIT_ASSERT(_ap->GetInt("IntValue") == 1);

  _ap->SetReal("RealValue", 1.2);
  CPPUNIT_ASSERT(_ap->IsSet("RealValue", PT_REAL));
  CPPUNIT_ASSERT(_ap->GetReal("RealValue") == 1.2);

  _ap->SetString("StringValue", "hello");
  CPPUNIT_ASSERT(_ap->IsSet("StringValue", PT_STRING));
  CPPUNIT_ASSERT(_ap->GetString("StringValue") == "hello");

  _ap->SetBool("BoolValue", 0);
  CPPUNIT_ASSERT(_ap->IsSet("BoolValue", PT_BOOLEAN));
  CPPUNIT_ASSERT(!_ap->GetBool("BoolValue"));

  _ap->SetBool("BoolValue", 0);
  CPPUNIT_ASSERT(_ap->IsSet("BoolValue", PT_BOOLEAN));
  CPPUNIT_ASSERT(!_ap->GetBool("BoolValue"));

  vector<int> intArray;
  intArray.push_back(0);
  intArray.push_back(1);

  _ap->SetIntArray("IntArray", intArray);
  CPPUNIT_ASSERT(_ap->IsSet("IntArray", PT_INTARRAY));
  CPPUNIT_ASSERT(_ap->GetIntArray("IntArray")[0] == 0);
  CPPUNIT_ASSERT(_ap->GetIntArray("IntArray")[1] == 1); 

  vector<double> realArray;
  realArray.push_back(0.0);
  realArray.push_back(1.1);
  
  _ap->SetRealArray("RealArray", realArray);
  CPPUNIT_ASSERT(_ap->IsSet("RealArray", PT_REALARRAY));
  CPPUNIT_ASSERT(_ap->GetRealArray("RealArray")[0] == 0.0);
  CPPUNIT_ASSERT(_ap->GetRealArray("RealArray")[1] == 1.1); 

  vector<string> strArray;
  strArray.push_back("hello");
  strArray.push_back("world");
  
  _ap->SetStrArray("StrArray", strArray);
  CPPUNIT_ASSERT(_ap->IsSet("StrArray", PT_STRARRAY));
  CPPUNIT_ASSERT(_ap->GetStrArray("StrArray")[0] == "hello");
  CPPUNIT_ASSERT(_ap->GetStrArray("StrArray")[1] == "world"); 
}



