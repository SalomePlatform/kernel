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
//  File   : QAD_Desktop.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

/*!
  \class QAD_Desktop QAD_Desktop.h
  \brief Main desktop of QAD-based application.
*/
# include "Utils_ORB_INIT.hxx"
# include "Utils_SINGLETON.hxx"

#define	 INCLUDE_MENUITEM_DEF
#define  DEFAULT_BROWSER "mozilla"


#include "QAD.h"
#include "QAD_Help.h"
#include "QAD_Tools.h"
#include "QAD_Desktop.h"
#include "QAD_LeftFrame.h"
#include "QAD_RightFrame.h"
#include "QAD_PyEditor.h"
#include "QAD_Operation.h"
#include "QAD_XmlHandler.h"
#include "QAD_MessageBox.h"
#include "QAD_Application.h"
#include "QAD_Settings.h"
#include "QAD_Config.h"
#include "QAD_ObjectBrowser.h"
#include "QAD_Resource.h"
#include "QAD_FileDlg.h"
//NRI #include "QAD_HelpWindow.h"
#include "QAD_DirListDlg.h"
#include "QAD_WaitCursor.h"
#include "SALOMEGUI.h"
#include "SALOMEGUI_OpenWith.h"
#include "SALOMEGUI_StudyPropertiesDlg.h"
#include "SALOMEGUI_TrihedronSizeDlg.h"
#include "SALOMEGUI_ExternalBrowserDlg.h"
#include "SALOMEGUI_LoadStudiesDlg.h"
//#include "SALOME_Selection.h"
#include "SALOME_InteractiveObject.hxx"
#include "SALOME_ListIteratorOfListIO.hxx"
#include "SALOMEGUI_AboutDlg.h"
#include "SALOMEGUI_ViewChoiceDlg.h"
#include "SALOMEGUI_SetValueDlg.h"
#include "utilities.h"

#include "SALOMEGUI_CloseDlg.h"
#include "SALOMEGUI_ActivateComponentDlg.h"
#include "SALOMEGUI_QtCatchCorbaException.hxx"

#include "SALOME_Event.hxx"

// QT Includes
#include <qlabel.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <qcolordialog.h>
#include <qradiobutton.h>
#include <qapplication.h>
#include <qvbuttongroup.h>
#include <qpixmap.h>
#include <qmessagebox.h>
#include <qwidget.h>
#include <stdio.h>
#include <qpopupmenu.h>
#include <qlistview.h>
#include <qspinbox.h>
#include <qhbox.h>
#include <qiconset.h>
#include <qfontdialog.h>
#include <qlineedit.h>
#include <qdatetime.h>
#include <qthread.h>

#include <qstringlist.h>

#if QT_VERSION > 300
  #include <qlistbox.h>
  #include <qregexp.h>
#endif

// Open CASCADE Includes
#include <OSD_SharedLibrary.hxx>
#include <OSD_LoadMode.hxx>
#include <OSD_Function.hxx>
#include <TCollection_AsciiString.hxx>
using namespace std;

static const char* SEPARATOR    = ":";

extern "C"
{
# include <string.h>
}

QAD_ResourceMgr* QAD_Desktop::resourceMgr = 0;
QPalette*	 QAD_Desktop::palette = 0;

static QString createString( int theItemId, int thePosId ) 
{
  QString aRetString = QString("item-id=");
  QString aString;
  QString aItemId = aString.setNum(theItemId);
  QString aPosId = aString.setNum(thePosId);
  aRetString = aRetString + '"'; aRetString = aRetString + aItemId; aRetString = aRetString + '"'; 
  aRetString = aRetString + " pos-id="; aRetString = aRetString + '"';
  aRetString = aRetString + aPosId; 
  aRetString = aRetString + '"'; aRetString = aRetString + ">";
  return aRetString;
}

/*!
    Creates the resource manager [ static ]
*/
QAD_ResourceMgr* QAD_Desktop::createResourceManager()
{
  if ( !resourceMgr )
    resourceMgr = new QAD_ResourceMgr;
  return resourceMgr;
}

/*!
    Returns the resource manager [ static ]
*/
QAD_ResourceMgr* QAD_Desktop::getResourceManager()
{
  return resourceMgr;
}


/*!
    Loads the palette from settings [ static ]
*/
QPalette* QAD_Desktop::createPalette()
{
  if ( !palette )
    palette = new QPalette();

  return palette;
}

/*!
    Returns the palette [ static ]
*/
QPalette* QAD_Desktop::getPalette()
{
  return palette;
}

/*!
    Gets window ratio width/heght [ static ]
*/
static double myWindowRatio = 1;
static double getWindowRatio()
{
  return myWindowRatio;
}

/*!
    Sets window ratio width/heght [ static ]
*/
static void setWindowRatio(double aRatio)
{
  myWindowRatio = aRatio;
}

/*!
    Constructor
*/
QAD_Desktop::QAD_Desktop(SALOME_NamingService* name_service) :
QMainWindow(0, 0, WType_TopLevel | WDestructiveClose),
myStdToolBar(0),
myStatusBar(0),
myActiveApp(0),
myActiveStudy(0),
myCntUntitled(0),
//NRImyHelpWindow(0),
myDefaultTitle( tr("DESK_DEFAULTTITLE") ),
myQueryClose( true ),
myAboutToClose( false )
{
  /* Force reading of user config file */
  QAD_CONFIG->readConfigFile();  

  /* menubar and status bar */
  myStatusBar = statusBar();
  myMainMenu = menuBar();
  myActiveComp = "";
  myNameService = name_service;

  /* default background icon */
  QPixmap backgroundicon ( QAD_Desktop::getResourceManager()->loadPixmap( "CLIENT",
									  tr("ICON_DESK_BACKGROUNDICON") ));
//myToolBarAction.setAutoDelete( true );

  /* default icon and title */
  QPixmap icon ( QAD_Desktop::getResourceManager()->loadPixmap( "CLIENT",
								tr("ICON_DESK_DEFAULTICON") ));
  if ( !icon.isNull() ) {
    myDefaultIcon = icon;
    setIcon( myDefaultIcon );
  }
  setCaption( myDefaultTitle );

  /* set size as 1/2 of the screen and center it */
  QWidget* d = QApplication::desktop();
  resize( 2*d->width()/3, 2*d->height()/3 );
  QAD_Tools::centerWidget( this, d );

  /* workspace will manage child frames */
  QHBox* border = new QHBox ( this );
  border->setFrameStyle ( QFrame::Panel | QFrame::Sunken );
  setCentralWidget( border );
  myWorkspace = new QWorkspaceP( border );

  QPalette pal = QAD_Application::getPalette();
  setPalette(pal);
  QColorGroup cgA = pal.active();
  QColorGroup cgI = pal.inactive();
  QColorGroup cgD = pal.disabled();
  cgA.setColor( QColorGroup::Background, QColor(192, 192, 192));
  cgI.setColor( QColorGroup::Background, QColor(192, 192, 192));
  cgD.setColor( QColorGroup::Background, QColor(192, 192, 192));
  pal.setActive  ( cgA );
  pal.setInactive( cgI );
  pal.setDisabled( cgD );
  myWorkspace->setPalette( pal );
  if ( !backgroundicon.isNull() ) {
    MESSAGE("!!!DESKTOP background icon found!!!");
    myWorkspace->setPaletteBackgroundPixmap(backgroundicon);
  }

  /* define standard file commands */
  createActions();

  /* define operator menus for xml */
  myOperatorMenus = new QAD_OperatorMenus(this);
  myXmlHandler = new QAD_XmlHandler();

  /* New catalogue */
  CORBA::Object_var objVarN = myNameService->Resolve("/Kernel/ModulCatalog");
  myCatalogue  = SALOME_ModuleCatalog::ModuleCatalog::_narrow(objVarN);

  SALOME_ModuleCatalog::ListOfIAPP_Affich_var list_composants =
    myCatalogue->GetComponentIconeList();


  QToolBar* tbComponent = new QToolBar( tr("MEN_DESK_COMPONENTTOOLBAR"), this );
  tbComponent->setCloseMode( QDockWindow::Undocked );
  addToolBar(tbComponent, tr("MEN_DESK_COMPONENTTOOLBAR"), Bottom, TRUE );
  setDockEnabled( tbComponent, DockLeft,  false );
  setDockEnabled( tbComponent, DockRight, false );

  myCombo = new QComboBox( FALSE, tbComponent, "comboBox" );
  myCombo->setFocusPolicy( NoFocus );

  tbComponent->addSeparator();

  // PG : add ResourceManager to get function findFile !!
  QAD_ResourceMgr* resMgr = QAD_Desktop::createResourceManager();

  for (unsigned int ind = 0; ind < list_composants->length();ind++) {
    QString resDir;

    /* find component icon */
    QString iconfile = CORBA::string_dup(list_composants[ind].moduleicone) ;
    QString modulename = CORBA::string_dup(list_composants[ind].modulename) ;
    QString moduleusername = CORBA::string_dup(list_composants[ind].moduleusername) ;

    //    MESSAGE ( " MODULE = " << modulename )
    //    MESSAGE ( " MODULE icon = " << iconfile )
    //    MESSAGE ( " MODULE username = " << moduleusername )

    if ( mapComponentName.contains( moduleusername ) ) {
      QMessageBox::warning( this, tr("WRN_WARNING"), QString( moduleusername + " is already associated with " + mapComponentName[moduleusername] + ".\nPlease, change the component username of " + modulename) , tr ("BUT_OK") );
      continue;
    }
    mapComponentName.insert( moduleusername, modulename );
      
    resDir = resMgr->findFile(iconfile,modulename) ;
    if (resDir)
      {
	MESSAGE ( "resDir" << resDir )
	//resDir = QAD_Tools::addSlash(resDir) ;
	//QPixmap Icone(resDir+iconfile) ;
	QPixmap Icone( QAD_Tools::addSlash( resDir ) + iconfile );
	QToolButton * toolb = 
	  new QToolButton( QIconSet( Icone ), moduleusername, QString::null, this, 
			   SLOT( onButtonActiveComponent () ),tbComponent );
	toolb->setToggleButton( true );
	myComponentButton.append(toolb);
      }
    else
      {
	QString errMsg =  tr("INF_ICON_RESOURCES").arg(iconfile).arg(modulename) +
	  tr("INF_RESOURCES");
	//QMessageBox::warning( this, tr("WRN_WARNING"), errMsg, tr ("BUT_OK") );
      }
    if ( ( resDir || moduleusername == "Salome" ) && !modulename.isEmpty() ) // VSR: Force "Salome" component to appear in the combo box
      myCombo->insertItem( moduleusername );
  }

  myCombo->adjustSize();
  connect( myCombo, SIGNAL(activated(const QString&)),
	   this, SLOT( onComboActiveComponent(const QString&) ) );

  /* new LifeCycleCORBA client, for Engines */
  myEnginesLifeCycle = new SALOME_LifeCycleCORBA(name_service);
  
  /* VSR 13/01/03 : installing global event filter for the application */
  qApp->installEventFilter( this );

  /* init pseudo random numbers generator */
  QTime cTime = QTime::currentTime();
  int secs = cTime.second() + 1;
  for ( int s = 0; s < secs; s++ ) random();
}

/*!
    Destructor
*/
QAD_Desktop::~QAD_Desktop ()
{
  qApp->removeEventFilter( this );
  myFilePopup.clear();
  myEditPopup.clear();
  myViewPopup.clear();
  myObjBrowserPopup.clear();
  //VRV: T2.5 - add default viewer
  myDefaultViewer.clear();
  //VRV: T2.5 - add default viewer
  myViewerPopup.clear();
  //NRI : SAL2214
  myNewViewPopup.clear();
  //NRI : SAL2214
  myHelpContentsModulePopup.clear();
  myToolsPopup.clear();
  myPrefPopup.clear();
  myStdActions.clear();
  myHelpPopup.clear();
  myToolBarsPopup.clear();
  myToolBarAction.clear();
  myApps.clear();
  delete resourceMgr;
//NRI   if (myHelpWindow)
//     myHelpWindow->close();
  resourceMgr = 0;
  QAD_Application::desktop = 0;
}

const int IdCut       = 1001;
const int IdCopy      = 1002;
const int IdPaste     = 1003;
const int IdSelectAll = 1004;
#ifndef QT_NO_ACCEL
#include <qkeysequence.h>
#define ACCEL_KEY(k) "\t" + QString(QKeySequence( Qt::CTRL | Qt::Key_ ## k ))
#else
#define ACCEL_KEY(k) "\t" + QString("Ctrl+" #k)
#endif
#include <qclipboard.h>
/*!
  Global event filter for qapplication (VSR 13/01/03)
*/
bool QAD_Desktop::eventFilter( QObject* o, QEvent* e )
{
  if (e->type() == QEvent::Close && o == this )
    myAboutToClose = true;
  else if (e->type() == 2000   ) {
    QMessageBox::information (this, tr ( "Help Information" ), tr ( "Can't run choosen browser.\nRunning default browser (Mozilla). "));
    return TRUE;
  }
  else if (e->type() == 2001 ) {
    QMessageBox::critical(this, tr ( "Help Error" ), tr ( "Can't run the default browser.") );
    return TRUE;
  }
  else if ( e->type() == QEvent::ContextMenu ) {
    QContextMenuEvent* ce = (QContextMenuEvent*)e;
    if ( o->inherits("QRenameEdit") ) {
      return TRUE;
    }
    else if ( o->inherits("QLineEdit") ) {
      QLineEdit* le = (QLineEdit*)o;
      if ( le->parentWidget() ) {
	if ( ( le->parentWidget()->inherits("QSpinBox") || 
	       le->parentWidget()->inherits("QSpinWidget") ||
	       le->parentWidget()->inherits("QAD_SpinBoxDbl") ) &&
	     le->isEnabled() ) {
	  QPopupMenu* popup = new QPopupMenu( 0, "qt_edit_menu" );
	  popup->insertItem( tr( "EDIT_CUT_CMD" ) + ACCEL_KEY( X ), IdCut );
	  popup->insertItem( tr( "EDIT_COPY_CMD" ) + ACCEL_KEY( C ), IdCopy );
	  popup->insertItem( tr( "EDIT_PASTE_CMD" ) + ACCEL_KEY( V ), IdPaste );
	  popup->insertSeparator();
#if defined(Q_WS_X11)
	  popup->insertItem( tr( "EDIT_SELECTALL_CMD" ), IdSelectAll );
#else
	  popup->insertItem( tr( "EDIT_SELECTALL_CMD" ) + ACCEL_KEY( A ), IdSelectAll );
#endif
	  bool enableCut = !le->isReadOnly() && le->hasSelectedText();
	  popup->setItemEnabled( IdCut, enableCut );
	  popup->setItemEnabled( IdCopy, le->hasSelectedText() );
	  bool enablePaste = !le->isReadOnly() && !QApplication::clipboard()->text().isEmpty();
	  popup->setItemEnabled( IdPaste, enablePaste );
	  bool allSelected = (le->selectedText() == le->text() );
	  popup->setItemEnabled( IdSelectAll, (bool)(le->text().length()) && !allSelected );

	  QPoint pos = ce->reason() == QContextMenuEvent::Mouse ? ce->globalPos() :
	    le->mapToGlobal( QPoint(ce->pos().x(), 0) ) + QPoint( le->width() / 2, le->height() / 2 );
	  if ( popup ) {
	    int r = popup->exec( pos );
	    switch ( r ) {
	    case IdCut:
	      le->cut();
	      break;
	    case IdCopy:
	      le->copy();
	      break;
	    case IdPaste:
	      le->paste();
	      break;
	    case IdSelectAll:
	      le->selectAll();
	      break;
	    }
	    delete popup;
	  }
	  return TRUE;
	}
      }
    }
  }
  else if ( e->type() == SALOME_EVENT ) { 
    SALOME_Event* aSE = (SALOME_Event*)((QCustomEvent*)e)->data();
    processEvent(aSE);
    ((QCustomEvent*)e)->setData( 0 );
    return TRUE;
  }
  return QMainWindow::eventFilter( o, e );
}

/*!
    Dispatches <theEvent> to the target component GUI
*/
void QAD_Desktop::processEvent( SALOME_Event* theEvent )
{
  if(theEvent){
    theEvent->Execute();
    // Signal the calling thread that the event has been processed
    theEvent->processed();
  }
}

