//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : QAD_ListView.cxx
//  Author : Vadim SANDLER
//  Module : SALOME
//  $Header$

#include "QAD_ListView.h"

#include <qheader.h>
#include <qvalidator.h>
#include <qapplication.h>
#include <qtoolbutton.h>

#include "QAD_Desktop.h"

//#include <QAD_Application.h>
//#include <QAD_Document.h>


#include <TColStd_ListIteratorOfListOfInteger.hxx>
#include <TColStd_ListIteratorOfListOfReal.hxx>
using namespace std;

//////////////////////////////////////////////////////////////////////
// QAD_ListView class implementation
//////////////////////////////////////////////////////////////////////

//================================================================
// Function : computeEditGeometry
// Purpose  : static function - used for resizing editing widget
//================================================================
void computeEditGeometry(QAD_ListViewItem* theItem,
                         QAD_EntityEdit*   theWidget)  
{
  if (!theItem)
    return;
  QListView* aListView = theItem->listView();
  int anEditColumn = theItem->getEditedColumn();
  if (anEditColumn < 0)
    return;
  
  int aX = 0, aY = 0, aW = 0, aH = 0;

  QRect aRect = aListView->itemRect(theItem);
  aListView->contentsToViewport(aListView->header()->sectionPos(anEditColumn), 0, aX, aY);
  if (aX < 0)
    aX = 0; // THIS CAN BE REMOVED
  QSize aSize = theWidget->getControl()->sizeHint();
  aH = QMAX(aSize.height() , aRect.height() );
  aY = aRect.y() - ((aH - aRect.height()) / 2);
  //aW = aListView->columnWidth(anEditColumn); // CAN SUBSTITUTE NEXT 3 ROWS
  aW = aListView->viewport()->width() - aX;
  if (aW < 0)
    aW = 0;
  theWidget->setGeometry(aX, aY, aW, aH);
}

//================================================================
// Function : QAD_ListView::QAD_ListView
// Purpose  : constructor
//================================================================
QAD_ListView::QAD_ListView(QWidget *parent) :
QListView(parent) 
{
  myPopup = 0;
  myMouseEnabled = true;
  myEditingEnabled = false;
  enablePopup( true );
  setSelectionMode(Single);
  setSorting(-1);
  setRootIsDecorated(false);
  setAllColumnsShowFocus(false);
//  header()->setClickEnabled(false);
  header()->setMovingEnabled(false);

  myEditedItem = 0;
  myEdit = 0;

  viewport()->installEventFilter(this);

  connect(this, SIGNAL(selectionChanged()), 
	  this, SLOT(onSelectionChanged()));
  connect(header(), SIGNAL(sizeChange(int, int, int)), 
	  this,     SLOT(onHeaderSizeChange(int, int, int)));
}

//================================================================
// Function : QAD_ListView::~QAD_ListView
// Purpose  : destructor
//================================================================
QAD_ListView::~QAD_ListView() 
{
// destroy popup menu
  deletePopupMenu();
  if (myEdit) {
    delete myEdit;
  }
  myEdit = 0;
  myEditedItem = 0;
}

//================================================================
// Function : QAD_ListView::updateViewer
// Purpose  : updates all data viewer
//================================================================
void QAD_ListView::updateViewer() 
{
  // temporary disconnecting selection changed SIGNAL
  blockSignals(true);
  QAD_ListViewItem* aRoot = (QAD_ListViewItem*)firstChild();
  if (aRoot)
    aRoot->updateAllLevels();
  updateContents();
  // connecting again selection changed SIGNAL
  blockSignals(false);
  emit selectionChanged();
}

//================================================================
// Function : QAD_ListView::updateSelected
// Purpose  : updates currently selected item(s)
//================================================================
void QAD_ListView::updateSelected() 
{
  // temporary disconnecting selection changed SIGNAL
  blockSignals(true);
  QAD_ListViewItem* aChild = (QAD_ListViewItem*)selectedItem();
  if (aChild)
    aChild->updateAllLevels();
  updateContents();
  // connecting again selection changed SIGNAL
  blockSignals(false);
  emit selectionChanged();
}

//================================================================
// Function : QAD_ListView::onCreatePopup
// Purpose  : fills popup menu with items
//================================================================
void QAD_ListView::onCreatePopup() 
{
  if (myPopup) {
    // add items here...
  }
}

//================================================================
// Function : QAD_ListView::deletePopupMenu
// Purpose  : delete popup menu
//================================================================
void QAD_ListView::deletePopupMenu() 
{
  if (myPopup) {
    destroyPopup();
    delete myPopup;
    myPopup = 0;
  }
}

//================================================================
// Function : QAD_ListView::clear
// Purpose  : clears view
//================================================================
void QAD_ListView::clear() 
{
  if (myEdit) {
    delete myEdit;
    myEdit = 0;
    myEditedItem = 0;
  }
  QListView::clear();
}

//================================================================
// Function : QAD_ListView::isMouseEnabled
// Purpose  : returms true if mouse events are enabled
//================================================================
bool QAD_ListView::isMouseEnabled() 
{
  return myMouseEnabled;
}

//================================================================
// Function : QAD_ListView::enableMouse
// Purpose  : enabled/disables mouse events (excluding MouseMove)
//================================================================
void QAD_ListView::enableMouse(bool enable) 
{
  myMouseEnabled = enable;
}

//================================================================
// Function : QAD_ListView::eventFilter
// Purpose  : event filter
//================================================================
bool QAD_ListView::eventFilter(QObject* object, QEvent* event) 
{
  if (object == viewport() &&
       (event->type() == QEvent::MouseButtonPress   ||
        event->type() == QEvent::MouseButtonRelease ||
        event->type() == QEvent::MouseButtonDblClick)  &&
      !isMouseEnabled())
    return true;
  else
    return QListView::eventFilter(object, event);
}

