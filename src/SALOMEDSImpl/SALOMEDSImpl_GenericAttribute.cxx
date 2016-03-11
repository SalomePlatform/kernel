// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
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

//  File   : SALOMEDSImpl_GenericAttribute.hxx
//  Author : SERGEY_RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_Attributes.hxx"
#include "SALOMEDSImpl_Study.hxx"
#include "SALOMEDSImpl_StudyBuilder.hxx"

std::string SALOMEDSImpl_GenericAttribute::Impl_GetType(DF_Attribute* theAttr)
{
  SALOMEDSImpl_GenericAttribute* ga = dynamic_cast<SALOMEDSImpl_GenericAttribute*>(theAttr);  
  if (ga)
    return ga->Type();

  return "";
}

std::string SALOMEDSImpl_GenericAttribute::Impl_GetClassType(DF_Attribute* theAttr)
{
  SALOMEDSImpl_GenericAttribute* ga = dynamic_cast<SALOMEDSImpl_GenericAttribute*>(theAttr);
  if (ga)
    return ga->GetClassType();
  
  return "";
} 

void SALOMEDSImpl_GenericAttribute::Impl_CheckLocked(DF_Attribute* theAttr)
{
  SALOMEDSImpl_GenericAttribute* ga = dynamic_cast<SALOMEDSImpl_GenericAttribute*>(theAttr);
  ga->CheckLocked();
}

std::string SALOMEDSImpl_GenericAttribute::Type() 
{ 
    return _type; 
}


void SALOMEDSImpl_GenericAttribute::CheckLocked()
{
  DF_Label aLabel = Label();
  if(aLabel.IsNull()) return;

  SALOMEDSImpl_Study* aStudy = SALOMEDSImpl_Study::GetStudy(aLabel);
  if(!aStudy) return;
  if(aStudy->IsLocked()) {
    aStudy->_errorCode = "LockProtection";
    throw LockProtection("LockProtection");
  }                                         
}

SALOMEDSImpl_SObject SALOMEDSImpl_GenericAttribute::GetSObject()
{
  SALOMEDSImpl_SObject so; 
  DF_Label aLabel = Label();
  if(aLabel.IsNull()) return so;
  return SALOMEDSImpl_Study::SObject(aLabel);
}

/*!
 * Set modification flag of the Attribute.
 * param reason reason of the modification (by default equal to 0)
 */
void SALOMEDSImpl_GenericAttribute::SetModifyFlag(int reason)
{
   DF_Label aLabel = Label();
   if(aLabel.IsNull()) return; 

  SALOMEDSImpl_Study* aStudy = SALOMEDSImpl_Study::GetStudy(aLabel);
  if(aStudy) aStudy->modifySO_Notification(GetSObject(), reason);
  if(aStudy) aStudy->Modify();
}
