//  File      : SALOMEDS_SComponent_i.hxx
//  Created   : Wed Nov 28 16:24:50 2001
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header$

#ifndef __SALOMEDS_SCOMPONENT_I_H__
#define __SALOMEDS_SCOMPONENT_I_H__

// std C++ headers
#include <iostream.h>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

//SALOMEDS headers
#include "SALOMEDS_SObject_i.hxx"
#include "SALOMEDS_IORAttribute.hxx"

// Cascade headers
#include <TDF_Label.hxx>
#include <TDataStd_Name.hxx>
#include <TDataStd_Comment.hxx>
#include <TCollection_AsciiString.hxx> 
#include <TDF_Tool.hxx>
#include <stdio.h>

class SALOMEDS_SComponent_i: public POA_SALOMEDS::SComponent,
			     public SALOMEDS_SObject_i
{
public:
  
  SALOMEDS_SComponent_i(const TDF_Label lab, CORBA::ORB_ptr);
  
  ~SALOMEDS_SComponent_i();
  

  virtual char* ComponentDataType();
  virtual CORBA::Boolean ComponentIOR(CORBA::String_out theID);

  static Standard_Boolean IsA(const TDF_Label Lab);
    

};
#endif
