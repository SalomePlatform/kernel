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
//  File   : SALOMEDSImpl_AttributeReal.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDSImpl_AttributeReal.hxx"

using namespace std;

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeReal, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeReal, SALOMEDSImpl_GenericAttribute )

//=======================================================================
//function : GetID
//purpose  :
//=======================================================================
const Standard_GUID& SALOMEDSImpl_AttributeReal::GetID ()
{
  static Standard_GUID realID ("1D1992F0-56F4-46b4-8065-CDEA68061CAB");
  return realID;
}   

Handle(SALOMEDSImpl_AttributeReal) SALOMEDSImpl_AttributeReal::Set (const TDF_Label& L, const Standard_Real Val) 
{
  Handle(SALOMEDSImpl_AttributeReal) A;
  if (!L.FindAttribute(SALOMEDSImpl_AttributeReal::GetID(), A)) {
    A = new  SALOMEDSImpl_AttributeReal(); 
    L.AddAttribute(A);
  }

  A->SetValue(Val); 
  return A;
}

//=======================================================================
//function : SetValue
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeReal::SetValue(const Standard_Real v)
{
  CheckLocked();

  if( myValue == v) return;

  Backup();
  myValue = v;  

  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

//=======================================================================
//function : ID
//purpose  :
//=======================================================================
const Standard_GUID& SALOMEDSImpl_AttributeReal::ID () const 
{ 
  return GetID(); 
} 

//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================
Handle(TDF_Attribute) SALOMEDSImpl_AttributeReal::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeReal(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================
void SALOMEDSImpl_AttributeReal::Restore(const Handle(TDF_Attribute)& with) 
{
  myValue = Handle(SALOMEDSImpl_AttributeReal)::DownCast (with)->Value ();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================
void SALOMEDSImpl_AttributeReal::Paste (const Handle(TDF_Attribute)& into,
					const Handle(TDF_RelocationTable)& RT) const
{
  Handle(SALOMEDSImpl_AttributeReal)::DownCast (into)->SetValue(myValue);
}