/*!
    Creates and initializes the standard file operations
    such as 'New/Open/Save/SaveAs/Close' and 'Help'.
*/
void QAD_Desktop::createActions()
{

  /* Used for string compare */
  const QString& aTrueQString = "true" ;
  
  /* create 'standard' toolbar */
  if ( !myStdToolBar ) {
	myStdToolBar = new QToolBar ( tr("MEN_DESK_VIEW_STDTOOLBAR"), this );
	myStdToolBar->setCloseMode( QDockWindow::Undocked );
  }

  if ( !myMainMenu->count() ) {
    /* Create main menu bar */
    myMainMenu->insertItem ( tr("MEN_DESK_FILE"),   &myFilePopup, 1 );	/* add popup FILE */
    myMainMenu->insertItem ( tr("MEN_DESK_VIEW"),   &myViewPopup, 2 );	/* add popup VIEW */
    myMainMenu->insertItem ( tr("MEN_DESK_TOOLS"),  &myToolsPopup, 5 );	/* add popup TOOLS */
    myMainMenu->insertItem ( tr("MEN_DESK_PREF"),   &myPrefPopup, 4 );	/* add popup PREF */
    myMainMenu->insertItem ( tr("MEN_DESK_WINDOW"), &myWindowPopup, 6 );	/* add popup WINDOW */
    myMainMenu->insertItem ( tr("MEN_DESK_HELP"),   &myHelpPopup, 7 );	/* add popup HELP */

    /*	Applications will insert their items after 'File' 'Edit' and 'View'
	( 'Edit' will be inserted later )
    */
    myMainMenuPos = 3;
  }
  
  /* insert logo picture to menu bar */
  QHBox* aLogoFrm = new QHBox(this);
  aLogoFrm->setFrameStyle( QFrame::Plain | QFrame::NoFrame );
  QPixmap aLogoPixmap ( QAD_Desktop::getResourceManager()->loadPixmap( "CLIENT",
								       tr("ICON_DESK_LOGO") ));
  QLabel* aLogoLab = new QLabel(aLogoFrm);
  aLogoLab->setPixmap(aLogoPixmap);
  aLogoLab->setAlignment(AlignCenter);
  aLogoLab->setScaledContents(false);
  myMainMenu->insertItem(aLogoFrm);

  if ( myStdActions.isEmpty() ) {
    /*	Define standard actions. They should be inserted
	into the list in order of their IDs.
    */

    /*	'File' actions */
    /* new */
    QAD_ResourceMgr* rmgr = QAD_Desktop::getResourceManager();
    QActionP* fileNewAction = new QActionP ( tr("TOT_DESK_FILE_NEW"),
					   rmgr->loadPixmap( "QAD", tr("ICON_FILE_NEW") ) ,
					   tr("MEN_DESK_FILE_NEW"), CTRL+Key_N, this );
    fileNewAction->setStatusTip ( tr("PRP_DESK_FILE_NEW") );
    fileNewAction->setEnabled ( true );
    QAD_ASSERT ( connect( fileNewAction, SIGNAL( activated() ), this, SLOT( onNewStudy() )));
    fileNewAction->addTo( myStdToolBar );
    fileNewAction->addTo( &myFilePopup );
    myStdActions.insert ( FileNewId, fileNewAction );

    /* open */
    QActionP* fileOpenAction = new QActionP( tr("TOT_DESK_FILE_OPEN"), rmgr->loadPixmap( "QAD", tr("ICON_FILE_OPEN") ),
					   tr("MEN_DESK_FILE_OPEN"), CTRL+Key_O, this );
    fileOpenAction->setStatusTip ( tr("PRP_DESK_FILE_OPEN") );
    fileOpenAction->setEnabled ( true );
    QAD_ASSERT ( connect( fileOpenAction, SIGNAL( activated() ), this, SLOT( onOpenStudy() )));
    fileOpenAction->addTo( myStdToolBar );
    fileOpenAction->addTo( &myFilePopup );
    myStdActions.insert ( FileOpenId, fileOpenAction );

    /* load */
    QActionP* fileLoadAction = new QActionP( tr("TOT_DESK_FILE_LOAD"), rmgr->loadPixmap( "QAD", tr("ICON_FILE_LOAD") ),
					   tr("MEN_DESK_FILE_LOAD"), CTRL+Key_L, this );
    fileLoadAction->setStatusTip ( tr("PRP_DESK_FILE_LOAD") );
    fileLoadAction->setEnabled ( true );
    QAD_ASSERT ( connect( fileLoadAction, SIGNAL( activated() ), this, SLOT( onLoadStudy() )));
    fileLoadAction->addTo( &myFilePopup );
    myStdActions.insert ( FileLoadId, fileLoadAction );

    /* close */
    QActionP* fileCloseAction = new QActionP( "", rmgr->loadPixmap( "QAD", tr("ICON_FILE_CLOSE") ),
					    tr("MEN_DESK_FILE_CLOSE"), CTRL+Key_W, this );
    fileCloseAction->setStatusTip ( tr("PRP_DESK_FILE_CLOSE") );
    QAD_ASSERT ( connect( fileCloseAction, SIGNAL( activated() ), this, SLOT( onCloseStudy() )));
    fileCloseAction->addTo( &myFilePopup );
    myStdActions.insert ( FileCloseId, fileCloseAction );

    /* separator */
    myFilePopup.insertSeparator();

    /* save */
    QActionP* fileSaveAction = new QActionP( tr("TOT_DESK_FILE_SAVE"), rmgr->loadPixmap( "QAD", tr("ICON_FILE_SAVE") ),
					   tr("MEN_DESK_FILE_SAVE"), CTRL+Key_S, this );
    fileSaveAction->setStatusTip ( tr("PRP_DESK_FILE_SAVE") );
    QAD_ASSERT ( connect( fileSaveAction, SIGNAL( activated() ), this, SLOT( onSaveStudy() )));
    fileSaveAction->addTo( myStdToolBar );
    fileSaveAction->addTo( &myFilePopup );
    myStdActions.insert ( FileSaveId, fileSaveAction );

    /* save as */
    QActionP* fileSaveAsAction = new QActionP( "", tr("MEN_DESK_FILE_SAVEAS"), 0, this );
    fileSaveAsAction->setStatusTip ( tr("PRP_DESK_FILE_SAVEAS") );
    QAD_ASSERT ( connect( fileSaveAsAction, SIGNAL( activated() ),
			  this, SLOT( onSaveAsStudy() )));
    fileSaveAsAction->addTo( &myFilePopup );
    myStdActions.insert ( FileSaveAsId, fileSaveAsAction );

    
    /* separator */
    myFilePopup.insertSeparator();

    // Study properties
    QActionP* filePropsAction = new QActionP( "", QPixmap(), tr("MEN_DESK_FILE_PROPERTIES"), 0, this );
    filePropsAction->setStatusTip ( tr("PRP_DESK_FILE_PROPERTIES") );
    filePropsAction->setEnabled(false);
    QAD_ASSERT ( connect( filePropsAction, SIGNAL( activated() ), this, SLOT( onStudyProperties() )));
    filePropsAction->addTo( &myFilePopup );
    myStdActions.insert ( FilePropsId, filePropsAction );


    int id = myFilePopup.insertSeparator();
    /*	keep the position from which an application will insert its items
	to menu 'File' at the time of customization of the desktop */

    myFilePos = myFilePopup.indexOf( id ) + 1;

    /* exit application */
    QActionP* exitAction = new QActionP( "", tr("MEN_DESK_FILE_EXIT"),
				       CTRL+Key_X, this );
    exitAction->setStatusTip ( tr("PRP_DESK_FILE_EXIT") );
    QAD_ASSERT ( connect( exitAction, SIGNAL( activated() ),
			  this, SLOT( onExit() )));
    exitAction->addTo( &myFilePopup );
    myStdActions.insert ( FileExitId, exitAction );
 
    QAD_ASSERT( connect( &myFilePopup, SIGNAL(highlighted( int )), 
			 this, SLOT(onFilePopupStatusText( int )) ));
    

    /* 'Edit' actions : provided by application only */
    myEditPos = 0;

    QAD_ASSERT( connect( &myEditPopup, SIGNAL(highlighted( int )), 
			 this, SLOT(onEditPopupStatusText( int )) ));

    /* 'View' actions */
    /* toolbars popup menu */
    myViewPopup.insertItem( tr("MEN_DESK_VIEW_TOOLBARS"), &myToolBarsPopup );
    QAD_ASSERT( connect ( &myViewPopup, SIGNAL(aboutToShow()),
			  this, SLOT(onToolBarPopupAboutToShow()) ));

    /*	status bar */
    QActionP* viewStatusBarAction = new QActionP( "",
						tr("MEN_DESK_VIEW_STATUSBAR"),
						0, this, 0, true );
    viewStatusBarAction->setStatusTip ( tr("PRP_DESK_VIEW_STATUSBAR") );
    viewStatusBarAction->setOn( true );
    QAD_ASSERT(connect( viewStatusBarAction, SIGNAL(activated()), this, SLOT(onViewStatusBar() )));
    viewStatusBarAction->addTo( &myViewPopup );
    myStdActions.insert( ViewStatusBarId, viewStatusBarAction );

//    myViewPopup.insertItem( tr("MEN_DESK_SELECTION_MODE"), &mySelectionModePopup );

    QActionP* SelectionPointAction = new QActionP( "", tr("MEN_DESK_SELECTION_POINT"), 0, this, 0, true  );
    QAD_ASSERT(connect( SelectionPointAction, SIGNAL(activated()), this, SLOT(onSelectionMode() )));
    SelectionPointAction->addTo( &mySelectionModePopup );
    myStdActions.insert( SelectionPointId, SelectionPointAction );

    QActionP* SelectionEdgeAction = new QActionP( "", tr("MEN_DESK_SELECTION_EDGE"), 0, this, 0, true  );
    QAD_ASSERT(connect( SelectionEdgeAction, SIGNAL(activated()), this, SLOT(onSelectionMode() )));
    SelectionEdgeAction->addTo( &mySelectionModePopup );
    myStdActions.insert( SelectionEdgeId, SelectionEdgeAction );

    QActionP* SelectionCellAction = new QActionP( "", tr("MEN_DESK_SELECTION_CELL"), 0, this, 0, true  );
    QAD_ASSERT(connect( SelectionCellAction, SIGNAL(activated()), this, SLOT(onSelectionMode() )));
    SelectionCellAction->addTo( &mySelectionModePopup );
    myStdActions.insert( SelectionCellId, SelectionCellAction );

    QActionP* SelectionActorAction = new QActionP( "", tr("MEN_DESK_SELECTION_ACTOR"), 0, this, 0, true );
    QAD_ASSERT(connect( SelectionActorAction, SIGNAL(activated()), this, SLOT(onSelectionMode() )));
    SelectionActorAction->addTo( &mySelectionModePopup );
    myStdActions.insert( SelectionActorId, SelectionActorAction );
    SelectionActorAction->setOn(true);

    myViewPos = myViewPopup.count();

    QAD_ASSERT( connect( &myViewPopup, SIGNAL(highlighted( int )), 
			 this, SLOT(onViewPopupStatusText( int )) ));

    /* Parse xml file */
    QAD_ResourceMgr* resMgr = QAD_Desktop::createResourceManager();
    if ( resMgr ) {
      QString msg;
      if(!resMgr->loadResources( "ToolsGUI", msg ))
	{
	  //NRI	    QCString errMsg;
	  //	    errMsg.sprintf( "Do not load all resources for module ToolsGUI.\n" );
	  QMessageBox::warning( this, tr("WRN_WARNING"), msg, tr ("BUT_OK") );
	}
    }
    
    myOperatorMenus = new QAD_OperatorMenus(this);
    myXmlHandler = new QAD_XmlHandler();
    ASSERT(myXmlHandler) ;
    myXmlHandler->setMainWindow(this);
    if ( myXmlHandler->setComponent( resMgr->resources( "ToolsGUI" ) ) ) {
      QString language = resMgr->language( "ToolsGUI" );
      QString ToolsXml = QString( "Tools_" ) + language + QString( ".xml" );

      //ToolsXml = resMgr->resources("ToolsGUI") ;
      //ToolsXml = QAD_Tools::addSlash(ToolsXml) ;
      //ToolsXml = ToolsXml + "Tools_" + language + ".xml" ;
      ToolsXml = QAD_Tools::addSlash( resMgr->findFile( ToolsXml, "ToolsGUI" ) ) + ToolsXml;

      QFile file( QAD_Tools::unix2win( ToolsXml ) );
      if ( file.exists() && file.open( IO_ReadOnly ) )  {
	file.close();
	QXmlInputSource source( file );
	QXmlSimpleReader reader;
	reader.setContentHandler( myXmlHandler );
	reader.setErrorHandler( myXmlHandler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
	  QMessageBox::critical( 0,
				 tr( "INF_PARSE_ERROR" ),
				 tr( myXmlHandler->errorProtocol() ) );
	} else {
	  myMenusList=myXmlHandler->myMenusList;
	  myActiveMenus=myMenusList.at(0);
	  myOperatorMenus->showMenuBar(0);
	  myActiveMenus->showAllToolBars();
	}
      }
    }
    //  }

    if ( myToolsPopup.count() == 0 ) {
      myMainMenu->removeItem(5);
    }

    /* 'Pref' actions  */
    /* Viewer BackgroundColor */
    myPrefPopup.insertItem( tr("MEN_DESK_PREF_VIEWER"), &myViewerPopup );
    
    QActionP* viewerOCCAction = new QActionP( "", tr("MEN_DESK_PREF_VIEWER_OCC"), 0, this );
    QAD_ASSERT(connect( viewerOCCAction, SIGNAL(activated()), this, SLOT(onViewerOCC() )));
    viewerOCCAction->addTo( &myViewerPopup );
    myStdActions.insert( PrefViewerOCCId, viewerOCCAction );

    QActionP* viewerVTKAction = new QActionP( "", tr("MEN_DESK_PREF_VIEWER_VTK"), 0, this );
    QAD_ASSERT(connect( viewerVTKAction, SIGNAL(activated()), this, SLOT(onViewerVTK() )));
    viewerVTKAction->addTo( &myViewerPopup );
    myStdActions.insert( PrefViewerVTKId, viewerVTKAction );

    QActionP* graphSupervisorAction = new QActionP( "", tr("MEN_DESK_PREF_GRAPH_SUPERVISOR"), 0, this );
    QAD_ASSERT(connect( graphSupervisorAction, SIGNAL(activated()), this, SLOT(onGraphSupervisor() )));
    graphSupervisorAction->addTo( &myViewerPopup );
    myStdActions.insert( PrefGraphSupervisorId, graphSupervisorAction );

    QActionP* viewerPlot2dAction = new QActionP( "", tr("MEN_DESK_PREF_VIEWER_PLOT2D"), 0, this );
    QAD_ASSERT(connect( viewerPlot2dAction, SIGNAL(activated()), this, SLOT(onPlot2d() )));
    viewerPlot2dAction->addTo( &myViewerPopup );
    myStdActions.insert( PrefViewerPlot2dId, viewerPlot2dAction );

    //VRV: T2.5 - add default viewer
    QString viewerValue = QAD_CONFIG->getSetting( "Viewer:DefaultViewer" );
    bool ok;
    int aViewerValue = viewerValue.toInt( &ok, 10 ); 
    if (!ok || aViewerValue < VIEW_OCC || aViewerValue >= VIEW_TYPE_MAX)
      aViewerValue = VIEW_VTK;

    myPrefPopup.insertItem( tr("MEN_DESK_PREF_DEFAULT_VIEWER"), &myDefaultViewer );
    QActionPGroup* myQAG = new QActionPGroup ( this);
    QActionP* viewerOCCAction1 = new QActionP( "", tr("MEN_DESK_PREF_VIEWER_OCC"), 0, this );
    viewerOCCAction1->setToggleAction ( true);
    viewerOCCAction1->setOn ( aViewerValue == VIEW_OCC );
    myQAG->insert( viewerOCCAction1 );
    myStdActions.insert( DefaultViewerOCCId, viewerOCCAction1 );

    QActionP* viewerVTKAction1 = new QActionP( "", tr("MEN_DESK_PREF_VIEWER_VTK"), 0, this );
    viewerVTKAction1->setToggleAction ( true);
    viewerVTKAction1->setOn ( aViewerValue == VIEW_VTK );
    myQAG->insert( viewerVTKAction1 );
    myStdActions.insert( DefaultViewerVTKId, viewerVTKAction1 );

/*    Remove Supervisor viewer from setting the background */
//    QActionP* graphSupervisorAction1 = new QActionP( "", tr("MEN_DESK_PREF_GRAPH_SUPERVISOR"), 0, this );
//    graphSupervisorAction1->setToggleAction ( true);
//    graphSupervisorAction1->setOn ( aViewerValue == VIEW_GRAPHSUPERV );
//    myQAG->insert( graphSupervisorAction1 );
//    myStdActions.insert( DefaultGraphSupervisorId, graphSupervisorAction1 );

    QActionP* viewerPlot2dAction1 = new QActionP( "", tr("MEN_DESK_PREF_VIEWER_PLOT2D"), 0, this );
    viewerPlot2dAction1->setToggleAction ( true);
    viewerPlot2dAction1->setOn ( aViewerValue == VIEW_PLOT2D );
    myQAG->insert( viewerPlot2dAction1 );
    myStdActions.insert( DefaultPlot2dId, viewerPlot2dAction1 );

    myQAG->addTo( &myDefaultViewer );
    QAD_ASSERT(connect( myQAG, SIGNAL(selected(QActionP * )), this, SLOT(onDefaultViewer(QActionP *) )));
    //VRV: T2.5 - add default viewer

    myPrefPopup.insertSeparator();

    QActionP* viewerTrihedronAction = new QActionP( "", tr("MEN_DESK_PREF_VIEWER_TRIHEDRON"), 0, this );
    QAD_ASSERT(connect( viewerTrihedronAction, SIGNAL(activated()), this, SLOT(onViewerTrihedron() )));
    viewerTrihedronAction->addTo( &myPrefPopup );
    myStdActions.insert( PrefViewerTrihedronId, viewerTrihedronAction );

    QActionP* consoleFontAction = new QActionP( "", tr("MEN_DESK_PREF_CONSOLE_FONT"), 0, this );
    QAD_ASSERT(connect( consoleFontAction, SIGNAL(activated()), this, SLOT(onConsoleFontAction() )));
    consoleFontAction->addTo( &myPrefPopup );
    myStdActions.insert( PrefConsoleFontId, consoleFontAction );

    /* MultiFile save */
    QActionP* multiFileSaveAction = new QActionP( "", tr("MEN_DESK_PREF_MULTI_FILE_SAVE"), 0, this, 0, true );
    QAD_ASSERT(connect( multiFileSaveAction, SIGNAL(activated()), this, SLOT(onMultiFileSave() )));
    multiFileSaveAction->setToggleAction( true );
    QString MultiSave = QAD_CONFIG->getSetting("Desktop:MultiFileSave");
    multiFileSaveAction->setOn( MultiSave.compare( aTrueQString ) == 0 );
    multiFileSaveAction->addTo( &myPrefPopup );
    myStdActions.insert( PrefMultiFileSave, multiFileSaveAction );

    /* ASCII save */
    QActionP* ASCIISaveAction = new QActionP( "", tr("MEN_DESK_PREF_ASCII_SAVE"), 0, this, 0, true );
    QAD_ASSERT(connect( ASCIISaveAction, SIGNAL(activated()), this, SLOT(onASCIISave() )));
    ASCIISaveAction->setToggleAction( true );
    QString ASCIISave = QAD_CONFIG->getSetting("Desktop:ASCIISave");
    ASCIISaveAction->setOn( ASCIISave.compare( aTrueQString ) == 0 );
    ASCIISaveAction->addTo( &myPrefPopup );
    myStdActions.insert( PrefASCIISave, ASCIISaveAction );

    /* Undo level */
    QActionP* UndoLevelAction = new QActionP( "", tr("MEN_DESK_PREF_UNDO_LEVEL"), 0, this );
    QAD_ASSERT(connect( UndoLevelAction, SIGNAL(activated()), this, SLOT(onUndoLevel() )));
    UndoLevelAction->addTo( &myPrefPopup );
    myStdActions.insert( PrefUndoLevelId, UndoLevelAction );

    myPrefPopup.insertSeparator();
    
    /* External Browser */
    QActionP* externalBrowserAction = new QActionP( "", tr("MEN_DESK_PREF_EXTERNAL_BROWSER"), 0, this );
    QAD_ASSERT(connect( externalBrowserAction, SIGNAL(activated()), this, SLOT(onExternalBrowser() )));
    externalBrowserAction->addTo( &myPrefPopup );
    myStdActions.insert( PrefExternalBrowserId, externalBrowserAction );
        
    myPrefPopup.insertSeparator();
    /* BrowserPopup */
    myPrefPopup.insertItem( tr("MEN_DESK_PREF_OBJECTBROWSER"), &myObjBrowserPopup );
    
    QActionP* objectBrowserEntryAction = new QActionP( "", tr("MEN_DESK_PREF_OBJECTBROWSER_ENTRY"), 0, this, 0, true );
    QAD_ASSERT(connect( objectBrowserEntryAction, SIGNAL(activated()), this, SLOT(onObjectBrowser() )));
    objectBrowserEntryAction->setToggleAction(true);
    QString AddColumn = QAD_CONFIG->getSetting("ObjectBrowser:AddColumn");
    
    if ( AddColumn.compare( aTrueQString ) == 0 )
      objectBrowserEntryAction->setOn(true);
    else
      objectBrowserEntryAction->setOn(false);

    objectBrowserEntryAction->addTo( &myObjBrowserPopup );
    myStdActions.insert( PrefObjectBrowserEntryId, objectBrowserEntryAction );

    QActionP* objectBrowserValueAction = new QActionP( "", tr("MEN_DESK_PREF_OBJECTBROWSER_VALUE"), 0, this, 0, true );
    QAD_ASSERT(connect( objectBrowserValueAction, SIGNAL(activated()), this, SLOT(onObjectBrowser() )));
    objectBrowserValueAction->setToggleAction(true);
    QString ValueColumn = QAD_CONFIG->getSetting("ObjectBrowser:ValueColumn");
    
    if ( ValueColumn.compare( aTrueQString ) == 0 )
      objectBrowserValueAction->setOn(true);
    else
      objectBrowserValueAction->setOn(false);

    objectBrowserValueAction->addTo( &myObjBrowserPopup );
    myStdActions.insert( PrefObjectBrowserValueId, objectBrowserValueAction );

    QActionP* objectBrowserIAPPAction = new QActionP( "", tr("MEN_DESK_PREF_OBJECTBROWSER_IAPP"), 0, this, 0, true );
    QAD_ASSERT(connect( objectBrowserIAPPAction, SIGNAL(activated()), this, SLOT(onObjectBrowser() )));
    objectBrowserIAPPAction->setToggleAction(true);
    QString showIAPP = QAD_CONFIG->getSetting("ObjectBrowser:IAPP");

    if ( showIAPP.compare( aTrueQString ) == 0 )
      objectBrowserIAPPAction->setOn(true);
    else
      objectBrowserIAPPAction->setOn(false);

    objectBrowserIAPPAction->addTo( &myObjBrowserPopup );
    myStdActions.insert( PrefObjectBrowserIAPPId, objectBrowserIAPPAction );
    
    /* Chronological sorting of shapes on the entry creation */
    QActionP* objectBrowserCHRONO_SORTAction = new QActionP( "", tr("MEN_DESK_PREF_OBJECTBROWSER_CHRONO_SORT"), 0, this, 0, true );
    QAD_ASSERT(connect( objectBrowserCHRONO_SORTAction, SIGNAL(activated()), this, SLOT(onObjectBrowser() )));
    objectBrowserCHRONO_SORTAction->setToggleAction(true);
    QString showSORT = QAD_CONFIG->getSetting("ObjectBrowser:ChronologicalSort");
    
    if ( showSORT.compare( aTrueQString ) == 0 )
      objectBrowserCHRONO_SORTAction->setOn(true) ;
    else
      objectBrowserCHRONO_SORTAction->setOn(false) ;

    objectBrowserCHRONO_SORTAction->addTo( &myObjBrowserPopup );
    myStdActions.insert( PrefObjectBrowserCHRONO_SORTId, objectBrowserCHRONO_SORTAction ) ;
    
    /* Show or don't Show UseCase browser */
    QActionP* objectBrowserShowUseCaseAction = new QActionP( "", tr("MEN_DESK_PREF_OBJECTBROWSER_SHOW_USECASE"), 0, this, 0, true );
    QAD_ASSERT(connect( objectBrowserShowUseCaseAction, SIGNAL(activated()), this, SLOT(onObjectBrowser() )));
    objectBrowserShowUseCaseAction->setToggleAction(true);
    QString showUseCase = QAD_CONFIG->getSetting("ObjectBrowser:ShowUseCaseBrowser");
    
    if ( showUseCase.compare( aTrueQString ) == 0 )
      objectBrowserShowUseCaseAction->setOn(true) ;
    else
      objectBrowserShowUseCaseAction->setOn(false) ;
    
    objectBrowserShowUseCaseAction->addTo( &myObjBrowserPopup );
    myStdActions.insert( PrefObjectBrowserShowUseCaseId, objectBrowserShowUseCaseAction ) ;

    /* Resize or don't resize columns automatically */
    QActionP* objectBrowserNoAutoSizeAction = new QActionP( "", tr("MEN_DESK_PREF_OBJECTBROWSER_NO_AUTOSIZE"), 0, this, 0, true );
    QAD_ASSERT(connect( objectBrowserNoAutoSizeAction, SIGNAL(activated()), this, SLOT(onObjectBrowser() )));
    objectBrowserNoAutoSizeAction->setToggleAction(true);
    QString noAutoSize = QAD_CONFIG->getSetting("ObjectBrowser:NoAutoSizeColumns");
    
    if ( noAutoSize.compare( aTrueQString ) == 0 )
      objectBrowserNoAutoSizeAction->setOn(true) ;
    else
      objectBrowserNoAutoSizeAction->setOn(false) ;
    
    objectBrowserNoAutoSizeAction->addTo( &myObjBrowserPopup );
    myStdActions.insert( PrefObjectBrowserNoAutoSizeColumnsId, objectBrowserNoAutoSizeAction ) ;

    myPrefPopup.insertSeparator();

    QActionP* dirAction = new QActionP( "", tr("MEN_DESK_PREF_DIRICTORIES"), ALT+Key_D, this );
    QAD_ASSERT(connect( dirAction, SIGNAL(activated()), this, SLOT(onDirList() )));
    dirAction->addTo( &myPrefPopup );
    myStdActions.insert( PrefDirsId, dirAction );
    
    myPrefPopup.insertSeparator();

    QActionP* saveAction = new QActionP( "", tr("MEN_DESK_PREF_SAVE"), 0, this );
    QAD_ASSERT(connect( saveAction, SIGNAL(activated()), this, SLOT(onSavePref() )));
    saveAction->addTo( &myPrefPopup );
    myStdActions.insert( PrefSaveId, saveAction );

    /* 'Window' actions	 */
    /*  new Window 3D    */
    //NRI : SAL2214
    myWindowPopup.insertItem( tr("MEN_DESK_WINDOW_NEW3D"), &myNewViewPopup, WindowNew3dId );
    
    QActionP* viewOCCAction = new QActionP( "", tr("MEN_DESK_VIEW_OCC"), ALT+Key_O, this );
    QAD_ASSERT(connect( viewOCCAction, SIGNAL(activated()), this, SLOT(onNewWindow3d() )));
    viewOCCAction->addTo( &myNewViewPopup );
    myStdActions.insert( ViewOCCId, viewOCCAction );

    QActionP* viewVTKAction = new QActionP( "", tr("MEN_DESK_VIEW_VTK"), ALT+Key_K, this );
    QAD_ASSERT(connect( viewVTKAction, SIGNAL(activated()), this, SLOT(onNewWindow3d() )));
    viewVTKAction->addTo( &myNewViewPopup );
    myStdActions.insert( ViewVTKId, viewVTKAction );

    QActionP* viewPlot2dAction = new QActionP( "", tr("MEN_DESK_VIEW_PLOT2D"), ALT+Key_P, this );
    QAD_ASSERT(connect( viewPlot2dAction, SIGNAL(activated()), this, SLOT(onNewWindow3d() )));
    viewPlot2dAction->addTo( &myNewViewPopup );
    myStdActions.insert( ViewPlot2dId, viewPlot2dAction );
    
    //  QActionP* windowNew3dAction = new QActionP( "",/* rmgr->loadPixmap( "QAD", tr("ICON_DESK_WINDOW_NEW3D") ), */
    /*tr("MEN_DESK_WINDOW_NEW3D"), 0, this );
      windowNew3dAction->addTo( &myWindowPopup );
      QAD_ASSERT( connect( windowNew3dAction, SIGNAL(activated()), this ,
      SLOT( onNewWindow3d() )));
      myStdActions.insert( WindowNew3dId, windowNew3dAction );
    */
 
    //NRI : SAL2214

    /* cascaded */
    QActionP* windowCascadeAction = new QActionP( "", rmgr->loadPixmap( "QAD", tr("ICON_DESK_WINDOW_CASCADE") ),
						tr("MEN_DESK_WINDOW_CASCADE"), 0, this );
    windowCascadeAction->setStatusTip ( tr("PRP_DESK_WINDOW_CASCADE") );
    QAD_ASSERT( connect( windowCascadeAction, SIGNAL(activated()), this,
			 SLOT( onCascade() )));
    myStdActions.insert( WindowCascadeId, windowCascadeAction );

    /* tiled */
    QActionP* windowTileAction = new QActionP( "", rmgr->loadPixmap( "QAD", tr("ICON_DESK_WINDOW_TILE") ),
					     tr("MEN_DESK_WINDOW_TILE"), 0, this );
    windowTileAction->setStatusTip ( tr("PRP_DESK_WINDOW_TILE") );
    QAD_ASSERT( connect( windowTileAction, SIGNAL(activated()), myWorkspace, SLOT( tile() )));
    myStdActions.insert( WindowTileId, windowTileAction );

    QAD_ASSERT( connect ( &myWindowPopup, SIGNAL(aboutToShow()),
			  this, SLOT(onWindowPopupAboutToShow()) ));
    /* 'Help' actions
     */
    /* contents */
    // MZN : Commented
    // QActionP* helpContentsAction = new QActionP( "", tr("MEN_DESK_HELP_CONTENTS"), Key_F1, this );
    // helpContentsAction->setStatusTip ( tr("PRP_DESK_HELP_CONTENTS") );
//     QAD_ASSERT(connect( helpContentsAction, SIGNAL(activated()),
// 			this, SLOT( onHelpContents() )));
//     helpContentsAction->addTo( &myHelpPopup );
//     myStdActions.insert( HelpContentsId , helpContentsAction );
				
//     id = myHelpPopup.insertSeparator();
						   
    /* GUI contents */
    myHelpPopup.insertItem( tr("MEN_DESK_HELP_MODULECONTENTS"), &myHelpContentsModulePopup, HelpContentsModuleId);
    bool toEnable = false;
    
    CORBA::Object_var objVarN = myNameService->Resolve("/Kernel/ModulCatalog");
    myCatalogue  = SALOME_ModuleCatalog::ModuleCatalog::_narrow(objVarN);
    
    SALOME_ModuleCatalog::ListOfIAPP_Affich_var list_composants =
      myCatalogue->GetComponentIconeList();
      
    for (unsigned int ind = 0; ind < list_composants->length(); ind++) {
      QString aModuleName = CORBA::string_dup(list_composants[ind].modulename) ;
      QString dir;
      if (dir = getenv( aModuleName + "_ROOT_DIR")) {
	dir = QAD_Tools::addSlash( QAD_Tools::addSlash(dir) + "doc/salome/" );
	QString aFileName = aModuleName + "_index.html"; 
	if ( QFileInfo( dir + aFileName ).exists() ) {
	  QString aModuleUserName = CORBA::string_dup(list_composants[ind].moduleusername) ;
	  if ( aModuleUserName == "Salome" )  aModuleUserName = "Kernel" ;
	  QActionP* moduleHelpAction = new QActionP( "", aModuleUserName + " Help" , 0, this, aModuleName);
	  QAD_ASSERT(connect( moduleHelpAction, SIGNAL(activated()), this, SLOT(onHelpContentsModule() )));
	  moduleHelpAction->addTo( &myHelpContentsModulePopup );
	  if (!toEnable) toEnable = true;
	}
      }
    }
    
    myHelpContentsModulePopup.setEnabled(toEnable);
              
   // NRI : Temporary commented

//     QActionP* helpContentsActionGUI = new QActionP( "", tr("MEN_DESK_HELP_GUICONTENTS"), 0, this );
//     helpContentsActionGUI->setStatusTip ( tr("PRP_DESK_HELP_GUICONTENTS") );
//     QAD_ASSERT(connect( helpContentsActionGUI, SIGNAL(activated()),
// 			this, SLOT( onHelpContentsGUI() )));
//     helpContentsActionGUI->addTo( &myHelpPopup );
//     myStdActions.insert( HelpContentsId , helpContentsActionGUI );

    
    /* TUI contents */
    /*
    QActionP* helpContentsActionTUI = new QActionP( "", tr("MEN_DESK_HELP_TUICONTENTS"), 0, this );
    helpContentsActionTUI->setStatusTip ( tr("PRP_DESK_HELP_TUICONTENTS") );
    QAD_ASSERT(connect( helpContentsActionTUI, SIGNAL(activated()),
			this, SLOT( onHelpContentsTUI() )));
    helpContentsActionTUI->addTo( &myHelpPopup );
    myStdActions.insert( HelpContentsId , helpContentsActionTUI );						   
    */


    // Provide status tip for Module help menu item
    QAD_ASSERT( connect( &myHelpPopup, SIGNAL(highlighted( int )), this,
			 SLOT(onHelpModulePopupStatusText( int )) ));
       

    /* search */
//    QActionP* helpSearchAction = new QActionP( "", tr("MEN_DESK_HELP_SEARCH"), 0, this );
//    helpSearchAction->setStatusTip ( tr("PRP_DESK_HELP_SEARCH") );
//    QAD_ASSERT( connect( helpSearchAction, SIGNAL(activated()), this, SLOT( onHelpSearch() )));
//    helpSearchAction->addTo( &myHelpPopup );
//    myStdActions.insert( HelpSearchId, helpSearchAction );

    /* What's This */
//    QActionP* helpWhatsThisAction = new QActionP( "", tr("MEN_DESK_HELP_WHATSTHIS"), SHIFT+Key_F1, this );
//    helpWhatsThisAction->setStatusTip ( tr("PRP_DESK_HELP_WHATSTHIS" ));
//    QAD_ASSERT( connect( helpWhatsThisAction, SIGNAL(activated()), this, SLOT( whatsThis() )));
//    helpWhatsThisAction->addTo( &myHelpPopup );
//    myStdActions.insert( HelpWhatsThisId, helpWhatsThisAction );
//    (void)QWhatsThis::whatsThisButton( myStdToolBar );

    id = myHelpPopup.insertSeparator();
    myHelpPos = myHelpPopup.indexOf( id );

    /* about */
    QActionP* helpAboutAction = new QActionP( "", tr("MEN_DESK_HELP_ABOUT"), 0, this );
    helpAboutAction->setStatusTip ( tr("PRP_DESK_HELP_ABOUT") );
    QAD_ASSERT( connect( helpAboutAction, SIGNAL(activated()), this, SLOT( onHelpAbout() )));
    helpAboutAction->addTo( &myHelpPopup );
    myStdActions.insert(HelpAboutId, helpAboutAction );
  }
  updateActions();
}

