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
 * Check all methods of SALOMEDS_SObject
 * Use code of SALOMEDS_SObject.cxx
 */

void SALOMEDSTest::testSObject()
{
  //Create or find the Study manager
  _PTR(StudyManager) sm ( new SALOMEDS_StudyManager(_sm) );

  CPPUNIT_ASSERT(sm);

  //Create a new study
  _PTR(Study) study = sm->NewStudy("TestSObject");

  CPPUNIT_ASSERT(study);

  //Create Study Builder
  _PTR(StudyBuilder) studyBuilder = study->NewBuilder();

  CPPUNIT_ASSERT(studyBuilder);

  //Create a new SComponent
  _PTR(SComponent) sco = studyBuilder->NewComponent("Test");

  //Create new SObject
  _PTR(SObject) so = studyBuilder->NewObjectToTag(sco, 2);

  //Check the creation of the SObject
  CPPUNIT_ASSERT(so);

  //Check method GetID
  CPPUNIT_ASSERT(so->GetID() == "0:1:1:2");

  //Check method Tag
  CPPUNIT_ASSERT(so->Tag() == 2);

  //Check method GetFatherComponent
  _PTR(SComponent) father = so->GetFatherComponent();
  CPPUNIT_ASSERT(father->GetID() == sco->GetID());

  //Check method GetFather
  _PTR(SObject) so1 = studyBuilder->NewObject(so);
  CPPUNIT_ASSERT(so1->GetFather()->GetID() == so->GetID());

  //Check method FindAttribute
     
  _PTR(AttributeIOR) _attrIOR = studyBuilder->FindOrCreateAttribute(so, "AttributeIOR");
  _PTR(AttributeName) _attrName = studyBuilder->FindOrCreateAttribute(so, "AttributeName");
  _PTR(AttributeComment) _attrComment = studyBuilder->FindOrCreateAttribute(so, "AttributeComment"); 

   std::string ior = _orb->object_to_string(_sm);
  _attrIOR->SetValue(ior);
  _attrName->SetValue("SO name");
  _attrComment->SetValue("SO comment");

  _PTR(GenericAttribute) ga;
  CPPUNIT_ASSERT(so->FindAttribute(ga, "AttributeIOR"));

  //Try to find attribute with empty type
  CPPUNIT_ASSERT(!so->FindAttribute(ga, ""));

  //Check method ReferencedObject
  studyBuilder->Addreference(so1, so);
  _PTR(SObject) so2; 
  CPPUNIT_ASSERT(so1->ReferencedObject(so2));
  CPPUNIT_ASSERT(so2->GetID() == so->GetID());

  //Check method FindSubObject
  CPPUNIT_ASSERT(so->FindSubObject(1, so2));
  CPPUNIT_ASSERT(so2->GetID() == so1->GetID());

  //Check method GetStudy
  CPPUNIT_ASSERT(so->GetStudy()->StudyId() == study->StudyId());

  //Check methods Name
  so->Name("test");
  CPPUNIT_ASSERT(so->Name() == "test");

  //Check method GetAllAttributes
  std::vector< _PTR(GenericAttribute) > v = so->GetAllAttributes();
   
  CPPUNIT_ASSERT(v.size() == 4); //+AttributeTarget +AttributeTreeNode

  //Check method GetName
  CPPUNIT_ASSERT(so->GetName() == "SO name");

  //Check method GetComment
  CPPUNIT_ASSERT(so->GetComment() == "SO comment");

  //Check method GetIOR
  CPPUNIT_ASSERT(so->GetIOR() == ior);

  //Check method Depth
  CPPUNIT_ASSERT(so->Depth() == 3);

  //Check method GetObject
  CORBA::Object_var obj = dynamic_cast<SALOMEDS_SObject*>(so.get())->GetObject();
  CPPUNIT_ASSERT(!CORBA::is_nil(obj));

  sm->Close(study);
}



