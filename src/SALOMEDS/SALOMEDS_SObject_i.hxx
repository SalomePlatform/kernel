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

#include <map>
#include <string>

// Cascade headers
#include <TDF_Label.hxx>
#include <TDocStd_Document.hxx>
#include <Standard_GUID.hxx>

#include "SALOMEDS_Study_i.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

class SALOMEDS_GenericAttribute_i;


namespace SALOMEDS
{
  const char* Str(const TCollection_ExtendedString& theString);

  std::string GetType(const Handle(TDF_Attribute)& theAttr);

  Standard_GUID GetGUID(const char* theType);

}


class SALOMEDS_SObject_i: public virtual POA_SALOMEDS::SObject,
			  public virtual PortableServer::RefCountServantBase 
{
public:
  static 
  SALOMEDS_Study_i::TSObjectHolder 
  New(SALOMEDS_Study_i* theStudy,
      const TDF_Label& theLabel);

  static 
  SALOMEDS_SObject_i* 
  NewPtr(SALOMEDS_Study_i* theStudy,
	 const TDF_Label& theLabel);

  static 
  SALOMEDS::SObject_var 
  NewRef(SALOMEDS_Study_i* theStudy,
	 const TDF_Label& theLabel);

  virtual SALOMEDS::SObject_ptr GetFather() ;
  virtual SALOMEDS::SComponent_ptr GetFatherComponent();
  virtual CORBA::Boolean ReferencedObject(SALOMEDS::SObject_out theSObject);
  virtual CORBA::Boolean FindSubObject(CORBA::Long theTag, SALOMEDS::SObject_out theSObject);

  virtual SALOMEDS::Study_ptr GetStudy();
  virtual SALOMEDS::ListOfAttributes* GetAllAttributes();

  virtual CORBA::Object_ptr GetObject();

  virtual char* GetID();
  virtual CORBA::Short Tag();
  virtual CORBA::Short Depth();

  virtual char* Name();
  virtual void Name(const char* theName);

  virtual char* GetName();
  virtual char* GetComment();
  virtual char* GetIOR();

  CORBA::Boolean 
  FindAttribute(SALOMEDS::GenericAttribute_out theAttribute, 
		const char* theTypeOfAttribute);
  
  SALOMEDS::GenericAttribute_ptr 
  FindOrCreateAttribute(const char* theTypeOfAttribute);

  void RemoveAttribute(const char* theTypeOfAttribute);
  void OnRemove();

  SALOMEDS_Study_i* GetStudyServant(){ return _study;}

  TDF_Label GetLabel(){ return _lab;}

  CORBA::ORB_var GetORB() const;

  PortableServer::POA_var GetPOA() const;
  
protected:
  friend class SALOMEDS_GenericAttribute_i;

  typedef std::string TAttributeID;
  typedef std::pair<SALOMEDS_GenericAttribute_i*,SALOMEDS::GenericAttribute_var> TAttrHolder;
  typedef std::map<TAttributeID,TAttrHolder> TAttrMap;
  TAttrMap myAttrMap;

  TAttrHolder 
  _FindGenAttribute(const Handle(TDF_Attribute)& theAttr);

  TAttrHolder 
  _CreateGenAttribute(const Handle(TDF_Attribute)& theAttr,
		      const char* theTypeOfAttribute);

  TAttrHolder 
  _FindGenAttribute(const char* theTypeOfAttribute);

  SALOMEDS::GenericAttribute_ptr 
  _FindCORBAAttribute(const char* theTypeOfAttribute);

  Handle(TDF_Attribute) 
    _AddAttribute(const char* theTypeOfAttribute);

  SALOMEDS_Study_i* _study;
  std::string _name;
  TDF_Label _lab;

  SALOMEDS_SObject_i(SALOMEDS_Study_i* theStudy, 
		     const TDF_Label& theLabel);
  
  ~SALOMEDS_SObject_i();

private:
  SALOMEDS_SObject_i(); // Not implemented
  void operator=(const SALOMEDS_SObject_i&); // Not implemented

};


#endif