/*!
    Adds the given application into the list of supported applications
*/
void QAD_Desktop::addApplication(QAD_Application* app)
{
  /* add only if is not already in the list */
  if ( myApps.findRef( app ) == -1 )
    {
      myApps.append( app );

      /* set activation/deactivation listener */
      QAD_ASSERT ( connect( app, SIGNAL(appActivated(QAD_Application*)),
			    this, SLOT(onActivateApp(QAD_Application*))) );

      QAD_ASSERT ( connect( app, SIGNAL(appDeactivated(QAD_Application*)),
			    this, SLOT(onDeactivateApp(QAD_Application*))) );
    }
}

/*!
    Displays the control panel of the application.
    This function is called when the desktop is shown first time.
*/
void QAD_Desktop::showDesktop()
{
  show();
}

/*!
  Closes Desktop. If <forceClose> is true query for exit will be skipped.
*/
void QAD_Desktop::closeDesktop( bool forceClose )
{
  if ( forceClose)
    myQueryClose = false;
  close();
}

/*!
    Puts the message to the status bar
*/
void QAD_Desktop::putInfo ( const QString& msg )
{
  if ( myStatusBar )
    //NRI : bug 1209. message is now displayed during 3000 ms
    myStatusBar->message ( msg, 3000 );
}

/*!
    Puts the message to the status bar for ms milli-seconds
*/
void QAD_Desktop::putInfo ( const QString& msg, int ms )
{
  if ( myStatusBar )
    myStatusBar->message ( msg, ms );
}

/*!
    Returns the standard toolbar
*/
QMenuBar* QAD_Desktop::getMainMenuBar() const
{
    return myMainMenu;
}

/*!
    Returns the standard toolbar
*/
QToolBar* QAD_Desktop::getStdToolBar() const
{
    return myStdToolBar;
}

/*!
    Updates desktop panel (menu, toolbar and status bar) according to current state of
    the application. Calls update() for the active application.
    The command { UD_ADD_APP or UD_REMOVE_APP ) specifies the desktop components
    will be changed according to new active application.
    This function is called after activating new study, closing current study etc.
*/
void QAD_Desktop::updateDesktop( UpdateCommand cmd )
{
  updateMenu( cmd );
  updateToolBars( cmd );
  updateStatusBar( cmd );
  updateActions();
}

/*!
    Updates the desktop caption
*/
void QAD_Desktop::updateCaption( UpdateCommand cmd )
{
    if ( cmd == ActivateApp )
    {	/* application's icon and title */
	setIcon( myActiveApp->getApplicationIcon() );
	setCaption( myActiveApp->getApplicationName() );
    }
    else
    {	/* default icon and title */
	setIcon( myDefaultIcon );
	qApp->processEvents();
	setCaption( myDefaultTitle );
    }
}

/*!
    Updates status bar when a new application becomes current
*/
void QAD_Desktop::updateStatusBar( UpdateCommand cmd )
{
    if ( myActiveApp )
    {
	bool activate = ( cmd == ActivateApp );
	myActiveApp->updateStatusBar ( myStatusBar, activate );
    }
}

/*!
    Updates toolbars according to current active application.
    This function is called from updateDesktop().
*/
void QAD_Desktop::updateToolBars( UpdateCommand cmd )
{
    if ( myActiveApp )
    {
	bool activate = ( cmd == ActivateApp );
	myActiveApp->updateToolBars ( activate );
    }
}

/*!
    Updates main menu bar of the application when application is activated
    or deactivated.
    This function is called by updateDesktop()
*/
void QAD_Desktop::updateMenu( UpdateCommand cmd )
{
    if ( myActiveApp )
    {
	bool activate = ( cmd == ActivateApp );
	myActiveApp->updateFilePopup( &myFilePopup, activate, myFilePos );
	myActiveApp->updateEditPopup( &myEditPopup, activate, myEditPos );
	myActiveApp->updateViewPopup( &myViewPopup, activate, myViewPos );
	myActiveApp->updateHelpPopup( &myHelpPopup, activate, myHelpPos );
	myActiveApp->updateMainMenu( myMainMenu, activate, myMainMenuPos );

	/*  NOTE:
	    Since 'Edit' popup is completely customized by applications
	    desktop doesn't show it in main menubar without the active
	    application and we insert/remove this popup from the menubar
	    when activating/deactivating applications.

	    We can't have 'Edit' popup be customized by an application
	    via UpdateMainMenu() method because applications may insert
	    its items to the menubar only AFTER 'View' item ( this fact
	    is respected by myMainMenuPos ) and the possibility of an
	    application to add its items into any arbitrary positions of
	    the main menubar requires a bit sophisticated implementation,
	    so we decided to avoid it, sorry :-(.
	*/
#if defined( DEBUG )
	if ( !activate )
	    /* applications MUST clear 'Edit' after themselves */
	    QAD_ASSERT_DEBUG_ONLY( !myEditPopup.count() );
#endif

	if ( activate )  /* insert after 'File' item */
	  myMainMenu->insertItem( tr("MEN_DESK_EDIT"), &myEditPopup, 3, 1 );
	else
	  myMainMenu->removeItem( 3 );
    }
}

/*!
    Returns icon file name used by the application
*/
const QPixmap& QAD_Desktop::getDefaultIcon() const
{
    static QPixmap defaultPixmap( QAD_Desktop::getResourceManager()->loadPixmap( "QAD",
							    tr("ICON_DESK_DEFAULTICON") ));
    return defaultPixmap;
}

/*!
    Returns name of the application used as the title of desktop panel
*/
const QString& QAD_Desktop::getDefaultTitle() const
{
    static QString defaultTitle ( tr("DESK_DEFAULTTITLE") );
    return defaultTitle;
}

/*!
    Returns default name of new studies
*/
const QString& QAD_Desktop::getDefaultStudyName() const
{
  static QString defaultStudyName( tr("DESK_DEFAULTDOC") );
  return defaultStudyName;
}

/*!
    Returns next name for new study
*/
QString QAD_Desktop::getNewStudyName()
{
    QString number;
    number.sprintf( "%d", ++myCntUntitled );
    return ( getDefaultStudyName() + number );
}

/*!
  Searches QAD_Study corresponding to <study>, returns NULL if fails
*/
QAD_Study* QAD_Desktop::findStudy( SALOMEDS::Study_ptr theStudy )
{
  for ( QAD_Application* app = myApps.first(); app; app = myApps.next() ) {
    QList<QAD_Study> studies; 
    studies.setAutoDelete( false );
    studies = app->getStudies();
    for ( QAD_Study* study = studies.first(); study; study = studies.next() ) {
      if ( study->getStudyDocument()->_is_equivalent( theStudy ) )
	return study;
    }
  }
  return 0;
}

/*!
  Gets value of max undo level for SALOMEDS::Study's from preferences
*/
int QAD_Desktop::getUndoLevel()
{
  static int MAX_UNDO = 10;
  int anUndoLevel = MAX_UNDO;
  QString aLevel = QAD_CONFIG->getSetting("Desktop:UndoLevel");
  if(!aLevel.isEmpty()) anUndoLevel = aLevel.toInt();
  return anUndoLevel;
}

/*!
    Returns current active application
*/
QAD_Application* QAD_Desktop::getActiveApp() const
{
    return myActiveApp;
}

/*!
    Returns current active study
*/
QAD_Study* QAD_Desktop::getActiveStudy() const
{
    return myActiveStudy;
}

/*!
    Returns main frame
*/
QWorkspaceP* QAD_Desktop::getMainFrame() const
{
  return myWorkspace;
}

/*!
    Find or load Engine, given a container name and an engine name
*/
Engines::Component_var QAD_Desktop::getEngine(const char *containerName,
					      const char *componentName)
{
  Engines::Component_var eng =
    myEnginesLifeCycle->FindOrLoad_Component(containerName,
					     componentName);
  return eng._retn();
}

QString QAD_Desktop::getComponentName(const char *componentUserName)
{
  if ( mapComponentName.contains(componentUserName) )
    return mapComponentName[ componentUserName ] ;
  else
    return "";
}

