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

#define PT_INTEGER 0
#define PT_REAL    1
#define PT_BOOLEAN 2
#define PT_STRING  3
#define PT_REALARRAY 4
#define PT_INTARRAY  5
#define PT_STRARRAY  6

/*!
 * Check all methods of SALOMEDS_AttributeParameter
 * Use code of SALOMEDS_AttributeParameter.cxx
 */
void SALOMEDSTest::testAttributeParameter()
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

  //Create an attribute AttributeParameter
  _PTR(AttributeParameter) _attr = studyBuilder->FindOrCreateAttribute(so, "AttributeParameter");

  //Check the attribute creation
  CPPUNIT_ASSERT(_attr);

  //Try to retreive a value with invalid ID
  bool isRaised = false;
  CPPUNIT_ASSERT(!_attr->IsSet("invalid ID", PT_INTEGER));
  try {
    _attr->GetInt("invalid ID");
  }
  catch(...) {
    isRaised = true;
  }
  CPPUNIT_ASSERT(isRaised);

  //Check method SetInt and GetInt
  _attr->SetInt("IntValue", 1);
  CPPUNIT_ASSERT(_attr->IsSet("IntValue", PT_INTEGER));
  CPPUNIT_ASSERT(_attr->GetInt("IntValue") == 1);

  //Check method SetReal an GetReal
  _attr->SetReal("RealValue", 1.2);
  CPPUNIT_ASSERT(_attr->IsSet("RealValue", PT_REAL));
  CPPUNIT_ASSERT(_attr->GetReal("RealValue") == 1.2);

  //Check method SetString and GetString
  _attr->SetString("StringValue", "hello");
  CPPUNIT_ASSERT(_attr->IsSet("StringValue", PT_STRING));
  CPPUNIT_ASSERT(_attr->GetString("StringValue") == "hello");

  //Check method SetBool and GetBool
  _attr->SetBool("BoolValue", 0);
  CPPUNIT_ASSERT(_attr->IsSet("BoolValue", PT_BOOLEAN));
  CPPUNIT_ASSERT(!_attr->GetBool("BoolValue"));

  _attr->SetBool("BoolValue", 0);
  CPPUNIT_ASSERT(_attr->IsSet("BoolValue", PT_BOOLEAN));
  CPPUNIT_ASSERT(!_attr->GetBool("BoolValue"));

  std::vector<int> intArray;
  intArray.push_back(0);
  intArray.push_back(1);

  //Check method SetIntArray and GetIntArray
  _attr->SetIntArray("IntArray", intArray);
  CPPUNIT_ASSERT(_attr->IsSet("IntArray", PT_INTARRAY));
  CPPUNIT_ASSERT(_attr->GetIntArray("IntArray")[0] == 0);
  CPPUNIT_ASSERT(_attr->GetIntArray("IntArray")[1] == 1); 

  std::vector<double> realArray;
  realArray.push_back(0.0);
  realArray.push_back(1.1);
  
  //Check method SetRealArray and GetRealArray
  _attr->SetRealArray("RealArray", realArray);
  CPPUNIT_ASSERT(_attr->IsSet("RealArray", PT_REALARRAY));
  CPPUNIT_ASSERT(_attr->GetRealArray("RealArray")[0] == 0.0);
  CPPUNIT_ASSERT(_attr->GetRealArray("RealArray")[1] == 1.1); 

  std::vector<std::string> strArray;
  strArray.push_back("hello");
  strArray.push_back("world");
  
  //Check method SetStrArray and GetStrArray
  _attr->SetStrArray("StrArray", strArray);
  CPPUNIT_ASSERT(_attr->IsSet("StrArray", PT_STRARRAY));
  CPPUNIT_ASSERT(_attr->GetStrArray("StrArray")[0] == "hello");
  CPPUNIT_ASSERT(_attr->GetStrArray("StrArray")[1] == "world"); 

  /*
  string saved = _attr->Save();
  _attr->Load(saved);

  CPPUNIT_ASSERT(_attr->IsSet("IntValue", PT_INTEGER) && _attr->GetInt("IntValue") == 1);
  CPPUNIT_ASSERT(_attr->IsSet("RealValue", PT_REAL) && _attr->GetReal("IntValue") == 1.2);
  CPPUNIT_ASSERT(_attr->IsSet("StringValue", PT_STRING) && _attr->GetString("StringValue") == "hello");
  CPPUNIT_ASSERT(_attr->IsSet("BoolValue", PT_BOOLEAN) && _attr->GetBool("BoolValue") == 0);
  CPPUNIT_ASSERT(_attr->IsSet("IntArray", PT_INTARRAY) && _attr->GetIntArray("IntArray")[0] == 0);
  CPPUNIT_ASSERT(_attr->IsSet("IntArray", PT_INTARRAY) && _attr->GetIntArray("IntArray")[1] == 1);
  CPPUNIT_ASSERT(_attr->IsSet("RealArray", PT_REALARRAY) && _attr->GetRealArray("RealArray")[0] == 0.0);
  CPPUNIT_ASSERT(_attr->IsSet("RealArray", PT_REALARRAY) && _attr->GetRealArray("RealArray")[1] == 1.1);
  CPPUNIT_ASSERT(_attr->IsSet("StrArray", PT_STRARRAY) && _attr->GetStrArray("StrArray")[0] == "hello");
  CPPUNIT_ASSERT(_attr->IsSet("StrArray", PT_STRARRAY) && _attr->GetStrArray("StrArray")[1] == "world");
  */

  sm->Close(study);
}



