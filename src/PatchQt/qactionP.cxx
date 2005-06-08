/////////////////////////////////////////////////////////////////////////////
// Module      : PatchQt
// File        : qactionP.cxx
// Description : the patch for Qt's QAction class (qaction.cpp)
/////////////////////////////////////////////////////////////////////////////

/****************************************************************************
** $Id$
**
** Implementation of QAction class
**
** Created : 000000
**
** Copyright (C) 2000 Trolltech AS.  All rights reserved.
**
** This file is part of the widgets module of the Qt GUI Toolkit.
**
** This file may be distributed under the terms of the Q Public License
** as defined by Trolltech AS of Norway and appearing in the file
** LICENSE.QPL included in the packaging of this file.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid Qt Enterprise Edition or Qt Professional Edition
** licenses may use this file in accordance with the Qt Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.trolltech.com/pricing.html or email sales@trolltech.com for
**   information about Qt Commercial License Agreements.
** See http://www.trolltech.com/qpl/ for QPL licensing information.
** See http://www.trolltech.com/gpl/ for GPL licensing information.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#include "qactionP.h"

#if (QT_VERSION < 0x030303)

#include "qactionP_moc.cxx"

#ifndef QT_NO_ACTION

#include <qtoolbar.h>
#include <qptrlist.h>
#include <qpopupmenu.h>
#include <qaccel.h>
#include <qtoolbutton.h>
#include <qcombobox.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qstatusbar.h>
#include <qobjectlist.h>


/*!
  \class QActionP qaction.h
  \ingroup basic
  \ingroup application
  \mainclass
  \brief The QActionP class provides an abstract user interface
  action that can appear both in menus and tool bars.

  In GUI applications many commands can be invoked via a menu option, a
  toolbar button and a keyboard accelerator. Since the same action must
  be performed regardless of how the action was invoked, and since the
  menu and toolbar should be kept in sync, it is useful to represent a
  command as an \e action. An action can be added to a menu and a
  toolbar and will automatically keep them in sync. For example, if the
  user presses a Bold toolbar button the Bold menu item will
  automatically be checked.

  A QActionP may contain an icon, a menu text, an accelerator, a status
  text, a whats this text and a tool tip. Most of these can be set in
  the constructor. They can also be set independently with setIconSet(),
  setText(), setMenuText(), setToolTip(), setStatusTip(), setWhatsThis()
  and setAccel(), respectively.

  An action may be a toggle action e.g. a Bold toolbar button, or a
  command action, e.g. 'Open File' to invoke an open file dialog.
  Toggle actions emit the toggled() signal when their state changes.
  Both command and toggle actions emit the activated() signal when they
  are invoked. Use setToggleAction() to set an action's toggled status.
  To see if an action is a toggle action use isToggleAction(). A toggle
  action may be "on", isOn() returns TRUE, or "off", isOn() returns
  FALSE.

  Actions are added to widgets (menus or toolbars) using addTo(), and
  removed using removeFrom().

  Once a QActionP has been created it should be added to the relevant
  menu and toolbar and then connected to the slot which will perform
  the action. For example:

  \quotefile action/application.cpp
  \skipto Save File
  \printuntil connect

  We create a "File Save" action with a menu text of "&Save" and
  \e{Ctrl+S} as the keyboard accelerator. We connect the
  fileSaveAction's activated() signal to our own save() slot. Note that at
  this point there is no menu or toolbar action, we'll add them next:

  \skipto new QToolBar
  \printline
  \skipto fileSaveAction->addTo
  \printline
  \skipto new QPopupMenu
  \printuntil insertItem
  \skipto fileSaveAction->addTo
  \printline

  We create a toolbar and add our fileSaveAction to it. Similarly we
  create a menu, add a top-level menu item, and add our
  fileSaveAction.

  (See the \link simple-application-action.html Simple Application
  Walkthrough featuring QActionP \endlink for a detailed example.)

  We recommend that actions are created as children of the window that
  they are used in. In most cases actions will be children of the
  application's main window.

  To prevent recursion, don't create an action as a child of a widget
  that the action is later added to.
*/


class QActionPPrivate
{
public:
    QActionPPrivate();
    ~QActionPPrivate();
    QIconSet *iconset;
    QString text;
    QString menutext;
    QString tooltip;
    QString statustip;
    QString whatsthis;
    QKeySequence key;
#ifndef QT_NO_ACCEL
    QAccel* accel;
    int accelid;
#endif
    uint enabled : 1;
    uint toggleaction :1;
    uint on : 1;
#ifndef QT_NO_TOOLTIP
    QToolTipGroup* tipGroup;
#endif

    struct MenuItem {
	MenuItem():popup(0),id(0){}
	QPopupMenu* popup;
	int id;
    };
    // ComboItem is only necessary for actions that are
    // in dropdown/exclusive actiongroups. The actiongroup
    // will clean this up
    struct ComboItem {
	ComboItem():combo(0), id(0) {}
	QComboBox *combo;
	int id;
    };
    QPtrList<MenuItem> menuitems;
    QPtrList<QToolButton> toolbuttons;
    QPtrList<ComboItem> comboitems;

    enum Update { Everything, Icons, State }; // Everything means everything but icons and state
    void update( Update upd = Everything );

    QString menuText() const;
    QString toolTip() const;
    QString statusTip() const;
};

QActionPPrivate::QActionPPrivate()
{
    iconset = 0;
#ifndef QT_NO_ACCEL
    accel = 0;
    accelid = 0;
#endif
    key = 0;
    enabled = 1;
    toggleaction  = 0;
    on = 0;
    menuitems.setAutoDelete( TRUE );
    comboitems.setAutoDelete( TRUE );
#ifndef QT_NO_TOOLTIP
    tipGroup = new QToolTipGroup( 0 );
#endif
}

QActionPPrivate::~QActionPPrivate()
{
    QPtrListIterator<QToolButton> ittb( toolbuttons );
    QToolButton *tb;

    while ( ( tb = ittb.current() ) ) {
	++ittb;
	delete tb;
    }

    QPtrListIterator<QActionPPrivate::MenuItem> itmi( menuitems);
    QActionPPrivate::MenuItem* mi;
    while ( ( mi = itmi.current() ) ) {
	++itmi;
	QPopupMenu* menu = mi->popup;
	if ( menu->findItem( mi->id ) )
	    menu->removeItem( mi->id );
    }

#ifndef QT_NO_ACCEL
    delete accel;
#endif
    delete iconset;
#ifndef QT_NO_TOOLTIP
    delete tipGroup;
#endif
}