QString QAD_Desktop::getComponentUserName(const char *componentName)
{
  QMap<QString,QString>::Iterator it;
  for( it = mapComponentName.begin(); it != mapComponentName.end(); ++it )
    if (it.data() == componentName )
      return it.key();
  return "";
}

// /*!
//   gets application Help Window (and creates if necessary)
// */
// QAD_HelpWindow* QAD_Desktop::getHelpWindow()
// {
//   if (!myHelpWindow) {
//     myHelpWindow = new QAD_HelpWindow();  
    
//     QMap<QString,QString>::Iterator it;
//     for( it = mapComponentName.begin(); it != mapComponentName.end(); ++it ) {
//       QCString dir;
//       QString root;
      
//       // look for index.html and set homeDir
//       // 1. $(MODULE_ROOT_DIR)/doc/index.html
//       // 2. $(MODULE_ROOT_DIR)/doc/html/index.html
//       // 3. $(MODULE_ROOT_DIR)/doc/html/html/index.html

//       if (dir = getenv( QString( it.data() + "_ROOT_DIR")) ) {
// 	root = QAD_Tools::addSlash( QAD_Tools::addSlash(dir) + QAD_Tools::addSlash("share")  + QAD_Tools::addSlash("salome")  + "doc" );
// 	if ( QFileInfo( root + "index.html" ).exists() ) {
// 	  helpContext( root + "index.html", "" );
// 	}
// 	else {
// 	  root = QAD_Tools::addSlash( root + "html" );
// 	  if ( QFileInfo( root + "index.html" ).exists() ) {
// 	    helpContext( root + "index.html", "" );
// 	  }
// 	  else {
// 	    root = QAD_Tools::addSlash( root + "html" );
// 	    if ( QFileInfo( root + "index.html" ).exists() ) {
// 	      helpContext( root + "index.html", "" );
// 	    }
// 	  }
// 	}
//       }
//     }
    
//     connect(myHelpWindow, SIGNAL(helpWindowClosed()), this, SLOT(onHelpWindowClosed()));
//   }
//   return myHelpWindow;
// }

/*!
    Called when desktop is closing
*/
void QAD_Desktop::closeEvent ( QCloseEvent* e )
{
  bool doClose = true;
  if  ( myQueryClose ) {
    doClose = QAD_MessageBox::info2 ( this, tr("INF_DESK_EXIT"),
				      tr("QUE_DESK_EXIT"), tr ("BUT_OK"),
				      tr ("BUT_CANCEL"), QAD_YES,
				      QAD_NO, QAD_NO ) == QAD_YES;
  }

  for ( QAD_Application* app = myApps.first(); doClose && app; app = myApps.next() ) { 
    QList<QAD_Study>& studies = app->getStudies();
    for(QAD_Study* study = studies.first(); doClose && study != 0; study = studies.next()) {
      if(myQueryClose && study->getStudyDocument()->IsModified()) {
	SALOMEGUI_CloseDlg aDlg( this );
	switch ( aDlg.exec() ) {
	case 1:
	  if ( !onSaveStudy( study ) ) {
	    putInfo( tr("INF_CANCELLED") );
	    doClose = false;
	  }
	  break;
	case 2:
	case 3:
	  break;
	case 0:
	default: 
	  doClose = false;
	  putInfo( tr("INF_CANCELLED") );
	}
      }
      if ( doClose )
	study->close();
    }
  }

  myQueryClose = true;

  if ( !doClose ) {
    myAboutToClose = false;
    // onActiveStudyChanged() is normally caused by QWorkspace::eventFilter(), 
    // but this call was blocked by myAboutToClose == true, so now we should do it manually
    onActiveStudyChanged(); 

    e->ignore();
  }
  else
    e->accept();
}

/*!
    Called when desktop is resized
*/
void QAD_Desktop::resizeEvent( QResizeEvent* e )
{
    QMainWindow::resizeEvent( e );
    myWorkspace->setGeometry( 1,1, centralWidget()->width()-2,
			      centralWidget()->height()-2 );
}


/*!
    Terminates the application.
*/
void QAD_Desktop::onExit()
{
  close();
}

/************************************************************************
**			Study operations
*************************************************************************/

/*!
    Creates new study.
    If several application are supported it displays dialog window
    to choose type of study.
*/
void QAD_Desktop::createStudy(){
  onNewStudy();
}
void QAD_Desktop::onNewStudy()
{
  unsigned numApps = myApps.count();
  if ( numApps ) {
    if ( numApps == 1 )
      onNewStudy ( myApps.at(0) );
    else  {   /* select the type of application for new study */
      Desktop_AppSelectionDlg selDlg( this, myApps );
      if ( selDlg.exec() == QDialog::Accepted ) {
	onNewStudy ( selDlg.selectedApp() );
      }
    }
  } else  {   /* no applications available in desktop */
    QAD_MessageBox::error1( this, 
			    tr("ERR_ERROR"),
			    tr("ERR_APP_NOAPP"), 
			    tr("BUT_OK") );
  }
}

/*!
    Creates new study for the given application.
    Calls the respective function of the application class.
*/
void QAD_Desktop::onNewStudy( QAD_Application* app )
{
  if ( !app ) return;

  /* create new study of the specific app */
  putInfo ( tr ("INF_DOC_CREATING") );
  QAD_Study* newStudy = app->newStudy();

  if ( !newStudy ) {
    /* can't create new study */
    QAD_MessageBox::error1( this, tr("ERR_ERROR"),
			    tr("ERR_DOC_CANTCREATE"), 
			    tr("BUT_OK") );
  }
  putInfo ( tr("INF_READY") );
}

/*!
    Loads the existing study.
*/
void QAD_Desktop::onLoadStudy()
{
  QString name, studyname, ext;

  SALOMEGUI_LoadStudiesDlg* aDlg = new SALOMEGUI_LoadStudiesDlg( this, "Load Study", TRUE);

  CORBA::Object_var obj = myNameService->Resolve("/myStudyManager");
  SALOMEDS::StudyManager_var myStudyMgr = SALOMEDS::StudyManager::_narrow(obj);
  ASSERT(! CORBA::is_nil(myStudyMgr));
  SALOMEDS::ListOfOpenStudies_var List = myStudyMgr->GetOpenStudies();
  for (unsigned int ind = 0; ind < List->length();ind++) {
    studyname = List[ind];
    aDlg->ListComponent->insertItem( studyname );
  }
  
  int retVal = aDlg->exec();
  studyname = aDlg->ListComponent->currentText();
  delete aDlg;
  if (retVal == QDialog::Rejected) 
    return;

  if ( studyname.isNull() || studyname.isEmpty() )
    return;
  
  name = studyname;
  name.replace( QRegExp(":"), "/" );
  
  /*
    Try to associate an application to the opened study.
    Assumed that the association study<->application is unique.
  */
  bool appFound = false;
  putInfo ( tr("INF_DOC_OPENING") + " " + name );
  for ( QAD_Application* app = myApps.first(); app; app = myApps.next() )
    {
      /* application found */
      if ( app->isStudyOpened( name ) )
	{
	  /* already opened: prompt for reopen */
	  if ( QAD_MessageBox::warn2 ( this, tr("WRN_WARNING"),
				       tr("QUE_DOC_ALREADYOPEN").arg( name ),
				       tr ("BUT_YES"), tr ("BUT_NO"), QAD_YES, QAD_NO,
				       QAD_NO ) == QAD_NO )
	    {
	      putInfo ( tr("INF_READY") );
	      return;
	    }
	  /* now the study is closed to be re-opened */
	  //don't ask user to remove study permanently
	  if (app->getStudyByName ( name ) != NULL)
	    onCloseStudy ( app->getStudyByName ( name ), false );
	  else if (app->getStudyByName ( QAD_Tools::getFileNameFromPath( name, true )) != NULL)
	    onCloseStudy ( app->getStudyByName ( QAD_Tools::getFileNameFromPath( name, true )), false );
	}
      appFound = true;
      
      /* open the study */
      QAD_Study* openStudy = app->loadStudy( studyname );
      if ( !openStudy ) {
	QAD_MessageBox::error1( this, 
			        tr("ERR_ERROR"),
			        tr("ERR_DOC_CANTOPEN") + "\n" + name,
			        tr("BUT_OK") );
      }
      break;
    }
  
  if ( !appFound ) {
    QAD_MessageBox::error1( this, 
			    tr("ERR_ERROR"), 
			    tr("ERR_DOC_UNKNOWNTYPE"), 
 			    tr("BUT_OK") );
  }
  putInfo ( tr("INF_READY") );
}



QAD_Study* QAD_Desktop::loadStudy(QString theStudyName) {
  QString studyname("");

  CORBA::Object_var obj = myNameService->Resolve("/myStudyManager");
  SALOMEDS::StudyManager_var myStudyMgr = SALOMEDS::StudyManager::_narrow(obj);
  ASSERT(! CORBA::is_nil(myStudyMgr));

  SALOMEDS::ListOfOpenStudies_var List = myStudyMgr->GetOpenStudies();
  for (unsigned int ind = 0; ind < List->length();ind++) {
    if (theStudyName == QString(List[ind])) {
      studyname = theStudyName;
      break;
    }
  }
  if (studyname == "") {
    return NULL;
  }
  bool appFound = false;
  for ( QAD_Application* app = myApps.first(); app; app = myApps.next() ) {
    appFound = true;
    QAD_Study* openStudy = app->loadStudy( studyname );
    if (openStudy) {
      //      if (myActiveApp == 0)
      //myActiveApp = app;
      return openStudy;
    }
    break;
  }
  return NULL;
}


/*!
    Opens the existing study.
    Displays select file dialog and calls corresponding function of
    application class
    (application is defined according to extension of selected file).
*/
void QAD_Desktop::onOpenStudy()
{
    QString name, ext;

    /* Select a file to open  */
    name = selectFileName ( true );
    if ( name.isNull() || name.isEmpty() )
	return;
    
    // Workaround for non-existent files
    QFileInfo fi(name);
    if (!fi.exists() || !fi.isFile()) {
      QAD_MessageBox::error1 ( this, tr("ERR_ERROR"),
				     tr("ERR_DOC_CANTOPEN") +": " +  name,
				     tr("BUT_OK") );
      return;
    }

    CORBA::Object_var obj = myNameService->Resolve("/myStudyManager");
    SALOMEDS::StudyManager_var myStudyMgr = SALOMEDS::StudyManager::_narrow(obj);
    ASSERT(! CORBA::is_nil(myStudyMgr));
    SALOMEDS::ListOfOpenStudies_var List = myStudyMgr->GetOpenStudies();
    for (unsigned int ind = 0; ind < List->length();ind++) {
      QString NameExistingStudy(List[ind]);
      QString NameOpeningStudy = QAD_Tools::getFileNameFromPath( name, true );
      if ( NameExistingStudy.compare( NameOpeningStudy ) == 0 ) {
	if ( QAD_MessageBox::warn2 ( this, tr("WRN_WARNING"),
				     tr("QUE_DOC_ALREADYEXIST").arg( name ),
				     tr ("BUT_YES"), tr ("BUT_NO"), QAD_YES, QAD_NO,
				     QAD_NO ) == QAD_NO )
	  {
	    putInfo ( tr("INF_READY") );
	    return;
	  }
//	else 
//	  {
//	    onLoadStudy();
//	    return;
//	  }
      }
    }

    /*
	Try to associate an application to the opened study.
	Assumed that the association study<->application is unique.
    */
    bool appFound = false;
    putInfo ( tr("INF_DOC_OPENING") + " " + name );
    for ( QAD_Application* app = myApps.first(); app; app = myApps.next() )
    {
	QString ext = QAD_Tools::getFileExtensionFromPath ( name );
	if ( app->getStudyExtension() == ext )
	{
	    /* application found */
	    if ( app->isStudyOpened( name ) )
	    {
		/* already opened: prompt for reopen */
		if ( QAD_MessageBox::warn2 ( this, tr("WRN_WARNING"),
					     tr("QUE_DOC_ALREADYOPEN").arg( name ),
					     tr ("BUT_YES"), tr ("BUT_NO"), QAD_YES, QAD_NO,
					     QAD_NO ) == QAD_NO )
		{
		    putInfo ( tr("INF_READY") );
		    return;
		}
		/* now the study is closed to be re-opened */
		//don't ask user to remove study permanently
		if (app->getStudyByName ( name ) != NULL)
		  onCloseStudy ( app->getStudyByName ( name ), false );
		else if (app->getStudyByName ( QAD_Tools::getFileNameFromPath( name, true )) != NULL)
		  onCloseStudy ( app->getStudyByName ( QAD_Tools::getFileNameFromPath( name, true )), false );
	    }
	    appFound = true;

	    /* open the study */
	    QAD_Study* openStudy = app->openStudy( name );
	    if ( !openStudy ) {
	      QAD_MessageBox::error1( this, 
				      tr("ERR_ERROR"),
				      tr("ERR_DOC_CANTOPEN") + "\n" + name,
				      tr("BUT_OK") );
	    } 
	    break;
	}
    }

    if ( !appFound )
    {
      QAD_MessageBox::error1( this, 
			      tr("ERR_ERROR"), 
			      tr("ERR_DOC_UNKNOWNTYPE"), 
			      tr("BUT_OK"));
    }
    putInfo ( tr("INF_READY") );
}

bool QAD_Desktop::loadComponentData( const QString& compName )
{
  QAD_WaitCursor wc;

  if ( compName.isEmpty() ) {
    MESSAGE("loadComponentData(): empty component name passed!")
    return false;
  }

  // Open component's data in active study if any
  MESSAGE("loadComponentData(): Opening " << compName << " component data ")
  if (!myActiveStudy) {
    MESSAGE("loadComponentData(): No active study exists")
    return false;
  }

  Engines::Component_var comp ;
  if ( compName.compare("SUPERV") == 0 ) {
    comp = getEngine( "SuperVisionContainer", compName) ;
  }
  else {
    /*    comp = ( compName.compare("Data1") != 0 ) ? getEngine( "FactoryServer", compName) :
	                                        getEngine( "FactoryServerPy", compName);
    */
    comp = getEngine( "FactoryServer", compName);
    if ( comp->_is_nil() )
      comp = getEngine( "FactoryServerPy", compName);
  }

  if ( CORBA::is_nil( comp ) ) {
    MESSAGE("loadComponentData(): Engine is null");
    return false;
  }  

  SALOMEDS::Study_var aStudy = myActiveStudy->getStudyDocument();
  SALOMEDS::SComponent_var SCO = SALOMEDS::SComponent::_narrow(aStudy->FindObject( getComponentUserName(compName) ));
	   
  if (!SCO->_is_nil()) {
    SALOMEDS::Driver_var   driver = SALOMEDS::Driver::_narrow(comp);
    if (!CORBA::is_nil(driver)) {
      SALOMEDS::StudyBuilder_var  B = aStudy->NewBuilder();
      if (!CORBA::is_nil(B)) {
	try {
	  B->LoadWith(SCO,driver);
	}
	catch( const SALOME::SALOME_Exception& ) {
	  // Oops, something went wrong while loading -> return an error
	  return false;
	}
      } 
    } else {
      MESSAGE("loadComponentData(): Driver is null");
      // Incorrect! All components should inherit SALOMEDS::Driver
      return false;
    }
  } else {
    MESSAGE("loadComponentData(): SComponent is null");
    // Don't return false here, for there might be no data 
    // for a given component in the study yet
  }

  return true;
}

/*!
    Saves the active study
*/
bool QAD_Desktop::onSaveStudy()
{
    return onSaveStudy ( myActiveStudy );
}

/*!
    Saves the given study
*/
bool QAD_Desktop::onSaveStudy( QAD_Study* study )
{
    if ( !study ) return true;

    if ( !study->isSaved() )
	return onSaveAsStudy ( study );

    /* saving study... */
    QAD_Application* app = study->getApp();
    QAD_ASSERT_DEBUG_ONLY ( app );
    putInfo ( tr("INF_DOC_SAVING") + study->getTitle() );
    if ( !app->saveStudy( study ) )
    {
	putInfo( tr("INF_CANCELLED") );
	return false;
    }

    /* saved ok */
    putInfo ( tr("INF_DOC_SAVED").arg( "" ) );
    return true;
}

/*!
    Saves the active study under a new name
*/
bool QAD_Desktop::onSaveAsStudy()
{
    return onSaveAsStudy( myActiveStudy );
}

/*!
    Saves the given study under a new name
*/
bool QAD_Desktop::onSaveAsStudy( QAD_Study* study )
{
    if ( !study ) return true;

    /* Save study in a format supported by its application
    */
    QAD_Application* app = study->getApp();
    QAD_ASSERT_DEBUG_ONLY ( app );

    /*	Select a file where to save
    */
    QString name = selectFileName ( false );
    if ( name.isNull() || name.isEmpty() )
    {
	putInfo( tr("INF_CANCELLED") );
	return false;
    }

    /*	Saving study
    */
    putInfo ( tr("INF_DOC_SAVING") + name );
    if ( !app->saveAsStudy( study, name ) ) {
      /* can't save the file */
      QAD_MessageBox::error1( this, 
			      tr("ERR_ERROR"), 
			      tr("ERR_DOC_CANTWRITE") + "\n" + name,
			      tr("BUT_OK") );
      putInfo("");
      return false;	/* cannot save */
    }
    putInfo ( tr("INF_DOC_SAVED").arg( name ) );
    return true;	/* saved ok */
}

/*!
    Closes the active study
*/
bool QAD_Desktop::onCloseStudy()
{
  bool close = this->onCloseStudy ( getActiveStudy(), true );
  return close;
}

/*!
    Closes the given study and ask user to remove study
    from the study manager permanently
*/
bool QAD_Desktop::onCloseStudy( QAD_Study* study )
{
  return onCloseStudy( study, true );
}

/*!
    Closes the given study and ask user to remove study
    from the study manager permanently if ask is equal true
*/
bool QAD_Desktop::onCloseStudy( QAD_Study* study, bool ask ) 
{
  bool removePerm = false;

  if ( ask )
    {
      SALOMEGUI_CloseDlg aDlg( this );
      int res = aDlg.exec();
      
      switch ( res )
	{
	case 1:
	  //if ( study->isModified() )
	  if ( !onSaveStudy( study ) ) {
	    putInfo( tr("INF_CANCELLED") );
	    return false;
	  }
	  removePerm = true;
	  break;
	case 2:
	  removePerm = true;
	  break;
	case 3:
	  removePerm = false;
	  break;
	case 0:
	default:
	  putInfo( tr("INF_CANCELLED") );
	  return false;
	}
    }
  /* close active component */
  if (!myXmlHandler->myIdList.IsEmpty())
    {
      deactivateComponent();
      clearMenus();
      myActiveComp="";
      myCombo->setCurrentItem (0);
      for ( QToolButton* aButton=myComponentButton.first(); aButton; aButton=myComponentButton.next() )
	aButton->setOn(false);
      qApp->processEvents();
    }
  /* close the study */
  return study->getApp()->closeStudy( study, removePerm );
}

// Displays study properties dialog box
void QAD_Desktop::onStudyProperties()
{
  if (myActiveStudy) {
    // open transaction
    QAD_Operation* op = new QAD_Operation( myActiveStudy );
    op->start();

    SALOMEGUI_StudyPropertiesDlg aDlg(this);
    int res = aDlg.exec();

    if ( res == QDialog::Accepted && aDlg.isChanged() )
      op->finish();
    else
      op->abort(); 

    // T2.12 - add the word "locked" to study frame captions if <myActiveStudy> study is locked
    myActiveStudy->updateCaptions();
  }
}


/*!
  locate all windows in cascade mode
  and resize them
*/
void QAD_Desktop::onCascade()
{
  myWorkspace->cascade();
  int w = myWorkspace->width();
  int h = myWorkspace->height();
  QWidgetList wList = myWorkspace->windowList();
  QWidgetListIt it(wList);

  for(;it.current(); ++it)
      it.current()->resize((int)(0.8*w), (int)(0.8*h));
}

// /*!
//   called when help window closed
// */
// void QAD_Desktop::onHelpWindowClosed()
// {
//   myHelpWindow = 0;
// }

