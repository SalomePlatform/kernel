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
 * Check all methods of SALOMEDS_AttributeTableOfString
 * Use code of SALOMEDS_AttributeTableOfString.cxx
 */
void SALOMEDSTest::testAttributeTableOfString()
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

  //Create an attribute AttributeTableOfString
  _PTR(AttributeTableOfString) _attr = studyBuilder->FindOrCreateAttribute(so, "AttributeTableOfString");

  //Check the attribute creation
  CPPUNIT_ASSERT(_attr);
  //Check method SetTitle
  _attr->SetTitle("Table_1");

  //Check method GetTitle
  CPPUNIT_ASSERT(_attr->GetTitle() == "Table_1");

  //Check method SetNbColumns
  _attr->SetNbColumns(2);

  //Check method GetNbColumns
  CPPUNIT_ASSERT(_attr->GetNbColumns() == 2);

  //Check method HasValue
  CPPUNIT_ASSERT(!_attr->HasValue(1, 1));

  bool isCaught = false;
  try {
    _attr->GetValue(1, 1);
  }
  catch(...) {
     isCaught = true;
  }
  CPPUNIT_ASSERT(isCaught);


  //Check method PutValue
  _attr->PutValue("23", 1,1);

  CPPUNIT_ASSERT(_attr->HasValue(1, 1));

  //Check method GetValue
  CPPUNIT_ASSERT(_attr->GetValue(1, 1) == "23");

  //Check method GetRowSetIndices
  std::vector<int> rs = _attr->GetRowSetIndices(1);

  CPPUNIT_ASSERT(rs.size() == 1 && rs[0] == 1);

  _attr->PutValue("32", 2,2);
  CPPUNIT_ASSERT(_attr->HasValue(2, 2));

  std::vector<std::string> rowTitles;
  rowTitles.push_back("title1");
  rowTitles.push_back("title2");

  //Check method SetRowTitles
  _attr->SetRowTitles(rowTitles);

  //Check method SetRowTitle
  _attr->SetRowTitle(1, "new_title");

  //Check method GetRowTitles
  std::vector<std::string> rt = _attr->GetRowTitles();

  CPPUNIT_ASSERT(rt.size() == 2 && rt[0] == "new_title" && rt[1] == "title2");


  std::vector<std::string> colTitles;
  colTitles.push_back("title1");
  colTitles.push_back("title2");

  //Check method SetColumnTitles
  _attr->SetColumnTitles(colTitles);

  //Check method SetColumnTitle
  _attr->SetColumnTitle(1, "new_title");

  //Check method GetColumnTitles
  std::vector<std::string> ct = _attr->GetColumnTitles();

  CPPUNIT_ASSERT(ct.size() == 2 && ct[0] == "new_title" && ct[1] == "title2");

  std::vector<std::string> rowUnits;
  rowUnits.push_back("unit1");
  rowUnits.push_back("unit2");

  //Check method SetRowUnits
  _attr->SetRowUnits(rowUnits);

  //Check method SetRowUnit
  _attr->SetRowUnit(1, "new_unit");

  //Check method GetRowUnits
  std::vector<std::string> ru = _attr->GetRowUnits();

  CPPUNIT_ASSERT(ru.size() == 2 && ru[0] == "new_unit" && ru[1] == "unit2");

  //Check method GetNbColumns
  CPPUNIT_ASSERT(_attr->GetNbColumns() == 2);

  //Check method AddRow
  std::vector<std::string> data;
  data.push_back("11");
  data.push_back("22");

  _attr->AddRow(data);

  CPPUNIT_ASSERT(_attr->GetNbRows() == 3);

  //Check method GetRow
  std::vector<std::string> data2 = _attr->GetRow(3);

  CPPUNIT_ASSERT(data2.size() == 2 && data2[0] == "11" && data2[1] == "22");

  //Check method SetRow
  data[0] = "33";
  _attr->SetRow(3, data);

  data2 = _attr->GetRow(3);

  CPPUNIT_ASSERT(data2.size() == 2 && data2[0] == "33" && data2[1] == "22");

   //Check method AddColumn
  data[0] = "-11";
  data[1] = "-22";
  data.push_back("-33");

  _attr->AddColumn(data);

  CPPUNIT_ASSERT(_attr->GetNbColumns() == 3);

  //Check method GetColumn
  data2 = _attr->GetColumn(3);

  CPPUNIT_ASSERT(data2.size() == 3 && data2[0] == "-11" && data2[1] == "-22" && data2[2] == "-33");

  //Check method SetColumn
  data[0] = "11";
  _attr->SetColumn(3, data);

  data2 = _attr->GetColumn(3);

  CPPUNIT_ASSERT(data2.size() == 3 && data2[0] == "11" && data2[1] == "-22" && data2[2] == "-33");

  sm->Close(study);
}



