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
 * Check all methods of SALOMEDS_AttributeTarget
 * Use code of SALOMEDS_AttributeTarget.cxx
 */
void SALOMEDSTest::testAttributeTarget()
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

  //Create an attribute AttributeTarget
  _PTR(AttributeTarget) _attr = studyBuilder->FindOrCreateAttribute(so, "AttributeTarget");

  //Check the attribute creation
  CPPUNIT_ASSERT(_attr);

  //Check method Get
  CPPUNIT_ASSERT(_attr->Get().size() == 0);

  _PTR(SObject) so1 = study->CreateObjectID("0:1:2");
  studyBuilder->Addreference(so1, so);

  _PTR(SObject) so2 = study->CreateObjectID("0:1:3");
  studyBuilder->Addreference(so2, so1);

  //Check method Add
  _attr->Add(so1);

  _attr->Add(so2);

  //Check method Get
  std::vector< _PTR(SObject) > v = _attr->Get();

  CPPUNIT_ASSERT(v.size() == 2);

  CPPUNIT_ASSERT(v[0]->GetID() == "0:1:2");

  CPPUNIT_ASSERT(v[1]->GetID() == "0:1:3");

  //Check method Remove
  _attr->Remove(so1);

  v = _attr->Get();

  CPPUNIT_ASSERT(v.size() == 1);

  CPPUNIT_ASSERT(v[0]->GetID() == "0:1:3");

  sm->Close(study);
}



