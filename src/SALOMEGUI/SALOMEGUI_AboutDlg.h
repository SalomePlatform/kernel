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
//  File   : SALOMEGUI_AboutDlg.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef SALOMEGUI_ABOUTDLG_H
#define SALOMEGUI_ABOUTDLG_H

#include <qvariant.h>
#include <qdialog.h>
#include <qlabel.h>

class SALOMEGUI_AboutDlg : public QDialog
{ 
public:
    SALOMEGUI_AboutDlg( QWidget* parent = 0, const char* name = 0 );
    ~SALOMEGUI_AboutDlg();

protected:
    bool event( QEvent* );
    bool eventFilter( QObject*,  QEvent* );

protected:
    QLabel* AboutPicLab;
    QLabel* VersionLab;
    QLabel* CopyrightLab;
    QLabel* PartnerPicLab;
    QLabel* LicenseLab;
};

#endif // SALOMEGUI_ABOUTDIALOG_H
