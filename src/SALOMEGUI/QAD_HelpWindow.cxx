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
//  File   : QAD_HelpWindow.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

using namespace std;
/*!
  \class QAD_HelpWindow QAD_HelpWindow.h
  \brief Html browser for help.
*/

#include "QAD_HelpWindow.h"
#include "QAD_ResourceMgr.h"
#include "QAD_Desktop.h"
#include "QAD_Application.h"
#include "QAD_Tools.h"

// QT Includes
#include <qapplication.h>
#include <qtoolbar.h>
#include <qmenubar.h>
#include <qiconset.h>
#include <qtoolbutton.h>
#include <qcombobox.h>
#include <qfileinfo.h>

/*!
  Constructor
*/
QAD_HelpWindow::QAD_HelpWindow()
  : QMainWindow( 0, "SALOME Professional Help", WStyle_Customize | WType_TopLevel | WDestructiveClose ),
    pathCombo( 0 ), selectedURL()
{
  QAD_ResourceMgr* rmgr = QAD_Desktop::getResourceManager();

  // alternative palette
  setPalette( QAD_Application::getPalette(true) );
  // caption
  setCaption( tr("SALOMEGUI_HELP_TITLE") );
  // icon
  QPixmap icon ( rmgr->loadPixmap( "CLIENT", tr("ICON_DESK_DEFAULTICON") ) );
  if ( !icon.isNull() ) setIcon( icon );

  // create browser
  browser = new QTextBrowser( this );
  browser->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  connect( browser, SIGNAL( textChanged() ), this, SLOT( textChanged() ) );
  setCentralWidget( browser );

  // menu/toolbar actions
  menuBar()->setItemEnabled( forwardId, FALSE);
  menuBar()->setItemEnabled( backwardId, FALSE);
  connect( browser, SIGNAL( backwardAvailable( bool ) ),
	   this, SLOT( setBackwardAvailable( bool ) ) );
  connect( browser, SIGNAL( forwardAvailable( bool ) ),
	   this, SLOT( setForwardAvailable( bool ) ) );
  
  QIconSet icon_back( QPixmap(rmgr->loadPixmap("QAD", tr("ICON_APP_HELP_BACK")) ));
  QIconSet icon_forward( QPixmap(rmgr->loadPixmap("QAD", tr("ICON_APP_HELP_FORWARD")) ));
  QIconSet icon_home( QPixmap(rmgr->loadPixmap("QAD", tr("ICON_APP_HELP_HOME")) ));
  
  QToolBar* toolbar = new QToolBar( this );
  addToolBar( toolbar, "Toolbar");
  QToolButton* button;
  
  button = new QToolButton( icon_back, tr("TOT_HELPWINDOW_BACKWARD"), "", browser, SLOT(backward()), toolbar );
  connect( browser, SIGNAL( backwardAvailable(bool) ), button, SLOT( setEnabled(bool) ) );
  button->setEnabled( FALSE );
  button = new QToolButton( icon_forward, tr("TOT_HELPWINDOW_FORWARD"), "", browser, SLOT(forward()), toolbar );
  connect( browser, SIGNAL( forwardAvailable(bool) ), button, SLOT( setEnabled(bool) ) );
  button->setEnabled( FALSE );
  button = new QToolButton( icon_home, tr("TOT_HELPWINDOW_HOME"), "", browser, SLOT(home()), toolbar );
  
  toolbar->addSeparator();
  
  pathCombo = new QComboBox( TRUE, toolbar );
  pathCombo->setInsertionPolicy(QComboBox::NoInsertion);
  pathCombo->setDuplicatesEnabled(false);
  connect( pathCombo, SIGNAL( activated( const QString & ) ),
	   this, SLOT( pathSelected( const QString & ) ) );
  toolbar->setStretchableWidget( pathCombo );
  setRightJustification( TRUE );
  setDockEnabled( Left, FALSE );
  setDockEnabled( Right, FALSE );
  setDockEnabled( Bottom, FALSE );
  setDockEnabled( TornOff, FALSE );
  setDockMenuEnabled( false );

  /*
  // look for index.html and set homeDir
  // 1. $(SALOME_ROOT_DIR)/doc/index.html
  // 2. $(SALOME_ROOT_DIR)/doc/html/index.html
  // 3. $(SALOME_ROOT_DIR)/doc/html/html/index.html
  // 4. /usr/local/doc/html/index.html

  QCString dir;
  QString root;
  if ( (dir = getenv("KERNEL_ROOT_DIR")) ) {
    root = QAD_Tools::addSlash( QAD_Tools::addSlash(dir) + QAD_Tools::addSlash("share")  + QAD_Tools::addSlash("salome")  + "doc" );
    if ( QFileInfo( root + "index.html" ).exists() ) {
      homeDir = root;
    }
    else {
      root = QAD_Tools::addSlash( root + "html" );
      if ( QFileInfo( root + "index.html" ).exists() ) {
	homeDir = root;
      }
      else {
	root = QAD_Tools::addSlash( root + "html" );
	if ( QFileInfo( root + "index.html" ).exists() ) {
	  homeDir = root;
	}
      }
    }
  }
  if ( root.isEmpty() ) {
    if ( QFileInfo( "/usr/local/doc/html/index.html" ).exists() ) {
      homeDir = "/usr/local/doc/html/";
    }
  }
  if ( root.isEmpty() ) 
    root = "./doc/";

  browser->setSource( QFileInfo( homeDir + "index.html" ).absFilePath() );
  */
  browser->setFocus();

  QSize dSize = QApplication::desktop()->size();
  int x = ( dSize.width()  - 750 ) / 2;
  int y = ( dSize.height() - 680 ) / 2;
  setGeometry( x, y, 750, 680 );
}

