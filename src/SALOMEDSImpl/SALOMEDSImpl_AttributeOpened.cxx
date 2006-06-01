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
//  File   : SALOMEDSImpl_AttributeOpened.cxx
//  Author : Sergey RUIN
//  Module : SALOME


#include "SALOMEDSImpl_AttributeOpened.hxx"
#include <Standard_GUID.hxx>

using namespace std;

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeOpened, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeOpened, SALOMEDSImpl_GenericAttribute )

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributeOpened::GetID () 
{
  static Standard_GUID SALOMEDSImpl_AttributeOpenedID ("12837186-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeOpenedID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(SALOMEDSImpl_AttributeOpened) SALOMEDSImpl_AttributeOpened::Set (const TDF_Label& L,
									const Standard_Integer value) 
{
  Handle(SALOMEDSImpl_AttributeOpened) A;
  if (!L.FindAttribute(SALOMEDSImpl_AttributeOpened::GetID(),A)) {
    A = new  SALOMEDSImpl_AttributeOpened(); 
    L.AddAttribute(A);
  }
  
  A->SetOpened (value); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeOpened::SALOMEDSImpl_AttributeOpened()
:SALOMEDSImpl_GenericAttribute("AttributeOpened")
{
  myValue = 0; 
}

//=======================================================================
//function : SetOpened
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeOpened::SetOpened(const Standard_Integer theValue)
{
  Backup();

  (theValue!=0)?myValue=1:myValue=0;
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributeOpened::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDSImpl_AttributeOpened::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeOpened(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeOpened::Restore(const Handle(TDF_Attribute)& with) 
{
  myValue = Handle(SALOMEDSImpl_AttributeOpened)::DownCast (with)->IsOpened ();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeOpened::Paste (const Handle(TDF_Attribute)& into,
					  const Handle(TDF_RelocationTable)& RT) const
{
  Handle(SALOMEDSImpl_AttributeOpened)::DownCast (into)->SetOpened (myValue);
}

