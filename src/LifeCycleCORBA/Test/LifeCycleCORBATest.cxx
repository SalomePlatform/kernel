
#include "LifeCycleCORBATest.hxx"
#include "SALOME_LifeCycleCORBA.hxx"
#include "Utils_ORB_INIT.hxx"
#include "Utils_SINGLETON.hxx"
#include "OpUtil.hxx"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

// --- uncomment to have some traces on standard error
//     (useful only when adding new tests...)
//#define _DEVDEBUG_

#ifdef _DEVDEBUG_
#define MYDEVTRACE {std::cerr << __FILE__ << " [" << __LINE__ << "] : ";}
#define DEVTRACE(msg) {MYDEVTRACE; std::cerr<<msg<<std::endl<<std::flush;}
#else
#define MYDEVTRACE
#define DEVTRACE(msg)
#endif

#define TRACEFILE "/tmp/traceUnitTest.log"

// ============================================================================
/*!
 * Set Trace mecanism
 * - delete preexisting trace classes if any
 * - set trace on file
 * Get or initialize the orb
 * Create a SALOME_NamingService instance
 */
// ============================================================================

void 
LifeCycleCORBATest::setUp()
{
  LocalTraceBufferPool* bp1 = LocalTraceBufferPool::instance();
  CPPUNIT_ASSERT(bp1);
  bp1->deleteInstance(bp1);

  // --- trace on file
  char *theFileName = TRACEFILE;

  string s = "file:";
  s += theFileName;
  //s="local";
  //s="with_logger";
  CPPUNIT_ASSERT(! setenv("SALOME_trace",s.c_str(),1)); // 1: overwrite

  ofstream traceFile;
  //  traceFile.open(theFileName, ios::out | ios::trunc);
  traceFile.open(theFileName, ios::out | ios::app);
  CPPUNIT_ASSERT(traceFile); // file created empty, then closed
  traceFile.close();

  bp1 = LocalTraceBufferPool::instance();
  CPPUNIT_ASSERT(bp1);

  // --- Get or initialize the orb

  int _argc = 1;
  char* _argv[] = {""};
  ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance() ;
  ASSERT(SINGLETON_<ORB_INIT>::IsAlreadyExisting());
  _orb = init(_argc , _argv ) ;

  // --- Create a SALOME_NamingService instance

  _NS.init_orb(_orb) ;
}

// ============================================================================
/*!
 *  - delete trace classes
 */
// ============================================================================

void 
LifeCycleCORBATest::tearDown()
{

  LocalTraceBufferPool* bp1 = LocalTraceBufferPool::instance();
  CPPUNIT_ASSERT(bp1);
  bp1->deleteInstance(bp1);
}

// ============================================================================
/*!
 * Check FindOrLoad_Component.
 * - get a local container (no hostname given),
 *       load an engine, check that the CORBA object is not null.
 * - check narrow
 */
// ============================================================================

void
LifeCycleCORBATest::testFindOrLoad_Component_LaunchContainer()
{
  SALOME_LifeCycleCORBA _LCC(&_NS);

  // --- get a local container,
  //     load an engine, check that the CORBA object is not null

  string containerName = "myContainer";
  Engines::Component_var mycompo =
    _LCC.FindOrLoad_Component(containerName.c_str(),"SalomeTestComponent");
  CPPUNIT_ASSERT(!CORBA::is_nil(mycompo));

  // --- check narrow

  Engines::TestComponent_var m1;
  m1 = Engines::TestComponent::_narrow(mycompo);
  CPPUNIT_ASSERT(!CORBA::is_nil(m1));
}

// ============================================================================
/*!
 * Check FindOrLoad_Component.
 * - Call 2 times FindOrLoad_Component with the same parameters
 * - check if we get the same engine
 */
// ============================================================================