/*!
  Destructor
*/
QAD_HelpWindow::~QAD_HelpWindow()
{
  emit(helpWindowClosed());
}

/*!
  enables/disables <Backward> button
*/
void QAD_HelpWindow::setBackwardAvailable( bool b)
{
  menuBar()->setItemEnabled( backwardId, b);
}

/*!
  enables/disables <Forward> button
  */
void QAD_HelpWindow::setForwardAvailable( bool b)
{
  menuBar()->setItemEnabled( forwardId, b);
}

/*!
  called when new document is loaded into browser
*/
void QAD_HelpWindow::textChanged()
{
  if ( browser->documentTitle().isNull() && browser->context().isNull() && browser->source().isNull() )
    browser->clear();
  selectedURL = browser->source(); //caption();
  if ( !selectedURL.isEmpty() && pathCombo ) {
    bool exists = FALSE;
    int i;
    for ( i = 0; i < pathCombo->count(); ++i ) {
      if ( pathCombo->text( i ) == selectedURL ) {
	exists = TRUE;
	break;
      }
    }
    if ( !exists ) {
      pathCombo->insertItem( selectedURL, 0 );
      pathCombo->setCurrentItem( 0 );
    } else {
      pathCombo->setCurrentItem( i );
    }
    selectedURL = QString::null;
  }
}

/*!
  called when users selectes dosument from the combo box
*/
void QAD_HelpWindow::pathSelected( const QString& _path )
{
  browser->setSource( _path );
}

/*!
  goes back to the contents
*/
void QAD_HelpWindow::contents()
{
  browser->home();
}

/*!
  opens new document in browser window and makes context search
  <_source>  is a filename, can be relative, if empty, index.html name is used
  <_context> is a context, e.g. link in the file
*/
void QAD_HelpWindow::context( const QString& _source, const QString& _context)
{
  QString src;
  if ( _source.isEmpty() ) {
    src = homeDir + "index.html";
  }
  else {
    QFileInfo fi( _source );
    if ( fi.isRelative() )
      src = homeDir + _source;
    else
      src = _source;
  }
  if ( !_context.isEmpty() )
    src += "#" + _context;
  browser->setSource( src );
}
