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
//  File   : SALOMEDS_SObject.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDS_SOBJECT_H__
#define __SALOMEDS_SOBJECT_H__

// std C++ headers
#include <vector>

#include <SALOMEDSClient.hxx>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

#include "SALOMEDSImpl_SObject.hxx"

class Standard_EXPORT SALOMEDS_SObject: public virtual SALOMEDSClient_SObject
{
protected:

  bool                         _isLocal;
  SALOMEDSImpl_SObject*        _local_impl;
  SALOMEDS::SObject_var        _corba_impl;
  CORBA::ORB_var               _orb;

public:
  
  SALOMEDS_SObject(SALOMEDS::SObject_ptr theSObject);
  SALOMEDS_SObject(const SALOMEDSImpl_SObject& theSObject);       
  virtual ~SALOMEDS_SObject();

  virtual std::string GetID();
  virtual _PTR(SComponent) GetFatherComponent();
  virtual _PTR(SObject)    GetFather();
  virtual bool FindAttribute(_PTR(GenericAttribute)& anAttribute, const std::string& aTypeOfAttribute);
  virtual bool ReferencedObject(_PTR(SObject)& theObject);
  virtual bool FindSubObject(int theTag, _PTR(SObject)& theObject);
  virtual _PTR(Study) GetStudy();
  virtual std::string Name();
  virtual void  Name(const std::string& theName);
  virtual std::vector<_PTR(GenericAttribute)> GetAllAttributes();
  virtual std::string GetName();
  virtual std::string GetComment();
  virtual std::string GetIOR();
  virtual int   Tag();
  virtual int   Depth();

  CORBA::Object_ptr GetObject();
  SALOMEDS::SObject_ptr GetSObject();

  SALOMEDS::SObject_ptr        GetCORBAImpl() { return SALOMEDS::SObject::_duplicate(_corba_impl); }
  SALOMEDSImpl_SObject*        GetLocalImpl() { return _local_impl; } 

private:
  void init_orb();
};

#endif
