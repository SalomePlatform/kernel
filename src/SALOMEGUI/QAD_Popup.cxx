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
//  File   : QAD_Popup.cxx
//  Author : UI team
//  Module : SALOME
//  $Header$

#include "QAD.h"
#include "QAD_Popup.h"
#include "utilities.h"
using namespace std;

/*****************************************************************************
**  Class QAD_PopupClientServer
*****************************************************************************/

/*!
    Constructor
*/
QAD_PopupServer::QAD_PopupServer() :
myPopup( NULL ),
myEnablePopup( true )
{
}

/*!
    Enables/disables creation of popup.
    Useful when user doesn't want to create popup
    provided by server by default and does not 
    want to inherit from the server
*/
void QAD_PopupServer::enablePopup( bool enable )
{
    myEnablePopup = enable;
}

/*!
    Called by popup client when it wants
    to activate popup
*/
QPopupMenu* QAD_PopupServer::createPopup()
{
  if ( !myEnablePopup )
    return 0;
  
  if ( !myPopup ) 
    myPopup = new QPopupMenu;
  onCreatePopup();	/* add specific menu items */
  return myPopup;	
}

/*!
    Called by popup client when popup
    is deactivated.
*/
void QAD_PopupServer::destroyPopup()
{
  if ( myPopup ) myPopup->clear();
}

/*!
    Destructor
*/
QAD_PopupServer::~QAD_PopupServer()
{
  delete (QPopupMenu*) myPopup;
}

/*****************************************************************************
**  Class QAD_PopupClient
*****************************************************************************/

/*!
    Constructor
*/
QAD_PopupClient::QAD_PopupClient() :
myPopupServer( 0 )
{
}

/*!
    Destructor
*/
QAD_PopupClient::~QAD_PopupClient() 
{
}

/*!
    Sets a popup server for this client
*/
void QAD_PopupClient::setPopupServer ( QAD_PopupServer* server)
{
  myPopupServer = server;
}

/*!
    Returns the popup server for this client
*/
QAD_PopupServer* QAD_PopupClient::getPopupServer () const
{
  return myPopupServer;
}

/*****************************************************************************
**  Class QAD_PopupClientServer
*****************************************************************************/

/*!
    Constructor
*/
QAD_PopupClientServer::QAD_PopupClientServer( bool separateItems, bool smartSeparator ) :
  myOnlyServer( false ),
  mySeparateItems( separateItems ),
  mySmartSeparator( smartSeparator )
{
}

/*!
    Destructor
*/
QAD_PopupClientServer::~QAD_PopupClientServer()
{
}

/*!
    Called by popup client when it wants
    to activate popup. The result is the
    popup returned by this object's server
    ( client role ) + the appended popup 
    provided by itself ( server role ).
*/
QPopupMenu*	QAD_PopupClientServer::createPopup()
{
  if ( !myEnablePopup )
    return 0;
  
  QPopupMenu* popupMenu = 0;
  if ( myPopupServer )
    {	/* get the popup provided by my server */
      popupMenu = myPopupServer->createPopup();
    }
  
  if ( !popupMenu ) 
    {	/* there is no popup from my server */
      if ( !myPopup ) myPopup = new QPopupMenu;
      popupMenu = myPopup;
      myOnlyServer = true;
    }
  else 
    {
      if ( myPopup ) myPopup->clear();	
      myPopup = popupMenu;				
      myOnlyServer = false;
    }
  
  /* Attach my popup to the popup of my server */ 
  if ( popupMenu->count() && mySeparateItems )
    {	
      /* Separate my items only if I will really 
	 add some items 
      */
      int sepId, numBefore, numAfter;
      sepId = mySmartSeparator ? popupMenu->insertSeparator(0) : popupMenu->insertSeparator();
      numBefore = popupMenu->count();
      int sepPosBefore = mySmartSeparator ? 0 : numBefore - 1;

      /* add items */
      onCreatePopup();    
        
      numAfter = popupMenu->count();
      int sepPosAfter = popupMenu->indexOf(sepId);
      if ( numAfter > numBefore || ( sepPosAfter != -1 && sepPosAfter && sepPosAfter != numAfter - 1 ) ) { 
	myIDs.append( sepId );
      }
      else if ( sepPosAfter != -1 )
	popupMenu->removeItem( sepId );
    }
  else
    onCreatePopup();    /* add items */
  
  return popupMenu;						
}

/*!
    Deactivates the popup
*/
void QAD_PopupClientServer::destroyPopup()
{
  if ( myPopup ) 
    {	
      /* remove all my items */
      QValueList<int>::ConstIterator it;
      for( it = myIDs.begin(); it != myIDs.end(); ++it )            
	myPopup->removeItem ( *it );	
      
      /* clear list of IDs */
      myIDs.clear();

      if ( myOnlyServer )					
	{	/* popup must be empty now */            
	  QAD_ASSERT_DEBUG_ONLY ( !myPopup->count() );
	  return;
	}
      myPopup = 0;	
    }
  
  /* dispatch to my server */
  if ( myPopupServer )
    myPopupServer->destroyPopup();
}