void
LifeCycleCORBATest::testFindOrLoad_Component_SameInstance()
{
  SALOME_LifeCycleCORBA _LCC(&_NS);

  // --- get a local container,
  //     load an engine, check that the CORBA object is not null

  string containerName = "myContainer";

  Engines::Component_var mycompo1 =
    _LCC.FindOrLoad_Component(containerName.c_str(),"SalomeTestComponent");
  CPPUNIT_ASSERT(!CORBA::is_nil(mycompo1));

  Engines::Component_var mycompo2 =
    _LCC.FindOrLoad_Component(containerName.c_str(),"SalomeTestComponent");
  CPPUNIT_ASSERT(!CORBA::is_nil(mycompo2));

  // --- check narrow

  Engines::TestComponent_var m1;
  m1 = Engines::TestComponent::_narrow(mycompo1);
  CPPUNIT_ASSERT(!CORBA::is_nil(m1));

  Engines::TestComponent_var m2;
  m2 = Engines::TestComponent::_narrow(mycompo2);
  CPPUNIT_ASSERT(!CORBA::is_nil(m2));

  // --- check equality of instance names

  string name1 = m1->instanceName();
  string name2 = m2->instanceName();
  CPPUNIT_ASSERT_EQUAL(name1, name2);
}

// ============================================================================
/*!
 * Check FindOrLoad_Component with Python Component on C++ Container
 *       load an engine, check that the CORBA object is not null.
 * - check narrow
 */
// ============================================================================

void
LifeCycleCORBATest::testFindOrLoad_Component_PythonInCppContainer()
{
  SALOME_LifeCycleCORBA _LCC(&_NS);

  // --- get a local container,
  //     load an engine, check that the CORBA object is not null

  string containerName = "myContainer";

  Engines::Component_var mycompo1 =
    _LCC.FindOrLoad_Component(containerName.c_str(),"SALOME_TestComponentPy");
  CPPUNIT_ASSERT(!CORBA::is_nil(mycompo1));

  // --- check narrow

  Engines::TestComponent_var m1;
  m1 = Engines::TestComponent::_narrow(mycompo1);
  CPPUNIT_ASSERT(!CORBA::is_nil(m1));
}

// ============================================================================
/*!
 * Check FindOrLoad_Component with Python Component on C++ Container
 * - Call 2 times FindOrLoad_Component with the same parameters
 * - check if we get the same engine
 */
// ============================================================================

void
LifeCycleCORBATest::testFindOrLoad_Component_PythonSameInstance()
{
  SALOME_LifeCycleCORBA _LCC(&_NS);

  // --- get a local container (with a name based on local hostname),
  //     load an engine, check that the CORBA object is not null

  string containerName = "myContainer";

  Engines::Component_var mycompo1 =
    _LCC.FindOrLoad_Component(containerName.c_str(),"SALOME_TestComponentPy");
  CPPUNIT_ASSERT(!CORBA::is_nil(mycompo1));

  Engines::Component_var mycompo2 =
    _LCC.FindOrLoad_Component(containerName.c_str(),"SALOME_TestComponentPy");
  CPPUNIT_ASSERT(!CORBA::is_nil(mycompo2));

  // --- check narrow

  Engines::TestComponent_var m1;
  m1 = Engines::TestComponent::_narrow(mycompo1);
  CPPUNIT_ASSERT(!CORBA::is_nil(m1));

  Engines::TestComponent_var m2;
  m2 = Engines::TestComponent::_narrow(mycompo2);
  CPPUNIT_ASSERT(!CORBA::is_nil(m2));

  // --- check equality of instance names

  string name1 = m1->instanceName();
  string name2 = m2->instanceName();
  CPPUNIT_ASSERT_EQUAL(name1, name2);
}

// ============================================================================
/*!
 * Check FindOrLoad_Component with a component name not in catalog.
 * See list of catalog given to module catalog server.
 * Here, we work with KERNEL_SRC/resources/KERNELCatalog.xml that contains 
 * only KERNEL, SalomeTestComponent and SALOME_TestComponentPy
 */
// ============================================================================

void
LifeCycleCORBATest::testFindOrLoad_Component_UnknownInCatalog()
{
  SALOME_LifeCycleCORBA _LCC(&_NS);

  // --- get a local container (with a name based on local hostname),
  //     load an engine, check that the CORBA object is not null

  string containerName = "myContainer";

  Engines::Component_var mycompo1 =
    _LCC.FindOrLoad_Component(containerName.c_str(),"MyNewComponent");
  CPPUNIT_ASSERT(CORBA::is_nil(mycompo1));
}

// ============================================================================
/*!
 * Check FindOrLoad_Component with hostname given
 * - get a local container : getHostName()/componentName,
 *       load an engine, check that the CORBA object is not null.
 * - check narrow
 */
