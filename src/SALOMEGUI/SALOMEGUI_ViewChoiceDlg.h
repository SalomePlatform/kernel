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
//  File   : SALOMEGUI_ViewChoiceDlg.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef SALOMEGUI_VIEWCHOICEDLG_H
#define SALOMEGUI_VIEWCHOICEDLG_H

#include <qvariant.h>
#include <qdialog.h>
#include <qvaluelist.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QComboBox;
class QGroupBox;
class QLabel;
class QPushButton;

class SALOMEGUI_ViewChoiceDlg : public QDialog
{ 
    Q_OBJECT

public:
    SALOMEGUI_ViewChoiceDlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~SALOMEGUI_ViewChoiceDlg();

    int getSelectedViewer();

    QGroupBox* GroupBox1;
    QLabel* TextLabel1;
    QComboBox* ComboBox1;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    QValueList<int> myViewers;
};

#endif // SALOMEGUI_VIEWCHOICEDLG_H
