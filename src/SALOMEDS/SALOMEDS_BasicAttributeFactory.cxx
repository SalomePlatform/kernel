using namespace std;
//=============================================================================
// File      : SALOMEDS_BasicAttributeFactory.cxx
// Created   : ven mai 31 13:45:47 CEST 2002
// Author    : Estelle Deville, CEA
// Project   : SALOME
// Copyright : CEA 2002
// $Header$
//=============================================================================

#include "SALOMEDS_BasicAttributeFactory.hxx"
#include "utilities.h"

//============================================================================
/*! Function : Create
 *  Purpose  : create new basic attributes
 */
//============================================================================
SALOMEDS_BasicAttribute_i* BasicAttributeFactory::Create(const char* type)
    throw (SALOME_Exception)
{
  if (!strcmp(type,"AttReal"))
    {  
      SALOMEDS_AttReal_i *A = new SALOMEDS_AttReal_i();
      return A;
    }
  else if(!strcmp(type,"AttLong"))
    {
      SALOMEDS_AttLong_i *A = new SALOMEDS_AttLong_i();
      return A;
    }
  else throw(SALOME_Exception(LOCALIZED("bad attribute type name")));

}
