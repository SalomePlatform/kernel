//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEGUI_CloseDlg.h
//  Author : Vasily RUSYAEV (vrv)
//  Module : SALOME

#ifndef SALOMEGUI_CloseDlg_H
#define SALOMEGUI_CloseDlg_H

#include <qdialog.h> 

class SALOMEGUI_CloseDlg: public QDialog
{
  Q_OBJECT

public:
  SALOMEGUI_CloseDlg ( QWidget * parent = 0, const char * name = 0, bool modal = FALSE, WFlags f = 0 ) ;
  ~SALOMEGUI_CloseDlg ( ) { };

private slots:
  void onButtonClicked();

private:
  QPushButton* m_pb1;
  QPushButton *m_pb2;
  QPushButton *m_pb3;
  QPushButton *m_pb4;
};

#endif

