//  File      : QAD_PyEditor.h
//  Created   : Thu Jun 14 16:05:46 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$

#ifndef QAD_PyEditor_H
#define QAD_PyEditor_H

#include <qmultilineedit.h>

class QAD_PyInterp;

class QAD_PyEditor : public QMultiLineEdit
{
  Q_OBJECT

public:
  QAD_PyEditor(QAD_PyInterp* interp, QWidget *parent=0, const char *name=0);
  ~QAD_PyEditor();
  
  void setText(QString s); 
  bool isCommand(const QString& str) const;
  
protected:
  void keyPressEvent (QKeyEvent * e);
  void mousePressEvent (QMouseEvent * e);
  void mouseReleaseEvent (QMouseEvent * e);
  void dropEvent (QDropEvent *e);
  
public slots:
  void handleReturn();
  
private:
  QAD_PyInterp * _interp;
  QString        _buf;
  QString        _currentCommand;
  QString        _currentPrompt;
  bool           _isInHistory;
};

#endif

