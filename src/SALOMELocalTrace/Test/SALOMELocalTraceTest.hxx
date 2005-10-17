
#ifndef _SALOMELOCALTRACETEST_HXX_
#define _SALOMELOCALTRACETEST_HXX_

#include <cppunit/extensions/HelperMacros.h>

class SALOMELocalTraceTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( SALOMELocalTraceTest );
  CPPUNIT_TEST( testSingletonBufferPool );
  CPPUNIT_TEST( testLoadBufferPoolLocal );
  CPPUNIT_TEST( testLoadBufferPoolFile );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void testSingletonBufferPool();
  void testLoadBufferPoolLocal();
  void testLoadBufferPoolFile();
};

#endif
