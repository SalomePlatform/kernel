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
//  File   : QAD_Application.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

/*!
  \class QAD_Application QAD_Application.h
  \brief Study manager for QAD-based application.
*/

#include "QAD.h"
#include "QAD_Tools.h"
#include "QAD_Desktop.h"
#include "QAD_Application.h"
#include "QAD_MessageBox.h"
#include "QAD_Config.h"
#include "SALOMEGUI_AboutDlg.h"
#include "SALOME_Selection.h"

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_Exception)

// QT Includes
#include <qapplication.h>
#include <qtranslator.h>
#include <qpopupmenu.h>
#include <qstatusbar.h>
#include <qmenubar.h>
#include <qtoolbar.h>
#include <qdialog.h>
#include <qstring.h>
#include <qpixmap.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qmessagebox.h> 

// Open CASCADE Includes
#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>
#include <Standard_Failure.hxx>
using namespace std;

static bool checkPermission(QString fileName) {
  if ( QFile::exists( fileName ) ) {
    // if file exists - check it for writeability
    if ( !QFileInfo( fileName ).isWritable() ) {
      return false; 
    }
  }
  else {
    // if file doesn't exist - try to create it
    QFile qf( fileName );
    if ( !qf.open( IO_WriteOnly ) ) {
      return false;
    }
    else {
      qf.close();
      qf.remove();
    }
  }
  return true;
}

QAD_Desktop* QAD_Application::desktop = 0;

/*!
    Registers a new application object and
    adds it to the desktop [ static ]   
*/
bool QAD_Application::addToDesktop( QAD_Application* app, SALOME_NamingService* name_service )
{
    /* create CAF application */ 
    if ( !app->initApp(name_service) )
       return false;
  
    /* create the only desktop */
    if ( !app->getDesktop() ) 
    {
        app->createDesktop(name_service); 
        emit app->desktopCreated(); 
    }

    /* add application to the desktop */
    desktop->addApplication( app );
    return true;
}

/*! 
    Runs the application [ static ]
*/
void QAD_Application::run()
{
    /* at least one application must be on desktop */
    QAD_ASSERT_DEBUG_ONLY ( desktop );
    desktop->showDesktop();
}


/*!
    Returns reference to main desktop [ static ]
*/
QAD_Desktop* QAD_Application::getDesktop()
{
  return desktop;
}

/*!
    Returns standard Palette [ static ]
*/
QPalette QAD_Application::getPalette(bool alternative)
{
/*  $$$ ---> commented 20.09.2002 - application uses global palette but little changed if <alternative> is true
    QPalette pal;
    QColorGroup cg;
    cg.setColor( QColorGroup::Foreground, Qt::black );
    cg.setColor( QColorGroup::Button, QColor( 192, 192, 192) );
    cg.setColor( QColorGroup::Light, Qt::white );
    cg.setColor( QColorGroup::Midlight, QColor( 223, 223, 223) );
    cg.setColor( QColorGroup::Dark, QColor( 96, 96, 96) );
    cg.setColor( QColorGroup::Mid, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Text, Qt::black );
    cg.setColor( QColorGroup::BrightText, Qt::white );
    cg.setColor( QColorGroup::ButtonText, Qt::black );
    cg.setColor( QColorGroup::Base, QColor( 255,255,220 )  ); 
    cg.setColor( QColorGroup::Background, QColor( 192, 192, 192) );
    cg.setColor( QColorGroup::Shadow, Qt::black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, Qt::white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, Qt::black );
    cg.setColor( QColorGroup::Button, QColor( 192, 192, 192) );
    cg.setColor( QColorGroup::Light, Qt::white );
    cg.setColor( QColorGroup::Midlight, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Dark, QColor( 96, 96, 96) );
    cg.setColor( QColorGroup::Mid, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Text, Qt::black );
    cg.setColor( QColorGroup::BrightText, Qt::white );
    cg.setColor( QColorGroup::ButtonText, Qt::black );
    cg.setColor( QColorGroup::Base, QColor( 255,255,220 ) );
    cg.setColor( QColorGroup::Background, QColor( 192, 192, 192) );
    cg.setColor( QColorGroup::Shadow, Qt::black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, Qt::white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 192, 192, 192) );
    cg.setColor( QColorGroup::Light, Qt::white );
    cg.setColor( QColorGroup::Midlight, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Dark, QColor( 96, 96, 96) );
    cg.setColor( QColorGroup::Mid, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Text, Qt::black );
    cg.setColor( QColorGroup::BrightText, Qt::white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, QColor( 255,255,220 ) );
    cg.setColor( QColorGroup::Background, QColor( 192, 192, 192) );
    cg.setColor( QColorGroup::Shadow, Qt::black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, Qt::white );
    pal.setDisabled( cg );
 $$$ <--------------------------------------------------------------------- */
  QPalette pal = QApplication::palette();
  // $$$ --> this prevents qt bug - bag child window's icon drawing (not transparent)
  // when it is maximized
  pal.setColor(QPalette::Active,   QColorGroup::Background, pal.active().button());
  pal.setColor(QPalette::Inactive, QColorGroup::Background, pal.inactive().button());
  pal.setColor(QPalette::Disabled, QColorGroup::Background, pal.disabled().button());
  // $$$ <---
  if (alternative) {
    // alternative palette is used for Object Browser, PyEditor, Message and Help windows
    QColorGroup cg = pal.active();
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, Qt::white );
    cg.setColor( QColorGroup::Base, QColor( 255,255,220 )  ); 
    cg.setColor( QColorGroup::Text, Qt::black );
    pal.setActive  ( cg );
    cg = pal.inactive();
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, Qt::white );
    cg.setColor( QColorGroup::Base, QColor( 255,255,220 )  ); 
    cg.setColor( QColorGroup::Text, Qt::black );
    pal.setInactive( cg );
    cg = pal.disabled();
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, Qt::white );
    cg.setColor( QColorGroup::Base, QColor( 255,255,220 )  ); 
    cg.setColor( QColorGroup::Text, Qt::black );
    pal.setDisabled( cg );
  }
  return pal; 
}

