//  File      : SALOMEGUI_AboutDlg.h
//  Created   : Fri Sep 14 09:56:08 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
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
