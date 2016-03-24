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
 * Check all methods of SALOMEDS_SComponent
 * Use code of SALOMEDS_SComponent.cxx
 */
void SALOMEDSTest::testSComponent()
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

  _PTR(SComponent) sco = studyBuilder->NewComponent("Test");

  //Check the creation of the SComponent
  CPPUNIT_ASSERT(sco);

  //Create an attribute AttributeIOR
  _PTR(AttributeIOR) _attr = studyBuilder->FindOrCreateAttribute(sco, "AttributeIOR");

  //Check the attribute creation
  CPPUNIT_ASSERT(_attr);

  //Check method ComponentIOR

  std::string ior = _orb->object_to_string(_sm);

  _attr->SetValue(ior);
  std::string new_ior;
  CPPUNIT_ASSERT(sco->ComponentIOR(new_ior));

  CPPUNIT_ASSERT(new_ior == ior);

  //Check method ComponentDataType
  CPPUNIT_ASSERT(sco->ComponentDataType() == "Test");

  sm->Close(study);
}