void QActionPPrivate::update( Update upd )
{
    for ( QPtrListIterator<MenuItem> it( menuitems); it.current(); ++it ) {
	MenuItem* mi = it.current();
	QString t = menuText();
#ifndef QT_NO_ACCEL
	if ( key )
	    t += '\t' + QAccel::keyToString( key );
#endif
	switch ( upd ) {
	case State:
	    mi->popup->setItemEnabled( mi->id, enabled );
	    if ( toggleaction )
		mi->popup->setItemChecked( mi->id, on );
	    break;
	case Icons:
	    if ( iconset )
		mi->popup->changeItem( mi->id, *iconset, t );
	    break;
	default:
	    mi->popup->changeItem( mi->id, t );
	    if ( !whatsthis.isEmpty() )
		mi->popup->setWhatsThis( mi->id, whatsthis );
	    if ( toggleaction ) {
		mi->popup->setCheckable( TRUE );
		mi->popup->setItemChecked( mi->id, on );
	    }
	}
    }
    for ( QPtrListIterator<QToolButton> it2( toolbuttons); it2.current(); ++it2 ) {
	QToolButton* btn = it2.current();
	switch ( upd ) {
	case State:
	    btn->setEnabled( enabled );
	    if ( toggleaction )
		btn->setOn( on );
	    break;
	case Icons:
	    if ( iconset )
		btn->setIconSet( *iconset );
	    break;
	default:
	    btn->setToggleButton( toggleaction );
	    if ( !text.isEmpty() )
		btn->setTextLabel( text, FALSE );
#ifndef QT_NO_TOOLTIP
	    QToolTip::remove( btn );
	    QToolTip::add( btn, toolTip(), tipGroup, statusTip() );
#endif
#ifndef QT_NO_WHATSTHIS
	    QWhatsThis::remove( btn );
	    if ( !whatsthis.isEmpty() )
		QWhatsThis::add( btn, whatsthis );
#endif
	}
    }
    // Only used by actiongroup
    for ( QPtrListIterator<ComboItem> it3( comboitems ); it3.current(); ++it3 ) {
	ComboItem *ci = it3.current();
	if ( !ci->combo )
	    return;
	if ( iconset )
	    ci->combo->changeItem( iconset->pixmap(), text, ci->id );
	else
	    ci->combo->changeItem( text, ci->id );
    }
    // VSR : enable/disable accel according to action state
#ifndef QT_NO_ACCEL
    if ( upd == State && accel )
      accel->setItemEnabled( key, enabled );
#endif
}

QString QActionPPrivate::menuText() const
{
    if ( menutext.isNull() )
	return text;
    return menutext;
}

QString QActionPPrivate::toolTip() const
{
    if ( tooltip.isNull() ) {
#ifndef QT_NO_ACCEL
	if ( accel )
	    return text + " (" + QAccel::keyToString( accel->key( accelid )) + ")";
#endif
	return text;
    }
    return tooltip;
}

QString QActionPPrivate::statusTip() const
{
    if ( statustip.isNull() )
	return toolTip();
    return statustip;
}



/*!
  Constructs an action with parent \a parent and name \a name.

  If \a toggle is TRUE the action will be a toggle action, otherwise it
  will be a command action.

  If \a parent is a QActionPGroup, the new action inserts itself into \a parent.

  For accelerators and status tips to work, \a parent must either be a
  widget, or an action group whose parent is a widget.
*/
QActionP::QActionP( QObject* parent, const char* name, bool toggle )
    : QObject( parent, name )
{
    d = new QActionPPrivate;
    d->toggleaction = toggle;
    init();
}


/*!
  This constructor creates an action with the following properties:
  the description \a text, the icon or iconset \a icon, the menu text
  \a menuText and keyboard accelerator \a accel. It is a child of \a parent
  and named \a name. If \a toggle is TRUE the action will be a toggle
  action, otherwise it will be a command action.

  If  \a parent is a QActionPGroup, the action automatically becomes a
  member of it.

  For accelerators and status tips to work, \a parent must either be a
  widget, or an action group whose parent is a widget.

  The \a text and \a accel will be used for tool tips and status tips
  unless you provide specific text for these using setToolTip() and
  setStatusTip().
*/
QActionP::QActionP( const QString& text, const QIconSet& icon, const QString& menuText, QKeySequence accel, QObject* parent, const char* name, bool toggle )
    : QObject( parent, name )
{
    d = new QActionPPrivate;
    d->toggleaction = toggle;
    if ( !icon.isNull() )
	setIconSet( icon );

    d->text = text;
    d->menutext = menuText;
    setAccel( accel );
    init();
}

/*! This constructor results in an iconless action with the description
  \a text, the menu text \a menuText and the keyboard accelerator \a accel.
  Its parent is \a parent and its name \a
  name. If \a toggle is TRUE the action will be a toggle
  action, otherwise it will be a command action.

  The action automatically becomes a member of \a parent if \a parent
  is a QActionPGroup.

  For accelerators and status tips to work, \a parent must either be a
  widget, or an action group whose parent is a widget.

  The \a text and \a accel will be used for tool tips and status tips
  unless you provide specific text for these using setToolTip() and
  setStatusTip().
*/
QActionP::QActionP( const QString& text, const QString& menuText, QKeySequence accel, QObject* parent, const char* name, bool toggle )
    : QObject( parent, name )
{
    d = new QActionPPrivate;
    d->toggleaction = toggle;
    d->text = text;
    d->menutext = menuText;
    setAccel( accel );
    init();
}

/*!
  \internal
*/
void QActionP::init()
{
    if ( parent() && parent()->inherits("QActionPGroup") ) {
	((QActionPGroup*) parent())->add( this );		// insert into action group
    }
}

/*! Destroys the object and frees allocated resources. */

QActionP::~QActionP()
{
    delete d;
}

/*! \property QActionP::iconSet
  \brief  the action's icon

  The icon is used as the tool button icon and in the menu to the left
  of the menu text. There is no default icon.

  (See the action/toggleaction/toggleaction.cpp example.)

*/
void QActionP::setIconSet( const QIconSet& icon )
{
    if ( icon.isNull() )
	return;

    register QIconSet *i = d->iconset;
    d->iconset = new QIconSet( icon );
    delete i;
    d->update( QActionPPrivate::Icons );
}

QIconSet QActionP::iconSet() const
{
    if ( d->iconset )
	return *d->iconset;
    return QIconSet();
}

/*! \property QActionP::text
  \brief the action's descriptive text

  If \l QMainWindow::usesTextLabel is TRUE, the text appears as a
  label in the relevant tool button. It also serves as the default text
  in menus and tool tips if these have not been specifically defined. There
  is no default text.

  \sa setMenuText() setToolTip() setStatusTip()
*/
void QActionP::setText( const QString& text )
{
    d->text = text;
    d->update();
}

QString QActionP::text() const
{
    return d->text;
}


