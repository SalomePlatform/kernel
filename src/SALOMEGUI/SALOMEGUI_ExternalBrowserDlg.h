//  File   : SALOMEGUI_ExternalBrowserDlg.h
//  Author : Michael Zorin
//  Module : SALOME
//  $Header: /dn05/salome/CVS/SALOME_ROOT/SALOME/src/SALOMEGUI/SALOMEGUI_ExternalBrowserDlg.h

#ifndef SALOMEGUI_EXTERNAL_BROWSER_DLG_H
#define SALOMEGUI_EXTERNAL_BROWSER_DLG_H

#include <qdialog.h>
#include <qlineedit.h>

class SALOMEGUI_ExternalBrowserDlg : public QDialog
{ 
  Q_OBJECT
    
    public:
  
  SALOMEGUI_ExternalBrowserDlg( QWidget* parent = 0 );
  ~SALOMEGUI_ExternalBrowserDlg();
  void setSettings(const QString& AppString, const QString& ParamsString);
  QString getApp();
  QString getParams();
  
  private slots:

    void onButtonClicked();
    void accept(); 
  
 private:
    
    QPushButton* browseButton;
    QLineEdit* ApplicationLine;
    QLineEdit* ParametersLine;
};

#endif // SALOMEGUI_EXTERNAL_BROWSER_DLG_H
