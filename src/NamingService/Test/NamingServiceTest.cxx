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

#include "NamingServiceTest.hxx"
#include "Utils_ORB_INIT.hxx"
#include "Utils_SINGLETON.hxx"
#include "Basics_Utils.hxx"
#include "Basics_DirUtils.hxx"
#include "SALOME_LifeCycleCORBA.hxx"
#include "utilities.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>


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

// ============================================================================
/*!
 * a basic CORBA object implementation for use with namingService tests
 */
// ============================================================================

NSTEST_echo_i::NSTEST_echo_i()
{
  _num=-1;
}

NSTEST_echo_i::NSTEST_echo_i(CORBA::Long num)
{
  _num=num;
}

NSTEST_echo_i::~NSTEST_echo_i()
{
}

CORBA::Long NSTEST_echo_i::getId()
{
  return _num;
}

// ============================================================================
/*!
 * a factory of CORBA objects for use with namingService tests
 */
// ============================================================================

NSTEST_aFactory_i::NSTEST_aFactory_i()
{
  _num=0;
}

NSTEST_aFactory_i::~NSTEST_aFactory_i()
{
}

NSTEST::echo_ptr NSTEST_aFactory_i::createInstance()
{
  NSTEST_echo_i * anEcho = new NSTEST_echo_i(_num);
  _num++;
  NSTEST::echo_var anEchoRef = anEcho->_this();
  anEcho->_remove_ref();
  return anEchoRef._retn();
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

std::string
NamingServiceTest::_getTraceFileName()
{
  std::string dir = Kernel_Utils::GetTmpDir();
  return dir + "traceUnitTest-NamingServiceTest.log";
}

void
NamingServiceTest::setUp()
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
  //  traceFile.open(theFileName, ios::out | ios::trunc);
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

  // --- Create an NSTEST::factory

  CORBA::Object_var obj = _orb->resolve_initial_references("RootPOA");
  ASSERT(!CORBA::is_nil(obj));
  _root_poa = PortableServer::POA::_narrow(obj);
  _pman = _root_poa->the_POAManager();
  _myFactory  = new NSTEST_aFactory_i();
  _myFactoryId = _root_poa->activate_object(_myFactory);
  _factoryRef = _myFactory->_this();
  _pman->activate();
  _myFactory->_remove_ref();

}

// ============================================================================
/*!
 *  - delete trace classes
 */
// ============================================================================

void
NamingServiceTest::tearDown()
{

  LocalTraceBufferPool* bp1 = LocalTraceBufferPool::instance();
  CPPUNIT_ASSERT(bp1);
  bp1->deleteInstance(bp1);
}

// ============================================================================
/*!
 *  Test default constructor: must be followed by a call to init_orb(ORB)
 */
// ============================================================================

void
NamingServiceTest::testConstructorDefault()
{
  SALOME_NamingService  NS;
  //CPPUNIT_ASSERT_THROW(NS.getIORaddr(),CORBA::Exception);
  NS.init_orb(_orb);

  CORBA::String_var root = NS.getIORaddr();
  CORBA::Object_var obj = _orb->string_to_object(root);
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));

  CosNaming::NamingContext_var rootContext =
    CosNaming::NamingContext::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(rootContext));
}

// ============================================================================
/*!
 *  Test constructor with ORB parameter
 */
// ============================================================================

void
NamingServiceTest::testConstructorOrb()
{
  SALOME_NamingService  NS(_orb);
  CORBA::String_var root = NS.getIORaddr();
  CORBA::Object_var obj = _orb->string_to_object(root);
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));

  CosNaming::NamingContext_var rootContext =
    CosNaming::NamingContext::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(rootContext));
}

// ============================================================================
/*!
 * Test Register and resolve of a single CORBA object with absolute pathname,
 * without subdirectories
 */
// ============================================================================

void
NamingServiceTest::testRegisterResolveAbsNoPath()
{
  _NS.Register(_factoryRef,"/nstest_factory");
  CORBA::Object_var obj = _NS.Resolve("/nstest_factory");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::aFactory_var myFactory = NSTEST::aFactory::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(myFactory));
}

// ============================================================================
/*!
 * Test Register and resolve of a single CORBA object with relative pathname,
 * without subdirectories
 */
// ============================================================================

