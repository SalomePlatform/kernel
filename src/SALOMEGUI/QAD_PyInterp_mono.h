//=============================================================================
// File      : QAD_PyInterp_mono.h
// Created   : ven fév  7 10:01:36 CET 2003
// Author    : Paul RASCLE, EDF
// Project   : SALOME
// Copyright : EDF 2003
// $Header$
//=============================================================================

#ifndef _QAD_PYINTERP_MONO_H_
#define _QAD_PYINTERP_MONO_H_

#include "PyInterp_base.h"

class QAD_PyInterp_mono: public PyInterp_base
{
 public:
  QAD_PyInterp_mono();
  ~QAD_PyInterp_mono();

 protected:
  void initState();
  void initContext();
};

#endif