/*!
    Constructor
*/
QAD_Application::QAD_Application( const QString& format, const QString& description, 
                                  const QString& extension ) :
myActiveStudy( 0 ),
myStudyFormat( format ),
myStudyExtension( extension ),
myStudyDescription( description )
{        
  /* actions are stored in vectors only */
  
  /* studies are managed by me */
  myStudies.setAutoDelete( true );   
  
  /* load resources for all QAD */
  QAD_ResourceMgr* resMgr = QAD_Desktop::getResourceManager();
  if ( !resMgr ) resMgr = QAD_Desktop::createResourceManager();
  QString message;
  resMgr->loadResources( "QAD", message );    
  resMgr->loadResources( "CLIENT", message );    
  
  /* set default icon */
  myIcon = resMgr->loadPixmap( "CLIENT", tr("ICON_APP_DEFAULTICON") );
  myName = tr("APP_DEFAULTTITLE");        
  
  /*  We need to know that desktop is created in order
      to have some additional internal initialization */
  QAD_ASSERT( connect( this, SIGNAL(desktopCreated()), this, 
		       SLOT(onDesktopCreated()) ));
}   

/*!
    Destructor
*/
QAD_Application::~QAD_Application ()
{
  myEditActions.clear();
  myViewActions.clear();
  myHelpActions.clear();        
  QMap<int, QToolBar*>::Iterator it;
  for ( it = myToolBars.begin(); it != myToolBars.end(); it++ ) {
    delete it.data();
  }
  myToolBars.clear();
}

/*!
    Creates the main desktop( called once )
*/
bool QAD_Application::createDesktop( SALOME_NamingService* name_service )
{    
    QAD_ASSERT_DEBUG_ONLY( !desktop );
    desktop = new QAD_Desktop ( name_service ); 
    return ( desktop != NULL );
}

/*!
    Creates the actions provided by this application 
    ( internal )
*/
void QAD_Application::createActions()
{
  /*  Basic application provides only Undo/Redo 
      functionalities which is disabled by default.
      All others must be added by subclasses. 
  */  
  QAD_ResourceMgr* rmgr = QAD_Desktop::getResourceManager();
  // Undo
  if ( !myEditActions.at( EditUndoId ) )
    {        
      QActionP* editUndo = new QActionP( tr("TOT_APP_EDIT_UNDO"), 
					rmgr->loadPixmap( "QAD",
							  tr("ICON_APP_EDIT_UNDO") ), 
					tr("MEN_APP_EDIT_UNDO"),
					CTRL+Key_Z, desktop );
      editUndo->setStatusTip ( tr("PRP_APP_EDIT_UNDO") );
      QAD_ASSERT ( connect( editUndo, SIGNAL( activated() ), this, SLOT( onUndo() )));
      myEditActions.insert( EditUndoId, editUndo );        
    }
  // Redo
  if ( !myEditActions.at( EditRedoId ) )
    {               
      QActionP* editRedo = new QActionP ( tr("TOT_APP_EDIT_REDO"), 
					rmgr->loadPixmap( "QAD",  tr("ICON_APP_EDIT_REDO") ), 
					tr("MEN_APP_EDIT_REDO"), CTRL+Key_Y, desktop );
      editRedo->setStatusTip ( tr("PRP_APP_EDIT_REDO") );
      QAD_ASSERT ( connect( editRedo, SIGNAL( activated() ), this, SLOT( onRedo() )));
      myEditActions.insert( EditRedoId, editRedo );        
    }
  // Copy
  if ( !myEditActions.at( EditCopyId ) )
    {        
      QActionP* editCopy = new QActionP ( tr("TOT_APP_EDIT_COPY"), 
					rmgr->loadPixmap( "QAD",
							  tr("ICON_APP_EDIT_COPY") ), 
					tr("MEN_APP_EDIT_COPY"),
					CTRL+Key_C, desktop );
      editCopy->setStatusTip ( tr("PRP_APP_EDIT_COPY") );
      QAD_ASSERT ( connect( editCopy, SIGNAL( activated() ), this, SLOT( onCopy() )));
      myEditActions.insert( EditCopyId, editCopy );        
    }
  // Paste
  if ( !myEditActions.at( EditPasteId ) )
    {        
      QActionP* editPaste = new QActionP ( tr("TOT_APP_EDIT_PASTE"), 
					rmgr->loadPixmap( "QAD",
							  tr("ICON_APP_EDIT_PASTE") ), 
					tr("MEN_APP_EDIT_PASTE"),
					CTRL+Key_V, desktop );
      editPaste->setStatusTip ( tr("PRP_APP_EDIT_PASTE") );
      QAD_ASSERT ( connect( editPaste, SIGNAL( activated() ), this, SLOT( onPaste() )));
      myEditActions.insert( EditPasteId, editPaste );        
    }
  // Update action state
  updateActions();
}

