// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  File   : SALOMEDSImpl_GenericAttribute.hxx
//  Author : SERGEY_RUIN
//  Module : SALOME

#ifndef _GENERICIMPL_ATTRIBUTE_HXX_
#define _GENERICIMPL_ATTRIBUTE_HXX_

#include "DF_Label.hxx"
#include "DF_Attribute.hxx"
#include <string>

#include "SALOMEDSImpl_SObject.hxx"

class SALOMEDSImpl_GenericAttribute: public DF_Attribute
{
protected:

  std::string _type; //This field must be filled in each attribute that inherits from this class.

public:

Standard_EXPORT SALOMEDSImpl_GenericAttribute(const std::string& theType) 
:_type(theType)
{}

Standard_EXPORT virtual std::string Type();
Standard_EXPORT virtual void CheckLocked();
Standard_EXPORT std::string GetClassType() { return _type; }
Standard_EXPORT SALOMEDSImpl_SObject GetSObject();
Standard_EXPORT void SetModifyFlag();

Standard_EXPORT static char* Impl_GetType(DF_Attribute* theAttr); 
Standard_EXPORT static char* Impl_GetClassType(DF_Attribute* theAttr);
Standard_EXPORT static void Impl_CheckLocked(DF_Attribute* theAttr);

};


#endif
