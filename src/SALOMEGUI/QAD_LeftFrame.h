//  File      : QAD_LeftFrame.h
//  Created   : Thu Jun 14 14:51:17 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$

#ifndef QAD_LeftFrame_H
#define QAD_LeftFrame_H

#include "QAD.h"
#include <qwidget.h>
// IDL Headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
 
class QAD_ObjectBrowser;

class QAD_EXPORT QAD_LeftFrame : public QWidget
{
public:
  QAD_LeftFrame( SALOMEDS::Study_var study, QWidget *parent, const char *name );
  ~QAD_LeftFrame();
  
  QAD_ObjectBrowser*  getObjectBrowser() const;

protected:
  QAD_ObjectBrowser*  myObjectBrowser;
};

#endif
