using namespace std;
//  File      : OCCViewer.cxx
//  Created   : Wed Mar 20 11:56:18 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : OCCViewer
//  Copyright : Open CASCADE 2002
//  $Header$

#include "OCCViewer.h"
#include "OCCViewer_ViewFrame.h"

QAD_ViewFrame* OCCViewer::createView(QAD_RightFrame* parent)
{
  return new OCCViewer_ViewFrame( parent, "occView" ); 
}

extern "C"
{
  QAD_ViewFrame* createView(QAD_RightFrame* parent)
  {
    return OCCViewer::createView(parent);
  }
}
