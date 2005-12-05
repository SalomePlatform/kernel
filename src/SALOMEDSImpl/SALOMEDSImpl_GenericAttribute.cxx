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
//  File   : SALOMEDSImpl_GenericAttribute.hxx
//  Author : SERGEY_RUIN
//  Module : SALOME


#include "SALOMEDSImpl_Attributes.hxx"
#include "SALOMEDSImpl_Study.hxx"
#include "SALOMEDSImpl_StudyBuilder.hxx"

using namespace std;

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_GenericAttribute, TDF_Attribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_GenericAttribute, TDF_Attribute )

char* SALOMEDSImpl_GenericAttribute::Impl_GetType(const Handle(TDF_Attribute)& theAttr)
{
  Handle(SALOMEDSImpl_GenericAttribute) ga = Handle(SALOMEDSImpl_GenericAttribute)::DownCast(theAttr);  
  return ga->Type().ToCString();
}

char* SALOMEDSImpl_GenericAttribute::Impl_GetClassType(const Handle(TDF_Attribute)& theAttr)
{
  Handle(SALOMEDSImpl_GenericAttribute) ga = Handle(SALOMEDSImpl_GenericAttribute)::DownCast(theAttr);
  return ga->GetClassType().ToCString();
} 

void SALOMEDSImpl_GenericAttribute::Impl_CheckLocked(const Handle(TDF_Attribute)& theAttr)
{
  Handle(SALOMEDSImpl_GenericAttribute) ga = Handle(SALOMEDSImpl_GenericAttribute)::DownCast(theAttr);
  ga->CheckLocked();
}

TCollection_AsciiString SALOMEDSImpl_GenericAttribute::Type() 
{ 
    return _type; 
}


void SALOMEDSImpl_GenericAttribute::CheckLocked()
{
  TDF_Label aLabel = Label();
  if(aLabel.IsNull()) return;

  Handle(SALOMEDSImpl_Study) aStudy = SALOMEDSImpl_Study::GetStudy(aLabel);
  if(aStudy.IsNull() || aStudy->NewBuilder()->HasOpenCommand()) return;
  if(aStudy->IsLocked()) {
    aStudy->_errorCode = "LockProtection";
    throw LockProtection("LockProtection");
  }                                         
}

Handle(SALOMEDSImpl_SObject) SALOMEDSImpl_GenericAttribute::GetSObject()
{
  TDF_Label aLabel = Label();
  if(aLabel.IsNull()) return NULL;
  return SALOMEDSImpl_Study::SObject(aLabel);
}

void SALOMEDSImpl_GenericAttribute::SetModifyFlag()
{
   TDF_Label aLabel = Label();
   if(aLabel.IsNull()) return; 

  Handle(SALOMEDSImpl_Study) aStudy = SALOMEDSImpl_Study::GetStudy(aLabel);
  if(!aStudy.IsNull()) aStudy->Modify();
}

