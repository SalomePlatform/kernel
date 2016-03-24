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
 * Check all methods of SALOMEDS_StudyManager
 * Use code of SALOMEDS_StudyManager.cxx
 */
void SALOMEDSTest::testStudyManager()
{
  //Create or find the Study manager
  _PTR(StudyManager) sm ( new SALOMEDS_StudyManager(_sm) );

  CPPUNIT_ASSERT(sm);

  //Check method NewStudy
  _PTR(Study) study1 = sm->NewStudy("Study1");
  CPPUNIT_ASSERT(study1);

  //Check method GetStudyByName
  _PTR(Study) study3 = sm->GetStudyByName("Study1");
  CPPUNIT_ASSERT(study3->StudyId() == study1->StudyId());
  CPPUNIT_ASSERT(study3->Name() == study1->Name());

  //Check method GetOpenStudies
  std::vector<std::string> v = sm->GetOpenStudies();
  CPPUNIT_ASSERT(v.size() == 1);

  //Check method GetStudyByID for invalid ID
  CPPUNIT_ASSERT(!sm->GetStudyByID(-1));

  //Check methods CanPaste, CanCopy, Copy, Paste
  _PTR(StudyBuilder) sb1 = study1->NewBuilder();
  _PTR(SComponent) sco1 = sb1->NewComponent("Test");
  _PTR(SObject) so1 = sb1->NewObject(sco1);
  _PTR(AttributeName) na1 = sb1->FindOrCreateAttribute(so1, "AttributeName");
  CPPUNIT_ASSERT(na1);
  na1->SetValue("Some name");

  CPPUNIT_ASSERT(!sm->CanCopy(so1)); //The father component has no IOR
  
  CPPUNIT_ASSERT(sm->Copy(so1));

  CPPUNIT_ASSERT(!sm->CanPaste(so1)); //The father component has no IOR

  _PTR(SObject) so1_2 = sb1->NewObject(sco1);
  _PTR(SObject) pasted_so = sm->Paste(so1_2);
  CPPUNIT_ASSERT(pasted_so);

  _PTR(AttributeName) na2 = sb1->FindOrCreateAttribute(pasted_so, "AttributeName");
  CPPUNIT_ASSERT(na2 && na2->Value() == "Some name");


  //Check method SaveAs
  sm->SaveAs("srn_UnitTest_Save.hdf", study1, false);
  std::string url = study1->URL();
  sm->Close(study1);

  //Check method Open
  _PTR(Study) study1_opened = sm->Open("srn_UnitTest_Save.hdf");  //Contains Test component
  system("rm -f srn_UnitTest_Save.hdf");
  url = study1_opened->URL();
  CPPUNIT_ASSERT(study1_opened);
  CPPUNIT_ASSERT(url == "srn_UnitTest_Save.hdf");

  //Check method Save
  _PTR(StudyBuilder) sb3 = study1_opened->NewBuilder();
  _PTR(SComponent) sco3 = study1_opened->FindComponent("Test");
  CPPUNIT_ASSERT(sco3);
  //   Add a new SObject with AttributeName that contains "Saved study" string
  _PTR(SObject) so3 = sb3->NewObject(sco3);
  std::string soID = so3->GetID();
  _PTR(AttributeName) na3 = sb3->FindOrCreateAttribute(so3, "AttributeName");
  CPPUNIT_ASSERT(na3);
 
  na3->SetValue("Saved study");

  //   Save and close the study
  sm->Save(study1_opened, false);

  sm->Close(study1_opened);

  //  Open saved study and find the created SObject with AttributeName, then compare the stored string
  _PTR(Study) study2_opened = sm->Open("srn_UnitTest_Save.hdf");

  system("rm -f srn_UnitTest_Save.hdf");

  CPPUNIT_ASSERT(study2_opened);

  _PTR(SObject) so4 = study2_opened->CreateObjectID(soID);
  _PTR(StudyBuilder) sb4 = study2_opened->NewBuilder();
  _PTR(AttributeName) na4 = sb4->FindOrCreateAttribute(so4, "AttributeName");
  CPPUNIT_ASSERT(na4 && na4->Value() == "Saved study"); //Compare the value of restored attribute with string that has to be saved.
 
  //Check method SaveAsASCII
  sm->SaveAsASCII("srn_UnitTest_SaveASCII.hdf", study2_opened, false);
  url = study2_opened->URL();
  sm->Close(study2_opened);

  _PTR(Study) study3_opened = sm->Open("srn_UnitTest_SaveASCII.hdf");  //Contains Test component
  system("rm -f srn_UnitTest_SaveASCII.hdf");
  CPPUNIT_ASSERT(study3_opened);
  CPPUNIT_ASSERT(url == "srn_UnitTest_SaveASCII.hdf");

  //Check method SaveASCII
  _PTR(StudyBuilder) sb5 = study3_opened->NewBuilder();
  _PTR(SComponent) sco5 = study3_opened->FindComponent("Test");
  CPPUNIT_ASSERT(sco5);
  //   Add a new SObject with AttributeName that contains "Saved study" string
  _PTR(SObject) so5 = sb5->NewObject(sco5);
  soID = so5->GetID();
  _PTR(AttributeName) na5 = sb5->FindOrCreateAttribute(so5, "AttributeName");
  CPPUNIT_ASSERT(na5);
  na5->SetValue("Saved study ASCII");
  //   Save and close the study
  sm->Save(study3_opened, false);
  sm->Close(study3_opened);

  //  Open saved study and find the created SObject with AttributeName, then compare the stored string
  _PTR(Study) study4_opened = sm->Open("srn_UnitTest_SaveASCII.hdf");
  system("rm -f srn_UnitTest_SaveASCII.hdf");
  CPPUNIT_ASSERT(study4_opened);
  _PTR(SObject) so6 = study4_opened->CreateObjectID(soID);
  _PTR(StudyBuilder) sb6 = study4_opened->NewBuilder();
  _PTR(AttributeName) na6 = sb6->FindOrCreateAttribute(so6, "AttributeName");
  CPPUNIT_ASSERT(na6 && na6->Value() == "Saved study ASCII"); //Compare the value of restored attribute with string that has to be saved.
}



