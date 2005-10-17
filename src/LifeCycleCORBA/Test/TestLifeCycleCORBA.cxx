
// --- include all Unit Test from basics until the present directory

#include "SALOMELocalTraceTest.hxx"
#include "SALOMETraceCollectorTest.hxx"
#include "UtilsTest.hxx"
#include "LifeCycleCORBATest.hxx"

// --- Registers the fixture into the 'registry'

CPPUNIT_TEST_SUITE_REGISTRATION( SALOMELocalTraceTest );
CPPUNIT_TEST_SUITE_REGISTRATION( SALOMETraceCollectorTest );
CPPUNIT_TEST_SUITE_REGISTRATION( UtilsTest );
CPPUNIT_TEST_SUITE_REGISTRATION( LifeCycleCORBATest );

// --- generic Main program from Basic/Test

#include "BasicMainTest.hxx"
