//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : QAD_PyEditor.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef QAD_PyEditor_H
#define QAD_PyEditor_H

#include "QAD_PyInterp.h" // this include must be first (see PyInterp_base.h)!
#include <qtextedit.h>
#include <qevent.h>

class QMutex;

class QAD_PyInterp;
class TInitEditorThread;
class TExecCommandThread;

class QAD_PyEditor : public QTextEdit
{
  Q_OBJECT;

  friend class TInitEditorThread;
  friend class TExecCommandThread;

public:
  enum { PYTHON_OK = QEvent::User + 5000, PYTHON_ERROR, PYTHON_INCOMPLETE, 
	 INITIALIZE, SET_WAIT_CURSOR, UNSET_CURSOR };

public:
  QAD_PyEditor(QAD_PyInterp*& theInterp, QMutex* theMutex,
	       QWidget *theParent = 0, const char* theName = "");
  virtual void Init(); 
  ~QAD_PyEditor();
  
  virtual void setText(QString s); 
  bool isCommand(const QString& str) const;
  
protected:
  virtual void contentsDropEvent( QDropEvent* event );
  virtual void contentsMouseReleaseEvent( QMouseEvent* event );
  virtual void keyPressEvent (QKeyEvent* event);
  virtual void mousePressEvent (QMouseEvent* event);
  virtual void customEvent (QCustomEvent* event);
  
public slots:
  void handleReturn();
  
private:
  QString        _buf;
  QString        _currentCommand;
  QString        _currentPrompt;
  bool           _isInHistory;

  QAD_PyInterp*& myInterp;
  QMutex* myStudyMutex;
  QMutex* myInitEditorMutex;
  QMutex* myExecCommandMutex;
  TInitEditorThread* myInitEditorThread;
  TExecCommandThread* myExecCommandThread;

  QString myBanner;
  QString myOutput;
  QString myError;
};

#endif