//================================================================
// Function : QAD_ListView::enableEditing
// Purpose  : setting editing of items availbale/not available
//================================================================
void QAD_ListView::enableEditing(bool theFlag) 
{
  myEditingEnabled = theFlag;
  if (!myEditingEnabled) {
    if (myEdit) {
      delete myEdit;
      myEdit = 0;
      myEditedItem = 0;
    }
  }
}

//================================================================
// Function : QAD_ListView::isEnableEditing
// Purpose  : says if editing is enabled
//================================================================
bool QAD_ListView::isEnableEditing() 
{
  return myEditingEnabled;
}

//================================================================
// Function : QAD_ListView::accept
// Purpose  : calls finishEditing(true)...
//================================================================
void QAD_ListView::accept() 
{
  finishEditing(true);
}

//================================================================
// Function : QAD_ListView::contentsMouseReleaseEvent
// Purpose  : mouse release button event
//================================================================
void QAD_ListView::contentsMouseReleaseEvent(QMouseEvent* e) 
{
  QListView::contentsMouseReleaseEvent(e);
  // destroy popup menu if exist
  qApp->processEvents();
  // create popup menu on right button click
  if(e->button() == RightButton) {

    deletePopupMenu();
    QPopupMenu* aPopup = createPopup();
    if (aPopup) {
      myPopup = aPopup;
      if (myPopup->count() > 0)
        //myPopup->exec(QCursor::pos());
        myPopup->popup(e->globalPos());
    }    
  }
}

//================================================================
// Function : QAD_ListView::onSelectionChanged
// Purpose  : slot, called when selection changed in List Viewer
//================================================================
void QAD_ListView::onSelectionChanged() 
{
  if (myEdit) {
    finishEditing(true);
    delete myEdit;
    myEdit = 0;
    if (myEditedItem && !myEditedItem->isAccepted()) {
      delete myEditedItem;
      updateContents();
    }
    myEditedItem = 0;
  } 
  // editing is allowed in Single Selection Mode only
  if (selectionMode() != Single || !isEnableEditing())
    return;
  QAD_ListViewItem* anItem = (QAD_ListViewItem*)selectedItem();
  if (anItem) {
    if (!anItem->isEditable())
      return;
    myEdit = anItem->startEditing();
    if (myEdit) {
      connect(myEdit, SIGNAL(returnPressed()), this, SLOT(onEditOk()));
      connect(myEdit, SIGNAL(escapePressed()), this, SLOT(onEditCancel()));
      myEditedItem = anItem;
      myEdit->show();
      myEdit->setFocus();
    }
  }
}

//================================================================
// Function : QAD_ListView::resizeEvent
// Purpose  : called when Data Viewer is resized
//================================================================
void QAD_ListView::resizeEvent( QResizeEvent * e) 
{
  QListView::resizeEvent(e);
  int aW = columnWidth(columns()-1);
  int aX = header()->sectionPos(columns()-1);
  if (aW < width() - frameWidth() * 2 - aX - 1)
    setColumnWidth(columns()-1, width() - frameWidth() * 2 - aX - 1);
  updateContents();
}

//================================================================
// Function : QAD_ListView::onHeaderSizeChange
// Purpose  : slot, called when columns sizes are changed
//================================================================
void QAD_ListView::onHeaderSizeChange(int, int, int) 
{
  int aW = columnWidth(columns()-1);
  int aX = header()->sectionPos(columns()-1);
  if (aW < width() - frameWidth() * 2 - aX - 1)
    setColumnWidth(columns()-1, width() - frameWidth() * 2 - aX - 1);
}

//================================================================
// Function : QAD_ListView::viewportPaintEvent
// Purpose  : handler for paint event
//================================================================
void QAD_ListView::viewportPaintEvent(QPaintEvent* e) 
{
  QListView::viewportPaintEvent(e);
  if (myEditedItem && myEdit) {
    computeEditGeometry(myEditedItem, myEdit);
  }
}

//================================================================
// Function : QAD_ListView::onEditOk
// Purpose  : called when user finishes in editing of item
//================================================================
void QAD_ListView::onEditOk() 
{
  finishEditing(true);
}
  
//================================================================
// Function : QAD_ListView::onEditCancel
// Purpose  : called when user cancels item editing
//================================================================
void QAD_ListView::onEditCancel() 
{
  finishEditing(false);
}

//================================================================
// Function : QAD_ListView::finishEditing
// Purpose  : finishes editing of entity
//================================================================
UpdateType QAD_ListView::finishEditing(bool ok) 
{
  UpdateType aNeedsUpdate = utCancel;
  if (myEditedItem && myEdit)
  {
    disconnect(myEdit, SIGNAL(returnPressed()), this, SLOT(onEditOk()));
    disconnect(myEdit, SIGNAL(escapePressed()), this, SLOT(onEditCancel()));
    myEditedItem->setAccepted(true);
    if (ok) {
      aNeedsUpdate = myEditedItem->finishEditing(myEdit);
      if (aNeedsUpdate == utCancel) {
	// something to do here on Cancel...
      }
      else {
	// something to do here on OK...
      }
      // updating contents
      switch (aNeedsUpdate) {
      case utUpdateItem:
        {
          if (myEditedItem)
            myEditedItem->updateAllLevels();
          break;
        }
      case utUpdateParent:
        {
          if (myEditedItem) {
            QAD_ListViewItem* aParent = (QAD_ListViewItem*)(myEditedItem->parent());
            if (aParent)
              aParent->updateAllLevels();
            else 
              myEditedItem->updateAllLevels();
          }
          break;
        }
      case utUpdateViewer:
        {
          updateViewer();
          break;
        }
      case utUpdateAll:
        {
          // doing the same as for utUpdateViewer here
          // descendants can add extra processing
          updateViewer();
          break;
        }
      default:
        break;
      }
    }
  }

  // hide <myEdit> widget
  if (myEdit) {
    myEdit->hide();
  }

  return aNeedsUpdate;
}

