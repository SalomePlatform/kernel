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
//  File   : SALOMEDSImpl_SObject.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSIMPL_SOBJECT_H__
#define __SALOMEDSIMPL_SOBJECT_H__

//Handle definition
#include <Handle_MMgt_TShared.hxx>
#include <Standard_DefineHandle.hxx>
DEFINE_STANDARD_HANDLE( SALOMEDSImpl_SObject, MMgt_TShared )

class SALOMEDSImpl_SComponent;
class Handle_SALOMEDSImpl_SComponent;
class SALOMEDSImpl_Study;
class Handle_SALOMEDSImpl_Study;

// Cascade headers
#include <TDF_Label.hxx>
#include <TDF_Tool.hxx>
#include <TDF_Attribute.hxx>
#include <Standard_GUID.hxx>
#include <Standard_NoSuchObject.hxx>
#include <TCollection_AsciiString.hxx>
#include <TColStd_HSequenceOfTransient.hxx>

class SALOMEDSImpl_SObject : public MMgt_TShared 
{
protected:
  TDF_Label                _lab;
  TCollection_AsciiString  _name;  
  TCollection_AsciiString  _value;
  TCollection_AsciiString  _type;

public:
  
  Standard_EXPORT SALOMEDSImpl_SObject(const TDF_Label& theLabel);
  Standard_EXPORT ~SALOMEDSImpl_SObject();
  
  Standard_EXPORT virtual TCollection_AsciiString GetID();
  Standard_EXPORT virtual Handle(SALOMEDSImpl_SComponent) GetFatherComponent();
  Standard_EXPORT virtual Handle(SALOMEDSImpl_SObject) GetFather() ;
  Standard_EXPORT virtual bool FindAttribute(Handle(TDF_Attribute)& theAttribute, const TCollection_AsciiString& theTypeOfAttribute);
  Standard_EXPORT virtual bool ReferencedObject(Handle(SALOMEDSImpl_SObject)& theObject) ;
  Standard_EXPORT virtual bool FindSubObject(int theTag, Handle(SALOMEDSImpl_SObject)& theObject);

  Standard_EXPORT virtual Handle(SALOMEDSImpl_Study) GetStudy() ;
  Standard_EXPORT virtual TCollection_AsciiString Name() { return _name; }
  Standard_EXPORT virtual void Name(const TCollection_AsciiString& theName) { _name = theName; }
  Standard_EXPORT virtual Handle(TColStd_HSequenceOfTransient) GetAllAttributes();

  Standard_EXPORT virtual TCollection_AsciiString GetName();
  Standard_EXPORT virtual TCollection_AsciiString GetComment();
  Standard_EXPORT virtual TCollection_AsciiString GetIOR();

  Standard_EXPORT virtual int Tag() { return _lab.Tag(); }
  Standard_EXPORT virtual int Depth() { return _lab.Depth(); }

  Standard_EXPORT virtual TDF_Label GetLabel() { return _lab; }   

  Standard_EXPORT static Standard_GUID GetGUID(const TCollection_AsciiString& theTypeOfAttribute);

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_SObject )
};
#endif
