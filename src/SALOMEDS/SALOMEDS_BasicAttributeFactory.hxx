//=============================================================================
// File      : SALOMEDS_BasicAttributeFactory.hxx
// Created   : ven mai 31 13:45:47 CEST 2002
// Author    : Estelle Deville, CEA
// Project   : SALOME
// Copyright : CEA 2002
// $Header$
//=============================================================================

#ifndef _BASIC_ATTRIBUTEFACTORY_HXX_
#define _BASIC_ATTRIBUTEFACTORY_HXX_
#include "SALOMEDS_BasicAttribute_i.hxx"
// Add new Attribute type here
#include "SALOMEDS_AttReal_i.hxx"
#include "SALOMEDS_AttLong_i.hxx"

#include "Utils_SALOME_Exception.hxx"

class BasicAttributeFactory
{
public:
  BasicAttributeFactory() {};
  virtual ~BasicAttributeFactory() {};

  SALOMEDS_BasicAttribute_i* Create(const char* type)
    throw (SALOME_Exception);
};

#endif
