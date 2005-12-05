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
//  File   : SALOMEDSImpl_AttributeDrawable.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef WNT
using namespace std;
#endif
#include "SALOMEDSImpl_AttributeDrawable.hxx"

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeDrawable, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeDrawable, SALOMEDSImpl_GenericAttribute )

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================
const Standard_GUID& SALOMEDSImpl_AttributeDrawable::GetID () 
{
  static Standard_GUID SALOMEDSImpl_AttributeDrawableID ("12837184-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeDrawableID;
}


//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(SALOMEDSImpl_AttributeDrawable) SALOMEDSImpl_AttributeDrawable::Set (const TDF_Label& L,
									    const Standard_Integer value) 
{
  Handle(SALOMEDSImpl_AttributeDrawable) A;
  if (!L.FindAttribute(SALOMEDSImpl_AttributeDrawable::GetID(),A)) {
    A = new  SALOMEDSImpl_AttributeDrawable(); 
    L.AddAttribute(A);
  }
  
  A->SetDrawable (value); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeDrawable::SALOMEDSImpl_AttributeDrawable()
:SALOMEDSImpl_GenericAttribute("AttributeDrawable") 
{
  myValue = 0;
}

//=======================================================================
//function : SetDrawable
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeDrawable::SetDrawable(const Standard_Integer theValue)
{
  CheckLocked();

  Backup();

  (theValue!=0)?myValue=1:myValue=0;
}
           

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributeDrawable::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDSImpl_AttributeDrawable::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeDrawable(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeDrawable::Restore(const Handle(TDF_Attribute)& with) 
{
  myValue = Handle(SALOMEDSImpl_AttributeDrawable)::DownCast (with)->IsDrawable ();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeDrawable::Paste (const Handle(TDF_Attribute)& into,
                                    const Handle(TDF_RelocationTable)& RT) const
{
  Handle(SALOMEDSImpl_AttributeDrawable)::DownCast (into)->SetDrawable (myValue);
}

