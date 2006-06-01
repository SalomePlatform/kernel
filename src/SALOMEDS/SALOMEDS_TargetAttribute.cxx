//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : SALOMEDS_TargetAttribute.cxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#include "SALOMEDS_TargetAttribute.ixx"
#include <TDF_Reference.hxx>
#include <TDF_ListIteratorOfAttributeList.hxx>
#include "utilities.h"
using namespace std;

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_TargetAttribute::GetID () 
{
  static Standard_GUID SALOMEDS_TargetAttributeID ("12837197-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDS_TargetAttributeID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(SALOMEDS_TargetAttribute) SALOMEDS_TargetAttribute::Set (const TDF_Label& L) 
{
  Handle(SALOMEDS_TargetAttribute) A;
  if (!L.FindAttribute(SALOMEDS_TargetAttribute::GetID(),A)) {
    A = new  SALOMEDS_TargetAttribute(); 
    L.AddAttribute(A);
  }
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDS_TargetAttribute::SALOMEDS_TargetAttribute()
{}

//=======================================================================
//function : Append
//purpose  : 
//=======================================================================

void SALOMEDS_TargetAttribute::Append(TDF_Label& theReferencedObject) {
  Backup();
  Handle(TDF_Reference) aReference;
  if (theReferencedObject.FindAttribute(TDF_Reference::GetID(),aReference)) {
    TDF_ListIteratorOfAttributeList anIter(GetVariables());
    for(;anIter.More();anIter.Next()) if(anIter.Value()->Label() == theReferencedObject) return; //BugID: PAL6192    
    GetVariables().Append(aReference);
  } else {
    MESSAGE("SALOMEDS_TargetAttribute::Append: can't append referenced object");
  }
}

//=======================================================================
//function : Get
//purpose  : 
//=======================================================================
void SALOMEDS_TargetAttribute::Get(TDF_LabelList& theReferencedObjects) {
  theReferencedObjects.Clear();
  TDF_ListIteratorOfAttributeList anIter(GetVariables());
  for(;anIter.More();anIter.Next()) {
    theReferencedObjects.Append(anIter.Value()->Label());
  }
}

//=======================================================================
//function : Remove
//purpose  : 
//=======================================================================
void SALOMEDS_TargetAttribute::Remove(TDF_Label& theReferencedObject) {
  Backup();
  TDF_ListIteratorOfAttributeList anIter(GetVariables());
  for(;anIter.More();anIter.Next()) {
    if (anIter.Value()->Label() == theReferencedObject) {
      GetVariables().Remove(anIter);
      return;
    }
  }  
  MESSAGE("SALOMEDS_TargetAttribute::Remove: nothing to remove");
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_TargetAttribute::ID () const { return GetID(); }

