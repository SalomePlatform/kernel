using namespace std;
//  File      : SUPERVGraph.cxx
//  Created   : Wed Mar 20 11:58:44 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SUPERVGraph
//  Copyright : Open CASCADE 2002
//  $Header$

#include "SUPERVGraph.h"
#include "SUPERVGraph_ViewFrame.h"

QAD_ViewFrame* SUPERVGraph::createView(QAD_RightFrame* parent)
{
  return new SUPERVGraph_ViewFrame( parent, "vtkView" ); 
}

extern "C"
{
  QAD_ViewFrame* createView(QAD_RightFrame* parent)
  {
    return SUPERVGraph::createView(parent);
  }
}
