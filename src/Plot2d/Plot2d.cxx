//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : Plot2d.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "Plot2d.h"
#include "Plot2d_ViewFrame.h"
using namespace std;

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
