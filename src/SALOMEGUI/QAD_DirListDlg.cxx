//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : QAD_DirListDlg.cxx
//  Author : Vadim SANDLER
//  Module : SALOME
//  $Header$

#include "QAD_DirListDlg.h"
#include "QAD_Desktop.h"
#include "QAD_FileDlg.h"
#include "QAD_MessageBox.h"
#include "QAD_Tools.h"
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qapplication.h>
using namespace std;

#define MIN_WIDTH     400
#define MIN_HEIGHT    200
#define MARGIN_SIZE    11
#define SPACING_SIZE    6
#define SPACER_SIZE     5

/*!
  Constructor
*/
QAD_DirListDlg::QAD_DirListDlg(QWidget* parent, const char* name) 
     : QDialog (parent, name, true, WStyle_NormalBorder | WStyle_Customize | WStyle_Title |  WStyle_SysMenu ) 
{
  myEdited       = false;
  myLastSelected = 0;
  myEdit         = 0; 
  myBtn          = 0;

  setCaption(tr("QUICK_DIR_LIST_TLT"));
  setSizeGripEnabled( true );

  QGridLayout* topLayout = new QGridLayout(this);
  topLayout->setMargin(MARGIN_SIZE);
  topLayout->setSpacing(SPACING_SIZE);

  myDirList = new QListBox(this);
  myDirList->setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
//  myDirList->setMaximumSize(MIN_WIDTH, MIN_HEIGHT);
  myDirList->setSelectionMode(QListBox::Single);
  myDirList->setHScrollBarMode(QListBox::AlwaysOff);
  myDirList->horizontalScrollBar()->installEventFilter(this);
  myDirList->verticalScrollBar()->installEventFilter(this);
  myDirList->insertItem(tr("EMPTY_DIR"));
  myDirList->installEventFilter(this);

  QHBoxLayout* ctrlLayout = new QHBoxLayout;
  ctrlLayout->setMargin(0);
  ctrlLayout->setSpacing(0);

  QLabel* lab = new QLabel(myDirList, tr("DIRECTORIES_LBL"), this);

  QToolButton* insertBtn = new QToolButton(this);
  insertBtn->setIconSet(QAD_Desktop::getResourceManager()->loadPixmap("QAD", tr("ICON_DIRLIST_INSERT")));
  insertBtn->setAutoRaise(true);

  QToolButton* deleteBtn = new QToolButton(this);
  deleteBtn->setIconSet(QAD_Desktop::getResourceManager()->loadPixmap("QAD", tr("ICON_DIRLIST_DELETE")));
  deleteBtn->setAutoRaise(true);

  QToolButton* upBtn = new QToolButton(this);
  upBtn->setIconSet(QAD_Desktop::getResourceManager()->loadPixmap("QAD", tr("ICON_DIRLIST_MOVEUP")));
  upBtn->setAutoRaise(true);

  QToolButton* downBtn = new QToolButton(this);
  downBtn->setIconSet(QAD_Desktop::getResourceManager()->loadPixmap("QAD", tr("ICON_DIRLIST_MOVEDOWN")));
  downBtn->setAutoRaise(true);
  
  ctrlLayout->addWidget(lab);
  ctrlLayout->addItem(new QSpacerItem(SPACER_SIZE, SPACER_SIZE, QSizePolicy::Expanding, QSizePolicy::Minimum));
  ctrlLayout->addWidget(insertBtn);
  ctrlLayout->addWidget(deleteBtn);
  ctrlLayout->addWidget(upBtn);
  ctrlLayout->addWidget(downBtn);

  QHBoxLayout* btnLayout = new QHBoxLayout;
  btnLayout->setMargin(0);
  btnLayout->setSpacing(6);

  QPushButton * okBtn     = new QPushButton(tr("BUT_OK"), this);
  QPushButton * cancelBtn = new QPushButton(tr("BUT_CANCEL"), this);
  okBtn->setDefault(true);
  okBtn->setAutoDefault(true);
  cancelBtn->setAutoDefault(true);

  btnLayout->addWidget(okBtn);
  btnLayout->addItem(new QSpacerItem(SPACER_SIZE, SPACER_SIZE, QSizePolicy::Expanding, QSizePolicy::Minimum));
  btnLayout->addWidget(cancelBtn);

  topLayout->addLayout(ctrlLayout, 0, 0);
  topLayout->addWidget(myDirList,  1, 0);
  topLayout->addLayout(btnLayout,  2, 0);

  connect(myDirList, SIGNAL(mouseButtonClicked(int, QListBoxItem*, const QPoint&)), 
          this, SLOT(onMouseButtonClicked(int, QListBoxItem*, const QPoint&)));
  connect(myDirList, SIGNAL(doubleClicked(QListBoxItem*)), 
          this, SLOT(onDblClicked(QListBoxItem*)));
  
  connect(insertBtn, SIGNAL(clicked()), this, SLOT(onInsert()));
  connect(deleteBtn, SIGNAL(clicked()), this, SLOT(onDelete()));
  connect(upBtn,     SIGNAL(clicked()), this, SLOT(onUp()));
  connect(downBtn,   SIGNAL(clicked()), this, SLOT(onDown()));
  connect(okBtn,     SIGNAL(clicked()), this, SLOT(onOk()));
  connect(cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));

  QAD_Tools::centerWidget(this, parent);
}

