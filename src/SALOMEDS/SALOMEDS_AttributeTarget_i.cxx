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
//  $Header$

#include <TDF_ListIteratorOfLabelList.hxx>
#include <TDF_LabelList.hxx>
#include <TDF_Label.hxx>
#include <TDF_Tool.hxx>

#include "SALOMEDS_AttributeTarget_i.hxx"
#include "SALOMEDS_SObject_i.hxx"

using namespace std;

void SALOMEDS_AttributeTarget_i::Add(SALOMEDS::SObject_ptr anObject)
{
  TDF_Label aLabel;
  TDF_Tool::Label(_myAttr->Label().Data(),anObject->GetID(),aLabel,1);
  _myAttr->Append(aLabel);
}

SALOMEDS::Study::ListOfSObject* SALOMEDS_AttributeTarget_i::Get() {
  TDF_LabelList aLList;

  _myAttr->Get(aLList);
  SALOMEDS::Study::ListOfSObject_var aSList = new SALOMEDS::Study::ListOfSObject;

  if (aLList.Extent() == 0) 
    return aSList._retn();

  aSList->length(aLList.Extent());
  TDF_ListIteratorOfLabelList anIter(aLList);
  SALOMEDS_Study_i* aStudy = _mySObject->GetStudyServant();
  for(int index = 0; anIter.More(); anIter.Next(), index++){
    const TDF_Label& aLabel = anIter.Value();
    SALOMEDS_SObject_i* anSO = SALOMEDS_SObject_i::New(aStudy,aLabel);
    aSList[index] = anSO->_this();
  }
  return aSList._retn();
}

void SALOMEDS_AttributeTarget_i::Remove(SALOMEDS::SObject_ptr anObject) {
  TDF_Label aLabel;
  CORBA::String_var anID = anObject->GetID();
  TDF_Tool::Label(_myAttr->Label().Data(),anID.inout(),aLabel,1);
  _myAttr->Remove(aLabel);
}
