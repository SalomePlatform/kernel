//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : PyInterp_PyQt.h
//  Author : Christian CAREMOLI, Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

#ifndef _PYINTERP_PYQT_H_
#define _PYINTERP_PYQT_H_

#include "PyInterp_base.h"

class PyInterp_PyQt : public PyInterp_base
{
 public:
  PyInterp_PyQt();
  ~PyInterp_PyQt();

  void run(const char *command);

 protected:
  virtual void initState();
  virtual void initContext();  
};

#endif
