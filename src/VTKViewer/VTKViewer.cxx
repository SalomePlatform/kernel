using namespace std;
//  File      : VTKViewer.cxx
//  Created   : Wed Mar 20 11:58:44 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : VTKViewer
//  Copyright : Open CASCADE 2002
//  $Header$

#include "VTKViewer.h"
#include "VTKViewer_ViewFrame.h"

QAD_ViewFrame* VTKViewer::createView(QAD_RightFrame* parent)
{
  return new VTKViewer_ViewFrame( parent, "vtkView" ); 
}

extern "C"
{
  QAD_ViewFrame* createView(QAD_RightFrame* parent)
  {
    return VTKViewer::createView(parent);
  }
}
