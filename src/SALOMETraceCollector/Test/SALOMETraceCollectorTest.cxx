
#include "SALOMETraceCollectorTest.hxx"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "LocalTraceBufferPool.hxx"
#include "utilities.h"

using namespace std;

// ============================================================================
/*!
 *
 */
// ============================================================================

void 
SALOMETraceCollectorTest::setUp()
{
}

// ============================================================================
/*!
 *
 */
// ============================================================================

void 
SALOMETraceCollectorTest::tearDown()
{
}

#define NUM_THREADS  20
#define NUM_MESSAGES 20
void *PrintHello(void *threadid);

// ============================================================================
/*!
 *  open a trace on a CORBA, multithread writing on file, close
 */
// ============================================================================

void 
SALOMETraceCollectorTest::testLoadBufferPoolCORBA()
{
  string s = "with_logger";
  CPPUNIT_ASSERT(! setenv("SALOME_trace",s.c_str(),1)); // 1: overwrite

  // --- NUM_THREADS thread creation for trace generation.

  pthread_t threads[NUM_THREADS];
  int rc, t;
  for(t=0;t<NUM_THREADS;t++)
    {
      MESSAGE("Creating thread " << t);
      rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t) ;
      CPPUNIT_ASSERT( !rc);
    }

  // --- wait for end of each thread producing trace.

  for(t=0;t<NUM_THREADS;t++)
    {
      int ret = pthread_join(threads[t], NULL);
      MESSAGE("--------------------- end of PrintHello thread " << t);
    }
  MESSAGE(" ---- end of PrintHello threads ---- ");

  LocalTraceBufferPool* bp1 = LocalTraceBufferPool::instance();
  CPPUNIT_ASSERT(bp1);
  bp1->deleteInstance(bp1);
}

// ============================================================================
/*!
 * NUM_THREAD are created with function PrintHello,
 * which produces NUM_MESSAGES traces.
 */
// ============================================================================

void *PrintHello(void *threadid)
{
  int id_thread = (int)threadid;
  for (int i=0; i<NUM_MESSAGES;i++)
    MESSAGE("Hello World! This is a trace test : " << id_thread 
	    << " - iter " << i);
  pthread_exit(NULL);
}
