using namespace std;
//=============================================================================
// File      : SALOMEDS_BasicAttribute_i.cxx
// Created   : ven mai 31 13:45:47 CEST 2002
// Author    : Estelle Deville, CEA
// Project   : SALOME
// Copyright : CEA 2002
// $Header$
//=============================================================================


#include "SALOMEDS_BasicAttribute_i.hxx"

//============================================================================
/*! Function : SetLabel
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_BasicAttribute_i::SetLabel(const TDF_Label Lab)
{
  _Lab = Lab;
}

