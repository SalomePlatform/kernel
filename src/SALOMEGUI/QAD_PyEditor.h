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

