using namespace std;
//  File      : SALOMEDS_AttLong_i.cxx
//  Created   : Mon Jun 10 10:54:58 CEST 2002 
//  Author    : Estelle Deville

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : CEA 2002
//  $Header$



#include "SALOMEDS_AttLong_i.hxx"
#include "utilities.h"
#include <TDF_Tool.hxx>
#include <stdio.h>

//============================================================================
/*! Function : Set
 *  Purpose  : Affect a value to the basic attribute
 */
//============================================================================
void SALOMEDS_AttLong_i::Set(CORBA::Long i)
{
  CheckLocked();
  TDataStd_Integer::Set (_Lab,i);
}

//============================================================================
/*! Function : Get
 *  Purpose  : Get basic attribute's value
 */
//============================================================================
CORBA::Long SALOMEDS_AttLong_i::Get()
{
  Handle(TDataStd_Integer) Att;
  CORBA::Long x;
  if (_Lab.FindAttribute(TDataStd_Integer::GetID(),Att))
    x = Att->Get ();
  return x;
}

//============================================================================
/*! Function : Save
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_AttLong_i::Save()
{
  BEGIN_OF("SALOMEDS_AttLong_i::Save");
  CORBA::Long x = this->Get();
  char* out= new char[12];
  sprintf(out,"%ld",x);
  return out; 
}

//============================================================================
/*! Function : Load
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_AttLong_i::Load()
{
  BEGIN_OF("SALOMEDS_AttLong_i::Load");
}

//============================================================================
/*! Function : GetPtr
 *  Purpose  : return sobject associated to the basic attribute (if exist)
 */
//============================================================================
CORBA::Boolean SALOMEDS_AttLong_i::GetPtr(SALOMEDS::SObject_ptr anObject)
{
  Handle(TDataStd_Integer) Att;
  return _Lab.FindAttribute(TDataStd_Integer::GetID(),Att);
}

//============================================================================
/*! Function : GetType
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_AttLong_i::GetType()
{
  return CORBA::string_dup(_type);
}
