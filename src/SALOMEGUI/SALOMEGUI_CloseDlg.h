// File      : SALOMEGUI_CloseDlg.h
// Created   : Wed Feb 26 11:40:52 2003
// Author    : Vasily RUSYAEV (vrv)
// Copyright : Open CASCADE

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

