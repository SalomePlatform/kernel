//=============================================================================
// File      : PyInterp_PyQt.h
// Created   : ven fév  7 10:01:36 CET 2003
// Author    : Christian CAREMOLI, Paul RASCLE, EDF
// Project   : SALOME
// Copyright : EDF 2003
// $Header$
//=============================================================================

#ifndef _PYINTERP_PYQT_H_
#define _PYINTERP_PYQT_H_

#include "PyInterp_base.h"

class PyInterp_PyQt : public PyInterp_base
{
 public:
  PyInterp_PyQt();
  ~PyInterp_PyQt();
  void enter();
  void quit();
  void run(const char *command);

 protected:
  void initState();
  void initContext();  
};

#endif
