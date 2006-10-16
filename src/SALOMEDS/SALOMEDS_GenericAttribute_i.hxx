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
//  File   : SALOMEDS_GenericAttribute_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _GENERIC_ATTRIBUTE_I_HXX_
#define _GENERIC_ATTRIBUTE_I_HXX_

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include <SALOME_GenericObj_i.hh>
#include <TDF_Attribute.hxx>
#include "SALOMEDS_SObject_i.hxx"
#include "SALOMEDSImpl_GenericAttribute.hxx"

class Standard_EXPORT SALOMEDS_GenericAttribute_i: public virtual POA_SALOMEDS::GenericAttribute,
				   public virtual PortableServer::RefCountServantBase,
				   public virtual SALOME::GenericObj_i
{

protected:
  Handle(TDF_Attribute) _impl;
  CORBA::ORB_ptr        _orb;

public:
  SALOMEDS_GenericAttribute_i(const Handle(TDF_Attribute)& theImpl, CORBA::ORB_ptr theOrb);
  virtual ~SALOMEDS_GenericAttribute_i() {};

  void CheckLocked() throw (SALOMEDS::GenericAttribute::LockProtection);
  
  virtual char* Type();

  char* GetClassType();

  SALOMEDS::SObject_ptr GetSObject();
  
  virtual Handle(TDF_Attribute) GetImpl() { return _impl; }

  static SALOMEDS::GenericAttribute_ptr CreateAttribute(const Handle(TDF_Attribute)& theAttr, CORBA::ORB_ptr theOrb);  

  virtual CORBA::LongLong GetLocalImpl(const char* theHostname, CORBA::Long thePID, CORBA::Boolean& isLocal);
};

#endif