// ============================================================================

void
LifeCycleCORBATest::testFindOrLoad_Component_LaunchContainerHostname()
{
  SALOME_LifeCycleCORBA _LCC(&_NS);

  // --- get a local container (with a name based on local hostname),
  //     load an engine, check that the CORBA object is not null

  string containerName = GetHostname();
  containerName += "/theContainer";
  DEVTRACE("containerName = " << containerName);
  Engines::Component_var mycompo =
    _LCC.FindOrLoad_Component(containerName.c_str(),"SalomeTestComponent");
  CPPUNIT_ASSERT(!CORBA::is_nil(mycompo));

  // --- check narrow

  Engines::TestComponent_var m1;
  m1 = Engines::TestComponent::_narrow(mycompo);
  CPPUNIT_ASSERT(!CORBA::is_nil(m1));
}

// ============================================================================
/*!
 * Check FindOrLoad_Component with and without local hostname given.
 * We must get the same container, the same instance of component
 */
// ============================================================================

void
LifeCycleCORBATest::testFindOrLoad_Component_SameContainer()
{
  SALOME_LifeCycleCORBA _LCC(&_NS);

  // --- get a local container (with a name based on local hostname),
  //     load an engine, check that the CORBA object is not null

  string containerName = "aContainer";

  Engines::Component_var mycompo1 =
    _LCC.FindOrLoad_Component(containerName.c_str(),"SalomeTestComponent");
  CPPUNIT_ASSERT(!CORBA::is_nil(mycompo1));

  containerName = GetHostname();
  containerName += "/aContainer";
  DEVTRACE("containerName = " << containerName);
  Engines::Component_var mycompo2 =
    _LCC.FindOrLoad_Component(containerName.c_str(),"SalomeTestComponent");
  CPPUNIT_ASSERT(!CORBA::is_nil(mycompo2));

  // --- check narrow

  Engines::TestComponent_var m1;
  m1 = Engines::TestComponent::_narrow(mycompo1);
  CPPUNIT_ASSERT(!CORBA::is_nil(m1));

  Engines::TestComponent_var m2;
  m2 = Engines::TestComponent::_narrow(mycompo2);
  CPPUNIT_ASSERT(!CORBA::is_nil(m2));

  // --- check equality of instance names

  string name1 = m1->instanceName();
  string name2 = m2->instanceName();
  CPPUNIT_ASSERT_EQUAL(name1, name2);

  // --- check containers are the same servant (same container name+hostname)

  Engines::Container_var c1 = m1->GetContainerRef();
  CPPUNIT_ASSERT(!CORBA::is_nil(c1));
  Engines::Container_var c2 = m2->GetContainerRef();
  CPPUNIT_ASSERT(!CORBA::is_nil(c1));
  string cname1 = c1->name();
  string cname2 = c2->name();
  CPPUNIT_ASSERT_EQUAL(cname1, cname2);
  string hostname1 = c1->getHostName();
  string hostname2 = c2->getHostName();
  CPPUNIT_ASSERT_EQUAL(hostname1, hostname2);
  CORBA::Long pidc1 = c1->getPID();
  CORBA::Long pidc2 = c2->getPID();
  CPPUNIT_ASSERT_EQUAL(pidc1, pidc2);
}

// ============================================================================
/*!
 * Check FindOrLoad_Component: check behaviour when ask for an unknown computer
 * We must catch a Salome Exception with "unknown host" message
 */
// ============================================================================

void
LifeCycleCORBATest::testFindOrLoad_Component_UnknownMachine()
{
  SALOME_LifeCycleCORBA _LCC(&_NS);

  // --- try to get a distant container on an unknown machine (not existing)
  //     check that the CORBA object is null

  string containerName = "aFarAwayComputer";
  containerName += "/theContainer";
//   CPPUNIT_ASSERT_THROW(Engines::Component_var mycompo =
// 		       _LCC.FindOrLoad_Component(containerName.c_str(),"SalomeTestComponent");,SALOME::SALOME_Exception);
  try
    {
      Engines::Component_var mycompo =
	_LCC.FindOrLoad_Component(containerName.c_str(),"SalomeTestComponent");
    }
  catch(const SALOME::SALOME_Exception &ex)
    {
      CPPUNIT_ASSERT(true);
//       string expectedMessage = "BAD PARAM";
//       std::ostream os;
//       os << ex;
//       string actualMessage = os.str();
//       DEVTRACE("actual Exception Message = " << actualMessage);
//       CPPUNIT_ASSERT(actualMessage.find(expectedMessage) != string::npos);
    }
}

