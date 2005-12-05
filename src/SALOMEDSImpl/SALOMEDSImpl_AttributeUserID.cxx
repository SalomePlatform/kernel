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
//  File   : SALOMEDSImpl_AttributeUAttribute.cxx
//  Author : Sergey RUIN
//  Module : SALOME


#include "SALOMEDSImpl_AttributeUserID.hxx"

using namespace std;

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeUserID, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeUserID, SALOMEDSImpl_GenericAttribute )

Handle(SALOMEDSImpl_AttributeUserID) SALOMEDSImpl_AttributeUserID::Set (const TDF_Label& L, const Standard_GUID& ID) 
{
  Handle(SALOMEDSImpl_AttributeUserID) A;
  if (!L.FindAttribute(ID, A)) {
    A = new  SALOMEDSImpl_AttributeUserID(); 
    A->SetValue(ID);
    L.AddAttribute(A);
  }
  return A;
}

//=======================================================================
//function : ID
//purpose  :
//=======================================================================
const Standard_GUID& SALOMEDSImpl_AttributeUserID::ID() const
{ return myID; }


//=======================================================================
//function : SetValue
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeUserID::SetValue( const Standard_GUID&  guid)
{
  CheckLocked();
  if(myID == guid) return;

  Backup();
  myID = guid;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}        

//=======================================================================
//function : NewEmpty
//purpose  :
//=======================================================================
Handle(TDF_Attribute) SALOMEDSImpl_AttributeUserID::NewEmpty () const
{
  Handle(SALOMEDSImpl_AttributeUserID) A = new SALOMEDSImpl_AttributeUserID();
  A->SetValue(myID);
  return A;
}

//=======================================================================
//function : Restore
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeUserID::Restore(const Handle(TDF_Attribute)& with)
{
  Handle(SALOMEDSImpl_AttributeUserID) A = Handle(SALOMEDSImpl_AttributeUserID)::DownCast(with);
  SetValue( A->ID() );
}

//=======================================================================
//function : Paste
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeUserID::Paste (const Handle(TDF_Attribute)& into,
					  const Handle(TDF_RelocationTable)& RT) const
{
  Handle(SALOMEDSImpl_AttributeUserID) A = Handle(SALOMEDSImpl_AttributeUserID)::DownCast(into);
  A->SetValue( myID );
}    

TCollection_AsciiString SALOMEDSImpl_AttributeUserID::Type() 
{

  char* aUAttrName = new char[60];
  char aGUID[40];
  ID().ToCString(aGUID);
  sprintf(aUAttrName, "AttributeUserID_%s",aGUID);

  TCollection_AsciiString ret(aUAttrName);
  delete aUAttrName;

  return ret;
}
