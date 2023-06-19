// Copyright (C) 2007-2023  CEA, EDF, OPEN CASCADE
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

// --- include all Unit Test from basics until the present directory
//
#include "SALOMELocalTraceTest.hxx"

#ifdef WITH_CORBA
#include "SALOMETraceCollectorTest.hxx"
#include "UtilsTest.hxx"
#include "NamingServiceTest.hxx"
#include "LifeCycleCORBATest.hxx"
#include "SALOMEDSImplTest.hxx"
#include "SALOMEDSTest.hxx"
#endif

// --- Registers the fixture into the 'registry'

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SALOMELocalTraceTest,"0" );
#ifdef WITH_CORBA
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SALOMETraceCollectorTest,"1" );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( UtilsTest,"2" );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( NamingServiceTest,"3" );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( LifeCycleCORBATest,"4" );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SALOMEDSImplTest,"5" );
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SALOMEDSTest, "6" );
#endif

// --- generic Main program from Basic/Test

#include "BasicMainTestOrdered.hxx"
