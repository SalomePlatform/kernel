//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : Plot2d_CurveContainer.h
//  Author : Vadim SANDLER
//  Module : SALOME
//  $Header$

#ifndef Plot2d_CurveContainer_h
#define Plot2d_CurveContainer_h

#include "Plot2d_Curve.h"
#include <qlist.h>

class Plot2d_CurveContainer 
{
public:
  Plot2d_CurveContainer();
  ~Plot2d_CurveContainer();

  void                addCurve( Plot2d_Curve* curve );
  void                removeCurve( const int index, bool alsoDelete = false ) ;
  void                clear( bool alsoDelete = false );
  int                 count();
  bool                isEmpty() const;
  Plot2d_Curve*       curve( const int index );

private:
  QList<Plot2d_Curve>  myCurves;
};

#endif // Plot2d_CurveContainer_h


