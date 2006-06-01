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
//  File   : SALOMEDS_SObject_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

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

#ifdef GetObject
#undef GetObject
#endif

class Standard_EXPORT SALOMEDS_SObject_i: public virtual POA_SALOMEDS::SObject,
			  public virtual PortableServer::RefCountServantBase,
			  public virtual SALOME::GenericObj_i
{
protected:
  CORBA::ORB_ptr                _orb;
  Handle(SALOMEDSImpl_SObject)  _impl;

public:

  static SALOMEDS::SObject_ptr New(const Handle(SALOMEDSImpl_SObject)&, CORBA::ORB_ptr); 
  
  SALOMEDS_SObject_i(const Handle(SALOMEDSImpl_SObject)&, CORBA::ORB_ptr);
  
  virtual ~SALOMEDS_SObject_i();
  
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

  virtual CORBA::Short Tag();
  virtual CORBA::Short Depth();

  virtual CORBA::Long GetLocalImpl(const char* theHostname, CORBA::Long thePID, CORBA::Boolean& isLocal);
};

#endif
