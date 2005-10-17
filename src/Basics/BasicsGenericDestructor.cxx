//  SALOME Basics : general SALOME definitions and tools (C++ part - no CORBA)
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
//  File   : BasicGenericDestructor.cxx
//  Author : Antoine YESSAYAN, Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

#include <iostream>
#include <list>
#include <cstdlib>

#include "BasicsGenericDestructor.hxx"

using namespace std;

void HouseKeeping();

std::list<PROTECTED_DELETE*> PROTECTED_DELETE::_objList;
pthread_mutex_t PROTECTED_DELETE::_listMutex;

std::list<GENERIC_DESTRUCTOR*> *GENERIC_DESTRUCTOR::Destructors = 0;
static bool atExitSingletonDone = false ;

// ============================================================================
/*! 
 *  deleteInstance deletes only once the object. Only object present on the
 *  static list of PROTECTED_DELETE* are deleted, then removed of the list.
 *  The operation is protected by a mutex.
 */
// ============================================================================

void PROTECTED_DELETE::deleteInstance(PROTECTED_DELETE *anObject)
  {
    if (std::find(_objList.begin(), _objList.end(),anObject) == _objList.end())
      return;
    else
      {
	int ret;
	ret = pthread_mutex_lock(&_listMutex); // acquire lock, an check again
	if (std::find(_objList.begin(), _objList.end(), anObject)
	    != _objList.end())
	  {
	    DEVTRACE("PROTECTED_DELETE::deleteInstance1 " << anObject);
	    delete anObject;
	    DEVTRACE("PROTECTED_DELETE::deleteInstance2 " << &_objList);
	    _objList.remove(anObject);
	  }
	ret = pthread_mutex_unlock(&_listMutex); // release lock
      }
  }

// ============================================================================
/*! 
 * To allow a further destruction of a PRTECTED_DELETE object, it must be added
 * to the static list of PROTECTED_DELETE*
 */
// ============================================================================

void PROTECTED_DELETE::addObj(PROTECTED_DELETE *anObject)
{
  DEVTRACE("PROTECTED_DELETE::addObj " << anObject);
  _objList.push_back(anObject);
}

// ============================================================================
/*! 
 *  Herited classes have there own destructors
 */
// ============================================================================

PROTECTED_DELETE::~PROTECTED_DELETE()
{
  DEVTRACE("PROTECTED_DELETE::~PROTECTED_DELETE()");
}

// ============================================================================
/*! 
 * To execute only once GENERIC_DESTRUCTOR::HouseKeeping et the end of process,
 * a dedicated object is created, as a singleton: atExitSingleton.
 * When the singleton is created, the HouseKeeping() function is registered in
 * atExit().
 * Destructors is a list created on heap, and deleted by HouseKeeping(), with
 * the list content.
 */
// ============================================================================

class atExitSingleton
{
public:
  atExitSingleton(bool Make_ATEXIT)
  {
    if (Make_ATEXIT && !atExitSingletonDone)
      {
	DEVTRACE("atExitSingleton(" << Make_ATEXIT << ")");
	assert(GENERIC_DESTRUCTOR::Destructors == 0);
	GENERIC_DESTRUCTOR::Destructors = new std::list<GENERIC_DESTRUCTOR*>;
	int cr = atexit(HouseKeeping);
	assert(cr == 0);
	atExitSingletonDone = true;
      }
  }

  ~atExitSingleton()
  {
    DEVTRACE("atExitSingleton::~atExitSingleton()");
  }
};

//! static singleton for atExitSingleton class

static atExitSingleton HouseKeeper = atExitSingleton(false);

// ============================================================================
/*! 
 *  Executes all objects of type DESTRUCTOR_OF in the Destructors list. 
 *  Deletes  all objects of type DESTRUCTOR_OF in the Destructors list.
 *  Deletes the list.
 */
// ============================================================================

void HouseKeeping( void )
{
  DEVTRACE("HouseKeeping()");
  assert(GENERIC_DESTRUCTOR::Destructors);
  if(GENERIC_DESTRUCTOR::Destructors->size())
    {
      std::list<GENERIC_DESTRUCTOR*>::iterator it =
	GENERIC_DESTRUCTOR::Destructors->end();

      do
	{
	  it-- ;
	  GENERIC_DESTRUCTOR* ptr = *it ;
	  DEVTRACE("HouseKeeping() " << typeid(ptr).name());
	  (*ptr)();
	  delete ptr ;
	}
      while (it !=  GENERIC_DESTRUCTOR::Destructors->begin()) ;

      DEVTRACE("HouseKeeping() end list ");
      GENERIC_DESTRUCTOR::Destructors->clear() ;
      assert(GENERIC_DESTRUCTOR::Destructors->size() == 0);
      assert(GENERIC_DESTRUCTOR::Destructors->empty());
      DEVTRACE("HouseKeeping()after clear ");
    }

  delete GENERIC_DESTRUCTOR::Destructors;
  GENERIC_DESTRUCTOR::Destructors = 0;
  DEVTRACE("HouseKeeping() very end ");
  return ;
}

// ============================================================================
/*!
 * Adds a destruction object to the list of actions to be performed at the end
 * of the process
 */
// ============================================================================

const int GENERIC_DESTRUCTOR::Add(GENERIC_DESTRUCTOR &anObject)
{
  DEVTRACE("GENERIC_DESTRUCTOR::Add("<<typeid(anObject).name()<<") "
	   << &anObject);
  if (!atExitSingletonDone)
    {
      HouseKeeper = atExitSingleton(true);
    }
  assert(Destructors);
  Destructors->push_back(&anObject);
  return Destructors->size();
}
