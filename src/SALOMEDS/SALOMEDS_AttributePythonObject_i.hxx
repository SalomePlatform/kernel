//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEDS_AttributePythonObject_i.hxx
//  Author : Michael Ponikarov
//  Module : SALOME
//  $Header$

#ifndef SALOMEDS_AttributePythonObject_i_HeaderFile
#define SALOMEDS_AttributePythonObject_i_HeaderFile

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDS_PythonObjectAttribute.hxx"

DEFINE_DERIVED_ATTR(AttributePythonObject,SALOMEDS_PythonObjectAttribute,true);

class SALOMEDS_AttributePythonObject_i: 
  public virtual POA_SALOMEDS::AttributePythonObject,
  public virtual SALOMEDS_TAttributePythonObject_i 
{
  DEFINE_DERIVED_ATTR_METH_DEFAULT(AttributePythonObject,SALOMEDS_PythonObjectAttribute);
public:
  virtual void SetObject(const char* theSequence, CORBA::Boolean IsScript);
  virtual char* GetObject();
  virtual CORBA::Boolean IsScript();

  virtual char* Store();
  virtual void Restore(const char*);

};

#endif