//================================================================
// Function : QAD_ListView::tip
// Purpose  : gets current tooltip for list view
//            returns valid rect in success
//================================================================
QRect QAD_ListView::tip(QPoint aPos, 
			QString& aText, 
			QRect& dspRect, 
			QFont& dspFnt) const 
{
  QRect result( -1, -1, -1, -1 );
  QAD_ListViewItem* aItem = (QAD_ListViewItem*)itemAt( aPos );
  if ( aItem ) {
    for (int i = 0; i < columns(); i++) {
      QRect aItemRect = aItem->itemRect(i);
      QRect aTextRect = aItem->textRect(i);
      if ( !aItem->text(i).isEmpty() &&
	   ( aItemRect.width()  > header()->sectionSize(i) ||
	     aTextRect.left()   < 0 || 
             aTextRect.top()    < 0 ||
             aTextRect.right()  > viewport()->width() ||
             aTextRect.bottom() > viewport()->height() ) ) {
        // calculating tip data
        aText   = aItem->tipText();
	dspRect = aItem->tipRect();
	dspFnt  = font();
        if (dspRect.isValid()) {
          result  = QRect(QPoint(0, aItemRect.top()), 
                          QSize(viewport()->width(), aItemRect.height()));
        }
      }
    }
  }
  return result;
}

//////////////////////////////////////////////////////////////////////
// QAD_ListViewItem Class Implementation
//////////////////////////////////////////////////////////////////////

//================================================================
// Function : QAD_ListViewItem::QAD_ListViewItem
// Purpose  : constructor
//================================================================
QAD_ListViewItem::QAD_ListViewItem(QAD_ListView* parent) :
QListViewItem( parent ) 
{
  init();
}

//================================================================
// Function : QAD_ListViewItem::QAD_ListViewItem
// Purpose  : constructor
//================================================================
QAD_ListViewItem::QAD_ListViewItem(QAD_ListView*     parent, 
				   QAD_ListViewItem* after) :
QListViewItem( parent, after ) 
{
  init();
}

//================================================================
// Function : QAD_ListViewItem::QAD_ListViewItem
// Purpose  : constructor
//================================================================
QAD_ListViewItem::QAD_ListViewItem(QAD_ListView*     parent, 
				   const QString     theName,
				   const bool        theEditable) :
QListViewItem(parent, theName) 
{
  init();
  setEditable(theEditable);
}

//================================================================
// Function : QAD_ListViewItem::QAD_ListViewItem
// Purpose  : constructor
//================================================================
QAD_ListViewItem::QAD_ListViewItem(QAD_ListView*     parent, 
				   const QString     theName,
				   const QString     theValue,
				   const bool        theEditable) :
QListViewItem(parent, theName, theValue) 
{
  init();
  setEditable(theEditable);
}

//================================================================
// Function : QAD_ListViewItem::QAD_ListViewItem
// Purpose  : constructor
//================================================================
QAD_ListViewItem::QAD_ListViewItem(QAD_ListViewItem* parent, 
				   const QString     theName,
				   const bool        theEditable) :
QListViewItem(parent, theName)
{
  init();
  setEditable(theEditable);
}

//================================================================
// Function : QAD_ListViewItem::QAD_ListViewItem
// Purpose  : constructor
//================================================================
QAD_ListViewItem::QAD_ListViewItem(QAD_ListViewItem* parent, 
				   QAD_ListViewItem* after, 
				   const QString     theName,
				   const bool        theEditable) :
QListViewItem(parent, after, theName)
{
  init();
  setEditable(theEditable);
}

//================================================================
// Function : QAD_ListViewItem::QAD_ListViewItem
// Purpose  : constructor
//================================================================
QAD_ListViewItem::QAD_ListViewItem(QAD_ListView*     parent, 
				   QAD_ListViewItem* after, 
				   const QString     theName,
				   const bool        theEditable) :
QListViewItem(parent, after, theName)
{
  init();
  setEditable(theEditable);
}


//================================================================
// Function : QAD_ListViewItem::QAD_ListViewItem
// Purpose  : constructor
//================================================================
QAD_ListViewItem::QAD_ListViewItem(QAD_ListViewItem* parent, 
				   const QString     theName,
				   const QString     theValue,
				   const bool        theEditable) :
QListViewItem(parent, theName, theValue)
{
  init();
  setEditable(theEditable);
}


//================================================================
// Function : QAD_ListViewItem::QAD_ListViewItem
// Purpose  : constructor
//================================================================
QAD_ListViewItem::QAD_ListViewItem(QAD_ListViewItem* parent, 
				   QAD_ListViewItem* after, 
				   const QString     theName,
				   const QString     theValue,
				   const bool        theEditable) :
QListViewItem(parent, after, theName, theValue)
{
  init();
  setEditable(theEditable);
}

//================================================================
// Function : QAD_ListViewItem::QAD_ListViewItem
// Purpose  : constructor
//================================================================
QAD_ListViewItem::QAD_ListViewItem(QAD_ListView*     parent, 
				   QAD_ListViewItem* after, 
				   const QString     theName,
				   const QString     theValue,
				   const bool        theEditable) :
