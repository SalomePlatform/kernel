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

using namespace std;

#include "LocalTraceCollector.hxx"

// Class attributes initialisation, for class method LocalTraceCollector::run

LocalTraceCollector* LocalTraceCollector::_singleton = 0;
pthread_mutex_t LocalTraceCollector::_singletonMutex;
int LocalTraceCollector::_threadToClose = 0;
pthread_t* LocalTraceCollector::_threadId = 0; // used to control single run
int LocalTraceCollector::_toFile = 0;
std::string LocalTraceCollector::_fileName = "";

// ============================================================================
/*!
 *  This class is for use without CORBA, outside SALOME.
 *  SALOME uses SALOMETraceCollector, to allow trace collection via CORBA.
 *
 *  guarantees a unique object instance of the class (singleton thread safe)
 *  a separate thread for loop to print traces is launched.
 *  \param typeTrace 0=standard out, 1=file(/tmp/tracetest.log)
 *  If typeTrace=0, checks environment for "SALOME_trace". Test values in
 *  the following order:
 *  - "local"  standard out
 *  - anything else is kept as a file name
 */
// ============================================================================

LocalTraceCollector* LocalTraceCollector::instance(int typeTrace)
{
  if (_singleton == 0) // no need of lock when singleton already exists
    {
      int ret;
      ret = pthread_mutex_lock(&_singletonMutex); // acquire lock to be alone
      if (_singleton == 0)                     // another thread may have got
	{                                      // the lock after the first test
	  _singleton = new LocalTraceCollector();

	  _fileName = "/tmp/tracetest.log";
	  _toFile=0;

	  if (typeTrace)       // caller sets a value different from default=0
	    _toFile = typeTrace; 
	  else                 // check environment
	    {
	      char* traceKind = getenv("SALOME_trace");
	      //cout<<"SALOME_trace="<<traceKind<<endl;
	      if (traceKind)
		{
		  if (strcmp(traceKind,"local")==0) _toFile=0;

		  else
		    {
		      _toFile=1;
		      _fileName = traceKind;
		    }	    
		}
	    }
	  //cout <<"_toFile: "<<_toFile<<" _fileName: "<<_fileName<<endl;

	  pthread_t traceThread;
	  int bid;
	  int re2 = pthread_create(&traceThread, NULL,
				   LocalTraceCollector::run, (void *)bid);
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

void* LocalTraceCollector::run(void *bid)
{
  int isOKtoRun = 0;
  int ret = pthread_mutex_lock(&_singletonMutex); // acquire lock to be alone
  if (! _threadId)  // only one run
    {
      isOKtoRun = 1;
      if(_threadId == 0) {
	_threadId = new pthread_t;
      }
      *_threadId = pthread_self();
    }
  else cout << "----- Comment est-ce possible de passer la ? -------" <<endl;
  ret = pthread_mutex_unlock(&_singletonMutex); // release lock

  if (isOKtoRun)
    { 
      if(_threadId == 0) {
	_threadId = new pthread_t;
      }

      *_threadId = pthread_self();
      LocalTraceBufferPool* myTraceBuffer = LocalTraceBufferPool::instance();
      LocalTrace_TraceInfo myTrace;

      // if trace in file requested, opens a file with append mode
      // so, several processes can share the same file


      ofstream traceFile;

      switch (_toFile)
	{
	case 1 :  // --- trace to file
	  {
	    const char *fileName = _fileName.c_str();
	    traceFile.open(fileName, ios::out | ios::app);
	    if (!traceFile)
	      {
		cerr << "impossible to open trace file "<< fileName << endl;
		exit (1);
	      }
	  }
	  break;

	case 0 : ; // --- trace to standard output
	default :  // --- on standard output, too
	  break;
	}

      // Loop until there is no more buffer to print,
      // and no ask for end from destructor.

      while ((!_threadToClose) || myTraceBuffer->toCollect() )
	{
	  int fullBuf = myTraceBuffer->retrieve(myTrace);
	  if (myTrace.traceType == ABORT_MESS)
	    {
	      switch (_toFile)
		{
		case 1 :  // --- trace to file
#ifndef WNT
		  traceFile << "INTERRUPTION from thread " << myTrace.threadId
        << " : " <<  myTrace.trace;
#else
		  traceFile << "INTERRUPTION from thread " << (void*)(&myTrace.threadId)
        << " : " <<  myTrace.trace;
#endif
		  traceFile.close();
		  // no break here !
		case 0 :  // --- trace to standard output
		default : // --- on standard output, too
		  cout << flush ;
#ifndef WNT
		  cerr << "INTERRUPTION from thread " << myTrace.threadId
		       << " : " <<  myTrace.trace;
#else
		  cerr << "INTERRUPTION from thread " << (void*)(&myTrace.threadId)
		       << " : " <<  myTrace.trace;
#endif
      cerr << flush ; 
		  exit(1);     
		  break;
		}
	    }
	  else
	    {
	      switch (_toFile)
		{
		case 1 :  // --- trace to file
#ifndef WNT
      traceFile << "th. " << myTrace.threadId
			    << " " << myTrace.trace;
#else
      traceFile << "th. " << (void*)(&myTrace.threadId)
			    << " " << myTrace.trace;
#endif
		  break;
		case 0 :  // --- trace to standard output
		default : // --- on standard output, too
#ifndef WNT
		  cout << "th. " << myTrace.threadId << " " << myTrace.trace;
#else
		  cout << "th. " << (void*)(&myTrace.threadId) << " " << myTrace.trace;
#endif
		  break;
		}
	    }
	}

      if (_toFile==1) traceFile.close();
    }
  pthread_exit(NULL);
  return NULL;
}

// ============================================================================
/*!
 *  Destructor: wait until printing thread ends (LocalTraceCollector::run)
 */
// ============================================================================

LocalTraceCollector:: ~LocalTraceCollector()
{
  LocalTraceBufferPool* myTraceBuffer = LocalTraceBufferPool::instance();
  _threadToClose = 1;
  myTraceBuffer->insert(NORMAL_MESS,"end of trace "); //needed to wake up thread
  if (_threadId)
    {
      int ret = pthread_join(*_threadId, NULL);
      if (ret) cout << "error close LocalTraceCollector : "<< ret << endl;
      else cout << "LocalTraceCollector destruction OK" << endl;
    }
  delete myTraceBuffer;
}

// ============================================================================
/*!
 * Constructor: no need of LocalTraceBufferPool object initialization here,
 * thread safe singleton used in LocalTraceBufferPool::instance()
 */
// ============================================================================

LocalTraceCollector::LocalTraceCollector()
{
  _threadId=0;
}


