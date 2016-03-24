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

/*!
 * Check all methods of SALOMEDS_Study
 * Use code of SALOMEDS_Study.cxx
 */

void SALOMEDSTest::testStudy()
{
  //Create or find the Study manager
  _PTR(StudyManager) sm ( new SALOMEDS_StudyManager(_sm) );

  CPPUNIT_ASSERT(sm);

  //Create a new study
  _PTR(Study) study = sm->NewStudy("Test");

  //Check the creation of the study
  CPPUNIT_ASSERT(study);

  //Check method NewBuilder
  _PTR(StudyBuilder) studyBuilder = study->NewBuilder();
  CPPUNIT_ASSERT(studyBuilder);

  //Check method NewComponentIterator
  _PTR(SComponentIterator) componentIterator = study->NewComponentIterator();

  CPPUNIT_ASSERT(componentIterator);

  //Check method GetTransientReference
  CPPUNIT_ASSERT(!study->GetTransientReference().empty());

  //Check method StudyId
  CPPUNIT_ASSERT(study->StudyId() > 0);

  //Check method Name (get/set)
  CPPUNIT_ASSERT(study->Name() == "Test");
  study->Name("New name");
  CPPUNIT_ASSERT(study->Name() == "New name");
  study->Name("Test");

  //Check method URL (get/set)
  study->URL("");
  CPPUNIT_ASSERT(study->URL() == "");
  study->URL("some url");
  CPPUNIT_ASSERT(study->URL() == "some url");

  //Check method GetPersistentReference == URL
  CPPUNIT_ASSERT(study->GetPersistentReference() == "some url");

  //Check method IsSaved (get/set)
  study->IsSaved(true);
  CPPUNIT_ASSERT(study->IsSaved());
  study->IsSaved(false);
  CPPUNIT_ASSERT(!study->IsSaved());

  //Check method CreateObjectID
  _PTR(SObject) so = study->CreateObjectID("0:2:1:3");
  CPPUNIT_ASSERT(so);

  //Try to create SObject with empty and invalid entries
  //CPPUNIT_ASSERT(!study->CreateObjectID(""));
  CPPUNIT_ASSERT(!study->CreateObjectID("entry"));

  //Check method NewChildIterator
  _PTR(ChildIterator) childIterator = study->NewChildIterator(so);
  CPPUNIT_ASSERT(childIterator);

  //Check method IsEmpty
  CPPUNIT_ASSERT(!study->IsEmpty());

  //Check method FindComponent
  _PTR(SComponent) sco1 = studyBuilder->NewComponent("test1");
  _PTR(SComponent) sco2 = study->FindComponent("test1");
  CPPUNIT_ASSERT(sco1 && sco2);
  CPPUNIT_ASSERT(sco1->GetID() == sco2->GetID());
  _PTR(AttributeName) name_attr_sco1 = studyBuilder->FindOrCreateAttribute(sco1, "AttributeName");
  CPPUNIT_ASSERT(name_attr_sco1);
  name_attr_sco1->SetValue("sco1");

  //Try to find component with empty type
  CPPUNIT_ASSERT(!study->FindComponent(""));

  //Check method GetComponentNames
  std::vector<std::string> components = study->GetComponentNames(""); //The context doesn't matter
  CPPUNIT_ASSERT(components.size() == 1 && components[0] == "sco1");

  //Check method FindComponentID
  _PTR(SComponent) sco3 = study->FindComponentID(sco1->GetID());
  CPPUNIT_ASSERT(sco3 && sco3->GetID() == sco1->GetID());

  //Try to find component with empty id
  CPPUNIT_ASSERT(!study->FindComponentID(""));

  _PTR(SObject) so1 = studyBuilder->NewObject(sco1);
  CPPUNIT_ASSERT(so1);
  _PTR(AttributeName) name_attr_so1 = studyBuilder->FindOrCreateAttribute(so1, "AttributeName");
  CPPUNIT_ASSERT(name_attr_so1);
  name_attr_so1->SetValue("so1");
  
   //Create an attribute AttributeIOR
  _PTR(AttributeIOR) ior_attr_so1 = studyBuilder->FindOrCreateAttribute(so1, "AttributeIOR");
  CPPUNIT_ASSERT(ior_attr_so1);

  std::string ior = _orb->object_to_string(_sm);
  ior_attr_so1->SetValue(ior);
  
  _PTR(SObject) so2 = studyBuilder->NewObject(so1);
  CPPUNIT_ASSERT(so2);
  _PTR(AttributeName) name_attr_so2 = studyBuilder->FindOrCreateAttribute(so2, "AttributeName");
  CPPUNIT_ASSERT(name_attr_so2);
  name_attr_so2->SetValue("so2");

  //Check method FindObject
  _PTR(SObject) so3 = study->FindObject("so1");
  CPPUNIT_ASSERT(so3 && so3->GetID() == so1->GetID());

  //Try to find SObject with empty name
  CPPUNIT_ASSERT(!study->FindObject(""));

  //Check method FindObjectID
  _PTR(SObject) so4 = study->FindObjectID(so1->GetID());
  CPPUNIT_ASSERT(so4 && so4->GetID() == so1->GetID());

  //Try to find SObject with empty ID
  //CPPUNIT_ASSERT(!study->FindObjectID(""));

  //Check method FindObjectByName
  std::vector< _PTR(SObject) > v = study->FindObjectByName("so1", sco1->ComponentDataType());
  CPPUNIT_ASSERT(v.size()==1 && v[0]->GetID() == so1->GetID());

  //Try to find SObject with empty name and empty component type
  CPPUNIT_ASSERT((study->FindObjectByName("", "")).size() == 0);

  //Check method FindObjectByPath
  _PTR(SObject) path_so = study->FindObjectByPath("/"+sco1->GetName()+"/"+so1->GetName());
  CPPUNIT_ASSERT(path_so && path_so->GetID() == so1->GetID());

  //Check method FindObjectIOR
  _PTR(SObject) so5 = study->FindObjectIOR(ior);
  CPPUNIT_ASSERT(so5 && so5->GetID() == so1->GetID());

  //Try to find SObject with empty IOR
  CPPUNIT_ASSERT(!study->FindObjectIOR(""));

  //Check method GetObjectPath
  std::string path = study->GetObjectPath(so2);

  //Try to get path of NULL SObject
  _PTR(SObject) emptySO;
  path = study->GetObjectPath(emptySO);
  CPPUNIT_ASSERT(path.empty());

  //Check method SetContext
  study->SetContext("/sco1"); 
  CPPUNIT_ASSERT(study->GetContext() == "/sco1");

  //Check method FindObjectByPath
  _PTR(SObject) so6 = study->FindObjectByPath("so1");
  CPPUNIT_ASSERT(so6 && so6->GetID() == so1->GetID());

  
  //Try to find SObject with empty path
  _PTR(SObject) tmp = study->FindObjectByPath(""); //Must return the Context SObject
  CPPUNIT_ASSERT(tmp && tmp->GetID() == sco1->GetID());

  study->SetContext("/"); //Root

  //Check method GetObjectNames
  std::vector<std::string> vs = study->GetObjectNames("/sco1");  
  CPPUNIT_ASSERT(vs.size() == 2);
    
  //Check method GetDirectoryNames
  _PTR(AttributeLocalID) locid_attr_sco1 = studyBuilder->FindOrCreateAttribute(sco1, "AttributeLocalID");
  CPPUNIT_ASSERT(locid_attr_sco1);
  locid_attr_sco1->SetValue(16661); //DIRECTORYID
  _PTR(AttributeLocalID) locid_attr_so1 = studyBuilder->FindOrCreateAttribute(so1, "AttributeLocalID");
  CPPUNIT_ASSERT(locid_attr_so1);
  locid_attr_so1->SetValue(16661); //DIRECTORYID
  vs = study->GetDirectoryNames(""); //Empty context (the current is taken)
  CPPUNIT_ASSERT(vs.size() == 2);

  //Check method GetFileNames
  locid_attr_sco1->SetValue(26662); //FILELOCALID
  _PTR(AttributePersistentRef) persref_attr_sco1 = studyBuilder->FindOrCreateAttribute(sco1, "AttributePersistentRef");
  CPPUNIT_ASSERT(persref_attr_sco1);
  persref_attr_sco1->SetValue("FILE: filename1");
  locid_attr_so1->SetValue(26662); //FILELOCALID
  _PTR(AttributePersistentRef) persref_attr_so1 = studyBuilder->FindOrCreateAttribute(so1, "AttributePersistentRef");
  CPPUNIT_ASSERT(persref_attr_so1);
  persref_attr_so1->SetValue("FILE: filename2");
  vs = study->GetFileNames("");
  CPPUNIT_ASSERT(vs.size() == 2 && vs[0] == "filename1" && vs[1] == "filename2");

  //Check method StudyId (get/set)
  int id = study->StudyId();
  study->StudyId(-1);
  CPPUNIT_ASSERT(study->StudyId() == -1);
  study->StudyId(id);

  //Check method FindDependances
  studyBuilder->Addreference(so2, so1);
  studyBuilder->Addreference(sco1, so1);
  std::vector< _PTR(SObject) > vso = study->FindDependances(so1);
  CPPUNIT_ASSERT(vso.size() == 2 && vso[0]->GetID() == sco1->GetID() && vso[1]->GetID() == so2->GetID());

  //Check method GetProperties
  _PTR(AttributeStudyProperties) sp = study->GetProperties();
  CPPUNIT_ASSERT(sp);

  //Check Lock functionality
  /*sp->SetLocked(true);
  bool isLockRaised = false;
  try {
     name_attr_so1->SetValue("test");      
  }
  catch(...) {
    isLockRaised = true;
  }
  CPPUNIT_ASSERT(isLockRaised);
 
  sp->SetLocked(false);
  isLockRaised = false;
  try {
     name_attr_so1->SetValue("test");      
  }
  catch(...) {
    isLockRaised = true;
  }
  CPPUNIT_ASSERT(!isLockRaised);*/

  //Check method GetLastModificationDate
  sp->SetModification("srn", 1, 2, 3, 4, 5);  
  sp->SetModification("srn", 6, 7, 8, 9, 10);
  std::string date = study->GetLastModificationDate();  

  CPPUNIT_ASSERT(date == "08/09/0010 07:06");

  //Check method GetModificationsDate
  vs = study->GetModificationsDate();
  CPPUNIT_ASSERT(vs.size() == 2 && vs[0] == "03/04/0005 02:01" && vs[1] == "08/09/0010 07:06");

  //Check method GetCommonParameters
  _PTR(AttributeParameter) cp = study->GetCommonParameters("id", 1); //Save point = 1
  CPPUNIT_ASSERT(cp);
  cp->SetBool("test_true", true);
  cp->SetBool("test_false", false);
  _PTR(AttributeParameter) cp2 = study->GetCommonParameters("id", 1); //Save point = 1
  CPPUNIT_ASSERT(cp2);
  CPPUNIT_ASSERT(cp2->GetBool("test_true") && !cp2->GetBool("test_false"));
  _PTR(AttributeParameter) cp21 = study->GetCommonParameters("id", 2); //Save point = 2
  CPPUNIT_ASSERT(cp21);
  CPPUNIT_ASSERT(!cp21->IsSet("test_true", PT_BOOLEAN) && !cp21->IsSet("test_false", PT_BOOLEAN));

  //Check method GetModuleParameters
  _PTR(AttributeParameter) cp3 = study->GetModuleParameters("id", "module name", 1); //Save point = 1, name = "module name"
  CPPUNIT_ASSERT(cp3);
  cp3->SetBool("test_true", true);
  cp3->SetBool("test_false", false);
  _PTR(AttributeParameter) cp4 = study->GetModuleParameters("id", "module name", 1); //Save point = 1, name = "module name"
  CPPUNIT_ASSERT(cp4);
  CPPUNIT_ASSERT(cp4->GetBool("test_true") && !cp4->GetBool("test_false"));
  _PTR(AttributeParameter) cp5 = study->GetModuleParameters("id", "module name 2", 1); //Save point = 1, name = "module name 2"
  CPPUNIT_ASSERT(cp5);
  CPPUNIT_ASSERT(!cp5->IsSet("test_true", PT_BOOLEAN) && !cp5->IsSet("test_false", PT_BOOLEAN));
  _PTR(AttributeParameter) cp6 = study->GetModuleParameters("id", "module name", 2); //Save point = 2, name = "module name"
  CPPUNIT_ASSERT(cp6);
  CPPUNIT_ASSERT(!cp6->IsSet("test_true", PT_BOOLEAN) && !cp6->IsSet("test_false", PT_BOOLEAN));

  //Check method GetUseCaseBuilder
  _PTR(UseCaseBuilder) ub = study->GetUseCaseBuilder();
  CPPUNIT_ASSERT(ub);

  //Check method SetStudyLock
  study->SetStudyLock("locker1");

  //Check method IsStudyLocked
  CPPUNIT_ASSERT(study->IsStudyLocked());

  //Check method GetLockerID
  study->SetStudyLock("locker2");
  vs = study->GetLockerID();
  CPPUNIT_ASSERT(vs.size() == 2 && vs[0] == "locker1" && vs[1] == "locker2");

  //Check method UnLockStudy
  study->UnLockStudy("locker1");
  CPPUNIT_ASSERT(study->IsStudyLocked());
  vs = study->GetLockerID();
  CPPUNIT_ASSERT(vs.size() == 1 && vs[0] == "locker2");
  study->UnLockStudy("locker2");
  CPPUNIT_ASSERT(!study->IsStudyLocked());
  vs = study->GetLockerID();
  CPPUNIT_ASSERT(vs.size() == 0);

  //Check method EnableUseCaseAutoFilling
  study->EnableUseCaseAutoFilling(false);
  _PTR(SObject) uso1 = study->NewBuilder()->NewObject(sco1);
  std::vector< _PTR(GenericAttribute) > va1 = uso1->GetAllAttributes();
  CPPUNIT_ASSERT(va1.size() == 0); 

  study->EnableUseCaseAutoFilling(true);
  _PTR(SObject) uso2 = study->NewBuilder()->NewObject(sco1);
  std::vector< _PTR(GenericAttribute) > va2 = uso2->GetAllAttributes();
  CPPUNIT_ASSERT(va2.size() == 1); // +AttributeTreeNode

  //Check method DumpStudy
  study->DumpStudy(".", "SRN", false, false);

  std::fstream f("SRN.py");
  char buffer[128];
  buffer[81] = (char)0;
  for(int i=0;i<4;i++)
    f.getline(buffer, 80);
  std::string line(buffer);
 
  f.close();
  system("rm -f SRN.py");
  CPPUNIT_ASSERT(line.substr(0,50) == "### This file is generated automatically by SALOME");

  //Check method Close 
  bool isException = false;
  try {
    sm->Close(study);  //Close is called inside StudyManager::Close
  }
  catch(...) {
    isException = true;
  }
  CPPUNIT_ASSERT(!isException);
}



