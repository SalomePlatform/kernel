//  File      : QAD_Message.h
//  Created   : Thu Jun 14 15:54:52 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$

#ifndef QAD_Message_H
#define QAD_Message_H

#include "QAD.h"
#include <qmultilineedit.h>

class QAD_EXPORT QAD_Message : public QMultiLineEdit
{
public:
// constructor
  QAD_Message(QWidget *parent = 0, const char* name = 0 );
// destructor
  ~QAD_Message();

// adds message
  void setMessage(const QString& s); 
// sets prompt
  void setPrompt(const QString& prompt);

protected:
// MousePress event handler
  void mousePressEvent(QMouseEvent* event);

private:
// Title
  QString myTitle;
// Prompt
  QString myPrompt;
};

#endif
