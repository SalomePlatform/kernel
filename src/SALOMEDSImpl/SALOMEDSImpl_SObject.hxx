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
//  File   : SALOMEDSImpl_SObject.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSIMPL_SOBJECT_H__
#define __SALOMEDSIMPL_SOBJECT_H__

#include "DF_Label.hxx"
#include "DF_Attribute.hxx"
#include <string>
#include <vector>

class SALOMEDSImpl_SComponent;
class SALOMEDSImpl_Study;

class SALOMEDSImpl_SObject
{
protected:
  DF_Label     _lab;
  std::string  _name;  
  std::string  _value;
  std::string  _type;

public:
  
  Standard_EXPORT SALOMEDSImpl_SObject();
  Standard_EXPORT SALOMEDSImpl_SObject(const DF_Label& theLabel);
  Standard_EXPORT SALOMEDSImpl_SObject(const SALOMEDSImpl_SObject& theSObject);
  Standard_EXPORT virtual ~SALOMEDSImpl_SObject();
  
  Standard_EXPORT virtual std::string GetID() const;
  Standard_EXPORT virtual SALOMEDSImpl_SComponent GetFatherComponent() const;
  Standard_EXPORT virtual SALOMEDSImpl_SObject GetFather() const ;
  Standard_EXPORT virtual bool FindAttribute(DF_Attribute*& theAttribute, const std::string& theTypeOfAttribute) const;
  Standard_EXPORT virtual bool ReferencedObject(SALOMEDSImpl_SObject& theObject) const ;
  Standard_EXPORT virtual bool FindSubObject(int theTag, SALOMEDSImpl_SObject& theObject);

  Standard_EXPORT virtual SALOMEDSImpl_Study*  GetStudy() const;
  Standard_EXPORT virtual std::string Name() const { return _name; }
  Standard_EXPORT virtual void Name(const std::string& theName) { _name = theName; }
  Standard_EXPORT virtual std::vector<DF_Attribute*> GetAllAttributes() const; 

  Standard_EXPORT virtual std::string GetName() const ;
  Standard_EXPORT virtual std::string GetComment() const;
  Standard_EXPORT virtual std::string GetIOR() const;

  Standard_EXPORT virtual int Tag() const { return _lab.Tag(); }
  Standard_EXPORT virtual int Depth() const { return _lab.Depth(); }

  Standard_EXPORT virtual DF_Label GetLabel() const { return _lab; }   

  Standard_EXPORT bool IsNull() const { return _lab.IsNull(); }

  Standard_EXPORT bool IsComponent() const;

  Standard_EXPORT operator SALOMEDSImpl_SComponent() const; 

  Standard_EXPORT operator bool () const { return !IsNull(); }
  
  Standard_EXPORT SALOMEDSImpl_SObject* GetPersistentCopy() const;

  Standard_EXPORT static std::string GetGUID(const std::string& theTypeOfAttribute);

  
};
#endif
