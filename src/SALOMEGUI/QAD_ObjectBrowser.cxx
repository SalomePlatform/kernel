//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : QAD_ObjectBrowser.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "QAD_ObjectBrowserItem.h"
#include "QAD_ObjectBrowser.h"
#include "QAD_Application.h"
#include "QAD_MessageBox.h"
#include "QAD_Desktop.h"
#include "QAD_Config.h"
#include "QAD_Settings.h"
#include "QAD_Tools.h"
#include "QAD_RightFrame.h"
#include "QAD_LeftFrame.h"
#include "SALOME_Selection.h"
#include "SALOME_InteractiveObject.hxx"
#include "SALOME_ListIteratorOfListIO.hxx"
#include "SALOMEGUI_SetValueDlg.h"
#include "SALOMEGUI_TableDlg.h"
#include "SALOMEGUI_NameDlg.h"
 
// QT Includes
#include <qlistview.h>
#include <qheader.h>
#include <qpopupmenu.h>
#include <qtabwidget.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvbox.h>
#include <qhbox.h>
#include <qtooltip.h>
#include <qdragobject.h>
#include <qstringlist.h>
#include <qmap.h>

using namespace std;

//VRV: porting on Qt 3.0.5
#if QT_VERSION >= 0x030005
#include <qcursor.h> 
#endif
//VRV: porting on Qt 3.0.5

#define UC_NEW_EMPTY_ID    1000000
#define UC_NEW_COMP_ID     1000002
#define UC_NEW_FULL_ID     1000004
#define UC_NEW_COPY_ID     1000006
#define UC_APPEND_ID       1000008
#define UC_REMOVE_ID       1000010
#define UC_RENAME_ID       1000012
#define UC_CLEAR_ID        1000014
#define UC_SET_CURRENT_ID  1000016


#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif


/*!
  Small button which updates Object Browser's contents
*/
Btn::Btn ( QWidget * parent, const char * name ) : QToolButton( parent, name ) 
{
  connect( this, SIGNAL(clicked()), this, SLOT(onClicked()) );
}
void Btn::onClicked()
{
  QAD_ObjectBrowser* OB = QAD_Application::getDesktop()->getActiveApp()->getActiveStudy()->getActiveStudyFrame()->getLeftFrame()->getObjectBrowser();
  OB->Update();
}

/*!
  Gets selected top-level items (i.e. not including sub-items) [ static ]
*/
static void getSelectedParents( QListViewItem* listViewItem, QList<QListViewItem>& itemList )
{
  QListViewItem* item = listViewItem->firstChild();
  while ( item ) {
    if ( item->isSelected() ) {
      itemList.append( item );
    }
    else {
      getSelectedParents( item, itemList );
    }
    item = item->nextSibling();
  }
}
/*!
  Gets selected top-level items (i.e. not including sub-items) [ static ]
*/
static void getSelectedParents( QListView* listView, QList<QListViewItem>& itemList, bool ignoreRoot = false )
{
  itemList.clear();
  QListViewItem* item = listView->firstChild();
  while ( item ) {
    if ( item->isSelected() && !ignoreRoot ) {
      itemList.append( item );
    }
    else {
      getSelectedParents( item, itemList );
    }
    item = item->nextSibling();
  }
}
/*!
  Gets top-most parent item [ static ]
*/
static QListViewItem* getTopParent( QListViewItem* item )
{
  if ( !item )
    return 0;
  QListViewItem* p = item->parent();
  if ( !p )
    return item;
  while ( p->parent() ) {
    p = p->parent();
  }
  return p;
}
/*!
  Returns true if the item is top-level (root)
*/
static bool isRootItem( QListViewItem* item )
{
  return ( item->listView() && !item->parent() );
}
/*!
  Constructor
*/
QAD_ObjectBrowser::QAD_ObjectBrowser( SALOMEDS::Study_var study, QWidget* parent, const char* name, WFlags fl )
     : QTabWidget( parent, name, fl )
{
  myStudy = SALOMEDS::Study::_duplicate( study );
  myListViewMap.clear();
  myUseCaseMap.clear();
  myListView=0;    // must be done before setupListView(): setCornerWidget() provoque call to eventFilter
  myUseCaseView=0; // and test myUseCaseView->viewport() before initialisation
  setupListView();
}

/*!
  Destructor
*/
QAD_ObjectBrowser::~QAD_ObjectBrowser() 
{
  if (!myStudy->_is_nil())
    CORBA::release(myStudy);
}

/*!
  Configures Object Browser and UseCase Browser. 
  Columns, Selection Mode and Palette.
*/
void QAD_ObjectBrowser::setupListView()
{
  QAD_ResourceMgr* resMgr = QAD_Desktop::createResourceManager();

  this->setTabPosition( QTabWidget::Bottom );
  
  /* Reading setting : Columns for Value, OCAF Doc entry, object IOR, OCAF Doc ref entry and Chrono sorting */
  QString AddColumn       = QAD_CONFIG->getSetting( "ObjectBrowser:AddColumn"   );
  QString ValueColumn     = QAD_CONFIG->getSetting( "ObjectBrowser:ValueColumn" );
  QString ShowCHRONO_SORT = QAD_CONFIG->getSetting( "ObjectBrowser:ChronologicalSort" );
  QString showUseCase = QAD_CONFIG->getSetting("ObjectBrowser:ShowUseCaseBrowser");
  QString noAutoSizeColumns = QAD_CONFIG->getSetting( "ObjectBrowser:NoAutoSizeColumns" );
 
  /* create and setup Object Browser ================================= */
  myListView = new QListView( this, "Object Browser");
  myListView->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );
  myListView->setMinimumSize( 1, 1 );
  myListView->setPalette( QAD_Application::getPalette( true ) );

  /* First two columns = object name and value - always visible */
  myListView->addColumn( tr( "OBJECT_BROWSER_OBJECT" ) );   /* Adding Object column             */
  myListView->addColumn( tr( "OBJECT_BROWSER_VALUE" ) );    /* Adding Value column              */
  myListView->addColumn( tr( "OBJECT_BROWSER_ENTRY" ) );    /* Adding Doc entry column          */
  myListView->addColumn( tr( "OBJECT_BROWSER_IOR" ) );      /* Adding Object IOR column         */
  myListView->addColumn( tr( "OBJECT_BROWSER_REFENTRY" ) ); /* Adding OCAF Doc ref entry column */
  myListView->header()->setMovingEnabled( false );
 
  /* Properties */
  myListView->header()->setClickEnabled( TRUE );          /* Enable clicking on the header                    */
  myListView->setShowSortIndicator( TRUE ) ;              /* Add user arrows to indicate the sort order : LPN */
  myListView->setRootIsDecorated( TRUE );                 /* Enable expand/collapse control for the root item */
  myListView->setSelectionMode ( QListView::Extended );   /* Enable multiple selection                        */
  myListView->setVScrollBarMode( QScrollView::AlwaysOn ); /* Set scrollbars always visible                    */
  myListView->setHScrollBarMode( QScrollView::AlwaysOn ); /* ...                                              */

  myListView->setCornerWidget( new Btn( this ) );
  /* Connect section */
  //VRV: porting on Qt 3.0.5
#if QT_VERSION < 0x030005
  connect( myListView, SIGNAL( rightButtonClicked( QListViewItem*, const QPoint&, int ) ),
 	   this,       SLOT(   showPopupMenu( QListViewItem* ) ) );
#else
  connect( myListView, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ),
	   this,       SLOT(   showPopupMenu( QListViewItem* ) ) );
#endif  
  //VRV: porting on Qt 3.0.5

  connect( myListView, SIGNAL( selectionChanged() ),
	   this,       SLOT( onSelectedItem() ) );
  connect( myListView, SIGNAL( expanded( QListViewItem* ) ),
	   this,       SLOT( onExpanded( QListViewItem* ) ) );
  connect( myListView, SIGNAL( collapsed( QListViewItem* ) ),
	   this,       SLOT( onCollapsed( QListViewItem* ) ) );
    
  /* create and setup UseCase Browser ================================ */
  myVBox = new QWidget( this );
  QVBoxLayout* vBoxLayout = new QVBoxLayout( myVBox );
  
  myUseCaseView = new QListView( myVBox, "UseCase Browser");
  myUseCaseView->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );
  myUseCaseView->setMinimumSize( 1, 1 );
  myUseCaseView->setPalette( QAD_Application::getPalette( true ) );

  /* First column = object name */
  myUseCaseView->addColumn( tr( "OBJECT_BROWSER_OBJECT" ) );
  /* Properties */
  myUseCaseView->header()->setClickEnabled( TRUE );          /* Enable clicking on the header                    */
  myUseCaseView->setShowSortIndicator( TRUE ) ;              /* Add user arrows to indicate the sort order : LPN */
  myUseCaseView->setRootIsDecorated( TRUE );                 /* Enable expand/collapse control for the root item */
  myUseCaseView->setSelectionMode ( QListView::Extended );   /* Enable multiple selection                        */
  myUseCaseView->setVScrollBarMode( QScrollView::AlwaysOn ); /* Set scrollbars always visible                    */
  myUseCaseView->setHScrollBarMode( QScrollView::AlwaysOn ); /* ...                                              */
  myUseCaseView->header()->setMovingEnabled( false );
  myUseCaseView->viewport()->setAcceptDrops( true );
  myUseCaseView->installEventFilter( this );
  myUseCaseView->viewport()->installEventFilter( this );
  myUseCaseView->setSorting( -1 ) ;
  vBoxLayout->addWidget( myUseCaseView ); 

  myUseCaseView->setCornerWidget( new Btn( this ) );

  myNewBtn = new QToolButton( myVBox );
  myNewBtn->setIconSet( resMgr->loadPixmap( "QAD", tr("ICON_UC_NEW") ) );
  myNewBtn->setAutoRaise( true );
  QToolTip::add( myNewBtn, tr( "UC_NEW_ID" ), QAD_Application::getDesktop()->toolTipGroup(), tr( "UC_NEW_TIP" ) );
  myAddBtn = new QToolButton( myVBox );
  myAddBtn->setIconSet( resMgr->loadPixmap( "QAD", tr("ICON_UC_ADD") ) );
  myAddBtn->setAutoRaise( true );
  QToolTip::add( myAddBtn, tr( "UC_APPEND_ID" ), QAD_Application::getDesktop()->toolTipGroup(), tr( "UC_APPEND_TIP" ) );
  myDelBtn = new QToolButton( myVBox );
  myDelBtn->setIconSet( resMgr->loadPixmap( "QAD", tr("ICON_UC_REMOVE") ) );
  myDelBtn->setAutoRaise( true );
  QToolTip::add( myDelBtn, tr( "UC_REMOVE_ID" ), QAD_Application::getDesktop()->toolTipGroup(), tr( "UC_REMOVE_TIP" ) );
  myClearBtn = new QToolButton( myVBox );
  myClearBtn->setIconSet( resMgr->loadPixmap( "QAD", tr("ICON_UC_CLEAR") ) );
  myClearBtn->setAutoRaise( true );
  QToolTip::add( myClearBtn, tr( "UC_CLEAR_ID" ), QAD_Application::getDesktop()->toolTipGroup(), tr( "UC_CLEAR_TIP" ) );
  myCurrentBtn = new QToolButton( myVBox );
  myCurrentBtn->setIconSet( resMgr->loadPixmap( "QAD", tr("ICON_UC_SET_CURRENT") ) );
  myCurrentBtn->setAutoRaise( true );
  QToolTip::add( myCurrentBtn, tr( "UC_SET_CURRENT_ID" ), QAD_Application::getDesktop()->toolTipGroup(), tr( "UC_SET_CURRENT_TIP" ) );

  QHBoxLayout* btnLayout = new QHBoxLayout;
  btnLayout->setMargin( 3 ); btnLayout->setSpacing( 3 );
  btnLayout->addWidget( myNewBtn );
  btnLayout->addStretch();
  btnLayout->addSpacing( 5 );
  btnLayout->addStretch();
  btnLayout->addWidget( myAddBtn );
  btnLayout->addWidget( myDelBtn );
  btnLayout->addStretch();
  btnLayout->addSpacing( 5 );
  btnLayout->addStretch();
  btnLayout->addWidget( myClearBtn );
  btnLayout->addStretch();
  btnLayout->addSpacing( 5 );
  btnLayout->addStretch();
  btnLayout->addWidget( myCurrentBtn );
  btnLayout->addStretch();

  vBoxLayout->addLayout( btnLayout ); 

  connect( myNewBtn,     SIGNAL( clicked() ), this, SLOT( onUseCaseBtn() ) );
  connect( myAddBtn,     SIGNAL( clicked() ), this, SLOT( onUseCaseBtn() ) );
  connect( myDelBtn,     SIGNAL( clicked() ), this, SLOT( onUseCaseBtn() ) );
  connect( myClearBtn,   SIGNAL( clicked() ), this, SLOT( onUseCaseBtn() ) );
  connect( myCurrentBtn, SIGNAL( clicked() ), this, SLOT( onUseCaseBtn() ) );

  /* Connect section */
