//  File      : SALOMEGUI_LoadStudiesDlg.h
//  Created   : Wed Apr 03 13:36:09 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE 2002
//  $Header$

#ifndef SALOMEGUI_LOADSTUDIESDLG_H
#define SALOMEGUI_LOADSTUDIESDLG_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QLabel;
class QListBox;
class QListBoxItem;
class QPushButton;

class SALOMEGUI_LoadStudiesDlg : public QDialog
{ 
    Q_OBJECT

public:
    SALOMEGUI_LoadStudiesDlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~SALOMEGUI_LoadStudiesDlg();

    QLabel* TextLabel1;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    QListBox* ListComponent;

};

#endif // SALOMEGUI_LOADSTUDIESDLG_H
