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

#include <string>
#include <iostream>
#include "SALOME_Log.hxx"

/* ---  INFOS is always defined (without _DEBUG_): to be used for warnings, with release version --- */

#define INFOS(msg)    {SLog.putMessage(SLog<<__FILE__<<" ["<<__LINE__<<"] : "<<msg<<std::endl<<std::ends);}
#define PYSCRIPT(msg) {SLog.putMessage(SLog<<"---PYSCRIPT--- "<<msg<<std::endl<<std::ends);}

/* --- To print date and time of compilation of current source --- */

#if defined ( __GNUC__ )
#define COMPILER		"g++" 
#elif defined ( __sun )
#define COMPILER		"CC" 
#elif defined ( __KCC )
#define COMPILER		"KCC" 
#elif defined ( __PGI )
#define COMPILER		"pgCC" 
#else
#define COMPILER		"undefined" 
#endif

#ifdef INFOS_COMPILATION
#error INFOS_COMPILATION already defined
#endif

#define INFOS_COMPILATION { \
			   SLog.putMessage(\
					   SLog<<__FILE__<<" ["<< __LINE__<<"] : "\
					   << "COMPILED with " << COMPILER \
					   << ", " << __DATE__ \
					   << " at " << __TIME__ <<std::endl<<std::ends); }

#ifdef _DEBUG_

/* --- the following MACROS are useful at debug time --- */

#define MYTRACE SLog<<"- Trace "<<__FILE__<<" ["<<__LINE__<<"] : " 

#define MESSAGE(msg) {SLog.putMessage(MYTRACE<<msg<<std::endl<<std::ends);}
#define SCRUTE(var)  {SLog.putMessage(MYTRACE<<#var<<"="<<var<<std::endl<<std::ends);}

#define BEGIN_OF(msg) {SLog.putMessage(MYTRACE<<"Begin of: "<<msg<<"\n\n"<<std::ends);} 
#define END_OF(msg)   {SLog.putMessage(SLog<<"\n"<<std::ends); \
                       SLog.putMessage(MYTRACE<<"Normal end of: "<<msg<<"\n"<<std::ends);}

#define HERE {std::cout<<std::flush ;std::cerr<<"- Trace "<<__FILE__<<" ["<<__LINE__<<"] : "<<std::flush;}

#define INTERRUPTION(code) {HERE; \
                            cerr<<"INTERRUPTION return code= "<<code<<std::endl; \
                            std::exit(code);}

#ifndef ASSERT
#define ASSERT(condition) \
        if(!(condition)){ \
          HERE; \
          std::cerr<<"CONDITION "<<#condition<<" NOT VERIFIED"<<std::endl; \
          INTERRUPTION(1); \
        }

#endif /* ASSERT */


#else /* ifdef _DEBUG_*/

#define HERE 
#define SCRUTE(var) {}
#define MESSAGE(msg) {}
#define BEGIN_OF(msg) {}
#define END_OF(msg) {}

#define INTERRUPTION(code) {}

#ifndef ASSERT
#define ASSERT(condition) {}
#endif /* ASSERT */


#endif /* ifdef _DEBUG_*/

#endif /* ifndef UTILITIES_H */
