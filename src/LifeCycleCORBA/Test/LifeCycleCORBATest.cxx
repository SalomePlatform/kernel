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

#include "LifeCycleCORBATest.hxx"
#include "SALOME_LifeCycleCORBA.hxx"
#include "SALOME_FileTransferCORBA.hxx"
#include "Utils_ORB_INIT.hxx"
#include "Utils_SINGLETON.hxx"
#include "Basics_Utils.hxx"
#include "Basics_DirUtils.hxx"
#include "utilities.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>


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
#ifdef WIN32
#define setenv Kernel_Utils::setenv
#endif

std::string
LifeCycleCORBATest::_getTraceFileName()
{
  std::string dir = Kernel_Utils::GetTmpDir();
  return dir + "traceUnitTest-LifeCycleCORBATest.log";
}

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
  std::string theFileName = _getTraceFileName();

  std::string s = "file:";
  s += theFileName;
  //s="local";
  //s="with_logger";
  CPPUNIT_ASSERT(! setenv("SALOME_trace",s.c_str(),1)); // 1: overwrite

  std::ofstream traceFile;
  //  traceFile.open(theFileName, std::ios::out | std::ios::trunc);
  traceFile.open(theFileName.c_str(), std::ios::out | std::ios::app);
  CPPUNIT_ASSERT(traceFile); // file created empty, then closed
  traceFile.close();

  bp1 = LocalTraceBufferPool::instance();
  CPPUNIT_ASSERT(bp1);

  // --- Get or initialize the orb

  int _argc = 1;
  char* _argv[] = {(char*)""};
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

  std::string containerName = "myContainer";
  Engines::EngineComponent_var mycompo =
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

  std::string containerName = "myContainer";

  Engines::EngineComponent_var mycompo1 =
    _LCC.FindOrLoad_Component(containerName.c_str(),"SalomeTestComponent");
  CPPUNIT_ASSERT(!CORBA::is_nil(mycompo1));

  Engines::EngineComponent_var mycompo2 =
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

  std::string name1 = m1->instanceName();
  std::string name2 = m2->instanceName();
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

  std::string containerName = "myContainer";

  Engines::EngineComponent_var mycompo1 =
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

  std::string containerName = "myContainer";

  Engines::EngineComponent_var mycompo1 =
    _LCC.FindOrLoad_Component(containerName.c_str(),"SALOME_TestComponentPy");
  CPPUNIT_ASSERT(!CORBA::is_nil(mycompo1));

  Engines::EngineComponent_var mycompo2 =
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

  std::string name1 = m1->instanceName();
  std::string name2 = m2->instanceName();
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

  std::string containerName = "myContainer";

  Engines::EngineComponent_var mycompo1 =
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

  std::string containerName = Kernel_Utils::GetHostname();
  containerName += "/theContainer";
  DEVTRACE("containerName = " << containerName);
  Engines::EngineComponent_var mycompo =
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

  std::string containerName = "aContainer";

  Engines::EngineComponent_var mycompo1 =
    _LCC.FindOrLoad_Component(containerName.c_str(),"SalomeTestComponent");
  CPPUNIT_ASSERT(!CORBA::is_nil(mycompo1));

  containerName = Kernel_Utils::GetHostname();
  containerName += "/aContainer";
  DEVTRACE("containerName = " << containerName);
  Engines::EngineComponent_var mycompo2 =
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

  std::string name1 = m1->instanceName();
  std::string name2 = m2->instanceName();
  CPPUNIT_ASSERT_EQUAL(name1, name2);

  // --- check containers are the same servant (same container name+hostname)

  Engines::Container_var c1 = m1->GetContainerRef();
  CPPUNIT_ASSERT(!CORBA::is_nil(c1));
  Engines::Container_var c2 = m2->GetContainerRef();
  CPPUNIT_ASSERT(!CORBA::is_nil(c1));
  std::string cname1 = c1->name();
  std::string cname2 = c2->name();
  CPPUNIT_ASSERT_EQUAL(cname1, cname2);
  std::string hostname1 = c1->getHostName();
  std::string hostname2 = c2->getHostName();
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

  std::string containerName = "aFarAwayComputer";
  containerName += "/theContainer";
