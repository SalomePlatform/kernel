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

#include <TDF_Attribute.hxx>
#include <Standard_GUID.hxx>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

class SALOMEDS_SObject_i;

class SALOMEDS_GenericAttribute_i:
  public virtual POA_SALOMEDS::GenericAttribute,
  public virtual PortableServer::RefCountServantBase 
{ 
private: 
  friend class SALOMEDS_SObject_i;

  SALOMEDS_GenericAttribute_i(); // Not implemented
  void operator=(const SALOMEDS_GenericAttribute_i&);  //Not implemented

protected:
  SALOMEDS_SObject_i* _mySObject;
  Handle(TDF_Attribute) _myBasicAttr;

  SALOMEDS_GenericAttribute_i(const Handle(TDF_Attribute)& theAttr,
			      SALOMEDS_SObject_i* theSObject);
  
  virtual ~SALOMEDS_GenericAttribute_i();

  void SetBasicAttribute(const Handle(TDF_Attribute)& theAttr){
    _myBasicAttr = theAttr;
  }
  
public:
  void Restore(const char*);

  char* Store();

  char* Type();
  
  SALOMEDS::SObject_ptr GetSObject();

  void CheckLocked() 
    throw (SALOMEDS::GenericAttribute::LockProtection);

  virtual void SetAttribute(const Handle(TDF_Attribute)& theAttr) = 0;

  virtual const Handle(TDF_Attribute)& GetAttribute() const = 0;

};


template<class TDFAttribute, class TStoreTDFAttribute, bool TIsCheckLockedStudy = true>
class SALOMEDS_TGenericAttribute_i:
  public virtual SALOMEDS_GenericAttribute_i
{
public:
  typedef TDFAttribute TAttr;
  typedef TStoreTDFAttribute TStoreAttr;

  virtual void SetAttribute(const Handle(TDF_Attribute)& theAttr){
    _myAttr = TStoreAttr::DownCast(theAttr);
    SetBasicAttribute(theAttr);
  }
  virtual const Handle(TDF_Attribute)& GetAttribute() const{
    return _myAttr;
  }
  static bool IsCheckLockedStudy(){
    return TIsCheckLockedStudy;
  }
  static Handle(TDF_Attribute) NewAttribute(){
    return new TAttr;
  }

protected:
  TStoreAttr _myAttr;

  SALOMEDS_TGenericAttribute_i(const Handle(TDF_Attribute)& theAttr,
			       SALOMEDS_SObject_i* theSObject):
    SALOMEDS_GenericAttribute_i(theAttr,theSObject),
    _myAttr(TStoreAttr::DownCast(theAttr))
  {
  }
  
private: 
  friend class SALOMEDS_SObject_i;

  SALOMEDS_TGenericAttribute_i(); //Not implemented
  void operator=(const SALOMEDS_TGenericAttribute_i&);  //Not implemented

};


#define DEFINE_DERIVED_ATTR(TName,TAttr,TCheck) \
  typedef SALOMEDS_TGenericAttribute_i<TAttr,Handle_##TAttr,TCheck> \
    SALOMEDS_T##TName##_i


#define DEFINE_DERIVED_ATTR_METH_BASE(TName) \
public: \
  friend class SALOMEDS_SObject_i; \
  static SALOMEDS_GenericAttribute_i* \
    NewInstance(const Handle(TDF_Attribute)& theAttr, \
                SALOMEDS_SObject_i* theSObject) \
      { return new SALOMEDS_##TName##_i(theAttr,theSObject);} \
private: \
  SALOMEDS_##TName##_i(const Handle(TDF_Attribute)& theAttr, \
                       SALOMEDS_SObject_i* theSObject): \
    SALOMEDS_GenericAttribute_i(theAttr,theSObject), \
    SALOMEDS_T##TName##_i(theAttr,theSObject) \
    {} \
  void operator=(const SALOMEDS_##TName##_i&); \
  SALOMEDS_##TName##_i()


#define DEFINE_DERIVED_ATTR_METH_DEFAULT(TName,TAttr) \
DEFINE_DERIVED_ATTR_METH_BASE(TName); \
public: \
  static Standard_GUID GetGUID(){ return TAttr::GetID(); } \
private:


#define DEFINE_DERIVED_ATTR_METH(TName,theGUID) \
DEFINE_DERIVED_ATTR_METH_BASE(TName); \
public: \
  static Standard_GUID GetGUID(){ return theGUID; } \
private:


#endif
