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
//  File   : QAD_HelpWindow.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef QAD_HELPWINDOW_H
#define QAD_HELPWINDOW_H

#include <qmainwindow.h>
#include <qtextbrowser.h>

class QComboBox;
class QPopupMenu;

class QAD_HelpWindow : public QMainWindow
{
  Q_OBJECT
public:
  QAD_HelpWindow();
  ~QAD_HelpWindow();
  
  void contents();
  void context( const QString& _source, const QString& _context = QString::null );

private slots:
  void setBackwardAvailable( bool );
  void setForwardAvailable( bool );
  void textChanged();
  void pathSelected( const QString & );
    
signals:
  void helpWindowClosed();

private:
  QTextBrowser* browser;
  QComboBox*    pathCombo;
  int           backwardId, forwardId;
  QString       selectedURL;
  QString       homeDir;
    
};

#endif

