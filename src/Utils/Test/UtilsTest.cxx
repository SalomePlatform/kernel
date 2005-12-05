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

#include "UtilsTest.hxx"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "Utils_SALOME_Exception.hxx"
#include "utilities.h"

using namespace std;

#define TRACEFILE "/tmp/traceUnitTest.log"

// ============================================================================
/*!
 * Set Trace mecanism
 * - delete preexisting trace classes if any
 * - set trace on file
 */
// ============================================================================

void 
UtilsTest::setUp()
{
  LocalTraceBufferPool* bp1 = LocalTraceBufferPool::instance();
  CPPUNIT_ASSERT(bp1);
  bp1->deleteInstance(bp1);

  // --- trace on file
  char *theFileName = TRACEFILE;

  string s = "file:";
  s += theFileName;
  CPPUNIT_ASSERT(! setenv("SALOME_trace",s.c_str(),1)); // 1: overwrite

  ofstream traceFile;
  traceFile.open(theFileName, ios::out | ios::app);
  CPPUNIT_ASSERT(traceFile); // file created empty, then closed
  traceFile.close();

  bp1 = LocalTraceBufferPool::instance();
  CPPUNIT_ASSERT(bp1);
}

// ============================================================================
/*!
 *  - delete trace classes
 */
// ============================================================================

void 
UtilsTest::tearDown()
{
  LocalTraceBufferPool* bp1 = LocalTraceBufferPool::instance();
  CPPUNIT_ASSERT(bp1);
  bp1->deleteInstance(bp1);
}

int genExcept()
{
  throw SALOME_Exception("a message");
};

// ============================================================================
/*!
 * Check basic SALOME_exception mecanism
 */
// ============================================================================

void
UtilsTest::testSALOME_ExceptionThrow()
{
  CPPUNIT_ASSERT_THROW(genExcept(), SALOME_Exception);
}

// ============================================================================
/*!
 * Check message on catch
 */
// ============================================================================

void
UtilsTest::testSALOME_ExceptionMessage()
{
#define EXAMPLE_EXCEPTION_MESSAGE "something for the end user"
  try
    {
      throw SALOME_Exception(EXAMPLE_EXCEPTION_MESSAGE);
    }
  catch (const SALOME_Exception &ex)
    {
      string expectedMessage = EXAMPLE_EXCEPTION_MESSAGE;
      string actualMessage = ex.what();
      CPPUNIT_ASSERT(actualMessage.find(expectedMessage) != string::npos);
    }
}
