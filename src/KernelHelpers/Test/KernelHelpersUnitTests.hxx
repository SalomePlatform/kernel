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

#ifndef _SalomeAppTEST_HXX_
#define _SalomeAppTEST_HXX_

#include <cppunit/extensions/HelperMacros.h>

#ifdef WIN32
# if defined KERNELHELPERSTEST_EXPORTS || defined SalomeKernelHelpersTest_EXPORTS
#  define KERNELHELPERSTEST_EXPORT __declspec( dllexport )
# else
#  define KERNELHELPERSTEST_EXPORT __declspec( dllimport )
# endif
#else
# define KERNELHELPERSTEST_EXPORT
#endif

class KERNELHELPERSTEST_EXPORT KernelHelpersUnitTests : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( KernelHelpersUnitTests );
  CPPUNIT_TEST( TEST_corba );
  CPPUNIT_TEST( TEST_getLifeCycleCORBA );
  CPPUNIT_TEST( TEST_getStudyManager );
  CPPUNIT_TEST( TEST_getSalomeLauncher );
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp();
  void tearDown();

  void TEST_corba();
  void TEST_getLifeCycleCORBA();
  void TEST_getStudyManager();
  void TEST_getSalomeLauncher();
};

#endif