//   CPPUNIT_ASSERT_THROW(Engines::Component_var mycompo =
//                     _LCC.FindOrLoad_Component(containerName.c_str(),"SalomeTestComponent");,SALOME::SALOME_Exception);
  try
    {
      Engines::EngineComponent_var mycompo =
        _LCC.FindOrLoad_Component(containerName.c_str(),"SalomeTestComponent");
    }
  catch(const SALOME::SALOME_Exception &ex)
    {
      CPPUNIT_ASSERT(true);
//       std::string expectedMessage = "BAD PARAM";
//       std::ostream os;
//       os << ex;
//       std::string actualMessage = os.str();
//       DEVTRACE("actual Exception Message = " << actualMessage);
//       CPPUNIT_ASSERT(actualMessage.find(expectedMessage) != std::string::npos);
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

  Engines::ContainerParameters params;
  _LCC.preSet(params);
  Engines::EngineComponent_var mycompo =
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

  Engines::ContainerParameters params;
  _LCC.preSet(params);
  std::string hostname=Kernel_Utils::GetHostname();
  params.resource_params.hostname=hostname.c_str();
  Engines::EngineComponent_var mycompo =
    _LCC.FindOrLoad_Component(params,"SalomeTestComponent");
  CPPUNIT_ASSERT(!CORBA::is_nil(mycompo));

  // --- check narrow

  Engines::TestComponent_var m1;
  m1 = Engines::TestComponent::_narrow(mycompo);

  // --- check that container is on local computer

  CPPUNIT_ASSERT(!CORBA::is_nil(m1));
  Engines::Container_var c1 = m1->GetContainerRef();
  CPPUNIT_ASSERT(!CORBA::is_nil(c1));
  std::string hostname1 = c1->getHostName();
  CPPUNIT_ASSERT_EQUAL(hostname1, Kernel_Utils::GetHostname());
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

  Engines::ContainerParameters params;
  _LCC.preSet(params);
  std::string containerName = "myContainer";
  params.container_name = containerName.c_str();
  Engines::EngineComponent_var mycompo =
    _LCC.FindOrLoad_Component(params,"SalomeTestComponent");
  CPPUNIT_ASSERT(!CORBA::is_nil(mycompo));

  // --- check narrow

  Engines::TestComponent_var m1;
  m1 = Engines::TestComponent::_narrow(mycompo);

  // --- check that container has good name

  CPPUNIT_ASSERT(!CORBA::is_nil(m1));
  Engines::Container_var c1 = m1->GetContainerRef();
  CPPUNIT_ASSERT(!CORBA::is_nil(c1));
  std::string hostname1 = c1->getHostName();
  CPPUNIT_ASSERT_EQUAL(hostname1, Kernel_Utils::GetHostname());
  std::string cname1 = c1->name();
  CPPUNIT_ASSERT(cname1.find(containerName) != std::string::npos);
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

  std::string remoteHost = GetRemoteHost();

  std::string containerName = remoteHost;
  containerName += "/aContainer";
  DEVTRACE("containerName = " << containerName);
  Engines::EngineComponent_var mycompo1 =
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
  std::string hostname1 = c1->getHostName();
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

  std::string remoteHost = GetRemoteHost();

  Engines::ContainerParameters params;
  _LCC.preSet(params);
  params.resource_params.hostname = remoteHost.c_str();

  Engines::EngineComponent_var mycompo1 =
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
  std::string hostname1 = c1->getHostName();
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

  std::string remoteHost = GetRemoteHost();

  Engines::ContainerParameters params;
  _LCC.preSet(params);
  params.resource_params.hostname = remoteHost.c_str();
  params.container_name = "anotherContainer";

  Engines::EngineComponent_var mycompo1 =
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
  std::string hostname1 = c1->getHostName();
  CPPUNIT_ASSERT_EQUAL(hostname1, remoteHost);
}

// ============================================================================
/*!
 * Check SALOME_FileTransferCORBA on local machine
 */
// ============================================================================

void  LifeCycleCORBATest::testgetLocalFile_localComputer()
{
  SALOME_LifeCycleCORBA _LCC(&_NS);
  std::string origFileName = getenv("KERNEL_ROOT_DIR");
  origFileName += "/lib/salome/libSalomeLifeCycleCORBA.so.0.0.0";
  SALOME_FileTransferCORBA transfer( Kernel_Utils::GetHostname(),
                                     origFileName);
  std::string local = transfer.getLocalFile();
  CPPUNIT_ASSERT(!local.empty());
  CPPUNIT_ASSERT_EQUAL(local, origFileName);
}

// ============================================================================
/*!
 * Check SALOME_FileTransferCORBA on remote machine
 */
// ============================================================================

void  LifeCycleCORBATest::testgetLocalFile_remoteComputer()
{
  SALOME_LifeCycleCORBA _LCC(&_NS);
  std::string origFileName = getenv("KERNEL_ROOT_DIR");
  origFileName += "/lib/salome/libSalomeContainer.so.0.0.0";
  SALOME_FileTransferCORBA transfer( GetRemoteHost(),
                                     origFileName);
  std::string local = transfer.getLocalFile();
  CPPUNIT_ASSERT(!local.empty());
  std::string local2 = transfer.getLocalFile();
  CPPUNIT_ASSERT(!local2.empty());
  CPPUNIT_ASSERT_EQUAL(local, local2);
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

std::string LifeCycleCORBATest::GetRemoteHost()
{
  SALOME_LifeCycleCORBA _LCC(&_NS);

  CORBA::Object_var obj = _NS.Resolve("/ResourcesManager");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  Engines::ResourcesManager_var resourcesManager =
    Engines::ResourcesManager::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(resourcesManager));

  Engines::ContainerParameters params;
  _LCC.preSet(params);               // empty params to get all the machines
  params.resource_params.componentList.length(1);
  params.resource_params.componentList[0]="SalomeTestComponent";
  params.resource_params.can_run_containers = true;

  Engines::ResourceList_var hostList = resourcesManager->GetFittingResources(params.resource_params);
  CPPUNIT_ASSERT(hostList->length() > 1);

  std::string localHost = Kernel_Utils::GetHostname();
  std::string remoteHost;
  for (unsigned int i=0; i < hostList->length(); i++)
    {
      const char* aMachine = hostList[i];
      Engines::ResourceDefinition_var resource_definition = resourcesManager->GetResourceDefinition(aMachine);
      std::string machine(resource_definition->hostname.in());
      if (machine != localHost)
        {
          remoteHost = machine;
          break;
        }
    }
  CPPUNIT_ASSERT(remoteHost != "");
  return remoteHost;
}