#if QT_VERSION < 0x030005
  connect( myUseCaseView, SIGNAL( rightButtonClicked( QListViewItem*, const QPoint&, int ) ),
 	   this,       SLOT(   showPopupMenu( QListViewItem* ) ) );
#else
  connect( myUseCaseView, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ),
	   this,       SLOT(   showPopupMenu( QListViewItem* ) ) );
#endif  
  
  connect( myUseCaseView, SIGNAL( selectionChanged() ),
	   this,       SLOT( onSelectedItem() ) );

  /* add Object Browser and UseCase Browser as pages ================= */
  this->addTab( myListView,    tr( "TLT_OBJECT_BROWSER" ) );
  this->addTab( myVBox, tr( "TLT_USECASE_BROWSER" ) );
  
  if ( showUseCase != "true")
    this->removePage(myVBox);
 
  setShowInfoColumns( AddColumn == "true" );
  setShowValueColumn( ValueColumn == "true" );
  setEnableChronoSort( ShowCHRONO_SORT == "true" );
  
  if ( noAutoSizeColumns == "true" ) 
    {
      for (int i = 0; i < myListView->header()->count(); i++ )
	myListView->setColumnWidthMode(i, QListView::Manual);
    }

  resize( QSize( 100, 400 ) );
}

/*!
  Event filter
*/

bool QAD_ObjectBrowser::eventFilter( QObject* o, QEvent* e )
{
  QAD_Desktop* Desktop     = (QAD_Desktop*) QAD_Application::getDesktop();
  QAD_Study* myActiveStudy = Desktop->getActiveStudy();
 
  if ( myStudy->_is_nil() )
    return QTabWidget::eventFilter( o, e );

  SALOMEDS::UseCaseBuilder_var UCBuilder = myStudy->GetUseCaseBuilder();
  if (! myUseCaseView) return QTabWidget::eventFilter( o, e );
  if (o == myUseCaseView->viewport()) {
    if ( e->type() == QEvent::MouseButtonPress ) {
      // Test if clicked on selection and start drag if necessary
      QMouseEvent* me = ( QMouseEvent* )e;
      QAD_ObjectBrowserItem* item = ( QAD_ObjectBrowserItem* )( myUseCaseView->itemAt( me->pos() ) );
      if ( item && item->isSelected() && me->button() == LeftButton) {
	if ( me->pos().x() > myUseCaseView->header()->sectionPos( myUseCaseView->header()->mapToIndex( 0 ) ) +
	                     myUseCaseView->treeStepSize() * ( item->depth() + ( myUseCaseView->rootIsDecorated() ? 1 : 0 ) ) + 
			     myUseCaseView->itemMargin() ||
	     me->pos().x() < myUseCaseView->header()->sectionPos( myUseCaseView->header()->mapToIndex( 0 ) ) ) { 
	  QStringList entryList;
	  QListViewItemIterator it( myUseCaseView );
	  bool done = true;
	  for ( ; it.current(); ++it ) {
	    QAD_ObjectBrowserItem* selItem = ( QAD_ObjectBrowserItem* )( it.current() );
	    if ( selItem->isSelected() ) {
	      if ( isRootItem( selItem ) ) {
		done = false;
		break;
	      }
	      else {
		entryList.append( selItem->getEntry() );
	      }
	    }
	  }
	  if ( done && !entryList.isEmpty() ) {
	    QTextDrag *d = new QTextDrag( entryList.join("*"), myUseCaseView->viewport() );
	    d->dragMove();
	    return true;
	  }
	}
      }
    }
    else if ( e->type() == QEvent::DragMove ) {
      QDragMoveEvent* dme = ( QDragMoveEvent* )e;
      if ( dme->source() == myUseCaseView->viewport() ) {
	dme->accept();
      }
      else {
	dme->ignore();
      }
      return true;
    }
    else if ( e->type() == QEvent::Drop ) {
      QDropEvent* de = ( QDropEvent* )e;
      QString text;
      QAD_ObjectBrowserItem* dropItem = ( QAD_ObjectBrowserItem* )( myUseCaseView->itemAt( de->pos() ) );
      if ( de->source() == myUseCaseView->viewport() ) {
	if ( QTextDrag::decode( de, text ) && dropItem && !dropItem->isSelected() ) {
	  QAD_ObjectBrowserItem* useCaseItem = ( QAD_ObjectBrowserItem* )getTopParent( dropItem );
	  if ( useCaseItem ) {
	    if ( !UCBuilder->_is_nil() ) {
	      UCBuilder->SetRootCurrent();
	      if ( useCaseItem != dropItem ) {
		SALOMEDS::SObject_var dropObject = myStudy->FindObjectID( dropItem->getEntry() );
		if ( !dropObject->_is_nil() )
		  UCBuilder->SetCurrentObject( dropObject );
	      }
	      QStringList entryList = QStringList::split( "*", text, false );
	      // if Move action then first remove all selected items
	      if ( de->action() == QDropEvent::Move ) {
		QList<QListViewItem> ucSelected; 
		ucSelected.setAutoDelete( false );
		getSelectedParents( myUseCaseView, ucSelected );
		for ( int j = 0; j < entryList.count(); j++ ) {
		  QAD_ObjectBrowserItem* delItem = ( QAD_ObjectBrowserItem* )( ucSelected.at( j ) );
		  QAD_ObjectBrowserItem* ucDelItem = ( QAD_ObjectBrowserItem* )getTopParent( delItem );
		  SALOMEDS::SObject_var delSO = myStudy->FindObjectID( delItem->getEntry() );
		  if ( !delSO->_is_nil() && ucDelItem ) {
		    UCBuilder->Remove( delSO );
		  }
		}
	      }
	      // then try to append items to destination
	      for ( int i = 0; i < entryList.count(); i++ ) {
		SALOMEDS::SObject_var SO = myStudy->FindObjectID( entryList[i] );
		if ( !SO->_is_nil() ) {
		  UCBuilder->Remove( SO );
		  UCBuilder->Append( SO );
		}
	      }
	      myActiveStudy->updateUseCaseBrowser();
	    }
	  }
	}
	de->acceptAction();
      }
      else {
	de->ignore();
      }
      return true;
    }
  }
  else if ( o == myUseCaseView ) {
    if ( e->type() == QEvent::KeyPress ) {
      QKeyEvent* ke = ( QKeyEvent* )e;
      if ( ke->key() == Key_Delete ) {
	onUseCasePopupMenu( UC_REMOVE_ID );
	return true;
      }
    }
  }
  return QTabWidget::eventFilter( o, e );
}


/*!
  Returns true if item is collapsed has collapsed children
*/
static bool hasCollapsed( QListViewItem* item )
{
  if ( !item )
    return false;

  bool res = ( item->childCount() && !item->isOpen() );
  for ( QListViewItem* child = item->firstChild(); !res && child; child = child->nextSibling() )
    res = hasCollapsed( child );

  return res;
}

/*!
  Expands item and all its children
*/
static void expand(QListViewItem* item)
{
  if ( !item )
    return;

  item->setOpen( true );

  for ( QListViewItem* child = item->firstChild(); child; child = child->nextSibling() )
    expand( child );
}