/*!
    Called when 'view status bar' option
    is switched on/off.
*/
void QAD_Desktop::onViewStatusBar()
{
    bool show = myStdActions.at( ViewStatusBarId )->isOn();
    if ( show == myStatusBar->isVisible() )
	return;
    if ( show )
	myStatusBar->show();
    else
	myStatusBar->hide();
}

/*!
  Called when View/Toolbars popup menu is about to show
*/
void QAD_Desktop::onToolBarPopupAboutToShow()
{
  myToolBarAction.clear();
  myToolBarsPopup.clear();
  QPtrList<QDockWindow> wlist = dockWindows();
  for ( QDockWindow* w = wlist.first(); w; w = wlist.next() ) {
    if ( w->inherits( "QToolBar" ) ) {
      QToolBar* tb = ( QToolBar* )w;
      if ( appropriate( tb ) ) { 
	ToggleAction* a = new ToggleAction( tb->label(), tb->label(), 0, this, 0, true );
	a->setStatusTip ( tr("PRP_DESK_VIEW_TOOLBAR") );
	a->setOn( tb->isVisible() );
	QAD_ASSERT( connect( a, SIGNAL( toggledOn()  ), tb, SLOT( show() ) ) );
	QAD_ASSERT( connect( a, SIGNAL( toggledOff() ), tb, SLOT( hide() ) ) );
	a->addTo( &myToolBarsPopup );
	myToolBarAction.append( a );
      }
    }
  }
}

/*!
    Called on 'View\Selection Mode'
*/
void QAD_Desktop::onSelectionMode()
{
  const QActionP* obj = (QActionP*) sender();
  
  Selection_Mode SelectionMode = ActorSelection;

  if ( obj == myStdActions.at(SelectionActorId) ) {
    SelectionMode = ActorSelection;
    if ( obj->isOn() ) {
      myStdActions.at(SelectionCellId)->setOn(false);
      myStdActions.at(SelectionEdgeId)->setOn(false);
      myStdActions.at(SelectionPointId)->setOn(false);
    } else
      myStdActions.at(SelectionActorId)->setOn(true);

  } else if ( obj == myStdActions.at(SelectionCellId) ) {
    SelectionMode = CellSelection; 
    if ( obj->isOn() ) {
      myStdActions.at(SelectionActorId)->setOn(false);
      myStdActions.at(SelectionEdgeId)->setOn(false);
      myStdActions.at(SelectionPointId)->setOn(false);
    } else
      myStdActions.at(SelectionActorId)->setOn(true);
      
  } else if ( obj == myStdActions.at(SelectionEdgeId) ) {
    SelectionMode = EdgeOfCellSelection;    
    if ( obj->isOn() ) {
      myStdActions.at(SelectionActorId)->setOn(false);
      myStdActions.at(SelectionCellId)->setOn(false);
      myStdActions.at(SelectionPointId)->setOn(false);
    } else
      myStdActions.at(SelectionActorId)->setOn(true);
      
  } else if ( obj == myStdActions.at(SelectionPointId) ) {
    SelectionMode = NodeSelection;
    if ( obj->isOn() ) {
      myStdActions.at(SelectionEdgeId)->setOn(false);
      myStdActions.at(SelectionCellId)->setOn(false);
      myStdActions.at(SelectionActorId)->setOn(false);
    } else
      myStdActions.at(SelectionActorId)->setOn(true);
  }

  QAD_StudyFrame* sf = myActiveApp->getActiveStudy()->getActiveStudyFrame();
  sf->getRightFrame()->getViewFrame()->SetSelectionMode( SelectionMode );
  
  SALOME_Selection* Sel = SALOME_Selection::Selection( myActiveApp->getActiveStudy()->getSelection() );
  Sel->SetSelectionMode( SelectionMode );
}

/*!
    Called on 'View\Selection Mode'
*/
void QAD_Desktop::SetSelectionMode(Selection_Mode mode, bool activeCompOnly)
{
  switch (mode) {
  case NodeSelection:
    {
      myStdActions.at(SelectionEdgeId)->setOn(false);
      myStdActions.at(SelectionCellId)->setOn(false);
      myStdActions.at(SelectionActorId)->setOn(false);
      myStdActions.at(SelectionPointId)->setOn(true);
      break;
    }
  case EdgeOfCellSelection:
    {
      myStdActions.at(SelectionActorId)->setOn(false);
      myStdActions.at(SelectionCellId)->setOn(false);
      myStdActions.at(SelectionPointId)->setOn(false);
      myStdActions.at(SelectionEdgeId)->setOn(true);
      break;
    }
  case CellSelection:
  case EdgeSelection:
  case FaceSelection:
  case VolumeSelection:
   {
      myStdActions.at(SelectionActorId)->setOn(false);
      myStdActions.at(SelectionEdgeId)->setOn(false);
      myStdActions.at(SelectionPointId)->setOn(false);
      myStdActions.at(SelectionCellId)->setOn(true);
      break;
    }
  case ActorSelection:
    {
      myStdActions.at(SelectionCellId)->setOn(false);
      myStdActions.at(SelectionEdgeId)->setOn(false);
      myStdActions.at(SelectionPointId)->setOn(false);
      myStdActions.at(SelectionActorId)->setOn(true);
      break;
    }
  }

  QAD_StudyFrame* sf = myActiveApp->getActiveStudy()->getActiveStudyFrame();
  if ( sf->getTypeView() == VIEW_VTK ) {
    sf->getRightFrame()->getViewFrame()->SetSelectionMode( mode );
    SALOME_Selection* Sel = SALOME_Selection::Selection( myActiveApp->getActiveStudy()->getSelection() );
    Sel->SetSelectionMode( mode, activeCompOnly );
  } else {
  }
}

/*!
    Called on 'Window\NewWindow3d'
*/
void QAD_Desktop::onNewWindow3d()
{
  //NRI : SAL2214
  /*
    //VRV: T2.5 - add default viewer
    QApplication::restoreOverrideCursor();
    SALOMEGUI_ViewChoiceDlg *choice = new SALOMEGUI_ViewChoiceDlg(this, "", TRUE);
    int r = choice->exec();
    if (r) 
    {
    myActiveApp->getActiveStudy()->newWindow3d(QString(""),(ViewType)(choice->getSelectedViewer()));
    }
    //VRV: T2.5 - add default viewer
  */
  const QActionP* obj = (QActionP*) sender();
  if ( obj == myStdActions.at(ViewOCCId) ) {
    myActiveApp->getActiveStudy()->newWindow3d(QString(""),(ViewType)(0));
  } else if ( obj == myStdActions.at(ViewVTKId) ) {
    myActiveApp->getActiveStudy()->newWindow3d(QString(""),(ViewType)(1));
  } else if ( obj == myStdActions.at(ViewPlot2dId) ) {
    myActiveApp->getActiveStudy()->newWindow3d(QString(""),(ViewType)(3));
  }
  //NRI : SAL2214
}

// Helps to execute command
class RunBrowser: public QThread {
public:
  
  RunBrowser(QString theApp, QString theParams, QString theHelpFile, QString theContext=NULL): 
    myApp(theApp), myParams(theParams), myHelpFile("file:" + theHelpFile + theContext), myStatus(0) {};
 
  virtual void run()
  {
    QString aCommand;
    
    if ( !myApp.isEmpty())
      {
	aCommand.sprintf("%s %s %s",myApp.latin1(),myParams.latin1(),myHelpFile.latin1());
	myStatus = system(aCommand);
	if(myStatus != 0)
	  {
	    QCustomEvent* ce2000 = new QCustomEvent (2000);
	    postEvent (qApp, ce2000);
	  }
      }
    
    if( myStatus != 0 || myApp.isEmpty())
      {
	myParams = "";
	aCommand.sprintf("%s %s %s", QString(DEFAULT_BROWSER).latin1(),myParams.latin1(), myHelpFile.latin1());	
	myStatus = system(aCommand);
	if(myStatus != 0)
	  {
	    QCustomEvent* ce2001 = new QCustomEvent (2001);
	    postEvent (qApp, ce2001);
	  }
      }
  }

private:
  QString myApp;
  QString myParams;
  QString myHelpFile;
  int myStatus;
  
};

// Provide status tip for GUI help menu item

void QAD_Desktop::onHelpModulePopupStatusText(int id)
{
  int Id = myHelpPopup.idAt( 0 ); // HelpContentsModuleId
  //  MESSAGE ( "myHelpContentsModulePopup : " << id << "-" << Id)
  if (id == Id)
    putInfo("Shows the help contents of each module");
}

/*!
    Called on 'help\contents'
*/
void QAD_Desktop::onHelpContents()
{  if (myActiveComp == "")
    myActiveComp = getComponentUserName( "KERNEL" ); //NRI "Salome";

  QCString dir;
  QString root;
  QString homeDir;
  if (dir = getenv( getComponentName( myActiveComp ) + "_ROOT_DIR")) {
    root = QAD_Tools::addSlash( QAD_Tools::addSlash(dir) + QAD_Tools::addSlash("share")  + QAD_Tools::addSlash("salome")  + QAD_Tools::addSlash("doc") + "html" );
    if ( QFileInfo( root + "index.html" ).exists() ) {
      homeDir = root;
    } else {
      QMessageBox::warning( this, tr("WRN_WARNING"), 
			    QString( "%1index.html doesn't exist." ).arg(root), tr ("BUT_OK") );
      return;
    }
  }

  QString helpFile = QFileInfo( homeDir + "index.html" ).absFilePath();   
  QString anApp = QAD_CONFIG->getSetting("ExternalBrowser:Application");
  QString aParams = QAD_CONFIG->getSetting("ExternalBrowser:Parameters");
   
  RunBrowser* rs = new RunBrowser(anApp, aParams, helpFile);
  rs->start();
}

/*!
    Called on 'Module Help Reference'
*/
void QAD_Desktop::onHelpContentsModule()
{ 
  const QActionP* obj = (QActionP*) sender();
  
  QString aComponentName = obj->name();
  QString aFileName = aComponentName + "_index.html";

  QCString dir;
  QString root;
  QString homeDir;
  if (dir = getenv( aComponentName + "_ROOT_DIR")) {
    root = QAD_Tools::addSlash( QAD_Tools::addSlash(dir) +  QAD_Tools::addSlash("doc") +  QAD_Tools::addSlash("salome") );
    if ( QFileInfo( root + aFileName ).exists() ) {
      homeDir = root;
    } else {
      QMessageBox::warning( this, tr("WRN_WARNING"), 
			    QString( "%1"+ aFileName + " doesn't exist." ).arg(root), tr ("BUT_OK") );
      return;
    }
  }

  QString helpFile = QFileInfo( homeDir + aFileName ).absFilePath();   
  QString anApp = QAD_CONFIG->getSetting("ExternalBrowser:Application");
  QString aParams = QAD_CONFIG->getSetting("ExternalBrowser:Parameters");
   
  RunBrowser* rs = new RunBrowser(anApp, aParams, helpFile);
  rs->start();
}

/*!
    Called on 'help\TUI Reference'
*/

/* Commented
void QAD_Desktop::onHelpContentsTUI()
{
  if (myActiveComp == "")
    myActiveComp = getComponentUserName( "KERNEL" ); //NRI "Salome";

  QCString dir;
  QString root;
  QString homeDir;
  if (dir = getenv( getComponentName( myActiveComp ) + "_ROOT_DIR")) {
    root = QAD_Tools::addSlash( QAD_Tools::addSlash(dir) + QAD_Tools::addSlash("doc") + "html" );
    if ( QFileInfo( root + "index.html" ).exists() ) {
      homeDir = root;
    } else if (QFileInfo( root + "html/index.html" ).exists())
      homeDir = root + QAD_Tools::addSlash("html");
    else {
      QMessageBox::warning( this, tr("WRN_WARNING"), 
			    QString( "%1index.html doesn't exist." ).arg(root), tr ("BUT_OK") );
      return;
    }
  }
  
  QString helpFile = QFileInfo( homeDir + "index.html" ).absFilePath(); 
  
  QString anApp = QAD_CONFIG->getSetting("ExternalBrowser:Application");
  QString aParams = QAD_CONFIG->getSetting("ExternalBrowser:Parameters");
   
  RunBrowser* rs = new RunBrowser(anApp, aParams, helpFile);
  rs->start();
}
*/


// /*!
//     Called on 'help\search'
// */
// void QAD_Desktop::onHelpSearch()
// {
//   if (myActiveApp)
//     myActiveApp->helpSearch();
//   else
//     helpSearch();
// }

/*!
    Called on 'help\about'
*/
void QAD_Desktop::onHelpAbout()
{
  if (myActiveApp)
    myActiveApp->helpAbout();
  else
    helpAbout();
}


/*!
    Activates the application and its active study
*/
void QAD_Desktop::onActivateApp( QAD_Application* activeApp )
{
  bool appChanged = ( activeApp != myActiveApp );
  if ( appChanged )
    {
      updateDesktop( DeactivateApp );
    }
  if ( activeApp )
    {
      myActiveApp = activeApp;
      myActiveStudy = myActiveApp->getActiveStudy();
    }
  else
    {
      myActiveApp = 0;
      myActiveStudy = 0;
    }

  if ( appChanged )
    {
      updateDesktop( ActivateApp );
      updateCaption( (activeApp != NULL) ? ActivateApp : DeactivateApp );
    }
  updateActions();
}

/*!
    Called when the last study is removed from
    the application 'app'.
*/
void QAD_Desktop::onDeactivateApp(QAD_Application* app)
{
  if ( app && app == myActiveApp )
    onActivateApp( 0 );
}

/*!
    Enables/disables standard actions
*/
void QAD_Desktop::updateActions()
{
    myStdActions.at( FileCloseId )->setEnabled ( myActiveStudy != NULL );
    myStdActions.at( FileSaveId )->setEnabled ( myActiveStudy != NULL );
    myStdActions.at( FileSaveAsId )->setEnabled ( myActiveStudy != NULL );
    myStdActions.at( FilePropsId )->setEnabled( myActiveStudy != NULL );
//    myStdActions.at( HelpContentsId )->setEnabled ( myActiveApp != NULL );
//    myStdActions.at( HelpSearchId )->setEnabled ( myActiveApp != NULL );
//    myStdActions.at( HelpWhatsThisId )->setEnabled ( myActiveApp != NULL );
//    myStdActions.at( HelpAboutId )->setEnabled ( myActiveApp != NULL );

//    myToolsPopup.setEnabled ( myActiveStudy != NULL );
//    myPrefPopup.setEnabled ( myActiveStudy != NULL );

    mySelectionModePopup.setEnabled ( myActiveStudy != NULL );

    // VSR ==> SAL2982
    myStdActions.at( ViewOCCId )->setEnabled( myActiveStudy != NULL );
    myStdActions.at( ViewVTKId )->setEnabled( myActiveStudy != NULL );
    myStdActions.at( ViewPlot2dId )->setEnabled( myActiveStudy != NULL );
    // VSR <== SAL2982
}

/*!
    Called when 'Window' popup is about to show
*/
void QAD_Desktop::onWindowPopupAboutToShow()
{
    /* add cascade/tile items */
    myWindowPopup.clear();

    //NRI : SAL2214
    //myStdActions.at( WindowNew3dId )->addTo( &myWindowPopup );
    myWindowPopup.insertItem( tr("MEN_DESK_WINDOW_NEW3D"), &myNewViewPopup, WindowNew3dId );
    //myStdActions.at( WindowNew3dId )->setEnabled( true );
    myWindowPopup.setItemEnabled( WindowNew3dId, true );
    //NRI : SAL2214
    myStdActions.at( WindowCascadeId )->addTo( &myWindowPopup );
    myStdActions.at( WindowCascadeId )->setEnabled( true );
    myStdActions.at( WindowTileId )->addTo( &myWindowPopup );
    myStdActions.at( WindowTileId )->setEnabled( true );

    /* add names of opened studies */
    QWidgetList windows = myWorkspace->windowList();
    if ( windows.isEmpty() )
    {
      //NRI : SAL2214 myStdActions.at( WindowNew3dId )->setEnabled( false );
      myWindowPopup.setItemEnabled( WindowNew3dId, false );
      //NRI : SAL2214
      myStdActions.at( WindowCascadeId )->setEnabled( false );
      myStdActions.at( WindowTileId )->setEnabled( false );
    }
    else
      {
	myWindowPopup.insertSeparator();
	for ( int i = 0; i < int(windows.count()); ++i )
	  {
	    int id = myWindowPopup.insertItem( windows.at(i)->caption(),
					       this, SLOT( onWindowsPopupActivated( int ) ) );
	    myWindowPopup.setItemParameter( id, i );
	    myWindowPopup.setItemChecked( id, myWorkspace->activeWindow() == windows.at(i) );
	  }
	QAD_ASSERT( connect( &myWindowPopup, SIGNAL(highlighted( int )), this,
			     SLOT(onWindowsPopupStatusText( int )) ));
    }
}

/*!
    Called when the item with 'id' of 'Windows' popup
    is highlighted
*/
void QAD_Desktop::onWindowsPopupStatusText( int id )
{
    int cascadeId = myWindowPopup.idAt( 1 );
    int tileId = myWindowPopup.idAt( 2 );
    if ( id == cascadeId || id == tileId )
      return;
    putInfo( tr("PRP_DESK_WINDOW_ACTIVATE") );
}

/*!
    Called when user wants to activate a child	window with 'wid'
    in the main frame
*/
void QAD_Desktop::onWindowsPopupActivated( int wid )
{
    QWidget* w = myWorkspace->windowList().at( wid );
    if ( w ) w->setFocus();
}

/*!
    Selects a file name when opening or saving files.
    The basic implementation just uses the standard
    Open/Save dialogs.
    Please redefine if another behaviour is required.
*/
QString QAD_Desktop::selectFileName ( bool open )
{
  QAD_FileDlg* fd = new QAD_FileDlg( this, open, true, true );    

  QString name = "";
  QString dir  = "";
  QStringList filters;

  if (open) {
    //  All the file types of the supported applications
    //    can be opened 
    for ( QAD_Application* app = myApps.first(); app; app = myApps.next() )
      {
	filters += app->getStudyDescription() + " " + tr("DESK_FILES") + " " +
	                         "(*." + app->getStudyExtension() + ")";
      }
  }
  else {
    QAD_ASSERT_DEBUG_ONLY ( myActiveStudy );
    if (myActiveStudy) {
      // get file name of active study
      name = QAD_Tools::getFileNameFromPath ( myActiveStudy->getTitle() );
      // get path to the active study file
      dir = QAD_Tools::getDirFromPath ( myActiveStudy->getPath(), false );
    }
    QAD_ASSERT_DEBUG_ONLY ( !name.isNull() );
    // the file types supported by the active application
    filters += myActiveApp->getStudyDescription() + " " + tr("DESK_FILES") + " " +
                                 "(*." + myActiveApp->getStudyExtension() + ")" ;
  }
  if (!dir.isEmpty()) {
    // when saving document ("Save As" command) - to get dir from active study filename
    // for new documents - to use last opened directory
    fd->setDir( dir );
  }
  fd->setSelection( name );
  fd->setFilters( filters );
   
  fd->exec();
  QString filename = fd->selectedFile();
  delete fd;
  qApp->processEvents();
  return filename;
}

/*!
    Closes all the studies of 'app'. Returns
    'true' if thay are closed OK, 'false' at least
    one can't be closed or user cancel the closure.
*/
bool QAD_Desktop::closeAllStudies( QAD_Application* app )
{
    QList<QAD_Study>& studies = app->getStudies();
    while ( !studies.isEmpty() )
    {
      //don't ask user to remove study permanently
      if ( !onCloseStudy( studies.at(0), false ) )
	    return false;   /* user cancelled closing */
    }
    return true;
}

