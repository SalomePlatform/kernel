//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : SALOMEDS_GenericAttribute_i.hxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#ifndef _GENERIC_ATTRIBUTE_I_HXX_
#define _GENERIC_ATTRIBUTE_I_HXX_

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include <TDF_Attribute.hxx>
#include "SALOMEDS_SObject_i.hxx"

class SALOMEDS_GenericAttribute_i: public POA_SALOMEDS::GenericAttribute,
				   public PortableServer::RefCountServantBase {
protected:
  Handle(TDF_Attribute) _myAttr;
  CORBA::ORB_ptr        _myOrb;
public:
  SALOMEDS_GenericAttribute_i() {};
  
  void CheckLocked() throw (SALOMEDS::GenericAttribute::LockProtection);

  char* Store() {return "";};

  void Restore(const char*) {};
  
  char* Type();

  SALOMEDS::SObject_ptr GetSObject();

  ~SALOMEDS_GenericAttribute_i() {};

  static Standard_GUID GetGUID(const char* theType);

  static SALOMEDS::GenericAttribute_ptr CreateAttribute(CORBA::ORB_ptr theOrb, const Handle(TDF_Attribute)& theAttr);
};

// defines for creation attributes objects

//MESSAGE("*** Create new CORBA attribute for "<<#CORBA_Name);
#define __ReturnCORBAAttribute(OCAF_Name, CORBA_Name) if (theAttr->ID() == OCAF_Name::GetID()) { \
    SALOMEDS_##CORBA_Name##_i* Attr = new SALOMEDS_##CORBA_Name##_i(Handle(OCAF_Name)::DownCast(theAttr), theOrb); \
    return Attr->CORBA_Name::_this(); \
  }

//MESSAGE("Create New Attribute "<<#CORBA_Name);
#define __FindOrCreateAttribute(OCAF_Name, CORBA_Name) if (strcmp(aTypeOfAttribute, #CORBA_Name) == 0) { \
    Handle(OCAF_Name) anAttr; \
    if (!Lab.FindAttribute(OCAF_Name::GetID(), anAttr)) { \
      anAttr = new OCAF_Name; \
      Lab.AddAttribute(anAttr); \
    } \
    SALOMEDS_##CORBA_Name##_i* Attr = new SALOMEDS_##CORBA_Name##_i(anAttr, _orb); \
    return Attr->CORBA_Name::_this(); \
  }

//MESSAGE("Create New Attribute "<<#CORBA_Name);
#define __FindOrCreateAttributeLocked(OCAF_Name, CORBA_Name) if (strcmp(aTypeOfAttribute, #CORBA_Name) == 0) { \
    Handle(OCAF_Name) anAttr; \
    if (!Lab.FindAttribute(OCAF_Name::GetID(), anAttr)) { \
      CheckLocked(); \
      anAttr = new OCAF_Name; \
      Lab.AddAttribute(anAttr); \
    } \
    SALOMEDS_##CORBA_Name##_i* Attr = new SALOMEDS_##CORBA_Name##_i(anAttr, _orb); \
    return Attr->CORBA_Name::_this(); \
  }

#endif
