//  File      : QAD_ObjectBrowser.h
//  Created   : Thu Jun 14 15:40:24 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
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

#endif
