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
//  File   : SALOMEDSImpl_AttributeSelectable.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDSImpl_AttributeSelectable.hxx"
#include <Standard_GUID.hxx>

using namespace std;

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeSelectable, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeSelectable, SALOMEDSImpl_GenericAttribute )

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributeSelectable::GetID () 
{
  static Standard_GUID SALOMEDSImpl_AttributeSelectableID ("12837188-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeSelectableID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(SALOMEDSImpl_AttributeSelectable) SALOMEDSImpl_AttributeSelectable::Set (const TDF_Label& L,
										const Standard_Integer value) 
{
  Handle(SALOMEDSImpl_AttributeSelectable) A;
  if (!L.FindAttribute(SALOMEDSImpl_AttributeSelectable::GetID(),A)) {
    A = new  SALOMEDSImpl_AttributeSelectable(); 
    L.AddAttribute(A);
  }
  
  A->SetSelectable (value); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeSelectable::SALOMEDSImpl_AttributeSelectable()
:SALOMEDSImpl_GenericAttribute("AttributeSelectable")
{
  myValue = 0;
}

//=======================================================================
//function : SetSelectable
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeSelectable::SetSelectable(const Standard_Integer theValue)
{
  Backup();

  (theValue!=0)?myValue=1:myValue=0;
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributeSelectable::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDSImpl_AttributeSelectable::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeSelectable(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeSelectable::Restore(const Handle(TDF_Attribute)& with) 
{
  myValue = Handle(SALOMEDSImpl_AttributeSelectable)::DownCast (with)->IsSelectable ();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeSelectable::Paste (const Handle(TDF_Attribute)& into,
                                              const Handle(TDF_RelocationTable)& RT) const
{
  Handle(SALOMEDSImpl_AttributeSelectable)::DownCast (into)->SetSelectable (myValue);
}

