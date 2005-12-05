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
//  File   : SALOMEDSImpl_AttributeDrawable.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _SALOMEDSImpl_AttributeDrawable_HeaderFile
#define _SALOMEDSImpl_AttributeDrawable_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <TDF_Attribute.hxx>
#include <TCollection_AsciiString.hxx>
#include <Standard_GUID.hxx>
#include <TDF_Label.hxx>      
#include "SALOMEDSImpl_GenericAttribute.hxx"
 
class Handle(TDF_Attribute);
class Handle(TDF_RelocationTable);


DEFINE_STANDARD_HANDLE( SALOMEDSImpl_AttributeDrawable, SALOMEDSImpl_GenericAttribute )


class SALOMEDSImpl_AttributeDrawable :  public SALOMEDSImpl_GenericAttribute 
{
private:
Standard_Integer myValue;

public:
Standard_EXPORT  virtual TCollection_AsciiString Save() { return (myValue == 0)?(char*)"0":(char*)"1"; }
Standard_EXPORT  virtual void Load(const TCollection_AsciiString& theValue) { (theValue == "0")?myValue=0:myValue=1; }
 
Standard_EXPORT static const Standard_GUID& GetID() ;
Standard_EXPORT static  Handle_SALOMEDSImpl_AttributeDrawable Set(const TDF_Label& label,const Standard_Integer value) ;

Standard_EXPORT void SetDrawable(const Standard_Integer theValue); 
Standard_EXPORT Standard_Integer IsDrawable() const { return myValue; }
Standard_EXPORT SALOMEDSImpl_AttributeDrawable();
Standard_EXPORT  const Standard_GUID& ID() const;
Standard_EXPORT   void Restore(const Handle(TDF_Attribute)& with) ;
Standard_EXPORT   Handle_TDF_Attribute NewEmpty() const;
Standard_EXPORT   void Paste(const Handle(TDF_Attribute)& into,const Handle(TDF_RelocationTable)& RT) const;
Standard_EXPORT ~SALOMEDSImpl_AttributeDrawable() {}

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_AttributeDrawable )

};

#endif
