//  File      : QAD_FileValidator.h
//  Created   : UI team, 17.09.02
//  Descr     : File validator for QFileDlg class

//  Modified  : Tue Sep 17 10:40:50 2002
//  Author    : 
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE 2001
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
