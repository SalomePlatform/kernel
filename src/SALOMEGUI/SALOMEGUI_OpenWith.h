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
//  File   : SALOMEGUI_OpenWith.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef SALOMEGUI_OPENWITH_H
#define SALOMEGUI_OPENWITH_H

#include <qdialog.h>

class QListBox;
class QListBoxItem;
class QCheckBox;
class QPushButton;

class SALOMEGUI_OpenWith : public QDialog
{ 
  Q_OBJECT

public:
  SALOMEGUI_OpenWith(QWidget* parent = 0, const char* name = 0);
  ~SALOMEGUI_OpenWith();

  void      addComponent(const QString& aComp);
  QString   getComponent();
  bool      getAlwaysFlag();
  
protected slots:
  void      onSelectionChanged();
  void      onDoubleClickEvent( QListBoxItem* item );

protected:
  void      updateButtonState();
 
protected:
  QListBox*    ListComponent;
  QCheckBox*   AllwaysCheckBox; 
  QPushButton* buttonHelp;
  QPushButton* buttonCancel;
  QPushButton* buttonOk;
};

#endif // SALOMEGUI_OPENWITH_H
