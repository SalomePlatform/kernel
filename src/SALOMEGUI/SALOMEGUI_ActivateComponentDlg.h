//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEGUI_ActivateComponentDlg.h
//  Author : Michael ZORIN (mzn)
//  Module : SALOME

#ifndef SALOMEGUI_ActivateComponentDlg_H
#define SALOMEGUI_ActivateComponentDlg_H

#include <qdialog.h> 

class SALOMEGUI_ActivateComponentDlg: public QDialog
{
  Q_OBJECT

public:
  SALOMEGUI_ActivateComponentDlg ( QWidget * parent = 0, const char * name = 0, bool modal = FALSE, WFlags f = 0 ) ;
  ~SALOMEGUI_ActivateComponentDlg ( ) { };

private slots:
  void onButtonClicked();

private:
  QPushButton* m_pb1;
  QPushButton *m_pb2;
  QPushButton *m_pb3;
  QPushButton *m_pb4;
};

#endif

