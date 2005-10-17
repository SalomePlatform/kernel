//  File   : SALOMEDS_AttributeName_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//  $Header:

#ifndef SALOMEDS_AttributeName_i_HeaderFile
#define SALOMEDS_AttributeName_i_HeaderFile

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeName.hxx"

class SALOMEDS_AttributeName_i: public virtual POA_SALOMEDS::AttributeName,
				public virtual SALOMEDS_GenericAttribute_i 
{
public:
  
  SALOMEDS_AttributeName_i(const Handle(SALOMEDSImpl_AttributeName)& theAttr, CORBA::ORB_ptr orb) 
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {}; 

  ~SALOMEDS_AttributeName_i() {};

  char* Value();
  void SetValue(const char* value);

};




#endif
