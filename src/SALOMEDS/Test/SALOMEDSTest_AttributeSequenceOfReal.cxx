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
 * Check all methods of SALOMEDS_AttributeSequenceOfReal
 * Use code of SALOMEDS_AttributeSequenceOfReal.cxx
 */
void SALOMEDSTest::testAttributeSequenceOfReal()
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

  //Create an attribute AttributeSequenceOfReal
  _PTR(AttributeSequenceOfReal) _attr = studyBuilder->FindOrCreateAttribute(so, "AttributeSequenceOfReal");

  //Check the attribute creation
  CPPUNIT_ASSERT(_attr);

  //Check method Length
  CPPUNIT_ASSERT(_attr->Length() == 0);

  //Check method Add
  _attr->Add(1.1);

  _attr->Add(2.2);

  _attr->Add(3.3);

  _attr->Add(4.4);

  CPPUNIT_ASSERT(_attr->Length() == 4);

  CPPUNIT_ASSERT(_attr->Value(2) == 2.2);

  //Check method Remove
  _attr->Remove(3);

  CPPUNIT_ASSERT(_attr->Value(3) == 4.4);

  //Check method ChangeValue
  _attr->ChangeValue(3, 3.3);

  CPPUNIT_ASSERT(_attr->Value(3) == 3.3);

  //Check method CorbaSequence
  std::vector<double> v = _attr->CorbaSequence();
  CPPUNIT_ASSERT(v.size() == 3);

  v.push_back(5.5);

  _attr->Assign(v);

  CPPUNIT_ASSERT(_attr->Length() == 4);

  CPPUNIT_ASSERT(_attr->Value(4) == 5.5);

  //Check processing of invalid indicies
  bool isRaised = false;
  try {
    _attr->Value(-1);
  }
  catch(...) {
    isRaised = true;
  }
  CPPUNIT_ASSERT(isRaised);
  isRaised = false;
  try {
    _attr->ChangeValue(12, 1);
  }
  catch(...) {
    isRaised = true;
  }
  CPPUNIT_ASSERT(isRaised);
  isRaised = false;
  try {
    _attr->Remove(10);
  }
  catch(...) {
    isRaised = true;
  }
  CPPUNIT_ASSERT(isRaised);

  sm->Close(study);
}