QListViewItem(parent, after, theName, theValue)
{
  init();
  setEditable(theEditable);
}

//================================================================
// Function : QAD_ListViewItem::~QAD_ListViewItem
// Purpose  : destructor
//================================================================
QAD_ListViewItem::~QAD_ListViewItem() 
{
}

//================================================================
// Function : QAD_ListViewItem::init
// Purpose  : initialization
//================================================================
void QAD_ListViewItem::init() 
{
  myEditable    = false;
  myAccepted    = true;
  myEditingType = (int)QAD_EntityEdit::etLineEdit;
  myValueType   = (int)QAD_EntityEdit::vtString;
  myButtons     = 0;
  myUserType    = -1;
}

//================================================================
// Function : QAD_ListViewItem::getName
// Purpose  : as default returns text in the first column
//================================================================
QString QAD_ListViewItem::getName() const 
{
  return ( listView()->columns() > 0 ) ? text(0) : QString("");
}

//================================================================
// Function : QAD_ListViewItem::setName
// Purpose  : as default sets text in the first column
//================================================================
UpdateType QAD_ListViewItem::setName(const QString theName) 
{
  UpdateType aNeedsUpdate = utCancel;
  if (listView()->columns() > 0) {
    setText(0, theName);
    aNeedsUpdate = utNone;
  }
  return aNeedsUpdate;
}

//================================================================
// Function : QAD_ListViewItem::getValue
// Purpose  : as default returns text in the second column
//================================================================
QString QAD_ListViewItem::getValue() const 
{
  return ( listView()->columns() > 1 ) ? text(1) : QString("");
}

//================================================================
// Function : QAD_ListViewItem::setValue
// Purpose  : as default sets text in the second column
//================================================================
UpdateType QAD_ListViewItem::setValue(const QString theValue) 
{
  UpdateType aNeedsUpdate = utCancel;
  if (listView()->columns() > 1) {
    setText(1, theValue);
    aNeedsUpdate = utNone;
  }
  return aNeedsUpdate;
}

//================================================================
// Function : QAD_ListViewItem::fullName
// Purpose  : returns full path to the entity from the root
//================================================================
QString QAD_ListViewItem::fullName() 
{
  QString aFullName = getName();
  QAD_ListViewItem* aParent = (QAD_ListViewItem*)parent();
  while(aParent != NULL) {
    aFullName = aParent->getName() + QString(".") + aFullName;
    aParent = (QAD_ListViewItem*)(aParent->parent());
  }
  return aFullName;
}

//================================================================
// Function : QAD_ListViewItem::openAllLevels
// Purpose  : expands all entities beginning from this level
//================================================================
void QAD_ListViewItem::openAllLevels() 
{
  setOpen(true);
  QAD_ListViewItem* aChild = (QAD_ListViewItem*)firstChild();
  while( aChild ) {
    aChild->openAllLevels();
    aChild = (QAD_ListViewItem*)(aChild->nextSibling());
  }
}

//================================================================
// Function : QAD_ListViewItem::updateAllLevels
// Purpose  : update all entites beginning from this level
//================================================================
void QAD_ListViewItem::updateAllLevels() 
{
  QAD_ListViewItem* aChild = (QAD_ListViewItem*)firstChild();
  while( aChild ) {
    aChild->updateAllLevels();
    aChild = (QAD_ListViewItem*)(aChild->nextSibling());
  }
}

//================================================================
// Function : QAD_EditBox::isEditable
// Purpose  : return true if entity is editable
//================================================================
bool QAD_ListViewItem::isEditable() const 
{
  return myEditable;
}

//================================================================
// Function : QAD_ListViewItem::setEditable
// Purpose  : sets editable flag fo the entity
//================================================================
void QAD_ListViewItem::setEditable(bool theEditable) 
{
  myEditable = theEditable;
}

//================================================================
// Function : QAD_ListViewItem::isAccepted
// Purpose  : returns true if entitiy is accepted after editing
//================================================================
bool QAD_ListViewItem::isAccepted() const 
{
  return myAccepted;
}

//================================================================
// Function : QAD_ListViewItem::setAccepted
// Purpose  : set entitiy accepted or not after editing
//================================================================
void QAD_ListViewItem::setAccepted(bool theAccepted) 
{
  myAccepted = theAccepted;
}

//================================================================
// Function : QAD_ListViewItem::getEditingType
// Purpose  : returns type of edit control (0 - edit box, 1 - combo box, 
//            2 - editable combo box),  default is edit box
//================================================================
int QAD_ListViewItem::getEditingType() 
{
  return myEditingType;
}

//================================================================
// Function : QAD_ListViewItem::setEditingType
// Purpose  : sets type of edit control (0 - edit box, 1 - combo box, 
//            2 - editable combo box), negative value means none
//================================================================
void QAD_ListViewItem::setEditingType(const int type) 
{
  myEditingType = type;
}

//================================================================
// Function : QAD_ListViewItem::getEditedColumn
// Purpose  : returns edited column, default is last column
//            negative value means there are no editable columns
//================================================================
int QAD_ListViewItem::getEditedColumn() 
{
  return listView()->columns()-1;
}

//================================================================
// Function : QAD_ListViewItem::getValueType
// Purpose  : returns type of edited value (string, int, double)
//            default is string
//================================================================
int QAD_ListViewItem::getValueType() 
{
  return myValueType;
}

//================================================================
// Function : QAD_ListViewItem::setValueType
// Purpose  : sets type of edited value
//================================================================
void QAD_ListViewItem::setValueType(const int valueType) 
{
  myValueType = valueType;
}

//================================================================
// Function : QAD_ListViewItem::getUserType
// Purpose  : sets type of edited value
//================================================================
int QAD_ListViewItem::getUserType() 
{
  return myUserType;
}

