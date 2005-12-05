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
//File:    testDS.cxx
//Author:  Sergey RUIN

#include <stdio.h>
#include <iostream> 

#include <TColStd_HSequenceOfTransient.hxx>
#include <TCollection_AsciiString.hxx>
#include <TDocStd_Document.hxx>
#include <TDF_Attribute.hxx>
#include <TDF_Label.hxx>
#include <TDF_Data.hxx>
#include <TDF_Tool.hxx>

#include "SALOMEDSImpl_Attributes.hxx"
#include "SALOMEDSImpl_StudyManager.hxx"
#include "SALOMEDSImpl_Study.hxx"
#include "SALOMEDSImpl_StudyBuilder.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_SComponent.hxx"
//#include "SALOMEDSImpl_.hxx"

int main (int argc, char * argv[])
{
  cout << "Test started " << endl;

  Handle(SALOMEDSImpl_StudyManager) aSM = new SALOMEDSImpl_StudyManager();
  cout << "Manager is created " << endl;
  Handle(SALOMEDSImpl_Study) aStudy = aSM->NewStudy("SRN");
  cout << "Study with id = " << aStudy->StudyId() << " is created " << endl; 
  Handle(SALOMEDSImpl_StudyBuilder) aBuilder = aStudy->NewBuilder();
  cout << "StudyBuilder is created " << endl;
  Handle(SALOMEDSImpl_SComponent) aSC = aBuilder->NewComponent("TEST");
  cout << "New component with type " << aSC->ComponentDataType() << " is created " << endl;
  Handle(SALOMEDSImpl_SObject) aSO = aBuilder->NewObject(aSC);
  cout << "New SObject with  ID = " << aSO->GetID() << " is created"  << endl;
  TCollection_AsciiString anEntry;
  TDF_Tool::Entry(aSO->GetLabel(), anEntry);
  cout << "An entry of newly created SO is "  << anEntry << endl;
  Handle(SALOMEDSImpl_AttributeIOR) aIORA = SALOMEDSImpl_AttributeIOR::Set(aSO->GetLabel(), "ior1234");
  cout << "New AttributeIOR is created, it contains " << aIORA->Value() << endl;
  Handle(SALOMEDSImpl_GenericAttribute) ga = Handle(SALOMEDSImpl_GenericAttribute)::DownCast(aIORA);
  cout << "Attribute has type: " << ga->Type() << " and value: " << ga->Save() << endl; 
  cout << "Just another way to create an attribute: official one :) " << endl;
  Handle(TDF_Attribute) aTDFAttr =  aBuilder->FindOrCreateAttribute(aSO, "AttributeName");  
  Handle(SALOMEDSImpl_AttributeName) aRN = Handle(SALOMEDSImpl_AttributeName)::DownCast(aTDFAttr);
  aRN->SetValue("name_attribute");
  cout << " The type = " << aRN->Type() << endl;
  ga = Handle(SALOMEDSImpl_GenericAttribute)::DownCast(aRN);
  cout << "Attribute has type: " << ga->Type() << " and value: " << ga->Save() << endl;   
  cout << "Check GetObjectPath: " << aStudy->GetObjectPath(aSO) << endl;
  
  Handle(SALOMEDSImpl_SObject) aSubSO = aBuilder->NewObject(aSO);
  aTDFAttr =  aBuilder->FindOrCreateAttribute(aSubSO, "AttributeIOR");  
  Handle(SALOMEDSImpl_AttributeIOR) aIOR2 = Handle(SALOMEDSImpl_AttributeIOR)::DownCast(aTDFAttr);
  aIOR2->SetValue("some ior");
  aBuilder->Addreference(aSubSO, aSO);
  Handle(SALOMEDSImpl_SObject) aRefObject;
  aSubSO->ReferencedObject(aRefObject);
  cout << "Check reference : ReferencedObject is " << aRefObject->GetID() << endl;
  cout << "Check : Remove object: " << endl;
  aBuilder->RemoveObject(aSubSO);
  cout << "Remove: done" << endl;

  cout << "Check the attributes on SObject" << endl;
  Handle(TColStd_HSequenceOfTransient) aSeq = aSO->GetAllAttributes();
  for(int i = 1; i <= aSeq->Length(); i++) 
    cout << "Found: " << Handle(SALOMEDSImpl_GenericAttribute)::DownCast(aSeq->Value(i))->Type() << endl;

  cout << "Check AttributeTreeNode " << endl;
  aTDFAttr =  aBuilder->FindOrCreateAttribute(aSO, "AttributeTreeNode");  
  cout << Handle(SALOMEDSImpl_GenericAttribute)::DownCast(aTDFAttr)->Type() << endl;
  cout << "Check AttributeTreeNode : done " << endl;

  cout << "Test finished " << endl;    
  return 0;
}

