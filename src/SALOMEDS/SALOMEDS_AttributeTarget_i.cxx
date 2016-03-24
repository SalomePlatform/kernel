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

//  File   : SALOMEDS_AttributeTarget_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_AttributeTarget_i.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_Study.hxx"
#include "SALOMEDS.hxx"
#include "DF_Label.hxx"

#include <vector>

void SALOMEDS_AttributeTarget_i::Add(SALOMEDS::SObject_ptr anObject) 
{
  SALOMEDS::Locker lock; 
  DF_Label aLabel = DF_Label::Label(_impl->Label(), anObject->GetID(), 1);
  dynamic_cast<SALOMEDSImpl_AttributeTarget*>(_impl)->Add(SALOMEDSImpl_Study::SObject(aLabel));
}

SALOMEDS::Study::ListOfSObject* SALOMEDS_AttributeTarget_i::Get() 
{
  SALOMEDS::Locker lock; 
  std::vector<SALOMEDSImpl_SObject> aSeq = dynamic_cast<SALOMEDSImpl_AttributeTarget*>(_impl)->Get();
  SALOMEDS::Study::ListOfSObject_var aSList = new SALOMEDS::Study::ListOfSObject;
  int aLength = aSeq.size(), i;
  if (aLength == 0) return aSList._retn();
  aSList->length(aLength);
  for(i=0; i <aLength; i++) {
    SALOMEDS::SObject_var anSO = SALOMEDS_SObject_i::New(aSeq[i], _orb);
    aSList[i] = anSO;
  }
  return aSList._retn();
}

void SALOMEDS_AttributeTarget_i::Remove(SALOMEDS::SObject_ptr anObject) 
{
  SALOMEDS::Locker lock; 
  DF_Label aLabel = DF_Label::Label(_impl->Label(), anObject->GetID(), 1);
  dynamic_cast<SALOMEDSImpl_AttributeTarget*>(_impl)->Remove(SALOMEDSImpl_Study::SObject(aLabel));
}
