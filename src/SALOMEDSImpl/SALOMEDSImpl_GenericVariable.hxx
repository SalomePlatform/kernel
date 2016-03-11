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

//  File   : SALOMEDSImpl_GenericVariable.hxx
//  Author : Roman NIKOLAEV, Open CASCADE S.A.S.
//  Module : SALOME
//
#ifndef _GENERICIMPL_VARIABLE_HXX_
#define _GENERICIMPL_VARIABLE_HXX_

#include "SALOMEDSImpl_Defines.hxx"

#include <string>

//This is base class for all variable of the Salome notebook

class SALOMEDSIMPL_EXPORT SALOMEDSImpl_GenericVariable
{
 public:
  
  //Supported types of the nootebook variables
  enum VariableTypes{REAL_VAR, INTEGER_VAR, BOOLEAN_VAR, STRING_VAR};

  SALOMEDSImpl_GenericVariable(VariableTypes theType, const std::string& theName);
  ~SALOMEDSImpl_GenericVariable();

  VariableTypes Type() const;

  std::string Name() const;

  static VariableTypes String2VariableType(const std::string& theStrType);
  
  bool setType(const VariableTypes theType);

  bool setName(const std::string& theName);

  virtual std::string Save() const;
  virtual std::string SaveToScript() const;
  virtual std::string SaveType() const;

  virtual void Load(const std::string& theStrValue);
  
 private:
  VariableTypes _type;
  std::string _name;
};

#endif //_GENERICIMPL_VARIABLE_HXX_
