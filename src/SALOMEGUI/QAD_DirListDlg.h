//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : QAD_DirListDlg.h
//  Author : Vadim SANDLER
//  Module : SALOME
//  $Header$

#if !defined(QAD_DirListDlg_h)
#define QAD_DirListDlg_h

#include <qdialog.h>
#include <qtoolbutton.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qcombobox.h>

class QAD_DirListDlg : public QDialog {

  Q_OBJECT

public:
// constructor
  QAD_DirListDlg(QWidget* parent, const char* name = 0);
// destructor
  ~QAD_DirListDlg();

// gets list of paths
  void          getPathList(QStringList& list);
// sets list of paths
  void          setPathList(const QStringList& list);
// event filter
  bool          eventFilter(QObject* object, QEvent* event);

protected:
// validates entered path, returns true if OK
  bool          validate();
// appends/changes path
  void          appendDir(QListBoxItem* item, const QString& dir);
// removes directory from list
  void          removeDir(QListBoxItem* item);
// KeyPress event handler, processes <Enter> and <Escape> keys
  void          keyPressEvent(QKeyEvent* event);
// Resize event
  void          resizeEvent(QResizeEvent* event);

protected slots:
// called when user clicks inside directories list box
  void          onMouseButtonClicked(int, QListBoxItem*, const QPoint&);
// called when user double-clicks on any item
  void          onDblClicked(QListBoxItem*);
// <...> (Browse dir) button slot
  void          onBtnClicked();
// called when user finises editing of path by pressing <Enter>
  void          onEditFinished();
// <Insert> button slot
  void          onInsert();
// <Delete> button slot
  void          onDelete();
// <Move up> button slot
  void          onUp();
// <Move down> button slot
  void          onDown();
// <OK> button slot
  void          onOk();

private:
  QListBox*     myDirList;          // directory list
  QLineEdit*    myEdit;             // path edit box
  QToolButton*  myBtn;              // browse pah button
  bool          myEdited;           // edit mode flag
  QListBoxItem* myLastSelected;     // last selected row

};

#endif // !defined(QAD_DirListDlg_h)