/*!
  Builds popup menu
*/
void QAD_ObjectBrowser::onCreatePopup()
{
  QAD_Desktop*     Desktop = (QAD_Desktop*) QAD_Application::getDesktop();
  QAD_Study*   myActiveStudy = Desktop->getActiveStudy();
  SALOME_Selection*      Sel = SALOME_Selection::Selection( myActiveStudy->getSelection() );

  bool canExpand = false;
  /* VSR : Creation of common POPUP menu for Object Browser/Use Case Browser */
  if ( Sel->IObjectCount() > 0 ) {
    QString theContext;
    QString theParent;
    QString theObject;
    
    Desktop->definePopup( theContext, theParent, theObject );
    if ( myPopup ) {
      Desktop->createPopup( myPopup, theContext, "ObjectBrowser", theObject);
      if ( !Desktop->getActiveComponent().isEmpty() ) {
	Desktop->customPopup( myPopup, theContext, "ObjectBrowser", theObject );
      }
      // VSR:22/01/03 - Command for edit attributes value
      bool EditValues  = QAD_CONFIG->getSetting( "ObjectBrowser:EditValues" ) == "true";
      if ( EditValues && Sel->IObjectCount() == 1 && hasEditableAttribute(Sel->firstIObject()) ) {
	if ( myPopup->count() > 0 )
	  myPopup->insertSeparator();
	myPopup->insertItem( tr( "EDIT_VALUE_CMD" ),
			     this,
			     SLOT( onEditAttribute() ) );
      }

      for ( QListViewItemIterator it( currentPage() == myListView ? myListView : myUseCaseView ); it.current() && !canExpand; ++it )
	canExpand = canExpand || ( it.current()->isSelected() && hasCollapsed( it.current() ) );

      if ( canExpand ) {
	if ( myPopup->count() > 0 )
	  myPopup->insertSeparator();
	myPopup->insertItem( tr( "EXPAND_ALL_CMD" ),
			     this,
			     SLOT( onExpandAll() ) );
      }
    }
  } else {
    // NRI 02/12/2002
    // No selected item => it's not necessary to create popup
    //NRI- :  Desktop->createPopup( myPopup, "", "ObjectBrowser", "");
  }
  if ( currentPage() != myListView ) {
    /* VSR : Creation of POPUP menu for UseCase Browser */
    QList<QListViewItem> ucSelected; 
    ucSelected.setAutoDelete( false );
    getSelectedParents( myUseCaseView, ucSelected );
    
    if ( myPopup && !myStudy->_is_nil() ) {
      SALOMEDS::UseCaseBuilder_var UCBuilder = myStudy->GetUseCaseBuilder();
//      myPopup->clear();
      bool isOne = ucSelected.count() == 1;
      bool isRoot = isOne && isRootItem( ucSelected.at( 0 ) );
      bool manyChildren = myUseCaseView->childCount() > 0 && myUseCaseView->firstChild()->childCount() > 0;
      bool isUseCase = isOne && 
	( isRoot || UCBuilder->IsUseCase( myStudy->FindObjectID( (( QAD_ObjectBrowserItem* )( ucSelected.at(0) ))->getEntry() ) ) ); 

      if ( isRoot ) {
	myPopup->clear();
	if ( canExpand ) {
	  myPopup->insertItem( tr( "EXPAND_ALL_CMD" ),
			       this,
			       SLOT( onExpandAll() ) );
	}
      }
      QPopupMenu *UseCasePopup = new QPopupMenu( myPopup );
      if ( isOne )
	UseCasePopup->insertItem( tr( "UC_NEW_ID" ), this, SLOT( onUseCasePopupMenu( int ) ), 0, UC_NEW_EMPTY_ID );
      if ( isOne )
	UseCasePopup->insertItem( tr( "UC_SET_CURRENT_ID" ), this, SLOT( onUseCasePopupMenu( int ) ), 0, UC_SET_CURRENT_ID );
      if ( isUseCase )
	UseCasePopup->insertItem( tr( "UC_RENAME_ID" ),    this, SLOT( onUseCasePopupMenu( int ) ), 0, UC_RENAME_ID );
      if ( isRoot && manyChildren )
	UseCasePopup->insertItem( tr( "UC_CLEAR_ID" ),   this, SLOT( onUseCasePopupMenu( int ) ), 0, UC_CLEAR_ID );

      if ( UseCasePopup->count() > 0 ) {
	if ( myPopup->count() > 0 )
	  myPopup->insertSeparator();
	myPopup->insertItem( tr( "UC_POPUP" ), UseCasePopup );
      }
      else {
	delete UseCasePopup;
      }
    }
  }
}

/*!
  Called on "Expand all" popup menu command
*/
void QAD_ObjectBrowser::onExpandAll()
{
  for ( QListViewItemIterator it( currentPage() == myListView ? myListView : myUseCaseView ); it.current(); ++it )
    if ( it.current()->isSelected() )
      expand( it.current() );  
}

/*!
  Returns Object Browser list view
*/
QListView* QAD_ObjectBrowser::getListView() const
{
  return myListView;
}

/*!
  Returns UseCase Browser list view
*/
QListView* QAD_ObjectBrowser::getUseCaseView() const
{
  return myUseCaseView;
}

/*!
  Adds anItem ( probably a SComponent )
*/
QAD_ObjectBrowserItem* QAD_ObjectBrowser::AddItem(QListView*             theView,
						  const QString&         theName,
						  const QString&         theEntry,
						  const QString&         theIOR, 
						  int                    theType, 
						  const QString&         theValue,
						  QAD_ObjectBrowserItem* theAfter)

{
  QAD_ObjectBrowserItem* last = (QAD_ObjectBrowserItem*)(theView->firstChild());
  if ( theAfter )
    last = theAfter;
  else if ( last ) {
    for ( ; last->nextSibling(); last = (QAD_ObjectBrowserItem*)(last->nextSibling()) );
  }
  QAD_ObjectBrowserItem* anItem = new QAD_ObjectBrowserItem( theView, last );
  
  anItem->setType( theType );
  anItem->setOpen( FALSE );
  anItem->setName( theName );
  anItem->setValue( theValue );
  anItem->setEntry( theEntry );
  anItem->setIOR( theIOR );
  anItem->setReference( "" );

  return anItem;
}

/*!
  Adds anItem ( probably a StudyObject )
*/
QAD_ObjectBrowserItem* QAD_ObjectBrowser::AddItem(QAD_ObjectBrowserItem* theParentItem,
						  const QString&         theName,
						  const QString&         theEntry,
						  const QString&         theIOR,
						  int                    theType,
						  const QString&         theRef,
						  const QString&         theValue,
						  QAD_ObjectBrowserItem* theAfter)

{
  QAD_ObjectBrowserItem* last = (QAD_ObjectBrowserItem*)(theParentItem->firstChild());
  if ( theAfter )
    last = theAfter;
  else if ( last ) {
    for ( ; last->nextSibling(); last = (QAD_ObjectBrowserItem*)(last->nextSibling()) );
  }
  QAD_ObjectBrowserItem* anItem = new QAD_ObjectBrowserItem(theParentItem, last);
  anItem->setType( theType );
  anItem->setOpen( FALSE );
  anItem->setName( theName );
  anItem->setValue( theValue );
  anItem->setEntry( theEntry );
  anItem->setIOR( theIOR );
  anItem->setReference( theRef );

  return anItem;
}

/*!
  Updates Object Browsers' item [ protected ]
*/
void QAD_ObjectBrowser::Update( SALOMEDS::SObject_ptr SO,
			        QAD_ObjectBrowserItem* theParentItem )
{
  if ( myStudy->_is_nil() || SO->_is_nil() || !theParentItem ) {
    return;
  }
  SALOMEDS::ChildIterator_var it = myStudy->NewChildIterator(SO);

  for (; it->More();it->Next()) {
    SALOMEDS::SObject_var             CSO = it->Value();
    SALOMEDS::SObject_var             RefSO;
    QString                           ior = "";
    CORBA::String_var                 aString(CSO->GetID());
    QString                           CSOEntry(aString.in());
    SALOMEDS::GenericAttribute_var    anAttr;
    SALOMEDS::AttributeName_var       aName;
    SALOMEDS::AttributeComment_var    aCmnt;
    SALOMEDS::AttributeIOR_var        anIOR;
    SALOMEDS::AttributeReal_var       aReal;
    SALOMEDS::AttributeInteger_var    anInt;
    SALOMEDS::AttributeSelectable_var aSelectable;
    SALOMEDS::AttributeExpandable_var anExpandable;
    SALOMEDS::AttributeOpened_var     anOpened;
    SALOMEDS::AttributePixMap_var     aPixmap;
    SALOMEDS::AttributeTextColor_var  aTextColor;
    SALOMEDS::AttributeTextHighlightColor_var  aTextHighlightColor;

    QAD_ObjectBrowserItem*            Item = 0;
    QString                           valueString;
   
    if ( CSO->ReferencedObject(RefSO) && !RefSO->_is_nil() ) {
      
      aString = RefSO->GetID();
      QString RefSOEntry(aString.in());
      if (CSO->FindAttribute(anAttr, "AttributeName") || RefSO->FindAttribute(anAttr, "AttributeName")) {
        aName = SALOMEDS::AttributeName::_narrow(anAttr);
        if (RefSO->FindAttribute(anAttr, "AttributeIOR")) {
          anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
	  aString = anIOR->Value();
          ior = aString.in();
        }
 	valueString = getValueFromObject( RefSO );
	aString = aName->Value();
	Item = AddItem(theParentItem, 
		       QString(" * ") + aString.in(), 
		       RefSOEntry, 
		       ior, 
		       2, 
		       CSOEntry, 
		       valueString);
	myListViewMap[ RefSOEntry ].append( Item );
      }
      else {
	if(MYDEBUG) MESSAGE("QAD_ObjectBrowser::Update : noname item: "<<CSO->GetID());
      }
    } else {
      // getting Value
      valueString = getValueFromObject( CSO );
      // getting IOR
      if (CSO->FindAttribute(anAttr, "AttributeIOR"))  {
        anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
	aString = anIOR->Value();
	ior = aString.in();
      }
      // getting Name and adding new Item
      if (CSO->FindAttribute(anAttr, "AttributeName") ) {
        aName = SALOMEDS::AttributeName::_narrow(anAttr);
	aString = aName->Value();
        Item = AddItem(theParentItem, aString.in(), CSOEntry, ior, 0, "", valueString);
	myListViewMap[ CSOEntry ].append( Item );
      } 
      else {
	if(MYDEBUG) MESSAGE("QAD_ObjectBrowser::Update : noname item: "<<CSO->GetID());
      }
      // adding other attributes 
      if (Item) {
        // Selectable
        if ( CSO->FindAttribute(anAttr, "AttributeSelectable") ) {
          aSelectable = SALOMEDS::AttributeSelectable::_narrow(anAttr);
          Item->setSelectable(aSelectable->IsSelectable());
        }
        // Expandable
        if ( CSO->FindAttribute(anAttr, "AttributeExpandable") ) {
          anExpandable = SALOMEDS::AttributeExpandable::_narrow(anAttr);
          Item->setExpandable(anExpandable->IsExpandable());
        }
        // Opened
        if ( CSO->FindAttribute(anAttr, "AttributeOpened") ) {
          anOpened = SALOMEDS::AttributeOpened::_narrow(anAttr);
          Item->setOpen(anOpened->IsOpened());
        }
	// TextColor
        if ( CSO->FindAttribute(anAttr, "AttributeTextColor") ) {
          aTextColor = SALOMEDS::AttributeTextColor::_narrow(anAttr);
	  QColor aColor((int)(aTextColor->TextColor().R), (int)(aTextColor->TextColor().G), (int)(aTextColor->TextColor().B)) ;
          Item->setTextColor(aColor);
        }
	// TextHighlightColor
        if ( CSO->FindAttribute(anAttr, "AttributeTextHighlightColor") ) {
          aTextHighlightColor = SALOMEDS::AttributeTextHighlightColor::_narrow(anAttr);
	  QColor aColor((int)(aTextHighlightColor->TextHighlightColor().R), 
			(int)(aTextHighlightColor->TextHighlightColor().G), 
			(int)(aTextHighlightColor->TextHighlightColor().B)) ;
          Item->setTextHighlightColor(aColor);
        }
	// Pixmap
	if ( CSO->FindAttribute(anAttr, "AttributePixMap") ) {
	  aPixmap = SALOMEDS::AttributePixMap::_narrow(anAttr);
	  if ( aPixmap->HasPixMap() ) {
	    SALOMEDS::SComponent_var father = CSO->GetFatherComponent();
	    if (father->FindAttribute(anAttr, "AttributeName") ) {
	      SALOMEDS::AttributeName_var aFatherName;
	      aFatherName = SALOMEDS::AttributeName::_narrow(anAttr);

	      QString msg;
	      QAD_ResourceMgr* resMgr = QAD_Desktop::createResourceManager();
	      if ( resMgr ) {
		//if(resMgr->loadResources( QString(aFatherName->Value()) + "GUI", msg )) {
		if( resMgr->loadResources( QAD_Application::getDesktop()->getComponentName( QString( aFatherName->Value() ) ), msg ) ) {
		  QPixmap icon ( resMgr->loadPixmap( QAD_Application::getDesktop()->getComponentName( QString( aFatherName->Value() ) ),
						    tr( aPixmap->GetPixMap() )  /*tr( "ICON_OBJBROWSER_" + theComponent )*/ ) );
		  //QPixmap icon ( resMgr->loadPixmap( QString(aFatherName->Value()) + "GUI",
		  //tr(aPixmap->GetPixMap()) /*tr( "ICON_OBJBROWSER_" + theComponent )*/ ));
		  Item->setPixmap( 0, icon );
		}
	      }
	    }
	  }
	}
      } 
    }
//  if ( theParentItem->isOpen() )
//    Item->setOpen(TRUE);

    if ( Item )
      Update(CSO, Item);
  }
}