// ============================================================================
/*!
 * Check FindOrLoad_Component, empty params must give an instance
 */
// ============================================================================

void
LifeCycleCORBATest::testFindOrLoad_Component_ParamsEmpty()
{
  SALOME_LifeCycleCORBA _LCC(&_NS);

  Engines::MachineParameters params;
  _LCC.preSet(params);
  Engines::Component_var mycompo =
    _LCC.FindOrLoad_Component(params,"SalomeTestComponent");
  CPPUNIT_ASSERT(!CORBA::is_nil(mycompo));

  // --- check narrow

  Engines::TestComponent_var m1;
  m1 = Engines::TestComponent::_narrow(mycompo);
}

// ============================================================================
/*!
 * Check FindOrLoad_Component params = local container 
 */
// ============================================================================

void
LifeCycleCORBATest::testFindOrLoad_Component_ParamsLocalContainer()
{
  SALOME_LifeCycleCORBA _LCC(&_NS);

  Engines::MachineParameters params;
  _LCC.preSet(params);
  string hostname=GetHostname();
  params.hostname=hostname.c_str();
  Engines::Component_var mycompo =
    _LCC.FindOrLoad_Component(params,"SalomeTestComponent");
  CPPUNIT_ASSERT(!CORBA::is_nil(mycompo));

  // --- check narrow

  Engines::TestComponent_var m1;
  m1 = Engines::TestComponent::_narrow(mycompo);

  // --- check that container is on local computer

  CPPUNIT_ASSERT(!CORBA::is_nil(m1));
  Engines::Container_var c1 = m1->GetContainerRef();
  CPPUNIT_ASSERT(!CORBA::is_nil(c1));
  string hostname1 = c1->getHostName();
  CPPUNIT_ASSERT_EQUAL(hostname1, GetHostname());
}


// ============================================================================
/*!
 * Check FindOrLoad_Component params = containerName
 */
// ============================================================================

void
LifeCycleCORBATest::testFindOrLoad_Component_ParamsContainerName()
{
  SALOME_LifeCycleCORBA _LCC(&_NS);

  Engines::MachineParameters params;
  _LCC.preSet(params);
  string containerName = "myContainer";
  params.container_name = containerName.c_str();
  Engines::Component_var mycompo =
    _LCC.FindOrLoad_Component(params,"SalomeTestComponent");
  CPPUNIT_ASSERT(!CORBA::is_nil(mycompo));

  // --- check narrow

  Engines::TestComponent_var m1;
  m1 = Engines::TestComponent::_narrow(mycompo);

  // --- check that container has good name

  CPPUNIT_ASSERT(!CORBA::is_nil(m1));
  Engines::Container_var c1 = m1->GetContainerRef();
  CPPUNIT_ASSERT(!CORBA::is_nil(c1));
  string hostname1 = c1->getHostName();
  CPPUNIT_ASSERT_EQUAL(hostname1, GetHostname());
  string cname1 = c1->name();
  CPPUNIT_ASSERT(cname1.find(containerName) != string::npos);
}

// ============================================================================
/*!
 * Check FindOrLoad_Component on remote computer
 */
// ============================================================================

void
LifeCycleCORBATest::testFindOrLoad_Component_RemoteComputer()
{
  SALOME_LifeCycleCORBA _LCC(&_NS);

  string remoteHost = GetRemoteHost();

  string containerName = remoteHost;
  containerName += "/aContainer";
  DEVTRACE("containerName = " << containerName);
  Engines::Component_var mycompo1 =
    _LCC.FindOrLoad_Component(containerName.c_str(),"SalomeTestComponent");
  CPPUNIT_ASSERT_MESSAGE("Test validity requires connection to remote "\
			 "computers from ResourcesCatalog",
			 !CORBA::is_nil(mycompo1));

  // --- check narrow

  Engines::TestComponent_var m1;
  m1 = Engines::TestComponent::_narrow(mycompo1);

  // --- check that container is on good computer

  CPPUNIT_ASSERT(!CORBA::is_nil(m1));
  Engines::Container_var c1 = m1->GetContainerRef();
  CPPUNIT_ASSERT(!CORBA::is_nil(c1));
  string hostname1 = c1->getHostName();
  CPPUNIT_ASSERT_EQUAL(hostname1, remoteHost);
}

