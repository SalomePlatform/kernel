using namespace std;
//  File      : SALOMEDS_TargetAttribute.cxx
//  Created   : Fri Aug 16 10:20:05 2002
//  Author    : Yves FRICAUD
//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header:  

#include "SALOMEDS_TargetAttribute.ixx"
#include <TDF_Reference.hxx>
#include <TDF_ListIteratorOfAttributeList.hxx>
#include "utilities.h"

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

