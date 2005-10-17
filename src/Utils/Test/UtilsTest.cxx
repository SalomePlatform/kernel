
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
