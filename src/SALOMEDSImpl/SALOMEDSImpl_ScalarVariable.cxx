// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

//  File   : SALOMEDSImpl_ScalarVariable.cxx
//  Author : Roman NIKOLAEV, Open CASCADE S.A.S.
//  Module : SALOME
//
#include "SALOMEDSImpl_ScalarVariable.hxx"
#include "SALOMEDSImpl_GenericVariable.hxx"
#include "Basics_Utils.hxx"
#include <iostream>
#include <cstdlib>
#include <cstdio>

#define OLDSTUDY_COMPATIBILITY

//============================================================================
/*! Function : SALOMEDSImpl_ScalarVariable
 *  Purpose  : 
 */
//============================================================================
SALOMEDSImpl_ScalarVariable::
SALOMEDSImpl_ScalarVariable(SALOMEDSImpl_GenericVariable::VariableTypes type,
                            const std::string& theName):
  SALOMEDSImpl_GenericVariable(type,theName),
  myValue( 1e+100 )
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
bool SALOMEDSImpl_ScalarVariable::setStringValue(const std::string& theValue)
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
std::string SALOMEDSImpl_ScalarVariable::getStringValue() const
{
  return myStrValue;
}

//============================================================================
/*! Function : Save()
 *  Purpose  : 
 */
//============================================================================
std::string SALOMEDSImpl_ScalarVariable::Save() const{
  Kernel_Utils::Localizer loc;

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
        sprintf(buffer, "%s", myStrValue.c_str());
        break;
      }
    default:break;
    }
  return std::string(buffer);
}

//============================================================================
/*! Function : SaveToScript()
 *  Purpose  : 
 */
//============================================================================
std::string SALOMEDSImpl_ScalarVariable::SaveToScript() const
{
  Kernel_Utils::Localizer loc;

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
	sprintf(buffer, "%s", ((bool)myValue) ? "True" : "False");
        break;
      }
    case SALOMEDSImpl_GenericVariable::STRING_VAR:
      {
        sprintf(buffer, "\"%s\"", myStrValue.c_str());
        break;
      }
    default:break;
    }
  return std::string(buffer);
}

//============================================================================
/*! Function : SaveType()
 *  Purpose  : 
 */
//============================================================================
std::string SALOMEDSImpl_ScalarVariable::SaveType() const{
  char buffer[255];
  sprintf(buffer, "%d", (int)Type());
  return std::string(buffer);
}

//============================================================================
/*! Function : Load()
 *  Purpose  : 
 */
//============================================================================
void SALOMEDSImpl_ScalarVariable::Load(const std::string& theStrValue)
{
  Kernel_Utils::Localizer loc;

  std::string strCopy = theStrValue;
  if ( Type() == SALOMEDSImpl_GenericVariable::STRING_VAR ) {
#ifdef OLDSTUDY_COMPATIBILITY
    if (strCopy.size() > 1 && strCopy[0] == '\"' && strCopy[strCopy.size()-1] == '\"')
      strCopy = strCopy.substr(1, strCopy.size()-2);
#endif // OLDSTUDY_COMPATIBILITY
    setStringValue( strCopy );
  }
  else {
#ifdef OLDSTUDY_COMPATIBILITY
    int dotpos = strCopy.find(',');
    if (dotpos != std::string::npos)
      strCopy.replace(dotpos, 1, ".");
#endif // OLDSTUDY_COMPATIBILITY
    double aValue = atof(strCopy.c_str());
    setValue(aValue);
  }
}