/*!
    Enables/disables the actions according to the
    application state
*/
void QAD_Application::updateActions()
{  
  QAD_ASSERT_DEBUG_ONLY( !myEditActions.isEmpty() );        
  if ( myActiveStudy ) {        
    myEditActions.at( EditUndoId )->setEnabled( myActiveStudy->canUndo() );
    myEditActions.at( EditRedoId )->setEnabled( myActiveStudy->canRedo() );
    myEditActions.at( EditCopyId )->setEnabled( canCopy() );
    myEditActions.at( EditPasteId )->setEnabled( canPaste() );
  }    
  else {
    myEditActions.at( EditUndoId )->setEnabled( false );
    myEditActions.at( EditRedoId )->setEnabled( false );
    myEditActions.at( EditCopyId )->setEnabled( false );
    myEditActions.at( EditPasteId )->setEnabled( false );
  }
}

/*!
    Returns the aplication name
*/
const QString& QAD_Application::getApplicationName() const
{
  return myName;
}

/*! 
    Returns the application icon   
*/
const QPixmap& QAD_Application::getApplicationIcon() const
{
  return myIcon;
}

/*!
    Returns the study description
*/
const QString&  QAD_Application::getStudyDescription() const
{
  return myStudyDescription;
}

/*!
    Returns the CAF study format
*/
const QString& QAD_Application::getStudyFormat() const
{
  return myStudyFormat;
}

/*!
    Returns the extension of supported type of files
*/
const QString& QAD_Application::getStudyExtension() const
{
  return myStudyExtension;
}

/*! 
    Returns 'true' if study is opened
*/
bool QAD_Application::isStudyOpened( const QString& studyName )
{
  QString Name = QAD_Tools::getFileNameFromPath( studyName, true );
  return (( getStudyByName( studyName ) != NULL ) || (getStudyByName( Name ) != NULL )); 
}

/*! 
    Returns the study object by its name
*/
QAD_Study* QAD_Application::getStudyByName( const QString& studyname )
{
  for ( QAD_Study* study = myStudies.first(); study; study = myStudies.next() )
    {
      if ( (studyname.compare( study->getPath() ) == 0 ) ||  
	   (studyname.compare( study->getTitle() ) == 0 ))
	return study;
    }
  return NULL;
}

/*!
    Returns vector of all opened studies
*/
QList<QAD_Study>& QAD_Application::getStudies() 
{
  return myStudies;
}

/*!
    Adds the study to the list of opened studies
    and set listeners for the study signals.    
*/
void QAD_Application::addStudy( QAD_Study* study )
{
  if ( !study ) return;
  
  QAD_ASSERT( connect(study,SIGNAL(docClosing(QAD_Study*)),desktop,
		      SLOT(onCloseStudy(QAD_Study*))) );    
  myStudies.append( study );
}

/*!
    Removes the study from the list of opened studies
*/
void QAD_Application::removeStudy( QAD_Study* study )
{
  if ( study ) 
    {
      /* close and destroy */        
      if ( study == myActiveStudy )
        myActiveStudy = 0; 
      myStudies.removeRef( study );
    }
}

/*!
    Activates 'About' dialog
*/
void QAD_Application::helpAbout()
{
  /* just calls QAD_Desktop::helpAbout() but can have own implementation */
  getDesktop()->helpAbout();
}

/*!
    Activates search in help 
*/
void QAD_Application::helpSearch()
{
  /* just calls QAD_Desktop::helpSearch() but can have own implementation */
//  getDesktop()->helpSearch();
}

/*!
    Activates help contents
*/
void QAD_Application::helpContents()
{
  /* just calls QAD_Desktop::helpContents() but can have own implementation */
//  getDesktop()->helpContents();
}

/*!
    Customizes menu "File" for the active application
*/
void QAD_Application::updateFilePopup( QPopupMenu* menu, bool add, int index )
{    
  if ( !menu ) return;
  
#ifdef DEBUG
  /*  ensure that 'add' command is followed by 'remove' */        
  static bool isAdded = false;            
  QAD_ASSERT_DEBUG_ONLY ( (add && !isAdded) || (!add && isAdded) );     
  isAdded = add;
#endif
  onUpdateFileActions( menu, add, index );
}