/*!
  Destructor
*/
QAD_DirListDlg::~QAD_DirListDlg() {
}

/*!
  Gets list of paths
*/
void QAD_DirListDlg::getPathList(QStringList& list) {
  list.clear();
  for (unsigned i = 0; i < myDirList->count()-1; i++)
    list.append(myDirList->text(i));
}

/*!
  Sets list of paths
*/
void QAD_DirListDlg::setPathList(const QStringList& list) {
  for (unsigned i = 0; i < list.count(); i++)
    myDirList->insertItem(list[i], myDirList->count()-1);
}

/*!
  Validates entered path, returns true if OK
*/
bool QAD_DirListDlg::validate() {
  if (myEdited) {
    QString dirPath = myEdit->text().stripWhiteSpace();
    QDir dir(dirPath);
    QListBoxItem* found = 0;
    for (unsigned i = 0; i < myDirList->count()-1; i++) {
      QDir aDir(myDirList->text(i));
      if ( aDir.canonicalPath().isNull() && myDirList->text(i) == dir.absPath() ||
          !aDir.canonicalPath().isNull() && aDir.exists() && aDir.canonicalPath() == dir.canonicalPath()) {
          found = myDirList->item(i);
        break;
      }
    }
    if (dirPath.isEmpty()) {
      if (found) {
        // it should be last (empty) item in the list - nothing to do
        return true;
      }
      else {
        // delete directory from the list
        removeDir(myLastSelected);
        return true;
      }
    }
    else {
      if (found) {
        if (found != myLastSelected) {
          // it is forbidden to add directory more then once
	  QAD_MessageBox::error1(this, 
				 tr("ERR_ERROR"),
				 tr("ERR_DIRECTORY_SPECIFIED"), 
				 tr("BUT_OK"));
	  myEdit->setFocus();
          return false;
        }
      }
      else {
        if (!dir.exists()) {
	  if ( QAD_MessageBox::info2(this, 
				     tr("WRN_WARNING"),
				     tr("WRN_DIRECTORY_N0T_EXIST").arg(dir.absPath()),
				     tr ("BUT_YES"), tr ("BUT_NO"), 
				     QAD_YES, QAD_NO, QAD_NO ) == QAD_NO ) {
	    myEdit->setFocus();
            return false;
	  }
        }
        // append
        appendDir(myLastSelected, dir.absPath());
      }
    }
  }
  return true;
}

/*!
  Appends/changes directory
*/
void QAD_DirListDlg::appendDir(QListBoxItem* item, const QString& dir) {
  int index = myDirList->index(item);
  if (index >= 0 && index < (int)myDirList->count()) {
    if (index == (int)myDirList->count()-1) {
      // it is the last item (new), well, insert it before the last (empty)
      myDirList->insertItem(dir, myDirList->count()-1);
    }
    else {
      // change item
      myDirList->changeItem(dir, index);
    }
  }
}

/*!
  Removes directory from list
*/
void QAD_DirListDlg::removeDir(QListBoxItem* item) {
  // do not remove last item (empty)
  int index = myDirList->index(item);
  if (index >= 0 && index < (int)myDirList->count()-1) {
    delete item;
    myLastSelected = myDirList->item(index);
    myDirList->setSelected(myLastSelected, true);
  }
}

/*!
  KeyPress event handler, processes <Enter> and <Escape> keys
*/
void QAD_DirListDlg::keyPressEvent(QKeyEvent* event) {
  if ( myEdited ) {
    if ( event->key() == Key_Escape ) {
      delete myEdit;
      delete myBtn;
      myEdit = 0;
      myBtn  = 0;
      myEdited = false;
      myDirList->setFocus();
    }
    return;
  }
  if (event->key() == Key_Return || event->key() == Key_Enter) {
    accept();
  }
  QDialog::keyPressEvent(event);
}