/*!
  Called on Popup item when SComponent representation in Object Browser is selected.
*/
void QAD_Desktop::onOpenWith()
{
  Engines::Component_var comp;
  QString ComponentSelected;

  SALOMEDS::SObject_var SO;
  SALOMEDS::SComponent_var SCO;
  SALOMEDS::Study_var aStudy = myActiveStudy->getStudyDocument();
  SALOMEDS::GenericAttribute_var anAttr;
  SALOMEDS::AttributeName_var aName;

  SALOME_Selection* Sel = SALOME_Selection::Selection( myActiveStudy->getSelection() );
  if ( Sel->IObjectCount() == 1 ) {
    Handle(SALOME_InteractiveObject) IO = Sel->firstIObject();
    SO = aStudy->FindObjectID( IO->getEntry() );
    SCO = SO->GetFatherComponent();
    char* name;
    if (SCO->FindAttribute(anAttr, "AttributeName")) {
      aName = SALOMEDS::AttributeName::_narrow(anAttr);
      name = aName->Value();
      if ( getComponentName( name ).isEmpty() )
	return;
      SALOME_ModuleCatalog::Acomponent_var Comp = myCatalogue->GetComponent( getComponentName(name) );
      if ( !Comp->_is_nil() ) {
	
	SALOME_ModuleCatalog::ListOfComponents_var list_type_composants =
	  myCatalogue->GetTypedComponentList( Comp->component_type() );
	
	if ( list_type_composants->length() == 1 ) {
	  ComponentSelected = name;
	} else if ( list_type_composants->length() > 1 ) {
	  SALOMEGUI_OpenWith* aDlg = new SALOMEGUI_OpenWith( this );
	  for (unsigned int ind = 0; ind < list_type_composants->length();ind++) {
	    QString compusername = getComponentUserName( (char*)list_type_composants[ind] );
	    if ( !compusername.isEmpty() )
	      aDlg->addComponent( compusername );
	  }
	  
	  int retVal = aDlg->exec();
	  ComponentSelected = aDlg->getComponent();
	  delete aDlg;
	  if ( retVal == QDialog::Rejected || ComponentSelected.isEmpty()) {
	    return;
	  }
	} else {
	  MESSAGE( "No component with type : " << Comp->component_type() << " exists" );
	  return;
	}
	
      } else {
	MESSAGE( "This component not exists in the catalog" );
	return;
      }
      
      QApplication::setOverrideCursor( Qt::waitCursor );
      Sel->ClearIObjects();
      
//      if (loadComponentData(ComponentSelected)) {
	onComboActiveComponent(ComponentSelected);
//	int nbItem = myCombo->count();
//	int Index = 0;
//	for (int i = 0; i < nbItem; i++) {
//	  if (myCombo->text(i).compare(ComponentSelected)==0) {
//	    Index = i;
//	    break;
//	  }
//	}
//	myCombo->setCurrentItem(Index);
//	myActiveStudy->updateObjBrowser(true);
//      }
    }
  }
  QApplication::restoreOverrideCursor();
}


/*!
  Called to define settings of component.
*/
void QAD_Desktop::setSettings()
{
  SALOMEGUI* anActiveGUI = getActiveGUI();
  if ( anActiveGUI ) {
    QString Component = mapComponentName[myActiveComp];
    anActiveGUI->SetSettings( this, (char*)Component.latin1() );
  }
}

/*!
  Called to load Component : xml resources and GUI librairy.
*/
bool QAD_Desktop::loadComponent(QString Component)
{
  if ( Component.isEmpty() ) {
    MESSAGE("loadComponent(): empty component name passed!")
    return false;
  }

  QAD_WaitCursor wc;
  QString resDir("/");  //NRI : Pb under Windows

  QAD_ResourceMgr* resMgr = QAD_Desktop::createResourceManager();
  if ( resMgr ) {
    QString msg;
    if (!resMgr->loadResources( Component, msg ))
      {
	//NRI	QCString errMsg;
	//	errMsg.sprintf( "Do not load all resources for module %sGUI.\n" ,
	//		Component.latin1() );
	wc.stop();
	QMessageBox::warning( this, tr("WRN_WARNING"), msg, tr ("BUT_OK") );
	return false;
      }
  }
    
  /* Parse xml file */
  myXmlHandler = new QAD_XmlHandler();
  myXmlHandler->setMainWindow(this);
  if (!myXmlHandler->setComponent(resMgr->resources( Component ))) return false;

  QString language = resMgr->language( Component );

  QString ComponentXml = Component + "_" + language + ".xml";
  //ComponentXml = resMgr->resources(Component + "GUI") ;
  //ComponentXml = QAD_Tools::addSlash(ComponentXml) ;
  ComponentXml = QAD_Tools::addSlash( resMgr->findFile( ComponentXml, Component ) ) + ComponentXml;
  QFile file( ComponentXml );

  if ( !file.exists() || !file.open( IO_ReadOnly ) )  {
    QString errMsg;
    errMsg.sprintf( "Cannot open xml file: %s\n.",
		    (file.name()).latin1() );
    wc.stop();
    QMessageBox::critical( this, tr("ERR_ERROR"), errMsg, tr ("BUT_OK") );
    return false;
  }
  file.close();

  QXmlInputSource source( file );
  QXmlSimpleReader reader;
  reader.setContentHandler( myXmlHandler );
  reader.setErrorHandler( myXmlHandler );

  bool IsMaxActStudy = myActiveStudy->getActiveStudyFrame()->isMaximized();
  if (IsMaxActStudy) {
    QString aSourceData = source.data();
    aSourceData = changeXmlInputSourceData( aSourceData, Component );
    source.setData(aSourceData);
  }

  bool ok = reader.parse( source );
  file.close();
  if ( !ok ) {
    wc.stop();
    QMessageBox::critical( this,
			   tr( "INF_PARSE_ERROR" ),
			   tr( myXmlHandler->errorProtocol() ) );
    return false;
  }

  myMenusList=myXmlHandler->myMenusList;
  myActiveMenus=myMenusList.at(0);
  myOperatorMenus->showMenuBar(0);
  myActiveMenus->showAllToolBars();

  /* Components toolbars */
  int nbToolbars = 0;
  if (myActiveMenus)
    nbToolbars = myActiveMenus->getToolBarList().count();

  /* COMPONENT INTERFACE */
  SALOME_ModuleCatalog::Acomponent_ptr aComponent =
    myCatalogue->GetComponent(Component.latin1());

  if (CORBA::is_nil (aComponent)) {
    MESSAGE( "Catalog Error : Component not found in the catalog" )
    return false;
  }

  myActiveStudy->setMessage(QString("Component : ") +
			    aComponent->componentusername() + " created " );
  myActiveStudy->setMessage(QString("Type : ") +
			    QString::number(aComponent->component_type()));
  myActiveStudy->setMessage(QString("Constraint : ") +
			    aComponent->constraint() );

  /* obtain interfaces list of the component */
  SALOME_ModuleCatalog::ListOfInterfaces_var _list =
    new SALOME_ModuleCatalog::ListOfInterfaces;
  _list = aComponent->GetInterfaceList();

  if (_list->length() != 0 ) {

    for (unsigned int ind = 0; ind < _list->length();ind++)
      myActiveStudy->setMessage( QString("Interface name : ") +
				 _list[ind] );


    /* obtain services list of the component */
    SALOME_ModuleCatalog::ListOfServices_var list_services_component =
      new SALOME_ModuleCatalog::ListOfServices;
    list_services_component = aComponent->GetServiceList(_list[0]);

    if (list_services_component->length() != 0 ) {

      for (unsigned int ind = 0; ind < list_services_component->length();ind++)
	myActiveStudy->setMessage( QString("Service name : ") +
				   list_services_component[ind] );

      // Get a service of the component
      SALOME_ModuleCatalog::Service_var service = new SALOME_ModuleCatalog::Service;
      service  = aComponent->GetService(_list[0],list_services_component[0]) ;
      myActiveStudy->setMessage( QString("Service name: ") + service->ServiceName);
      for (unsigned int ind1 = 0; ind1 <service->ServiceinParameter.length();ind1++) {
	myActiveStudy->setMessage( QString("Type of the in Parameter of this service : ") +
				   (const char *) service->ServiceinParameter[ind1].Parametertype);
	myActiveStudy->setMessage( QString("Name of the in Parameter of this service : ") +
				   (const char *) service->ServiceinParameter[ind1].Parametername);
      }
      for (unsigned int ind1 = 0; ind1 <service->ServiceoutParameter.length();ind1++) {
	myActiveStudy->setMessage( QString("Type of the out Parameter of this service : ") +
				   (const char *) service->ServiceoutParameter[ind1].Parametertype);
	myActiveStudy->setMessage( QString("Name of the out Parameter of this service : ") +
				   (const char *) service->ServiceoutParameter[ind1].Parametername);
      }
    }
  }
  return true;
}

QString QAD_Desktop::changeXmlInputSourceData(QString theData, QString theComponent) 
{
  //  MESSAGE ( " changeXmlInputSourceData : " << theComponent.latin1() )
  if ( theComponent=="SUPERV" ) {
    //Supervision main menu item
    int aItemId = 300;
    int aPosId = 3;
    QString aStrOld = createString( aItemId, aPosId );
    QString aStrNew = createString( aItemId, aPosId+1 );
    theData = theData.replace( QRegExp(aStrOld), aStrNew );
  }
  
  if ( theComponent == "VISU" ) {
    //Visualization main menu item
    int aItemId = 401;
    int aPosId = 3;
    QString aStrOld = createString( aItemId, aPosId );
    QString aStrNew = createString( aItemId, aPosId+1 );
    theData = theData.replace( QRegExp(aStrOld), aStrNew );

    //Selection main menu item
    aItemId = 41;
    aPosId = 4;
    aStrOld = createString( aItemId, aPosId );
    aStrNew = createString( aItemId, aPosId+1 );
    theData = theData.replace( QRegExp(aStrOld), aStrNew );
        
    //Representation main menu item
    aItemId = 42;
    aPosId = 5;
    aStrOld = createString( aItemId, aPosId );
    aStrNew = createString( aItemId, aPosId+1 );
    theData = theData.replace( QRegExp(aStrOld), aStrNew );
  }

  if ( theComponent == "SMESH" ) {
    //Hypotheses main menu item
    int aItemId = 50;
    int aPosId = 3;
    QString aStrOld = createString( aItemId, aPosId );
    QString aStrNew = createString( aItemId, aPosId+1 );
    theData = theData.replace( QRegExp(aStrOld), aStrNew );

    //Mesh main menu item
    aItemId = 70;
    aPosId = 4;
    aStrOld = createString( aItemId, aPosId );
    aStrNew = createString( aItemId, aPosId+1 );
    theData = theData.replace( QRegExp(aStrOld), aStrNew );

    //Controls main menu item
    aItemId = 60;
    aPosId = 5;
    aStrOld = createString( aItemId, aPosId );
    aStrNew = createString( aItemId, aPosId+1 );
    theData = theData.replace( QRegExp(aStrOld), aStrNew );

    //Modification main menu item
    aItemId = 40;
    aPosId = 6;
    aStrOld = createString( aItemId, aPosId );
    aStrNew = createString( aItemId, aPosId+1 );
    theData = theData.replace( QRegExp(aStrOld), aStrNew );

    //Numbering main menu item
//     aItemId = 80;
//     aPosId = 7;
//     aStrOld = createString( aItemId, aPosId );
//     aStrNew = createString( aItemId, aPosId+1 );
//     theData = theData.replace( QRegExp(aStrOld), aStrNew );
  }
  
  if ( theComponent == "GEOM" ) {
    //New Entity main menu item
    int aItemId = 40;
    int aPosId = 3;
    QString aStrOld = createString( aItemId, aPosId );
    QString aStrNew = createString( aItemId, aPosId+1 );
    theData = theData.replace( QRegExp(aStrOld), aStrNew );

    //Operations main menu item
    aItemId = 50;
    aPosId = 4;
    aStrOld = createString( aItemId, aPosId );
    aStrNew = createString( aItemId, aPosId+1 );
    theData = theData.replace( QRegExp(aStrOld), aStrNew );

    //Repair main menu item
    aItemId = 60;
    aPosId = 5;
    aStrOld = createString( aItemId, aPosId );
    aStrNew = createString( aItemId, aPosId+1 );
    theData = theData.replace( QRegExp(aStrOld), aStrNew );

    //Measures main menu item
    aItemId = 70;
    aPosId = 6;
    aStrOld = createString( aItemId, aPosId );
    aStrNew = createString( aItemId, aPosId+1 );
    theData = theData.replace( QRegExp(aStrOld), aStrNew );
  }

  if ( theComponent == "MED" ) {
    //MED main menu item
    int aItemId = 90;
    int aPosId = 3;
    QString aStrOld = createString( aItemId, aPosId );
    QString aStrNew = createString( aItemId, aPosId+1 );
    theData = theData.replace( QRegExp(aStrOld), aStrNew );
  }
  
  return theData;
}

typedef bool OneDim(int, QAD_Desktop*);
typedef void (*PTR_FACTORY_FUNCTION)( CORBA::ORB_var &orb, QWidget *parent, const char *name ) ;

/*!
 */
void QAD_Desktop::onDispatch()
{
  const QObject* obj = sender();

  QList<QAD_ListMenuIdAction> MenuList = myActiveMenus->getMenuList();
  QAD_ListMenuIdAction* aMenu;
  int id = -1;
  for(aMenu=MenuList.first();aMenu!=NULL;aMenu=MenuList.next()) {
    if (aMenu->getAction()==obj) {
      id = aMenu->getId();
      break;
    }
  }
  onDispatch(id);
}

/*!
 */
void QAD_Desktop::onDispatchTools(int id)
{
  /* Open Shared Library */
  OSD_SharedLibrary ToolsLibrary = OSD_SharedLibrary();

  QString ToolsLib;
  QCString dir;
  QFileInfo fileInfo ;
  bool found = false;

  char arg[256] ;
  bool libToolsGUI = true;
  if( QAD_XmlHandler::_bibmap[ id ].isEmpty() ) 
  { 
    if ( (dir = getenv("KERNEL_ROOT_DIR")) && !found ) {
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "lib" ;
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "salome" ;
      dir = QAD_Tools::addSlash(dir) ;
#ifdef WNT
      dir = dir + "libToolsGUI.dll" ;
#else
      dir = dir + "libToolsGUI.so" ;
#endif
      MESSAGE ( " GUI library = " << dir );
      fileInfo.setFile(dir) ;
      if (fileInfo.exists()) {
	ToolsLib = fileInfo.fileName() ;
	found = true;
      }
    }
  }
  else {
    libToolsGUI = false;
    SCRUTE( QAD_XmlHandler::_bibmap[ id ] ) ;
    if ( (dir = getenv("KERNEL_ROOT_DIR")) && !found ) {
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "lib" ;
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "salome" ;
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + QAD_XmlHandler::_bibmap[ id ].latin1() ;
      MESSAGE ( " GUI library = " << dir );
      fileInfo.setFile(dir) ;
      if (fileInfo.exists()) {
	ToolsLib = fileInfo.fileName() ;
	found = true;
      }
    }
  }

  ToolsLibrary.SetName(TCollection_AsciiString((char*)ToolsLib.latin1()).ToCString());
  bool ok = ToolsLibrary.DlOpen(OSD_RTLD_LAZY);
  if (ok) {
    if ( libToolsGUI ) {
      OSD_Function osdF = ToolsLibrary.DlSymb("OnGUIEvent");
      OneDim (*f1) = NULL;
      f1 = (bool (*) (int, QAD_Desktop*)) osdF;
      (*f1)(id,this);
    } else {
      ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance() ;
      ASSERT(SINGLETON_<ORB_INIT>::IsAlreadyExisting()) ;
      CORBA::ORB_var &orb = init(0 , 0 ) ;
      OSD_Function osdF = ToolsLibrary.DlSymb("OnGUIEvent");
      PTR_FACTORY_FUNCTION f1 = NULL;
      f1 = (PTR_FACTORY_FUNCTION) osdF;
      f1(orb,0,"Registry");
    }
  } else {
    QMessageBox::critical( this,
			   tr("ERR_ERROR"),
			   tr( ToolsLibrary.DlError() ) );
  }
}

/*!
 */
void QAD_Desktop::onDispatch(int id)
{
  SALOMEGUI* anActiveGUI = getActiveGUI();
  if ( anActiveGUI ) 
    anActiveGUI->OnGUIEvent(id,this);
}

/*!
  Returns the component's catalogue
*/
SALOME_ModuleCatalog::ModuleCatalog_var QAD_Desktop::getCatalogue()
{
  return myCatalogue;
}

/*!
 */
void QAD_Desktop::onComboActiveComponent( const QString & component ){
  onComboActiveComponent(component,true);
}
void QAD_Desktop::onComboActiveComponent( const QString & component, bool isLoadData)
{
  if (myActiveStudy != 0) {
    if (myActiveComp.compare(component)!=0) {
      // deactivate previous component
      deactivateComponent();

      if (!myXmlHandler->myIdList.IsEmpty()) clearMenus();
      if ( myCombo->currentText() != component )
	myCombo->setCurrentText( component );
      //NRI if (component.compare(QString("Salome"))!= 0) {
      if (component.compare( getComponentUserName( "KERNEL" ) )!= 0) {
//	QApplication::setOverrideCursor( Qt::waitCursor );
	bool isOk = ( !isLoadData || loadComponentData( getComponentName(component) ) );
	if ( !isOk ) {
	  QAD_MessageBox::error1( this, 
				 tr("ERR_ERROR"),
				 tr("ERR_COMP_DATA_NOT_LOADED").arg( component ), 
				 tr("BUT_OK") );	  
	}

	if ( !isOk || !loadComponent( getComponentName(component) ) ) {
	  myCombo->setCurrentItem (0);
	  for ( QToolButton* aButton=myComponentButton.first(); aButton; aButton=myComponentButton.next() ) {
	    aButton->setOn(false);
	  }
	  myActiveComp = "";
	  return;
	}

	myActiveComp = component;

	SALOME_Selection* oldSel = SALOME_Selection::Selection( myActiveStudy->getSelection() );

	for ( QToolButton* aButton=myComponentButton.first(); aButton; aButton=myComponentButton.next() ) {
	  if ( aButton->textLabel().compare(component)==0)
	    aButton->setOn(true);
	  else
	    aButton->setOn(false);
	}

	myActiveStudy->Selection( component );

	SALOME_Selection* Sel = SALOME_Selection::Selection( myActiveStudy->getSelection() );
	SALOME_ListIteratorOfListIO It( oldSel->StoredIObjects() );
	for(;It.More();It.Next()) {
  	  int res = Sel->AddIObject( It.Value() );

	  if ( res == -1 )
	    myActiveStudy->highlight( It.Value(), false );
	  if ( res == 0 )
	    myActiveStudy->highlight( It.Value(), true );
	  
	}

	oldSel->Clear();

	/* SETTINGS */
	// IMPORTANT: SetSettings() should be called AFTER SALOME_Selection
	// has been created for a newly activated component
	getComponentGUI(component)->SetSettings( this, (char*)getComponentName(component).latin1() );

	myActiveStudy->updateObjBrowser(true);

//	QApplication::restoreOverrideCursor();

      } else {  // component == "Salome"
	myActiveComp = "";
	//NRI	myActiveStudy->Selection( "Salome" );
	myActiveStudy->Selection( getComponentUserName( "KERNEL" ) );
	SALOME_Selection* Sel = SALOME_Selection::Selection( myActiveStudy->getSelection() );
	Sel->ClearIObjects();
	for ( QToolButton* aButton=myComponentButton.first(); aButton; aButton=myComponentButton.next() ) {
	  aButton->setOn(false);
	}
      }
    } else {
      for ( QToolButton* aButton=myComponentButton.first(); aButton; aButton=myComponentButton.next() ) {
	if ( aButton->textLabel().compare(component)==0)
	  aButton->setOn(true);
      }
    }
  } else if (component.compare(QString("Salome"))!= 0) {

      QPixmap pm;
      for ( QToolButton* aButton=myComponentButton.first(); aButton; aButton=myComponentButton.next() ) {
	if ( aButton->textLabel().compare( component ) == 0 )
	  pm = aButton->iconSet().pixmap();
      }
      SALOMEGUI_ActivateComponentDlg aDlg( this, component, pm );
      int res = aDlg.exec();
      
      switch ( res )
	{
	case 1:
	  onNewStudy();
	  onComboActiveComponent(component,true);            	  
	  break;
	case 2:
          onOpenStudy();	
	  onComboActiveComponent(component,true);
	  break;
	case 3:
	  onLoadStudy();
	  onComboActiveComponent(component,true);;
	  break;
	case 0:
	default:
	  putInfo( tr("INF_CANCELLED") );
	  myCombo->setCurrentItem (0);	
	  for ( QToolButton* aButton=myComponentButton.first(); aButton; aButton=myComponentButton.next() ) {
          aButton->setOn(false);
          // return;
	  }
	}
      }
}

