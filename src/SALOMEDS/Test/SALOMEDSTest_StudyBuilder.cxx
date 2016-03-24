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
 * Check all methods of SALOMEDS_StudyBuilder
 * Use code of SALOMEDS_StudyBuilder.cxx
 */

void SALOMEDSTest::testStudyBuilder()
{
  //Create or find the Study manager
  _PTR(StudyManager) sm ( new SALOMEDS_StudyManager(_sm) );

  CPPUNIT_ASSERT(sm);

  //Create a new study
  _PTR(Study) study = sm->NewStudy("TestStudyBuilder");

  CPPUNIT_ASSERT(study);

  //Create Study Builder
  _PTR(StudyBuilder) studyBuilder = study->NewBuilder();

  //Check the StudyBuilder creation
  CPPUNIT_ASSERT(studyBuilder);

  //Check method NewComponent
  _PTR(SComponent) sco1 = studyBuilder->NewComponent("Test");
  CPPUNIT_ASSERT(sco1 && sco1->ComponentDataType() == "Test");

  //Check method DefineComponentInstance
  std::string ior = _orb->object_to_string(_sm);
  studyBuilder->DefineComponentInstance(sco1, ior);
  std::string newior;
  sco1->ComponentIOR(newior);
  CPPUNIT_ASSERT(newior == ior);

  //Check method RemoveComponent
  studyBuilder->RemoveComponent(sco1);
  _PTR(SComponent) sco2 = study->FindComponent("Test");
  CPPUNIT_ASSERT(!sco2);

  //Try to create and find the component with empty type
  _PTR(SComponent) sco_empty = studyBuilder->NewComponent(""); 
  CPPUNIT_ASSERT(!sco_empty);

  _PTR(SComponent) sco3 = studyBuilder->NewComponent("NewComp");
  CPPUNIT_ASSERT(sco3);

  //Check method NewObject
  _PTR(SObject) so1 = studyBuilder->NewObject(sco3);
  CPPUNIT_ASSERT(so1);
  std::string id1 = so1->GetID();

  //Check method NewObjectToTag
  _PTR(SObject) so2 = studyBuilder->NewObjectToTag(so1, 2);
  CPPUNIT_ASSERT(so2 && so2->Tag() == 2);
  std::string id2 = so2->GetID();

  //Check method FindOrCreateAttribute
  _PTR(SObject) so3 = studyBuilder->NewObject(sco3);
  CPPUNIT_ASSERT(so3);
  _PTR(AttributeName) an3 = studyBuilder->FindOrCreateAttribute(so3, "AttributeName");
  CPPUNIT_ASSERT(an3);

  //Try to create attribute with invalid type
  CPPUNIT_ASSERT(!studyBuilder->FindOrCreateAttribute(so3, "invalid type"));

  //Check method FindAttribute
  _PTR(GenericAttribute) ga;
  CPPUNIT_ASSERT(studyBuilder->FindAttribute(so3, ga, "AttributeName"));

  //Try to find attribute with invalid type
  CPPUNIT_ASSERT(!studyBuilder->FindAttribute(so3, ga, "invalid type"));

  //Check method RemoveObject
  studyBuilder->RemoveObject(so3);
  CPPUNIT_ASSERT(!studyBuilder->FindAttribute(so3, ga, "AttributeName"));

  //Check method RemoveObjectWithChildren
  _PTR(AttributeName) an2 = studyBuilder->FindOrCreateAttribute(so2, "AttributeName");
  CPPUNIT_ASSERT(an2);
  studyBuilder->RemoveObjectWithChildren(so1);
  CPPUNIT_ASSERT(!studyBuilder->FindAttribute(so2, ga, "AttributeName"));

  //Check method RemoveAttribute
  _PTR(AttributeName) an1 = studyBuilder->FindOrCreateAttribute(so1, "AttributeName");
  CPPUNIT_ASSERT(an1);
  CPPUNIT_ASSERT(studyBuilder->FindAttribute(so1, ga, "AttributeName"));
  studyBuilder->RemoveAttribute(so1, "AttributeName");
  CPPUNIT_ASSERT(!studyBuilder->FindAttribute(so1, ga, "AttributeName"));

  //Check method Addreference
  studyBuilder->Addreference(so2, so1);
  _PTR(SObject) refSO;
  CPPUNIT_ASSERT(so2->ReferencedObject(refSO) && refSO->GetID() == so1->GetID());

  //Try to set reference to NULL SObject
  bool isRaised = false;
  _PTR(SObject) empty_so;
  try {
    studyBuilder->Addreference(so2, empty_so);
  }
  catch(...) {
    isRaised = true;
  }
  CPPUNIT_ASSERT(isRaised);

  //Check method RemoveReference
  studyBuilder->RemoveReference(so2);
  CPPUNIT_ASSERT(!so2->ReferencedObject(refSO));

  //Check method SetGUID and IsGUID
  std::string value = "0e1c36e6-379b-4d90-ab3b-17a14310e648";
  studyBuilder->SetGUID(so1, value);

  CPPUNIT_ASSERT(studyBuilder->IsGUID(so1, value));

/* Not implemented
  //Check method UndoLimit (set/get)
  studyBuilder->UndoLimit(10);
  CPPUNIT_ASSERT(studyBuilder->UndoLimit() == 10);
*/

  //Check method SetName
  studyBuilder->SetName(so1, "new name");
  CPPUNIT_ASSERT(so1->GetName() == "new name");

  //Try to set empty Name
  studyBuilder->SetName(so1, "");
  CPPUNIT_ASSERT(so1->GetName() == "");

  //Check method SetComment
  studyBuilder->SetComment(so1, "new comment");
  CPPUNIT_ASSERT(so1->GetComment() == "new comment");

  //Check method empty Comment
  studyBuilder->SetComment(so1, "");
  CPPUNIT_ASSERT(so1->GetComment() == "");

  //Try to set empty IOR
  studyBuilder->SetIOR(so1, "");
  CPPUNIT_ASSERT(so1->GetIOR() == "");

  //Check method SetIOR
  studyBuilder->SetIOR(so1, ior);
  CPPUNIT_ASSERT(so1->GetIOR() == ior);

  sm->Close(study);

  //Check method LoadWith
  _PTR(Study) study2 = sm->NewStudy("Study2");
  
  SALOME_NamingService NS(_orb);
  CORBA::Object_var obj = SALOME_LifeCycleCORBA(&NS).FindOrLoad_Component("FactoryServer", "SMESH");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));

  MESSAGE("Created a new SMESH component");

  SALOMEDS::Driver_var drv = SALOMEDS::Driver::_narrow(obj);
  CPPUNIT_ASSERT(!CORBA::is_nil(drv));
 
  _PTR(StudyBuilder) sb2 = study2->NewBuilder();
  _PTR(SComponent) sco = sb2->NewComponent("SMESH");
  
  ior = _orb->object_to_string(drv);
  sb2->DefineComponentInstance(sco, ior);

  sm->SaveAs("srn_SALOMEDS_UnitTests.hdf", study2, false);
  sm->Close(study2);

  _PTR(Study) study3 = sm->Open("srn_SALOMEDS_UnitTests.hdf");
  _PTR(StudyBuilder) sb3 = study3->NewBuilder();
  _PTR(SComponent) aComp = study3->FindComponent("SMESH");
  CPPUNIT_ASSERT(aComp);

  CORBA::Object_var obj2 = SALOME_LifeCycleCORBA(&NS).FindOrLoad_Component("FactoryServer", "SMESH");
  CPPUNIT_ASSERT(!CORBA::is_nil(obj2));
  SALOMEDS::Driver_var drv2 = SALOMEDS::Driver::_narrow(obj2);
  ior = _orb->object_to_string(drv2);

  isRaised = false;
  try {
    //getchar();
    sb3->LoadWith(aComp, ior);
  }
  catch(...) {
    isRaised = true;
  }


  CPPUNIT_ASSERT(!isRaised);

  ior = "";
  aComp->ComponentIOR(ior);
  CPPUNIT_ASSERT(!ior.empty());

  system("rm -f srn_SALOMEDS_UnitTests.hdf");

  //Check method AddDirectory
  _PTR(AttributeName) na1 = sb3->FindOrCreateAttribute(aComp, "AttributeName");
  na1->SetValue("Component");

  isRaised = false;
  try {
    sb3->AddDirectory("/Component/Dir1");
  } catch(...) {
    isRaised = true;
  }


  CPPUNIT_ASSERT(!isRaised);
  _PTR(SObject) so5 = study3->FindObjectByPath("/Component/Dir1");
  CPPUNIT_ASSERT(so5);

  isRaised = false;
  try {
    sb3->AddDirectory("/Component/Dir1"); //Attempt to create the same directory
  } catch(...) {
    isRaised = true;
  }
  CPPUNIT_ASSERT(isRaised);

  isRaised = false;
  try {
    sb3->AddDirectory("/MyComponent/Dir1"); //Attempt to create the invalid directory
  } catch(...) {
    isRaised = true;
  }
  CPPUNIT_ASSERT(isRaised);

  sm->Close(study3);  
}