/*!
    Customizes menu "Edit" for the active application
*/
void QAD_Application::updateEditPopup( QPopupMenu* menu, bool add, int index  )
{
  if ( !menu ) return;
  
#ifdef DEBUG
  /*  ensure that 'add' command is followed by 'remove' */        
  static bool isAdded = false;            
  QAD_ASSERT_DEBUG_ONLY ( (add && !isAdded) || (!add && isAdded) );     
  isAdded = add;
#endif
  onUpdateEditActions( menu, add, index );
}

/*!
    Customizes menu "View" for the active application
*/
void QAD_Application::updateViewPopup( QPopupMenu* menu, bool add, int index  )
{
  if ( !menu ) return;
  
#ifdef DEBUG
  /*  ensure that 'add' command is followed by 'remove' */        
  static bool isAdded = false;            
  QAD_ASSERT_DEBUG_ONLY ( (add && !isAdded) || (!add && isAdded) );     
  isAdded = add;
#endif
  onUpdateViewActions( menu, add, index ); 
}

/*!
    Customizes menu "Help" for the active application
*/
void QAD_Application::updateHelpPopup( QPopupMenu* menu, bool add, int index )
{
  if ( !menu ) return;
#ifdef DEBUG
  /*  ensure that 'add' command is followed by 'remove' */        
  static bool isAdded = false;            
  QAD_ASSERT_DEBUG_ONLY ( (add && !isAdded) || (!add && isAdded) );         
  isAdded = add;
#endif
  onUpdateHelpActions ( menu, add, index ); 
}

/*!
  Customizes the main menu bar to add some new popup, 
  'Options' or 'Tools' for example.
*/
void QAD_Application::updateMainMenu( QMenuBar* menubar, bool add, int index )
{
  if ( !menubar ) return;
#ifdef DEBUG
  /*  ensure that 'add' command is followed by 'remove' */        
  static bool isAdded = false;            
  QAD_ASSERT_DEBUG_ONLY ( (add && !isAdded) || (!add && isAdded) );         
  isAdded = add;
#endif
  onUpdateMainMenuItems ( menubar, add, index ); 
}

/*!
    Adds specific application buttons for standard toolbar
*/
void QAD_Application::updateToolBars( bool add )
{    
#ifdef DEBUG
  /*  ensure that 'add' command is followed by 'remove' */        
  static bool isAdded = false;            
  QAD_ASSERT_DEBUG_ONLY ( (add && !isAdded) || (!add && isAdded) );         
  isAdded = add;
#endif
  onUpdateToolBars ( add ); 
}

/*!
    Customizes the status bar
*/
void QAD_Application::updateStatusBar( QStatusBar* statusBar, bool add ) 
{
#ifdef DEBUG
  /*  ensure that 'add' command is followed by 'remove' */        
  static bool isAdded = false;            
  QAD_ASSERT_DEBUG_ONLY ( (add && !isAdded) || (!add && isAdded) );         
  isAdded = add;
#endif
  onUpdateStatusBar( statusBar, add );
}

/*!
  Creates toolbar to be managed by QAD_Application
*/
QToolBar* QAD_Application::createToolBar( int   id,
					  const QString& label, Dock dock, 
					  bool  dockTop,        bool dockBottom, 
					  bool  dockLeft,       bool dockRight, 
					  bool  dockMinimized,  bool dockTornOff )
{
  QToolBar* tb = 0;
  if ( getDesktop() ) {
    tb = new QToolBar( label, getDesktop(), 0 );
    tb->setCloseMode( QDockWindow::Undocked );
    getDesktop()->setDockEnabled( tb, DockTop,       dockTop );
    getDesktop()->setDockEnabled( tb, DockBottom,    dockBottom );
    getDesktop()->setDockEnabled( tb, DockLeft,      dockLeft );
    getDesktop()->setDockEnabled( tb, DockRight,     dockRight );
    getDesktop()->setDockEnabled( tb, DockMinimized, dockMinimized );
    getDesktop()->setDockEnabled( tb, DockTornOff,   dockTornOff );
    getDesktop()->addDockWindow( tb, label, dock );
    getDesktop()->setAppropriate( tb, false );
    tb->hide();
    removeToolBar( id );
    myToolBars.insert( id, tb);
  }
  return tb;
}

/*!
  Removes toolbar
*/
void QAD_Application::removeToolBar( int id )
{
  QToolBar* tb = myToolBars[ id ];
  if ( tb ) {
    myToolBars.remove( id );
    getDesktop()->removeDockWindow( tb );
    delete tb;
  }
}

/*!
  Gets toolbar by id
*/
QToolBar* QAD_Application::getToolBar( int id )
{
  return myToolBars[ id ];
}

/*!
  Gets all toolbars managed by QAD_Applica
*/
QList<QToolBar> QAD_Application::getToolBars()
{
  QList<QToolBar> toolbars;
  QMap<int, QToolBar*>::Iterator it;
  for ( it = myToolBars.begin(); it != myToolBars.end(); it++ )
    toolbars.append( it.data() );
  return toolbars;
}

