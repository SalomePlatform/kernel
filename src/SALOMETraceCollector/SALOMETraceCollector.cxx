//  Copyright (C) 2004  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : LocalTraceCollector.cxx
//  Author : Paul RASCLE (EDF)
//  Module : KERNEL
//  $Header$

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <CORBA.h>

using namespace std;

#include "SALOMETraceCollector.hxx"
#include "TraceCollector_WaitForServerReadiness.hxx"
#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(Logger)

// Class attributes initialisation, for class method SALOMETraceCollector::run

CORBA::ORB_ptr SALOMETraceCollector::_orb = 0;

// ============================================================================
/*!
 *  This class is for use with CORBA, inside SALOME.
 *  Type of trace (and corresponding class) is choosen in LocalTraceBufferPool.
 *
 *  Guarantees a unique object instance of the class (singleton thread safe)
 *  a separate thread for loop to print traces is launched.
 */
// ============================================================================

BaseTraceCollector* SALOMETraceCollector::instance()
{
  if (_singleton == 0) // no need of lock when singleton already exists
    {
      int ret;
      ret = pthread_mutex_lock(&_singletonMutex); // acquire lock to be alone
      if (_singleton == 0)                     // another thread may have got
	{                                      // the lock after the first test
	  BaseTraceCollector* myInstance = new SALOMETraceCollector();
	  int argc=0;
	  char *_argv=0;
	  char ** argv = &_argv;
	  _orb = CORBA::ORB_init (argc, argv);

	  sem_init(&_sem,0,0); // to wait until run thread is initialized
	  pthread_t traceThread;
	  int bid;
	  int re2 = pthread_create(&traceThread, NULL,
				   SALOMETraceCollector::run, (void *)bid);
	  sem_wait(&_sem);
	  _singleton = myInstance; // _singleton known only when init done
	}
      ret = pthread_mutex_unlock(&_singletonMutex); // release lock
    }
  return _singleton;
}

// ============================================================================
/*!
 *  In a separate thread, loop to print traces.
 *  Mutex garantees intialisation on instance method is done and only one run
 *  allowed (double check ...)
 *  Loop until there is no more buffer to print,
 *  and no ask for end from destructor.
 *  Get a buffer. If type = ABORT then exit application with message.
 */
// ============================================================================

void* SALOMETraceCollector::run(void *bid)
{
  _threadId = new pthread_t;
  *_threadId = pthread_self();
  sem_post(&_sem); // unlock instance

  LocalTraceBufferPool* myTraceBuffer = LocalTraceBufferPool::instance();
  LocalTrace_TraceInfo myTrace;

  SALOME_Logger::Logger_var m_pInterfaceLogger;
  CORBA::Object_var obj;

  obj = TraceCollector_WaitForServerReadiness(_orb,"Logger");
  if (!CORBA::is_nil(obj))
    m_pInterfaceLogger = SALOME_Logger::Logger::_narrow(obj);
  if (CORBA::is_nil(m_pInterfaceLogger))
    {
      cerr << "Logger server not found ! Abort" << endl;
      cerr << flush ; 
      exit(1);
    } 
  else
    {
      CORBA::String_var LogMsg =
	CORBA::string_dup("\n---Init logger trace---\n");
      m_pInterfaceLogger->putMessage(LogMsg);
      DEVTRACE("Logger server found");
    }

  // --- Loop until there is no more buffer to print,
  //     and no ask for end from destructor.

  while ((!_threadToClose) || myTraceBuffer->toCollect() )
    {
      if (_threadToClose)
	{
	  DEVTRACE("SALOMETraceCollector _threadToClose");
	  //break;
	}

      int fullBuf = myTraceBuffer->retrieve(myTrace);
      if (!CORBA::is_nil(_orb))
	{
	  if (myTrace.traceType == ABORT_MESS)
	    {
	      stringstream abortMessage("");
#ifndef WNT
	      abortMessage << "INTERRUPTION from thread "
			   << myTrace.threadId << " : " << myTrace.trace;
#else
	      abortMessage << "INTERRUPTION from thread "
			   << (void*)&myTrace.threadId 
			   << " : " << myTrace.trace;
#endif
	      CORBA::String_var LogMsg =
		CORBA::string_dup(abortMessage.str().c_str());
	      m_pInterfaceLogger->putMessage(LogMsg);
	      exit(1);
	    }
	  else
	    {
	      stringstream aMessage("");
#ifndef WNT
	      aMessage << "th. " << myTrace.threadId
#else
		aMessage << "th. " << (void*)&myTrace.threadId
#endif
		       << " " << myTrace.trace;
	      CORBA::String_var LogMsg =
		CORBA::string_dup(aMessage.str().c_str());
	      m_pInterfaceLogger->putMessage(LogMsg);
	    }
	}
    }
  pthread_exit(NULL);
  return NULL;
}

// ============================================================================
/*!
 *  Destructor: wait until printing thread ends (SALOMETraceCollector::run)
 */
// ============================================================================

SALOMETraceCollector:: ~SALOMETraceCollector()
{
  int ret;
  ret = pthread_mutex_lock(&_singletonMutex); // acquire lock to be alone
  if (_singleton)
    {
      DEVTRACE("SALOMETraceCollector:: ~SALOMETraceCollector()");
      LocalTraceBufferPool* myTraceBuffer = LocalTraceBufferPool::instance();
      _threadToClose = 1;
      myTraceBuffer->insert(NORMAL_MESS,"end of trace\n"); // to wake up thread
      if (_threadId)
	{
	  int ret = pthread_join(*_threadId, NULL);
	  if (ret) cerr << "error close SALOMETraceCollector : "<< ret << endl;
	  else DEVTRACE("SALOMETraceCollector destruction OK");
	  _threadId = 0;
	  _threadToClose = 0;
	}
      _singleton = 0;
    }
  ret = pthread_mutex_unlock(&_singletonMutex); // release lock
}

// ============================================================================
/*!
 * Constructor: no need of LocalTraceBufferPool object initialization here,
 * thread safe singleton used in LocalTraceBufferPool::instance()
 */
// ============================================================================

SALOMETraceCollector::SALOMETraceCollector()
{
  _threadId=0;
  _threadToClose = 0;
}

// ============================================================================
/*!
 * 
 */
// ============================================================================

extern "C"
{
  BaseTraceCollector *SingletonInstance(void)
  {
    BaseTraceCollector *instance = SALOMETraceCollector::instance();
    return instance;
  }
}
