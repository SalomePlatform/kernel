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

//  File   : SALOMEDSImpl_GenericVariable.cxx
//  Author : Roman NIKOLAEV, Open CASCADE S.A.S.
//  Module : SALOME
//
#include "SALOMEDSImpl_GenericVariable.hxx"
#include "SALOMEDSImpl_Attributes.hxx"
#include "SALOMEDSImpl_Study.hxx"

#include <string>
#include <stdlib.h>


//============================================================================
/*! Function : SALOMEDSImpl_GenericVariable
 *  Purpose  : 
 */
//============================================================================
SALOMEDSImpl_GenericVariable::
SALOMEDSImpl_GenericVariable(SALOMEDSImpl_GenericVariable::VariableTypes theType,
                             const std::string& theName): 
  _type(theType),
  _name(theName)
{}

//============================================================================
/*! Function : ~SALOMEDSImpl_GenericVariable
 *  Purpose  : 
 */
//============================================================================
SALOMEDSImpl_GenericVariable::~SALOMEDSImpl_GenericVariable()
{}

//============================================================================
/*! Function : Type
 *  Purpose  : 
 */
//============================================================================
SALOMEDSImpl_GenericVariable::VariableTypes SALOMEDSImpl_GenericVariable::Type() const
{
  return _type;
}

//============================================================================
/*! Function : Name
 *  Purpose  : 
 */
//============================================================================
std::string SALOMEDSImpl_GenericVariable::Name() const
{
  return _name;
}

//============================================================================
/*! Function : setType
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_GenericVariable::setType(const SALOMEDSImpl_GenericVariable::VariableTypes theType)
{
  if(_type == theType)
    return false;

  _type = theType;
  return true;
}

//============================================================================
/*! Function : setName
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_GenericVariable::setName(const std::string& theName)
{
  if(_name.compare(theName) == 0)
    return false;

  _name = theName;
  return true;
}

//============================================================================
/*! Function : String2VariableType
 *  Purpose  : 
 */
//============================================================================
SALOMEDSImpl_GenericVariable::VariableTypes SALOMEDSImpl_GenericVariable::String2VariableType(const std::string& theStrType)
{
  return(SALOMEDSImpl_GenericVariable::VariableTypes)atoi((char*)theStrType.c_str());
}

//============================================================================
/*! Function : Save
 *  Purpose  : 
 */
//============================================================================
std::string SALOMEDSImpl_GenericVariable::Save() const
{
  return std::string();
}


//============================================================================
/*! Function : SaveToScript
 *  Purpose  : 
 */
//============================================================================
std::string SALOMEDSImpl_GenericVariable::SaveToScript() const
{
  return std::string();
}
//============================================================================
/*! Function : SaveType
 *  Purpose  : 
 */
//============================================================================
std::string SALOMEDSImpl_GenericVariable::SaveType() const
{
  return std::string();
}

//============================================================================
/*! Function : Load
 *  Purpose  : 
 */
//============================================================================
void SALOMEDSImpl_GenericVariable::Load(const std::string& theStrValue)
{
}