//================================================================
// Function : QAD_ListViewItem::setUserType
// Purpose  : sets type of edited value
//================================================================
void QAD_ListViewItem::setUserType(const int userType) 
{
  myUserType = userType;
}

//================================================================
// Function : QAD_ListViewItem::getButtons
// Purpose  : returns buttons for editing widget (Apply (V), Cancel (X))
//            default is both buttons
//================================================================
int QAD_ListViewItem::getButtons() 
{
  return myButtons;
}

//================================================================
// Function : QAD_ListViewItem::getButtons
// Purpose  : sets buttons for editing widget (Apply (V), Cancel (X))
//================================================================
void QAD_ListViewItem::setButtons(const int buttons) 
{
  myButtons = buttons;
}

//================================================================
// Function : QAD_ListViewItem::startEditing
// Purpose  : creates control for editing and fills it with values
//================================================================
QAD_EntityEdit* QAD_ListViewItem::startEditing() 
{
  QAD_EntityEdit* aWidget = 0;
  QListView* aListView = listView();
  if (aListView) {
    if (!isEditable())
      return 0;
    int anEditType   = getEditingType();
    int aValueType   = getValueType();
    int aButtons     = getButtons();
    int anEditColumn = getEditedColumn();
    if (anEditColumn < 0 || anEditType < 0)
      return 0;
    aWidget = new QAD_EntityEdit(aListView->viewport(), 
                                 anEditType, 
				 aValueType, 
				 aButtons & QAD_EntityEdit::btApply,
				 aButtons & QAD_EntityEdit::btCancel);
    computeEditGeometry(this, aWidget);

    fillWidgetWithValues(aWidget);
  }
  return aWidget;
}

//================================================================
// Function : QAD_ListViewItem::fillWidgetWithValues
// Purpose  : fills widget with initial values (list or single value)
//================================================================
void QAD_ListViewItem::fillWidgetWithValues(QAD_EntityEdit* theWidget) 
{
  int anEditColumn = getEditedColumn();
  if (theWidget && anEditColumn >= 0 && !text(anEditColumn).isEmpty())
    theWidget->insertItem(text(anEditColumn), true);
}

//================================================================
// Function : QAD_ListViewItem::finishEditing
// Purpose  : finishes editing of entity
//================================================================
UpdateType QAD_ListViewItem::finishEditing(QAD_EntityEdit* theWidget) 
{
  UpdateType aNeedsUpdate = utCancel;
  try {
    if (theWidget) {
      int anEditColumn = getEditedColumn();
      switch (anEditColumn) {
      case 0:
        aNeedsUpdate = setName(theWidget->getText());
        break;
      case 1:
        aNeedsUpdate = setValue(theWidget->getText());
        break;
      default: 
        break;
      }
    } 
  }
  catch (...) {
    MESSAGE( "System error has been caught - QAD_ListViewItem::finishEditing" )
  }
  return aNeedsUpdate;
}

//================================================================
// Function : QAD_ListViewItem::tipRect
// Purpose  : calculates rectangle which should contain item's tip
//================================================================
QRect QAD_ListViewItem::tipRect() 
{
  QRect aRect = QRect(-1, -1, -1, -1);
  QRect aItemRect = listView()->itemRect(this);
  if ( !aItemRect.isValid() )
    return aItemRect;

  QString aTip = tipText();
  if (!aTip.isEmpty()) {
    QRect aRect0 = textRect(0);
    QFont aFont(listView()->font());
    QFontMetrics fm(aFont);
    int iw = fm.width(aTip);
    aRect = QRect(QPoint(aRect0.x() < 0 ? 0 : aRect0.x(), 
                         aRect0.y()), 
                  QSize (iw,         
                         aRect0.height()));
  }
  return aRect;
}

//================================================================
// Function : QAD_ListViewItem::tipText
// Purpose  : returns text for tooltip
//================================================================
QString QAD_ListViewItem::tipText() 
{
  QString aText = getName();
  if (!getValue().isEmpty())
    aText += QString(" : ") + getValue();
  return aText;
}

//================================================================
// Function : QAD_ListViewItem::textRect
// Purpose  : calculates rect of item text in viewport coordinates
//================================================================
QRect QAD_ListViewItem::textRect(const int column) const
{
  QRect aItemRect = listView()->itemRect( this );
  if ( !aItemRect.isValid() )
    return aItemRect;
  
  QFont aFont(listView()->font());
  QFontMetrics fm(aFont);
  
  int decorWidth  = ( listView()->rootIsDecorated() ) ? 
                    ( listView()->treeStepSize() * (depth() + 1) ) :
                    ( listView()->treeStepSize() *  depth() );
  int pixmapWidth = ( pixmap(column) ) ? 
                      pixmap(column)->width() +  listView()->itemMargin() * 2 : 
                      listView()->itemMargin();
  int prevWidth = 0;
  for (int i = 0; i < column; i++)
    prevWidth += listView()->header()->sectionSize(i);
  int ix = prevWidth   +
           pixmapWidth + 
           ((column == 0) ? decorWidth : 0);
  int iy = aItemRect.y();
  int iw = fm.width(text(column));
  int ih = aItemRect.height();
  if (pixmap(column)) {
    iy += listView()->itemMargin();
    ih -= listView()->itemMargin() * 2;
  }
  ix -= listView()->contentsX();

  QRect theResult(QPoint(ix, iy), QSize(iw, ih));
  return theResult;
}