void
NamingServiceTest::testRegisterResolveRelativeNoPath()
{
  CORBA::Object_var obj = _NS.Resolve("/nstest_factory");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::aFactory_var myFactory = NSTEST::aFactory::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(myFactory));

  _NS.Create_Directory("/myContext");
  _NS.Change_Directory("/myContext");

  NSTEST::echo_var anEchoRef = myFactory->createInstance();
  _NS.Register(anEchoRef,"echo_0");

  obj = _NS.Resolve("echo_0");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::echo_var anEchoRef2 = NSTEST::echo::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(anEchoRef2));
  CPPUNIT_ASSERT(anEchoRef->getId() == anEchoRef2->getId());

  obj = _NS.Resolve("/myContext/echo_0");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::echo_var anEchoRef3 = NSTEST::echo::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(anEchoRef3));
  CPPUNIT_ASSERT(anEchoRef->getId() == anEchoRef3->getId());
}

// ============================================================================
/*!
 * Test Register and resolve of a single CORBA object with absolute pathname,
 * in a subdirectory
 */
// ============================================================================

void
NamingServiceTest::testRegisterResolveAbsWithPath()
{
  CORBA::Object_var obj = _NS.Resolve("/nstest_factory");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::aFactory_var myFactory = NSTEST::aFactory::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(myFactory));

  NSTEST::echo_var anEchoRef = myFactory->createInstance();
  _NS.Register(anEchoRef,"/nstest/echo_0");

  obj = _NS.Resolve("/nstest/echo_0");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::echo_var anEchoRefa = NSTEST::echo::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(anEchoRefa));
  CPPUNIT_ASSERT(anEchoRef->getId() == anEchoRefa->getId());

  NSTEST::echo_var anEchoRef1 = myFactory->createInstance();
  _NS.Register(anEchoRef1,"/nstest2/rep2/rep3/echo_1");
  CPPUNIT_ASSERT(anEchoRef->getId() != anEchoRef1->getId());

  obj = _NS.Resolve("/nstest2/rep2/rep3/echo_1");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::echo_var anEchoRef1a = NSTEST::echo::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(anEchoRef1a));
  CPPUNIT_ASSERT(anEchoRef1->getId() == anEchoRef1a->getId());

  NSTEST::echo_var anEchoRef2 = myFactory->createInstance();
  _NS.Register(anEchoRef2,"/nstest2/1/2/3/4/echo_1");

  obj = _NS.Resolve("/nstest2/1/2/3/4/echo_1");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::echo_var anEchoRef2a = NSTEST::echo::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(anEchoRef2a));
  CPPUNIT_ASSERT(anEchoRef2->getId() == anEchoRef2a->getId());
}

// ============================================================================
/*!
 * Test Register and resolve of a single CORBA object with relative pathname,
 * in a subdirectory.
 * Relative Path is changed to the created subdirectory when Register()
 */
// ============================================================================

void
NamingServiceTest::testRegisterResolveRelativeWithPath()
{
  CORBA::Object_var obj = _NS.Resolve("/nstest_factory");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::aFactory_var myFactory = NSTEST::aFactory::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(myFactory));
  _NS.Create_Directory("/myContext");
  _NS.Change_Directory("/myContext");

  NSTEST::echo_var anEchoRef = myFactory->createInstance();
  _NS.Register(anEchoRef,"subdir/echo_0");

  obj = _NS.Resolve("echo_0");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::echo_var anEchoRef2 = NSTEST::echo::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(anEchoRef2));

  obj = _NS.Resolve("/myContext/subdir/echo_0");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::echo_var anEchoRef3 = NSTEST::echo::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(anEchoRef3));
  CPPUNIT_ASSERT(anEchoRef->getId() == anEchoRef3->getId());

  _NS.Change_Directory("/myContext");
  obj = _NS.Resolve("subdir/echo_0");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::echo_var anEchoRef4 = NSTEST::echo::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(anEchoRef4));
  CPPUNIT_ASSERT(anEchoRef->getId() == anEchoRef4->getId());
}

// ============================================================================
/*!
 * Test resolve with a name not known
 */
// ============================================================================

void
NamingServiceTest::testResolveBadName()
{
  CORBA::Object_var obj = _NS.Resolve("/notRegisteredName");
  CPPUNIT_ASSERT(CORBA::is_nil(obj));

  obj = _NS.Resolve("/nstest/notRegisteredName");
  CPPUNIT_ASSERT(CORBA::is_nil(obj));

  obj = _NS.Resolve("/unknownPath/notRegisteredName");
  CPPUNIT_ASSERT(CORBA::is_nil(obj));

  obj = _NS.Resolve("/anUnknown/ComplicatedPath/notRegisteredName");
  CPPUNIT_ASSERT(CORBA::is_nil(obj));
}

