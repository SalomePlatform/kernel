using namespace std;
//  File      : SALOMEDS_SComponent_i.cxx
//  Created   : Wed Nov 28 16:24:34 2001
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header$

#include "SALOMEDS_SComponent_i.hxx"
#include "utilities.h"


//============================================================================
/*! Function : constructor
 *  Purpose  : 
 */
//============================================================================
SALOMEDS_SComponent_i::SALOMEDS_SComponent_i(const TDF_Label lab, CORBA::ORB_ptr orb)
  :SALOMEDS_SObject_i(lab,orb)
{
}
  
//============================================================================
/*! Function : destructor
 *  Purpose  : 
 */
//============================================================================
SALOMEDS_SComponent_i::~SALOMEDS_SComponent_i()
{
}
  
  
//============================================================================
/*! Function : ComponentDataType
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_SComponent_i::ComponentDataType()
{
  //DEB
  //    MESSAGE("In SALOMEDS_SComponent_i::ComponentDataType");
  //    TCollection_AsciiString anEntry;
  //    TDF_Tool::Entry (_lab,anEntry);
  //    MESSAGE("in SALOMEDS_SComponent_i, Entry :"<<anEntry);
  // END DEB

  Standard_CString res = "";
  Handle(TDataStd_Comment) type;
  if ( _lab.FindAttribute(TDataStd_Comment::GetID(),type) ) {
    TCollection_AsciiString ch(type->Get());
    res = ch.ToCString();
  }

  return CORBA::string_dup(res);
}
  

//============================================================================
/*! Function : ComponentIOR
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_SComponent_i::ComponentIOR(CORBA::String_out IOR)
{
  Handle(SALOMEDS_IORAttribute) ior;
  if (!_lab.FindAttribute(SALOMEDS_IORAttribute::GetID(),ior) )
      return false;
  TCollection_AsciiString ch(ior->Get());
  IOR = strdup(ch.ToCString());
  return true;
}
  

//============================================================================
/*! Function : IsA
 *  Purpose  : 
 */
//============================================================================
Standard_Boolean SALOMEDS_SComponent_i::IsA(const TDF_Label Lab)
{
  Handle(TDF_Attribute) Att;
  // scomponent must contain comment and belong to the 2th depth label
  if ( Lab.FindAttribute(TDataStd_Comment::GetID(), Att) && Lab.Depth() == 2) {
    return Standard_True;
  }
  return Standard_False;
}
  
    