/*!
    Called by 'updateMainMenu' when application is activated/deactivated.
    Redefine to insert/remove the actions provided by the application
    to the main menu at 'index'.
    Returns the number of added/removed items.
*/
int QAD_Application::onUpdateMainMenuItems ( QMenuBar* mb, bool add, int index )
{
    return 0;
}

/*!
    Called by 'updateFilePopup' when application is activated/deactivated.
    Redefine to insert/remove the actions provided by the application to
    'File' menu of the desktop at 'index'.
    Returns the number of added/removed items.
*/
int QAD_Application::onUpdateFileActions ( QPopupMenu* popup, bool add, int index )
{
    return 0;
}

/*!
    Called by 'updateEditPopup' when application is activated/deactivated.
    Redefine to insert/remove the actions provided by the application to
    'Edit' menu of the desktop at 'index'.
    Returns the number of added/removed items.
*/
int QAD_Application::onUpdateEditActions ( QPopupMenu* popup, bool add, int index )
{
    QAD_ASSERT_DEBUG_ONLY( !myEditActions.isEmpty() );
    QToolBar* tb = getDesktop()->getStdToolBar();
    QActionP* undo  = myEditActions.at( EditUndoId );
    QActionP* redo  = myEditActions.at( EditRedoId );
    QActionP* copy  = myEditActions.at( EditCopyId );
    QActionP* paste = myEditActions.at( EditPasteId );
    if ( add ) 
    {         
      undo->addTo( popup );	
      index++;
      redo->addTo( popup );        
      index++;
      copy->addTo( popup );        
      index++;
      paste->addTo( popup );        
      index++;
      undo->addTo( tb );
      redo->addTo( tb );
      copy->addTo( tb );
      paste->addTo( tb );
    }
    else
    {
      popup->removeItemAt(index);
      popup->removeItemAt(index);
      popup->removeItemAt(index);
      popup->removeItemAt(index);

      undo->removeFrom( tb );
      redo->removeFrom( tb );
      copy->removeFrom( tb );
      paste->removeFrom( tb );
    }
    return index;
}

/*!
    Called by 'updateViewPopup' when application is activated/deactivated.
    Redefine to insert/remove the actions provided by the application to the
    'View' menu of the desktop at 'index'.
    Returns the number of added/removed items.
*/
int QAD_Application::onUpdateViewActions ( QPopupMenu* popup, bool add, int index )
{
    return 0;
}

/*!
    Called by 'updateHelpPopup' when application is activated/deactivated.
    Redefine to insert/remove the actions provided by the application
    to 'Help' menu of the desktop at 'index'.
    Returns the number of added/removed items.
*/
int QAD_Application::onUpdateHelpActions ( QPopupMenu* popup, bool add, int index )
{
    return 0;
}

/*!
    Called by 'updateStatusBar' when application is activated/deactivated.
    Redefine to customize status bar.
*/
void QAD_Application::onUpdateStatusBar( QStatusBar* sb, bool activate )
{
}

/*!
    Called by 'updateToolBars' when application is activated/deactivated.
    Redefine to show your specific toolbars on the desktop.
*/
void QAD_Application::onUpdateToolBars( bool activate )
{
  QMap<int, QToolBar*>::Iterator it;
  for ( it = myToolBars.begin(); it != myToolBars.end(); it++ ) {
    activate ? it.data()->show() : it.data()->hide();
    getDesktop()->setAppropriate( it.data(), activate );
  }
}

//=======================================================================//
//                          Study management                             //
//=======================================================================//
/*!
    Returns the active study
*/
QAD_Study* QAD_Application::getActiveStudy() const
{
    return myActiveStudy;
}

//=======================================================================//
//                          Operation management                         //
//=======================================================================//
/*!
    Runs a new operation of class appointed
*/
void QAD_Application::startOperation( QAD_Operation* op )
{
    if ( op ) op->start();
}


