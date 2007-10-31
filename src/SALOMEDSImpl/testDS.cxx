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

using namespace std;

int main (int argc, char * argv[])
{
  cout << "Test started " << endl;

  SALOMEDSImpl_StudyManager* aSM = new SALOMEDSImpl_StudyManager();
  cout << "Manager is created " << endl;
  SALOMEDSImpl_Study* aStudy = aSM->NewStudy("SRN");
  cout << "Study with id = " << aStudy->StudyId() << " is created " << endl; 

  cout << "Check the study lock, locking"   << endl;
  aStudy->SetStudyLock("SRN");
  cout << "Is study locked = " << aStudy->IsStudyLocked() << endl;
  vector<string> ids = aStudy->GetLockerID();
  for(int i = 0; i<ids.size(); i++)
    cout << "Get study locker : " << ids[i] << endl;
  aStudy->UnLockStudy("SRN");
  cout << "Is study locked = " << aStudy->IsStudyLocked()  << endl;

  SALOMEDSImpl_StudyBuilder* aBuilder = aStudy->NewBuilder();
  cout << "StudyBuilder is created " << endl;
  SALOMEDSImpl_SComponent aSC = aBuilder->NewComponent("TEST");
  cout << "New component with type " << aSC.ComponentDataType() << " is created " << endl;
  SALOMEDSImpl_SObject aSO = aBuilder->NewObject(aSC);
  cout << "New SObject with  ID = " << aSO.GetID() << " is created"  << endl;
  cout << "An entry of newly created SO is "  <<  aSO.GetLabel().Entry() << endl;
  SALOMEDSImpl_AttributeIOR* aIORA = SALOMEDSImpl_AttributeIOR::Set(aSO.GetLabel(), "ior1234");
  cout << "New AttributeIOR is created, it contains " << dynamic_cast<SALOMEDSImpl_AttributeIOR*>(aIORA)->Value() << endl;
  cout << "Attribute has type: " << aIORA->Type() << " and value: " << aIORA->Save() << endl; 
  cout << "Just another way to create an attribute: official one :) " << endl;
  cout << "Is SO null : " << aSO.IsNull()<< endl;
  DF_Attribute* aTDFAttr =  aBuilder->FindOrCreateAttribute(aSO, "AttributeName");  
  SALOMEDSImpl_AttributeName* aRN = dynamic_cast<SALOMEDSImpl_AttributeName*>(aTDFAttr);
  aRN->SetValue("name_attribute");
  cout << " The type = " << aRN->Type() << endl;
  cout << "Attribute has type: " << aRN->Type() << " and value: " << aRN->Save() << endl;   
  cout << "Check GetObjectPath: " << aStudy->GetObjectPath(aSO) << endl;
  
  SALOMEDSImpl_SObject aSubSO = aBuilder->NewObject(aSO);
  aTDFAttr =  aBuilder->FindOrCreateAttribute(aSubSO, "AttributeIOR");  
  SALOMEDSImpl_AttributeIOR* aIOR2 = dynamic_cast<SALOMEDSImpl_AttributeIOR*>(aTDFAttr);
  aIOR2->SetValue("some ior");
  aBuilder->Addreference(aSubSO, aSO);
  SALOMEDSImpl_SObject aRefObject;
  aSubSO.ReferencedObject(aRefObject);
  cout << "Check reference : ReferencedObject is " << aRefObject.GetID() << endl;
  cout << "Check : Remove object: " << endl;
  aBuilder->RemoveObject(aSubSO);
  cout << "Remove: done" << endl;

  cout << "Try invalid attribute creation" << endl;
  aTDFAttr = aBuilder->FindOrCreateAttribute(aSubSO, "invalid type");
  cout << "Address of created attribute : " << aTDFAttr << endl;

  cout << "Check AttributeUserID"   << endl;
  
  aTDFAttr = aBuilder->FindOrCreateAttribute(aSubSO, "AttributeUserID");
  if(aTDFAttr) {
    cout << "Attribute UserID was created succesfully : id = " << dynamic_cast<SALOMEDSImpl_AttributeUserID*>(aTDFAttr)->Value() << endl;
  }
  else cout << "Can't create AttributeUserID"   << endl;
  
  string id = "0e1c36e6-379b-4d90-ab3b-17a14310e648";
  dynamic_cast<SALOMEDSImpl_AttributeUserID*>(aTDFAttr)->SetValue(id);
  cout << "SetValue id = " << dynamic_cast<SALOMEDSImpl_AttributeUserID*>(aTDFAttr)->Value()  << endl;

  string id2 = "0e1c36e6-379b-4d90-ab3b-18a14310e648";
  aTDFAttr = aBuilder->FindOrCreateAttribute(aSubSO, "AttributeUserID"+id2);
  if(aTDFAttr) {
    cout << "Attribute UserID was created succesfully : id = " << dynamic_cast<SALOMEDSImpl_AttributeUserID*>(aTDFAttr)->Value() << endl;
  }
  else cout << "Can't create AttributeUserID"   << endl;

  cout << "Check AttributeTreeNode " << endl;
  aTDFAttr =  aBuilder->FindOrCreateAttribute(aSO, "AttributeTreeNode");  
  cout << dynamic_cast<SALOMEDSImpl_GenericAttribute*>(aTDFAttr)->Type() << endl;
  cout << "Check AttributeTreeNode : done " << endl;

  aTDFAttr =  aBuilder->FindOrCreateAttribute(aSO, "AttributeParameter");  
  cout << dynamic_cast<SALOMEDSImpl_GenericAttribute*>(aTDFAttr)->Type() << endl;

  cout << "Check the attributes on SObject" << endl;
  vector<DF_Attribute*> aSeq = aSO.GetAllAttributes();
  for(int i = 0; i < aSeq.size(); i++) 
    cout << "Found: " << dynamic_cast<SALOMEDSImpl_GenericAttribute*>(aSeq[i])->Type() << endl;


  cout << "Check UseCase"   << endl;
  SALOMEDSImpl_UseCaseBuilder* ucb = aStudy->GetUseCaseBuilder();
  ucb->AddUseCase("use_case1");
  ucb->AddUseCase("use_case2");
  SALOMEDSImpl_UseCaseIterator ucitr = ucb->GetUseCaseIterator(SALOMEDSImpl_SObject());
  ucitr.Init(false);
  cout << "More? : " << ucitr.More() << endl;

  cout << "Check AttributeParameter "   << endl;

  SALOMEDSImpl_AttributeParameter* AP = dynamic_cast<SALOMEDSImpl_AttributeParameter*>(aTDFAttr);

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
    string s((double)(1.0/i));
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

  string as = AP->Save();
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

  
  SALOMEDSImpl_SComponent tst = aBuilder->NewComponent("TEST2");
  aSO = aBuilder->NewObject(tst);
  SALOMEDSImpl_SObject ss = aBuilder->NewObjectToTag(aSO, 3);
  aBuilder->NewObjectToTag(ss, 1);
  aBuilder->NewObjectToTag(ss, 3);
  SALOMEDSImpl_SObject ss2 = aBuilder->NewObjectToTag(aSO, 2);
  aBuilder->NewObjectToTag(ss, 2);

  SALOMEDSImpl_ChildIterator ci=aStudy->NewChildIterator(tst);
  for(ci.InitEx(true); ci.More(); ci.Next())
    cout << "######## " << ci.Value().GetID() << endl;

  DF_ChildIterator dci(tst.GetLabel(), true);
  for(; dci.More(); dci.Next()) 
    cout << "###### DF:  " << dci.Value().Entry() << endl;

  cout << "Test finished " << endl;    
  return 0;
}

