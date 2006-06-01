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
//File:    testDS.cxx
//Author:  Sergey RUIN

#include <stdio.h>
#include <iostream> 
#include <vector>

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
#include "SALOMEDSImpl_AttributeParameter.hxx"

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

  cout << "Check AttributeTreeNode " << endl;
  aTDFAttr =  aBuilder->FindOrCreateAttribute(aSO, "AttributeTreeNode");  
  cout << Handle(SALOMEDSImpl_GenericAttribute)::DownCast(aTDFAttr)->Type() << endl;
  cout << "Check AttributeTreeNode : done " << endl;

  aTDFAttr =  aBuilder->FindOrCreateAttribute(aSO, "AttributeParameter");  
  cout << Handle(SALOMEDSImpl_GenericAttribute)::DownCast(aTDFAttr)->Type() << endl;

  cout << "Check the attributes on SObject" << endl;
  Handle(TColStd_HSequenceOfTransient) aSeq = aSO->GetAllAttributes();
  for(int i = 1; i <= aSeq->Length(); i++) 
    cout << "Found: " << Handle(SALOMEDSImpl_GenericAttribute)::DownCast(aSeq->Value(i))->Type() << endl;



  cout << "Check AttributeParameter "   << endl;

  Handle(SALOMEDSImpl_AttributeParameter) AP = Handle(SALOMEDSImpl_AttributeParameter)::DownCast(aTDFAttr);

  cout << "AttributeParameter with type : " << AP->Type() << endl;
  
  AP->SetInt("1", 123);
  cout << "IsSet for int: " << AP->IsSet("1", PT_INTEGER) << " value : " << AP->GetInt("1") << endl;
  //for(int i = 2; i < 5; i++) AP->SetInt(i, i*i);

  AP->SetReal("1", 123.123);
  cout << "IsSet for real: " << AP->IsSet("1", PT_REAL) << " value : " << AP->GetReal("1") << endl;
  //for(int i = 2; i < 5; i++) AP->SetReal(i, 0.1);

  AP->SetString("1", "value is 123.123!");
  cout << "IsSet for string: " << AP->IsSet("1", PT_STRING) << " value : " << AP->GetString("1") << endl; 
  /*
  for(int i = 2; i < 5; i++) {
    TCollection_AsciiString s((double)(1.0/i));
    cout << "Setting for " << i << " value : " << s  << endl;
    AP->SetString(i, s); 
  }
  */

  AP->SetBool("1", true);
  cout << "IsSet for bool: " << AP->IsSet("1", PT_BOOLEAN) << " value : " << AP->GetBool("1") << endl;
  //for(int i = 2; i < 5; i++) AP->SetBool(i, 0);
  
  vector<double> v;
  v.push_back(111.111);
  v.push_back(222.22222);
  v.push_back(333.3333333);
  AP->SetRealArray("1", v);
  cout << "IsSet for array: " << AP->IsSet("1", PT_REALARRAY);
  vector<double> v2 = AP->GetRealArray("1");
  cout.precision(10);
  cout << " values :  "; 
  for(int i = 0; i<v2.size(); i++) cout << v2[i] << " ";
  cout << endl;
  
  v[0] = 211.111;
  v[1] = 422.22222;
  v[2] = 633.3333333;
  AP->SetRealArray("2", v);

  vector<int> vi;
  vi.push_back(1);
  vi.push_back(2);
  AP->SetIntArray("2", vi);
  
  vector<string> vs;
  vs.push_back("hello, ");
  vs.push_back("world!");
  AP->SetStrArray("3", vs);        

  TCollection_AsciiString as = AP->Save();
  cout << "AS = " << as << endl;
  AP->Load(as);
  
  cout << "Restored string with id = 1 is: " << AP->GetString("1") << endl;
  cout << "Restored int with id = 2 is: " << AP->GetInt("1")  << endl;
  cout << "Restored real with id = 3 is: " << AP->GetReal("1")  << endl;
  cout << "Restored bool with id = 1 is: " << AP->GetBool("1")  << endl;
  
  v2 = AP->GetRealArray("2");
  cout << "Restored real array with id = 2 is: ";
  for(int i = 0; i<v2.size(); i++) cout << v2[i] << " ";
  cout << endl;

  vi = AP->GetIntArray("2");
  cout << "Restored int array with id = 2 is: ";
  for(int i = 0; i<vi.size(); i++) cout << vi[i] << " ";
  cout << endl;
  
  vs = AP->GetStrArray("3");
  cout << "Restored string array with id = 2 is: ";
  for(int i = 0; i<vs.size(); i++) cout << vs[i] << " ";
  cout << endl;

  cout << "Check RemoveID 1 with type PT_INTEGER" << endl;
  AP->RemoveID("1", PT_INTEGER);
  cout << "IsSet with id = 1, type = PT_INTEGER : " << AP->IsSet("1", PT_INTEGER)  << endl;
  cout << "Check RemoveID is done" << endl;
  
  cout << "Check AttributeParameter : done"   << endl;

  cout << "Test finished " << endl;    
  return 0;
}

