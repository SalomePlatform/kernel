
#ifndef _UTILSTEST_HXX_
#define _UTILSTEST_HXX_

#include <cppunit/extensions/HelperMacros.h>

class UtilsTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( UtilsTest );
  CPPUNIT_TEST( testSALOME_ExceptionThrow );
  CPPUNIT_TEST( testSALOME_ExceptionMessage );
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp();
  void tearDown();

  void testSALOME_ExceptionThrow();
  void testSALOME_ExceptionMessage();
};

#endif
