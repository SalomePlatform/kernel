//  File      : OCCViewer.h
//  Created   : Wed Mar 20 11:56:18 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : OCCViewer
//  Copyright : Open CASCADE 2002
//  $Header$

#ifndef OCCViewer_HeaderFile
#define OCCViewer_HeaderFile

#include "QAD_RightFrame.h"
#include "QAD_ViewFrame.h"

class OCCViewer : public QObject
{
  Q_OBJECT

public :

    Standard_EXPORT static QAD_ViewFrame* createView ( QAD_RightFrame* parent);
  
};

#endif
