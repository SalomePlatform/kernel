//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEDS_GenericAttribute_i.cxx
//  Author : Mikhail PONIKAROV
//  Module : SALOME
//  $Header$

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDS_SObject_i.hxx"
#include "SALOMEDS_Study_i.hxx"

using namespace std;


SALOMEDS_GenericAttribute_i::SALOMEDS_GenericAttribute_i(const Handle(TDF_Attribute)& theAttr,
							 SALOMEDS_SObject_i* theSObject):
  _myBasicAttr(theAttr),
  _mySObject(theSObject)
{
}


SALOMEDS_GenericAttribute_i::~SALOMEDS_GenericAttribute_i()
{
}


char* SALOMEDS_GenericAttribute_i::Store()
{
  return CORBA::string_dup("");
}


void SALOMEDS_GenericAttribute_i::Restore(const char*)
{
}


char* SALOMEDS_GenericAttribute_i::Type() 
{
  return CORBA::string_dup(SALOMEDS::GetType(_myBasicAttr).c_str());
}


SALOMEDS::SObject_ptr SALOMEDS_GenericAttribute_i::GetSObject()
{
  return _mySObject->_this();;
}


void SALOMEDS_GenericAttribute_i::CheckLocked() 
  throw (SALOMEDS::GenericAttribute::LockProtection) 
{
  _mySObject->GetStudyServant()->CheckLocked();
}
