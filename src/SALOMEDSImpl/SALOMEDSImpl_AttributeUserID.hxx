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

#include <Standard_DefineHandle.hxx>
#include <TDF_Attribute.hxx>
#include <Standard_GUID.hxx>
#include <TDF_Label.hxx>       
#include <TCollection_AsciiString.hxx>
#include "SALOMEDSImpl_GenericAttribute.hxx"

class Handle(TDF_Attribute);
class Handle(TDF_RelocationTable);


DEFINE_STANDARD_HANDLE( SALOMEDSImpl_AttributeUserID, SALOMEDSImpl_GenericAttribute )

class SALOMEDSImpl_AttributeUserID : public SALOMEDSImpl_GenericAttribute 
{
private:
Standard_GUID myID;   

public:
SALOMEDSImpl_AttributeUserID():SALOMEDSImpl_GenericAttribute("AttributeUserID") {}
static Handle(SALOMEDSImpl_AttributeUserID) Set (const TDF_Label& L, const Standard_GUID& ID);
static const Standard_GUID& DefaultID() 
{
  static Standard_GUID SALOMEDSImpl_DefaultUserAttributeID ("FFFFFFFF-D9CD-11d6-945D-1050DA506788");
  return SALOMEDSImpl_DefaultUserAttributeID;
}   

Standard_EXPORT  const Standard_GUID& Value() const { return ID(); }
Standard_EXPORT  void SetValue(const Standard_GUID& value);

Standard_EXPORT  virtual TCollection_AsciiString Type(); 

Standard_EXPORT  const Standard_GUID& ID() const;
Standard_EXPORT  void Restore(const Handle(TDF_Attribute)& with) ;
Standard_EXPORT  Handle_TDF_Attribute NewEmpty() const;
Standard_EXPORT  void Paste(const Handle(TDF_Attribute)& into,const Handle(TDF_RelocationTable)& RT) const;
Standard_EXPORT ~SALOMEDSImpl_AttributeUserID() {}

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_AttributeUserID )

};

#endif