/*! \property QActionP::menuText
  \brief the action's menu text

    If the action is added to a menu the menu option will consist of
    the icon (if there is one), the menu text and the accelerator (if
    there is one). If the menu text is not explicitly set in the
    constructor or by using setMenuText() the action's description
    text will be used as the menu text. There is no default menu text.

  \sa text
*/
void QActionP::setMenuText( const QString& text ) { d->menutext = text;
    d->update(); }

QString QActionP::menuText() const { return d->menuText(); }

/*!
  \property QActionP::toolTip \brief the action's tool tip

  This text is used for the tool tip. If no status tip has been set
  the tool tip will be used for the status tip.

  If no tool tip is specified the action's text is used, and if that
  hasn't been specified the description text is used as the tool tip
  text.

  There is no default tool tip text.

  \sa setStatusTip() setAccel()
*/
void QActionP::setToolTip( const QString& tip )
{
    d->tooltip = tip;
    d->update();
}

QString QActionP::toolTip() const
{
    return d->toolTip();
}

/*! \property QActionP::statusTip
  \brief the action's status tip

  The statusTip is displayed on all status bars that this action's
  toplevel parent widget provides.

  If no status tip is defined, the action uses the tool tip text.

  There is no default tooltip text.

  \sa setStatusTip() setToolTip()
*/
//#### Please reimp for QActionPGroup!
//#### For consistency reasons even action groups should show
//#### status tips (as they already do with tool tips)
//#### Please change QActionPGroup class doc appropriately after
//#### reimplementation.
void QActionP::setStatusTip( const QString& tip )
{
    d->statustip = tip;
    d->update();
}

QString QActionP::statusTip() const
{
    return d->statusTip();
}

/*!\property QActionP::whatsThis
  \brief the action's "What's This?" help text

  The whats this text is used to provide a brief description of the
  action. The text may contain rich text (i.e. HTML tags -- see
  QStyleSheet for the list of supported tags). There is no default
  "What's This" text.

  \sa QWhatsThis
*/
void QActionP::setWhatsThis( const QString& whatsThis )
{
    if ( d->whatsthis == whatsThis )
	return;
    d->whatsthis = whatsThis;
#ifndef QT_NO_ACCEL
    if ( !d->whatsthis.isEmpty() && d->accel )
	d->accel->setWhatsThis( d->accelid, d->whatsthis );
#endif
    d->update();
}

QString QActionP::whatsThis() const
{
    return d->whatsthis;
}


/*! \property QActionP::accel
  \brief the action's accelerator key

  The keycodes can be found in \l Qt::Key and \l
  Qt::Modifier. There is no default accelerator key.


*/
//#### Please reimp for QActionPGroup!
//#### For consistency reasons even QActionPGroups should respond to
//#### their accelerators and e.g. open the relevant submenu.
//#### Please change appropriate QActionPGroup class doc after
//#### reimplementation.
void QActionP::setAccel( const QKeySequence& key )
{
    d->key = key;
#ifndef QT_NO_ACCEL
    delete d->accel;
    d->accel = 0;
#endif

    if ( !(int)key ) {
	d->update();
	return;
    }

#ifndef QT_NO_ACCEL
    QObject* p = parent();
    while ( p && !p->isWidgetType() ) {
	p = p->parent();
    }
    if ( p ) {
	d->accel = new QAccel( (QWidget*)p, this, "qt_action_accel" );
	d->accelid = d->accel->insertItem( d->key );
	d->accel->connectItem( d->accelid, this, SLOT( internalActivation() ) );
	if ( !d->whatsthis.isEmpty() )
	    d->accel->setWhatsThis( d->accelid, d->whatsthis );
    }
#if defined(QT_CHECK_STATE)
    else
	qWarning( "QActionP::setAccel()  (%s) requires widget in parent chain.", name( "unnamed" ) );
#endif
#endif
    d->update();
}


QKeySequence QActionP::accel() const
{
    return d->key;
}


/*!
  \property QActionP::toggleAction
  \brief whether the action is a toggle action

  A toggle action is one which has an on/off state. For example a Bold
  toolbar button is either on or off. An action which is not a toggle
  action is a command action; a command action is simply executed.
  This property's default is FALSE.

  In some situations, the state of one toggle action should depend on
  the state of others. For example, "Left Align", "Center" and "Right
  Align" toggle actions are mutually exclusive. To achieve exclusive
  toggling, add the relevant toggle actions to a QActionPGroup with the
  \l QActionPGroup::exclusive property set to TRUE.

*/
void QActionP::setToggleAction( bool enable )
{
    if ( enable == (bool)d->toggleaction )
	return;

    if ( !enable )
	d->on = FALSE;

    d->toggleaction = enable;
    d->update();
}

bool QActionP::isToggleAction() const
{
    return d->toggleaction;
}

/*!
  Toggles the state of a toggle action.

  \sa on, toggled(), isToggleAction()
*/
void QActionP::toggle()
{
    if ( !isToggleAction() ) {
#if defined(QT_CHECK_STATE)
	qWarning( "QActionP::toggle() (%s) Only toggle actions "
		  "may be switched", name( "unnamed" ) );
#endif
	return;
    }
    setOn( !isOn() );
}

/*!
  \property QActionP::on
  \brief whether a toggle action is on

  This property is always on (TRUE) for command actions and
  \l{QActionPGroup}s; setOn() has no effect on them. For action's where
  isToggleAction() is TRUE, this property's default value is off
  (FALSE).

  \sa toggleAction
*/
void QActionP::setOn( bool enable )
{
    if ( !isToggleAction() ) {
#if defined(QT_CHECK_STATE)
	qWarning( "QActionP::setOn() (%s) Only toggle actions "
		  "may be switched", name( "unnamed" ) );
#endif
	return;
    }
    if ( enable == (bool)d->on )
	return;
    d->on = enable;
    d->update( QActionPPrivate::State );
    emit toggled( enable );
}

bool QActionP::isOn() const
{
    return d->on;
}

/*! \property QActionP::enabled
  \brief whether the action is enabled

  Disabled actions can't be chosen by the user. They don't
  disappear from the menu/tool bar but are displayed in a way which
  indicates that they are unavailable, e.g. they might be displayed
  greyed out.

  What's this? help on disabled actions is still available
  provided the \l QActionP::whatsThis property is set.

*/
void QActionP::setEnabled( bool enable )
{
    d->enabled = enable;
#ifndef QT_NO_ACCEL
    if ( d->accel )
	d->accel->setEnabled( enable );
#endif
    d->update( QActionPPrivate::State );
}

bool QActionP::isEnabled() const
{
    return d->enabled;
}

/*! \internal
*/
void QActionP::internalActivation()
{
    if ( isToggleAction() )
	setOn( !isOn() );
    emit activated();
}

/*! \internal
*/
void QActionP::toolButtonToggled( bool on )
{
    if ( !isToggleAction() )
	return;
    setOn( on );
}

