//  File      : ToolsGUI_CatalogGeneratorDlg.h
//  Created   : Thu Apr 25 18:52:53 2002
//  Author    : Nicolas REJNERI

//  Project   : SALOME
//  Module    : TOOLSGUI
//  Copyright : Open CASCADE 2002
//  $Header$


#ifndef DIALOGBOX_TOOLSGUI_CATALOGGENERATORDLG_H
#define DIALOGBOX_TOOLSGUI_CATALOGGENERATORDLG_H

#include <qdialog.h>

class QLineEdit;
class QPushButton;

//=================================================================================
// class    : ToolsGUI_CatalogGeneratorDlg
// purpose  :
//=================================================================================
class ToolsGUI_CatalogGeneratorDlg : public QDialog
{ 
    Q_OBJECT

public:
    ToolsGUI_CatalogGeneratorDlg( QWidget* parent = 0, const char* name = 0 );
    ~ToolsGUI_CatalogGeneratorDlg();

    QString getIdlFile();
    QString getXmlFile();
    QString getPngFile();
    QString getAuthor();
    QString getVersion();
    QString getCompName();
    QString getCompType();
    QString getCompMultiStd();

protected slots:
    void    onBrowseBtnClicked();
    void    updateButtonState();
    void    onApply();

private:
    QLineEdit*     myIdlEdit;
    QLineEdit*     myXmlEdit;
    QLineEdit*     myPngEdit;
    QLineEdit*     myVersionEdit;
    QLineEdit*     myAuthorEdit;
    QLineEdit*     myCompName;
    QLineEdit*     myCompType;
    QLineEdit*     myCompMultiStd;
    QPushButton*   myBrowseIdlBtn;
    QPushButton*   myBrowseXmlBtn;
    QPushButton*   myBrowsePngBtn;

    QPushButton*   myApplyBtn;
    QPushButton*   myCloseBtn;
};

#endif // DIALOGBOX_TOOLSGUI_CATALOGGENERATORDLG_H