// ============================================================================
/*!
 * Test resolve with a name not known, with a relative path
 */
// ============================================================================

void
NamingServiceTest::testResolveBadNameRelative()
{
  _NS.Create_Directory("/myContext");
  _NS.Change_Directory("/myContext");

  CORBA::Object_var obj = _NS.Resolve("notRegisteredName");
  CPPUNIT_ASSERT(CORBA::is_nil(obj));

  obj = _NS.Resolve("unknownPath/notRegisteredName");
  CPPUNIT_ASSERT(CORBA::is_nil(obj));

  obj = _NS.Resolve("anUnknown/ComplicatedPath/notRegisteredName");
  CPPUNIT_ASSERT(CORBA::is_nil(obj));
}

// ============================================================================
/*!
 * Test register and resolve multiple objects, test resolveFirst.
 * Register a few objects in /nstestfirst/echo_n where n is the object id.
 * Resolve all the objects.
 * ResolveFirst /nstestfirst/echo must give /nstestfirst/echo_i, corresponding
 * to the first object.
 */
// ============================================================================

#define NB_OBJS 10

void
NamingServiceTest::testResolveFirst()
{
  CORBA::Object_var obj = _NS.Resolve("/nstest_factory");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::aFactory_var myFactory = NSTEST::aFactory::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(myFactory));

  int ref[NB_OBJS];

  for (int i=0; i<NB_OBJS; i++)
    {
      NSTEST::echo_var anEchoRef = myFactory->createInstance();
      ref[i] = anEchoRef->getId();
      std::string name = "/nstestfirst/echo_";
      char anum[10];
      sprintf(anum,"%d",ref[i]);
      name += anum;
      _NS.Register(anEchoRef,name.c_str());
    }

  for (int i=0; i<NB_OBJS; i++)
    {
      std::string name = "/nstestfirst/echo_";
      char anum[10];
      sprintf(anum,"%d",ref[i]);
      name += anum;
      obj = _NS.Resolve(name.c_str());
      CPPUNIT_ASSERT(!CORBA::is_nil(obj));
      NSTEST::echo_var anEchoRef = NSTEST::echo::_narrow(obj);
      CPPUNIT_ASSERT(!CORBA::is_nil(anEchoRef));
      CPPUNIT_ASSERT(anEchoRef->getId() == ref[i]);
    }

  std::string name = "/nstestfirst/echo";
  obj = _NS.ResolveFirst(name.c_str());
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::echo_var anEchoRef = NSTEST::echo::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(anEchoRef));
  CPPUNIT_ASSERT(anEchoRef->getId() == ref[0]);
}

// ============================================================================
/*!
 * Test register and resolve multiple objects, test resolveFirst, relative path
 * Register a few objects in /nstestfirstrel/echo_n where n is the object id.
 * Resolve all the objects.
 * ResolveFirst echo with a relative path /nstestfirstrel must give
 * /nstestfirst/echo_i, corresponding to the first object.
 */
// ============================================================================

void
NamingServiceTest::testResolveFirstRelative()
{
  CORBA::Object_var obj = _NS.Resolve("/nstest_factory");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::aFactory_var myFactory = NSTEST::aFactory::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(myFactory));

  int ref[NB_OBJS];

  for (int i=0; i<NB_OBJS; i++)
    {
      NSTEST::echo_var anEchoRef = myFactory->createInstance();
      ref[i] = anEchoRef->getId();
      std::string name = "/nstestfirstrel/echo_";
      char anum[10];
      sprintf(anum,"%d",ref[i]);
      name += anum;
      _NS.Register(anEchoRef,name.c_str());
    }

  for (int i=0; i<NB_OBJS; i++)
    {
      _NS.Change_Directory("/nstestfirstrel");
      std::string name = "echo_";
      char anum[10];
      sprintf(anum,"%d",ref[i]);
      name += anum;
      obj = _NS.Resolve(name.c_str());
      CPPUNIT_ASSERT(!CORBA::is_nil(obj));
      NSTEST::echo_var anEchoRef = NSTEST::echo::_narrow(obj);
      CPPUNIT_ASSERT(!CORBA::is_nil(anEchoRef));
      CPPUNIT_ASSERT(anEchoRef->getId() == ref[i]);
    }

  _NS.Change_Directory("/nstestfirstrel");
  std::string name = "echo";
  obj = _NS.ResolveFirst(name.c_str());
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
}