/*! Adds this action to widget \a w.

  Currently actions may be added to QToolBar and QPopupMenu widgets.

  An action added to a tool bar is automatically displayed
  as a tool button; an action added to a pop up menu appears
  as a menu option.

  addTo() returns TRUE if the action was added successfully and FALSE
  otherwise. (If \a w is not a QToolBar or QPopupMenu the action will
  not be added and FALSE will be returned.)

  \sa removeFrom()
*/
bool QActionP::addTo( QWidget* w )
{
#ifndef QT_NO_TOOLBAR
    if ( w->inherits( "QToolBar" ) ) {
	if ( !qstrcmp( name(), "qt_separator_action" ) ) {
	    ((QToolBar*)w)->addSeparator();
	} else {
	    QCString bname = name() + QCString( "_action_button" );
	    QToolButton* btn = new QToolButton( (QToolBar*) w, bname );
	    addedTo( btn, w );
	    btn->setToggleButton( d->toggleaction );
	    d->toolbuttons.append( btn );
	    if ( d->iconset )
		btn->setIconSet( *d->iconset );
	    d->update( QActionPPrivate::State );
	    d->update( QActionPPrivate::Everything );
	    connect( btn, SIGNAL( clicked() ), this, SIGNAL( activated() ) );
	    connect( btn, SIGNAL( toggled(bool) ), this, SLOT( toolButtonToggled(bool) ) );
	    connect( btn, SIGNAL( destroyed() ), this, SLOT( objectDestroyed() ) );
#ifndef QT_NO_TOOLTIP
	    connect( d->tipGroup, SIGNAL(showTip(const QString&)), this, SLOT(showStatusText(const QString&)) );
	    connect( d->tipGroup, SIGNAL(removeTip()), this, SLOT(clearStatusText()) );
#endif
	}
    } else
#endif
    if ( w->inherits( "QPopupMenu" ) ) {
	if ( !qstrcmp( name(), "qt_separator_action" ) ) {
	    ((QPopupMenu*)w)->insertSeparator();
	} else {
	    QActionPPrivate::MenuItem* mi = new QActionPPrivate::MenuItem;
	    mi->popup = (QPopupMenu*) w;
	    QIconSet* diconset = d->iconset;
	    if ( diconset )
		mi->id = mi->popup->insertItem( *diconset, QString::fromLatin1("") );
	    else
		mi->id = mi->popup->insertItem( QString::fromLatin1("") );
	    addedTo( mi->popup->indexOf( mi->id ), mi->popup );
	    mi->popup->connectItem( mi->id, this, SLOT(internalActivation()) );
	    d->menuitems.append( mi );
	    d->update( QActionPPrivate::State );
	    d->update( QActionPPrivate::Everything );
	    w->topLevelWidget()->className();
	    connect( mi->popup, SIGNAL(highlighted( int )), this, SLOT(menuStatusText( int )) );
	    connect( mi->popup, SIGNAL(aboutToHide()), this, SLOT(clearStatusText()) );
	    connect( mi->popup, SIGNAL( destroyed() ), this, SLOT( objectDestroyed() ) );
	}
    // Makes only sense when called by QActionPGroup::addTo
    } else if ( w->inherits( "QComboBox" ) ) {
	if ( qstrcmp( name(), "qt_separator_action" ) ) {
	    QActionPPrivate::ComboItem *ci = new QActionPPrivate::ComboItem;
	    ci->combo = (QComboBox*)w;
	    connect( ci->combo, SIGNAL( destroyed() ), this, SLOT( objectDestroyed() ) );
	    ci->id = ci->combo->count();
	    if ( d->iconset )
		ci->combo->insertItem( d->iconset->pixmap(), text() );
	    else
		ci->combo->insertItem( text() );
	    d->comboitems.append( ci );
	}
    } else {
	qWarning( "QActionP::addTo(), unknown object" );
	return FALSE;
    }
    return TRUE;
}

/*! This function is called from the addTo() function when it created
  a widget (\a actionWidget) for the action in the \a container.
*/

void QActionP::addedTo( QWidget *actionWidget, QWidget *container )
{
    Q_UNUSED( actionWidget );
    Q_UNUSED( container );
}

/*! \overload

  This function is called from the addTo() function when it created
  a menu item at the index \a index in the popup menu \a menu.
*/

void QActionP::addedTo( int index, QPopupMenu *menu )
{
    Q_UNUSED( index );
    Q_UNUSED( menu );
}

/*! Sets the status message to \a text */
void QActionP::showStatusText( const QString& text )
{
#ifndef QT_NO_STATUSBAR
    // find out whether we are clearing the status bar by the popup that actually set the text
    static QPopupMenu *lastmenu = 0;
    QObject *s = (QObject*)sender();
    if ( s ) {
	QPopupMenu *menu = (QPopupMenu*)s->qt_cast( "QPopupMenu" );
	if ( menu && !!text )
	    lastmenu = menu;
	else if ( menu && text.isEmpty() ) {
	    if ( lastmenu && menu != lastmenu )
		return;
	    lastmenu = 0;
	}
    }

    QObject* par = parent();
    QObject* lpar = 0;
    QStatusBar *bar = 0;
    while ( par && !bar ) {
	lpar = par;
	bar = (QStatusBar*)par->child( 0, "QStatusBar", FALSE );
	par = par->parent();
    }
    if ( !bar && lpar ) {
	QObjectList *l = lpar->queryList( "QStatusBar" );
	if ( !l )
	    return;
	// #### hopefully the last one is the one of the mainwindow...
	bar = (QStatusBar*)l->last();
	delete l;
    }
    if ( bar ) {
	if ( text.isEmpty() )
	    bar->clear();
	else
	    bar->message( text );
    }
#endif
}

/*! Sets the status message to the menu item's status text, or
  to the tooltip, if there is no status text.
*/
void QActionP::menuStatusText( int id )
{
    QString text;
    QPtrListIterator<QActionPPrivate::MenuItem> it( d->menuitems);
    QActionPPrivate::MenuItem* mi;
    while ( ( mi = it.current() ) ) {
	++it;
	if ( mi->id == id ) {
	    text = statusTip();
	    break;
	}
    }

    if ( !text.isEmpty() )
	showStatusText( text );
}

/*! Clears the status text.
*/
void QActionP::clearStatusText()
{
    showStatusText( QString::null );
}

