//  File      : SALOMEGUI_StudyPropertiesDlg.h
//  Created   : Tue Oct 22 12:28:23 2002
//  Descr     : Dialog box for editing of study properties

//  Author    : Sergey ANIKIN
//  Project   : SALOME Pro
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
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
