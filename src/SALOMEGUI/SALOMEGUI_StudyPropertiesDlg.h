//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEGUI_StudyPropertiesDlg.h
//  Author : Sergey ANIKIN
//  Module : SALOME
//  $Header$

#ifndef SALOMEGUI_STUDY_PROPERTIES_DLG_H
#define SALOMEGUI_STUDY_PROPERTIES_DLG_H

#include <qdialog.h>

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

class QAD_ListView;
class QAD_ListViewItem;
class QPushButton;

class SALOMEGUI_StudyPropertiesDlg : public QDialog
{ 
  Q_OBJECT

public:

  enum {
    prpAuthorId,
    prpModeId,
    prpDateId,
    prpSavedId,
    prpLockedId,
    prpModificationsId,
    prpLastId
  };

  SALOMEGUI_StudyPropertiesDlg( QWidget* parent = 0 );
  ~SALOMEGUI_StudyPropertiesDlg();

  bool isChanged() { return myChanged; } 

public slots:
  void onOK();

private:
  void initData(); 
  bool acceptData(); 
  bool propChanged(); 
  
private:
  QAD_ListView*       myPropList;
  QPushButton*        myOKBtn;
  QPushButton*        myCancelBtn;
  bool                myChanged;  

  SALOMEDS::Study_var myStudyDoc;
};
 
#endif // SALOMEGUI_STUDY_PROPERTIES_DLG_H
