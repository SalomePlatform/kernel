//  SALOME Utils : general SALOME's definitions and tools
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  File   : utilities.h
//  Author : Antoine YESSAYAN, Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

/* ---  Definition macros file to print informations if _DEBUG_ is defined --- */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <sstream>
#include <cstdlib>


#include "LocalTraceBufferPool.hxx"

/*!
 * For each message to put in the trace, a specific ostingstream object is
 * created and destroyed automatically at the end of the message macro.
 * The insert function of LocalTraceBufferPool class gets a buffer in a
 * buffer pool (unique with the help of mutexes and semaphores) and copy the
 * message in the buffer.
 * This buffer is read later by a specific thread in charge of trace print.
 * Order of trace entries is globally respected. Nevertheless, if there are
 * several threads waiting for a free buffer to trace, the order of
 * thread waken up is not garanteed (no fifo or priority rules in Linux Kernel)
 */

#define MESS_INIT(deb) std::ostringstream os; os<<deb
#define MESS_BEGIN(deb) MESS_INIT(deb)<<__FILE__ <<" ["<<__LINE__<<"] : "
#define MESS_END std::endl; LocalTraceBufferPool::instance()->insert(NORMAL_MESS, os.str().c_str());
#define MESS_ABORT std::endl; LocalTraceBufferPool::instance()->insert(ABORT_MESS, os.str().c_str());

// --- Some macros are always defined (without _DEBUG_): for use with release version

#define INFOS(msg) {MESS_BEGIN("- Trace ") << msg << MESS_END}
#define PYSCRIPT(msg) {MESS_INIT("---PYSCRIPT--- ") << msg << MESS_END}
#define INTERRUPTION(msg) {MESS_BEGIN("- INTERRUPTION: ")<< msg << MESS_ABORT}
#define IMMEDIATE_ABORT(code) {cout <<std::flush; \
                               cerr << "- ABORT " << __FILE__ << " [" <<__LINE__<< "] : " << flush; \
                               cerr << "ABORT return code= "<< code << endl; \
                               std::exit(code);}

/* --- To print date and time of compilation of current source --- */

#if defined ( __GNUC__ )
#define COMPILER		"g++" 
#elif defined ( __sun )
#define COMPILER		"CC" 
#elif defined ( __KCC )
#define COMPILER		"KCC" 
#elif defined ( __PGI )
#define COMPILER		"pgCC" 
#elif defined ( __alpha )
#define COMPILER		"cxx" 
#else
#define COMPILER		"undefined" 
#endif

#ifdef INFOS_COMPILATION
#error INFOS_COMPILATION already defined
#endif

#define INFOS_COMPILATION { MESS_BEGIN("COMPILED with ") << COMPILER \
				       << ", " << __DATE__ \
				       << " at " << __TIME__ << MESS_END }
#ifdef _DEBUG_

// --- the following MACROS are useful at debug time

#define MESSAGE(msg) {MESS_BEGIN("- Trace ") << msg << MESS_END}
#define SCRUTE(var)  {MESS_BEGIN("- Trace ") << #var << "=" << var <<MESS_END}

#define REPERE ("------- ")
#define BEGIN_OF(msg) {MESS_BEGIN(REPERE) << "Begin of: "      << msg << MESS_END} 
#define END_OF(msg)   {MESS_BEGIN(REPERE) << "Normal end of: " << msg << MESS_END} 

#ifndef ASSERT
#define ASSERT(condition) \
        if (!(condition)){INTERRUPTION("CONDITION "<<#condition<<" NOT VERIFIED")}
#endif /* ASSERT */


#else /* ifdef _DEBUG_*/

#define MESSAGE(msg) {}
#define SCRUTE(var) {}
#define REPERE
#define BEGIN_OF(msg) {}
#define END_OF(msg) {}

#ifndef ASSERT
#define ASSERT(condition) {}
#endif /* ASSERT */

#endif /* ifdef _DEBUG_*/

#endif /* ifndef UTILITIES_H */
