// Copyright (C) 2008  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
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
//  File   : SALOMEDSImpl_ScalarVariable.cxx
//  Author : Roman NIKOLAEV, Open CASCADE S.A.S.
//  Module : SALOME

#include "SALOMEDSImpl_ScalarVariable.hxx"
#include "SALOMEDSImpl_GenericVariable.hxx"
#include <iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;

//============================================================================
/*! Function : SALOMEDSImpl_ScalarVariable
 *  Purpose  : 
 */
//============================================================================
SALOMEDSImpl_ScalarVariable::
SALOMEDSImpl_ScalarVariable(SALOMEDSImpl_GenericVariable::VariableTypes type,
                            const string& theName):
  SALOMEDSImpl_GenericVariable(type,theName)
{}

//============================================================================
/*! Function : ~SALOMEDSImpl_ScalarVariable()
 *  Purpose  : 
 */
//============================================================================
SALOMEDSImpl_ScalarVariable::~SALOMEDSImpl_ScalarVariable(){}

//============================================================================
/*! Function : 
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_ScalarVariable::setValue(const double theValue)
{
  
  if(myValue == theValue) 
    return false;
  
  myValue = theValue;
  return true;
}

//============================================================================
/*! Function : 
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_ScalarVariable::setStringValue(const string& theValue)
{
  
  if(myStrValue == theValue) 
    return false;
  
  myStrValue = theValue;
  return true;
}

//============================================================================
/*! Function : getValue()
 *  Purpose  : 
 */
//============================================================================
double SALOMEDSImpl_ScalarVariable::getValue() const
{
  return myValue;
}

//============================================================================
/*! Function : getStringValue()
 *  Purpose  : 
 */
//============================================================================
string SALOMEDSImpl_ScalarVariable::getStringValue() const
{
  return myStrValue;
}

//============================================================================
/*! Function : Save()
 *  Purpose  : 
 */
//============================================================================
string SALOMEDSImpl_ScalarVariable::Save() const{
  char buffer[255];
  switch(Type())
    {
    case SALOMEDSImpl_GenericVariable::REAL_VAR:
      {
        sprintf(buffer, "%.64e", myValue);
        break;
      }
    case SALOMEDSImpl_GenericVariable::BOOLEAN_VAR:
    case SALOMEDSImpl_GenericVariable::INTEGER_VAR:
      {
        sprintf(buffer, "%d", (int)myValue);
        break;
      }
    case SALOMEDSImpl_GenericVariable::STRING_VAR:
      {
        sprintf(buffer, "\"%s\"", myStrValue.c_str());
        break;
      }
    default:break;
    }
  return string(buffer);
}

//============================================================================
/*! Function : SaveToScript()
 *  Purpose  : 
 */
//============================================================================
string SALOMEDSImpl_ScalarVariable::SaveToScript() const
{
  char buffer[255];
  switch(Type())
    {
    case SALOMEDSImpl_GenericVariable::REAL_VAR:
      {
        sprintf(buffer, "%g", myValue);
        break;
      }
    case SALOMEDSImpl_GenericVariable::INTEGER_VAR:
      {
        sprintf(buffer, "%d", (int)myValue);
        break;
      }
    case SALOMEDSImpl_GenericVariable::BOOLEAN_VAR:
      {
        if((bool)myValue)
          sprintf(buffer, "%s", "True");
        else
          sprintf(buffer, "%s", "False");
        break;
      }
    case SALOMEDSImpl_GenericVariable::STRING_VAR:
      {
        sprintf(buffer, "\"%s\"", myStrValue.c_str());
        break;
      }
    default:break;
    }
  return string(buffer);
}

//============================================================================
/*! Function : SaveType()
 *  Purpose  : 
 */
//============================================================================
string SALOMEDSImpl_ScalarVariable::SaveType() const{
  char buffer[255];
  sprintf(buffer, "%d", (int)Type());
  return string(buffer);
}

//============================================================================
/*! Function : Load()
 *  Purpose  : 
 */
//============================================================================
void SALOMEDSImpl_ScalarVariable::Load(const string& theStrValue)
{
  double aValue = atof(theStrValue.c_str());
  setValue(aValue);
}
