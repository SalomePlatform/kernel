//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : QAD_FileValidator.h
//  Author : 
//  Module : SALOME
//  $Header$

#ifndef QAD_FILEVALIDATOR_H
#define QAD_FILEVALIDATOR_H

#include <qwidget.h>
#include "QAD.h"

class QAD_EXPORT QAD_FileValidator
{
public:
  QAD_FileValidator(QWidget* parent = 0);
  
  virtual bool    canOpen( const QString& file );
  virtual bool    canSave( const QString& file );

 private:
  QWidget*        myParent;
};

#endif
