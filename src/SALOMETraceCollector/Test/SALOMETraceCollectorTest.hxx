
#ifndef _SALOMETRACECOLLECTORTTEST_HXX_
#define _SALOMETRACECOLLECTORTTEST_HXX_

#include <cppunit/extensions/HelperMacros.h>

class SALOMETraceCollectorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( SALOMETraceCollectorTest );
  CPPUNIT_TEST( testLoadBufferPoolCORBA );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void testLoadBufferPoolCORBA();
};

#endif
