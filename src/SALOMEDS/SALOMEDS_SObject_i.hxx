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

//  File   : SALOMEDS_SObject_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef __SALOMEDS_SOBJECT_I_H__
#define __SALOMEDS_SOBJECT_I_H__

// std C++ headers
#include <iostream>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include <SALOME_GenericObj_i.hh>

// Cascade headers
#include "SALOMEDSImpl_SObject.hxx"

class Standard_EXPORT SALOMEDS_SObject_i: public virtual POA_SALOMEDS::SObject,
                          public virtual PortableServer::ServantBase,
                          public virtual SALOME::GenericObj_i
{
protected:
  CORBA::ORB_var               _orb;
  SALOMEDSImpl_SObject*        _impl;

public:

  static SALOMEDS::SObject_ptr New(const SALOMEDSImpl_SObject&, CORBA::ORB_ptr); 
  
  SALOMEDS_SObject_i(const SALOMEDSImpl_SObject&, CORBA::ORB_ptr);
  
  virtual ~SALOMEDS_SObject_i();
  
  virtual CORBA::Boolean IsNull();
  virtual char* GetID();
  virtual SALOMEDS::SComponent_ptr GetFatherComponent();
  virtual SALOMEDS::SObject_ptr    GetFather() ;
  virtual CORBA::Boolean FindAttribute(SALOMEDS::GenericAttribute_out anAttribute, const char* aTypeOfAttribute);
  virtual CORBA::Boolean ReferencedObject(SALOMEDS::SObject_out obj) ;
  virtual CORBA::Boolean FindSubObject(CORBA::Long atag, SALOMEDS::SObject_out obj );

  virtual SALOMEDS::Study_ptr    GetStudy() ;
  virtual char* Name();
  virtual void  Name(const char*);
  virtual SALOMEDS::ListOfAttributes* GetAllAttributes();

  virtual CORBA::Object_ptr GetObject();

  virtual char* GetName();
  virtual char* GetComment();
  virtual char* GetIOR();
  virtual void SetAttrString(const char*, const char*);

  virtual CORBA::Short Tag();
  virtual CORBA::Short GetLastChildTag();
  virtual CORBA::Short Depth();

  virtual CORBA::LongLong GetLocalImpl(const char* theHostname, CORBA::Long thePID, CORBA::Boolean& isLocal);
};

#endif
