//  File      : SUPERVGraph.h
//  Created   : Wed Mar 20 11:56:18 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SUPERVGraph
//  Copyright : Open CASCADE 2002
//  $Header$

#ifndef SUPERVGraph_HeaderFile
#define SUPERVGraph_HeaderFile

#include "QAD_RightFrame.h"
#include "QAD_ViewFrame.h"

class SUPERVGraph : public QObject
{
  Q_OBJECT

public :

    Standard_EXPORT static QAD_ViewFrame* createView ( QAD_RightFrame* parent);
  
};

#endif