/*!
  Updates Object Browser tree and UseCase Browser tree
*/
void QAD_ObjectBrowser::Update()
{
  int xc = myListView->contentsX();
  int yc = myListView->contentsY();
  myListView->viewport()->setUpdatesEnabled( false );
 
  myListView->clear();
  myListViewMap.clear();

  if ( myStudy->_is_nil() ) {
    return;
  }

  QString ShowIAPP = QAD_CONFIG->getSetting("ObjectBrowser:IAPP");

  /* Updating Object Browser ============================================== */
  SALOMEDS::SComponentIterator_var itcomp = myStudy->NewComponentIterator();
  for (; itcomp->More(); itcomp->Next()) {
    QAD_ObjectBrowserItem*   Item     = 0;
    SALOMEDS::SComponent_var SC       = itcomp->Value();
    CORBA::String_var        aString  = SC->ComponentDataType();
    QString                  dataType = aString.in();
    QString                  ior      = "";
    aString = SC->GetID();
    QString                  SCEntry  = aString.in();

    SALOMEDS::GenericAttribute_var    anAttr;
    SALOMEDS::AttributeName_var       aName;
    SALOMEDS::AttributeComment_var    aCmnt;
    SALOMEDS::AttributeIOR_var        anIOR;
    SALOMEDS::AttributeReal_var       aReal;
    SALOMEDS::AttributeInteger_var    anInt;
    SALOMEDS::AttributeSelectable_var aSelectable;
    SALOMEDS::AttributeExpandable_var anExpandable;
    SALOMEDS::AttributeOpened_var     anOpened;
    SALOMEDS::AttributePixMap_var     aPixmap;
    SALOMEDS::AttributeTextColor_var  aTextColor;
    SALOMEDS::AttributeTextHighlightColor_var  aTextHighlightColor;

    if (SC->FindAttribute(anAttr, "AttributeIOR")) {
      anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
      aString = anIOR->Value();
      ior = aString.in();
    }

    bool caseIAPP = false;

    // create data-tree item, corresponding to component (and set name if component has it)
    if ( dataType.compare("Interface Applicative") == 0 ) {
      caseIAPP = true;
      if ( ShowIAPP.compare("true") == 0 ) {
	if (SC->FindAttribute(anAttr, "AttributeName")) {
          aName = SALOMEDS::AttributeName::_narrow(anAttr);
	  aString = aName->Value();
	  Item = AddItem (myListView, aString.in(), SCEntry.latin1(), ior, 1, "");
	  myListViewMap[ SCEntry ].append( Item );
	} 
        else {
	  Item = AddItem (myListView, dataType, SCEntry, ior, 1, "");
	  myListViewMap[ SCEntry ].append( Item );
	}

      }
    } 
    else {
      caseIAPP = false;
      if (SC->FindAttribute(anAttr, "AttributeName")) {
        aName = SALOMEDS::AttributeName::_narrow(anAttr);
	aString = aName->Value();
	Item = AddItem (myListView, aString.in(), SCEntry, ior, 1, "");
	myListViewMap[ SCEntry ].append( Item );
      } 
      else {
	/*In according to CAF - any label (even if it is empty) exist during to whole session.
	  So, if label haven't an "AttributeName" it is means that the label is empty.
	  Therefore follow action couses bug - 
	Item = AddItem (myListView, dataType,       SC->GetID(), ior, 1, "");
	*/
      }
    }
    // add other attributes 
    if (Item) {
      // Selectable
      if ( SC->FindAttribute(anAttr, "AttributeSelectable") ) {
        aSelectable = SALOMEDS::AttributeSelectable::_narrow(anAttr);
        Item->setSelectable(aSelectable->IsSelectable());
      }
      // Expandable
      if ( SC->FindAttribute(anAttr, "AttributeExpandable") ) {
        anExpandable = SALOMEDS::AttributeExpandable::_narrow(anAttr);
        Item->setExpandable(anExpandable->IsExpandable());
      }
      // Opened
      if ( SC->FindAttribute(anAttr, "AttributeOpened") ) {
        anOpened = SALOMEDS::AttributeOpened::_narrow(anAttr);
	Item->setOpen(anOpened->IsOpened());
      }
      // TextColor
      if ( SC->FindAttribute(anAttr, "AttributeTextColor") ) {
	aTextColor = SALOMEDS::AttributeTextColor::_narrow(anAttr);
	QColor aColor((int)(aTextColor->TextColor().R), (int)(aTextColor->TextColor().G), (int)(aTextColor->TextColor().B)) ;
	Item->setTextColor(aColor);
      }
      // TextHighlightColor
      if ( SC->FindAttribute(anAttr, "AttributeTextHighlightColor") ) {
	aTextHighlightColor = SALOMEDS::AttributeTextHighlightColor::_narrow(anAttr);
	QColor aColor((int)(aTextHighlightColor->TextHighlightColor().R), 
		      (int)(aTextHighlightColor->TextHighlightColor().G), 
		      (int)(aTextHighlightColor->TextHighlightColor().B)) ;
	Item->setTextHighlightColor(aColor);
      }
      
      // Pixmap
      if ( SC->FindAttribute(anAttr, "AttributePixMap") ) {
	aPixmap = SALOMEDS::AttributePixMap::_narrow(anAttr);
	if ( aPixmap->HasPixMap() ) {
	  QString msg;
	  QAD_ResourceMgr* resMgr = QAD_Desktop::createResourceManager();
	  if ( resMgr ) {
	    if(MYDEBUG) MESSAGE ( " Component " << aName->Value() );
	    if(MYDEBUG) MESSAGE ( " Icon " << aPixmap->GetPixMap() );
	    if(resMgr->loadResources( QAD_Application::getDesktop()->getComponentName(QString(aName->Value())), msg )) {
	      QPixmap icon ( resMgr->loadPixmap( QAD_Application::getDesktop()->getComponentName(QString(aName->Value())),
						 tr(aPixmap->GetPixMap()) /*tr( "ICON_OBJBROWSER_" + theComponent )*/ ));
	      Item->setPixmap( 0, icon );
	    }
	  }
	}
      }
      /*
	 if ( strcmp(dataType, TypeComponent) == 0 )
	 Item->setOpen(TRUE);
      */
      if ( caseIAPP && ShowIAPP.compare("true") == 0 )
	Update (SC, Item);
      
      if ( !caseIAPP )
	Update (SC, Item);
    }
  }
  myListView->setContentsPos(xc,yc);

  myListView->viewport()->setUpdatesEnabled( true );
  myListView->viewport()->repaint( false );
  /* Updating UseCase Browser ============================================= */
  if ( this->count() > 1 ) 
    UpdateUseCaseBrowser(); 
} 

/*!
  Removes item with all childs from the map - used to optimize UseCase browser update
*/
void removeFromMap( ItemMap& theMap, QAD_ObjectBrowserItem* item )
{
  if ( item ) {
    QAD_ObjectBrowserItem* subItem = ( QAD_ObjectBrowserItem* )( item->firstChild() );
    while ( subItem ) {
      removeFromMap( theMap, subItem );
      subItem = ( QAD_ObjectBrowserItem* )( subItem->nextSibling() );
    }
    if ( theMap.contains( item->getEntry() ) ) {
      theMap[ item->getEntry() ].removeRef( item );
      if ( theMap[ item->getEntry() ].isEmpty() ) {
	theMap.remove( item->getEntry() );
      }
    }
  }
}

