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
//  File   : QAD_XmlHandler.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#define  INCLUDE_MENUITEM_DEF 

#include "QAD_Desktop.h"
#include "QAD_XmlHandler.h"
#include "QAD_ListMenuIdAction.h"
#include "QAD_Menus.h"
#include "QAD_MessageBox.h"
#include "QAD_Tools.h"
#include "QAD_Resource.h"

// QT Include
#include <qaccel.h>
#include <qstringlist.h>
#include <qfileinfo.h>
using namespace std;

map<int,QString>	      QAD_XmlHandler::_bibmap;

static const char* SEPARATOR    = ":";

static QString findFile( QString resDirs, QString fileName )
{
  QStringList dirList = QStringList::split( SEPARATOR, resDirs, false ); // skip empty entries
  for ( int i = 0; i < dirList.count(); i++ ) {
    QString dir = dirList[ i ];
    QFileInfo fi( QAD_Tools::addSlash( dir ) + fileName );
    if ( fi.isFile() && fi.exists() )
      return fi.filePath();
  }
  return QString();
}

/*!
  Constructor
*/
QAD_XmlHandler::QAD_XmlHandler()
{
}

/*!
  Destructor
*/
QAD_XmlHandler::~QAD_XmlHandler()
{
}

/*!
  Sets \a desktop widget which contains menubar.
*/
void QAD_XmlHandler::setMainWindow(QAD_Desktop* desktop)
{
  myDesktop       = desktop;
  myIdList        = TColStd_SequenceOfInteger();
  myBackMenu      = true;
  myBackPopupMenus.setAutoDelete(false);
}

/*!
  Returns TRUE if environment variable CSF_"Component"Resources is undefined.
  Returns FALSE otherwise.
*/
bool QAD_XmlHandler::setComponent (const QString& dirs )
{
  myPathResources = dirs;
  
  return true ;
}

/*!
  Starts parsing of document. Does some initialization

  Reimplemented from QXmlDefaultHandler.
*/
bool QAD_XmlHandler::startDocument()
{
  myErrorProt = "";
  return TRUE;
}