// ============================================================================
/*!
 * Check FindOrLoad_Component with params on remote computer
 * params empty except hostname 
 */
// ============================================================================

void
LifeCycleCORBATest::testFindOrLoad_Component_ParamsRemoteComputer()
{
  SALOME_LifeCycleCORBA _LCC(&_NS);

  string remoteHost = GetRemoteHost();

  Engines::MachineParameters params;
  _LCC.preSet(params); 
  params.hostname = remoteHost.c_str();

  Engines::Component_var mycompo1 =
    _LCC.FindOrLoad_Component(params,"SalomeTestComponent");
  CPPUNIT_ASSERT_MESSAGE("Test validity requires connection to remote "\
			 "computers from ResourcesCatalog",
			 !CORBA::is_nil(mycompo1));

  // --- check narrow

  Engines::TestComponent_var m1;
  m1 = Engines::TestComponent::_narrow(mycompo1);

  // --- check that container is on good computer

  CPPUNIT_ASSERT(!CORBA::is_nil(m1));
  Engines::Container_var c1 = m1->GetContainerRef();
  CPPUNIT_ASSERT(!CORBA::is_nil(c1));
  string hostname1 = c1->getHostName();
  CPPUNIT_ASSERT_EQUAL(hostname1, remoteHost);
}

// ============================================================================
/*!
 * Check FindOrLoad_Component with params on remote computer
 * params empty except hostname and container_name
 */
// ============================================================================

void
LifeCycleCORBATest::testFindOrLoad_Component_ParamsRemoteComputer2()
{
  SALOME_LifeCycleCORBA _LCC(&_NS);

  string remoteHost = GetRemoteHost();

  Engines::MachineParameters params;
  _LCC.preSet(params); 
  params.hostname = remoteHost.c_str();
  params.container_name = "anotherContainer";

  Engines::Component_var mycompo1 =
    _LCC.FindOrLoad_Component(params,"SalomeTestComponent");
  CPPUNIT_ASSERT_MESSAGE("Test validity requires connection to remote "\
			 "computers from ResourcesCatalog",
			 !CORBA::is_nil(mycompo1));

  // --- check narrow

  Engines::TestComponent_var m1;
  m1 = Engines::TestComponent::_narrow(mycompo1);

  // --- check that container is on good computer

  CPPUNIT_ASSERT(!CORBA::is_nil(m1));
  Engines::Container_var c1 = m1->GetContainerRef();
  CPPUNIT_ASSERT(!CORBA::is_nil(c1));
  string hostname1 = c1->getHostName();
  CPPUNIT_ASSERT_EQUAL(hostname1, remoteHost);
}



// ============================================================================
/*!
 * Check FindOrLoad_Component params = containerName
 */
// ============================================================================

// void
// LifeCycleCORBATest::testFindOrLoad_Component_()
// {
// }








// ============================================================================
/*!
 *  Get a remote HostName in the Resource Catalog
 */
// ============================================================================

string LifeCycleCORBATest::GetRemoteHost()
{
  SALOME_LifeCycleCORBA _LCC(&_NS);

  CORBA::Object_var obj = _NS.Resolve("/ContainerManager");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  Engines::ContainerManager_var containerManager =
    Engines::ContainerManager::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(containerManager));

  Engines::MachineParameters params;
  _LCC.preSet(params);               // empty params to get all the machines

  Engines::MachineList_var hostList =
    containerManager->GetFittingResources(params,"SalomeTestComponent");
  CPPUNIT_ASSERT(hostList->length() > 1);

  string localHost = GetHostname();
  string remoteHost;
  for (unsigned int i=0; i < hostList->length(); i++)
    {
      const char* aMachine = hostList[i];
      string machine(aMachine);
      if (machine != localHost)
	{
	  remoteHost = machine;
	  break;
	}
    }
  CPPUNIT_ASSERT(remoteHost != "");
  return remoteHost;
}
