//  File      : SALOMEDS_AttributeComment_i.hxx
//  Created   : Fri Jul 05 10:57:20 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$


#ifndef SALOMEDS_AttributeComment_i_HeaderFile
#define SALOMEDS_AttributeComment_i_HeaderFile

// IDL headers
#include <TDataStd_Comment.hxx>
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include "SALOMEDS_GenericAttribute_i.hxx"

class SALOMEDS_AttributeComment_i: public virtual POA_SALOMEDS::AttributeComment,
				   public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeComment_i(const Handle(TDataStd_Comment)& theCommentAttr, CORBA::ORB_ptr orb) 
  {
    _myOrb = CORBA::ORB::_duplicate(orb);
    _myAttr = theCommentAttr;
  }
  ~SALOMEDS_AttributeComment_i() {};

  char* Value();
  void SetValue(const char* value);
  
};



#endif
