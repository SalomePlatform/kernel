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
//  File   : SALOMEDS_SObject_i.hxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#ifndef __SALOMEDS_SOBJECT_I_H__
#define __SALOMEDS_SOBJECT_I_H__

// std C++ headers
#include <iostream>
#include <vector>
#include <string>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

// Cascade headers
#include <TDF_Label.hxx>
#include <stdio.h>
#include <TDF_Tool.hxx>
#include <TDF_Reference.hxx>
#include <Standard_GUID.hxx>
#include <Standard_NoSuchObject.hxx>
#include <TDataStd_Name.hxx>
#include <TDataStd_Comment.hxx>
#include <TDataStd_Integer.hxx>
#include <TDataStd_Real.hxx>

#include <TCollection_AsciiString.hxx>

class SALOMEDS_SObject_i: public POA_SALOMEDS::SObject,
			  public PortableServer::RefCountServantBase {
protected:
  CORBA::ORB_ptr           _orb;
  TDF_Label                _lab;
  char*                    _name;  
  char*                    _value;
  char*                    _type;
  std::vector<std::string>      _liste_ba_type;

public:
  
  SALOMEDS_SObject_i(const TDF_Label, CORBA::ORB_ptr);
  
  ~SALOMEDS_SObject_i();
  
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


  virtual CORBA::Short Tag();

  static char* AttributeIDToType(Standard_GUID);
  
  static Standard_GUID           ReturnGUIDForAttribute(const char* aTypeOfAttribute);

};
#endif
