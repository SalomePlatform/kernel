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
 * Check all methods of SALOMEDS_UseCaseBuilder and UseCaseIterator
 * Use code of SALOMEDS_UseCaseBuilder.cxx and SALOMEDS_UseCaseIterator.cxx
 */

#define SALOMEDS_ALL_TESTS

void SALOMEDSTest::testUseCase()
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

  //Create a new UseCaseBuilder
  _PTR(UseCaseBuilder) builder = study->GetUseCaseBuilder();
  CPPUNIT_ASSERT(builder);

  //Check method AddUseCase
  _PTR(SObject) root1 = builder->AddUseCase("UseCase_1");
  _PTR(SObject) root2 = builder->AddUseCase("UseCase_2");
  CPPUNIT_ASSERT(root1 && root2);

  //Check method SetCurrentObject
  builder->SetCurrentObject(root1);

  //Check method GetCurrentObject
  CPPUNIT_ASSERT(builder->GetCurrentObject() && builder->GetCurrentObject()->GetID() == root1->GetID());

  //Check method SetRootCurrent
  CPPUNIT_ASSERT(builder->SetRootCurrent());
  CPPUNIT_ASSERT(builder->GetCurrentObject()->GetID() == "0:2"); //Root in UseCases is always placed at label 0:2

  //Check method IsUseCase
  CPPUNIT_ASSERT(builder->IsUseCase(root1) && builder->IsUseCase(root2));

  //Check method SetName
  builder->SetName("A root use case");

  //Check method GetName
  CPPUNIT_ASSERT(builder->GetName() == "A root use case");

#ifdef SALOMEDS_ALL_TESTS
  //Check method GetUseCaseIterator
  builder->SetRootCurrent();
  _PTR(SObject) nullObject;
  _PTR(UseCaseIterator) root_it = builder->GetUseCaseIterator(nullObject); //The iterator on all UseCases
  root_it->Init(false); //Only the first level
  CPPUNIT_ASSERT(root_it->More());
  CPPUNIT_ASSERT(root_it->Value()->GetID() == root1->GetID());
  root_it->Next();
  CPPUNIT_ASSERT(root_it->More());
  CPPUNIT_ASSERT(root_it->Value()->GetID() == root2->GetID());
  root_it->Next();
  CPPUNIT_ASSERT(!root_it->More());
#else
  cout << endl << "THE TEST IS NOT COMPLETE !!!" << endl;
#endif

  builder->SetCurrentObject(root1);
  _PTR(UseCaseIterator) it = builder->GetUseCaseIterator(root1); //The iterator on the given UseCase

  //Check method Append
  _PTR(SObject) so1 = study->CreateObjectID("0:1:1");
  _PTR(SObject) so2 = study->CreateObjectID("0:1:2");
  CPPUNIT_ASSERT(builder->Append(so1));

  //Check methods Init, More, Next and Value of UseCaseIterator
  it->Init(false);
  CPPUNIT_ASSERT(it->More());
  CPPUNIT_ASSERT(it->Value()->GetID() == so1->GetID());
  it->Next();
  CPPUNIT_ASSERT(!it->More());

  //Check method AppendTo
  CPPUNIT_ASSERT(builder->AppendTo(so1, so2));

  //Check method HasChildren
  CPPUNIT_ASSERT(builder->HasChildren(so1));
  CPPUNIT_ASSERT(!builder->HasChildren(so2));

  //Check method SortChildren
  CPPUNIT_ASSERT(builder->SortChildren(so1, true));
  CPPUNIT_ASSERT(!builder->SortChildren(so2, false));

  //Check method Init of UseCaseIterator
  it->Init(false); //Only the first level
  CPPUNIT_ASSERT(it->More());
  CPPUNIT_ASSERT(it->Value()->GetID() == so1->GetID());
  it->Next();
  CPPUNIT_ASSERT(!it->More());

  it->Init(true); //All levels
  CPPUNIT_ASSERT(it->More());
  CPPUNIT_ASSERT(it->Value()->GetID() == so1->GetID());
  it->Next();
  CPPUNIT_ASSERT(it->More());
  CPPUNIT_ASSERT(it->Value()->GetID() == so2->GetID());
  it->Next();
  CPPUNIT_ASSERT(!it->More());

  //Check method Remove
  CPPUNIT_ASSERT(builder->Remove(so2));
  it->Init(true);
  CPPUNIT_ASSERT(it->More());
  CPPUNIT_ASSERT(it->Value()->GetID() == so1->GetID());
  it->Next();
  CPPUNIT_ASSERT(!it->More());

  CPPUNIT_ASSERT(builder->Append(so2));

  //Check method InsertBefore
  CPPUNIT_ASSERT(builder->InsertBefore(so2, so1));

  it->Init(false);
  CPPUNIT_ASSERT(it->More());
  CPPUNIT_ASSERT(it->Value()->GetID() == so2->GetID());
  it->Next();
  CPPUNIT_ASSERT(it->More());
  CPPUNIT_ASSERT(it->Value()->GetID() == so1->GetID());

  sm->Close(study);
}
#undef SALOMEDS_ALL_TESTS


