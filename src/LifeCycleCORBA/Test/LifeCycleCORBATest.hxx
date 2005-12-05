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
// See http://www.salome-platform.org/
//

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