//================================================================
// Function : QAD_ListViewItem::itemRect
// Purpose  : calculates rect of item data in viewport coordinates
//================================================================
QRect QAD_ListViewItem::itemRect(const int column) const
{
  QRect aItemRect = listView()->itemRect( this );
  if ( !aItemRect.isValid() )
    return aItemRect;
  
  QFont aFont(listView()->font());
  QFontMetrics fm(aFont);
  
  int decorWidth  = ( listView()->rootIsDecorated() ) ? 
                    ( listView()->treeStepSize() * (depth() + 1) ) :
                    ( listView()->treeStepSize() *  depth() );
  int pixmapWidth = ( pixmap(column) ) ? 
                      pixmap(column)->width() +  listView()->itemMargin() * 2 : 
                      0;
  int prevWidth = 0;
  for (int i = 0; i < column; i++)
    prevWidth += listView()->header()->sectionSize(i);
  int ix = prevWidth;
  int iy = aItemRect.y();
  int iw = pixmapWidth + 
           listView()->itemMargin() * 2 + 
           ((column == 0) ? decorWidth : 0) + 
           fm.width(text(column));
  int ih = aItemRect.height();
  ix -= listView()->contentsX();

  QRect theResult(QPoint(ix, iy), QSize(iw, ih));
  return theResult;
}

//////////////////////////////////////////////////////////////////////
// QAD_EditBox class implementation
//////////////////////////////////////////////////////////////////////

//================================================================
// Function : QAD_EditBox::QAD_EditBox
// Purpose  : constructor
//================================================================
QAD_EditBox::QAD_EditBox(QWidget* parent) :
QLineEdit(parent) 
{
}

//================================================================
// Function : QAD_EditBox::keyPressEvent
// Purpose  : event filter for key pressing
//================================================================
void QAD_EditBox::keyPressEvent( QKeyEvent *e ) 
{
  if ( e->key() == Key_Escape )
    emit escapePressed();
  else
    QLineEdit::keyPressEvent( e );
  e->accept();
}

//////////////////////////////////////////////////////////////////////
// QAD_ComboBox class implementation
//////////////////////////////////////////////////////////////////////

//================================================================
// Function : QAD_ComboBox::QAD_ComboBox
// Purpose  : constructor
//================================================================
QAD_ComboBox::QAD_ComboBox(bool rw, QWidget* parent, const char* name) :
QComboBox(rw, parent, name) 
{
}

//================================================================
// Function : QAD_ComboBox::findItem
// Purpose  : searches item in list and returns its index
//================================================================
int QAD_ComboBox::findItem(const QString theText) 
{
  for (int i = 0; i < count(); i++) 
    if (text(i) == theText)
      return i;
  return -1;
}

//================================================================
// Function : QAD_ComboBox::insertItem
// Purpose  : adds item in combo box
//================================================================
void QAD_ComboBox::insertItem(const QString& theValue,
			      int            theIndex) 
{
  if (duplicatesEnabled() || findItem(theValue) < 0)
    QComboBox::insertItem(theValue, theIndex);
}

//================================================================
// Function : QAD_ComboBox::insertList
// Purpose  : adds list of items in combo box
//================================================================
void QAD_ComboBox::insertList(const QStringList& theList) 
{
  for (unsigned i = 0; i < theList.count(); i++)
    insertItem(theList[i]);
}

//================================================================
// Function : QAD_ComboBox::insertItem
// Purpose  : adds item in combo box
//================================================================
void QAD_ComboBox::insertItem(const int theValue) 
{
  int aNum;
  bool bOk;
  for (int i = 0; i < count(); i++) {
    aNum = text(i).toInt(&bOk);
    if (bOk) {
      if (aNum > theValue || (aNum == theValue && duplicatesEnabled())) {
        insertItem(QString::number(theValue), i);
        return;
      }
    }
  }
  insertItem(QString::number(theValue));
}

//================================================================
// Function : QAD_ComboBox::insertList
// Purpose  : adds list of items in combo box
//================================================================
void QAD_ComboBox::insertList(const TColStd_ListOfInteger& theList) 
{
  for (TColStd_ListIteratorOfListOfInteger aIter(theList); aIter.More(); aIter.Next())
    insertItem(aIter.Value());
}

//================================================================
// Function : QAD_ComboBox::insertItem
// Purpose  : adds item in combo box
//================================================================
void QAD_ComboBox::insertItem(const double theValue) 
{
  double aNum;
  bool bOk;
  for (int i = 0; i < count(); i++) {
    aNum = text(i).toDouble(&bOk);
    if (bOk) {
      if (aNum > theValue || (aNum == theValue && duplicatesEnabled())) {
        insertItem(QString::number(theValue), i);
        return;
      }
    }
  }
  insertItem(QString::number(theValue));
}

//================================================================
// Function : QAD_ComboBox::insertList
// Purpose  : adds list of items in combo box
//================================================================
void QAD_ComboBox::insertList(const TColStd_ListOfReal& theList) 
{
  for (TColStd_ListIteratorOfListOfReal aIter(theList); aIter.More(); aIter.Next())
    insertItem(aIter.Value());
}

//////////////////////////////////////////////////////////////////////
// QAD_EntityEdit class implementation
//////////////////////////////////////////////////////////////////////

#include <qlayout.h>

#define MIN_COMBO_WIDTH     1
#define MIN_EDIT_WIDTH      1

//================================================================
// Function : QAD_EntityEdit::QAD_EntityEdit
// Purpose  : constructor
//================================================================
QAD_EntityEdit::QAD_EntityEdit(QWidget* parent, 
			       int      controlType,
			       int      valueType,
			       bool     butApply, 
			       bool     butCancel) :
