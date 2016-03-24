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

//  File   : SALOMEDSImpl_SObject.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef __SALOMEDSIMPL_SOBJECT_H__
#define __SALOMEDSIMPL_SOBJECT_H__

#include "SALOMEDSImpl_Defines.hxx"

#include "DF_Label.hxx"
#include "DF_Attribute.hxx"
#include <string>
#include <vector>

class SALOMEDSImpl_SComponent;
class SALOMEDSImpl_Study;

class SALOMEDSIMPL_EXPORT SALOMEDSImpl_SObject
{
protected:
  DF_Label     _lab;
  std::string  _name;  
  std::string  _value;
  std::string  _type;

public:

  SALOMEDSImpl_SObject();
  SALOMEDSImpl_SObject(const DF_Label& theLabel);
  SALOMEDSImpl_SObject(const SALOMEDSImpl_SObject& theSObject);
  virtual ~SALOMEDSImpl_SObject();

  virtual std::string GetID() const;
  virtual SALOMEDSImpl_SComponent GetFatherComponent() const;
  virtual SALOMEDSImpl_SObject GetFather() const ;
  virtual int  GetLastChildTag() const ;
  virtual bool FindAttribute(DF_Attribute*& theAttribute, const std::string& theTypeOfAttribute) const;
  virtual bool ReferencedObject(SALOMEDSImpl_SObject& theObject) const ;
  virtual bool FindSubObject(int theTag, SALOMEDSImpl_SObject& theObject);

  virtual SALOMEDSImpl_Study*  GetStudy() const;
  virtual std::string Name() const { return _name; }
  virtual void Name(const std::string& theName) { _name = theName; }
  virtual std::vector<DF_Attribute*> GetAllAttributes() const; 

  virtual std::string GetName() const ;
  virtual std::string GetComment() const;
  virtual std::string GetIOR() const;
  virtual void SetAttrString(const std::string& name,const std::string& value);

  virtual int Tag() const { return _lab.Tag(); }
  virtual int Depth() const { return _lab.Depth(); }

  virtual DF_Label GetLabel() const { return _lab; }   

  bool IsNull() const { return _lab.IsNull(); }

  bool IsComponent() const;

  operator SALOMEDSImpl_SComponent() const; 

  operator bool () const { return !IsNull(); }

  SALOMEDSImpl_SObject* GetPersistentCopy() const;

  static std::string GetGUID(const std::string& theTypeOfAttribute);


};
#endif
