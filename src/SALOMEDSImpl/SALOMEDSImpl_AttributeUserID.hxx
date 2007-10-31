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
//  File   : SALOMEDSImpl_AttributeUAttribute.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _SALOMEDSImpl_AttributeUserID_HeaderFile
#define _SALOMEDSImpl_AttributeUserID_HeaderFile

#include "DF_Attribute.hxx"
#include "DF_Label.hxx"
#include <string>
#include "SALOMEDSImpl_GenericAttribute.hxx"


class SALOMEDSImpl_AttributeUserID : public SALOMEDSImpl_GenericAttribute 
{
private:
std::string myID;   

public:
SALOMEDSImpl_AttributeUserID():SALOMEDSImpl_GenericAttribute("AttributeUserID") {}
static SALOMEDSImpl_AttributeUserID* Set (const DF_Label& L, const std::string& ID);
static const std::string& DefaultID() 
{
  static std::string SALOMEDSImpl_DefaultUserAttributeID ("FFFFFFFF-D9CD-11d6-945D-1050DA506788");
  return SALOMEDSImpl_DefaultUserAttributeID;
}   

Standard_EXPORT  const std::string& Value() const { return ID(); }
Standard_EXPORT  void SetValue(const std::string& value);

Standard_EXPORT  virtual std::string Type(); 

Standard_EXPORT  const std::string& ID() const;
Standard_EXPORT  void Restore(DF_Attribute* with) ;
Standard_EXPORT  DF_Attribute* NewEmpty() const;
Standard_EXPORT  void Paste(DF_Attribute* into);
Standard_EXPORT ~SALOMEDSImpl_AttributeUserID() {}

};

#endif
