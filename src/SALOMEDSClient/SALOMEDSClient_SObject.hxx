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

//  File   : SALOMEDSClient_SObject.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef __SALOMEDSCLIENT_SOBJECT_H__
#define __SALOMEDSCLIENT_SOBJECT_H__

// std C++ headers
#include <vector>
#include <string> 

#include "SALOMEDSClient_definitions.hxx"

class SALOMEDSClient_Study;
class SALOMEDSClient_SComponent;
class SALOMEDSClient_GenericAttribute;

class SALOMEDSClient_SObject
{
public:
  virtual ~SALOMEDSClient_SObject() {}

  virtual bool IsNull() const = 0;
  virtual std::string GetID()  = 0;
  virtual _PTR(SComponent) GetFatherComponent() = 0;
  virtual _PTR(SObject)    GetFather() = 0;
  virtual bool FindAttribute(_PTR(GenericAttribute)& anAttribute, const std::string& aTypeOfAttribute) = 0;
  virtual bool ReferencedObject(_PTR(SObject)& theObject) = 0;
  virtual bool FindSubObject(int theTag, _PTR(SObject)& theObject) = 0;
  virtual _PTR(Study) GetStudy() = 0;
  virtual std::string Name() = 0;
  virtual void  Name(const std::string& theName)  = 0;
  virtual std::vector<_PTR(GenericAttribute)> GetAllAttributes() = 0;
  virtual std::string GetName() = 0;
  virtual std::string GetComment() = 0;
  virtual std::string GetIOR() = 0;
  virtual int   Tag() = 0;
  virtual int   Depth() = 0;
};

#endif