/*!
  Resize event
*/
void QAD_DirListDlg::resizeEvent(QResizeEvent* event) {
  QDialog::resizeEvent(event);
  if ( myEdited ) {
    myEdit->resize(myDirList->viewport()->width()-myBtn->sizeHint().width(), myEdit->height());
    myBtn->move(myEdit->width(), myEdit->y());
  }
}

/*!
  Called when user clicks inside directories list box
*/
void QAD_DirListDlg::onMouseButtonClicked(int           button, 
					  QListBoxItem* item, 
					  const QPoint& point) {
  if (myEdited) {
    if (!validate()) {
      myDirList->setCurrentItem(myLastSelected);
      myDirList->setSelected(myLastSelected, true);
      return;
    }
    delete myEdit;
    delete myBtn;
    myEdit = 0;
    myBtn  = 0;
    myEdited = false;
    myDirList->setFocus();
  }
  if (item) {
    myDirList->setCurrentItem(item);
    myDirList->setSelected(item, true);
    myDirList->ensureCurrentVisible();
    qApp->processEvents();
    if (button == LeftButton && myLastSelected == item) {
      QRect ir = myDirList->itemRect(myLastSelected);
      
      myEdit = new QLineEdit(myDirList->viewport());
      myBtn  = new QToolButton(myDirList->viewport());
      myBtn->setText(" ... ");
      connect(myBtn, SIGNAL(clicked()), this, SLOT(onBtnClicked()));
      myEdit->setGeometry(0, 
                          ir.top()-(myEdit->sizeHint().height()-ir.height())/2, 
                          myDirList->viewport()->width()-myBtn->sizeHint().width(), 
                          myEdit->sizeHint().height());
      myBtn->setGeometry (myEdit->width(), 
                          ir.top()-(myEdit->sizeHint().height()-ir.height())/2, 
                          myBtn->sizeHint().width(),
                          myEdit->sizeHint().height());
      connect(myEdit, SIGNAL(returnPressed()), this, SLOT(onEditFinished()));
      myEdited = true;
      myEdit->show();
      myBtn->show();
      if (myDirList->index(myLastSelected) != (int)myDirList->count()-1)
        myEdit->setText(myLastSelected->text());
      myEdit->selectAll();
      myEdit->setCursorPosition(myEdit->text().length());
      myEdit->installEventFilter(this);
      myEdit->setFocus();
    }
  }
  else {
    myDirList->clearSelection();
  }
  myLastSelected = item;
}

/*!
  Called when user double-clicks on any item
*/
void QAD_DirListDlg::onDblClicked(QListBoxItem* item) {
  onMouseButtonClicked(LeftButton, item, QPoint(0,0));
}

/*!
  <...> (Browse dir) button slot
*/
void QAD_DirListDlg::onBtnClicked() {
  QString dir = myEdit->text().stripWhiteSpace().isEmpty() ? 
                QString::null : 
                myEdit->text().stripWhiteSpace();
  dir = QAD_FileDlg::getExistingDirectory(this, dir, tr("SELECT_DIRECTORY"), true);
  if (!dir.isEmpty()) {
    myEdit->setText(dir);
    myEdit->selectAll();
    myEdit->setCursorPosition(myEdit->text().length());
  }
}

/*!
  Called when user finises editing of path by pressing <Enter>
*/
void QAD_DirListDlg::onEditFinished() {
  if (myEdit) {
    if (!validate()) {
      myDirList->setCurrentItem(myLastSelected);
      myDirList->setSelected(myLastSelected, true);
      return;
    }
    delete myEdit;
    delete myBtn;
    myEdit = 0;
    myBtn  = 0;
    myEdited = false;
    myDirList->setFocus();
  }
}

