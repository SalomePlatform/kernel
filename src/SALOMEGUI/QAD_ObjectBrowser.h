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
//  File   : QAD_ObjectBrowser.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef QAD_OBJECTBROWSER_H
#define QAD_OBJECTBROWSER_H

#include "QAD.h"
#include "QAD_Popup.h"

#include "SALOME_InteractiveObject.hxx"
 
// QT Includes
#include <qlist.h>
#include <qvariant.h>
#include <qwidget.h>
#include <qtabwidget.h>
#include <qtoolbutton.h>
#include <qmap.h>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

class QListViewItem;
class QListView;
class QAD_ObjectBrowserItem;
typedef QList<QAD_ObjectBrowserItem> ItemList;
typedef QMap<QString, ItemList>      ItemMap;

class QAD_EXPORT QAD_ObjectBrowser : public QTabWidget, public QAD_PopupClientServer
{
  Q_OBJECT

public:
    QAD_ObjectBrowser( SALOMEDS::Study_var study, QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    virtual ~QAD_ObjectBrowser();

    bool                   eventFilter( QObject* o, QEvent* e );

    void		   setupListView();
    /* popup management */
    void		   onCreatePopup();

    QListView*		   getListView() const;
    QListView*		   getUseCaseView() const;

    void		   Update();
    void		   UpdateUseCaseBrowser();

    void                   unHighlightAll();
    void                   highlight( const Handle(SALOME_InteractiveObject)& IObject, bool highlight );
    bool                   getItems( const Handle(SALOME_InteractiveObject)& IObject, 
                                     QList<QAD_ObjectBrowserItem>& List);
    bool                   getItems( SALOMEDS::SObject_ptr SO, 
                                     QList<QAD_ObjectBrowserItem>& List);
    QAD_ObjectBrowserItem* getItem( const Handle(SALOME_InteractiveObject)& IObject );
    QAD_ObjectBrowserItem* getItem( SALOMEDS::SObject_ptr SO );
    QAD_ObjectBrowserItem* getUCItem( const Handle(SALOME_InteractiveObject)& IObject );
    QAD_ObjectBrowserItem* getUCItem( SALOMEDS::SObject_ptr SO );
    void                   rename( const Handle(SALOME_InteractiveObject)& IObject, QString newName );

    void                   setShowIAPP( bool show );
    void                   setShowInfoColumns( bool show );
    void                   setShowValueColumn( bool show );
    void                   setEnableChronoSort( bool enable );
    void                   showUseCaseBrowser ( bool show );
    void                   autoSizeColumns ( bool autosize );

protected:
    void		   Update( SALOMEDS::SObject_ptr SO,
				   QAD_ObjectBrowserItem* theParentItem );
    void                   UpdateUCItem( SALOMEDS::SObject_var UCObject, 
				         QAD_ObjectBrowserItem* UCItem );
    QAD_ObjectBrowserItem* AddItem( QListView*             theView,
				    const QString&         theName,
				    const QString&         theEntry,
				    const QString&         theIOR, 
				    int                    theType,
				    const QString&         theValue,
				    QAD_ObjectBrowserItem* theAfter = 0 );
    QAD_ObjectBrowserItem* AddItem( QAD_ObjectBrowserItem* theParentItem,
				    const QString&         theName,
				    const QString&         theEntry,
				    const QString&         theIOR, 
				    int                    theType,
				    const QString&         theRef,
				    const QString&         theValue,
				    QAD_ObjectBrowserItem* theAfter = 0 );
    QString                getValueFromObject( SALOMEDS::SObject_ptr SO );
    bool                   hasEditableAttribute( const Handle(SALOME_InteractiveObject)& object );
    bool                   hasTable( const Handle(SALOME_InteractiveObject)& object );

protected slots:
    void		   onSelectedItem();
    void                   onUseCaseSelectedItem();
    void                   onEditAttribute();

    void		   showPopupMenu(QListViewItem*);
    void                   showUseCasePopupMenu(QListViewItem*);
    
    void                   onExpanded (QListViewItem*);
    void                   onCollapsed(QListViewItem*);
    void                   onUseCasePopupMenu( int );

    void                   onUseCaseBtn();

private:
    ItemMap                myListViewMap;
    ItemMap                myUseCaseMap;
    QListView*		   myListView;
    QWidget*               myVBox;
    QListView*		   myUseCaseView;
    QPopupMenu*		   myPopupMenu;
    SALOMEDS::Study_ptr    myStudy;
    QToolButton*           myNewBtn;
    QToolButton*           myAddBtn;
    QToolButton*           myDelBtn;
    QToolButton*           myCurrentBtn;
    QToolButton*           myRenameBtn;
    QToolButton*           myClearBtn;
};


class Btn : public QToolButton
{
  Q_OBJECT
public:
  Btn ( QWidget * parent, const char * name = 0 );
public slots:
  void onClicked();
};

#endif
