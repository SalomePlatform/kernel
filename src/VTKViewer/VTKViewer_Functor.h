//  SALOME VTKViewer : build VTK viewer into Salome desktop
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
//  File   : VTKViewer_ViewFrame.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef VTKViewer_Functor_H
#define VTKViewer_Functor_H

#include <functional>

#include <string>
#include "SALOME_InteractiveObject.hxx"

namespace SALOME{
  namespace VTK{


    template<class TActor> struct TIsSameEntry
    {
      std::string myEntry;
      TIsSameEntry(const char* theEntry): myEntry(theEntry){}
      bool operator()(TActor* theActor){
	if(theActor->hasIO()){
	  Handle(SALOME_InteractiveObject) anIO = theActor->getIO();
	  if(anIO->hasEntry())
	    return myEntry == anIO->getEntry();
	}
	return false;
      }
    };


    template<class TActor> struct TIsSameIObject
    {
      Handle(SALOME_InteractiveObject) myIObject;
      TIsSameIObject(const Handle(SALOME_InteractiveObject)& theIObject):
	myIObject(theIObject)
      {}
      bool operator()(TActor* theActor){
	if(theActor->hasIO()){
	  Handle(SALOME_InteractiveObject) anIO = theActor->getIO();
	  return myIObject->isSame(anIO);
	}
	return false;
      }
    };


    template<class TActor, class TArg, class TStoreArg = TArg> struct TSetFunction
    {
      typedef void (TActor::* TAction)(TArg);
      TAction myAction;
      TStoreArg myArg;
      TSetFunction(TAction theAction, TArg theArg):
	myAction(theAction), myArg(theArg)
      {}
      void operator()(TActor* theActor){
	(theActor->*myAction)(myArg);
      }
    };


    template<class TActor, class TArg = int> struct TSetVisibility: TSetFunction<TActor,TArg>
    {
      TSetVisibility(TArg theArg): 
	TSetFunction<TActor,TArg>(&TActor::SetVisibility,theArg)
      {}
    };


  }
}


#endif