/*!
 */
void QAD_Desktop::activateComponent(const QString& theName, bool isLoadData){
  
  int nbItem = myCombo->count();
  int Index = 0;

  for (int i = 0; i < nbItem; i++) {
    if (myCombo->text(i).compare(theName)==0) {
      Index = i;
      break;
    }
  }
  myCombo->setCurrentItem (Index);
  onComboActiveComponent(theName,isLoadData);
};

void QAD_Desktop::onButtonActiveComponent( )
{
  QToolButton* obj = (QToolButton*)sender();
  activateComponent(obj->textLabel());
}

/*!
  Clears All Resources of current component
 */
void QAD_Desktop::clearMenus()
{
  // san - commented as presumably obsolete
  //  onActiveStudyChanged();

  /* menus */
  myMenusList.clear();
  if (myXmlHandler) {
    int length = myXmlHandler->myIdList.Length();
    for (int k=length; k>0; k--) {
      QMenuData* parent;
      if ( menuBar()->findItem(myXmlHandler->myIdList.Value(k), &parent) )
	parent->removeItem( myXmlHandler->myIdList.Value(k) );
    }
  }

  /* toolbars */
  if (myActiveMenus) {
    int nbtoolbars = myActiveMenus->getToolBarList().count();
    for (int k=0; k<nbtoolbars; k++) {
      myActiveMenus->getToolBarList().at(k)->hide();
      myActiveMenus->getToolBarList().at(k)->clear();
    }
    myActiveMenus->clearToolBar();

    /* popups */
    QList<QAD_ListPopupMenu> PopupList = myActiveMenus->getPopupMenuList();
    QAD_ListPopupMenu* aPopup;
    for(aPopup=PopupList.first();aPopup!=NULL;aPopup=PopupList.next()){
      aPopup->getPopup()->clear();
      
    }
    myActiveMenus->clearPopupMenu();

  }
  myActiveComp = "";
}

typedef bool TwoDim1(QKeyEvent* pe, QAD_Desktop*, QAD_StudyFrame*);
/*!
  Exports Key Events in active component ( GUI Librairy )
 */
void QAD_Desktop::onKeyPress( QKeyEvent* pe )
{
  //  MESSAGE ( "QAD_Desktop::onKeyPress" )
  SALOMEGUI* anActiveGUI = getActiveGUI();
  if ( anActiveGUI ) 
    anActiveGUI->OnKeyPress(pe,this,myActiveStudy->getActiveStudyFrame());
}

typedef bool TwoDim(QMouseEvent* pe, QAD_Desktop*, QAD_StudyFrame*);
/*!
  Exports Mouse Press Events in active component ( GUI Librairy )
 */
bool QAD_Desktop::onMousePress( QMouseEvent* pe )
{
  //  MESSAGE ( "QAD_Desktop::onMousePress" )
  SALOMEGUI* anActiveGUI = getActiveGUI();
  if ( anActiveGUI ) 
    return anActiveGUI->OnMousePress(pe,this,myActiveStudy->getActiveStudyFrame());
  return false;
}

/*!
  Exports Mouse Move Events in active component ( GUI Librairy )
 */
void QAD_Desktop::onMouseMove( QMouseEvent* pe )
{
  SALOMEGUI* anActiveGUI = getActiveGUI();
  if ( anActiveGUI ) 
    anActiveGUI->OnMouseMove(pe,this,myActiveStudy->getActiveStudyFrame());
}

/*!
    Returns name of active component
*/
const QString& QAD_Desktop::getActiveComponent() const
{
  return myActiveComp;
}

SALOMEGUI* QAD_Desktop::getActiveGUI()
{
  SALOMEGUI* anActiveGUI = 0;
  if ( myComponents.find( myActiveComp ) != myComponents.end() )
    anActiveGUI = myComponents[myActiveComp];
  return anActiveGUI;
}

typedef SALOMEGUI* (*ComponentGUI)();

SALOMEGUI* QAD_Desktop::getComponentGUI( const QString& component )
{
  SALOMEGUI* aCompGUI = 0;
  if ( component.isEmpty() || getComponentName( component ).isEmpty() )
    return aCompGUI;
  
  // Load component GUI if requested for the first time
  if ( myComponents.find( component ) == myComponents.end() ) {
    OSD_SharedLibrary aSharedLibrary;
    QString ComponentLib;
    QCString libs;
    QFileInfo fileInfo ;
    QString fileString ;
    QString dir;

    QAD_WaitCursor wc;
    
    _islibso= false;

    if ( libs = getenv("LD_LIBRARY_PATH")) {
      //    MESSAGE ( " LD_LIBRARY_PATH : " << libs );
      QStringList dirList = QStringList::split( SEPARATOR, libs, false ); // skip empty entries
      for ( int i = dirList.count()-1; i >= 0; i-- ) {
	dir = dirList[ i ];
#ifdef WNT
	fileString = QAD_Tools::addSlash( dir ) + "lib" + getComponentName( component ) + "GUI.dll" ;
#else
	fileString = QAD_Tools::addSlash( dir ) + "lib" + getComponentName( component ) + "GUI.so" ;
#endif

	fileInfo.setFile(fileString) ;
	if (fileInfo.exists()) {
	  // MESSAGE( " GUI library = " << fileString.latin1() );
	  ComponentLib = fileInfo.fileName() ;
	  _islibso = true;
	  break;
	}
      }
    }

    if (!_islibso) // component GUI could be in PyQt, use generic library
    {
      MESSAGE("GUI library not found, trying generic library for PyQt GUI");
      bool found = false;
      if (dir = getenv("KERNEL_ROOT_DIR"))
      {
	dir = QAD_Tools::addSlash(dir) ;
	dir = dir + "lib" ;
	dir = QAD_Tools::addSlash(dir) ;
	dir = dir + "salome" ;
	dir = QAD_Tools::addSlash(dir) ;
#ifdef WNT
	dir = dir + "libSalomePyQtcmodule.dll" ;
#else
	dir = dir + "libSalomePyQtcmodule.so" ;
#endif
	MESSAGE ( " GUI library = " << dir );
	fileInfo.setFile(dir) ;
	if (fileInfo.exists())
	{
	  ComponentLib = fileInfo.fileName() ;
	  found = true;
	}
      }
      if ( !found )
      {
	QMessageBox::critical( this,
			      tr("ERR_ERROR"),
			      tr("ERR_LIBGUI" ).arg(component) );
	return aCompGUI;
      }
    }

    aSharedLibrary.SetName(TCollection_AsciiString((char*)ComponentLib.latin1()).ToCString());
    bool ok = aSharedLibrary.DlOpen(OSD_RTLD_LAZY);
    if (!ok) {
      wc.stop();
      QMessageBox::critical( this,
			    tr("ERR_ERROR"),
			    tr( aSharedLibrary.DlError() ) );
      return aCompGUI;
    }

    OSD_Function osdF = aSharedLibrary.DlSymb("GetComponentGUI");
    if ( osdF != NULL ) {
      ComponentGUI f1 = (SALOMEGUI* (*) ()) osdF;
      SALOMEGUI* aCompGUI = (*f1)();
      if ( aCompGUI )
	myComponents.insert( component, aCompGUI );
      else {
	wc.stop();
	QMessageBox::critical( this,
			      tr("ERR_ERROR"),
			      tr("ERR_GET_GUI_FAILED" ).arg(component) );
	return aCompGUI;
      }
    }
    else {
      wc.stop();
      QMessageBox::critical( this,
			    tr("ERR_ERROR"),
			    tr("ERR_GET_GUI_NOT_FOUND" ).arg(ComponentLib) );
      return aCompGUI;
    }
  }
  aCompGUI = myComponents[component];
  return aCompGUI;
}


/*!
    Returns name of active component
*/
QString QAD_Desktop::getComponentDataType() const
{
  using namespace SALOMEDS;
  Study_var aStudy = getActiveStudy()->getStudyDocument();
  SObject_var aSObject = aStudy->FindObject(myActiveComp.latin1());
  SComponent_var aComponent = SComponent::_narrow(aSObject);
  if(!aComponent->_is_nil()){
    CORBA::String_var aString = aComponent->ComponentDataType();
    return aString.in();
  }
  return "";
}

void QAD_Desktop::definePopup(QString & theContext,
			      QString & theParent, 
			      QString & theObject ) 
{
  SALOMEGUI* anActiveGUI = getActiveGUI();
  if ( anActiveGUI ) 
    anActiveGUI->DefinePopup(theContext, theParent, theObject);
}


/*!
    Copy popup menu [ static ]
*/
static void makePopup( QPopupMenu* popup, QPopupMenu* fromPopup, QAD_Desktop* receiver ) 
{
  // safe check
  if ( !popup || !fromPopup ) 
    return;
  // iterator through all popup items
  for ( int i = fromPopup->count()-1; i >= 0; i-- ) {
    int cmdId = fromPopup->idAt( i );
    QMenuItem* item = fromPopup->findItem( cmdId );
    if ( item ) {
      // if item is a separator
      if ( item->isSeparator() ) {
	popup->insertSeparator( 0 );
      }
      else {
	QIconSet* iconSet = item->iconSet();
	QKeySequence accel = fromPopup->accel( cmdId );

	QPopupMenu* popupChild = item->popup();
	// if item is a popup
	if ( popupChild && popupChild != fromPopup ) {
	  QPopupMenu* newPopup = new QPopupMenu( popup );
	  if ( iconSet ) 
	    popup->insertItem( *iconSet, item->text(), newPopup, cmdId, 0 );
	  else
	    popup->insertItem( item->text(), newPopup, cmdId, 0 );
	  makePopup( newPopup, popupChild, receiver );
	}
	// if item is a command
	else {
	  if ( iconSet ) 
	    popup->insertItem( *iconSet, item->text(), cmdId, 0 );
	  else
	    popup->insertItem( item->text(), cmdId, 0 );
	  popup->connectItem( cmdId, receiver, SLOT( onDispatch( int ) ) );
	}
	popup->setAccel( accel, cmdId );
      }
    }
  }
}
    
/*!
    Create popup
*/
void QAD_Desktop::createPopup(QPopupMenu* popup, const QString & theContext,
			      const QString & theParent, const QString & theObject )
{
  if ( !myActiveComp.isEmpty() && 
       getOperatorMenus()->createPopupMenu(theContext,theParent,theObject) != NULL ) {
    makePopup( popup, getOperatorMenus()->createPopupMenu(theContext,theParent,theObject)->getPopup(), this );
  }
  // IAPP Popup 
  // Should be moved to SALOMEGUI_Application::onCreatePopup()...
  if ( myActiveComp.isEmpty() ) {
    popup->removeItem(QAD_Display_Popup_ID);
    popup->removeItem(QAD_DisplayOnly_Popup_ID);
    popup->removeItem(QAD_Erase_Popup_ID);
    int id = popup->idAt(popup->count()-1); // last item
    if (id < 0 && id != -1) popup->removeItem(id); // separator

    if ( popup && theParent.compare("ObjectBrowser")==0 ) {
      if ( popup->count()>0 ) {
	popup->insertItem (tr ("MEN_OPENWITH"), this, SLOT(onOpenWith()), 0, -1, 2);
	popup->insertSeparator (3);
      }
      else {
	popup->insertItem (tr ("MEN_OPENWITH"), this, SLOT(onOpenWith()) );
      }
    } 
  }

}

void QAD_Desktop::onActiveStudyChanged()
{
  // Avoid recursive calls caused by QAD_MessageBox
  static bool isRecursion = false;
  if ( isRecursion || myAboutToClose )
    return;

  if (myActiveComp != "") {
    // Try to load active component's data in the activated study
    if ( !loadComponentData(mapComponentName[myActiveComp]) ) {
      isRecursion = true;
      QAD_MessageBox::error1( this, 
			     tr("ERR_ERROR"),
			     tr("ERR_COMP_DATA_NOT_LOADED").arg( myActiveComp ), 
			     tr("BUT_OK") );
      // Error while loading component's data -> deactivate it
      deactivateComponent();
      if (!myXmlHandler->myIdList.IsEmpty()) clearMenus();
      myCombo->setCurrentItem (0);
      for ( QToolButton* aButton=myComponentButton.first(); aButton; aButton=myComponentButton.next() ) {
	aButton->setOn(false);
      }
      myActiveComp = "";
      isRecursion = false;
      return;
    }
    else
      myActiveStudy->updateObjBrowser(true);
  }

  SALOMEGUI* anActiveGUI = getActiveGUI();
  if ( anActiveGUI ) 
    anActiveGUI->ActiveStudyChanged(this);
}

void QAD_Desktop::deactivateComponent()
{
  SALOMEGUI* anActiveGUI = getActiveGUI();
  if ( anActiveGUI ) 
    anActiveGUI->Deactivate();
}

/*!
  Custom popup ( GUI Library )
*/
void QAD_Desktop::customPopup(QPopupMenu* popup, const QString & theContext,
			      const QString & theParent, const QString & theObject)
{
  SALOMEGUI* anActiveGUI = getActiveGUI();
  if ( anActiveGUI ) 
    anActiveGUI->CustomPopup(this, popup, theContext, theParent, theObject);
}

void QAD_Desktop::onObjectBrowser()
{
  bool checkDiff = false;

  bool state  = QAD_CONFIG->getSetting("ObjectBrowser:AddColumn").compare("true") == 0;

  if ( myStdActions.at( PrefObjectBrowserEntryId )->isOn() != state )
    checkDiff = true;

  bool showEntry;
  if ( myStdActions.at( PrefObjectBrowserEntryId )->isOn() ) {
    showEntry = true;
    QAD_CONFIG->addSetting( "ObjectBrowser:AddColumn", "true");
  } else {
    showEntry = false;
    QAD_CONFIG->addSetting( "ObjectBrowser:AddColumn", "false");
  }  

  bool showValue;
  if ( myStdActions.at( PrefObjectBrowserValueId )->isOn() ) {
    showValue = true;
    QAD_CONFIG->addSetting( "ObjectBrowser:ValueColumn", "true");
  } else {
    showValue = false;
    QAD_CONFIG->addSetting( "ObjectBrowser:ValueColumn", "false");
  }  

  bool showIAPP;
  if ( myStdActions.at( PrefObjectBrowserIAPPId )->isOn() ) {
    showIAPP = true;
    QAD_CONFIG->addSetting( "ObjectBrowser:IAPP", "true");
  } else {
    showIAPP = false;
    QAD_CONFIG->addSetting( "ObjectBrowser:IAPP", "false");
  }  
  
  /* To sort created object in chrono order or reverse chrono order */
  bool showCHRONO_SORT ;
  if ( myStdActions.at( PrefObjectBrowserCHRONO_SORTId )->isOn() ) {
    showCHRONO_SORT = true;
    QAD_CONFIG->addSetting( "ObjectBrowser:ChronologicalSort", "true");
  } else {
    showCHRONO_SORT = false;
    QAD_CONFIG->addSetting( "ObjectBrowser:ChronologicalSort", "false");
  }

  /* To show or not to show UseCase browser */
  bool showUseCase;
  if ( myStdActions.at( PrefObjectBrowserShowUseCaseId )->isOn() ) {
    showUseCase = true;
    QAD_CONFIG->addSetting( "ObjectBrowser:ShowUseCaseBrowser", "true");
  } else {
    showUseCase = false;
    QAD_CONFIG->addSetting( "ObjectBrowser:ShowUseCaseBrowser", "false");
  }

  /* Resize or don't resize columns automatically */
  bool autoSize;
  if ( myStdActions.at( PrefObjectBrowserNoAutoSizeColumnsId )->isOn() ) {
    autoSize = false;
    QAD_CONFIG->addSetting( "ObjectBrowser:NoAutoSizeColumns", "true");
  } else {
    autoSize = true;
    QAD_CONFIG->addSetting( "ObjectBrowser:NoAutoSizeColumns", "false");
  }

  if ( myActiveApp ) {
    QList<QAD_Study>& studies = myActiveApp->getStudies();
    for ( QAD_Study* study = studies.first(); study; study = studies.next() )  {
      int nbSf = study->getStudyFramesCount();
      for ( int i = 0; i < nbSf; i++ ) {
	QAD_StudyFrame* sf = study->getStudyFrame(i);
	sf->getLeftFrame()->getObjectBrowser()->setShowInfoColumns( showEntry );
	sf->getLeftFrame()->getObjectBrowser()->setShowValueColumn( showValue );
	sf->getLeftFrame()->getObjectBrowser()->setEnableChronoSort( showCHRONO_SORT );
//	sf->getLeftFrame()->getObjectBrowser()->setShowIAPP( showIAPP ); // this is done by below updateObjBrowser() call
	sf->getLeftFrame()->getObjectBrowser()->showUseCaseBrowser( showUseCase );
	sf->getLeftFrame()->getObjectBrowser()->autoSizeColumns( autoSize );
      }
      study->updateObjBrowser(true);
    }
  }
}

void QAD_Desktop::onViewerTrihedron()
{
  QString Size = QAD_CONFIG->getSetting("Viewer:TrihedronSize");
  float dim;

  SALOMEGUI_TrihedronSizeDlg *Dlg = new SALOMEGUI_TrihedronSizeDlg(this);
  if (!Size.isEmpty())
    Dlg->setTrihedronSize(Size.toInt());
  int r = Dlg->exec();
  dim = Dlg->getTrihedronSize();
  delete Dlg;
  
  if (r == QDialog::Accepted) {
    QAD_CONFIG->addSetting("Viewer:TrihedronSize", dim);

    if( myActiveApp ) {
      QList<QAD_Study>& studies = myActiveApp->getStudies();
      for ( QAD_Study* study = studies.first(); study; study = studies.next() )  {
	int nbSf = study->getStudyFramesCount();
	for ( int i = 0; i < nbSf; i++ ) {
	  study->getStudyFrame(i)->getRightFrame()->getViewFrame()->onAdjustTrihedron();
	}
      }
    }
  }
}

void QAD_Desktop::onExternalBrowser()
{
 
  QString theApp = QAD_CONFIG->getSetting("ExternalBrowser:Application");
  QString theParams = QAD_CONFIG->getSetting("ExternalBrowser:Parameters");
   
  SALOMEGUI_ExternalBrowserDlg *Dlg = new SALOMEGUI_ExternalBrowserDlg(this);
  
  if (!theApp.isEmpty())
    {
      QString theParams = QAD_CONFIG->getSetting("ExternalBrowser:Parameters");
      Dlg->setSettings(theApp, theParams);
    }
  int r = Dlg->exec();
  QString theAppFromDialog = Dlg->getApp();
  QString theParamsFromDialog = Dlg->getParams();
  delete Dlg;
  
  if (r == QDialog::Accepted) 
    {
      QAD_CONFIG->addSetting("ExternalBrowser:Application", theAppFromDialog );
      QAD_CONFIG->addSetting("ExternalBrowser:Parameters", theParamsFromDialog );
    }
  
}


void QAD_Desktop::onDirList() 
{
  // getting dir list from settings
  QStringList dirList = QStringList::split(';', QAD_CONFIG->getSetting("FileDlg:QuickDirList"), false);
  QAD_DirListDlg* dlg = new QAD_DirListDlg(this);
  dlg->setPathList(dirList);
  if ( dlg->exec() == QDialog::Accepted) {
    dlg->getPathList(dirList);
    QAD_CONFIG->addSetting("FileDlg:QuickDirList", dirList.join(";"));
  }
  delete dlg;
}

