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
//  File   : SALOMEDS_TextHighlightColorAttribute.cxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header: 

#include "SALOMEDS_TextHighlightColorAttribute.ixx"
using namespace std;

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_TextHighlightColorAttribute::GetID () 
{
  static Standard_GUID SALOMEDS_TextHighlightColorAttributeID ("12837190-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDS_TextHighlightColorAttributeID;
}



//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDS_TextHighlightColorAttribute::SALOMEDS_TextHighlightColorAttribute()
{Init(1,3);}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_TextHighlightColorAttribute::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDS_TextHighlightColorAttribute::NewEmpty () const
{  
  return new SALOMEDS_TextHighlightColorAttribute(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDS_TextHighlightColorAttribute::Restore(const Handle(TDF_Attribute)& with) 
{
  Handle(TColStd_HArray1OfReal) s = Handle(TDataStd_RealArray)::DownCast (with)->Array ();
  TDataStd_RealArray::ChangeArray(s);
  return;
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDS_TextHighlightColorAttribute::Paste (const Handle(TDF_Attribute)& into,
                                                  const Handle(TDF_RelocationTable)& ) const
{
  Handle(TDataStd_RealArray)::DownCast (into)->ChangeArray (Array());
}

