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
#include <qpixmap.h>

class QFrame;
class QLabel;
class QPushButton;

class SALOMEGUI_ActivateComponentDlg: public QDialog
{
  Q_OBJECT

public:
  SALOMEGUI_ActivateComponentDlg ( QWidget* parent, const QString& component, const QPixmap icon = QPixmap() ) ;
  ~SALOMEGUI_ActivateComponentDlg ( ) { };

private slots:
  void onButtonClicked();

private:
    QFrame*      myComponentFrame;
    QLabel*      myComponentLab;
    QLabel*      myComponentIcon;
    QLabel*      myInfoLabel;
    QPushButton* myNewBtn;
    QPushButton* myOpenBtn;
    QPushButton* myLoadBtn;
    QPushButton* myCancelBtn;
};

#endif

