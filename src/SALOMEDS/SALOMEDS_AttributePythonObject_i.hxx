// File:	SALOMEDS_AttributePythonObject_i.hxx
// Created:	Tue Oct  8 10:13:30 2002
// Author:	Michael Ponikarov
//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE
//  $Header$

#ifndef SALOMEDS_AttributePythonObject_i_HeaderFile
#define SALOMEDS_AttributePythonObject_i_HeaderFile

// IDL headers

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDS_PythonObjectAttribute.hxx"

class SALOMEDS_AttributePythonObject_i: public virtual POA_SALOMEDS::AttributePythonObject,
                                        public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributePythonObject_i(const Handle(SALOMEDS_PythonObjectAttribute)& theObjectAttr, CORBA::ORB_ptr orb) 
  {
    _myOrb = CORBA::ORB::_duplicate(orb);
    _myAttr = theObjectAttr;
  };
  ~SALOMEDS_AttributePythonObject_i() {};

  virtual void SetObject(const char* theSequence, CORBA::Boolean IsScript);
  virtual char* GetObject();
  virtual CORBA::Boolean IsScript();

};




#endif
