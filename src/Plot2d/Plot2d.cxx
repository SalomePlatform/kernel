//  File      : Plot2d.cxx
//  Created   : Wed Mar 20 11:56:18 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : OCCViewer
//  Copyright : Open CASCADE 2002
//  $Header$

#include "Plot2d.h"
#include "Plot2d_ViewFrame.h"

QAD_ViewFrame* Plot2d::createView(QAD_RightFrame* parent)
{
  return new Plot2d_ViewFrame( parent, "plotView" ); 
}

extern "C"
{
  QAD_ViewFrame* createView(QAD_RightFrame* parent)
  {
    return Plot2d::createView(parent);
  }
}