/*!
  Updates only UseCase Browser
*/
void QAD_ObjectBrowser::UpdateUseCaseBrowser() {
//  myUseCaseView->clear(); myUseCaseMap.clear();
  if ( myStudy->_is_nil() || this->count()<2 ) {
    return;
  }
  myUseCaseView->blockSignals( true );
//  myUseCaseView->setUpdatesEnabled( false );

  QAD_ObjectBrowserItem* root = ( QAD_ObjectBrowserItem* )myUseCaseView->firstChild();
  SALOMEDS::UseCaseBuilder_var  UCBuilder = myStudy->GetUseCaseBuilder();
  SALOMEDS::SObject_var SOCurrent = UCBuilder->GetCurrentObject();
  CORBA::String_var aString = UCBuilder->GetName();
  QString UCName = aString.in();
  if ( UCName.isEmpty() )
    UCName = QString( tr( "Root" ) );
  // creating root item if is not yet created
  if ( !root ) {
    root = AddItem( myUseCaseView, UCName, "0:2", "", 1, "" );
    myUseCaseMap[ "0:2" ].append( root );
  }
  else
    root->setName( UCName );
  // setting it current if necessary
  root->setCurrent( SOCurrent->_is_nil() || !strcmp( SOCurrent->GetID(), "0:2" ) );
  // updating list view...
  QMap<QString,SALOMEDS::SObject_var> ucList;
  SALOMEDS::UseCaseIterator_var UCIter = UCBuilder->GetUseCaseIterator( SALOMEDS::SObject::_nil() );
  if ( !UCIter->_is_nil() ) {
    for ( ; UCIter->More(); UCIter->Next() ) {
      SALOMEDS::SObject_var UCObject = UCIter->Value();
      aString = UCObject->GetID();
      QString UCEntry = aString.in();
      ucList[ UCEntry ] = UCObject;
    }
  }
  // ... 1) delete removed items
  QAD_ObjectBrowserItem* childItem = ( QAD_ObjectBrowserItem* )root->firstChild();
  while ( childItem ) {
    QAD_ObjectBrowserItem* prevItem = childItem;
    childItem = ( QAD_ObjectBrowserItem* )childItem->nextSibling();
    if ( !ucList.contains( prevItem->getEntry() ) ) {
//      myUseCaseMap[ prevItem->getEntry() ].removeRef( prevItem );
//      if ( myUseCaseMap[ prevItem->getEntry() ].isEmpty() )
//	myUseCaseMap.remove( prevItem->getEntry() );
      removeFromMap( myUseCaseMap, prevItem );
      delete prevItem;
    }
  }
  // ... 2) create/update existing
  QMap<QString,SALOMEDS::SObject_var>::Iterator it;
  for ( it = ucList.begin(); it != ucList.end(); ++it ) {
    UpdateUCItem( it.data(), root );
  }
  myUseCaseView->blockSignals( false );
//  myUseCaseView->setUpdatesEnabled( true );
//  myUseCaseView->update();
//  UCItem->setOpen( true );
}

/*!
  Updates UseCase Browser's item
*/
void QAD_ObjectBrowser::UpdateUCItem( SALOMEDS::SObject_var UCObject, QAD_ObjectBrowserItem* UCItem )
{
  if ( myStudy->_is_nil() || !UCItem ) 
    return;

  /* Creating SObjects item */
  SALOMEDS::SObject_var                      RefSO;
  SALOMEDS::GenericAttribute_var             anAttr;
  SALOMEDS::AttributeName_var                aName;
  SALOMEDS::AttributeIOR_var                 anIOR;
  SALOMEDS::AttributeSelectable_var          aSelectable;
  SALOMEDS::AttributeExpandable_var          anExpandable;
  SALOMEDS::AttributePixMap_var              aPixmap;
  SALOMEDS::AttributeTextColor_var           aTextColor;
  SALOMEDS::AttributeTextHighlightColor_var  aTextHighlightColor;
  QString                                    valueString;
  QString                                    ior = "";
  CORBA::String_var                          aString = UCObject->GetID();
  QString                                    UCEntry = aString.in();

  SALOMEDS::UseCaseBuilder_var               UCBuilder = myStudy->GetUseCaseBuilder();
  SALOMEDS::SObject_var                      SOCurrent = UCBuilder->GetCurrentObject();
  bool                                       isUseCase = UCBuilder->IsUseCase( UCObject );

  QAD_ObjectBrowserItem* UCSubItem = 0;
  if ( myUseCaseMap.contains( UCEntry ) && myUseCaseMap[ UCEntry ].count() > 0 )
    UCSubItem = myUseCaseMap[ UCEntry ].first();

  if ( UCObject->ReferencedObject( RefSO ) && !RefSO->_is_nil() ) {
    aString = RefSO->GetID();
    QString RefSOEntry = aString.in();
    if ( UCObject->FindAttribute( anAttr, "AttributeName" ) || RefSO->FindAttribute( anAttr, "AttributeName" ) ) {
      aName = SALOMEDS::AttributeName::_narrow( anAttr );
      if ( RefSO->FindAttribute( anAttr, "AttributeIOR" ) ) {
	anIOR = SALOMEDS::AttributeIOR::_narrow( anAttr );
	aString = anIOR->Value();
	ior = aString.in();
      }
      valueString = getValueFromObject( RefSO );
      aString = aName->Value();
      if ( !UCSubItem ) {
	UCSubItem = AddItem( UCItem, 
			     QString( " * " ) + aString.in(), 
			     RefSOEntry, 
			     ior, 
			     2, 
			     UCEntry, 
			     valueString );
	myUseCaseMap[ RefSOEntry ].append( UCSubItem );
      }
      else {
	UCSubItem->setName( QString( " * " ) + aString.in() );
	UCSubItem->setEntry( RefSOEntry );
	UCSubItem->setIOR( ior );
	UCSubItem->setReference( UCEntry );
	UCSubItem->setValue( valueString );
      }
      UCSubItem->setCurrent( !SOCurrent->_is_nil() && QString( SOCurrent->GetID() ) == QString( UCEntry ) );
    }
  } 
  else {
    // getting Value
    valueString = getValueFromObject( UCObject );
    // getting IOR
    if ( UCObject->FindAttribute( anAttr, "AttributeIOR" ) )  {
      anIOR = SALOMEDS::AttributeIOR::_narrow( anAttr );
      aString = anIOR->Value();
      ior = aString.in();
    }
    // getting Name and adding new Item
    if ( UCObject->FindAttribute( anAttr, "AttributeName" ) ) {
      aName = SALOMEDS::AttributeName::_narrow( anAttr );
      aString = aName->Value();
      if ( !UCSubItem ) {
	UCSubItem = AddItem( UCItem, aString.in(), UCEntry, ior, isUseCase ? 1 : 0, "", valueString );
	myUseCaseMap[ UCEntry ].append( UCSubItem );
      }
      else {
	UCSubItem->setName( aString.in() );
	UCSubItem->setEntry( UCEntry );
	UCSubItem->setIOR( ior );
	UCSubItem->setReference( "" );
	UCSubItem->setValue( valueString );
      }
    } 
    // adding other attributes 
    if ( UCSubItem ) {
      UCSubItem->setCurrent( !SOCurrent->_is_nil() && QString( SOCurrent->GetID() ) == QString( UCEntry ) );
//  Selectable
//      if ( UCObject->FindAttribute( anAttr, "AttributeSelectable" ) ) {
//	aSelectable = SALOMEDS::AttributeSelectable::_narrow( anAttr );
//	UCSubItem->setSelectable( aSelectable->IsSelectable() );
//      }
// Expandable
//      if ( UCObject->FindAttribute( anAttr, "AttributeExpandable" ) ) {
//	anExpandable = SALOMEDS::AttributeExpandable::_narrow( anAttr );
//	UCSubItem->setExpandable( anExpandable->IsExpandable() );
//      }
// TextColor
      if ( UCObject->FindAttribute(anAttr, "AttributeTextColor" ) ) {
	aTextColor = SALOMEDS::AttributeTextColor::_narrow( anAttr );
	QColor aColor( (int)(aTextColor->TextColor().R), 
		       (int)(aTextColor->TextColor().G), 
		       (int)(aTextColor->TextColor().B) );
	UCSubItem->setTextColor( aColor );
      }
      // TextHighlightColor
      if ( UCObject->FindAttribute( anAttr, "AttributeTextHighlightColor" ) ) {
	aTextHighlightColor = SALOMEDS::AttributeTextHighlightColor::_narrow( anAttr );
	QColor aColor( (int)(aTextHighlightColor->TextHighlightColor().R), 
		       (int)(aTextHighlightColor->TextHighlightColor().G), 
		       (int)(aTextHighlightColor->TextHighlightColor().B) );
	UCSubItem->setTextHighlightColor( aColor );
      }
      // Pixmap
      if ( UCObject->FindAttribute( anAttr, "AttributePixMap" ) ) {
	aPixmap = SALOMEDS::AttributePixMap::_narrow(anAttr);
	if ( aPixmap->HasPixMap() ) {
	  SALOMEDS::SComponent_var father = UCObject->GetFatherComponent();
	  if ( father->FindAttribute( anAttr, "AttributeName" ) ) {
	    SALOMEDS::AttributeName_var aFatherName;
	    aFatherName = SALOMEDS::AttributeName::_narrow( anAttr );
	    QAD_ResourceMgr* resMgr = QAD_Desktop::getResourceManager();
	    if ( resMgr ) {
	      QString msg;
	      if( resMgr->loadResources( QAD_Application::getDesktop()->getComponentName(QString( aFatherName->Value() )), msg ) ) {
		QPixmap icon ( resMgr->loadPixmap( QAD_Application::getDesktop()->getComponentName(QString( aFatherName->Value() )),
						   tr( aPixmap->GetPixMap() ) /*tr( "ICON_OBJBROWSER_" + theComponent )*/ ) );
		UCSubItem->setPixmap( 0, icon );
	      }
	    }
	  }
	}
      }
    } 
  }
  if ( UCSubItem ) {
    // updating children ...
    QMap<QString,SALOMEDS::SObject_var> ucList;
    SALOMEDS::UseCaseIterator_var UCIter = UCBuilder->GetUseCaseIterator( UCObject );
    if ( !UCIter->_is_nil() ) {
      for ( ; UCIter->More(); UCIter->Next() ) {
	SALOMEDS::SObject_var UCSubObject = UCIter->Value();
	aString = UCSubObject->GetID();
	QString UCSubEntry = aString.in();
	ucList[ UCSubEntry ] = UCSubObject;
      }
    }
    // ... 1) delete removed items
    QAD_ObjectBrowserItem* childItem = ( QAD_ObjectBrowserItem* )UCSubItem->firstChild();
    while ( childItem ) {
      QAD_ObjectBrowserItem* prevItem = childItem;
      childItem = ( QAD_ObjectBrowserItem* )childItem->nextSibling();
      if ( !ucList.contains( prevItem->getEntry() ) ) {
	//      myUseCaseMap[ prevItem->getEntry() ].removeRef( prevItem );
	//      if ( myUseCaseMap[ prevItem->getEntry() ].isEmpty() )
	//	myUseCaseMap.remove( prevItem->getEntry() );
	removeFromMap( myUseCaseMap, prevItem );
	delete prevItem;
      }
    }
    // ... 2) create/update existing
    QMap<QString,SALOMEDS::SObject_var>::Iterator it;
    for ( it = ucList.begin(); it != ucList.end(); ++it ) {
      UpdateUCItem( it.data(), UCSubItem );
    }
  }
}

