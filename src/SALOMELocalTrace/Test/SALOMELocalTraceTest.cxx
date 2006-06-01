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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

#include "SALOMELocalTraceTest.hxx"

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
SALOMELocalTraceTest::setUp()
{
}

// ============================================================================
/*!
 *
 */
// ============================================================================

void 
SALOMELocalTraceTest::tearDown()
{
}

#define TRACEFILE "/tmp/traceUnitTest.log"

// ============================================================================
/*!
 *  Open and close a trace on a file, test singleton
 */
// ============================================================================

void 
SALOMELocalTraceTest::testSingletonBufferPool()
{
  // --- trace on file
  char *theFileName = TRACEFILE;

  string s = "file:";
  s += theFileName;
  CPPUNIT_ASSERT(! setenv("SALOME_trace",s.c_str(),1)); // 1: overwrite

  ofstream traceFile;
  traceFile.open(theFileName, ios::out | ios::app);
  CPPUNIT_ASSERT(traceFile); // file created empty, then closed
  traceFile.close();

  LocalTraceBufferPool* bp1 = LocalTraceBufferPool::instance();
  CPPUNIT_ASSERT(bp1);
  LocalTraceBufferPool* bp2 = LocalTraceBufferPool::instance();
  CPPUNIT_ASSERT(bp1 == bp2);
  bp1->deleteInstance(bp1);
}



#define NUM_THREADS  2
#define NUM_MESSAGES 5
void *PrintHello(void *threadid);

// ============================================================================
/*!
 *  open a trace on console, multithread writing on file, close
 */
// ============================================================================

void 
SALOMELocalTraceTest::testLoadBufferPoolLocal()
{
  string s = "local";
  CPPUNIT_ASSERT(! setenv("SALOME_trace",s.c_str(),1)); // 1: overwrite

  // --- numThread thread creation for trace generation.
  int numThread = 2;
  pthread_t threads[numThread];
  int rc, t;
  for(t=0;t<numThread;t++)
    {
      MESSAGE("Creating thread " << t);
      rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t) ;
      CPPUNIT_ASSERT( !rc);
    }

  // --- wait for end of each thread producing trace.

  for(t=0;t<numThread;t++)
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
 *  open a trace on a file, multithread writing on file, close
 */
// ============================================================================

void 
SALOMELocalTraceTest::testLoadBufferPoolFile()
{
  char *theFileName = TRACEFILE;

  string s = "file:";
  s += theFileName;
  //s = "local";
  CPPUNIT_ASSERT(! setenv("SALOME_trace",s.c_str(),1)); // 1: overwrite

  ofstream traceFile;
  traceFile.open(theFileName, ios::out | ios::trunc);
  CPPUNIT_ASSERT(traceFile); // file created empty, then closed
  traceFile.close();

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
  long id_thread = (long)threadid;
  for (int i=0; i<NUM_MESSAGES;i++)
  MESSAGE("Hello World! This is a trace test : " << id_thread 
        << " - iter " << i);
  pthread_exit(NULL);
}
