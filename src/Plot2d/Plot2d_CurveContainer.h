//  File      : Plot2d_CurveContainer.h
//  Created   : UI team, 05.09.00
//  Descrip   : Curve container class

//  Modified  : Mon Dec 03 15:37:21 2001
//  Author    : Vadim SANDLER
//  Project   : SALOME
//  Module    : Plot2d
//  Copyright : Open CASCADE 2001
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
  bool                isEmpty();
  Plot2d_Curve*       curve( const int index );

private:
  QList<Plot2d_Curve>  myCurves;
};

#endif // Plot2d_CurveContainer_h


