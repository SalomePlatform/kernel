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
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : SALOMEDS_ExternalFileDef.cxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header: 

#include "SALOMEDS_ExternalFileDef.ixx"
#include <TDataStd_Comment.hxx>
#include <TCollection_ExtendedString.hxx>
using namespace std;

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_ExternalFileDef::GetID () 
{
  static Standard_GUID SALOMEDS_ExternalFileDefID ("7123AD4C-ACDB-4e3a-8FDC-70EA164D2CBE");
  return SALOMEDS_ExternalFileDefID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(SALOMEDS_ExternalFileDef) SALOMEDS_ExternalFileDef::Set (const TDF_Label& L,
								const TCollection_ExtendedString& S) 
{

  Handle(SALOMEDS_ExternalFileDef) A;
  if (!L.FindAttribute(SALOMEDS_ExternalFileDef::GetID(),A)) {
    A = new  SALOMEDS_ExternalFileDef(); 
    L.AddAttribute(A);
  }
  
  (Handle(TDataStd_Comment)::DownCast(A))->Set (S); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDS_ExternalFileDef::SALOMEDS_ExternalFileDef()
{}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_ExternalFileDef::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDS_ExternalFileDef::NewEmpty () const
{  
  return new SALOMEDS_ExternalFileDef(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDS_ExternalFileDef::Restore(const Handle(TDF_Attribute)& with) 
{
  TCollection_ExtendedString s = Handle(TDataStd_Comment)::DownCast (with)->Get ();
  TDataStd_Comment::Set(s);
  return;
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDS_ExternalFileDef::Paste (const Handle(TDF_Attribute)& into,
		           const Handle(TDF_RelocationTable)& RT) const
{
  Handle(TDataStd_Comment)::DownCast (into)->Set (Get());
}