/*!
  Unhighlights all items
*/
void QAD_ObjectBrowser::unHighlightAll()
{
  myListView->clearSelection();
  myUseCaseView->clearSelection();  
}

/*!
  Highlights items which refer to object
*/
void QAD_ObjectBrowser::highlight( const Handle(SALOME_InteractiveObject)& IObject, bool highlight )
{
  QList<QAD_ObjectBrowserItem> List;
  if (getItems(IObject, List)) {
    for (int i = 0; i < List.count(); i++) {
      List.at(i)->setSelected( highlight );
      List.at(i)->repaint();
    } 
  }
}

/*!
  Gets items from Object Browser which refer to object 
*/
bool QAD_ObjectBrowser::getItems( const Handle(SALOME_InteractiveObject)& IObject, 
                                  QList<QAD_ObjectBrowserItem>& List)
{
  List.clear();
  if ( myListViewMap.contains( IObject->getEntry() ) ) {
    ItemList ilist = myListViewMap[ IObject->getEntry() ];
    for ( int i = 0; i < ilist.count(); i++ )
      List.append( ilist.at(i) );
  }
  if ( myUseCaseMap.contains( IObject->getEntry() ) ) {
    ItemList ilist = myUseCaseMap[ IObject->getEntry() ];
    for ( int i = 0; i < ilist.count(); i++ )
      List.append( ilist.at(i) );
  }
  return (List.count() > 0);
}

/*!
  Gets items from Object Browser which refer to object 
*/
bool QAD_ObjectBrowser::getItems( SALOMEDS::SObject_ptr SO, 
                                  QList<QAD_ObjectBrowserItem>& List)
{
  List.clear();
  if ( myListViewMap.contains( SO->GetID() ) ) {
    ItemList ilist = myListViewMap[ SO->GetID() ];
    for ( int i = 0; i < ilist.count(); i++ )
      List.append( ilist.at(i) );
  }
  if ( myUseCaseMap.contains( SO->GetID() ) ) {
    ItemList ilist = myUseCaseMap[ SO->GetID() ];
    for ( int i = 0; i < ilist.count(); i++ )
      List.append( ilist.at(i) );
  }
  return (List.count() > 0);
}

/*!
  Gets first item from Object Browser which refers to object (not reference) 
*/
QAD_ObjectBrowserItem* QAD_ObjectBrowser::getItem( const Handle(SALOME_InteractiveObject)& IObject )
{
  if ( myListViewMap.contains( IObject->getEntry() ) ) {
    ItemList ilist = myListViewMap[ IObject->getEntry() ];
    for ( int i = 0; i < ilist.count(); i++ ) {
      QString aRef   = ilist.at(i)->getReference();
      if ( ilist.at(i)->getReference().isEmpty() )
	return ilist.at(i);
    }
  }
  return 0;
}

/*!
  Finds item from Object Browser corresponding to Object, returns 0 if fails
*/
QAD_ObjectBrowserItem* QAD_ObjectBrowser::getItem( SALOMEDS::SObject_ptr SO )
{
  if ( myListViewMap.contains( SO->GetID() ) ) {
    ItemList ilist = myListViewMap[ SO->GetID() ];
    for ( int i = 0; i < ilist.count(); i++ ) {
      QString aRef   = ilist.at(i)->getReference();
      if ( ilist.at(i)->getReference().isEmpty() )
	return ilist.at(i);
    }
  }
  return 0;
}


/*!
  Gets first item from UseCase Browser which refers to object (not reference) 
*/
QAD_ObjectBrowserItem* QAD_ObjectBrowser::getUCItem( const Handle(SALOME_InteractiveObject)& IObject )
{
  if ( myUseCaseMap.contains( IObject->getEntry() ) ) {
    ItemList ilist = myUseCaseMap[ IObject->getEntry() ];
    for ( int i = 0; i < ilist.count(); i++ ) {
      QString aRef   = ilist.at(i)->getReference();
      if ( ilist.at(i)->getReference().isEmpty() )
	return ilist.at(i);
    }
  }
  return 0;
}

/*!
  Finds item from UseCase Browser corresponding to Object, returns 0 if fails
*/
QAD_ObjectBrowserItem* QAD_ObjectBrowser::getUCItem( SALOMEDS::SObject_ptr SO )
{
  if ( myUseCaseMap.contains( SO->GetID() ) ) {
    ItemList ilist = myUseCaseMap[ SO->GetID() ];
    for ( int i = 0; i < ilist.count(); i++ ) {
      QString aRef   = ilist.at(i)->getReference();
      if ( ilist.at(i)->getReference().isEmpty() )
	return ilist.at(i);
    }
  }
  return 0;
}
/*!
  Finds item corresponding to interactive object, returns 0 if fails
*/
void QAD_ObjectBrowser::rename( const Handle(SALOME_InteractiveObject)& IObject, QString newName )
{
  QAD_ObjectBrowserItem* item = getItem( IObject );
  if ( item ) {
    item->setName( newName );
    item->repaint();
  }
  item = getUCItem( IObject );
  if ( item ) {
    item->setName( newName );
    item->repaint();
  }
}

/*!
  Creates and show specific Popup to this according to current selection (Object Browser)
*/
void QAD_ObjectBrowser::showPopupMenu(QListViewItem* theItem)
{
  QAD_Study* myActiveStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection*    Sel = SALOME_Selection::Selection( myActiveStudy->getSelection() );
  QString       ActiveComp = QAD_Application::getDesktop()->getActiveComponent();

  if (theItem != NULL && ( Sel->IObjectCount()>0 || ActiveComp.isEmpty() ) ) {

    // PAL5618: if IO is presented by several items, assure that the selected IO
    // is presented exactly by theItem: difference is in reference
    QAD_ObjectBrowserItem *item = (QAD_ObjectBrowserItem *) theItem;
    QString theEntry = item->getEntry();
    const SALOME_ListIO& SelIO = Sel->StoredIObjects();
    for ( SALOME_ListIteratorOfListIO SelIt( SelIO ); SelIt.More(); SelIt.Next() ) {
      const Handle(SALOME_InteractiveObject)& IO = SelIt.Value();
      if ( strcmp ( IO->getEntry(), theEntry ) == 0 )
        IO->setReference( item->getReference() );
    }
    
    createPopup();

    if ( myPopup ) {
      QAD_Tools::checkPopup( myPopup );
      if ( myPopup->count()>0 ) {
	myPopup->exec( QCursor::pos() );
      }
      destroyPopup();
    }
  }
}

/*!
  Creates and show specific Popup to this according to current selection (UseCase Browser)
*/
void QAD_ObjectBrowser::showUseCasePopupMenu(QListViewItem* theItem)
{
  createPopup();
  if ( myPopup ) {
    QAD_Tools::checkPopup( myPopup );
    if ( myPopup->count() > 0 ) {
      myPopup->exec( QCursor::pos() );
    }
    destroyPopup();
  }
}

/*!
  Called when current selection was changed (Object Browser)
*/
void QAD_ObjectBrowser::onSelectedItem()
{
  if (currentPage()==myListView)
    myUseCaseView->clearSelection();

  QListView* whoIs;
  if ( sender()->inherits("QListView") )
    whoIs = (QListView*)sender();
  else
    whoIs = myListView;
  QAD_Study*   myActiveStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOMEDS::Study_var aStudy = myActiveStudy->getStudyDocument();
  SALOME_Selection*	 Sel = SALOME_Selection::Selection( myActiveStudy->getSelection() );
  QAD_Application* myApp     = QAD_Application::getDesktop()->getActiveApp();

  SALOME_ListIO DeltaPos;
  DeltaPos.Clear();
  SALOME_ListIO DeltaNeg;
  DeltaNeg.Clear();

  typedef QMap<QString, SALOME_InteractiveObject*> SelMap;

  SelMap currMap, allMap;
  for( SALOME_ListIteratorOfListIO It( Sel->StoredIObjects() ); It.More(); It.Next() )
    currMap.insert( It.Value()->getEntry(), It.Value().operator->() );

  QListViewItemIterator it( whoIs );
  for ( ; it.current(); ++it ) {
    QAD_ObjectBrowserItem* item = (QAD_ObjectBrowserItem*)it.current();
    QString theEntry = item->getEntry();
    QString theName  = item->getName();
    QString theRef   = item->getReference();

    if ( !theEntry.isEmpty() ) {
      if ( item->isSelected() ) {
        allMap.insert( theEntry, 0 );

        if ( !currMap.contains( theEntry ) ) {
	  Handle(SALOME_InteractiveObject) newIO;
	  SALOMEDS::SObject_var obj = aStudy->FindObjectID( theEntry.latin1() );
	  if ( !obj->_is_nil() ) {
	    SALOMEDS::SComponent_var comp = obj->GetFatherComponent();
	    if ( !comp->_is_nil() ) {
	      CORBA::String_var datatype(comp->ComponentDataType());
	      newIO = new SALOME_InteractiveObject( theEntry.latin1(),
		  				    datatype.in(),
						    theName.latin1() );
  	      newIO->setReference( theRef.latin1() );
            }
	  } else {
	    newIO = new SALOME_InteractiveObject( theEntry.latin1(),
						  "",
						  theName.latin1() );
	    newIO->setReference( theRef.latin1() );
	  }
          if (!newIO.IsNull()) {
  	    DeltaPos.Append( newIO );
          } 
        }
      } else if ( currMap.contains( theEntry ) )
	DeltaNeg.Append( currMap[theEntry] );
    }
  }

  SALOME_ListIteratorOfListIO ItNeg( DeltaNeg );
  for(;ItNeg.More();ItNeg.Next()) {
    Sel->RemoveIObject( ItNeg.Value() );
  }
  
  SALOME_ListIteratorOfListIO ItPos( DeltaPos );
  for(;ItPos.More();ItPos.Next()) {
    Sel->AddIObject( ItPos.Value() );
  }

  for ( SelMap::iterator it = currMap.begin(); it != currMap.end(); ++it ) 
    if ( !allMap.contains( it.key() ) )
      Sel->RemoveIObject( it.data() );

  if ( myApp )
    myApp->updateActions();
}