// ============================================================================
/*!
 * Test resolveFirst with unknown name
 */
// ============================================================================

void
NamingServiceTest::testResolveFirstUnknown()
{
  std::string name = "/notYeyRegistered";
  CORBA::Object_var obj= _NS.ResolveFirst(name.c_str());
  CPPUNIT_ASSERT(CORBA::is_nil(obj));

  name = "/nstestfirst/notYeyRegistered";
  obj = _NS.ResolveFirst(name.c_str());
  CPPUNIT_ASSERT(CORBA::is_nil(obj));

  name = "/rrr/sss/ttt/notYeyRegistered";
  obj = _NS.ResolveFirst(name.c_str());
  CPPUNIT_ASSERT(CORBA::is_nil(obj));
}

// ============================================================================
/*!
 * Test resolveFirst with unknown name, relative Path
 */
// ============================================================================

void
NamingServiceTest::testResolveFirstUnknownRelative()
{
  _NS.Create_Directory("/myContext");
  _NS.Change_Directory("/myContext");

  std::string name = "RelnotYeyRegistered";
  CORBA::Object_var obj = _NS.ResolveFirst(name.c_str());
  CPPUNIT_ASSERT(CORBA::is_nil(obj));

  name = "Relnstestfirst/notYeyRegistered";
  obj = _NS.ResolveFirst(name.c_str());
  CPPUNIT_ASSERT(CORBA::is_nil(obj));

  name = "Relrrr/sss/ttt/notYeyRegistered";
  obj = _NS.ResolveFirst(name.c_str());
  CPPUNIT_ASSERT(CORBA::is_nil(obj));
}

// ============================================================================
/*!
 * Test ResolveComponent works as specified
 */
// ============================================================================

void
NamingServiceTest::testResolveComponentOK()
{
  CORBA::Object_var obj = _NS.Resolve("/nstest_factory");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::aFactory_var myFactory = NSTEST::aFactory::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(myFactory));

  NSTEST::echo_var anEchoRef = myFactory->createInstance();
  _NS.Register(anEchoRef,
               "/Containers/theHostName/theContainerName/theComponentName");

  obj = _NS.ResolveComponent("theHostName",
                             "theContainerName",
                             "theComponentName");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::echo_var anEchoRefa = NSTEST::echo::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(anEchoRefa));
  CPPUNIT_ASSERT(anEchoRefa->getId() == anEchoRef->getId());


  NSTEST::echo_var anEchoRef2 = myFactory->createInstance();
  _NS.Register(anEchoRef2,
               "/Containers/theHostName/theContainerName_2/theComponentName");

  obj = _NS.ResolveComponent("theHostName",
                             "theContainerName",
                             "theComponentName",
                             2);
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::echo_var anEchoRefb = NSTEST::echo::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(anEchoRefb));
  CPPUNIT_ASSERT(anEchoRefb->getId() == anEchoRef2->getId());
}

// ============================================================================
/*!
 * Test ResolveComponent gives nil pointer if hostname is not given (empty)
 */
// ============================================================================

void
NamingServiceTest::testResolveComponentEmptyHostname()
{
  CORBA::Object_var obj = _NS.Resolve("/nstest_factory");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::aFactory_var myFactory = NSTEST::aFactory::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(myFactory));

  NSTEST::echo_var anEchoRef = myFactory->createInstance();
  _NS.Register(anEchoRef,
               "/Containers/theHostName/theContainerName/theComponentName");

  obj = _NS.ResolveComponent("",
                             "theContainerName",
                             "theComponentName");
  CPPUNIT_ASSERT(CORBA::is_nil(obj));
}

// ============================================================================
/*!
 * Test ResolveComponent gives nil pointer if hostname is unknown
 */
// ============================================================================

void
NamingServiceTest::testResolveComponentUnknownHostname()
{
  CORBA::Object_var obj = _NS.Resolve("/nstest_factory");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::aFactory_var myFactory = NSTEST::aFactory::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(myFactory));

  NSTEST::echo_var anEchoRef = myFactory->createInstance();
  _NS.Register(anEchoRef,
               "/Containers/theHostName/theContainerName/theComponentName");

  obj = _NS.ResolveComponent("anUnknownHostName",
                             "theContainerName",
                             "theComponentName");
  CPPUNIT_ASSERT(CORBA::is_nil(obj));
}