/*!
  Does different actions depending on the name of the tag and the
  state you are in document.

  Reimplemented from QXmlDefaultHandler.
*/
bool QAD_XmlHandler::startElement( const QString&, const QString&, 
				   const QString& qName, 
				   const QXmlAttributes& atts )
{
  char Slot_Name[200];

  if ( qName == "application") {
    myMenus=new QAD_Menus(myDesktop);
    myMenusList.append(myMenus);
  } else if ( qName == "desktop" ) { //DESKTOP
  } else if ( qName == "menubar" ) { //MENUBAR
    QMenuBar* aMenuBar = myDesktop->menuBar();  /*new QMenuBar(myMainWindow);*/
    myMenus->myActiveMenuBar=aMenuBar;
    myMenus->append(aMenuBar);
    myMenus->myActiveMenuBar->hide();
    
  } else if ( qName == "menu-item" ) { //MENU-ITEM
    myBackMenu = true;
    QMenuItem* item = myMenus->myActiveMenuBar->findItem(atts.value( "item-id" ).toInt());
    if ( item ) {
      QPopupMenu* aMenuItem = item->popup();
      myMenus->myActivePopupMenu=aMenuItem;
    } else {
      QPopupMenu* aMenuItem=new QPopupMenu(myMenus->myActiveMenuBar);
      myMenus->myActivePopupMenu=aMenuItem;
      int id;
      int pos;
      if (atts.value( "pos-id" ).compare(QString(""))==0) {
	pos = -1;
      }
      else {
	pos = atts.value( "pos-id" ).toInt();
	if ( pos < 0 )
	  pos = myMenus->myActiveMenuBar->count() + pos;
      }
      id = myMenus->myActiveMenuBar->insertItem( atts.value( "label-id" ),
						 aMenuItem,
						 atts.value( "item-id" ).toInt(),
					 	 pos );
      myIdList.Append( id );
    }
  } else if ( qName == "popup-item" ) { //POPUP-ITEM
    QString aCommandID(atts.value( "item-id" ));	
    int id;
    int pos;
    if (atts.value( "pos-id" ).compare(QString(""))==0) {
      pos = -1;
    }
    else {
      pos = atts.value( "pos-id" ).toInt();
      if ( pos < 0 )
	pos = myMenus->myActivePopupMenu->count() + pos;
    }
    if (atts.value( "icon-id").compare(QString(""))==0) 
      if (atts.value( "execute-action" ).compare(QString(""))==0)
	id = myMenus->myActivePopupMenu->insertItem( atts.value( "label-id" ),
						     myDesktop, SLOT (onDispatch( int )),
						     QAccel::stringToKey(atts.value( "accel-id" )),
						     aCommandID.toInt(),
						     pos);
      else
	id = myMenus->myActivePopupMenu->insertItem( atts.value( "label-id" ),
						     myDesktop, SLOT (onDispatchTools( int )),
						     QAccel::stringToKey(atts.value( "accel-id" )),
						     aCommandID.toInt(),
						     pos);
    
    else 
      if (atts.value( "execute-action" ).compare(QString(""))==0)
	id = myMenus->myActivePopupMenu->insertItem( QPixmap( findFile( myPathResources, atts.value( "icon-id" ) ) ),
						     atts.value( "label-id" ),
						     myDesktop, SLOT (onDispatch( int )),
						     QAccel::stringToKey(atts.value( "accel-id" )),
						     aCommandID.toInt(),
						     pos );
      else
	id = myMenus->myActivePopupMenu->insertItem( QPixmap( findFile( myPathResources, atts.value( "icon-id") ) ),
						     atts.value( "label-id" ),
						     myDesktop, SLOT (onDispatchTools( int )),
						     QAccel::stringToKey(atts.value( "accel-id" )),
						     aCommandID.toInt(),
						     pos);
    if (atts.value( "chaine")!=NULL) {
      QAD_XmlHandler::_bibmap[atts.value( "item-id").toInt()]=QString(atts.value( "chaine"));
//        SCRUTE(atts.value( "item-id").toInt() ) ;
//        SCRUTE(QAD_XmlHandler::_bibmap[atts.value( "item-id").toInt()] ) ;
    }
    if (atts.value( "toggle-id" ).compare(QString(""))!=0) {
      if (atts.value( "toggle-id" ).compare(QString("false"))==0) {
	myMenus->myActivePopupMenu->setItemChecked(id,false);
      }
      if (atts.value( "toggle-id" ).compare(QString("true"))==0) {
	myMenus->myActivePopupMenu->setItemChecked(id,true);
      }
    }
    myIdList.Append( id );
    myMenus->myActivePopupMenu->setItemParameter( id, aCommandID.toInt());
  } else if ( qName == "submenu" ) {	//SUBMENU
    myBackPopupMenus.push(myMenus->myActivePopupMenu);
    QMenuItem* item = 0;
    if ( myBackMenu )
      item = myMenus->myActivePopupMenu->findItem(atts.value( "item-id" ).toInt());
      
    QPopupMenu* aSubmenu = new QPopupMenu();
    if ( item ) {
      if (item->popup()) {
	aSubmenu=item->popup();
	//Enable submenu only if disable
	if (!myMenus->myActivePopupMenu->isItemEnabled( atts.value( "item-id" ).toInt()))
	  myMenus->myActivePopupMenu->setItemEnabled( atts.value( "item-id" ).toInt(), true);
      }
    } else {      
      aSubmenu=new QPopupMenu(myMenus->myActivePopupMenu);
      int id;
      int pos;
      if (atts.value( "pos-id" ).compare(QString(""))==0) {
	pos = -1;
      }
      else {
	pos = atts.value( "pos-id" ).toInt();
	if ( pos < 0 )
	  pos = myMenus->myActivePopupMenu->count() + pos;
      }
      id = myMenus->myActivePopupMenu->insertItem(atts.value( "label-id" ),
						  aSubmenu,atts.value( "item-id" ).toInt(),
						  pos);
      myIdList.Append(id);
    }
    myMenus->myActivePopupMenu=aSubmenu;
  } else if ( qName == "endsubmenu" ) {	//SUBMENU
    myMenus->myActivePopupMenu = myBackPopupMenus.pop();
  } else if ( qName == "popupmenu" ) {	//POPUP MENU
    myBackMenu = false;
    QPopupMenu* aPopupMenu=new QPopupMenu;
    myMenus->myActivePopupMenu=aPopupMenu;
    aPopupMenu->insertItem( atts.value( "label-id" ), aPopupMenu, QAD_TopLabel_Popup_ID);
    QAD_ListPopupMenu* pmenu = new QAD_ListPopupMenu(myMenus->myActivePopupMenu,
						     atts.value( "context-id" ),
						     atts.value( "parent-id" ),
						     atts.value( "object-id" ));
    myMenus->append(pmenu);
  } else if ( qName == "toolbar" ) {	//TOOLBAR
    QToolBar* aToolBar=new QToolBar(atts.value( "label-id" ),myDesktop);
    aToolBar->setCloseMode( QDockWindow::Undocked );
    myMenus->append(aToolBar);
    myMenus->myActiveToolBar=aToolBar;
    myMenus->myActiveToolBar->hide();
  } else if ( qName == "toolbutton-item" ) {	//TOOL BUTTON
    QString aQStringToolButtonItemId(atts.value( "item-id" ));
    QActionP* anAction=
      new QActionP
      (QObject::tr(atts.value( "label-id" )),
       QPixmap( findFile( myPathResources, atts.value( "icon-id") ) ),
       QObject::tr(atts.value( "label-id" )),
       QAccel::stringToKey(QObject::tr(atts.value( "accel-id" ))),
       myDesktop);
    
    anAction->setStatusTip(QObject::tr(atts.value( "tooltip-id" )));            
    anAction->setToolTip(QObject::tr(atts.value( "tooltip-id" )));        
    anAction->addTo(myMenus->myActiveToolBar);
    QAD_ListMenuIdAction* menu = new QAD_ListMenuIdAction(myMenus->myActivePopupMenu,
							  aQStringToolButtonItemId.toInt(),
							  anAction);
    myMenus->append(menu);
    QObject::connect(anAction,SIGNAL(activated()), myDesktop,SLOT(onDispatch( )));
  } else if (qName == "separatorTB") {
    myMenus->myActiveToolBar->addSeparator();
  } else if (qName == "separator") {
    int id;
    int pos;
    if (atts.value( "pos-id" ).compare(QString(""))==0) {
      pos = -1;
    }
    else {
      pos = atts.value( "pos-id" ).toInt();
      if ( pos < 0 )
	pos = myMenus->myActivePopupMenu->count() + pos;
    }
    id = myMenus->myActivePopupMenu->insertSeparator( pos );
    myIdList.Append(id);
  }else {
    // error
    return FALSE;
  }
  return TRUE;
}