//=======================================================================//
//                          Study management                             //
//=======================================================================//
/*!
    Creates new study
*/
QAD_Study* QAD_Application::newStudy()
{       
  QAD_Study* newStudy = 0;
  QAD_ASSERT_DEBUG_ONLY( !myStudyMgr->_is_nil() );
  QApplication::setOverrideCursor( Qt::waitCursor );
  try {
    /* create QAD study*/
    QString StudyName = getDesktop()->getNewStudyName();

    CORBA::Object_var obj = getDesktop()->getNameService()->Resolve("/myStudyManager");
    SALOMEDS::StudyManager_var myStudyMgr = SALOMEDS::StudyManager::_narrow(obj);
    ASSERT(! CORBA::is_nil(myStudyMgr));
    SALOMEDS::ListOfOpenStudies_var List = myStudyMgr->GetOpenStudies();
    
    bool exist = true;
    
    while ( exist ) {
      exist = false;
      for (unsigned int ind = 0; ind < List->length();ind++) {
	QString NameS = QAD_Tools::getFileNameFromPath( QString( List[ind] ), true );
	if ( NameS.compare( StudyName ) == 0 ) {
	  StudyName = getDesktop()->getNewStudyName();
	  exist = true;
	  break;
	}
      }
    }
    
    if (getStudyByName ( StudyName ) != NULL )
      {
	/* create StudyDS */
	SALOMEDS::Study_var aStudy = myStudyMgr->NewStudy(getDesktop()->getNewStudyName());
	newStudy = new QAD_Study( this, aStudy, getDesktop()->getNewStudyName());
      }
    else 
      {
	/* create StudyDS */
	SALOMEDS::Study_var aStudy = myStudyMgr->NewStudy(StudyName);
	newStudy = new QAD_Study( this, aStudy, StudyName);
      }

    newStudy->Init();

    if ( newStudy->getResult() ) {
      addStudy( newStudy );
      
      /* activate */
      onStudyActivated( newStudy );
      
      /* customization on create ( called only once )*/
      onStudyCreated( newStudy );        
      
      /* show */
      newStudy->show();
      
      /* customization on show ( called only once ) */
      onStudyShown( newStudy );        
    }
  }
  catch( Standard_Failure ) {     
  }    
  qApp->processEvents();  /* update desktop*/
  QApplication::restoreOverrideCursor();
  return newStudy;
}

/*!
    Loads the existed study in StudyManager
*/
QAD_Study* QAD_Application::loadStudy( const QString& fileName )
{
  /* connect CAF document to CAF application */
  QAD_ASSERT_DEBUG_ONLY( !myStudyMgr->_is_nil() );
    
  SALOMEDS::Study_var aStudy = myStudyMgr->GetStudyByName( fileName );

 //   SALOMEDS::ListOfOpenStudies_var List = myStudyMgr->GetOpenStudies();
//    for (unsigned int ind = 0; ind < List->length();ind++) {
//      MESSAGE ( " Study index : " << List[ind] )
//      aStudy = myStudyMgr->GetStudyByName(List[ind]);
//    }
 
  QAD_Study* loadStudy = 0;
  QApplication::setOverrideCursor( Qt::waitCursor );
  try {        
    /* create QAD study */  
    loadStudy = new QAD_Study( this, aStudy, aStudy->Name() );
    
    if ( loadStudy->getResult() ) {
      addStudy( loadStudy );
      
     /* activate */
      onStudyActivated( loadStudy );
      
      /* customisation on load ( called only once ) */
      onStudyOpened( loadStudy );       
      
     /* show */
      loadStudy->show();
      
     /* customization on show ( called only once ) */
      onStudyShown( loadStudy );        
    }

  }
  catch( Standard_Failure ) {    
  }
  qApp->processEvents();  /* update desktop*/
  QApplication::restoreOverrideCursor();
  return loadStudy;
}

/*!
    Opens the existed study
*/
QAD_Study* QAD_Application::openStudy( const QString& fileName )
{
#ifdef WNT
    /*  Qt uses UNIX-like slashes even on WIN platform */        
    QString name = QAD_Tools::unix2win( fileName );
#else
    QString name = fileName;
#endif    
    SALOMEDS::Study_var aStudy;
    
    /* connect CAF document to CAF application */
    QAD_ASSERT_DEBUG_ONLY( !myStudyMgr->_is_nil() );
 
    QAD_Study* openStudy = 0;
    QApplication::setOverrideCursor( Qt::waitCursor );
    try {        
      /* open StudyDS */
      SALOMEDS::Study_var aStudy = myStudyMgr->Open((char*) name.latin1());

      //MPV: name of the study is set in the study manager now
      //NRI DEBUG : 11/02/2002
      //aStudy->Name( QAD_Tools::getFileNameFromPath(name,true) );

      /* create QAD study */            
      openStudy = new QAD_Study( this, aStudy, fileName );
      
      if ( openStudy->getResult() ) {
	addStudy( openStudy );
	
	/* activate */
	onStudyActivated( openStudy );
	
	/* customisation on open ( called only once ) */
	onStudyOpened( openStudy );       
	
	/* show */
	openStudy->show();
	
	/* customization on show ( called only once ) */
	onStudyShown( openStudy );        
      }

    }
    // Handle SALOME::SALOME_Exception raised by SALOMEDS::StudyMgr
    // Ensure that null study is returned in case of errors
    catch(SALOME::SALOME_Exception&) {
      if (openStudy) {
	delete openStudy;
	openStudy = 0;
      }
    }
    catch( Standard_Failure ) {    
    }
    qApp->processEvents();  /* update desktop*/
    QApplication::restoreOverrideCursor();
    return openStudy;
}

/*!
    Saves the study
*/
bool QAD_Application::saveStudy( QAD_Study* study )
{
    bool MultiSave = QAD_CONFIG->getSetting("Desktop:MultiFileSave") == "true";
    bool ASCIISave = QAD_CONFIG->getSetting("Desktop:ASCIISave") == "true";
    bool success = true;
    QApplication::setOverrideCursor( Qt::waitCursor );
    try {
      /* save StudyDS */
      if (ASCIISave) myStudyMgr->SaveASCII(study->getStudyDocument(), MultiSave);
      else myStudyMgr->Save(study->getStudyDocument(), MultiSave);
      study->updateCaptions();
    } 
    catch ( Standard_Failure ) {        
        success = false;
    }
    qApp->processEvents();  /* update desktop*/
    QApplication::restoreOverrideCursor();
    return success;
}