/*!
  Called when selection is changed (UseCase Browser)
*/
void QAD_ObjectBrowser::onUseCaseSelectedItem()
{
}

/*!
  Called when item is expanded
*/
void QAD_ObjectBrowser::onExpanded (QListViewItem* item)
{
  QAD_ObjectBrowserItem* aItem = (QAD_ObjectBrowserItem*)item;
  if (!myStudy->_is_nil() && aItem)
  {
    SALOMEDS::SObject_var Object = myStudy->FindObjectID(aItem->getEntry());
    if (!Object->_is_nil())
    {
      SALOMEDS::StudyBuilder_var StudyBuilder = myStudy->NewBuilder();
      SALOMEDS::AttributeOpened_var  anAttr = SALOMEDS::AttributeOpened::_narrow(
                   StudyBuilder->FindOrCreateAttribute(Object, "AttributeOpened") );
      anAttr->SetOpened(true);
    }
  }
}

/*!
  Called when item is collapsed
*/
void QAD_ObjectBrowser::onCollapsed(QListViewItem* item)
{
  QAD_ObjectBrowserItem* aItem = (QAD_ObjectBrowserItem*)item;
  if (!myStudy->_is_nil() && aItem)
  {
    SALOMEDS::SObject_var Object = myStudy->FindObjectID(aItem->getEntry());
    if (!Object->_is_nil())
    {
      SALOMEDS::StudyBuilder_var StudyBuilder = myStudy->NewBuilder();
      SALOMEDS::AttributeOpened_var  anAttr = SALOMEDS::AttributeOpened::_narrow(
                   StudyBuilder->FindOrCreateAttribute(Object, "AttributeOpened") );
      anAttr->SetOpened(false);
    }
  }
}

/*!
  Find and returns value for the object
  Warning: object can have several values attributes, search is done in the following order:
  [integer], [real], [table of integer], [table of real], [comment]
*/
QString QAD_ObjectBrowser::getValueFromObject( SALOMEDS::SObject_ptr SO )
{
  if ( SO->_is_nil() )
    return QString::null;
  SALOMEDS::SComponent_var isComponent = SALOMEDS::SComponent::_narrow( SO );
  if ( !isComponent->_is_nil() )
    return QString::null; // no values to show for components

  SALOMEDS::GenericAttribute_var        anAttr;
  SALOMEDS::AttributeInteger_var        anInt;
  SALOMEDS::AttributeReal_var           aReal;
  SALOMEDS::AttributeTableOfInteger_var aTableInt;
  SALOMEDS::AttributeTableOfReal_var    aTableReal;
  SALOMEDS::AttributeComment_var        aComment;
  CORBA::String_var aString;

  // Integer
  if ( SO->FindAttribute( anAttr, "AttributeInteger" ) ) {
    anInt = SALOMEDS::AttributeInteger::_narrow( anAttr );
    QString val = QString::number( anInt->Value() );
    return val;
  }
  // Real
  if ( SO->FindAttribute( anAttr, "AttributeReal" ) ) {
    aReal = SALOMEDS::AttributeReal::_narrow( anAttr );
    QString val = QString::number( aReal->Value() );
    return val;
  } 
  // Table of integer
  if ( SO->FindAttribute( anAttr, "AttributeTableOfInteger" ) ) {
    aTableInt = SALOMEDS::AttributeTableOfInteger::_narrow( anAttr );
    aString = aTableInt->GetTitle();
    QString tlt( aString.in() );
    if ( !tlt.isEmpty() )
      tlt += " ";
    int nbRows  = aTableInt->GetNbRows() ; 
    int nbCols  = aTableInt->GetNbColumns();
    QString val = tlt + QString( "[" ) + QString::number( nbRows ) + QString( "," ) + QString::number( nbCols ) + QString( "]" );
    return val;
  }
  // Table of real
  if ( SO->FindAttribute( anAttr, "AttributeTableOfReal" ) ) {
    aTableReal = SALOMEDS::AttributeTableOfReal::_narrow( anAttr );
    aString = aTableReal->GetTitle();
    QString tlt( aString.in() );
    if ( !tlt.isEmpty() )
      tlt += " ";
    int nbRows  = aTableReal->GetNbRows() ; 
    int nbCols  = aTableReal->GetNbColumns();
    QString val = tlt + QString( "[" ) + QString::number( nbRows ) + QString( "," ) + QString::number( nbCols ) + QString( "]" );
    return val;
  }
  // Comment
  if ( SO->FindAttribute(anAttr, "AttributeComment") ) {
    aComment = SALOMEDS::AttributeComment::_narrow( anAttr );
    aString = aComment->Value();
    QString val = QString( aString.in() );
    return val;
  }
  return QString::null;
}

/*!
  Shows/hides IAPP sub-tree in ObjectBrowser
  Current implementation just udpates browser
*/
void QAD_ObjectBrowser::setShowIAPP( bool show )
{
  Update();
}

/*!
  Shows/hides info columns: Entry, IOR, Reference
*/
void QAD_ObjectBrowser::setShowInfoColumns( bool show )
{
  bool shown = myListView->header()->isResizeEnabled( 2 );
  
  if ( show != shown ) {
    if ( show ) {
      myListView->header()->setResizeEnabled( true, 2 );
      myListView->header()->setResizeEnabled( true, 3 );
      myListView->header()->setResizeEnabled( true, 4 );
      myListView->setColumnWidthMode( 2, QListView::Maximum );
      myListView->setColumnWidthMode( 3, QListView::Maximum );
      myListView->setColumnWidthMode( 4, QListView::Maximum );
      if ( myListView->columnWidth( 2 ) <= 0 )
	myListView->header()->resizeSection( 2, 50 );
      if ( myListView->columnWidth( 3 ) <= 0 )
	myListView->header()->resizeSection( 3, 50 );
      if ( myListView->columnWidth( 4 ) <= 0 )
	myListView->header()->resizeSection( 4, 50 );
    }
    else {
      myListView->setColumnWidthMode( 2, QListView::Manual );
      myListView->setColumnWidthMode( 3, QListView::Manual );
      myListView->setColumnWidthMode( 4, QListView::Manual );
      myListView->header()->resizeSection( 2, 0 );
      myListView->header()->resizeSection( 3, 0 );
      myListView->header()->resizeSection( 4, 0 );
      myListView->header()->setResizeEnabled( false, 2 );
      myListView->header()->setResizeEnabled( false, 3 );
      myListView->header()->setResizeEnabled( false, 4 );
    }
  }
}

/*!
  Shows/hides Value column
*/
void QAD_ObjectBrowser::setShowValueColumn( bool show )
{
  bool shown = myListView->header()->isResizeEnabled( 1 );
  
  if ( show != shown ) {
    if ( show ) {
      myListView->header()->setResizeEnabled( true, 1 );
      myListView->setColumnWidthMode( 1, QListView::Maximum );
      if ( myListView->columnWidth( 1 ) <= 0 )
	myListView->header()->resizeSection( 1, 50 );
    }
    else {
      myListView->setColumnWidthMode( 1, QListView::Manual );
      myListView->header()->resizeSection( 1, 0 );
      myListView->header()->setResizeEnabled( false, 1 );
    }
  }
}

/*!
  Sets CHRONO sorting enabled/disabled
  Note: when CHRONO sorting is enabled items are sorted by Entry value and column headers 
        become unclickable to prohibit another sort type;
        when CHRONO sorting is disabled column headers become clickable - this allows
	to sort data by any column
*/
void QAD_ObjectBrowser::setEnableChronoSort( bool enable )
{
  if ( enable ) {
    // if CHRONO sorting is enabled set clickable header disabled ...
    myListView->header()->setClickEnabled( false );
    // ... and then sort by column 2 - OCAF doc entry
    myListView->setSorting( 2, TRUE ) ;
  }
  else {
    // if CHRONO sorting is enabled set clickable header enabled ...
    myListView->header()->setClickEnabled( true );
    // and clear sorting by column 2
    myListView->setSorting( 10, TRUE );
  }
//  myListView->sort( ) ;  
}

/*!
  Slot, called to edit value
*/
void QAD_ObjectBrowser::onEditAttribute()
{
  QAD_Study* myActiveStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection*    Sel = SALOME_Selection::Selection( myActiveStudy->getSelection() );
  QString       ActiveComp = QAD_Application::getDesktop()->getActiveComponent();
  if ( myStudy->_is_nil() )
    return;

  if ( Sel->IObjectCount() == 1 ) {
    SALOMEDS::SObject_var SO = myStudy->FindObjectID( Sel->firstIObject()->getEntry() );
    SALOMEDS::SComponent_var isComponent = SALOMEDS::SComponent::_narrow( SO );
    if ( !SO->_is_nil() && isComponent->_is_nil() ) { // no values to show for components
      SALOMEDS::GenericAttribute_var        anAttr;
      SALOMEDS::AttributeInteger_var        anInt;
      SALOMEDS::AttributeReal_var           aReal;
      SALOMEDS::AttributeTableOfInteger_var aTableInt;
      SALOMEDS::AttributeTableOfReal_var    aTableReal;
      SALOMEDS::AttributeComment_var        aComment;
      bool bOk = false;
	
      // if Integer
      if ( SO->FindAttribute( anAttr, "AttributeInteger" ) ) {
	anInt = SALOMEDS::AttributeInteger::_narrow( anAttr );
	int val = SALOMEGUI_SetValueDlg::getInteger( tr( "SET_VALUE_INT_TLT" ),
						     tr( "SET_VALUE_INT_LBL" ),
						     anInt->Value(),
						     &bOk,
						     QAD_Application::getDesktop() );
	if ( bOk ) {
	  anInt->SetValue( val );
	}
      }
      // if Real
      else 
      if ( SO->FindAttribute( anAttr, "AttributeReal" ) ) {
	aReal = SALOMEDS::AttributeReal::_narrow( anAttr );
	double val = SALOMEGUI_SetValueDlg::getDouble( tr( "SET_VALUE_REAL_TLT" ),
						       tr( "SET_VALUE_REAL_LBL" ),
						       aReal->Value(),
						       &bOk,
						       QAD_Application::getDesktop() );
	if ( bOk ) {
	  aReal->SetValue( val );
	}
      }
      // if Table of integer
      else
      if ( SO->FindAttribute( anAttr, "AttributeTableOfInteger" ) ) {
	SALOMEGUI_TableDlg* dlg = new SALOMEGUI_TableDlg( QAD_Application::getDesktop(),
							  SO, 
							  true, 
							  SALOMEGUI_TableDlg::ttInt );
	bOk = ( dlg->exec() == QDialog::Accepted );
      }
      // if Table of real
      else
      if ( SO->FindAttribute( anAttr, "AttributeTableOfReal" ) ) {
	SALOMEGUI_TableDlg* dlg = new SALOMEGUI_TableDlg( QAD_Application::getDesktop(),
							  SO, 
							  true, 
							  SALOMEGUI_TableDlg::ttReal );
	bOk = ( dlg->exec() == QDialog::Accepted );
      }
      // if Comment
      else
      if ( SO->FindAttribute(anAttr, "AttributeComment") ) {
	aComment = SALOMEDS::AttributeComment::_narrow( anAttr );
	QString val = SALOMEGUI_SetValueDlg::getString( tr( "SET_VALUE_COMMENT_TLT" ),
						        tr( "SET_VALUE_COMMENT_LBL" ),
						        aComment->Value(),
						        &bOk,
						        QAD_Application::getDesktop() );
	if ( bOk ) {
	  aComment->SetValue( val );
	}
      }
      if ( bOk ) {
	Update();
      }
    }
  }
}

