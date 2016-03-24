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

//File:    testDS.cxx
//Author:  Sergey RUIN
//
#include <stdio.h>
#include <iostream> 
#include <vector>
#include <string>

#include "DF_Document.hxx"
#include "DF_Attribute.hxx"
#include "DF_Label.hxx"
#include "DF_ChildIterator.hxx"

#include "SALOMEDSImpl_Attributes.hxx"
#include "SALOMEDSImpl_StudyManager.hxx"
#include "SALOMEDSImpl_Study.hxx"
#include "SALOMEDSImpl_StudyBuilder.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_SComponent.hxx"
#include "SALOMEDSImpl_AttributeParameter.hxx"
#include "SALOMEDSImpl_UseCaseBuilder.hxx"
#include "SALOMEDSImpl_UseCaseIterator.hxx"

//#include "SALOMEDSImpl_.hxx"

int main (int argc, char * argv[])
{
  std::cout << "Test started " << std::endl;

  SALOMEDSImpl_StudyManager* aSM = new SALOMEDSImpl_StudyManager();
  std::cout << "Manager is created " << std::endl;
  SALOMEDSImpl_Study* aStudy = aSM->NewStudy("SRN");
  std::cout << "Study with id = " << aStudy->StudyId() << " is created " << std::endl; 

  std::cout << "Check the study lock, locking"   << std::endl;
  aStudy->SetStudyLock("SRN");
  std::cout << "Is study locked = " << aStudy->IsStudyLocked() << std::endl;
  std::vector<std::string> ids = aStudy->GetLockerID();
  for(int i = 0; i<ids.size(); i++)
    std::cout << "Get study locker : " << ids[i] << std::endl;
  aStudy->UnLockStudy("SRN");
  std::cout << "Is study locked = " << aStudy->IsStudyLocked()  << std::endl;

  SALOMEDSImpl_StudyBuilder* aBuilder = aStudy->NewBuilder();
  std::cout << "StudyBuilder is created " << std::endl;
  SALOMEDSImpl_SComponent aSC = aBuilder->NewComponent("TEST");
  std::cout << "New component with type " << aSC.ComponentDataType() << " is created " << std::endl;
  SALOMEDSImpl_SObject aSO = aBuilder->NewObject(aSC);
  std::cout << "New SObject with  ID = " << aSO.GetID() << " is created"  << std::endl;
  std::cout << "An entry of newly created SO is "  <<  aSO.GetLabel().Entry() << std::endl;
  SALOMEDSImpl_AttributeIOR* aIORA = SALOMEDSImpl_AttributeIOR::Set(aSO.GetLabel(), "ior1234");
  std::cout << "New AttributeIOR is created, it contains " << dynamic_cast<SALOMEDSImpl_AttributeIOR*>(aIORA)->Value() << std::endl;
  std::cout << "Attribute has type: " << aIORA->Type() << " and value: " << aIORA->Save() << std::endl; 
  std::cout << "Just another way to create an attribute: official one :) " << std::endl;
  std::cout << "Is SO null : " << aSO.IsNull()<< std::endl;
  DF_Attribute* aTDFAttr =  aBuilder->FindOrCreateAttribute(aSO, "AttributeName");  
  SALOMEDSImpl_AttributeName* aRN = dynamic_cast<SALOMEDSImpl_AttributeName*>(aTDFAttr);
  aRN->SetValue("name_attribute");
  std::cout << " The type = " << aRN->Type() << std::endl;
  std::cout << "Attribute has type: " << aRN->Type() << " and value: " << aRN->Save() << std::endl;   
  std::cout << "Check GetObjectPath: " << aStudy->GetObjectPath(aSO) << std::endl;
  
  SALOMEDSImpl_SObject aSubSO = aBuilder->NewObject(aSO);
  aTDFAttr =  aBuilder->FindOrCreateAttribute(aSubSO, "AttributeIOR");  
  SALOMEDSImpl_AttributeIOR* aIOR2 = dynamic_cast<SALOMEDSImpl_AttributeIOR*>(aTDFAttr);
  aIOR2->SetValue("some ior");
  aBuilder->Addreference(aSubSO, aSO);
  SALOMEDSImpl_SObject aRefObject;
  aSubSO.ReferencedObject(aRefObject);
  std::cout << "Check reference : ReferencedObject is " << aRefObject.GetID() << std::endl;
  std::cout << "Check : Remove object: " << std::endl;
  aBuilder->RemoveObject(aSubSO);
  std::cout << "Remove: done" << std::endl;

  std::cout << "Try invalid attribute creation" << std::endl;
  aTDFAttr = aBuilder->FindOrCreateAttribute(aSubSO, "invalid type");
  std::cout << "Address of created attribute : " << aTDFAttr << std::endl;

  std::cout << "Check AttributeUserID"   << std::endl;
  
  aTDFAttr = aBuilder->FindOrCreateAttribute(aSubSO, "AttributeUserID");
  if(aTDFAttr) {
    std::cout << "Attribute UserID was created succesfully : id = " << dynamic_cast<SALOMEDSImpl_AttributeUserID*>(aTDFAttr)->Value() << std::endl;
  }
  else std::cout << "Can't create AttributeUserID"   << std::endl;
  
  std::string id = "0e1c36e6-379b-4d90-ab3b-17a14310e648";
  dynamic_cast<SALOMEDSImpl_AttributeUserID*>(aTDFAttr)->SetValue(id);
  std::cout << "SetValue id = " << dynamic_cast<SALOMEDSImpl_AttributeUserID*>(aTDFAttr)->Value()  << std::endl;

  std::string id2 = "0e1c36e6-379b-4d90-ab3b-18a14310e648";
  aTDFAttr = aBuilder->FindOrCreateAttribute(aSubSO, "AttributeUserID"+id2);
  if(aTDFAttr) {
    std::cout << "Attribute UserID was created succesfully : id = " << dynamic_cast<SALOMEDSImpl_AttributeUserID*>(aTDFAttr)->Value() << std::endl;
  }
  else std::cout << "Can't create AttributeUserID"   << std::endl;

  std::cout << "Check AttributeTreeNode " << std::endl;
  aTDFAttr =  aBuilder->FindOrCreateAttribute(aSO, "AttributeTreeNode");  
  std::cout << dynamic_cast<SALOMEDSImpl_GenericAttribute*>(aTDFAttr)->Type() << std::endl;
  std::cout << "Check AttributeTreeNode : done " << std::endl;

  aTDFAttr =  aBuilder->FindOrCreateAttribute(aSO, "AttributeParameter");  
  std::cout << dynamic_cast<SALOMEDSImpl_GenericAttribute*>(aTDFAttr)->Type() << std::endl;

  std::cout << "Check the attributes on SObject" << std::endl;
  std::vector<DF_Attribute*> aSeq = aSO.GetAllAttributes();
  for(int i = 0; i < aSeq.size(); i++) 
    std::cout << "Found: " << dynamic_cast<SALOMEDSImpl_GenericAttribute*>(aSeq[i])->Type() << std::endl;


  std::cout << "Check UseCase"   << std::endl;
  SALOMEDSImpl_UseCaseBuilder* ucb = aStudy->GetUseCaseBuilder();
  ucb->AddUseCase("use_case1");
  ucb->AddUseCase("use_case2");
  SALOMEDSImpl_UseCaseIterator ucitr = ucb->GetUseCaseIterator(SALOMEDSImpl_SObject());
  ucitr.Init(false);
  std::cout << "More? : " << ucitr.More() << std::endl;

  std::cout << "Check AttributeParameter "   << std::endl;

  SALOMEDSImpl_AttributeParameter* AP = dynamic_cast<SALOMEDSImpl_AttributeParameter*>(aTDFAttr);

  std::cout << "AttributeParameter with type : " << AP->Type() << std::endl;
  
  AP->SetInt("1", 123);
  std::cout << "IsSet for int: " << AP->IsSet("1", PT_INTEGER) << " value : " << AP->GetInt("1") << std::endl;
  //for(int i = 2; i < 5; i++) AP->SetInt(i, i*i);

  AP->SetReal("1", 123.123);
  std::cout << "IsSet for real: " << AP->IsSet("1", PT_REAL) << " value : " << AP->GetReal("1") << std::endl;
  //for(int i = 2; i < 5; i++) AP->SetReal(i, 0.1);

  AP->SetString("1", "value is 123.123!");
  std::cout << "IsSet for string: " << AP->IsSet("1", PT_STRING) << " value : " << AP->GetString("1") << std::endl; 
  /*
  for(int i = 2; i < 5; i++) {
    std::string s((double)(1.0/i));
    std::cout << "Setting for " << i << " value : " << s  << std::endl;
    AP->SetString(i, s); 
  }
  */

  AP->SetBool("1", true);
  std::cout << "IsSet for bool: " << AP->IsSet("1", PT_BOOLEAN) << " value : " << AP->GetBool("1") << std::endl;
  //for(int i = 2; i < 5; i++) AP->SetBool(i, 0);
  
  std::vector<double> v;
  v.push_back(111.111);
  v.push_back(222.22222);
  v.push_back(333.3333333);
  AP->SetRealArray("1", v);
  std::cout << "IsSet for array: " << AP->IsSet("1", PT_REALARRAY);
  std::vector<double> v2 = AP->GetRealArray("1");
  std::cout.precision(10);
  std::cout << " values :  "; 
  for(int i = 0; i<v2.size(); i++) std::cout << v2[i] << " ";
  std::cout << std::endl;
  
  v[0] = 211.111;
  v[1] = 422.22222;
  v[2] = 633.3333333;
  AP->SetRealArray("2", v);

  std::vector<int> vi;
  vi.push_back(1);
  vi.push_back(2);
  AP->SetIntArray("2", vi);
  
  std::vector<std::string> vs;
  vs.push_back("hello, ");
  vs.push_back("world!");
  AP->SetStrArray("3", vs);        

  std::string as = AP->Save();
  std::cout << "AS = " << as << std::endl;
  AP->Load(as);
  
  std::cout << "Restored string with id = 1 is: " << AP->GetString("1") << std::endl;
  std::cout << "Restored int with id = 2 is: " << AP->GetInt("1")  << std::endl;
  std::cout << "Restored real with id = 3 is: " << AP->GetReal("1")  << std::endl;
  std::cout << "Restored bool with id = 1 is: " << AP->GetBool("1")  << std::endl;
  
  v2 = AP->GetRealArray("2");
  std::cout << "Restored real array with id = 2 is: ";
  for(int i = 0; i<v2.size(); i++) std::cout << v2[i] << " ";
  std::cout << std::endl;

  vi = AP->GetIntArray("2");
  std::cout << "Restored int array with id = 2 is: ";
  for(int i = 0; i<vi.size(); i++) std::cout << vi[i] << " ";
  std::cout << std::endl;
  
  vs = AP->GetStrArray("3");
  std::cout << "Restored string array with id = 2 is: ";
  for(int i = 0; i<vs.size(); i++) std::cout << vs[i] << " ";
  std::cout << std::endl;

  std::cout << "Check RemoveID 1 with type PT_INTEGER" << std::endl;
  AP->RemoveID("1", PT_INTEGER);
  std::cout << "IsSet with id = 1, type = PT_INTEGER : " << AP->IsSet("1", PT_INTEGER)  << std::endl;
  std::cout << "Check RemoveID is done" << std::endl;
  
  std::cout << "Check AttributeParameter : done"   << std::endl;

  
  SALOMEDSImpl_SComponent tst = aBuilder->NewComponent("TEST2");
  aSO = aBuilder->NewObject(tst);
  SALOMEDSImpl_SObject ss = aBuilder->NewObjectToTag(aSO, 3);
  aBuilder->NewObjectToTag(ss, 1);
  aBuilder->NewObjectToTag(ss, 3);
  SALOMEDSImpl_SObject ss2 = aBuilder->NewObjectToTag(aSO, 2);
  aBuilder->NewObjectToTag(ss, 2);

  SALOMEDSImpl_ChildIterator ci=aStudy->NewChildIterator(tst);
  for(ci.InitEx(true); ci.More(); ci.Next())
    std::cout << "######## " << ci.Value().GetID() << std::endl;

  DF_ChildIterator dci(tst.GetLabel(), true);
  for(; dci.More(); dci.Next()) 
    std::cout << "###### DF:  " << dci.Value().Entry() << std::endl;

  std::cout << "Test finished " << std::endl;    
  return 0;
}