/*!
  Event filter
*/
bool QAD_DirListDlg::eventFilter(QObject* object, QEvent* event) {
  if ( myEdited ) {
    if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick) {
      if (object == myDirList->horizontalScrollBar() || object == myDirList->verticalScrollBar()) {
	if (!validate()) {
	  myDirList->setCurrentItem(myLastSelected);
	  myDirList->setSelected(myLastSelected, true);
	  return true;
	}
	delete myEdit;
	delete myBtn;
	myEdit = 0;
	myBtn  = 0;
	myEdited = false;
	myDirList->setFocus();
      }
    }
    else if (event->type() == QEvent::KeyPress) {
      QKeyEvent* ke = (QKeyEvent*)event;
      if (ke->key() == Key_Tab)
	return true;
      if (object == myDirList) {
	return true;
      }
      else if (object == myEdit) {
        if ( ke->key() == Key_Up || ke->key() == Key_Down || ke->key() == Key_PageUp || ke->key() == Key_PageDown ||
	     ( ke->key() == Key_Home  || ke->key() == Key_End  || ke->key() == Key_Prior || ke->key() == Key_Next ) && 
                 (ke->state() & ControlButton) ) {
          return true;
	}
	else if ( ke->key() == Key_Escape ) {
	  delete myEdit;
	  delete myBtn;
	  myEdit = 0;
	  myBtn  = 0;
	  myEdited = false;
	  myDirList->setFocus();
	  return true;
	}
      }
    }
  }
  else {
    if (event->type() == QEvent::KeyPress) {
      if (object == myDirList) {
	QKeyEvent* ke = (QKeyEvent*)event;
	if (ke->key() == Key_Return || ke->key() == Key_Enter) {
	  accept();
	}
	else {
	  QChar c(ke->ascii());
	  if (c.isPrint()) {
	    QListBoxItem* item = myDirList->item(myDirList->currentItem());
	    if (item) {
	      myDirList->setCurrentItem(item);
	      myDirList->setSelected(item, true);
	      myDirList->ensureCurrentVisible();
	      myLastSelected = item;
	      qApp->processEvents();
	      QRect ir = myDirList->itemRect(myLastSelected);
		
	      myEdit = new QLineEdit(myDirList->viewport());
	      myBtn  = new QToolButton(myDirList->viewport());
	      myBtn->setText(" ... ");
	      connect(myBtn, SIGNAL(clicked()), this, SLOT(onBtnClicked()));
	      myEdit->setGeometry(0, 
				  ir.top()-(myEdit->sizeHint().height()-ir.height())/2, 
				  myDirList->viewport()->width()-myBtn->sizeHint().width(), 
				  myEdit->sizeHint().height());
	      myBtn->setGeometry (myEdit->width(), 
				  ir.top()-(myEdit->sizeHint().height()-ir.height())/2, 
				  myBtn->sizeHint().width(),
				  myEdit->sizeHint().height());
	      connect(myEdit, SIGNAL(returnPressed()), this, SLOT(onEditFinished()));
	      myEdited = true;
	      myEdit->show();
	      myBtn->show();
	      myEdit->setText(c);
	      myEdit->setCursorPosition(myEdit->text().length());
	      myEdit->installEventFilter(this);
	      myEdit->setFocus();
	    }
	  }
	}
      }
    }
  }
  return QDialog::eventFilter(object, event);
}

/*!
  <Insert> button slot
*/
void QAD_DirListDlg::onInsert() {
  if (!myEdited) {
    myLastSelected = 0;
    onMouseButtonClicked(LeftButton, myDirList->item(myDirList->count()-1), QPoint(0,0));
    onMouseButtonClicked(LeftButton, myDirList->item(myDirList->count()-1), QPoint(0,0));
  }
}

/*!
  <Delete> button slot
*/
void QAD_DirListDlg::onDelete() {
  if (!myEdited && myDirList->currentItem() >=0) {
    removeDir(myDirList->item(myDirList->currentItem()));
    myDirList->setFocus();
  }
}

/*!
  <Move up> button slot
*/
void QAD_DirListDlg::onUp() {
  if (!myEdited && myLastSelected) {
    int index = myDirList->currentItem();
    if (index > 0 && index < (int)myDirList->count()-1 && myDirList->isSelected(index)) {
      QString t = myDirList->text(index-1);
      myDirList->changeItem(myDirList->text(index), index-1);
      myDirList->changeItem(t, index);
      myDirList->setCurrentItem(index-1);
      myLastSelected = myDirList->item(index-1);
      myDirList->setSelected(myLastSelected, true);
      myDirList->setFocus();
    }
  }
}

/*!
  <Move down> button slot
*/
void QAD_DirListDlg::onDown() {
  if (!myEdited && myLastSelected) {
    int index = myDirList->currentItem();
    if (index >= 0 && index < (int)myDirList->count()-2 && myDirList->isSelected(index)) {
      QString t = myDirList->text(index+1);
      myDirList->changeItem(myDirList->text(index), index+1);
      myDirList->changeItem(t, index);
      myDirList->setCurrentItem(index+1);
      myLastSelected = myDirList->item(index+1);
      myDirList->setSelected(myLastSelected, true);
      myDirList->setFocus();
    }
  }
}

/*!
  Purpose  : <OK> button slot
*/
void QAD_DirListDlg::onOk() {
  if (validate())
    accept();
}
