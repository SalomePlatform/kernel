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
//  File   : SALOMEDSImpl_AttributeExpandable.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef WNT
using namespace std;
#endif
#include "SALOMEDSImpl_AttributeExpandable.hxx"

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeExpandable, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeExpandable, SALOMEDSImpl_GenericAttribute )

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================
const Standard_GUID& SALOMEDSImpl_AttributeExpandable::GetID () 
{
  static Standard_GUID SALOMEDSImpl_AttributeExpandableID ("12837185-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeExpandableID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================
Handle(SALOMEDSImpl_AttributeExpandable) SALOMEDSImpl_AttributeExpandable::Set (const TDF_Label& L,
										const Standard_Integer value) 
{
  Handle(SALOMEDSImpl_AttributeExpandable) A;
  if (!L.FindAttribute(SALOMEDSImpl_AttributeExpandable::GetID(),A)) {
    A = new  SALOMEDSImpl_AttributeExpandable(); 
    L.AddAttribute(A);
  }
  
  A->SetExpandable(value); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeExpandable::SALOMEDSImpl_AttributeExpandable()
:SALOMEDSImpl_GenericAttribute("AttributeExpandable")
{
  myValue = 0;
}

//=======================================================================
//function : SetExpandable
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeExpandable::SetExpandable(const Standard_Integer theValue)
{
  CheckLocked();

  Backup();

  (theValue!=0)?myValue=1:myValue=0;
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributeExpandable::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDSImpl_AttributeExpandable::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeExpandable(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeExpandable::Restore(const Handle(TDF_Attribute)& with) 
{
  myValue = Handle(SALOMEDSImpl_AttributeExpandable)::DownCast (with)->IsExpandable ();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeExpandable::Paste (const Handle(TDF_Attribute)& into,
                                    const Handle(TDF_RelocationTable)& RT) const
{
  Handle(SALOMEDSImpl_AttributeExpandable)::DownCast (into)->SetExpandable (myValue);
}

