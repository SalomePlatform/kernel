//=============================================================================
//  File      : QAD_PyInterp.h
//  Created   : Thu Jun 14 14:04:27 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$
//=============================================================================

#ifndef _QAD_PYINTERP_H_
#define _QAD_PYINTERP_H_

#include "PyInterp_base.h"

class QAD_PyInterp : public PyInterp_base
{
 public:
  QAD_PyInterp();
  ~QAD_PyInterp();

 protected:
  void initState();
  void initContext();  
};

#endif