void QAD_Desktop::onConsoleFontAction()
{
  // getting font from settings
  QString fntSet = QAD_CONFIG->getSetting("Viewer:ConsoleFont");
  QFont font = QAD_Tools::stringToFont( fntSet );
  bool bOk;
  font = QFontDialog::getFont( &bOk, font, this );
  if ( bOk ) {
    fntSet = QAD_Tools::fontToString( font );
    QAD_CONFIG->addSetting("Viewer:ConsoleFont", fntSet );

    if( myActiveApp ) {
      QList<QAD_Study>& studies = myActiveApp->getStudies();
      for ( QAD_Study* study = studies.first(); study; study = studies.next() )  {
	int nbSf = study->getStudyFramesCount();
	for ( int i = 0; i < nbSf; i++ ) {
	  study->getStudyFrame(i)->getRightFrame()->getPyEditor()->setFont( font );
	  study->getStudyFrame(i)->getRightFrame()->getMessage()->setFont( font );
	}
      }
    }
  }
}

void QAD_Desktop::onSavePref()
{
  QAD_CONFIG->createConfigFile(true);
}

//VRV: T2.5 - add default viewer
void QAD_Desktop::onDefaultViewer(QActionP * theAction)
{
  int type = VIEW_TYPE_MAX;
  
  QString myValue;
  if (myStdActions.at(DefaultViewerOCCId) == theAction)
    type = VIEW_OCC;
  else if (myStdActions.at(DefaultViewerVTKId) == theAction)
    type = VIEW_VTK;
  else if (myStdActions.at(DefaultGraphSupervisorId) == theAction)
    type = VIEW_GRAPHSUPERV;
  else if (myStdActions.at(DefaultPlot2dId) == theAction)
    type = VIEW_PLOT2D;
  if ( type != VIEW_TYPE_MAX )
    QAD_CONFIG->addSetting( "Viewer:DefaultViewer", QString::number( type ) );
}
//VRV: T2.5 - add default viewer

void QAD_Desktop::onViewerOCC()
{
  QString BackgroundColorRed   = QAD_CONFIG->getSetting("OCCViewer:BackgroundColorRed");
  QString BackgroundColorGreen = QAD_CONFIG->getSetting("OCCViewer:BackgroundColorGreen");
  QString BackgroundColorBlue  = QAD_CONFIG->getSetting("OCCViewer:BackgroundColorBlue");
  QColor color;

  if( !BackgroundColorRed.isEmpty() && !BackgroundColorGreen.isEmpty() && !BackgroundColorBlue.isEmpty() )
    color = QColor(BackgroundColorRed.toInt(),
		   BackgroundColorGreen.toInt(),
		   BackgroundColorBlue.toInt());
  else
    color = QColor(35, 136, 145);

  QColor c = QColorDialog::getColor( color, QAD_Application::getDesktop() );

  if (c.isValid()) {
    /* VSR : PAL5420 ---------------------------------------------------
    if ( myActiveApp ) {
      QList<QAD_Study>& studies = myActiveApp->getStudies();
      for ( QAD_Study* study = studies.first(); study; study = studies.next() )  {
	int nbSf = study->getStudyFramesCount();
	for ( int i = 0; i < nbSf; i++ ) {
	  QAD_StudyFrame* sf = study->getStudyFrame(i);
	  if ( sf->getTypeView() == VIEW_OCC ) {
	    sf->getRightFrame()->getViewFrame()->setBackgroundColor( c );
	  }
	}
      }
    }
    VSR : PAL5420 --------------------------------------------------- */
    QAD_CONFIG->addSetting( "OCCViewer:BackgroundColorRed",   c.red() );
    QAD_CONFIG->addSetting( "OCCViewer:BackgroundColorGreen", c.green() );
    QAD_CONFIG->addSetting( "OCCViewer:BackgroundColorBlue",  c.blue() );
  }
}

void QAD_Desktop::onGraphSupervisor()
{
  QString BackgroundColorRed   = QAD_CONFIG->getSetting("SUPERVGraph:BackgroundColorRed");
  QString BackgroundColorGreen = QAD_CONFIG->getSetting("SUPERVGraph:BackgroundColorGreen");
  QString BackgroundColorBlue  = QAD_CONFIG->getSetting("SUPERVGraph:BackgroundColorBlue");
  QColor color;

  if( !BackgroundColorRed.isEmpty() && !BackgroundColorGreen.isEmpty() && !BackgroundColorBlue.isEmpty() )
    color = QColor(BackgroundColorRed.toInt(),
		   BackgroundColorGreen.toInt(),
		   BackgroundColorBlue.toInt());
  else
    color = QColor(35, 136, 145);

  QColor c = QColorDialog::getColor( color, QAD_Application::getDesktop() );

  if (c.isValid()) {
    /* VSR : PAL5420 ---------------------------------------------------
    if ( myActiveApp ) {
      QList<QAD_Study>& studies = myActiveApp->getStudies();
      for ( QAD_Study* study = studies.first(); study; study = studies.next() )  {
	int nbSf = study->getStudyFramesCount();
	for ( int i = 0; i < nbSf; i++ ) {
	  QAD_StudyFrame* sf = study->getStudyFrame(i);
	  if ( sf->getTypeView() == VIEW_GRAPHSUPERV ) {
	    sf->getRightFrame()->getViewFrame()->setBackgroundColor( c );
	  }
	}
      }
    }
    VSR : PAL5420 --------------------------------------------------- */
    QAD_CONFIG->addSetting( "SUPERVGraph:BackgroundColorRed",   c.red() );
    QAD_CONFIG->addSetting( "SUPERVGraph:BackgroundColorGreen", c.green() );
    QAD_CONFIG->addSetting( "SUPERVGraph:BackgroundColorBlue",  c.blue() );
  }
}

void QAD_Desktop::onViewerVTK()
{
  QString BackgroundColorRed   = QAD_CONFIG->getSetting("VTKViewer:BackgroundColorRed");
  QString BackgroundColorGreen = QAD_CONFIG->getSetting("VTKViewer:BackgroundColorGreen");
  QString BackgroundColorBlue  = QAD_CONFIG->getSetting("VTKViewer:BackgroundColorBlue");
  QColor color;

  if( !BackgroundColorRed.isEmpty() && !BackgroundColorGreen.isEmpty() && !BackgroundColorBlue.isEmpty() )
    color = QColor(BackgroundColorRed.toInt(),
		   BackgroundColorGreen.toInt(),
		   BackgroundColorBlue.toInt());
  else
    color = QColor(0, 0, 0);

  QColor c = QColorDialog::getColor( color, QAD_Application::getDesktop() );

  if (c.isValid()) {
    /* VSR : PAL5420 ---------------------------------------------------
    if ( myActiveApp ) {
      QList<QAD_Study>& studies = myActiveApp->getStudies();
      for ( QAD_Study* study = studies.first(); study; study = studies.next() )  {
	int nbSf = study->getStudyFramesCount();
	for ( int i = 0; i < nbSf; i++ ) {
	  QAD_StudyFrame* sf = study->getStudyFrame(i);
	  if ( sf->getTypeView() == VIEW_VTK ) {
	    sf->getRightFrame()->getViewFrame()->setBackgroundColor( c );
	  }
	}
      }
    }
    VSR : PAL5420 --------------------------------------------------- */
    QAD_CONFIG->addSetting( "VTKViewer:BackgroundColorRed",   c.red() );
    QAD_CONFIG->addSetting( "VTKViewer:BackgroundColorGreen", c.green() );
    QAD_CONFIG->addSetting( "VTKViewer:BackgroundColorBlue",  c.blue() );
  }
}

void QAD_Desktop::onPlot2d()
{
  QColor color;
  if ( QAD_CONFIG->hasSetting( "Plot2d:Background" ) ) {
    QString bgString = QAD_CONFIG->getSetting( "Plot2d:Background" );                               
    QStringList bgData = QStringList::split( ":", bgString, true );
    int bgRed = 0, bgGreen = 0, bgBlue = 0;
    if ( bgData.count() > 0 ) bgRed   = bgData[ 0 ].toInt();
    if ( bgData.count() > 1 ) bgGreen = bgData[ 1 ].toInt();
    if ( bgData.count() > 2 ) bgBlue  = bgData[ 2 ].toInt();
    color = QColor( bgRed, bgGreen, bgBlue );
  }
  else {
    color = QColor(255, 255, 255);  
  }

  color = QColorDialog::getColor( color, QAD_Application::getDesktop() );

  if ( color.isValid() ) {
    /* VSR : PAL5420 ---------------------------------------------------
    if ( myActiveApp ) {
      QList<QAD_Study>& studies = myActiveApp->getStudies();
      for ( QAD_Study* study = studies.first(); study; study = studies.next() )  {
	int nbSf = study->getStudyFramesCount();
	for ( int i = 0; i < nbSf; i++ ) {
	  QAD_StudyFrame* sf = study->getStudyFrame(i);
	  if ( sf->getTypeView() == VIEW_PLOT2D ) {
	    sf->getRightFrame()->getViewFrame()->setBackgroundColor( color );
	  }
	}
      }
    }
    VSR : PAL5420 --------------------------------------------------- */
    QStringList bgData; 
    bgData.append( QString::number( color.red() ) );
    bgData.append( QString::number( color.green() ) );
    bgData.append( QString::number( color.blue() ) );
    QAD_CONFIG->addSetting( "Plot2d:Background", bgData.join( ":" ) );
  }
}

/* Help About */
void QAD_Desktop::helpAbout()
{
  SALOMEGUI_AboutDlg* About = new SALOMEGUI_AboutDlg( QAD_Application::getDesktop(), tr("MEN_ABOUT"));
  About->exec();
  delete About;
}

// /* Help Search */
// void QAD_Desktop::helpSearch()
// {
// }

// /* Help Contents */
// void QAD_Desktop::helpContents()
// {
//   if (myActiveComp == "")
//     myActiveComp = getComponentUserName( "KERNEL" ); //NRI "Salome";

//   QCString dir;
//   QString root;
//   if (dir = getenv( getComponentName( myActiveComp ) + "_ROOT_DIR")) {
//     root = QAD_Tools::addSlash( QAD_Tools::addSlash(dir) + QAD_Tools::addSlash("share")  + QAD_Tools::addSlash("salome")  + "doc" );
//     if ( QFileInfo( root + "index.html" ).exists() ) {
//       helpContext( root + "index.html", "" );
//     }
//     else {
//       root = QAD_Tools::addSlash( root + "html" );
//       if ( QFileInfo( root + "index.html" ).exists() ) {
// 	helpContext( root + "index.html", "" );
//       }
//       else {
// 	root = QAD_Tools::addSlash( root + "html" );
// 	if ( QFileInfo( root + "index.html" ).exists() ) {
// 	  helpContext( root + "index.html", "" );
// 	}
//       }
//     }
//   }
  
// //   //NRI getHelpWindow()->contents();
// //   getHelpWindow()->show();
// //   getHelpWindow()->raise();
// //   getHelpWindow()->setActiveWindow();
// }

/* Help Context */
//void QAD_Desktop::helpContext(const QString& source, const QString& context)
//{
//  //getHelpWindow()->context(source, context);   //implemented in QAD_HelpWindow::context( const QString& _source, const QString& _context)
//  //getHelpWindow()->show();                     //from QMainWindow class
//  //getHelpWindow()->raise();                    //from QMainWindow class
//  //getHelpWindow()->setActiveWindow();          //from QMainWindow class

//  QString anApp = QAD_CONFIG->getSetting("ExternalBrowser:Application");
//  QString aParams = QAD_CONFIG->getSetting("ExternalBrowser:Parameters");
   
//  RunBrowser* rs = new RunBrowser(anApp, aParams, source, context);
//  rs->start();
//}

/* Preferences/MultiFile Save */
void QAD_Desktop::onMultiFileSave()
{
  if ( myStdActions.at( PrefMultiFileSave )->isOn() )
    QAD_CONFIG->addSetting( "Desktop:MultiFileSave", "true");
  else
    QAD_CONFIG->addSetting( "Desktop:MultiFileSave", "false");
}

/* Preferences/ASCII Save */
void QAD_Desktop::onASCIISave()
{
  if ( myStdActions.at( PrefASCIISave )->isOn() )
    QAD_CONFIG->addSetting( "Desktop:ASCIISave", "true");
  else
    QAD_CONFIG->addSetting( "Desktop:ASCIISave", "false");
}

/* Preferences / Undo Level */
void QAD_Desktop::onUndoLevel()
{
  static int MAX_UNDO_LEVEL = 1000;
  bool isAccepted = false;
  static QString aLabel = QString("Level value (%1 ... %2) : ").
    arg(1).arg(MAX_UNDO_LEVEL);
  int anUndoLevel = 
    SALOMEGUI_SetValueDlg::getInteger("Undo Level",aLabel,
				      1,MAX_UNDO_LEVEL,getUndoLevel(),
				      &isAccepted);
  if(!isAccepted) return;
  QAD_CONFIG->addSetting("Desktop:UndoLevel", anUndoLevel);
  if(!myActiveApp) return;
  QList<QAD_Study>& studies = myActiveApp->getStudies();
  int aWasWarning = 0;
  for(QAD_Study* study = studies.first(); study; study = studies.next()){
    SALOMEDS::Study_var aStudyDoc = study->getStudyDocument();
    SALOMEDS::StudyBuilder_var aStudyBuilder = aStudyDoc->NewBuilder();
    if (!aStudyDoc->GetProperties()->IsLocked()) {
      aStudyBuilder->UndoLimit(anUndoLevel);
    } else  {
      if (!aWasWarning) {
	QAD_MessageBox::warn1 ((QWidget*)QAD_Application::getDesktop(),
			       QObject::tr("WRN_WARNING"), 
			       QObject::tr("WRN_STUDY_LOCKED"),
			       QObject::tr("BUT_OK"));
	aWasWarning = 1;
      }
    }
  }
}

/* Update status bar for File menu items */
void QAD_Desktop::onFilePopupStatusText( int id )
{
  QString component = getActiveComponent();
  
  if (component != "") {
    //one of the modules is active now
    int importId = myFilePopup.idAt(8);
    int exportId = myFilePopup.idAt(9);
    int importTableId = myFilePopup.idAt(10);
   
    if (component == getComponentUserName("SMESH") || component == getComponentUserName("GEOM")) {
      if (id == importId)
	putInfo( tr("PRP_DESK_FILE_IMPORT") );
      if (id == exportId)
	putInfo( tr("PRP_DESK_FILE_EXPORT") );
    }
    if (component == getComponentUserName("VISU")) {
      if (id == importId)
	putInfo( tr("PRP_DESK_FILE_IMPORTMED") );
      if (id == importTableId)
	putInfo( tr("PRP_DESK_FILE_IMPORTTABLE") );
      if (id == exportId)
	putInfo( tr("PRP_DESK_FILE_EXPLOREMEDFILE") );
    }
    if (component == getComponentUserName("SUPERV")) {
      if (id == importId)
	putInfo( tr("PRP_DESK_FILE_IMPORTDF") );
      if (id == exportId)
	putInfo( tr("PRP_DESK_FILE_EXPORTDF") );
    }
  }
}

/* Update status bar for Edit menu items */
void QAD_Desktop::onEditPopupStatusText( int id )
{
  QString component = getActiveComponent();
  
  if (component != "") {
    //one of the modules is active now
   
    if (component == getComponentUserName("SMESH") || component == getComponentUserName("GEOM") ) {
      int deleteId = myEditPopup.idAt(5);
      if (id == deleteId)
	putInfo( tr("PRP_DESK_EDIT_DELETE") );
    }
    if (component == getComponentUserName("SUPERV") ) {
      int newDFId = myEditPopup.idAt(5);
      int modifyDFId = myEditPopup.idAt(6);
      if (id == newDFId)
	putInfo( tr("PRP_DESK_EDIT_NEWDF") );
      if (id == modifyDFId)
	putInfo( tr("PRP_DESK_EDIT_MODIFYDF") );
    }
  }
}

/* Update status bar for View menu items */
void QAD_Desktop::onViewPopupStatusText( int id )
{
  QString component = getActiveComponent();
  
  if (component != "") {
    //one of the modules is active now
    int DispModeId = myViewPopup.idAt(2);
    
    if (component == getComponentUserName("GEOM")) {
      if (id == DispModeId)
	putInfo( tr("PRP_DESK_VIEW_DISPLAYMODE") );
    }
    if (component == getComponentUserName("SMESH")) {
      int updateId = myViewPopup.idAt(3);

      if (id == DispModeId)
	putInfo( tr("PRP_DESK_VIEW_DISPLAYMODE") );
      if (id == updateId)
	putInfo( tr("PRP_DESK_VIEW_UPDATE") );
    }
  }
}

/*********************************************************************
** Class: AppSelectionDlg
** Descr: Dialog for the selection of the application when several
**	  applications exist in the desktop and new study creation
**	  is activated by user.
** Level: Internal
**********************************************************************/
/*!
  \class Desktop_AppSelectionDlg QAD_Desktop.h
  \brief Dialog for the selection of the application when several
  applications exist in the desktop and new study creation
  is activated by user.

  Level: Internal.
*/

Desktop_AppSelectionDlg::Desktop_AppSelectionDlg( QAD_Desktop* desktop,
						  QList<QAD_Application>& apps ) :
QDialog( desktop, 0, true ),
myAppId( 0 ),
myApps( apps )
{
    QAD_ASSERT_DEBUG_ONLY ( desktop );
    setCaption( tr("INF_DESK_DOC_CREATE") );
    if ( desktop->icon() )
    setIcon( *desktop->icon() );

    QBoxLayout* mainLayout = new QVBoxLayout( this, 5 );

    /* Create selection buttons */
    QVButtonGroup* buttonGrp = new QVButtonGroup ( this );
    buttonGrp->setExclusive ( true );
    QBoxLayout* vLayout = new QVBoxLayout( mainLayout, 5 );
    vLayout->addWidget ( buttonGrp );

    /* Create selection button for available app */
    unsigned id = 0;
    for ( QAD_Application* app = myApps.first(); app; app = myApps.next(), id++ )
    {
	/* add radio button for application */
	buttonGrp->insert( new QRadioButton( app->getStudyDescription() +
			   " (*." + app->getStudyExtension() + ")", buttonGrp ), id );
    }

    /* make the first application selected */
    buttonGrp->setButton( 0 );

    /* listen to the selection */
    QAD_ASSERT ( connect ( buttonGrp, SIGNAL(clicked(int)), this, SLOT(onAppSelected(int)) ));

    /* Create OK/Cancel buttons
    */
    QBoxLayout* hLayout = new QHBoxLayout( mainLayout, 5 );
    QPushButton* buttonOK = new QPushButton( tr("BUT_OK"), this );
    QPushButton* buttonCancel = new QPushButton( tr("BUT_CANCEL"), this );
    QPushButton* buttonHelp = new QPushButton( tr("BUT_HELP"), this );
    buttonOK->setDefault( true );

    hLayout->addStretch();
    hLayout->addWidget( buttonOK );
    hLayout->addWidget( buttonCancel );
    hLayout->addWidget( buttonHelp );

    /* connect buttons */
    QAD_ASSERT ( connect( buttonOK, SIGNAL(clicked()), this, SLOT(accept())) );
    QAD_ASSERT ( connect( buttonCancel, SIGNAL(clicked()), this, SLOT(reject())) );
    QAD_ASSERT ( connect( buttonHelp, SIGNAL(clicked()), this, SLOT(onHelp())) );

    /* disable resizing */
    setFixedSize( 0, 0 );
    setSizePolicy( QSizePolicy ( QSizePolicy::Fixed, QSizePolicy::Fixed ) );
}

Desktop_AppSelectionDlg::~Desktop_AppSelectionDlg()
{
}

QAD_Application* Desktop_AppSelectionDlg::selectedApp() const
{
    return myApps.at( myAppId );
}

void Desktop_AppSelectionDlg::resizeEvent( QResizeEvent* e )
{
    QAD_Tools::centerWidget( this, parentWidget() );
}

/*
 *  Slots
*/
void Desktop_AppSelectionDlg::onAppSelected( int id )
{
    myAppId = id;
}

void Desktop_AppSelectionDlg::onHelp()
{
}