/*!
    Saves the study in a new file
*/
bool QAD_Application::saveAsStudy( QAD_Study* study, const QString& fileName )
{   
    bool MultiSave = QAD_CONFIG->getSetting("Desktop:MultiFileSave") == "true";
    bool ASCIISave = QAD_CONFIG->getSetting("Desktop:ASCIISave") == "true";
    bool success = false;
#ifdef WNT
    /*  Qt uses UNIX-like slashes even on WIN platform */        
    QString name = QAD_Tools::unix2win( fileName );
#else
    QString name = fileName;
#endif    
    if ( !name.isNull() )
    {
       /* check permission */
        if (!checkPermission(name))
          return false;
        QApplication::setOverrideCursor( Qt::waitCursor );
        TCollection_ExtendedString fileNameExt ( (char*) name.latin1() );
        try {
	  /* save as StudyDS */
	  if (ASCIISave) myStudyMgr->SaveAsASCII(name.latin1(), study->getStudyDocument(), MultiSave);
	  else myStudyMgr->SaveAs(name.latin1(), study->getStudyDocument(), MultiSave);

	  study->setTitle( fileName );
	  study->updateCaptions();
	  success = true;
        }
        catch (Standard_Failure) {            
        }
        qApp->processEvents();  /* update desktop*/
        QApplication::restoreOverrideCursor(); 
    }
    return success;    
}

/*!
    Closes the study
*/
bool QAD_Application::closeStudy( QAD_Study* study, bool permanently /*=false*/ )
{
  bool success = true;
  /* close CAF document */
  QApplication::setOverrideCursor( Qt::waitCursor );
  try
    {
      SALOMEDS::Study_var aStudyDocument = study->getStudyDocument();
      
      /* customization */
      onStudyClosed( study );
      
      /* remove */
      removeStudy( study );
      
      /* activate */
      myActiveStudy = myStudies.last();
      
      /* close application when the last study is closed */
      if ( myStudies.isEmpty() )
	emit appDeactivated ( this );

      if ( permanently )
	{
	  /* close StudyDS */
	  myStudyMgr->Close( aStudyDocument );
	}
    } 
  catch ( Standard_Failure )
    {
      success = false;
    }
  qApp->processEvents();  /* update desktop*/
  QApplication::restoreOverrideCursor(); 
  return success;
}

/*!
    Undo operation on the active study. 
    Calls undo( activeStudy ).
*/
bool QAD_Application::onUndo()
{
    bool ok = undo( myActiveStudy );
    updateActions();     /* enable/disable undo/redo */
    return ok;
}

/*!
    Undo operation on the given study
*/
bool QAD_Application::undo(QAD_Study* study)
{
  bool status = false;
  if ( study ) 
    {
      status = study->undo();
    }
  return status;
}

/*!
    Redo operation on the active study.
    Calls redo( myActiveStudy ). 
*/
bool QAD_Application::onRedo()
{
  bool ok = redo( myActiveStudy );
  updateActions();   /* enable/disable undo/redo */
  return ok;
}

/*!
    Redo operation on the given study.
*/
bool QAD_Application::redo(QAD_Study* study)
{
  bool status = false;
  if ( study )
    {
      status = study->redo();
    }
  return status;
}

/*!
    Copies object to the pseudo-clipboard
*/
bool QAD_Application::onCopy()
{
  bool status = false;
  if ( myActiveStudy) {
    SALOME_Selection* Sel = SALOME_Selection::Selection( myActiveStudy->getSelection() );
    if ( Sel && Sel->IObjectCount() == 1 ) {
      SALOMEDS::SObject_var    SO = myActiveStudy->getStudyDocument()->FindObjectID( Sel->firstIObject()->getEntry() );
      if ( !SO->_is_nil() ) {
	SALOMEDS::SComponent_var SComp = SALOMEDS::SComponent::_narrow( SO );
	if ( SComp->_is_nil() ) { // disable 'copy' operations for components 
	  status = myStudyMgr->Copy( SO );
	  updateActions();     
	}
      }
    }
  } 
  return status;
}

/*!
    Pastes object from the pseudo-clipboard
*/
bool QAD_Application::onPaste()
{
  SALOMEDS::SObject_var aResult;
  if ( myActiveStudy) {
    SALOME_Selection* Sel = SALOME_Selection::Selection( myActiveStudy->getSelection() );
    if ( Sel && Sel->IObjectCount() == 1 ) {
      SALOMEDS::SObject_var    SO = myActiveStudy->getStudyDocument()->FindObjectID( Sel->firstIObject()->getEntry() );
      if ( !SO->_is_nil() ) {
	try {
	  aResult = myStudyMgr->Paste( SO );
	}
	catch ( SALOMEDS::StudyBuilder::LockProtection& ) {
	  myActiveStudy->updateObjBrowser( true );
	  updateActions();
	  QAD_MessageBox::warn1 ((QWidget*)QAD_Application::getDesktop(),
				 QObject::tr("WRN_WARNING"), 
				 QObject::tr("WRN_STUDY_LOCKED"),
				 QObject::tr("BUT_OK"));
	  return false;
	}
	myActiveStudy->updateObjBrowser( true );
	updateActions();
      }
    }
  } 
  return aResult->_is_nil();
}

