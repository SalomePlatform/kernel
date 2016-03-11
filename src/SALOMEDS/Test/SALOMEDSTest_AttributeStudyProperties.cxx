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
 * Check all methods of SALOMEDS_AttributeStudyProperties
 * Use code of SALOMEDS_AttributeStudyProperties.cxx
 */
void SALOMEDSTest::testAttributeStudyProperties()
{
  //Create or find the Study manager
  _PTR(StudyManager) sm ( new SALOMEDS_StudyManager(_sm) );

  CPPUNIT_ASSERT(sm);

  //Create a new study
  _PTR(Study) study = sm->NewStudy("Test");

  CPPUNIT_ASSERT(study);

  //Create Study Builder
  _PTR(StudyBuilder) studyBuilder = study->NewBuilder();

  CPPUNIT_ASSERT(studyBuilder);

  //Create a SObject with entry 0:1:1
  _PTR(SObject) so = study->CreateObjectID("0:1:1");

  CPPUNIT_ASSERT(so);

  //Create an attribute AttributeStudyProperties
  _PTR(AttributeStudyProperties) _attr = studyBuilder->FindOrCreateAttribute(so, "AttributeStudyProperties");

  //Check the attribute creation
  CPPUNIT_ASSERT(_attr);

  //Check method SetCreationDate
  _attr->SetCreationDate(1, 2, 3, 4, 5);

  //Check method GetCreationDate
  int MN, H, D, MH, Y;
  _attr->GetCreationDate(MN, H, D, MH, Y);

  CPPUNIT_ASSERT(MN == 1 && H == 2 && D == 3 && MH == 4 && Y == 5);

  //Check method SetUserName
  _attr->SetUserName("srn");

  //Check method GetUserName
  CPPUNIT_ASSERT(_attr->GetUserName() == "srn");

  //Check method SetCreationMode
  std::string value = "from scratch";
  _attr->SetCreationMode(value);

  //Check method GetCreationMode
  CPPUNIT_ASSERT(_attr->GetCreationMode() == value);

  //Check method SetModified
  _attr->SetModified(2);

  //Check method IsModified
  CPPUNIT_ASSERT(_attr->IsModified());

  //Check method GetModified
  CPPUNIT_ASSERT(_attr->GetModified() == 2);

  //Check method SetLocked
  _attr->SetLocked(true);

  //Check method IsLocked
  CPPUNIT_ASSERT(_attr->IsLocked());

  _attr->SetLocked(false);

  CPPUNIT_ASSERT(!_attr->IsLocked());

  //Check method SetModification
  _attr->SetModification("srn2", 6, 7, 8, 9, 10);

  //Check method GetModificationsList
  std::vector<std::string> vs;
  std::vector<int> vi[5];
  _attr->GetModificationsList(vs, vi[0], vi[1], vi[2], vi[3], vi[4], false);
  CPPUNIT_ASSERT(vs[0] == "srn2" && vi[0][0] == 6 && vi[1][0] == 7 && vi[2][0] == 8 && vi[3][0] == 9 && vi[4][0] == 10);

  vs.clear();
  for(int i=0; i<5; i++) vi[i].clear();

  _attr->GetModificationsList(vs, vi[0], vi[1], vi[2], vi[3], vi[4], true);
  CPPUNIT_ASSERT(vs[0] == "srn" && vi[0][0] == 1 && vi[1][0] == 2 && vi[2][0] == 3 && vi[3][0] == 4 && vi[4][0] == 5);

  sm->Close(study);
}