/*!
  Removes the action from widget \a w.

  Returns TRUE if the action was removed successfully; otherwise
  returns FALSE.

  \sa addTo()
*/
bool QActionP::removeFrom( QWidget* w )
{
#ifndef QT_NO_TOOLBAR
    if ( w->inherits( "QToolBar" ) ) {
	QPtrListIterator<QToolButton> it( d->toolbuttons);
	QToolButton* btn;
	while ( ( btn = it.current() ) ) {
	    ++it;
	    if ( btn->parentWidget() == w ) {
		d->toolbuttons.removeRef( btn );
		disconnect( btn, SIGNAL( destroyed() ), this, SLOT( objectDestroyed() ) );
		delete btn;
		// no need to disconnect from statusbar
	    }
	}
    } else
#endif
    if ( w->inherits( "QPopupMenu" ) ) {
	QPtrListIterator<QActionPPrivate::MenuItem> it( d->menuitems);
	QActionPPrivate::MenuItem* mi;
	while ( ( mi = it.current() ) ) {
	    ++it;
	    if ( mi->popup == w ) {
		disconnect( mi->popup, SIGNAL(highlighted( int )), this, SLOT(menuStatusText(int)) );
		disconnect( mi->popup, SIGNAL(aboutToHide()), this, SLOT(clearStatusText()) );
		disconnect( mi->popup, SIGNAL( destroyed() ), this, SLOT( objectDestroyed() ) );
		mi->popup->removeItem( mi->id );
		d->menuitems.removeRef( mi );
	    }
	}
    } else if ( w->inherits( "QComboBox" ) ) {
	QPtrListIterator<QActionPPrivate::ComboItem> it( d->comboitems );
	QActionPPrivate::ComboItem *ci;
	while ( ( ci = it.current() ) ) {
	    ++it;
	    if ( ci->combo == w ) {
		disconnect( ci->combo, SIGNAL(destroyed()), this, SLOT(objectDestroyed()) );
		d->comboitems.removeRef( ci );
	    }
	}
    } else {
	qWarning( "QActionP::removeFrom(), unknown object" );
	return FALSE;
    }
    return TRUE;
}

/*!
  \internal
*/
void QActionP::objectDestroyed()
{
    const QObject* obj = sender();
    QPtrListIterator<QActionPPrivate::MenuItem> it( d->menuitems );
    QActionPPrivate::MenuItem* mi;
    while ( ( mi = it.current() ) ) {
	++it;
	if ( mi->popup == obj )
	    d->menuitems.removeRef( mi );
    }
    QActionPPrivate::ComboItem *ci;
    QPtrListIterator<QActionPPrivate::ComboItem> it2( d->comboitems );
    while ( ( ci = it2.current() ) ) {
	++it2;
	if ( ci->combo == obj )
	    d->comboitems.removeRef( ci );
    }
    d->toolbuttons.removeRef( (QToolButton*) obj );
}

/*! \fn void QActionP::activated()

  This signal is emitted when an action is activated by the user, i.e.
  when the user clicks a menu option or a toolbar button or presses an
  action's accelerator key combination.

  Connect to this signal for command actions. Connect to the toggled()
  signal for toggle actions.
*/

/*! \fn void QActionP::toggled(bool)

  This signal is emitted when a toggle action changes state;
  command actions and QActionPGroups don't emit toggled().

  The argument denotes the new state; i.e. TRUE
  if the toggle action was switched on and FALSE if
  it was switched off.

  To trigger a user command depending on whether a toggle action has
  been switched on or off connect it to a slot that takes a bool to
  indicate the state, e.g.

  \quotefile action/toggleaction/toggleaction.cpp
  \skipto QMainWindow * window
  \printline QMainWindow * window
  \skipto labelonoffaction
  \printline labelonoffaction
  \skipto connect
  \printuntil setUsesTextLabel

  \sa activated() setToggleAction() setOn()
*/



class QActionPGroupPrivate
{
public:
    uint exclusive: 1;
    uint dropdown: 1;
    QPtrList<QActionP> actions;
    QActionP* selected;
    QActionP* separatorAction;

    struct MenuItem {
	MenuItem():popup(0),id(0){}
	QPopupMenu* popup;
	int id;
    };

    QPtrList<QComboBox> comboboxes;
    QPtrList<QToolButton> menubuttons;
    QPtrList<MenuItem> menuitems;
    QPtrList<QPopupMenu> popupmenus;

    void update( const QActionPGroup * );
};

void QActionPGroupPrivate::update( const QActionPGroup* that )
{
    for ( QPtrListIterator<QActionP> it( actions ); it.current(); ++it ) {
	it.current()->setEnabled( that->isEnabled() );
    }
    for ( QPtrListIterator<QComboBox> cb( comboboxes ); cb.current(); ++cb ) {
	cb.current()->setEnabled( that->isEnabled() );

#ifndef QT_NO_TOOLTIP
	QToolTip::remove( cb.current() );
	if ( !!that->toolTip() )
	    QToolTip::add( cb.current(), that->toolTip() );
#endif
#ifndef QT_NO_WHATSTHIS
	QWhatsThis::remove( cb.current() );
	if ( !!that->whatsThis() )
	    QWhatsThis::add( cb.current(), that->whatsThis() );
#endif
    }
    for ( QPtrListIterator<QToolButton> mb( menubuttons ); mb.current(); ++mb ) {
	mb.current()->setEnabled( that->isEnabled() );

	if ( !that->text().isNull() )
	    mb.current()->setTextLabel( that->text() );
	if ( !that->iconSet().isNull() )
	    mb.current()->setIconSet( that->iconSet() );

#ifndef QT_NO_TOOLTIP
	QToolTip::remove( mb.current() );
	if ( !!that->toolTip() )
	    QToolTip::add( mb.current(), that->toolTip() );
#endif
#ifndef QT_NO_WHATSTHIS
	QWhatsThis::remove( mb.current() );
	if ( !!that->whatsThis() )
	    QWhatsThis::add( mb.current(), that->whatsThis() );
#endif
    }
    for ( QPtrListIterator<QActionPGroupPrivate::MenuItem> pu( menuitems ); pu.current(); ++pu ) {
	QWidget* parent = pu.current()->popup->parentWidget();
	if ( parent->inherits( "QPopupMenu" ) ) {
	    QPopupMenu* ppopup = (QPopupMenu*)parent;
	    ppopup->setItemEnabled( pu.current()->id, that->isEnabled() );
	} else {
	    pu.current()->popup->setEnabled( that->isEnabled() );
	}
    }
    for ( QPtrListIterator<QPopupMenu> pm( popupmenus ); pm.current(); ++pm ) {
	QPopupMenu *popup = pm.current();
	QPopupMenu *parent = popup->parentWidget()->inherits( "QPopupMenu" ) ? (QPopupMenu*)popup->parentWidget() : 0;
	if ( !parent )
	    continue;

	int index;
	parent->findPopup( popup, &index );
	int id = parent->idAt( index );
	parent->changeItem( id, that->iconSet(), that->menuText() );
	parent->setItemEnabled( id, that->isEnabled() );
	parent->setAccel( that->accel(), id );
    }
}

