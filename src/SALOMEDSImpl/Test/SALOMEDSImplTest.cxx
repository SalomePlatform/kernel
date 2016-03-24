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

#include "SALOMEDSImplTest.hxx"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include "utilities.h"

#include "SALOMEDSImpl_AttributeParameter.hxx"
#include "SALOMEDSImpl_StudyManager.hxx"
#include "SALOMEDSImpl_Study.hxx"
#include "SALOMEDSImpl_StudyBuilder.hxx"
#include "SALOMEDSImpl_GenericAttribute.hxx"


// ============================================================================
/*!
 * Set up the environment
 */
// ============================================================================

void SALOMEDSImplTest::setUp()
{
}

// ============================================================================
/*!
 *  - delete trace classes
 */
// ============================================================================

void 
SALOMEDSImplTest::tearDown()
{
}

// ============================================================================
/*!
 * Check setting int value
 */
// ============================================================================
void SALOMEDSImplTest::testAttributeParameter()
{
  SALOMEDSImpl_StudyManager* sm = new SALOMEDSImpl_StudyManager();
  SALOMEDSImpl_Study* study = sm->NewStudy("Test");
  SALOMEDSImpl_AttributeParameter* _ap = study->GetCommonParameters("TestComp", 0);

  CPPUNIT_ASSERT(_ap);

  _ap->SetInt("IntValue", 1);
  CPPUNIT_ASSERT(_ap->IsSet("IntValue", PT_INTEGER));
  CPPUNIT_ASSERT(_ap->GetInt("IntValue") == 1);

  _ap->SetReal("RealValue", 1.2);
  CPPUNIT_ASSERT(_ap->IsSet("RealValue", PT_REAL));
  CPPUNIT_ASSERT(_ap->GetReal("RealValue") == 1.2);

  _ap->SetString("StringValue", "hello");
  CPPUNIT_ASSERT(_ap->IsSet("StringValue", PT_STRING));
  CPPUNIT_ASSERT(_ap->GetString("StringValue") == "hello");

  _ap->SetBool("BoolValue", 0);
  CPPUNIT_ASSERT(_ap->IsSet("BoolValue", PT_BOOLEAN));
  CPPUNIT_ASSERT(!_ap->GetBool("BoolValue"));

  _ap->SetBool("BoolValue", 0);
  CPPUNIT_ASSERT(_ap->IsSet("BoolValue", PT_BOOLEAN));
  CPPUNIT_ASSERT(!_ap->GetBool("BoolValue"));

  std::vector<int> intArray;
  intArray.push_back(0);
  intArray.push_back(1);

  _ap->SetIntArray("IntArray", intArray);
  CPPUNIT_ASSERT(_ap->IsSet("IntArray", PT_INTARRAY));
  CPPUNIT_ASSERT(_ap->GetIntArray("IntArray")[0] == 0);
  CPPUNIT_ASSERT(_ap->GetIntArray("IntArray")[1] == 1); 

  std::vector<double> realArray;
  realArray.push_back(0.0);
  realArray.push_back(1.1);
  
  _ap->SetRealArray("RealArray", realArray);
  CPPUNIT_ASSERT(_ap->IsSet("RealArray", PT_REALARRAY));
  CPPUNIT_ASSERT(_ap->GetRealArray("RealArray")[0] == 0.0);
  CPPUNIT_ASSERT(_ap->GetRealArray("RealArray")[1] == 1.1); 

  std::vector<std::string> strArray;
  strArray.push_back("hello");
  strArray.push_back("world");
  
  _ap->SetStrArray("StrArray", strArray);
  CPPUNIT_ASSERT(_ap->IsSet("StrArray", PT_STRARRAY));
  CPPUNIT_ASSERT(_ap->GetStrArray("StrArray")[0] == "hello");
  CPPUNIT_ASSERT(_ap->GetStrArray("StrArray")[1] == "world"); 

}