/*!
  Returns true if object has editable attrubute : 
  integer, real, table or comment
*/
bool QAD_ObjectBrowser::hasEditableAttribute( const Handle(SALOME_InteractiveObject)& object )
{
  if ( !myStudy->_is_nil() ) {
    SALOMEDS::SObject_var obj  = myStudy->FindObjectID( object->getEntry() );
    SALOMEDS::SComponent_var comp = myStudy->FindComponentID( object->getEntry() );
    if ( !comp->_is_nil() || obj->_is_nil() )
      return false;
    QString val = getValueFromObject( obj );
    return ( !val.isNull() );
  }
  return false;
}

/*!
  Returns true if object has table attrubute ( integer and/or real )
*/
bool QAD_ObjectBrowser::hasTable( const Handle(SALOME_InteractiveObject)& object )
{
  SALOMEDS::GenericAttribute_var anAttr;
  if ( !myStudy->_is_nil() ) {
    SALOMEDS::SObject_var obj  = myStudy->FindObjectID( object->getEntry() );
    SALOMEDS::SComponent_var comp = myStudy->FindComponentID( object->getEntry() );
    if ( !comp->_is_nil() || obj->_is_nil() )
      return false;
    if ( obj->FindAttribute( anAttr, "AttributeTableOfInteger" ) )
      return true;
    if ( obj->FindAttribute( anAttr, "AttributeTableOfReal" ) )
      return true;
  }
  return false;
}

/*!
  Slot, called when one of the UseCase Browser menu command is called
*/
void QAD_ObjectBrowser::onUseCasePopupMenu( int action )
{
  if ( myStudy->_is_nil() )
    return;
  
  // Check if the study is locked
  QAD_Desktop* Desktop     = (QAD_Desktop*) QAD_Application::getDesktop();
  QAD_Study* myActiveStudy = Desktop->getActiveStudy();
  SALOMEDS::Study_var aStudy = myActiveStudy->getStudyDocument();
  if ( aStudy->GetProperties()->IsLocked() )
    {
      QAD_MessageBox::warn1 ( (QWidget*)QAD_Application::getDesktop(),
			      QObject::tr("WRN_WARNING"), 
			      QObject::tr("WRN_STUDY_LOCKED"),
			      QObject::tr("BUT_OK") );
      return;
    }

  SALOME_Selection* Sel    = SALOME_Selection::Selection( myActiveStudy->getSelection() );
  SALOMEDS::UseCaseBuilder_var UCBuilder = myStudy->GetUseCaseBuilder();
  SALOMEDS::SObject_var Current = UCBuilder->GetCurrentObject();
  CORBA::String_var aString;

  QList<QListViewItem> ucSelected; 
  ucSelected.setAutoDelete( false );
  getSelectedParents( myUseCaseView, ucSelected );

  if ( action == UC_NEW_EMPTY_ID ) {
    if ( ucSelected.count() == 1 ) {
      if ( isRootItem( ucSelected.at(0) ) )
	UCBuilder->SetRootCurrent();
      else
	UCBuilder->SetCurrentObject( myStudy->FindObjectID( (( QAD_ObjectBrowserItem* )( ucSelected.at(0) ))->getEntry() ) );
      UCBuilder->AddUseCase( tr( "NEW_OBJECT" ) ); 
      myActiveStudy->updateUseCaseBrowser();
    }
  }
  else
  if ( action == UC_SET_CURRENT_ID ) {
    if ( ucSelected.count() > 0 ) {
      if ( isRootItem( ucSelected.at(0) ) )
	UCBuilder->SetRootCurrent();
      else
	UCBuilder->SetCurrentObject( myStudy->FindObjectID( (( QAD_ObjectBrowserItem* )( ucSelected.at(0) ))->getEntry() ) );
    }
    myActiveStudy->updateUseCaseBrowser( );
  }
  else
  if ( action == UC_RENAME_ID ) {
    if ( ucSelected.count() == 1 ) {
      QAD_ObjectBrowserItem* useCaseItem = ( QAD_ObjectBrowserItem* )( ucSelected.at( 0 ) );
      aString = UCBuilder->GetName();
      if ( isRootItem( useCaseItem ) ) {
	QString name = SALOMEGUI_NameDlg::getName( QAD_Application::getDesktop(), aString.in() );
	if ( !name.isEmpty() ) {
	  UCBuilder->SetName( name.latin1() );
	  myActiveStudy->updateUseCaseBrowser( );
	}
      }
      else/* if ( UCBuilder->IsUseCase( myStudy->FindObjectID( (( QAD_ObjectBrowserItem* )( ucSelected.at(0) ))->getEntry() ) ) )*/ {
	QString name = SALOMEGUI_NameDlg::getName( QAD_Application::getDesktop(), aString.in() );
	if ( !name.isEmpty() ) {
	  myActiveStudy->renameIObject( Sel->firstIObject(), name );
	}
      }
    }
  }
  else 
  if ( action == UC_APPEND_ID ) {
    if ( Sel->IObjectCount() > 0 ) {
      SALOME_ListIO SelIO; SelIO = Sel->StoredIObjects();
      SALOME_ListIteratorOfListIO SelIt( SelIO );
      for( ; SelIt.More(); SelIt.Next() ) {
	SALOMEDS::SObject_var selSO = myStudy->FindObjectID( SelIt.Value()->getEntry() );
	if ( !selSO->_is_nil() && strcmp( SelIt.Value()->getEntry(), ( "0:2" ) ) != 0 ) {
	  UCBuilder->Remove( selSO );  // first remove object for sure ...
	  UCBuilder->Append( selSO );  // ... and now re-append it
	}
      }
      myActiveStudy->updateUseCaseBrowser( true );
    }
  }
  else 
  if ( action == UC_REMOVE_ID ) {
    ucSelected.clear();
    getSelectedParents( myUseCaseView, ucSelected, true );
    for ( int i = 0; i < ucSelected.count(); i++ ) {
      QAD_ObjectBrowserItem* item = ( QAD_ObjectBrowserItem* )ucSelected.at( i );
      if ( !isRootItem( item ) ) {
	// SObject selected
	SALOMEDS::SObject_var SO = myStudy->FindObjectID( item->getEntry() );
	if ( !SO->_is_nil() ) {
	  UCBuilder->Remove( SO );
	}
      }
      else {
      }
    }
    Sel->ClearIObjects();
    myActiveStudy->updateUseCaseBrowser( true );
  }
  else 
  if ( action == UC_CLEAR_ID ) {
    if ( myUseCaseView->childCount() > 0 && myUseCaseView->firstChild()->childCount() > 0) {
      QAD_ObjectBrowserItem* child = ( QAD_ObjectBrowserItem* )myUseCaseView->firstChild()->firstChild();
      while ( child ) {
	UCBuilder->Remove( myStudy->FindObjectID( child->getEntry().latin1() ) );
	child = ( QAD_ObjectBrowserItem* )( child->nextSibling() );
      }
      Sel->ClearIObjects();
      myActiveStudy->updateUseCaseBrowser();
    }
  }
}
/*!
  Use Case browser buttons slot
*/
void QAD_ObjectBrowser::onUseCaseBtn()
{
  // Check if the study is locked
  QAD_Desktop* Desktop     = (QAD_Desktop*) QAD_Application::getDesktop();
  QAD_Study* myActiveStudy = Desktop->getActiveStudy();
  SALOMEDS::Study_var aStudy = myActiveStudy->getStudyDocument();
  if ( aStudy->GetProperties()->IsLocked() )
    {
      QAD_MessageBox::warn1 ( (QWidget*)QAD_Application::getDesktop(),
			      QObject::tr("WRN_WARNING"), 
			      QObject::tr("WRN_STUDY_LOCKED"),
			      QObject::tr("BUT_OK") );
      return;
    }
  
  if ( sender() == myNewBtn ) {
    SALOMEDS::UseCaseBuilder_var UCBuilder = myStudy->GetUseCaseBuilder();
    UCBuilder->AddUseCase( tr( "NEW_OBJECT" ) ); 
    myActiveStudy->updateUseCaseBrowser();
  }
  if ( sender() == myAddBtn )
    onUseCasePopupMenu( UC_APPEND_ID );
  if ( sender() == myDelBtn )
    onUseCasePopupMenu( UC_REMOVE_ID );
  if ( sender() == myClearBtn )
    onUseCasePopupMenu( UC_CLEAR_ID );
  if ( sender() == myCurrentBtn )
    onUseCasePopupMenu( UC_SET_CURRENT_ID );
}

/* 
   Show/remove UseCase Browser 
*/

void QAD_ObjectBrowser::showUseCaseBrowser ( bool show )
{
  bool shown = (this->count() > 1);
  
  if (show != shown)
    {
      if (show)
	{
	  this->addTab( myVBox, tr( "TLT_USECASE_BROWSER" ) );
	  UpdateUseCaseBrowser();
	  unHighlightAll();
	}
      else
	this->removePage(myVBox);
    }
}

/*!
  Switch between auto resizing of columns and manual mode
*/
void QAD_ObjectBrowser::autoSizeColumns( bool autosize )
{
  if (autosize)
    {
      for (int i = 0; i < myListView->header()->count(); i++ )
	if (myListView->header()->isResizeEnabled(i))
	  myListView->setColumnWidthMode(i, QListView::Maximum);
      
    }
  else
    {
      for (int i = 0; i < myListView->header()->count(); i++ )
	myListView->setColumnWidthMode(i, QListView::Manual); 
    }
}