/*!
  \class QActionPGroup qaction.h
  \ingroup basic
  \ingroup application

  \brief The QActionPGroup class groups actions together.

  In some situations it is useful to group actions together. For
  example, if you have a left justify action, a right justify action
  and a center action, only one of these actions should be active at
  any one time, and one simple way of achieving this is to group the
  actions together in an action group and call setExclusive(TRUE).

  An action group can also be added to a menu or a toolbar as a single
  unit, with all the actions within the action group appearing as
  separate menu options and toolbar buttons.

  Here's an example from examples/textedit:
  \quotefile textedit/textedit.cpp
  \skipto QActionPGroup
  \printuntil connect

  We create a new action  group and call setExclusive() to ensure that
  only one of the actions in the group is ever active at any one time.
  We then connect the group's selected() signal to our textAlign() slot.

  \printuntil actionAlignLeft->setToggleAction

  We create a left align action, add it to the toolbar and the menu
  and make it a toggle action. We create center and right align
  actions in exactly the same way.

  \omit
  A QActionPGroup emits an activated() signal when one of its actions
  is activated.
  \endomit
  The actions in an action group emit their activated()
  (and for toggle actions, toggled()) signals as usual.

  The setExclusive() function is used to ensure that only one action
  is active at any one time: it should be used with actions which have
  their \c toggleAction set to TRUE.

  Action group actions appear as individual menu options and toolbar
  buttons. For exclusive action groups use setUsesDropDown() to
  display the actions in a subwidget of any widget the action group is
  added to. For example, the actions would appear in a combobox in a
  toolbar or as a submenu in a menu.

  Actions can be added to an action group using add(), but normally
  they are added by creating the action with the action group as
  parent. Actions can have separators dividing them using
  addSeparator(). Action groups are added to widgets with addTo().

*/

/*! Constructs an action group with parent \a parent and name \a name.

    If \a exclusive is TRUE only one toggle action in the group will
    ever be active.

*/
QActionPGroup::QActionPGroup( QObject* parent, const char* name, bool exclusive )
    : QActionP( parent, name )
{
    d = new QActionPGroupPrivate;
    d->exclusive = exclusive;
    d->dropdown = FALSE;
    d->selected = 0;
    d->separatorAction = 0;

    connect( this, SIGNAL(selected(QActionP*)), SLOT(internalToggle(QActionP*)) );
}

/*! Destroys the object and frees allocated resources. */

QActionPGroup::~QActionPGroup()
{
    QPtrListIterator<QActionPGroupPrivate::MenuItem> mit( d->menuitems );
    while ( mit.current() ) {
	QActionPGroupPrivate::MenuItem *mi = mit.current();
	++mit;
	if ( mi->popup )
	    mi->popup->disconnect( SIGNAL(destroyed()), this, SLOT(objectDestroyed()) );
    }

    QPtrListIterator<QComboBox> cbit( d->comboboxes );
    while ( cbit.current() ) {
	QComboBox *cb = cbit.current();
	++cbit;
	cb->disconnect(  SIGNAL(destroyed()), this, SLOT(objectDestroyed()) );
    }
    QPtrListIterator<QToolButton> mbit( d->menubuttons );
    while ( mbit.current() ) {
	QToolButton *mb = mbit.current();
	++mbit;
	mb->disconnect(  SIGNAL(destroyed()), this, SLOT(objectDestroyed()) );
    }
    QPtrListIterator<QPopupMenu> pmit( d->popupmenus );
    while ( pmit.current() ) {
	QPopupMenu *pm = pmit.current();
	++pmit;
	pm->disconnect(  SIGNAL(destroyed()), this, SLOT(objectDestroyed()) );
    }

    delete d->separatorAction;
    d->menubuttons.setAutoDelete( TRUE );
    d->comboboxes.setAutoDelete( TRUE );
    d->menuitems.setAutoDelete( TRUE );
    d->popupmenus.setAutoDelete( TRUE );
    delete d;
}

/*! \property QActionPGroup::exclusive
  \brief whether the action group does exclusive toggling

    If exclusive is TRUE only one toggle action in the action group can
    ever be active at any one time. If the user chooses another toggle
    action in the group the one they chose becomes active and the one
    that was active becomes inactive. By default this property is FALSE.

  \sa QActionP::toggleAction
*/
void QActionPGroup::setExclusive( bool enable )
{
    d->exclusive = enable;
}

bool QActionPGroup::isExclusive() const
{
    return d->exclusive;
}

/*!  \property QActionPGroup::usesDropDown
  \brief whether the group's actions are displayed in a
  subwidget of the widgets the action group is added to

  Exclusive action groups added to a toolbar display their actions in
  a combobox with the action's \l QActionP::text and \l
  QActionP::iconSet properties shown. Non-exclusive groups are
  represented by a tool button showing their \l QActionP::iconSet and
  -- depending on \l QMainWindow::usesTextLabel() -- text() property.

  In a popup menu the member actions are displayed in a
  submenu.

  Changing usesDropDown only effects \e subsequent calls to addTo().

  This property's default is FALSE.

*/
void QActionPGroup::setUsesDropDown( bool enable )
{
    d->dropdown = enable;
}

bool QActionPGroup::usesDropDown() const
{
    return d->dropdown;
}

/*! Adds action \a action to this group.

    Normally an action is added to a group by creating it with the group
    as parent, so this function is not usually used.

  \sa addTo()
*/
void QActionPGroup::add( QActionP* action )
{
    if ( d->actions.containsRef( action ) )
	return;

    d->actions.append( action );

    if ( action->whatsThis().isNull() )
	action->setWhatsThis( whatsThis() );
    if ( action->toolTip().isNull() )
	action->setToolTip( toolTip() );
    action->setEnabled( isEnabled() );

    connect( action, SIGNAL( destroyed() ), this, SLOT( childDestroyed() ) );
    connect( action, SIGNAL( activated() ), this, SIGNAL( activated() ) );
    connect( action, SIGNAL( toggled( bool ) ), this, SLOT( childToggled( bool ) ) );

    for ( QPtrListIterator<QComboBox> cb( d->comboboxes ); cb.current(); ++cb ) {
	cb.current()->insertItem( action->iconSet().pixmap(), action->text() );
    }
    for ( QPtrListIterator<QToolButton> mb( d->menubuttons ); mb.current(); ++mb ) {
	QPopupMenu* popup = mb.current()->popup();
	if ( !popup )
	    continue;
	action->addTo( popup );
    }
    for ( QPtrListIterator<QActionPGroupPrivate::MenuItem> mi( d->menuitems ); mi.current(); ++mi ) {
	QPopupMenu* popup = mi.current()->popup;
	if ( !popup )
	    continue;
	action->addTo( popup );
    }
}

/*! Adds a separator to the group. */
void QActionPGroup::addSeparator()
{
    if ( !d->separatorAction )
	d->separatorAction = new QActionP( 0, "qt_separator_action" );
    d->actions.append( d->separatorAction );
}