// ============================================================================
/*!
 * Test ResolveComponent when containerName is empty.
 * check bad hostname gives nil pointer.
 * If componentName registered on a container from hostname, a component
 * reference is found (the first one).
 * Else give nil pointer.
 */
// ============================================================================

void
NamingServiceTest::testResolveComponentEmptyContainerName()
{
  CORBA::Object_var obj = _NS.Resolve("/nstest_factory");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::aFactory_var myFactory = NSTEST::aFactory::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(myFactory));

  NSTEST::echo_var anEchoRef = myFactory->createInstance();
  _NS.Register(anEchoRef,
               "/Containers/theHostName/theContainerName/theComponentName");

  NSTEST::echo_var anEchoRef2 = myFactory->createInstance();
  _NS.Register(anEchoRef2,
               "/Containers/theHostName/aContainerName/aComponentName");

  NSTEST::echo_var anEchoRef3 = myFactory->createInstance();
  _NS.Register(anEchoRef3,
               "/Containers/theHostName/otherContainerName/theComponentName");

  obj = _NS.ResolveComponent("anUnknownHostName",
                             "",
                             "theComponentName");
  CPPUNIT_ASSERT(CORBA::is_nil(obj));

  obj = _NS.ResolveComponent("theHostName",
                             "",
                             "theComponentName");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::echo_var anEchoRefa = NSTEST::echo::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(anEchoRefa));
  CPPUNIT_ASSERT(anEchoRefa->getId() == anEchoRef->getId());
}

// ============================================================================
/*!
 * Test
 */
// ============================================================================

void
NamingServiceTest::testResolveComponentUnknownContainerName()
{
  CORBA::Object_var obj = _NS.Resolve("/nstest_factory");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::aFactory_var myFactory = NSTEST::aFactory::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(myFactory));

  NSTEST::echo_var anEchoRef = myFactory->createInstance();
  _NS.Register(anEchoRef,
               "/Containers/theHostName/theContainerName/theComponentName");

  NSTEST::echo_var anEchoRef2 = myFactory->createInstance();
  _NS.Register(anEchoRef2,
               "/Containers/theHostName/aContainerName/aComponentName");

  NSTEST::echo_var anEchoRef3 = myFactory->createInstance();
  _NS.Register(anEchoRef3,
               "/Containers/theHostName/otherContainerName/theComponentName");

  obj = _NS.ResolveComponent("theHostName",
                             "anUnknownContainerName",
                             "theComponentName");
  CPPUNIT_ASSERT(CORBA::is_nil(obj));
}

// ============================================================================
/*!
 * Test
 */
// ============================================================================

void
NamingServiceTest::testResolveComponentEmptyComponentName()
{
  CORBA::Object_var obj = _NS.Resolve("/nstest_factory");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::aFactory_var myFactory = NSTEST::aFactory::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(myFactory));

  NSTEST::echo_var anEchoRef = myFactory->createInstance();
  _NS.Register(anEchoRef,
               "/Containers/theHostName/theContainerName/theComponentName");

  NSTEST::echo_var anEchoRef2 = myFactory->createInstance();
  _NS.Register(anEchoRef2,
               "/Containers/theHostName/EmptyContainerName/");

  obj = _NS.ResolveComponent("theHostName",
                             "EmptyContainerName",
                             "");
  CPPUNIT_ASSERT(CORBA::is_nil(obj));
}

// ============================================================================
/*!
 * Test
 */
// ============================================================================

void
NamingServiceTest::testResolveComponentUnknownComponentName()
{
  CORBA::Object_var obj = _NS.Resolve("/nstest_factory");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::aFactory_var myFactory = NSTEST::aFactory::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(myFactory));

  NSTEST::echo_var anEchoRef = myFactory->createInstance();
  _NS.Register(anEchoRef,
               "/Containers/theHostName/theContainerName/theComponentName");

  obj = _NS.ResolveComponent("theHostName",
                             "theContainerName",
                             "anUnknownComponentName");
  CPPUNIT_ASSERT(CORBA::is_nil(obj));
}

