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
// See http://www.salome-platform.org/
//
//  File   : SALOMEDSImpl_AttributeInteger.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _SALOMEDSImpl_AttributeInteger_HeaderFile
#define _SALOMEDSImpl_AttributeInteger_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <TDF_Attribute.hxx>
#include <TCollection_AsciiString.hxx>
#include <TDF_Label.hxx>
#include "SALOMEDSImpl_GenericAttribute.hxx"

class Standard_GUID;
class Handle(TDF_Attribute);
class Handle(TDF_RelocationTable);


DEFINE_STANDARD_HANDLE( SALOMEDSImpl_AttributeInteger, SALOMEDSImpl_GenericAttribute )


class SALOMEDSImpl_AttributeInteger : public SALOMEDSImpl_GenericAttribute 
{
private:
Standard_Integer myValue;

public:
Standard_EXPORT static const Standard_GUID& GetID() ;

Standard_EXPORT  SALOMEDSImpl_AttributeInteger():SALOMEDSImpl_GenericAttribute("AttributeInteger") {} 
Standard_EXPORT  static Handle(SALOMEDSImpl_AttributeInteger) Set (const TDF_Label& L, Standard_Integer Val); 
Standard_EXPORT  void SetValue(const Standard_Integer theVal);
Standard_EXPORT  Standard_Integer Value() const { return myValue; } 
Standard_EXPORT  virtual TCollection_AsciiString Save() { return TCollection_AsciiString(myValue); }
Standard_EXPORT  virtual void Load(const TCollection_AsciiString& theValue) { myValue = theValue.IntegerValue(); } 

Standard_EXPORT  const Standard_GUID& ID() const;
Standard_EXPORT  void Restore(const Handle(TDF_Attribute)& with) ;
Standard_EXPORT  Handle_TDF_Attribute NewEmpty() const;
Standard_EXPORT  void Paste(const Handle(TDF_Attribute)& into,const Handle(TDF_RelocationTable)& RT) const;
Standard_EXPORT ~SALOMEDSImpl_AttributeInteger() {}

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_AttributeInteger )

};

#endif
