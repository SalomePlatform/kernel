//  KERNEL SALOME_Event : Define event posting mechanism
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
//  File   : SALOME_Event.hxx
//  Author : Sergey ANIKIN
//  Module : KERNEL
//  $Header$


#ifndef SALOME_Event_HeaderFile
#define SALOME_Event_HeaderFile

#include <qevent.h>

#define SALOME_EVENT QEvent::Type( QEvent::User + 10000 )

class QSemaphore;

//===========================================================
/*!
 *  Class: SALOME_Event
 *  Description: 
 *  This class encapsulates data and functionality required for 
 *  posting component-specific events to perform arbitrary operations in main GUI thread. 
 *  SALOME_Event objects can be posted by any thread belonging to the GUI process.
 *
 *  It is necessary to derive a custom event class from SALOME_Event and 
 *  re-implement virtual Execute() method. This method should actually perform 
 *  the desirable operation. To pass all the required data to Execute() and store a return value,
 *  arbitrary data fields can be added to the custom event class. There is 
 *  no need to protect such fields with a mutex, for only one thread working with
 *  a SALOME_Event object is active at any moment.
 *
 *  It is possible to make the thread that creates SALOME_Event
 *  wait until the event is processed by the component GUI, SALOME_Event
 *  should be constructed with <wait> == TRUE in such a case.
 *
 *  SALOME_Event objects should be created on the heap. QAD_Desktop deletes
 *  these objects as soon as they have been processed.
 *
 *  Usage:
 *  - create SALOME_Event object on the heap with proper <type> and <wait> parameters. 
 *    Components can derive their own event class from SALOME_Event
 *    in order to pass custom data to the event handler.
 *  - call process() method to post the event. Between process() and release()
 *    it is possible to examine fields of your custom event object.
 *  - call release() method to wake up the desktop (you can also set <autoRelease>
 *    parameter to TRUE to automatically wake up desktop after process()
 * 
 *  processed() method is used by the desktop to signal that event processing 
 *  has been completed.
 *  
 *  Caveats: 
 *  1. Never create SALOME_Event with <wait> == TRUE in code that is 
 *     supposed to be called within main GUI thread, for this will result
 *     in GUI process deadlock.
 *  2. Always call release() method after process() if you use <wait> parameters as TRUE,
 *     otherwise processed() method will never return and main GUI thread will be blocked!
 *  3. Never use pointers to the event after it has been released (either by calling release()
 *     or automatically by process() if <autoRelease> == TRUE) to avoid application crashes!
 */
//===========================================================


class SALOME_Event
{
public:
  SALOME_Event();
  virtual ~SALOME_Event();

  // To do real work
  virtual void Execute() = 0;

  void process();
  void processed();

  void release();

private:
  bool        myWait;
  bool        myAutoRelease;
  QSemaphore* mySemaphore;
};


// Template classes for member function
//-------------------------------------
template<class TObject, typename TRes>
class TMemFunEvent: public SALOME_Event{
public:
  typedef TRes TResult;
  TResult myResult;
  typedef TResult (TObject::* TAction)();
  TMemFunEvent(TObject* theObject, TAction theAction, 
	       TResult theResult = TResult()):
    myObject(theObject),
    myAction(theAction),
    myResult(theResult)
  {}
  virtual void Execute(){
    myResult = (myObject->*myAction)();
  }
private:
  TObject* myObject;
  TAction myAction;
};


template<class TObject>
class TVoidMemFunEvent: public SALOME_Event{
public:
  typedef void (TObject::* TAction)();
  TVoidMemFunEvent(TObject* theObject, TAction theAction):
    myObject(theObject),
    myAction(theAction)
  {}
  virtual void Execute(){
    (myObject->*myAction)();
  }
private:
  TObject* myObject;
  TAction myAction;
};


// Template for member function with one argument
//-----------------------------------------------
template<class TObject, typename TRes, 
	 typename TArg, typename TStoreArg = TArg>
class TMemFun1ArgEvent: public SALOME_Event{
public:
  typedef TRes TResult;
  TResult myResult;
  typedef TResult (TObject::* TAction)(TArg);
  TMemFun1ArgEvent(TObject* theObject, TAction theAction, TArg theArg, 
		   TResult theResult = TResult()):
    myObject(theObject),
    myAction(theAction),
    myResult(theResult),
    myArg(theArg)
  {}
  virtual void Execute(){
    myResult = (myObject->*myAction)(myArg);
  }
private:
  TObject* myObject;
  TAction myAction;
  TStoreArg myArg;
};


template<class TObject, typename TArg, typename TStoreArg = TArg>
class TVoidMemFun1ArgEvent: public SALOME_Event{
public:
  typedef void (TObject::* TAction)(TArg);
  TVoidMemFun1ArgEvent(TObject* theObject, TAction theAction, TArg theArg):
    myObject(theObject),
    myAction(theAction),
    myArg(theArg)
  {}
  virtual void Execute(){
    (myObject->*myAction)(myArg);
  }
private:
  TObject* myObject;
  TAction myAction;
  TStoreArg myArg;
};


// Template for member function with one argument
//-----------------------------------------------
template<class TObject, typename TRes,
	 typename TArg, typename TArg1, 
	 typename TStoreArg = TArg, typename TStoreArg1 = TArg1>
class TMemFun2ArgEvent: public SALOME_Event{
public:
  typedef TRes TResult;
  TResult myResult;
  typedef TResult (TObject::* TAction)(TArg,TArg1);
  TMemFun2ArgEvent(TObject* theObject, TAction theAction, 
		   TArg theArg, TArg1 theArg1,
		   TResult theResult = TResult()):
    myObject(theObject),
    myAction(theAction),
    myResult(theResult),
    myArg(theArg),
    myArg1(theArg1)
  {}
  virtual void Execute(){
    myResult = (myObject->*myAction)(myArg,myArg1);
  }
private:
  TObject* myObject;
  TAction myAction;
  TStoreArg myArg;
  TStoreArg1 myArg1;
};


template<class TObject, typename TArg, typename TArg1, 
	 typename TStoreArg = TArg, typename TStoreArg1 = TArg1>
class TVoidMemFun2ArgEvent: public SALOME_Event{
public:
  typedef void (TObject::* TAction)(TArg,TArg1);
  TVoidMemFun2ArgEvent(TObject* theObject, TAction theAction, TArg theArg, TArg1 theArg1):
    myObject(theObject),
    myAction(theAction),
    myArg(theArg),
    myArg1(theArg1)
  {}
  virtual void Execute(){
    (myObject->*myAction)(myArg,myArg1);
  }
private:
  TObject* myObject;
  TAction myAction;
  TStoreArg myArg;
  TStoreArg1 myArg1;
};


// Template function for processing events with result returing
template<class TEvent> inline typename TEvent::TResult ProcessEvent(TEvent* theEvent){
  theEvent->process();
  typename TEvent::TResult aResult = theEvent->myResult;
  theEvent->release();
  return aResult;
}


// Template function for processing events without result
inline void ProcessVoidEvent(SALOME_Event* theEvent){
  theEvent->process();
  theEvent->release();
}


#endif
