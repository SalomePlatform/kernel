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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOMEDSImpl_AttributeLocalID.cxx
//  Author : Sergey RUIN
//  Module : SALOME


#include "SALOMEDSImpl_AttributeLocalID.hxx"
#include <Standard_GUID.hxx>

using namespace std;

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeLocalID, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeLocalID, SALOMEDSImpl_GenericAttribute )

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributeLocalID::GetID () 
{
  static Standard_GUID SALOMEDSImpl_AttributeLocalID ("12837196-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeLocalID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(SALOMEDSImpl_AttributeLocalID) SALOMEDSImpl_AttributeLocalID::Set (const TDF_Label& L,
									  const Standard_Integer value) 
{
  Handle(SALOMEDSImpl_AttributeLocalID) A;
  if (!L.FindAttribute(SALOMEDSImpl_AttributeLocalID::GetID(),A)) {
    A = new  SALOMEDSImpl_AttributeLocalID(); 
    L.AddAttribute(A);
  }
  
  A->SetValue(value); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeLocalID::SALOMEDSImpl_AttributeLocalID()
:SALOMEDSImpl_GenericAttribute("AttributeLocalID")
{
  myValue = 0; 
}

//=======================================================================
//function : Set
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeLocalID::SetValue(const Standard_Integer theValue)
{
  CheckLocked();

  Backup();

  myValue = theValue;

  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}
    

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributeLocalID::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDSImpl_AttributeLocalID::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeLocalID(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeLocalID::Restore(const Handle(TDF_Attribute)& with) 
{
  myValue = Handle(SALOMEDSImpl_AttributeLocalID)::DownCast (with)->Value();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeLocalID::Paste (const Handle(TDF_Attribute)& into,
					   const Handle(TDF_RelocationTable)& RT) const
{
  Handle(SALOMEDSImpl_AttributeLocalID)::DownCast (into)->SetValue(myValue);
}