/*!
    Checks if it is possible to make 'copy' operation
*/
bool QAD_Application::canCopy() 
{
  bool status = false;
  if ( myActiveStudy) {
    SALOME_Selection* Sel = SALOME_Selection::Selection( myActiveStudy->getSelection() );
    if ( Sel && Sel->IObjectCount() == 1 ) {
      SALOMEDS::SObject_var    SO = myActiveStudy->getStudyDocument()->FindObjectID( Sel->firstIObject()->getEntry() );
      if ( !SO->_is_nil() ) {
	SALOMEDS::SComponent_var SComp = SALOMEDS::SComponent::_narrow( SO );
	if ( SComp->_is_nil() ) // disable copy for components
	  status = myStudyMgr->CanCopy( SO );
      }
    }
  } 
  return status;
}

/*!
    Checks if it is possible to make 'paste' operation
*/
bool QAD_Application::canPaste() 
{
  bool status = false;
  if ( myActiveStudy) {
    SALOME_Selection* Sel = SALOME_Selection::Selection( myActiveStudy->getSelection() );
    if ( Sel && Sel->IObjectCount() == 1 ) {
      SALOMEDS::SObject_var    SO = myActiveStudy->getStudyDocument()->FindObjectID( Sel->firstIObject()->getEntry() );
      if ( !SO->_is_nil() ) {
	status = myStudyMgr->CanPaste( SO );
      }
    }
  } 
  return status;
}

/*!
    Called when the main desktop is created ( internal )
*/
void QAD_Application::onDesktopCreated()
{
  createActions();
}

/*!
    Called after creating the given study
*/
void QAD_Application::onStudyCreated(QAD_Study* study)
{
}

/*!
    Called after opening the given study
*/
void QAD_Application::onStudyOpened(QAD_Study* study)
{
}

/*!
    Called after the first showing of the given study
*/
void QAD_Application::onStudyShown( QAD_Study* study )
{
}

/*!
    Called when the given study is activated
*/
void QAD_Application::onStudyActivated(QAD_Study* study)
{
  //  MESSAGE ("QAD_Application::onStudyActivated init. "); 

  /* deactivate old study, activate new study*/

  /* NRI : init DEBUG IAP_001 */
  QAD_Study* oldActiveStudy = 0;
  if ( myActiveStudy != NULL )
    oldActiveStudy = myActiveStudy;
  /* NRI : end DEBUG IAP_001 */
  

  if ( myActiveStudy )    
    onStudyDeactivated( myActiveStudy );        
  
  myActiveStudy = study;

  // VSR : call app activate before informing modules that active study is changed ===>
  /* application activated */
  emit appActivated( this );

  /* notification */
  onActiveStudyChanged( oldActiveStudy, myActiveStudy );
  // VSR : call app activate before informing modules that active study is changed <===
}

/*!
    Called when the given study is deactivated
*/
void QAD_Application::onStudyDeactivated(QAD_Study* study)
{
  //  MESSAGE ("QAD_Application::onStudyDeactivated init. "); 
  QAD_ASSERT_DEBUG_ONLY ( myActiveStudy == study );
  if ( study )
    study->onStudyDeactivated();
  //  MESSAGE ("QAD_Application::onStudyDeactivated done. "); 
}

/*!
    Called before closing the given study
*/
void QAD_Application::onStudyClosed(QAD_Study* study)
{
}

/*!
    Called when the active study changed
*/
void QAD_Application::onActiveStudyChanged( QAD_Study* oldActiveStudy, 
					    QAD_Study* newActiveStudy )
{
  // san -- Check added: see comment by P.Rascle to PAL8065 about poor performance
  if ( oldActiveStudy != newActiveStudy )
    getDesktop()->onActiveStudyChanged();

  if ( oldActiveStudy )
    QAD_ASSERT( disconnect( oldActiveStudy, SIGNAL(docOperationTerminated( bool )),
			    this, SLOT( onOperationTerminated( bool ) ) ));
  if ( newActiveStudy )
    QAD_ASSERT( connect( newActiveStudy, SIGNAL(docOperationTerminated( bool )),
			 this, SLOT( onOperationTerminated( bool ) ) ));
  updateActions();
}

/*!
    Called when the operation 'op' terminated [ protected slot ]
*/
void QAD_Application::onOperationTerminated( bool successfully )
{
  if ( successfully )     
    updateActions();        /* enable/disable undo/redo */        
}

/*!
    Adds the specific items to context popup menu activated usually by MB3.
    Redefine to fill with desired items.
*/
void QAD_Application::onCreatePopup ()
{
}