QWidget(parent),
myEdit(0),
myCombo(0),
myApplyBtn(0),
myCancelBtn(0) 
{
  QHBoxLayout* aTopLayout = new QHBoxLayout(this);
  aTopLayout->setAlignment( Qt::AlignTop );
  aTopLayout->setSpacing( 0 );
  aTopLayout->setMargin( 1 );
  if (controlType != etLineEdit && 
      controlType != etComboBox && 
      controlType != etComboEdit)
    controlType = etLineEdit;
  if (controlType == etComboBox || controlType == etComboEdit) {
    // this is an editable combo box
    myCombo = new QAD_ComboBox(controlType == etComboEdit, this);
    myCombo->setMinimumSize(MIN_COMBO_WIDTH, 0);
    myCombo->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,
                                       QSizePolicy::Fixed));
    // no insertions
    myCombo->setInsertionPolicy(QComboBox::NoInsertion);
    // no duplicates enabled by default
    myCombo->setDuplicatesEnabled(false);
    aTopLayout->addWidget(myCombo);
    // connect signals
    connect(myCombo, SIGNAL(activated(const QString&)), this, SLOT(onComboActivated(const QString&)));
    connect(myCombo, SIGNAL(textChanged(const QString&)), this, SLOT(onTextChanged(const QString&)));
  }
  else {
    // and this is an edit box
    myEdit = new QAD_EditBox(this);
    myEdit->setMinimumSize(MIN_EDIT_WIDTH, 0);
    myEdit->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,
                                      QSizePolicy::Fixed));
    aTopLayout->addWidget(myEdit);
    connect(myEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onTextChanged(const QString&)));
    connect(myEdit, SIGNAL(returnPressed()), this, SLOT(onApply()));
    connect(myEdit, SIGNAL(escapePressed()), this, SLOT(onCancel()));
  }
  if (valueType != vtString && 
      valueType != vtInteger && 
      valueType != vtDouble)
    valueType = vtString;
  if (valueType == vtInteger)
    setValidator(new QIntValidator(this));
  else if (valueType == vtDouble)
    setValidator(new QDoubleValidator(this));
  if (butApply) {
    // Apply button (V)
    myApplyBtn = new QToolButton(this);
    QPixmap anIcon = QAD_Desktop::getResourceManager()->loadPixmap(
                          "QAD",tr("ICON_APPLY_BTN"));
    myApplyBtn->setPixmap(anIcon);
    myApplyBtn->setEnabled(false);
    myApplyBtn->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    myApplyBtn->setMinimumSize(16, 16);
    myApplyBtn->setMaximumSize(16, 20);
    aTopLayout->addWidget(myApplyBtn);
    connect(myApplyBtn, SIGNAL(clicked()), this, SLOT(onApply()));
  }
  if (butCancel) {
    // Cancel button (X)
    myCancelBtn = new QToolButton(this);
    QPixmap anIcon = QAD_Desktop::getResourceManager()->loadPixmap(
                          "QAD",tr("ICON_CANCEL_BTN"));
    myCancelBtn->setPixmap(anIcon);
    myCancelBtn->setEnabled(false);
    myCancelBtn->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    myCancelBtn->setMinimumSize(16, 16);
    myCancelBtn->setMaximumSize(16, 20);
    aTopLayout->addWidget(myCancelBtn);
    connect(myCancelBtn, SIGNAL(clicked()), this, SLOT(onCancel()));
  }
}

//================================================================
// Function : QAD_EntityEdit::~QAD_EntityEdit
// Purpose  : destructor
//================================================================
QAD_EntityEdit::~QAD_EntityEdit() 
{
}

//================================================================
// Function : QAD_EntityEdit::clear
// Purpose  : clears edit/combo box
//================================================================
void QAD_EntityEdit::clear() 
{
  if (myEdit)
    myEdit->clear();
  if (myCombo)
    myCombo->clear();
}

//================================================================
// Function : QAD_EntityEdit::getText
// Purpose  : returns current text in edit box or combo box
//================================================================
QString QAD_EntityEdit::getText() 
{
  if (myEdit)
    return myEdit->text();
  else if (myCombo)
    return myCombo->currentText();
  else
    return "";
}

//================================================================
// Function : QAD_EntityEdit::setText
// Purpose  : sets text
//================================================================
void QAD_EntityEdit::setText(const QString& theText) 
{
  myString = theText;
  if (myEdit)
    myEdit->setText(theText);
  if (myCombo) {
    int aFound = myCombo->findItem(theText);
    if (aFound >= 0) {
      myCombo->setCurrentItem(aFound);
      onTextChanged(theText);
    }
  }
}

//================================================================
// Function : QAD_EntityEdit::insertItem
// Purpose  : adds item in combo box, 
//            sets it current if theSetCurrent is true
//================================================================
void QAD_EntityEdit::insertItem(const QString& theValue, 
				bool           theSetCurrent,
				int            theOrder) 
{
  if (myCombo) {
    int aIndexAt = -1;
    if (theOrder == atTop)
      aIndexAt = 0;
    else if (theOrder == atBeforeCurrent && myCombo->count() > 0)
      aIndexAt = myCombo->currentItem();
    else if (theOrder == atAfterCurrent && 
             myCombo->count() > 0 && 
             myCombo->currentItem() < myCombo->count()-1)
      aIndexAt = myCombo->currentItem() + 1;
    myCombo->insertItem(theValue, aIndexAt);
  }
  if (theSetCurrent)
    setText(theValue);
}

//================================================================
// Function : QAD_EntityEdit::insertList
// Purpose  : adds items in combo box, 
//            sets item theCurrent as current
//================================================================
void QAD_EntityEdit::insertList(const QStringList& theList, 
				const int          theCurrent) 
{
  if (myCombo)
    myCombo->insertList(theList);
  if (theCurrent >= 0 && theCurrent < (int)theList.count())
    setText(theList[theCurrent]);
}

