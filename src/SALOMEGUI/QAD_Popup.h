//  File      : QAD_Popup.h
//  Created   : 22.09.00
//  Descr     : Popup menu management

//  Author    : UI team
//  Project   : SALOME
//  Module    : SALOMEGUI 
//  Copyright : Open CASCADE
//  $Header$

#ifndef	QAD_POPUP_H
#define QAD_POPUP_H

#include "QAD.h"
#include <qvaluelist.h>
#include <qpopupmenu.h>

/****************************************************************************
**
**  Interface QAD_PopupServer: must be implemented by a class which is going
**  to be a popup server for clients i.e. be able to create a popup menu 
**
*****************************************************************************/
class QAD_EXPORT QAD_PopupServer 
{
public:
  QAD_PopupServer();
  virtual ~QAD_PopupServer();

public:
  virtual QPopupMenu*  createPopup();	
  virtual void         destroyPopup();
  void                 enablePopup( bool );

protected:
  virtual void         onCreatePopup () = 0;	/* addition of menu items goes here */

protected:
  QPopupMenu*		myPopup;      /* resulting popup menu */
  bool            myEnablePopup;      /* enable/disable popup creation */    
};

/****************************************************************************
**
**  Class QAD_PopupClient: must be inherited by a class which is going
**  to be a popup client i.e. be able to show its server's popup menu
**
*****************************************************************************/
class QAD_EXPORT QAD_PopupClient
{
public:
	QAD_PopupClient();
	virtual ~QAD_PopupClient();

public:
	void			setPopupServer ( QAD_PopupServer* );
	QAD_PopupServer*	getPopupServer () const;

protected:
	QAD_PopupServer*	myPopupServer;	
};

/****************************************************************************
**
**  Class QAD_PopupClientServer: both client and server. Can be used as
**  a server only or as a client only ( if server is not set ).
**
*****************************************************************************/
class QAD_EXPORT QAD_PopupClientServer : public QAD_PopupClient, 
  public QAD_PopupServer
{
public:
	QAD_PopupClientServer( bool separateItems = true, bool smartSeparator = true );
	~QAD_PopupClientServer();

public:
	QPopupMenu*		createPopup();
	void			destroyPopup();

protected:
	QValueList<int> myIDs;				// ids of the items added by myself as a server 
	bool			myOnlyServer;		// true if I`m fully responsible for the menu  
	bool			mySeparateItems;	// to insert separator	before my items
	bool                    mySmartSeparator;       // if true then separator is inserted after own items,
							// not at the end of menu
};

#endif