// ============================================================================
/*!
 * Test with a false number nbproc.
 * A positive number not corresponding to a registered component gives nil ref.
 * A negative number is not taken into account and may give a non nil ref.
 */
// ============================================================================

void
NamingServiceTest::testResolveComponentFalseNbproc()
{
  CORBA::Object_var obj = _NS.Resolve("/nstest_factory");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::aFactory_var myFactory = NSTEST::aFactory::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(myFactory));

  NSTEST::echo_var anEchoRef = myFactory->createInstance();
  _NS.Register(anEchoRef,
               "/Containers/theHostName/theContainerName/theComponentName");

  obj = _NS.ResolveComponent("theHostName",
                             "theContainerName",
                             "theComponentName",
                             25);
  CPPUNIT_ASSERT(CORBA::is_nil(obj));

  obj = _NS.ResolveComponent("theHostName",
                             "theContainerName",
                             "theComponentName",
                             -25);
  CPPUNIT_ASSERT(! CORBA::is_nil(obj));
}

// ============================================================================
/*!
 * Test
 */
// ============================================================================

void
NamingServiceTest::testContainerName()
{
  std::string ref0 = "FactoryServer";
  std::string ret = _NS.ContainerName("");
  CPPUNIT_ASSERT(ret == ref0);

  ref0 = "MyContainerName";
  ret = _NS.ContainerName(ref0.c_str());
  CPPUNIT_ASSERT(ret == ref0);
}

// ============================================================================
/*!
 * Test
 */
// ============================================================================

void
NamingServiceTest::testContainerNameParams()
{
  Engines::ContainerParameters params;
  SALOME_LifeCycleCORBA::preSet(params);

  std::string ref0 = "FactoryServer";
  std::string ret = _NS.ContainerName(params);
  CPPUNIT_ASSERT(ret == ref0);

  ref0 = "MyContainerName";
  params.container_name = ref0.c_str();
  ret = _NS.ContainerName(params);
  CPPUNIT_ASSERT(ret == ref0);
}

// ============================================================================
/*!
 * Test
 */
// ============================================================================

void
NamingServiceTest::testBuildContainerNameForNS()
{
  std::string ref0 = "/Containers/theHostName/theContainerName";
  std::string ret = _NS.BuildContainerNameForNS("theContainerName","theHostName");
  CPPUNIT_ASSERT(ret == ref0);

  ref0 = "/Containers/theHostName/FactoryServer";
  ret = _NS.BuildContainerNameForNS("","theHostName");
  CPPUNIT_ASSERT(ret == ref0);
}

// ============================================================================
/*!
 * Test
 */
// ============================================================================

void
NamingServiceTest::testBuildContainerNameForNSParams()
{
  Engines::ContainerParameters params;
  SALOME_LifeCycleCORBA::preSet(params);

  params.container_name = "theContainerName";
  std::string ref0 = "/Containers/theHostName/theContainerName";
  std::string ret = _NS.BuildContainerNameForNS(params,"theHostName");
  CPPUNIT_ASSERT(ret == ref0);

  params.container_name = "";
  ref0 = "/Containers/theHostName/FactoryServer";
  ret = _NS.BuildContainerNameForNS(params,"theHostName");
  CPPUNIT_ASSERT(ret == ref0);
}

// ============================================================================
/*!
 * Test
 */
// ============================================================================

void
NamingServiceTest::testFind()
{
  CORBA::Object_var obj = _NS.Resolve("/nstest_factory");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::aFactory_var myFactory = NSTEST::aFactory::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(myFactory));

  NSTEST::echo_var anEchoRef = myFactory->createInstance();
  _NS.Register(anEchoRef,
               "/Containers/theHostName/theContainerName/theComponentName");

  NSTEST::echo_var anEchoRef2 = myFactory->createInstance();
  _NS.Register(anEchoRef2,
               "/Containers/theHostName/aContainerName/aComponentName");

  NSTEST::echo_var anEchoRef3 = myFactory->createInstance();
  _NS.Register(anEchoRef3,
               "/Containers/theHostName/otherContainerName/theComponentName");

  NSTEST::echo_var anEchoRef4 = myFactory->createInstance();
  _NS.Register(anEchoRef4,
               "/Containers/anHostName/oneContainerName/theComponentName");

  _NS.Change_Directory("/Containers");
  int occ= _NS.Find("theComponentName");
  CPPUNIT_ASSERT(occ >= 3); // see previous tests

  _NS.Change_Directory("/Containers");
  occ= _NS.Find("aComponentName");
  CPPUNIT_ASSERT(occ == 1);

  _NS.Change_Directory("/Containers");
  occ= _NS.Find("UnknownCompnentName");
  CPPUNIT_ASSERT(occ == 0);
}

