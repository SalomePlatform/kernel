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
 * Check all methods of SALOMEDS_AttributeTreeNode
 * Use code of SALOMEDS_AttributeTreeNode.cxx
 */
#define SALOMEDS_ALL_TESTS
void SALOMEDSTest::testAttributeTreeNode()
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

  //Create an attribute AttributeTreeNode
  _PTR(AttributeTreeNode) _attr = studyBuilder->FindOrCreateAttribute(so, "AttributeTreeNode");

  //Check the attribute creation
  CPPUNIT_ASSERT(_attr);

  std::string TreeNodeID = "0e1c36e6-379b-4d90-ab3b-17a14310e648";

  _PTR(SObject) so1 = study->CreateObjectID("0:1:2");
                                                               
  _PTR(AttributeTreeNode) _attr1 = studyBuilder->FindOrCreateAttribute(so1, "AttributeTreeNode");

  //Check the attribute creation
  CPPUNIT_ASSERT(_attr1);

  //Check method Label
  CPPUNIT_ASSERT(_attr1->Label() == "0:1:2");

  _PTR(SObject) so2 = study->CreateObjectID("0:1:3");

  _PTR(AttributeTreeNode) _attr2 = studyBuilder->FindOrCreateAttribute(so2, "AttributeTreeNode");

  //Check the attribute creation
  CPPUNIT_ASSERT(_attr2);

  //Check method SetFather 
  _attr1->SetFather(_attr);

  //Check method HasFather
  CPPUNIT_ASSERT(_attr1->HasFather());

#ifdef SALOMEDS_ALL_TESTS
  //Check method GetFather
  CPPUNIT_ASSERT(_attr1->GetFather()->Label() == _attr->Label());

  //Check method Append
  _attr->Append(_attr1);

  //Check possibility to Append to itself
  bool isRaised = false;
  try {
    _attr->Append(_attr);
  }catch(...) {
    isRaised = true;
  }
  CPPUNIT_ASSERT(isRaised);

  _attr->Append(_attr2);
  
  //Check method HasNext
  CPPUNIT_ASSERT(_attr1->HasNext());

  //Check method GetNext
  CPPUNIT_ASSERT(_attr1->GetNext()->Label() == _attr2->Label());

  //Check method HasPrevious
  CPPUNIT_ASSERT(_attr2->HasPrevious());

  //Check method GetPrevious
  CPPUNIT_ASSERT(_attr2->GetPrevious()->Label() == _attr1->Label());

  //Check method Depth
  CPPUNIT_ASSERT(_attr->Depth() == 1 && _attr1->Depth() == 2);

  //Check method IsRoot
  CPPUNIT_ASSERT(_attr->IsRoot());

  //Check method IsFather
  CPPUNIT_ASSERT(_attr->IsFather(_attr1));

  //Check method IsDescendant
  CPPUNIT_ASSERT(_attr2->IsDescendant(_attr));

  //Check method GetFirst
  CPPUNIT_ASSERT(_attr->GetFirst()->Label() == _attr1->Label());

  _attr2->Remove();

  //Check method Prepend
  _attr->Prepend(_attr2);

  CPPUNIT_ASSERT(_attr->GetFirst()->Label() == _attr2->Label());

  //Check possibility to Prepend to itself
  isRaised = false;
  try {
    _attr->Prepend(_attr);
  }catch(...) {
    isRaised = true;
  }
  CPPUNIT_ASSERT(isRaised);

  _attr1->Remove();

  //Check method InsertBefore
  _attr2->InsertBefore(_attr1);

  CPPUNIT_ASSERT(_attr->GetFirst()->Label() == _attr1->Label());

  //Check possibility to InsertBefore to itself
  isRaised = false;
  try {
    _attr->InsertBefore(_attr);
  }catch(...) {
    isRaised = true;
  }
  CPPUNIT_ASSERT(isRaised);

  _attr1->Remove();

  //Check method InsertAfter
  _attr2->InsertAfter(_attr1);

  CPPUNIT_ASSERT(_attr->GetFirst()->Label() == _attr2->Label());

  //Check possibility to InsertAfter to itself
  isRaised = false;
  try {
    _attr->InsertAfter(_attr);
  }catch(...) {
    isRaised = true;
  }
  CPPUNIT_ASSERT(isRaised);

  //Check method Remove
  _attr2->Remove();

  CPPUNIT_ASSERT(_attr->GetFirst()->Label() == _attr1->Label());

  //Check method SetTreeID and GetTreeID
  _attr2->SetTreeID(TreeNodeID);

  CPPUNIT_ASSERT(_attr2->GetTreeID() == TreeNodeID); 
#else
  std::cout << std::endl << "THE TEST IS NOT COMPLETE !!!" << std::endl;
#endif


  //Try to create the attribute with given TreeID
  std::string value = "0e1c36e6-1111-4d90-ab3b-18a14310e648";
  _PTR(AttributeTreeNode) _attr_guid = studyBuilder->FindOrCreateAttribute(so, "AttributeTreeNodeGUID"+value);
  CPPUNIT_ASSERT(_attr_guid && _attr_guid->GetTreeID() == value);
  
  sm->Close(study);
}
#undef SALOMEDS_ALL_TESTS


