// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/
//
//  File   : SALOMEDSImpl_AttributePersistentRef.cxx
//  Author : Sergey RUIN
//  Module : SALOME


#include "SALOMEDSImpl_AttributePersistentRef.hxx"
#include <Standard_GUID.hxx>

using namespace std;

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributePersistentRef, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributePersistentRef, SALOMEDSImpl_GenericAttribute )

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributePersistentRef::GetID () 
{
  static Standard_GUID SALOMEDSImpl_AttributePersistentRefID ("92888E06-7074-11d5-A690-0800369C8A03");
  return SALOMEDSImpl_AttributePersistentRefID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(SALOMEDSImpl_AttributePersistentRef) SALOMEDSImpl_AttributePersistentRef::Set (const TDF_Label& L,
										      const TCollection_ExtendedString& S)
{
  Handle(SALOMEDSImpl_AttributePersistentRef) A;
  if (!L.FindAttribute(SALOMEDSImpl_AttributePersistentRef::GetID(),A)) {
    A = new  SALOMEDSImpl_AttributePersistentRef(); 
    L.AddAttribute(A);
  }
  
  A->SetValue (S); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributePersistentRef::SALOMEDSImpl_AttributePersistentRef()
:SALOMEDSImpl_GenericAttribute("AttributePersistentRef")
{
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributePersistentRef::ID () const { return GetID(); }


//=======================================================================
//function : SetValue
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributePersistentRef::SetValue (const TCollection_ExtendedString& S)
{
  CheckLocked();

  if(myString == S) return;

  Backup();

  myString = S;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDSImpl_AttributePersistentRef::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributePersistentRef(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributePersistentRef::Restore(const Handle(TDF_Attribute)& with) 
{
  myString = Handle(SALOMEDSImpl_AttributePersistentRef)::DownCast (with)->Value ();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributePersistentRef::Paste (const Handle(TDF_Attribute)& into,
						 const Handle(TDF_RelocationTable)& RT) const
{
  Handle(SALOMEDSImpl_AttributePersistentRef)::DownCast (into)->SetValue(myString);
}

