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

//  File   : SALOMEDSImpl_GenericAttribute.hxx
//  Author : SERGEY_RUIN
//  Module : SALOME
//
#ifndef _GENERICIMPL_ATTRIBUTE_HXX_
#define _GENERICIMPL_ATTRIBUTE_HXX_

#include "SALOMEDSImpl_Defines.hxx"
#include "DF_Label.hxx"
#include "DF_Attribute.hxx"
#include <string>

#include "SALOMEDSImpl_SObject.hxx"

class SALOMEDSIMPL_EXPORT SALOMEDSImpl_GenericAttribute: 
  public DF_Attribute
{
protected:

  std::string _type; //This field must be filled in each attribute that inherits from this class.

public:

  SALOMEDSImpl_GenericAttribute(const std::string& theType) 
    :_type(theType)
  {}

  virtual std::string Type();
  virtual void CheckLocked();
  std::string GetClassType() { return _type; }
  SALOMEDSImpl_SObject GetSObject();
  void SetModifyFlag(int reason = 0);

  static std::string Impl_GetType(DF_Attribute* theAttr); 
  static std::string Impl_GetClassType(DF_Attribute* theAttr);
  static void Impl_CheckLocked(DF_Attribute* theAttr);

};


#endif