// ============================================================================
/*!
 * Test
 */
// ============================================================================

void
NamingServiceTest::testCreateDirectory()
{
  CORBA::Object_var obj = _NS.Resolve("/nstest_factory");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::aFactory_var myFactory = NSTEST::aFactory::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(myFactory));

  bool ret = _NS.Create_Directory("/aaa/bbb/ccc/ddd/eee");
  CPPUNIT_ASSERT(ret);

  _NS.Change_Directory("/aaa/bbb/ccc/ddd/eee");
  NSTEST::echo_var anEchoRef = myFactory->createInstance();
  int val = anEchoRef->getId();
  std::string name = "echo_";
  char anum[10];
  sprintf(anum,"%d",val);
  name += anum;
  _NS.Register(anEchoRef,name.c_str());

  std::string dirname = "/aaa/bbb/ccc/ddd/eee/";
  dirname += name;
  obj = _NS.Resolve(dirname.c_str());
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::echo_var anEchoRef2 = NSTEST::echo::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(anEchoRef2));
  CPPUNIT_ASSERT(anEchoRef->getId() == anEchoRef2->getId());

  ret = _NS.Create_Directory("/aaa/bbb/ccc/ddd/eee");
  CPPUNIT_ASSERT(ret);

  _NS.Change_Directory("/aaa/bbb");
  ret = _NS.Create_Directory("cccccc/dddddd/eeeeee");
  _NS.Register(anEchoRef,"echo_abcde");

  CPPUNIT_ASSERT(ret);
  _NS.Change_Directory("/aaa/bbb/cccccc/dddddd/eeeeee");
  obj = _NS.Resolve("echo_abcde");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
}

// ============================================================================
/*!
 * Test
 */
// ============================================================================

void
NamingServiceTest::testChangeDirectory()
{
  CORBA::Object_var obj = _NS.Resolve("/nstest_factory");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::aFactory_var myFactory = NSTEST::aFactory::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(myFactory));

  NSTEST::echo_var anEchoRef = myFactory->createInstance();
  _NS.Register(anEchoRef,
               "/Containers/theHostName/theContainerName/theComponentName");

  NSTEST::echo_var anEchoRef2 = myFactory->createInstance();
  _NS.Register(anEchoRef2,
               "/Containers/theHostName/aContainerName/aComponentName");

  NSTEST::echo_var anEchoRef3 = myFactory->createInstance();
  _NS.Register(anEchoRef3,
               "/Containers/theHostName/otherContainerName/theComponentName");

  NSTEST::echo_var anEchoRef4 = myFactory->createInstance();
  _NS.Register(anEchoRef4,
               "/Containers/anHostName/oneContainerName/theComponentName");

  _NS.Change_Directory("/Containers/theHostName/otherContainerName");
  obj = _NS.Resolve("theComponentName");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));

  NSTEST::echo_var anEchoRefa = NSTEST::echo::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(anEchoRefa));
  CPPUNIT_ASSERT(anEchoRefa->getId() == anEchoRef3->getId());
}

// ============================================================================
/*!
 * Test
 */
// ============================================================================

void
NamingServiceTest::testCurrentDirectory()
{
  std::string path = "/aaa/bbb/ccc/ddd/eee";
  bool ret = _NS.Create_Directory(path.c_str());
  CPPUNIT_ASSERT(ret);

  _NS.Change_Directory(path.c_str());
  char* acurdir = _NS.Current_Directory();
  std::string curdir = acurdir;
  free(acurdir);
  CPPUNIT_ASSERT(curdir == path);
}

// ============================================================================
/*!
 * Test
 */
// ============================================================================

void
NamingServiceTest::testList()
{
  _NS.Change_Directory("/Containers/theHostName/theContainerName");
  _NS.list();
  _NS.Change_Directory("/Containers");
  _NS.list();
}

// ============================================================================
/*!
 * Test
 */
// ============================================================================

void
NamingServiceTest::testListDirectory()
{
  _NS.Change_Directory("/Containers/theHostName/theContainerName");
  _NS.list_directory();
  _NS.Change_Directory("/Containers");
  _NS.list_directory();
}

