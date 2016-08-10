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

#include "SALOMELocalTraceTest.hxx"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include "LocalTraceBufferPool.hxx"
#include "utilities.h"
#include "Basics_Utils.hxx"
#include "Basics_DirUtils.hxx"

#ifdef WIN32
#define setenv Kernel_Utils::setenv
#endif

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

std::string
SALOMELocalTraceTest::_getTraceFileName()
{
  std::string dir = Kernel_Utils::GetTmpDir();
  return dir + "traceUnitTest-SALOMELocalTraceTest.log";
}

// ============================================================================
/*!
 *  Open and close a trace on a file, test singleton
 */
// ============================================================================

void
SALOMELocalTraceTest::testSingletonBufferPool()
{
  // --- trace on file
  std::string theFileName = _getTraceFileName();

  std::string s = "file:";
  s += theFileName;
  CPPUNIT_ASSERT(! setenv("SALOME_trace",s.c_str(),1)); // 1: overwrite

  std::ofstream traceFile;
  traceFile.open(theFileName.c_str(), std::ios::out | std::ios::app);
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
  std::string s = "local";
  CPPUNIT_ASSERT(! setenv("SALOME_trace",s.c_str(),1)); // 1: overwrite

  // --- NUM_THREADS thread creation for trace generation.
  pthread_t threads[NUM_THREADS];
  int rc, t;
  for(t=0;t<NUM_THREADS;t++)
    {
      MESSAGE("Creating thread " << t);
      rc = pthread_create(&threads[t], NULL, PrintHello, &t) ;
      CPPUNIT_ASSERT( !rc);
    }

  // --- wait for end of each thread producing trace.

  for(t=0;t<NUM_THREADS;t++)
    {
      pthread_join(threads[t], NULL);
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
  std::string theFileName = _getTraceFileName();

  std::string s = "file:";
  s += theFileName;
  //s = "local";
  CPPUNIT_ASSERT(! setenv("SALOME_trace",s.c_str(),1)); // 1: overwrite

  std::ofstream traceFile;
  traceFile.open(theFileName.c_str(), std::ios::out | std::ios::trunc);
  CPPUNIT_ASSERT(traceFile); // file created empty, then closed
  traceFile.close();

  // --- NUM_THREADS thread creation for trace generation.

  pthread_t threads[NUM_THREADS];
  int rc, t;
  for(t=0;t<NUM_THREADS;t++)
    {
      MESSAGE("Creating thread " << t);
      rc = pthread_create(&threads[t], NULL, PrintHello, &t) ;
      CPPUNIT_ASSERT( !rc);
    }

  // --- wait for end of each thread producing trace.

  for(t=0;t<NUM_THREADS;t++)
    {
      pthread_join(threads[t], NULL);
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
#if defined(_DEBUG_) || defined(_DEBUG)
  long id_thread = (long)threadid;
  for (int i=0; i<NUM_MESSAGES;i++)
  MESSAGE("Hello World! This is a trace test : " << id_thread
        << " - iter " << i);
#endif
  pthread_exit(NULL);
#ifdef WIN32
  return NULL;
#endif
}
