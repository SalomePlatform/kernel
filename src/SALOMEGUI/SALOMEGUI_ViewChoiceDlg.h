//  File      : SALOMEGUI_ViewChoiceDlg.h
//  Created   : Mon Jul 29 22:17:24 2002
//  Author    : Nicolas REJNERI

//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE 2002
//  $Header$

#ifndef SALOMEGUI_VIEWCHOICEDLG_H
#define SALOMEGUI_VIEWCHOICEDLG_H

#include <qvariant.h>
#include <qdialog.h>
#include <qvaluelist.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QComboBox;
class QGroupBox;
class QLabel;
class QPushButton;

class SALOMEGUI_ViewChoiceDlg : public QDialog
{ 
    Q_OBJECT

public:
    SALOMEGUI_ViewChoiceDlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~SALOMEGUI_ViewChoiceDlg();

    int getSelectedViewer();

    QGroupBox* GroupBox1;
    QLabel* TextLabel1;
    QComboBox* ComboBox1;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    QValueList<int> myViewers;
};

#endif // SALOMEGUI_VIEWCHOICEDLG_H