/*! \fn void QActionPGroup::insert( QActionP* a )

  \obsolete

  Use add() instead, or better still create the action with the action
  group as its parent.
 */

/*!
  Adds this action group to the widget \a w.

  If usesDropDown() is TRUE and exclusive is TRUE (see setExclusive())
  the actions are presented in a combobox if \a w is a toolbar and as
  a submenu if \a w is a menu. Otherwise (the default) the actions
  within the group are added to the widget individually. For example
  if the widget is a menu, the actions will appear as individual menu
  options, and if the widget is a toolbar, the actions will appear as
  toolbar buttons.

  It is recommended that actions in action groups, especially where
  usesDropDown() is TRUE, have their menuText() or text() property set.

  All actions should be added to the action group \e before the action
  group is added to the widget. If actions are added to the action
  group \e after the action group has been added to the widget these
  later actions will \e not appear.

  \sa setExclusive() setUsesDropDown() removeFrom()
*/
bool QActionPGroup::addTo( QWidget* w )
{
#ifndef QT_NO_TOOLBAR
    if ( w->inherits( "QToolBar" ) ) {
	if ( d->dropdown ) {
	    if ( !d->exclusive ) {
		QPtrListIterator<QActionP> it( d->actions);
		if ( !it.current() )
		    return TRUE;

		QActionP *defAction = it.current();

		QToolButton* btn = new QToolButton( (QToolBar*) w, "qt_actiongroup_btn" );
		addedTo( btn, w );
		connect( btn, SIGNAL(destroyed()), SLOT(objectDestroyed()) );
		d->menubuttons.append( btn );

		if ( !iconSet().isNull() )
		    btn->setIconSet( iconSet() );
		else if ( !defAction->iconSet().isNull() )
		    btn->setIconSet( defAction->iconSet() );
		if ( !!text() )
		    btn->setTextLabel( text() );
		else if ( !!defAction->text() )
		    btn->setTextLabel( defAction->text() );
#ifndef QT_NO_TOOLTIP
		if ( !!toolTip() )
		    QToolTip::add( btn, toolTip() );
		else if ( !!defAction->toolTip() )
		    QToolTip::add( btn, defAction->toolTip() );
#endif
#ifndef QT_NO_WHATSTHIS
		if ( !!whatsThis() )
		    QWhatsThis::add( btn, whatsThis() );
		else if ( !!defAction->whatsThis() )
		    QWhatsThis::add( btn, defAction->whatsThis() );
#endif

		connect( btn, SIGNAL( clicked() ), defAction, SIGNAL( activated() ) );
		connect( btn, SIGNAL( toggled(bool) ), defAction, SLOT( toolButtonToggled(bool) ) );
		connect( btn, SIGNAL( destroyed() ), defAction, SLOT( objectDestroyed() ) );

		QPopupMenu *menu = new QPopupMenu( btn, "qt_actiongroup_menu" );
		btn->setPopupDelay( 0 );
		btn->setPopup( menu );

		while( it.current() ) {
		    it.current()->addTo( menu );
		    ++it;
		}
		return TRUE;
	    } else {
		QComboBox *box = new QComboBox( FALSE, w, "qt_actiongroup_combo" );
		addedTo( box, w );
		connect( box, SIGNAL(destroyed()), SLOT(objectDestroyed()) );
		d->comboboxes.append( box );
#ifndef QT_NO_TOOLTIP
		if ( !!toolTip() )
		    QToolTip::add( box, toolTip() );
#endif
#ifndef QT_NO_WHATSTHIS
		if ( !!whatsThis() )
		    QWhatsThis::add( box, whatsThis() );
#endif

		for ( QPtrListIterator<QActionP> it( d->actions); it.current(); ++it ) {
		    it.current()->addTo( box );
		}
		connect( box, SIGNAL(activated(int)), this, SLOT( internalComboBoxActivated(int)) );
		return TRUE;
	    }
	}
    } else
#endif
    if ( w->inherits( "QPopupMenu" ) ) {
	QPopupMenu *popup;
	if ( d->dropdown ) {
	    QPopupMenu *menu = (QPopupMenu*)w;
	    popup = new QPopupMenu( w, "qt_actiongroup_menu" );
	    d->popupmenus.append( popup );
	    connect( popup, SIGNAL(destroyed()), SLOT(objectDestroyed()) );

	    int id;
	    if ( !iconSet().isNull() ) {
		if ( menuText().isEmpty() )
		    id = menu->insertItem( iconSet(), text(), popup );
		else
		    id = menu->insertItem( iconSet(), menuText(), popup );
	    } else {
		if ( menuText().isEmpty() )
		    id = menu->insertItem( text(), popup );
		else
		    id = menu->insertItem( menuText(), popup );
	    }

	    addedTo( menu->indexOf( id ), menu );

	    QActionPGroupPrivate::MenuItem *item = new QActionPGroupPrivate::MenuItem;
	    item->id = id;
	    item->popup = popup;
	    d->menuitems.append( item );
	} else {
	    popup = (QPopupMenu*)w;
	}
	for ( QPtrListIterator<QActionP> it( d->actions); it.current(); ++it ) {
	    // #### do an addedTo( index, popup, action), need to find out index
	    it.current()->addTo( popup );
	}
	return TRUE;
    }

    for ( QPtrListIterator<QActionP> it( d->actions); it.current(); ++it ) {
	// #### do an addedTo( index, popup, action), need to find out index
	it.current()->addTo( w );
    }

    return TRUE;
}

/*! \reimp
*/
bool QActionPGroup::removeFrom( QWidget* w )
{
    for ( QPtrListIterator<QActionP> it( d->actions); it.current(); ++it ) {
	it.current()->removeFrom( w );
    }

#ifndef QT_NO_TOOLBAR
    if ( w->inherits( "QToolBar" ) ) {
	QPtrListIterator<QComboBox> cb( d->comboboxes );
	while( cb.current() ) {
	    QComboBox *box = cb.current();
	    ++cb;
	    if ( box->parentWidget() == w )
		delete box;
	}
	QPtrListIterator<QToolButton> mb( d->menubuttons );
	while( mb.current() ) {
	    QToolButton *btn = mb.current();
	    ++mb;
	    if ( btn->parentWidget() == w )
		delete btn;
	}
    } else
#endif
    if ( w->inherits( "QPopupMenu" ) ) {
	QPtrListIterator<QActionPGroupPrivate::MenuItem> pu( d->menuitems );
	while ( pu.current() ) {
	    QActionPGroupPrivate::MenuItem *mi = pu.current();
	    ++pu;
	    if ( d->dropdown && mi->popup )
		( (QPopupMenu*)w )->removeItem( mi->id );
	    delete mi->popup;
	}
    }

    return TRUE;
}

