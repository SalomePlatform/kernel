//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : Plot2d.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef Plot2d_HeaderFile
#define Plot2d_HeaderFile

#include "QAD_RightFrame.h"
#include "QAD_ViewFrame.h"

class Plot2d : public QObject
{
  Q_OBJECT

public :

    Standard_EXPORT static QAD_ViewFrame* createView ( QAD_RightFrame* parent);
  
};

#endif // Plot2d_HeaderFile
