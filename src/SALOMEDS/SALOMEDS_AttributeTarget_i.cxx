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
//  File   : SALOMEDS_AttributeTarget_i.cxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header: 

#include "SALOMEDS_AttributeTarget_i.hxx"
#include "SALOMEDS_SObject_i.hxx"
#include <TDF_LabelList.hxx>
#include <TDF_ListIteratorOfLabelList.hxx>
using namespace std;

void SALOMEDS_AttributeTarget_i::Add(SALOMEDS::SObject_ptr anObject) {
  TDF_Label aLabel;
  TDF_Tool::Label(_myAttr->Label().Data(),anObject->GetID(),aLabel,1);
  (Handle(SALOMEDS_TargetAttribute)::DownCast(_myAttr))->Append(aLabel);
}

SALOMEDS::Study::ListOfSObject* SALOMEDS_AttributeTarget_i::Get() {
  TDF_LabelList aLList;
  SALOMEDS::Study::ListOfSObject_var aSList = new SALOMEDS::Study::ListOfSObject;
  (Handle(SALOMEDS_TargetAttribute)::DownCast(_myAttr))->Get(aLList);
  if (aLList.Extent() == 0) 
    return aSList._retn();
  aSList->length(aLList.Extent());
  TDF_ListIteratorOfLabelList anIter(aLList);
  int index;
  for(index=0;anIter.More();anIter.Next(),index++) {
    SALOMEDS_SObject_i* anSO = new SALOMEDS_SObject_i(anIter.Value(),_myOrb);
    aSList[index] = anSO->_this();
  }
  return aSList._retn();
}

void SALOMEDS_AttributeTarget_i::Remove(SALOMEDS::SObject_ptr anObject) {
  TDF_Label aLabel;
  TDF_Tool::Label(_myAttr->Label().Data(),anObject->GetID(),aLabel,1);
  (Handle(SALOMEDS_TargetAttribute)::DownCast(_myAttr))->Remove(aLabel);
}