/*!
  Reimplemented from QXmlDefaultHandler.
*/
bool QAD_XmlHandler::endElement( const QString&, const QString&, const QString& )
{
  return TRUE;
}


/*!
  Reimplemented from QXmlDefaultHandler.
*/
bool QAD_XmlHandler::characters( const QString& ch )
{
  // we are not interested in whitespaces
  QString ch_simplified = ch.simplifyWhiteSpace();
  if ( ch_simplified.isEmpty() )
    return TRUE;
  return TRUE;
}


/*!
  Returns the default error string.

  Reimplemented from QXmlDefaultHandler.
*/
QString QAD_XmlHandler::errorString()
{
  return "the document is not in the quote file format";
}

/*!
  Returns exception

  Reimplemented from QXmlDefaultHandler.
*/
bool QAD_XmlHandler::fatalError( const QXmlParseException& exception )
{
  myErrorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
    .arg( exception.message() )
    .arg( exception.lineNumber() )
    .arg( exception.columnNumber() );
  
  return QXmlDefaultHandler::fatalError( exception );
}

/*!
  Returns the error protocol if parsing failed

  Reimplemented from QXmlDefaultHandler.
*/
QString QAD_XmlHandler::errorProtocol()
{
  return myErrorProt;
}

QString QAD_XmlHandler::givebib(const int mykey) 
{
  return QAD_XmlHandler::_bibmap[mykey];
}