// ============================================================================
/*!
 * Test
 */
// ============================================================================

void
NamingServiceTest::testListDirectoryRecurs()
{
  _NS.Change_Directory("/Containers/theHostName/theContainerName");
  _NS.list_directory_recurs();
  _NS.Change_Directory("/Containers");
  _NS.list_directory_recurs();
  _NS.Change_Directory("/");
  _NS.list_directory_recurs();
}


// ============================================================================
/*!
 * Test
 */
// ============================================================================

void
NamingServiceTest::testListSubdirs()
{
  _NS.Change_Directory("/Containers/theHostName/theContainerName");
  _NS.list_subdirs();
  _NS.Change_Directory("/Containers");
  _NS.list_subdirs();
  _NS.Change_Directory("/");
  _NS.list_subdirs();
}
// ============================================================================
/*!
 * Test
 */
// ============================================================================

void
NamingServiceTest::testDestroyName()
{
  CORBA::Object_var obj = _NS.Resolve("/nstest_factory");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::aFactory_var myFactory = NSTEST::aFactory::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(myFactory));

  std::string path = "/Containers/theHostName/theContainerName/theComponentName";

  NSTEST::echo_var anEchoRef = myFactory->createInstance();
  _NS.Register(anEchoRef, path.c_str());

  obj=_NS.Resolve(path.c_str());
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));

  _NS.Destroy_Name(path.c_str());
  obj=_NS.Resolve(path.c_str());
  CPPUNIT_ASSERT(CORBA::is_nil(obj));
}

// ============================================================================
/*!
 * Test
 */
// ============================================================================

void
NamingServiceTest::testDestroyDirectory()
{
  CORBA::Object_var obj = _NS.Resolve("/nstest_factory");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
  NSTEST::aFactory_var myFactory = NSTEST::aFactory::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(myFactory));

  std::string path = "/Containers/theHostName/theContainerName/theComponentName";

  NSTEST::echo_var anEchoRef = myFactory->createInstance();
  _NS.Register(anEchoRef, path.c_str());

  _NS.Destroy_Directory("/Containers/theHostName/theContainerName");
  obj=_NS.Resolve(path.c_str());
  CPPUNIT_ASSERT(!CORBA::is_nil(obj)); // directory not empty: not destroyed

  _NS.Destroy_Name(path.c_str());
  _NS.Destroy_Directory("/Containers/theHostName/theContainerName");
  _NS.Change_Directory("/Containers/theHostName");
  _NS.list_subdirs();
}

// ============================================================================
/*!
 * DestroyFullDirectory is not recursive
 * Need Housekeeping of /Containers for further tests !
 */
// ============================================================================

void NamingServiceTest::_destroyDirectoryRecurs(std::string path)
{
  std::string current = path;
  SCRUTE(path);
  if (_NS.Change_Directory(path.c_str()))
    {
      std::vector<std::string> subdirs = _NS.list_subdirs();
      for (int i=0; i<subdirs.size(); i++)
        {
          std::string subpath=path + "/" +subdirs[i];
          _destroyDirectoryRecurs(subpath);
        }
      if (_NS.Change_Directory(path.c_str()))
        {
          _NS.Destroy_FullDirectory(path.c_str());
        }
    }
}

void
NamingServiceTest::testDestroyFullDirectory()
{
  _NS.Destroy_FullDirectory("/Containers");
  CPPUNIT_ASSERT(_NS.Change_Directory("/Containers"));
  std::vector<std::string> subdirs = _NS.list_subdirs();
  CPPUNIT_ASSERT(subdirs.size() >0);
  _NS.list_directory_recurs();
  std::string path = "/Containers";
  _destroyDirectoryRecurs(path);
  CPPUNIT_ASSERT( ! _NS.Change_Directory("/Containers"));
  _NS.Change_Directory("/");
  _NS.list_subdirs();
  _NS.list_directory_recurs();
}

// ============================================================================
/*!
 * Test
 */
// ============================================================================

void
NamingServiceTest::testGetIorAddr()
{
  CORBA::String_var root = _NS.getIORaddr();
  CORBA::Object_var obj = _orb->string_to_object(root);
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));
}

// ============================================================================
/*!
 * Test
 */
// ============================================================================

// void
// NamingServiceTest::()
// {
//   CPPUNIT_ASSERT(0);
// }

