//  File      : SALOMEDS_SAttribute_i.hxx
//  Created   : Thu Nov 29 21:02:46 2001
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header$

#ifndef __SALOMEDS_SATTRIBUTE_I_H__
#define __SALOMEDS_SATTRIBUTE_I_H__

// std C++ headers
#include <iostream.h>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)



// Cascade headers
#include <TDocStd_Document.hxx>
#include <stdio.h>

class SALOMEDS_SAttribute_i: public POA_IStudy::SAttribute,
			public PortableServer::RefCountServantBase {
private:
  CORBA::ORB_ptr           _orb;
  char*                    _Type;
  char
public:
  
  SALOMEDS_SAttribute_i(const Handle(TDocStd_Document), CORBA::ORB_ptr);
  
  ~SALOMEDS_SAttribute_i();

   virtual char* GetID() ;
   virtual SObject_ptr GetOwner();
   virtual char* GetType();
   virtual char* GetValue();
  
};
#endif