/*! \internal
*/
void QActionPGroup::childToggled( bool b )
{
    if ( !isExclusive() )
	return;
    QActionP* s = (QActionP*) sender();
    if ( b ) {
	if ( s != d->selected ) {
	    d->selected = s;
	    for ( QPtrListIterator<QActionP> it( d->actions); it.current(); ++it ) {
		if ( it.current()->isToggleAction() && it.current() != s )
		    it.current()->setOn( FALSE );
	    }
	    emit activated();
	    emit selected( s );
	}
    } else {
	if ( s == d->selected ) {
	    // at least one has to be selected
	    s->setOn( TRUE );
	}
    }
}

/*! \internal
*/
void QActionPGroup::childDestroyed()
{
    d->actions.removeRef( (QActionP*) sender() );
    if ( d->selected == sender() )
	d->selected = 0;
}

/*! \reimp
*/
void QActionPGroup::setEnabled( bool enable )
{
    if ( enable == isEnabled() )
	return;

    QActionP::setEnabled( enable );
    d->update( this );
}

/*! \reimp
*/
void QActionPGroup::setIconSet( const QIconSet& icon )
{
    QActionP::setIconSet( icon );
    d->update( this );
}

/*! \reimp
*/
void QActionPGroup::setText( const QString& txt )
{
    if ( txt == text() )
	return;

    QActionP::setText( txt );
    d->update( this );
}

/*! \reimp
*/
void QActionPGroup::setMenuText( const QString& text )
{
    if ( text == menuText() )
	return;

    QActionP::setMenuText( text );
    d->update( this );
}

/*! \reimp
*/
void QActionPGroup::setToolTip( const QString& text )
{
    if ( text == toolTip() )
	return;
    for ( QPtrListIterator<QActionP> it( d->actions); it.current(); ++it ) {
	if ( it.current()->toolTip().isNull() )
	    it.current()->setToolTip( text );
    }
    QActionP::setToolTip( text );
    d->update( this );
}

/*! \reimp
*/
void QActionPGroup::setWhatsThis( const QString& text )
{
    if ( text == whatsThis() )
	return;
    for ( QPtrListIterator<QActionP> it( d->actions); it.current(); ++it ) {
	if ( it.current()->whatsThis().isNull() )
	    it.current()->setWhatsThis( text );
    }
    QActionP::setWhatsThis( text );
    d->update( this );
}

/*! \reimp
*/
void QActionPGroup::childEvent( QChildEvent *e )
{
    if ( !e->child()->inherits( "QActionP" ) )
	return;

    QActionP *action = (QActionP*)e->child();

    if ( !e->removed() )
	return;

    for ( QPtrListIterator<QComboBox> cb( d->comboboxes ); cb.current(); ++cb ) {
	for ( int i = 0; i < cb.current()->count(); i++ ) {
	    if ( cb.current()->text( i ) == action->text() ) {
		cb.current()->removeItem( i );
		break;
	    }
	}
    }
    for ( QPtrListIterator<QToolButton> mb( d->menubuttons ); mb.current(); ++mb ) {
	QPopupMenu* popup = mb.current()->popup();
	if ( !popup )
	    continue;
	action->removeFrom( popup );
    }
    for ( QPtrListIterator<QActionPGroupPrivate::MenuItem> mi( d->menuitems ); mi.current(); ++mi ) {
	QPopupMenu* popup = mi.current()->popup;
	if ( !popup )
	    continue;
	action->removeFrom( popup );
    }
}

/*!
  \fn void QActionPGroup::selected( QActionP* )

  This signal is emitted from exclusive groups when toggle actions
  change state.

  The argument is the action whose state changed to "on".

  \quotefile action/actiongroup/editor.cpp
  \skipto QActionPGroup
  \printline QActionPGroup
  \skipto QObject::connect
  \printuntil SLOT

  In this example we connect the selected() signal to our own
  setFontColor() slot, passing the QActionP so that we know which
  action was chosen by the user.

  (See the \link actiongroup.html QActionPGroup Walkthrough. \endlink)

  \sa setExclusive(), isOn()
*/

/*! \internal
*/
void QActionPGroup::internalComboBoxActivated( int index )
{
    QActionP *a = d->actions.at( index );
    if ( a ) {
	if ( a != d->selected ) {
	    d->selected = a;
	    for ( QPtrListIterator<QActionP> it( d->actions); it.current(); ++it ) {
		if ( it.current()->isToggleAction() && it.current() != a )
		    it.current()->setOn( FALSE );
	    }
	    if ( a->isToggleAction() )
		a->setOn( TRUE );

	    emit activated();
	    emit selected( d->selected );
	    emit ((QActionPGroup*)a)->activated();
	}
    }
}

/*! \internal
*/
void QActionPGroup::internalToggle( QActionP *a )
{
    for ( QPtrListIterator<QComboBox> it( d->comboboxes); it.current(); ++it ) {
	int index = d->actions.find( a );
	if ( index != -1 )
	    it.current()->setCurrentItem( index );
    }
}

/*! \internal
*/
void QActionPGroup::objectDestroyed()
{
    const QObject* obj = sender();
    d->menubuttons.removeRef( (QToolButton*)obj );
    for ( QPtrListIterator<QActionPGroupPrivate::MenuItem> mi( d->menuitems ); mi.current(); ++mi ) {
	if ( mi.current()->popup == obj ) {
	    d->menuitems.removeRef( mi.current() );
	    break;
	}
    }
    d->popupmenus.removeRef( (QPopupMenu*)obj );
    d->comboboxes.removeRef( (QComboBox*)obj );
}

/*! This function is called from the addTo() function when it created
  a widget (\a actionWidget) for the child action \a a in the \a
  container.
*/

void QActionPGroup::addedTo( QWidget *actionWidget, QWidget *container, QActionP *a )
{
    Q_UNUSED( actionWidget );
    Q_UNUSED( container );
    Q_UNUSED( a );
}

/*! \overload

  This function is called from the addTo() function when it created a
  menu item for the child action at the index \a index in the popup
  menu \a menu.
*/

void QActionPGroup::addedTo( int index, QPopupMenu *menu, QActionP *a )
{
    Q_UNUSED( index );
    Q_UNUSED( menu );
    Q_UNUSED( a );
}

/*! \reimp
    \overload
  This function is called from the addTo() function when it created
  a widget (\a actionWidget) in the \a container.
*/

void QActionPGroup::addedTo( QWidget *actionWidget, QWidget *container )
{
    Q_UNUSED( actionWidget );
    Q_UNUSED( container );
}

/*! \reimp
    \overload
  This function is called from the addTo() function when it created a
  menu item at the index \a index in the popup menu \a menu.

*/

void QActionPGroup::addedTo( int index, QPopupMenu *menu )
{
    Q_UNUSED( index );
    Q_UNUSED( menu );
}

#endif

#endif
