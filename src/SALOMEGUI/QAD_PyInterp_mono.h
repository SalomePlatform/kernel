//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : QAD_PyInterp_mono.h
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

#ifndef _QAD_PYINTERP_MONO_H_
#define _QAD_PYINTERP_MONO_H_

#include "PyInterp_base.h" // this include must be first (see PyInterp_base.h)!

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
