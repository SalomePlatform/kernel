// File:	SALOMEDS_AttributeUserID_i.hxx
//  Created   : Tue Aug 13 15:05:03 2002
// Author:	Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header:

#ifndef SALOMEDS_AttributeUserID_i_HeaderFile
#define SALOMEDS_AttributeUserID_i_HeaderFile

// IDL headers

#include <TDataStd_UAttribute.hxx>
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include "SALOMEDS_GenericAttribute_i.hxx"

class SALOMEDS_AttributeUserID_i: public virtual POA_SALOMEDS::AttributeUserID,
                                  public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeUserID_i(const Handle(TDataStd_UAttribute)& theUAttr, CORBA::ORB_ptr orb) 
  {
    _myOrb = CORBA::ORB::_duplicate(orb);
    _myAttr = theUAttr;
  };
  ~SALOMEDS_AttributeUserID_i() {};

  char* Value();
  void SetValue(const char* value);

  static const Standard_GUID& DefaultID() {
    static Standard_GUID SALOMEDS_DefaultUserAttributeID ("FFFFFFFF-D9CD-11d6-945D-1050DA506788");
    return SALOMEDS_DefaultUserAttributeID;
  }
};




#endif
