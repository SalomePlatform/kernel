//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEGUI_NameDlg.h
//  Author : Vadim SANDLER
//  Module : SALOME
//  $Header$

#ifndef SALOMEGUI_NAMEDLG_H
#define SALOMEGUI_NAMEDLG_H

#include <qdialog.h>

class QLineEdit;
class QPushButton;

//=================================================================================
// class    : SALOMEGUI_NameDlg
// purpose  : Common <Rename> dialog box class
//=================================================================================
class SALOMEGUI_NameDlg : public QDialog
{ 
  Q_OBJECT

public:
  SALOMEGUI_NameDlg( QWidget* parent = 0 );
  ~SALOMEGUI_NameDlg();
    
  void            setName( const QString& name );
  QString         name();
    
  static QString  getName( QWidget* parent = 0, const QString& oldName = QString::null );
    
protected slots:
  void accept();
  
private:
  QPushButton*    myButtonOk;
  QPushButton*    myButtonCancel;
  QLineEdit*      myLineEdit;
};

#endif // SALOMEGUI_NAMEDLG_H
