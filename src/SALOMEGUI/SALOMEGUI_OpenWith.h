//  File      : SALOMEGUI_OpenWith.cxx
//  Created   : Wed Oct 24 12:19:34 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$


#ifndef SALOMEGUI_OPENWITH_H
#define SALOMEGUI_OPENWITH_H

#include <qdialog.h>

class QListBox;
class QCheckBox;
class QPushButton;

class SALOMEGUI_OpenWith : public QDialog
{ 
  Q_OBJECT

public:
  SALOMEGUI_OpenWith(QWidget* parent = 0, const char* name = 0);
  ~SALOMEGUI_OpenWith();

  void      addComponent(const QString& aComp);
  QString   getComponent();
  bool      getAlwaysFlag();
  
protected slots:
  void      onSelectionChanged();

protected:
  void      updateButtonState();

protected:
  QListBox*    ListComponent;
  QCheckBox*   AllwaysCheckBox; 
  QPushButton* buttonHelp;
  QPushButton* buttonCancel;
  QPushButton* buttonOk;
};

#endif // SALOMEGUI_OPENWITH_H
