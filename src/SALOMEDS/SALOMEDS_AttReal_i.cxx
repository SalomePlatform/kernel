using namespace std;
//  File      : SALOMEDS_AttReal_i.cxx
//  Created   : Mon Jun 10 10:54:58 CEST 2002 
//  Author    : Estelle Deville

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : CEA 2002
//  $Header$



#include "SALOMEDS_AttReal_i.hxx"
#include "utilities.h"
#include <TDF_Tool.hxx>
#include <stdio.h>
#include <cstring>

//============================================================================
/*! Function : Set
 *  Purpose  : Affect a value to the basic attribute
 */
//============================================================================
void SALOMEDS_AttReal_i::Set(CORBA::Double r)
{
  CheckLocked();
  TDataStd_Real::Set (_Lab,r);
}

//============================================================================
/*! Function : Get
 *  Purpose  : Get basic attribute's value
 */
//============================================================================
CORBA::Double SALOMEDS_AttReal_i::Get()
{
  Handle(TDataStd_Real) Att;
  CORBA::Double x;
  if (_Lab.FindAttribute(TDataStd_Real::GetID(),Att))
    x = Att->Get ();
  return x;
}

//============================================================================
/*! Function : Save
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_AttReal_i::Save()
{
  BEGIN_OF("SALOMEDS_AttReal_i::Save");
  CORBA::Double x = this->Get();
  char* out= new char[12];
  sprintf(out,"%g",x);
  return out; 
}

//============================================================================
/*! Function : Load
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_AttReal_i::Load()
{
  BEGIN_OF("SALOMEDS_AttReal_i::Load");
}

//============================================================================
/*! Function : GetPtr
 *  Purpose  : return sobject associated to the basic attribute (if exist)
 */
//============================================================================
CORBA::Boolean SALOMEDS_AttReal_i::GetPtr(SALOMEDS::SObject_ptr anObject)
{
  Handle(TDataStd_Real) Att;
  return _Lab.FindAttribute(TDataStd_Real::GetID(),Att);
}

//============================================================================
/*! Function : GetType
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_AttReal_i::GetType()
{
  return CORBA::string_dup(_type);
}
