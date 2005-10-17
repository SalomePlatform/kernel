
#ifndef _LIFECYCLECORBATEST_HXX_
#define _LIFECYCLECORBATEST_HXX_

#include <cppunit/extensions/HelperMacros.h>

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_Component)
#include CORBA_CLIENT_HEADER(SALOME_TestComponent)
#include "SALOME_NamingService.hxx"

class LifeCycleCORBATest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( LifeCycleCORBATest );
  CPPUNIT_TEST( testFindOrLoad_Component_LaunchContainer );
  CPPUNIT_TEST( testFindOrLoad_Component_SameInstance );
  CPPUNIT_TEST( testFindOrLoad_Component_PythonInCppContainer );
  CPPUNIT_TEST( testFindOrLoad_Component_PythonSameInstance );
  CPPUNIT_TEST( testFindOrLoad_Component_UnknownInCatalog );
  CPPUNIT_TEST( testFindOrLoad_Component_LaunchContainerHostname );
  CPPUNIT_TEST( testFindOrLoad_Component_SameContainer );
  CPPUNIT_TEST( testFindOrLoad_Component_UnknownMachine );
  CPPUNIT_TEST( testFindOrLoad_Component_ParamsEmpty );
  CPPUNIT_TEST( testFindOrLoad_Component_ParamsLocalContainer );
  CPPUNIT_TEST( testFindOrLoad_Component_ParamsContainerName );
  CPPUNIT_TEST( testFindOrLoad_Component_RemoteComputer );
  CPPUNIT_TEST( testFindOrLoad_Component_ParamsRemoteComputer );
  CPPUNIT_TEST( testFindOrLoad_Component_ParamsRemoteComputer2 );
//   CPPUNIT_TEST( testFindOrLoad_Component_ );
//   CPPUNIT_TEST( testFindOrLoad_Component_ );
//   CPPUNIT_TEST(  );
//   CPPUNIT_TEST(  );
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp();
  void tearDown();

  void testFindOrLoad_Component_LaunchContainer();
  void testFindOrLoad_Component_SameInstance();
  void testFindOrLoad_Component_PythonInCppContainer();
  void testFindOrLoad_Component_PythonSameInstance();
  void testFindOrLoad_Component_UnknownInCatalog();
  void testFindOrLoad_Component_LaunchContainerHostname();
  void testFindOrLoad_Component_SameContainer();
  void testFindOrLoad_Component_UnknownMachine();
  void testFindOrLoad_Component_ParamsEmpty();
  void testFindOrLoad_Component_ParamsLocalContainer();
  void testFindOrLoad_Component_ParamsContainerName();
  void testFindOrLoad_Component_RemoteComputer();
  void testFindOrLoad_Component_ParamsRemoteComputer();
  void testFindOrLoad_Component_ParamsRemoteComputer2();
//   void testFindOrLoad_Component_();
//   void testFindOrLoad_Component_();

protected:
  std::string GetRemoteHost();
  CORBA::ORB_var _orb;
  SALOME_NamingService _NS;
};

#endif
