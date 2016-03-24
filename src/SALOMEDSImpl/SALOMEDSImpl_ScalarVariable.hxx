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

//  File   : SALOMEDSImpl_ScalarVariable.hxx
//  Author : Roman NIKOLAEV, Open CASCADE S.A.S.
//  Module : SALOME
//
#ifndef _SALOMEDSImpl_ScalarVariable_HeaderFile
#define _SALOMEDSImpl_ScalarVariable_HeaderFile

#include "SALOMEDSImpl_Defines.hxx"
#include "SALOMEDSImpl_GenericVariable.hxx"

class SALOMEDSIMPL_EXPORT SALOMEDSImpl_ScalarVariable : 
  public SALOMEDSImpl_GenericVariable 
{
public:
  SALOMEDSImpl_ScalarVariable(SALOMEDSImpl_GenericVariable::VariableTypes theType,
                              const std::string& theName);
  ~SALOMEDSImpl_ScalarVariable();
  
  bool setValue(const double theValue);
  double getValue() const;

  bool setStringValue(const std::string& theValue);
  std::string getStringValue() const;

  virtual std::string Save() const;
  virtual std::string SaveToScript() const;
  virtual std::string SaveType() const;

  virtual void Load(const std::string& theStrValue);

private:
  double myValue;
  std::string myStrValue;
};


#endif //_SALOMEDSImpl_ScalarVariable_HeaderFile