//================================================================
// Function : QAD_EntityEdit::insertItem
// Purpose  : adds item in combo box, 
//            sets it current if theSetCurrent is true
//================================================================
void QAD_EntityEdit::insertItem(const int theValue, 
				bool      theSetCurrent) 
{
  if (myCombo) {
    myCombo->insertItem(theValue);
  }
  if (theSetCurrent)
    setText(QString::number(theValue));
}

//================================================================
// Function : QAD_EntityEdit::insertList
// Purpose  : adds items in combo box, 
//            sets item theCurrent as current
//================================================================
void QAD_EntityEdit::insertList(const TColStd_ListOfInteger& theList, 
				const int                    theCurrent) 
{
  if (myCombo)
    myCombo->insertList(theList);

  TColStd_ListIteratorOfListOfInteger aIter(theList);
  for (unsigned i = 0; aIter.More(); aIter.Next(), i++) {
    if (theCurrent == i) {
      setText(QString::number(aIter.Value()));
      break;
    }
  }
}

//================================================================
// Function : QAD_EntityEdit::insertItem
// Purpose  : adds item in combo box, 
//            sets it current if theSetCurrent is true
//================================================================
void QAD_EntityEdit::insertItem(const double theValue, 
				bool         theSetCurrent) 
{
  if (myCombo) {
    myCombo->insertItem(theValue);
  }
  if (theSetCurrent)
    setText(QString::number(theValue));
}

//================================================================
// Function : QAD_EntityEdit::insertList
// Purpose  : adds items in combo box, 
//            sets item theCurrent as current
//================================================================
void QAD_EntityEdit::insertList(const TColStd_ListOfReal& theList, 
				const int                 theCurrent) 
{
  if (myCombo)
    myCombo->insertList(theList);
  
  TColStd_ListIteratorOfListOfReal aIter(theList);
  for (unsigned i = 0; aIter.More(); aIter.Next(), i++) {
    if (theCurrent == i) {
      setText(QString::number(aIter.Value()));
      break;
    }
  }
}

//================================================================
// Function : QAD_EntityEdit::getControl
// Purpose  : gets actual widget
//================================================================
QWidget* QAD_EntityEdit::getControl() 
{
  if (myEdit)
    return myEdit;
  else if (myCombo)
    return myCombo;
  else
    return 0;
}

//================================================================
// Function : QAD_EntityEdit::setFocus
// Purpose  : redirect focus to corresponding widget
//================================================================
void QAD_EntityEdit::setFocus() 
{
  if (myEdit) {
    myEdit->setFocus();
    //myEdit->selectAll();
  }
  else if (myCombo && myCombo->editable()) {
    myCombo->setFocus();
    //myCombo->lineEdit()->selectAll();
  }
}

//================================================================
// Function : QAD_EntityEdit::setValidator
// Purpose  : sets validator for the control
//================================================================
void QAD_EntityEdit::setValidator(const QValidator* theValidator) 
{
  if (myEdit)
    myEdit->setValidator(theValidator);
  if (myCombo)
    myCombo->setValidator(theValidator);
}

//================================================================
// Function : QAD_EntityEdit::keyPressEvent
// Purpose  : event filter for KeyPress event
//================================================================
void QAD_EntityEdit::keyPressEvent( QKeyEvent * e) 
{
  if ( (e->key() == Key_Enter ||
        e->key() == Key_Return ) )
    onApply();
  else if (e->key() == Key_Escape)
    onCancel();
}

//================================================================
// Function : QAD_EntityEdit::onComboActivated
// Purpose  : called when item activated in combo box
//================================================================
void QAD_EntityEdit::onComboActivated(const QString& theText) 
{
  onTextChanged(theText);
}

//================================================================
// Function : QAD_EntityEdit::onTextChanged
// Purpose  : slot, called when text changed in line edit
//================================================================
void QAD_EntityEdit::onTextChanged(const QString& theText) 
{
  if (myApplyBtn)
    myApplyBtn->setEnabled(!(theText == myString));
  if (myCancelBtn)
    myCancelBtn->setEnabled(!(theText == myString));
}

//================================================================
// Function : QAD_EntityEdit::onCancel
// Purpose  : slot, called when user presses Cancel button
//================================================================
void QAD_EntityEdit::onCancel() 
{
  setText(myString);
  if (myApplyBtn)
    myApplyBtn->setEnabled(false);
  if (myCancelBtn)
    myCancelBtn->setEnabled(false);
  emit escapePressed();
}

//================================================================
// Function : QAD_EntityEdit::onApply
// Purpose  : slot, called when user presses Apply button
//================================================================
void QAD_EntityEdit::onApply() 
{
  myString = getText();
  if (myApplyBtn)
    myApplyBtn->setEnabled(false);
  if (myCancelBtn)
    myCancelBtn->setEnabled(false);
  emit returnPressed();
}

//================================================================
// Function : QAD_EntityEdit::showButtons
// Purpose  : shows/hides buttons
//================================================================
void QAD_EntityEdit::showButtons(bool show) 
{
  if (myApplyBtn)
    show ? myApplyBtn->show()  : myApplyBtn->hide();
  if (myCancelBtn)
    show ? myCancelBtn->show() : myCancelBtn->hide();
}

//================================================================
// Function : QAD_EntityEdit::setDuplicatesEnabled
// Purpose  : enables/disables data duplication (for combo box)
//================================================================
void QAD_EntityEdit::setDuplicatesEnabled(bool enabled)
{
  if (myCombo)
    myCombo->setDuplicatesEnabled(enabled);
}
