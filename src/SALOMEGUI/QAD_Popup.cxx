using namespace std;
//  File      : QAD_Popup.cxx
//  Created   : 22.09.00
//  Descr     : Popup menu management

//  Author    : UI team
//  Project   : SALOME
//  Module    : SALOMEGUI 
//  Copyright : Open CASCADE
//  $Header$

#include "QAD.h"
#include "QAD_Popup.h"
#include "utilities.h"

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
