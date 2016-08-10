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

#ifndef _NAMINGSERVICETEST_HXX_
#define _NAMINGSERVICETEST_HXX_

#include <cppunit/extensions/HelperMacros.h>
#include "SALOME_NamingService.hxx"
#include <string>

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(nstest)

#ifdef WIN32
# if defined NAMINGSERVICETEST_EXPORTS || defined NamingServiceTest_EXPORTS
#  define NAMINGSERVICETEST_EXPORT __declspec( dllexport )
# else
#  define NAMINGSERVICETEST_EXPORT __declspec( dllimport )
# endif
#else
# define NAMINGSERVICETEST_EXPORT
#endif

class NAMINGSERVICETEST_EXPORT NSTEST_echo_i : public virtual POA_NSTEST::echo,
                      public virtual PortableServer::ServantBase
{
public:
  NSTEST_echo_i();
  NSTEST_echo_i(CORBA::Long num);
  ~NSTEST_echo_i();
  CORBA::Long getId();
private:
  int _num;
};

class NSTEST_aFactory_i : public virtual POA_NSTEST::aFactory,
                          public virtual PortableServer::ServantBase
{
public:
  NSTEST_aFactory_i();
  ~NSTEST_aFactory_i();
  NSTEST::echo_ptr createInstance();
private:
  int _num;
};

class NAMINGSERVICETEST_EXPORT NamingServiceTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( NamingServiceTest );
  CPPUNIT_TEST( testConstructorDefault );
  CPPUNIT_TEST( testConstructorOrb );
  CPPUNIT_TEST( testRegisterResolveAbsNoPath );
  CPPUNIT_TEST( testRegisterResolveRelativeNoPath );
  CPPUNIT_TEST( testRegisterResolveAbsWithPath );
  CPPUNIT_TEST( testRegisterResolveRelativeWithPath );
  CPPUNIT_TEST( testResolveBadName );
  CPPUNIT_TEST( testResolveBadNameRelative );
  CPPUNIT_TEST( testResolveFirst );
  CPPUNIT_TEST( testResolveFirstRelative );
  CPPUNIT_TEST( testResolveFirstUnknown );
  CPPUNIT_TEST( testResolveFirstUnknownRelative );
  CPPUNIT_TEST( testResolveComponentOK );
  CPPUNIT_TEST( testResolveComponentEmptyHostname );
  CPPUNIT_TEST( testResolveComponentUnknownHostname );
  CPPUNIT_TEST( testResolveComponentEmptyContainerName );
  CPPUNIT_TEST( testResolveComponentUnknownContainerName );
  CPPUNIT_TEST( testResolveComponentEmptyComponentName );
  CPPUNIT_TEST( testResolveComponentUnknownComponentName );
  CPPUNIT_TEST( testResolveComponentFalseNbproc );
  CPPUNIT_TEST( testContainerName );
  CPPUNIT_TEST( testContainerNameParams );
  CPPUNIT_TEST( testBuildContainerNameForNS );
  CPPUNIT_TEST( testBuildContainerNameForNSParams );
  CPPUNIT_TEST( testFind );
  CPPUNIT_TEST( testCreateDirectory );
  CPPUNIT_TEST( testChangeDirectory );
  CPPUNIT_TEST( testCurrentDirectory );
  CPPUNIT_TEST( testList );
  CPPUNIT_TEST( testListDirectory );
  CPPUNIT_TEST( testListDirectoryRecurs );
  CPPUNIT_TEST( testListSubdirs );
  CPPUNIT_TEST( testDestroyName );
  CPPUNIT_TEST( testDestroyDirectory );
  CPPUNIT_TEST( testDestroyFullDirectory );
  CPPUNIT_TEST( testGetIorAddr );
//   CPPUNIT_TEST(  );
//   CPPUNIT_TEST(  );
//   CPPUNIT_TEST(  );

  CPPUNIT_TEST_SUITE_END();

public:

  void setUp();
  void tearDown();

  void testConstructorDefault();
  void testConstructorOrb();
  void testRegisterResolveAbsNoPath();
  void testRegisterResolveRelativeNoPath();
  void testRegisterResolveAbsWithPath();
  void testRegisterResolveRelativeWithPath();
  void testResolveBadName();
  void testResolveBadNameRelative();
  void testResolveFirst();
  void testResolveFirstRelative();
  void testResolveFirstUnknown();
  void testResolveFirstUnknownRelative();
  void testResolveComponentOK();
  void testResolveComponentEmptyHostname();
  void testResolveComponentUnknownHostname();
  void testResolveComponentEmptyContainerName();
  void testResolveComponentUnknownContainerName();
  void testResolveComponentEmptyComponentName();
  void testResolveComponentUnknownComponentName();
  void testResolveComponentFalseNbproc();
  void testContainerName();
  void testContainerNameParams();
  void testBuildContainerNameForNS();
  void testBuildContainerNameForNSParams();
  void testFind();
  void testCreateDirectory();
  void testChangeDirectory();
  void testCurrentDirectory();
  void testList();
  void testListDirectory();
  void testListDirectoryRecurs();
  void testListSubdirs();
  void testDestroyName();
  void testDestroyDirectory();
  void testDestroyFullDirectory();
  void testGetIorAddr();

 private:
  std::string _getTraceFileName();

protected:
  void _destroyDirectoryRecurs(std::string path);

  CORBA::ORB_var _orb;
  SALOME_NamingService _NS;

  PortableServer::POA_var _root_poa;
  PortableServer::POAManager_var _pman;
  PortableServer::ObjectId_var _myFactoryId;
  NSTEST_aFactory_i * _myFactory;
  CORBA::Object_var _factoryRef;
};

#endif
