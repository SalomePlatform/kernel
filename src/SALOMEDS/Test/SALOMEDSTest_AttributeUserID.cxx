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
 * Check all methods of SALOMEDS_AttributeUserID
 * Use code of SALOMEDS_AttributeUserID.cxx
 */
void SALOMEDSTest::testAttributeUserID()
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

  //Create an attribute AttributeUserID
  _PTR(AttributeUserID) _attr = studyBuilder->FindOrCreateAttribute(so, "AttributeUserID");

  //Check the attribute creation 
  CPPUNIT_ASSERT(_attr);

  std::string value = "0e1c36e6-379b-4d90-ab3b-17a14310e648";

  //Check method SetValue
  _attr->SetValue(value);

  //Check method Value
  CPPUNIT_ASSERT(value == _attr->Value());

  //Try to create the attribute with given UserID
  value = "0e1c36e6-379b-4d90-ab3b-18a14310e648";
  _PTR(AttributeUserID) _attr2 = studyBuilder->FindOrCreateAttribute(so, "AttributeUserID"+value);
  CPPUNIT_ASSERT(_attr2 && _attr2->Value() == value);
  
  sm->Close(study);
}



