/////////////////////////////////////////////////////////////////////////////
// Module      : PatchQt
// File        : qworkspaceP.cxx
// Description : the patch for Qt's QWorkspace class (qworkspace.cpp)
/////////////////////////////////////////////////////////////////////////////

/****************************************************************************
** $Id$
**
** Implementation of the QWorkspace class
**
** Created : 931107
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of the workspace module of the Qt GUI Toolkit.
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
** Licensees holding valid Qt Enterprise Edition licenses may use this
** file in accordance with the Qt Commercial License Agreement provided
** with the Software.
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

using namespace std;
#include "qworkspaceP.h"

#include <qapplication.h>
#include <qobjectlist.h>
#include <qlayout.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <qvbox.h>
#include <qaccel.h>
#include <qcursor.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qguardedptr.h>
#include <qiconset.h>
#include <qfocusdata.h>
#include <qdatetime.h>
#include <qtooltip.h>
#include <qwmatrix.h>
#include <qimage.h>
#include <qscrollbar.h>
#include <qstyle.h>
#include <qframe.h>
#if defined(Q_WS_WIN)
#include <qt_windows.h>
#endif
#include <qplatformdefs.h>
#include <qtimer.h>
#include <qpainter.h>

#include <private/qapplication_p.h>
#include <private/qinternal_p.h>

#define BUTTON_WIDTH	16
#define BUTTON_HEIGHT	14


/*!
    \class QWorkspaceP qworkspace.h
    \brief The QWorkspaceP widget provides a workspace window that can
    contain decorated windows, e.g. for MDI.

    \module workspace

    \ingroup application
    \ingroup organizers
    \mainclass

    An MDI (multiple document interface) application has one main
    window with a menu bar. The central widget of this window is a
    workspace. The workspace itself contains zero, one or more
    document windows, each of which displays a document.

    The workspace itself is an ordinary Qt widget. It has a standard
    constructor that takes a parent widget and an object name. The
    parent window is usually a QMainWindow, but it need not be.

    Document windows (i.e. MDI windows) are also ordinary Qt widgets
    which have the workspace as parent widget. When you call show(),
    hide(), showMaximized(), setCaption(), etc. on a document window,
    it is shown, hidden, etc. with a frame, caption, icon and icon
    text, just as you'd expect. You can provide widget flags which
    will be used for the layout of the decoration or the behaviour of
    the widget itself.

    To change the geometry of the MDI windows it is necessary to make
    the function calls to the parentWidget() of the widget, as this
    will move or resize the decorated window.

    A document window becomes active when it gets the keyboard focus.
    You can activate it using setFocus(), and the user can activate it
    by moving focus in the normal ways. The workspace emits a signal
    windowActivated() when it detects the activation change, and the
    function activeWindow() always returns a pointer to the active
    document window.

    The convenience function windowList() returns a list of all
    document windows. This is useful to create a popup menu
    "<u>W</u>indows" on the fly, for example.

    QWorkspaceP provides two built-in layout strategies for child
    windows: cascade() and tile(). Both are slots so you can easily
    connect menu entries to them.

    If you want your users to be able to work with document windows
    larger than the actual workspace, set the scrollBarsEnabled
    property to TRUE.

    If the top-level window contains a menu bar and a document window
    is maximised, QWorkspaceP moves the document window's minimize,
    restore and close buttons from the document window's frame to the
    workspace window's menu bar. It then inserts a window operations
    menu at the far left of the menu bar.
*/

static bool inCaptionChange = FALSE;

class QWorkspacePPrivate {
public:
    QWorkspacePChild* active;
    QPtrList<QWorkspacePChild> windows;
    QPtrList<QWorkspacePChild> focus;
    QPtrList<QWidget> icons;
    QWorkspacePChild* maxWindow;
    QRect maxRestore;
    QGuardedPtr<QFrame> maxcontrols;
    QGuardedPtr<QMenuBar> maxmenubar;

    int px;
    int py;
    QWidget *becomeActive;
    QGuardedPtr<QLabel> maxtools;
    QPopupMenu* popup;
    QPopupMenu* toolPopup;
    int menuId;
    int controlId;
    QString topCaption;
    bool autoFocusChange;

    QScrollBar *vbar, *hbar;
    QWidget *corner;
    int yoffset, xoffset;
};

/*!
  Constructs a workspace with a \a parent and a \a name.
 */
QWorkspaceP::QWorkspaceP( QWidget *parent, const char *name )
    : QWidget( parent, name, WNoMousePropagation )
{
    d = new QWorkspacePPrivate;
    d->maxcontrols = 0;
    d->active = 0;
    d->maxWindow = 0;
    d->maxtools = 0;
    d->px = 0;
    d->py = 0;
    d->becomeActive = 0;
    d->autoFocusChange = FALSE;
#if defined(Q_WS_WIN)
    d->popup = new QPopupMenu( this, "qt_internal_mdi_popup" );
    d->toolPopup = new QPopupMenu( this, "qt_internal_mdi_popup" );
#else
    d->popup = new QPopupMenu( parentWidget(), "qt_internal_mdi_popup" );
    d->toolPopup = new QPopupMenu( parentWidget(), "qt_internal_mdi_popup" );
#endif

    d->menuId = -1;
    d->controlId = -1;
    connect( d->popup, SIGNAL( aboutToShow() ), this, SLOT(operationMenuAboutToShow() ));
    connect( d->popup, SIGNAL( activated(int) ), this, SLOT( operationMenuActivated(int) ) );
    d->popup->insertItem(QIconSet(style().stylePixmap(QStyle::SP_TitleBarNormalButton)), tr("&Restore"), 1);
    d->popup->insertItem(tr("&Move"), 2);
    d->popup->insertItem(tr("&Size"), 3);
    d->popup->insertItem(QIconSet(style().stylePixmap(QStyle::SP_TitleBarMinButton)), tr("Mi&nimize"), 4);
    d->popup->insertItem(QIconSet(style().stylePixmap(QStyle::SP_TitleBarMaxButton)), tr("Ma&ximize"), 5);
    d->popup->insertSeparator();
    d->popup->insertItem(QIconSet(style().stylePixmap(QStyle::SP_TitleBarCloseButton)),
				  tr("&Close")
#ifndef QT_NO_ACCEL
					+"\t"+QAccel::keyToString(CTRL+Key_F4)
#endif
		    , this, SLOT( closeActiveWindow() ) );

    connect( d->toolPopup, SIGNAL( aboutToShow() ), this, SLOT(toolMenuAboutToShow() ));
    connect( d->toolPopup, SIGNAL( activated(int) ), this, SLOT( operationMenuActivated(int) ) );
    d->toolPopup->insertItem(tr("&Move"), 2);
    d->toolPopup->insertItem(tr("&Size"), 3);
    d->toolPopup->insertItem(tr("Stay on &Top"), 7);
    d->toolPopup->setItemChecked( 7, TRUE );
    d->toolPopup->setCheckable( TRUE );
    d->toolPopup->insertSeparator();
    d->toolPopup->insertItem(QIconSet(style().stylePixmap(QStyle::SP_TitleBarShadeButton)), tr("Sh&ade"), 6);
    d->toolPopup->insertItem(QIconSet(style().stylePixmap(QStyle::SP_TitleBarCloseButton)),
				      tr("&Close")
#ifndef QT_NO_ACCEL
					+"\t"+QAccel::keyToString( CTRL+Key_F4)
#endif
		, this, SLOT( closeActiveWindow() ) );

#ifndef QT_NO_ACCEL
    QAccel* a = new QAccel( this );
    a->connectItem( a->insertItem( ALT + Key_Minus),
		    this, SLOT( showOperationMenu() ) );

    a->connectItem( a->insertItem( CTRL + Key_F6),
		    this, SLOT( activateNextWindow() ) );
    a->connectItem( a->insertItem( CTRL + Key_Tab),
		    this, SLOT( activateNextWindow() ) );

    a->connectItem( a->insertItem( CTRL + SHIFT + Key_F6),
		    this, SLOT( activatePreviousWindow() ) );
    a->connectItem( a->insertItem( CTRL + SHIFT + Key_Tab),
		    this, SLOT( activatePreviousWindow() ) );

    a->connectItem( a->insertItem( CTRL + Key_F4 ),
		    this, SLOT( closeActiveWindow() ) );
#endif

    setBackgroundMode( PaletteDark );
    setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );

#ifndef QT_NO_WIDGET_TOPEXTRA
    d->topCaption = topLevelWidget()->caption();
#endif

    d->hbar = d->vbar = 0;
    d->corner = 0;
    d->xoffset = d->yoffset = 0;

    updateWorkspace();

    topLevelWidget()->installEventFilter( this );
}

/*!  Destroys the workspace and frees any allocated resources. */

QWorkspaceP::~QWorkspaceP()
{
    delete d;
    d = 0;
}

/*!\reimp */
QSize QWorkspaceP::sizeHint() const
{
    QSize s( QApplication::desktop()->size() );
    return QSize( s.width()*2/3, s.height()*2/3);
}

/*! \reimp */
void QWorkspaceP::setPaletteBackgroundColor( const QColor & c )
{
    setEraseColor( c );
}


/*! \reimp */
void QWorkspaceP::setPaletteBackgroundPixmap( const QPixmap & pm )
{
    setErasePixmap( pm );
}

/*! \reimp */
void QWorkspaceP::childEvent( QChildEvent * e)
{
    if (e->inserted() && e->child()->isWidgetType()) {
	QWidget* w = (QWidget*) e->child();
	if ( !w || !w->testWFlags( WStyle_Title | WStyle_NormalBorder | WStyle_DialogBorder )
	     || d->icons.contains( w ) || w == d->vbar || w == d->hbar || w == d->corner )
	    return;	    // nothing to do

	bool hasBeenHidden = w->isHidden();
	bool hasSize = w->testWState( WState_Resized );
	int x = w->x();
	int y = w->y();
	bool hasPos = x != 0 || y != 0;
	QSize s = w->size().expandedTo( w->minimumSizeHint() );
	if ( !hasSize && w->sizeHint().isValid() )
	    w->adjustSize();

	QWorkspacePChild* child = new QWorkspacePChild( w, this, "qt_workspacechild" );
	child->installEventFilter( this );

	connect( child, SIGNAL( popupOperationMenu( const QPoint& ) ),
		 this, SLOT( popupOperationMenu( const QPoint& ) ) );
	connect( child, SIGNAL( showOperationMenu() ),
		 this, SLOT( showOperationMenu() ) );
	d->windows.append( child );
	if ( child->isVisibleTo( this ) )
	    d->focus.append( child );
	child->internalRaise();

	if ( hasBeenHidden )
	    w->hide();
	else if ( !isVisible() )  // that's a case were we don't receive a showEvent in time. Tricky.
	    child->show();

	if ( !hasPos )
	    place( child );
	if ( hasSize )
	    child->resize( s + child->baseSize() );
	else
	    child->adjustSize();
	if ( hasPos )
	    child->move( x, y );

	activateWindow( w );
	updateWorkspace();
    } else if (e->removed() ) {
	if ( d->windows.contains( (QWorkspacePChild*)e->child() ) ) {
	    d->windows.removeRef( (QWorkspacePChild*)e->child() );
	    d->focus.removeRef( (QWorkspacePChild*)e->child() );
	    if (d->windows.count() <= 0) { // VSR+ 26/03/03 -->
	      activateWindow( 0 );         
	      d->maxWindow = 0;            
	      hideMaximizeControls();      
	    }                              // <-- VSR+ 26/03/03
	    updateWorkspace();
	}
    }
}

/*! \reimp
*/
#ifndef QT_NO_WHEELEVENT
void QWorkspaceP::wheelEvent( QWheelEvent *e )
{
    if ( !scrollBarsEnabled() )
	return;
    if ( d->vbar && d->vbar->isVisible() && !( e->state() & AltButton ) )
	QApplication::sendEvent( d->vbar, e );
    else if ( d->hbar && d->hbar->isVisible() )
	QApplication::sendEvent( d->hbar, e );
}
#endif

void QWorkspaceP::activateWindow( QWidget* w, bool change_focus )
{
    if ( !w ) {
	d->active = 0;
	emit windowActivated( 0 );
	return;
    }
    if ( !isVisibleTo( 0 ) ) {
	d->becomeActive = w;
	return;
    }
    if ( d->active && d->active->windowWidget() == w )
	return;
    QPtrListIterator<QWorkspacePChild> it( d->windows );
    while ( it.current () ) {
     	QWorkspacePChild* c = it.current();
	++it;
	c->setActive( c->windowWidget() == w );
	if (c->windowWidget() == w)
	    d->active = c;
    }

    if (!d->active)
	return;

    if ( d->maxWindow && d->maxWindow != d->active && d->active->windowWidget() &&
	 d->active->windowWidget()->testWFlags( WStyle_MinMax ) &&
	 !d->active->windowWidget()->testWFlags( WStyle_Tool ) ) {
	maximizeWindow( d->active->windowWidget() );
	if ( d->maxtools ) {
#ifndef QT_NO_WIDGET_TOPEXTRA
	    if ( w->icon() ) {
		QPixmap pm(*w->icon());
		if(pm.width() != 14 || pm.height() != 14) {
		    QImage im;
		    im = pm;
		    pm = im.smoothScale( 14, 14 );
		}
		d->maxtools->setPixmap( pm );
	    } else
#endif
	    {
		QPixmap pm(14,14);
		pm.fill( white );
		d->maxtools->setPixmap( pm );
	    }
	}
    }

    d->active->internalRaise();

    if ( change_focus ) {
	if ( d->focus.find( d->active ) >=0 ) {
	    d->focus.removeRef( d->active );
//	    d->focus.append( d->active ); //VSR-
	}
	d->focus.append( d->active ); //VSR+
    }

    updateWorkspace();
    emit windowActivated( w );
}


/*!
  Returns the active window, or 0 if no window is active.
 */
QWidget* QWorkspaceP::activeWindow() const
{
    return d->active?d->active->windowWidget():0;
}


void QWorkspaceP::place( QWidget* w)
{
    int overlap, minOverlap = 0;
    int possible;

    QRect r1(0, 0, 0, 0);
    QRect r2(0, 0, 0, 0);
    QRect maxRect = rect();
    int x = maxRect.left(), y = maxRect.top();
    QPoint wpos(maxRect.left(), maxRect.top());

    bool firstPass = TRUE;

    do {
	if ( y + w->height() > maxRect.bottom() ) {
	    overlap = -1;
	} else if( x + w->width() > maxRect.right() ) {
	    overlap = -2;
	} else {
	    overlap = 0;

	    r1.setRect(x, y, w->width(), w->height());

	    QWidget *l;
	    QPtrListIterator<QWorkspacePChild> it( d->windows );
	    while ( it.current () ) {
		l = it.current();
		++it;
		if (! d->icons.contains(l) && ! l->isHidden() && l != w ) {
		    if ( d->maxWindow == l )
			r2 = d->maxRestore;
		    else
			r2.setRect(l->x(), l->y(), l->width(), l->height());

		    if (r2.intersects(r1)) {
			r2.setCoords(QMAX(r1.left(), r2.left()),
				     QMAX(r1.top(), r2.top()),
				     QMIN(r1.right(), r2.right()),
				     QMIN(r1.bottom(), r2.bottom())
				     );

			overlap += (r2.right() - r2.left()) *
				   (r2.bottom() - r2.top());
		    }
		}
	    }
	}

	if (overlap == 0) {
	    wpos = QPoint(x, y);
	    break;
	}

	if (firstPass) {
	    firstPass = FALSE;
	    minOverlap = overlap;
	} else if ( overlap >= 0 && overlap < minOverlap) {
	    minOverlap = overlap;
	    wpos = QPoint(x, y);
	}

	if ( overlap > 0 ) {
	    possible = maxRect.right();
	    if ( possible - w->width() > x) possible -= w->width();

	    QWidget *l;
	    QPtrListIterator<QWorkspacePChild> it( d->windows );
	    while ( it.current () ) {
		l = it.current();
		++it;
		if (! d->icons.contains(l) && ! l->isHidden() && l != w ) {
		    if ( d->maxWindow == l )
			r2 = d->maxRestore;
		    else
			r2.setRect(l->x(), l->y(), l->width(), l->height());

		    if( ( y < r2.bottom() ) && ( r2.top() < w->height() + y ) ) {
			if( r2.right() > x )
			    possible = possible < r2.right() ?
				       possible : r2.right();

			if( r2.left() - w->width() > x )
			    possible = possible < r2.left() - w->width() ?
				       possible : r2.left() - w->width();
		    }
		}
	    }

	    x = possible;
	} else if ( overlap == -2 ) {
	    x = maxRect.left();
	    possible = maxRect.bottom();

	    if ( possible - w->height() > y ) possible -= w->height();

	    QWidget *l;
	    QPtrListIterator<QWorkspacePChild> it( d->windows );
	    while ( it.current () ) {
		l = it.current();
		++it;
		if (l != w && ! d->icons.contains(w)) {
		    if ( d->maxWindow == l )
			r2 = d->maxRestore;
		    else
			r2.setRect(l->x(), l->y(), l->width(), l->height());

		    if( r2.bottom() > y)
			possible = possible < r2.bottom() ?
				   possible : r2.bottom();

		    if( r2.top() - w->height() > y )
			possible = possible < r2.top() - w->height() ?
				   possible : r2.top() - w->height();
		}
	    }

	    y = possible;
	}
    }
    while( overlap != 0 && overlap != -1 );

    w->move(wpos);
    updateWorkspace();
}


void QWorkspaceP::insertIcon( QWidget* w )
{
    if ( !w || d->icons.contains( w ) )
	return;
    d->icons.append( w );
    if (w->parentWidget() != this )
	w->reparent( this, 0, QPoint(0,0), FALSE);


    QRect cr = updateWorkspace();
    int x = 0;
    int y = cr.height() - w->height();

    QPtrListIterator<QWidget> it( d->icons );
    while ( it.current () ) {
	QWidget* i = it.current();
	++it;
	if ( x > 0 && x + i->width() > cr.width() ){
	    x = 0;
	    y -= i->height();
	}

	if ( i != w &&
	    i->geometry().intersects( QRect( x, y, w->width(), w->height() ) ) )
	    x += i->width();
    }
    w->move( x, y );

    if ( isVisibleTo( parentWidget() ) ) {
	w->show();
	w->lower();
    }
    updateWorkspace();
}


void QWorkspaceP::removeIcon( QWidget* w)
{
    if ( !d->icons.contains( w ) )
	return;
    d->icons.remove( w );
    w->hide();
}


/*! \reimp  */
void QWorkspaceP::resizeEvent( QResizeEvent * )
{
    if ( d->maxWindow ) {
	d->maxWindow->adjustToFullscreen();
	((QWorkspaceP*)d->maxWindow->windowWidget())->setWState( WState_Maximized );
    }

    QRect cr = updateWorkspace();

    QPtrListIterator<QWorkspacePChild> it( d->windows );
    while ( it.current () ) {
	QWorkspacePChild* c = it.current();
	++it;
	if ( c->windowWidget() && !c->windowWidget()->testWFlags( WStyle_Tool ) )
	    continue;

	int x = c->x();
	int y = c->y();
	if ( c->snappedDown )
	    y =  cr.height() - c->height();
	if ( c->snappedRight )
	    x =  cr.width() - c->width();

	if ( x != c->x() || y != c->y() )
	    c->move( x, y );
    }

}

/*! \reimp */
void QWorkspaceP::showEvent( QShowEvent *e )
{
    if ( d->maxWindow && !style().styleHint(QStyle::SH_Workspace_FillSpaceOnMaximize, this))
	showMaximizeControls();
    QWidget::showEvent( e );
    if ( d->becomeActive ) {
	activateWindow( d->becomeActive );
	d->becomeActive = 0;
    }
    else if ( d->windows.count() > 0 && !d->active )
	activateWindow( d->windows.first()->windowWidget() );

    updateWorkspace();
}

/*! \reimp */
void QWorkspaceP::hideEvent( QHideEvent * )
{

    if ( !isVisibleTo(0) && !style().styleHint(QStyle::SH_Workspace_FillSpaceOnMaximize, this))
	hideMaximizeControls();
}

void QWorkspaceP::minimizeWindow( QWidget* w)
{
    QWorkspacePChild* c = findChild( w );
    QWorkspacePChild* oldActive = d->active;    //VSR+

    if ( !w || w && (!w->testWFlags( WStyle_Minimize ) || w->testWFlags( WStyle_Tool) ) )
	return;

    if ( c ) {
	QWorkspaceP *fake = (QWorkspaceP*)w;

	setUpdatesEnabled( FALSE );
	bool wasMax = FALSE;
	if ( c == d->maxWindow ) {
	    wasMax = TRUE;
	    d->maxWindow = 0;
	    inCaptionChange = TRUE;
#ifndef QT_NO_WIDGET_TOPEXTRA
	    if ( !!d->topCaption )
		topLevelWidget()->setCaption( d->topCaption );
#endif
	    inCaptionChange = FALSE;
	    if ( !style().styleHint(QStyle::SH_Workspace_FillSpaceOnMaximize, this) )
		hideMaximizeControls();
	    for (QPtrListIterator<QWorkspacePChild> it( d->windows ); it.current(); ++it ) {
		QWorkspacePChild* c = it.current();
		if ( c->titlebar )
		    c->titlebar->setMovable( TRUE );
		c->widgetResizeHandler->setActive( TRUE );
	    }
	}
	insertIcon( c->iconWidget() );
	bool wasFocused = c->titlebar->isActive();
        blockSignals( TRUE );//VSR+
	c->hide();
        blockSignals( FALSE );//VSR+
	if ( wasFocused && ( !d->active || d->active && d->active->testWState( WState_Minimized ) ) ) { //VSR+
//	if ( wasFocused && d->active && d->active->testWState( WState_Minimized ) ) {                   //VSR-
//VSR-	    blockSignals( TRUE ); 
	    activateWindow( w, FALSE );
//VSR-	    blockSignals( FALSE );
	}
        else if ( oldActive != d->active ) { //VSR+
           emit ( windowActivated( d->active ? d->active->windowWidget() : 0 ) ); //VSR+
        } //VSR+
	if ( wasMax )
	    c->setGeometry( d->maxRestore );
	if ( d->focus.find( c ) < 0 ) // VSR+
   	    d->focus.append( c );

	setUpdatesEnabled( TRUE );
	updateWorkspace();

	fake->clearWState( WState_Maximized );
	fake->setWState( WState_Minimized );
	c->clearWState( WState_Maximized );
	c->setWState( WState_Minimized );
    }
}

void QWorkspaceP::normalizeWindow( QWidget* w)
{
    QWorkspacePChild* c = findChild( w );
    if ( !w )
	return;
    if ( c ) {
	QWorkspaceP *fake = (QWorkspaceP*)w;
	fake->clearWState( WState_Minimized | WState_Maximized );
	if ( !style().styleHint(QStyle::SH_Workspace_FillSpaceOnMaximize, this) && d->maxWindow ) {
	    hideMaximizeControls();
	} else {
	    c->widgetResizeHandler->setActive( TRUE );
	    if ( c->titlebar )
		c->titlebar->setMovable(TRUE);
	}
	fake->clearWState( WState_Minimized | WState_Maximized );
	c->clearWState( WState_Minimized | WState_Maximized );

	if ( c == d->maxWindow ) {
	    c->setGeometry( d->maxRestore );
	    d->maxWindow = 0;
#ifndef QT_NO_WIDGET_TOPEXTRA
	    inCaptionChange = TRUE;
	    if ( !!d->topCaption )
		topLevelWidget()->setCaption( d->topCaption );
	    inCaptionChange = FALSE;
#endif
	} else {
	    if ( c->iconw )
		removeIcon( c->iconw->parentWidget() );
	    c->show();
	}

	if ( !style().styleHint(QStyle::SH_Workspace_FillSpaceOnMaximize, this))
	    hideMaximizeControls();
	for (QPtrListIterator<QWorkspacePChild> it( d->windows ); it.current(); ++it ) {
	    QWorkspacePChild* c = it.current();
	    if ( c->titlebar )
		c->titlebar->setMovable( TRUE );
	    c->widgetResizeHandler->setActive( TRUE );
	}
	activateWindow( w, TRUE );

	updateWorkspace();
    }
}

void QWorkspaceP::maximizeWindow( QWidget* w)
{
    QWorkspacePChild* c = findChild( w );

    if ( !w || w && (!w->testWFlags( WStyle_Maximize ) || w->testWFlags( WStyle_Tool) ) )
	return;
    if ( w->maximumSize().isValid() && ( w->maximumWidth() < width() || w->maximumHeight() < height() ) ) {
	w->resize( w->maximumSize() );
	return;
    }

    if ( c ) {
	setUpdatesEnabled( FALSE );
	if (c->iconw && d->icons.contains( c->iconw->parentWidget() ) )
	    normalizeWindow( w );
	QWorkspaceP *fake = (QWorkspaceP*)w;

	QRect r( c->geometry() );
	c->adjustToFullscreen();
	c->show();
	c->internalRaise();
	qApp->sendPostedEvents( c, QEvent::Resize );
	qApp->sendPostedEvents( c, QEvent::Move );
	qApp->sendPostedEvents( c, QEvent::ShowWindowRequest );
	if ( d->maxWindow != c ) {
	    if ( d->maxWindow )
		d->maxWindow->setGeometry( d->maxRestore );
	    d->maxWindow = c;
	    d->maxRestore = r;
	}

	activateWindow( w );
	if(!style().styleHint(QStyle::SH_Workspace_FillSpaceOnMaximize, this)) {
	    showMaximizeControls();
	} else {
	    c->widgetResizeHandler->setActive( FALSE );
	    if ( c->titlebar )
		c->titlebar->setMovable( FALSE );
	}
#ifndef QT_NO_WIDGET_TOPEXTRA
	inCaptionChange = TRUE;
	if ( !!d->topCaption )
	    topLevelWidget()->setCaption( tr("%1 - [%2]")
		.arg(d->topCaption).arg(c->caption()) );
	inCaptionChange = FALSE;
#endif
	setUpdatesEnabled( TRUE );

	updateWorkspace();

	fake->clearWState( WState_Minimized );
	fake->setWState( WState_Maximized );
	c->clearWState( WState_Minimized );
	c->setWState( WState_Maximized );
    }
}

void QWorkspaceP::showWindow( QWidget* w)
{
    if ( d->maxWindow && w->testWFlags( WStyle_Maximize ) && !w->testWFlags( WStyle_Tool) )
	maximizeWindow( w );
    else if ( !w->testWFlags( WStyle_Tool ) )
	normalizeWindow( w );
    else
	w->parentWidget()->show();
    if ( d->maxWindow )
	d->maxWindow->raise();
    updateWorkspace();
}


QWorkspacePChild* QWorkspaceP::findChild( QWidget* w)
{
    QPtrListIterator<QWorkspacePChild> it( d->windows );
    while ( it.current () ) {
	QWorkspacePChild* c = it.current();
	++it;
	if (c->windowWidget() == w)
	    return c;
    }
    return 0;
}

/*!
  Returns a list of all windows.
 */
QWidgetList QWorkspaceP::windowList() const
{
    QWidgetList windows;
    QPtrListIterator<QWorkspacePChild> it( d->windows );
    while ( it.current () ) {
	QWorkspacePChild* c = it.current();
	++it;
	if ( c->windowWidget() )
	    windows.append( c->windowWidget() );
    }
    return windows;
}

/*!\reimp*/
bool QWorkspaceP::eventFilter( QObject *o, QEvent * e)
{
    static QTime* t = 0;
    static QWorkspaceP* tc = 0;
#ifndef QT_NO_MENUBAR
    if ( o == d->maxtools && d->menuId != -1 ) {
	switch ( e->type() ) {
	case QEvent::MouseButtonPress:
	    {
		QMenuBar* b = (QMenuBar*)o->parent();
		if ( !t )
		    t = new QTime;
		if ( tc != this || t->elapsed() > QApplication::doubleClickInterval() ) {
		    if ( QApplication::reverseLayout() ) {
			QPoint p = b->mapToGlobal( QPoint( b->x() + b->width(), b->y() + b->height() ) );
			p.rx() -= d->popup->sizeHint().width();
			popupOperationMenu( p );
		    } else {
			popupOperationMenu( b->mapToGlobal( QPoint( b->x(), b->y() + b->height() ) ) );
		    }
		    t->start();
		    tc = this;
		} else {
		    tc = 0;
		    closeActiveWindow();
		}
		return TRUE;
	    }
	default:
	    break;
	}
	return QWidget::eventFilter( o, e );
    }
#endif
    switch ( e->type() ) {
    case QEvent::Hide:
    case QEvent::HideToParent:
	if ( !o->isA( "QWorkspacePChild" ) || !isVisible() )
	    break;
	d->focus.removeRef( (QWorkspacePChild*)o );
	if ( d->active != o )
	    break;
	if ( d->focus.isEmpty() ) {
	    activateWindow( 0 );
	} else {
	    d->autoFocusChange = TRUE;
	    activatePreviousWindow();
	    QWorkspacePChild* c = d->active;
	    while ( d->active &&
		    d->active->windowWidget() &&
		    d->active->windowWidget()->testWFlags( WStyle_Tool ) ) {
		activatePreviousWindow();
		if ( d->active == c )
		    break;
	    }
	    d->autoFocusChange = FALSE;
	}
	if ( d->maxWindow == o && d->maxWindow->isHidden() ) {
	    d->maxWindow->setGeometry( d->maxRestore );
	    d->maxWindow = 0;
	    if ( d->active )
		maximizeWindow( d->active );

	    if ( !d->maxWindow ) {

		if ( style().styleHint(QStyle::SH_Workspace_FillSpaceOnMaximize, this)) {
		    QWorkspacePChild *wc = (QWorkspacePChild *)o;
		    wc->widgetResizeHandler->setActive( TRUE );
		    if ( wc->titlebar )
			wc->titlebar->setMovable( TRUE );
		} else {
		    hideMaximizeControls();
		}
#ifndef QT_NO_WIDGET_TOPEXTRA
		inCaptionChange = TRUE;
		if ( !!d->topCaption )
		    topLevelWidget()->setCaption( d->topCaption );
		inCaptionChange = FALSE;
#endif
	    }
	}
	break;
    case QEvent::Show:
	if ( o->isA("QWorkspacePChild") && !d->focus.containsRef( (QWorkspacePChild*)o ) )
	    d->focus.append( (QWorkspacePChild*)o );
	updateWorkspace();
	break;
    case QEvent::CaptionChange:
	if ( inCaptionChange )
	    break;

#ifndef QT_NO_WIDGET_TOPEXTRA
	inCaptionChange = TRUE;
	if ( o == topLevelWidget() )
	    d->topCaption = ((QWidget*)o)->caption();

	if ( d->maxWindow && !!d->topCaption )
	    topLevelWidget()->setCaption( tr("%1 - [%2]")
		.arg(d->topCaption).arg(d->maxWindow->caption()));
	inCaptionChange = FALSE;
#endif

	break;
    case QEvent::Close:
	if ( o == topLevelWidget() )
	{
	    QPtrListIterator<QWorkspacePChild> it( d->windows );
	    while ( it.current () ) {
		QWorkspacePChild* c = it.current();
		++it;
		if ( c->shademode )
		    c->showShaded();
	    }
	} else if ( o->inherits("QWorkspacePChild") ) {
	    d->popup->hide();
	}
	if ( d->windows.count() > 1 || d->windows.first() && d->active != d->windows.first() )
	  activatePreviousWindow();
	updateWorkspace();
	break;
    default:
	break;
    }
    return QWidget::eventFilter( o, e);
}

void QWorkspaceP::showMaximizeControls()
{
#ifndef QT_NO_MENUBAR
    Q_ASSERT(d->maxWindow);
    QMenuBar* b = 0;
    
    // Do a breadth-first search first on every parent, 
    QWidget* w = parentWidget();
    QObjectList * l = 0;
    while ( !l && w ) {
	l = w->queryList( "QMenuBar", 0, FALSE, FALSE );
	w = w->parentWidget();	
	if ( l && !l->count() ) {
	    delete l;
	    l = 0;
	}
    } 
    
    // and query recursively if nothing is found.
    if ( !l || !l->count() ) {
	if ( l )
	    delete l;
	l = topLevelWidget()->queryList( "QMenuBar", 0, 0, TRUE );
    }
    if ( l && l->count() )
	b = (QMenuBar *)l->first();
    delete l;

    if ( !b )
	return;

    if ( !d->maxcontrols ) {
	d->maxmenubar = b;
	d->maxcontrols = new QFrame( topLevelWidget(), "qt_maxcontrols" );
	QHBoxLayout* l = new QHBoxLayout( d->maxcontrols,
					  d->maxcontrols->frameWidth(), 0 );
	if ( d->maxWindow->windowWidget()->testWFlags(WStyle_Minimize) ) {
	    QToolButton* iconB = new QToolButton( d->maxcontrols, "iconify" );
#ifndef QT_NO_TOOLTIP
	    QToolTip::add( iconB, tr( "Minimize" ) );
#endif
	    l->addWidget( iconB );
	    iconB->setFocusPolicy( NoFocus );
	    iconB->setIconSet(style().stylePixmap(QStyle::SP_TitleBarMinButton));
	    iconB->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	    connect( iconB, SIGNAL( clicked() ),
		     this, SLOT( minimizeActiveWindow() ) );
	}

	QToolButton* restoreB = new QToolButton( d->maxcontrols, "restore" );
#ifndef QT_NO_TOOLTIP
	QToolTip::add( restoreB, tr( "Restore Down" ) );
#endif
	l->addWidget( restoreB );
	restoreB->setFocusPolicy( NoFocus );
	restoreB->setIconSet( style().stylePixmap(QStyle::SP_TitleBarNormalButton));
	restoreB->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	connect( restoreB, SIGNAL( clicked() ),
		 this, SLOT( normalizeActiveWindow() ) );

	l->addSpacing( 2 );
	QToolButton* closeB = new QToolButton( d->maxcontrols, "close" );
#ifndef QT_NO_TOOLTIP
	QToolTip::add( closeB, tr( "Close" ) );
#endif
	l->addWidget( closeB );
	closeB->setFocusPolicy( NoFocus );
	closeB->setIconSet( style().stylePixmap(QStyle::SP_TitleBarCloseButton) );
	closeB->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	connect( closeB, SIGNAL( clicked() ),
		 this, SLOT( closeActiveWindow() ) );

	d->maxcontrols->setFixedSize( d->maxcontrols->minimumSizeHint() );
    }

    if ( d->controlId == -1 || b->indexOf( d->controlId ) == -1 ) {
	QFrame* dmaxcontrols = d->maxcontrols;
	d->controlId = b->insertItem( dmaxcontrols, -1, b->count() );
    }
    if ( d->active && ( d->menuId == -1 || b->indexOf( d->menuId ) == -1 ) ) {
	if ( !d->maxtools ) {
	    d->maxtools = new QLabel( topLevelWidget(), "qt_maxtools" );
	    d->maxtools->installEventFilter( this );
	}
#ifndef QT_NO_WIDGET_TOPEXTRA
	if ( d->active->windowWidget() && d->active->windowWidget()->icon() ) {
	    QPixmap pm(*d->active->windowWidget()->icon());
	    if(pm.width() != 14 || pm.height() != 14) {
		QImage im;
		im = pm;
		pm = im.smoothScale( 14, 14 );
	    }
	    d->maxtools->setPixmap( pm );
	} else
#endif
	{
	    QPixmap pm(14,14);
	    pm.fill( white );
	    d->maxtools->setPixmap( pm );
	}
	d->menuId = b->insertItem( d->maxtools, -1, 0 );
    }
#endif
}


void QWorkspaceP::hideMaximizeControls()
{
#ifndef QT_NO_MENUBAR
    if ( d->maxmenubar ) {
	int mi = d->menuId;
	if ( mi != -1 ) {
	    if ( d->maxmenubar->indexOf( mi ) != -1 )
		d->maxmenubar->removeItem( mi );
	    d->maxtools = 0;
	}
	int ci = d->controlId;
	if ( ci != -1 && d->maxmenubar->indexOf( ci ) != -1 )
	    d->maxmenubar->removeItem( ci );
    }
    d->maxcontrols = 0;
    d->menuId = -1;
    d->controlId = -1;
#endif
}

void QWorkspaceP::closeActiveWindow()
{
    setUpdatesEnabled( FALSE );
    if ( d->maxWindow && d->maxWindow->windowWidget() )
	d->maxWindow->windowWidget()->close();
    else if ( d->active && d->active->windowWidget() )
	d->active->windowWidget()->close();
    setUpdatesEnabled( TRUE );
    updateWorkspace();
}

void QWorkspaceP::closeAllWindows()
{
    QPtrListIterator<QWorkspacePChild> it( d->windows );
    QWorkspacePChild *c = 0;
    while ( ( c = it.current() ) != 0 ) {
	++it;
	if ( c->windowWidget() )
	    c->windowWidget()->close();
    }
}

void QWorkspaceP::normalizeActiveWindow()
{
    if  ( d->maxWindow )
	d->maxWindow->showNormal();
    else if ( d->active )
	d->active->showNormal();
}

void QWorkspaceP::minimizeActiveWindow()
{
    if ( d->maxWindow )
	d->maxWindow->showMinimized();
    else if ( d->active )
	d->active->showMinimized();
}

void QWorkspaceP::showOperationMenu()
{
    if  ( !d->active || !d->active->windowWidget() )
	return;
    Q_ASSERT( d->active->windowWidget()->testWFlags( WStyle_SysMenu ) );
    QPoint p;
    QPopupMenu *popup = d->active->windowWidget()->testWFlags( WStyle_Tool ) ? d->toolPopup : d->popup;
    if ( QApplication::reverseLayout() ) {
	p = QPoint( d->active->windowWidget()->mapToGlobal( QPoint(d->active->windowWidget()->width(),0) ) );
	p.rx() -= popup->sizeHint().width();
    } else {
	p = QPoint( d->active->windowWidget()->mapToGlobal( QPoint(0,0) ) );
    }
    if ( !d->active->isVisible() ) {
	p = d->active->iconWidget()->mapToGlobal( QPoint(0,0) );
	p.ry() -= popup->sizeHint().height();
    }
    popupOperationMenu( p );
}

void QWorkspaceP::popupOperationMenu( const QPoint&  p)
{
    if ( !d->active || !d->active->windowWidget() || !d->active->windowWidget()->testWFlags( WStyle_SysMenu ) )
	return;
    if ( d->active->windowWidget()->testWFlags( WStyle_Tool ))
	d->toolPopup->popup( p );
    else
	d->popup->popup( p );
}

void QWorkspaceP::operationMenuAboutToShow()
{
    for ( int i = 1; i < 6; i++ ) {
	bool enable = d->active != 0;
	d->popup->setItemEnabled( i, enable );
    }

    if ( !d->active || !d->active->windowWidget() )
	return;

    d->popup->setItemEnabled( 4, d->active->windowWidget()->testWFlags( WStyle_Minimize ) );
    d->popup->setItemEnabled( 5, d->active->windowWidget()->testWFlags( WStyle_Maximize ) );

    if ( d->active == d->maxWindow ) {
	d->popup->setItemEnabled( 2, FALSE );
	d->popup->setItemEnabled( 3, FALSE );
	d->popup->setItemEnabled( 5, FALSE );
    } else if ( d->active->isVisible() ){
	d->popup->setItemEnabled( 1, FALSE );
    } else {
	d->popup->setItemEnabled( 2, FALSE );
	d->popup->setItemEnabled( 3, FALSE );
	d->popup->setItemEnabled( 4, FALSE );
    }
}

void QWorkspaceP::toolMenuAboutToShow()
{
    if ( !d->active || !d->active->windowWidget() )
	return;

    d->toolPopup->setItemEnabled( 3, !d->active->shademode );
    if ( d->active->shademode )
	d->toolPopup->changeItem( 6,
				  QIconSet(style().stylePixmap(QStyle::SP_TitleBarUnshadeButton)), tr("&Unshade") );
    else
	d->toolPopup->changeItem( 6, QIconSet(style().stylePixmap(QStyle::SP_TitleBarShadeButton)), tr("Sh&ade") );
    d->toolPopup->setItemEnabled( 6, d->active->windowWidget()->testWFlags( WStyle_MinMax ) );
    d->toolPopup->setItemChecked( 7, d->active->windowWidget()->testWFlags( WStyle_StaysOnTop ) );
}

void QWorkspaceP::operationMenuActivated( int a )
{
    if ( !d->active )
	return;
    switch ( a ) {
    case 1:
	d->active->showNormal();
	break;
    case 2:
	d->active->doMove();
	break;
    case 3:
	if ( d->active->shademode )
	    d->active->showShaded();
	d->active->doResize();
	break;
    case 4:
	d->active->showMinimized();
	break;
    case 5:
	d->active->showMaximized();
	break;
    case 6:
	d->active->showShaded();
	break;
    case 7:
	{
	    QWorkspaceP* w = (QWorkspaceP*)d->active->windowWidget();
	    if ( !w )
		break;
	    if ( w->testWFlags( WStyle_StaysOnTop ) ) {
		w->clearWFlags( WStyle_StaysOnTop );
	    } else {
		w->setWFlags( WStyle_StaysOnTop );
		w->parentWidget()->raise();
	    }
	}
	break;
    default:
	break;
    }
}

void QWorkspaceP::activateNextWindow()
{
    if ( d->focus.isEmpty() )
	return;
    if ( !d->active ) {
	if ( d->focus.first() )
	    activateWindow( d->focus.first()->windowWidget(), FALSE );
	return;
    }

    int a = d->focus.find( d->active ) + 1;

    a = a % d->focus.count();

    if ( d->focus.at( a ) )
	activateWindow( d->focus.at( a )->windowWidget(), FALSE );
    else
	d->active = 0;
}

void QWorkspaceP::activatePreviousWindow()
{
    if ( d->focus.isEmpty() ) {
        if ( d->windows.count() > 0 ) { //VSR+
	    activateWindow( d->windows.last()->windowWidget() ); //VSR+
        } //VSR+
	return;
    }
    if ( !d->active ) {
	if ( d->focus.last() )
	    activateWindow( d->focus.first()->windowWidget(), FALSE );
	else
	    activateWindow( 0 );

	return;
    }

    int a = d->focus.find( d->active ) - 1;

    if ( a < 0 )
	a = d->focus.count()-1;

    if ( d->autoFocusChange ) {
	QWidget *widget = 0;
	while ( a >= 0 && d->focus.at( a ) && ( widget = d->focus.at( a )->windowWidget() ) && !widget->isVisible() )
	    a--;
	if ( a < 0 )
	    a = d->focus.count() - 1;
    }

    if ( d->focus.at( a ) )
	activateWindow( d->focus.at( a )->windowWidget(), FALSE );
    else
	activateWindow( 0 );
}


/*!
  \fn void QWorkspaceP::windowActivated( QWidget* w )

  This signal is emitted when the window widget \a w becomes active.
  Note that \a w can be null, and that more than one signal may be
  fired for one activation event.

  \sa activeWindow(), windowList()
*/



/*!
  Arranges all child windows in a cascade pattern.

  \sa tile()
 */
void QWorkspaceP::cascade()
{
    if  ( d->maxWindow )
	d->maxWindow->showNormal();

    if ( d->vbar ) {
	d->vbar->blockSignals( TRUE );
	d->vbar->setValue( 0 );
	d->vbar->blockSignals( FALSE );
	d->hbar->blockSignals( TRUE );
	d->hbar->setValue( 0 );
	d->hbar->blockSignals( FALSE );
	scrollBarChanged();
    }

    const int xoffset = 13;
    const int yoffset = 20;

    // make a list of all relevant mdi clients
    QPtrList<QWorkspacePChild> widgets;
    QWorkspacePChild* wc = 0;
    for ( wc = d->windows.first(); wc; wc = d->windows.next() )
	if ( wc->iconw )
	    normalizeWindow( wc->windowWidget() );
    for ( wc = d->focus.first(); wc; wc = d->focus.next() )
	if ( wc->windowWidget()->isVisibleTo( this ) && !wc->windowWidget()->testWFlags( WStyle_Tool ) )
	    widgets.append( wc );

    int x = 0;
    int y = 0;

    setUpdatesEnabled( FALSE );
    QPtrListIterator<QWorkspacePChild> it( widgets );
    int children = d->windows.count() - 1;
    while ( it.current () ) {
	QWorkspacePChild *child = it.current();
	++it;
	child->setUpdatesEnabled( FALSE );
	bool hasSizeHint = FALSE;
	QSize prefSize = child->windowWidget()->sizeHint().expandedTo( child->windowWidget()->minimumSizeHint() );

	if ( !prefSize.isValid() )
	    prefSize = QSize( width() - children * xoffset, height() - children * yoffset );
	else
	    hasSizeHint = TRUE;
	prefSize = prefSize.expandedTo( child->windowWidget()->minimumSize() ).boundedTo( child->windowWidget()->maximumSize() );
	if ( hasSizeHint )
	    prefSize += QSize( child->baseSize().width(), child->baseSize().height() );

	int w = prefSize.width();
	int h = prefSize.height();

	child->showNormal();
	qApp->sendPostedEvents( 0, QEvent::ShowNormal );
	if ( y + h > height() )
	    y = 0;
	if ( x + w > width() )
	    x = 0;
	child->setGeometry( x, y, w, h );
	x += xoffset;
	y += yoffset;
	child->internalRaise();
	child->setUpdatesEnabled( TRUE );
    }
    setUpdatesEnabled( TRUE );
    updateWorkspace();
}

/*!
  Arranges all child windows in a tile pattern.

  \sa cascade()
 */
void QWorkspaceP::tile()
{
    if  ( d->maxWindow )
	d->maxWindow->showNormal();

    if ( d->vbar ) {
	d->vbar->blockSignals( TRUE );
	d->vbar->setValue( 0 );
	d->vbar->blockSignals( FALSE );
	d->hbar->blockSignals( TRUE );
	d->hbar->setValue( 0 );
	d->hbar->blockSignals( FALSE );
	scrollBarChanged();
    }

    int rows = 1;
    int cols = 1;
    int n = 0;
    QWorkspacePChild* c;

    QPtrListIterator<QWorkspacePChild> it( d->windows );
    while ( it.current () ) {
	c = it.current();
	++it;
	if ( !c->windowWidget()->isHidden() &&
	     !c->windowWidget()->testWFlags( WStyle_StaysOnTop ) &&
	     !c->windowWidget()->testWFlags( WStyle_Tool ) )
	    n++;
    }

    while ( rows * cols < n ) {
	if ( cols <= rows )
	    cols++;
	else
	    rows++;
    }
    int add = cols * rows - n;
    bool* used = new bool[ cols*rows ];
    for ( int i = 0; i < rows*cols; i++ )
	used[i] = FALSE;

    int row = 0;
    int col = 0;
    int w = width() / cols;
    int h = height() / rows;

    it.toFirst();
    while ( it.current () ) {
	c = it.current();
	++it;
	if ( c->windowWidget()->isHidden() || c->windowWidget()->testWFlags( WStyle_Tool ) )
	    continue;
	if ( c->windowWidget()->testWFlags( WStyle_StaysOnTop ) ) {
	    QPoint p = c->pos();
	    if ( p.x()+c->width() < 0 )
		p.setX( 0 );
	    if ( p.x() > width() )
		p.setX( width() - c->width() );
	    if ( p.y() + 10 < 0 )
		p.setY( 0 );
	    if ( p.y() > height() )
		p.setY( height() - c->height() );


	    if ( p != c->pos() )
		c->QFrame::move( p );
	} else {
	    c->showNormal();
	    qApp->sendPostedEvents( 0, QEvent::ShowNormal );
	    used[row*cols+col] = TRUE;
	    if ( add ) {
		c->setGeometry( col*w, row*h, QMIN( w, c->windowWidget()->maximumWidth()+c->baseSize().width() ),
					      QMIN( 2*h, c->windowWidget()->maximumHeight()+c->baseSize().height() ) );
		used[(row+1)*cols+col] = TRUE;
		add--;
	    } else {
		c->setGeometry( col*w, row*h, QMIN( w, c->windowWidget()->maximumWidth()+c->baseSize().width() ),
					      QMIN( h, c->windowWidget()->maximumHeight()+c->baseSize().height() ) );
	    }
	    while( row < rows && col < cols && used[row*cols+col] ) {
		col++;
		if ( col == cols ) {
		    col = 0;
		    row++;
		}
	    }
	}
    }
    delete [] used;
    updateWorkspace();
}

QWorkspacePChild::QWorkspacePChild( QWidget* window, QWorkspaceP *parent,
				  const char *name )
    : QFrame( parent, name,
	      WStyle_Customize | WStyle_NoBorder  | WDestructiveClose | WNoMousePropagation | WSubWindow )
{
    setMouseTracking( TRUE );
    act = FALSE;
    iconw = 0;
    lastfocusw = 0;
    shademode = FALSE;
    titlebar = 0;
    snappedRight = FALSE;
    snappedDown = FALSE;

    if (window) {
	switch (window->focusPolicy()) {
	case QWidget::NoFocus:
	    window->setFocusPolicy(QWidget::ClickFocus);
	    break;
	case QWidget::TabFocus:
	    window->setFocusPolicy(QWidget::StrongFocus);
	    break;
	default:
	    break;
	}
    }

    if ( window && window->testWFlags( WStyle_Title ) ) {
	titlebar = new QTitleBarP( window, this, "qt_ws_titlebar" );
	connect( titlebar, SIGNAL( doActivate() ),
		 this, SLOT( activate() ) );
	connect( titlebar, SIGNAL( doClose() ),
		 window, SLOT( close() ) );
	connect( titlebar, SIGNAL( doMinimize() ),
		 this, SLOT( showMinimized() ) );
	connect( titlebar, SIGNAL( doNormal() ),
		 this, SLOT( showNormal() ) );
	connect( titlebar, SIGNAL( doMaximize() ),
		 this, SLOT( showMaximized() ) );
	connect( titlebar, SIGNAL( popupOperationMenu( const QPoint& ) ),
		 this, SIGNAL( popupOperationMenu( const QPoint& ) ) );
	connect( titlebar, SIGNAL( showOperationMenu() ),
		 this, SIGNAL( showOperationMenu() ) );
	connect( titlebar, SIGNAL( doShade() ),
		 this, SLOT( showShaded() ) );
	connect( titlebar, SIGNAL( doubleClicked() ),
		 this, SLOT( titleBarDoubleClicked() ) );
    }

    if ( window && window->testWFlags( WStyle_Tool ) ) {
	setFrameStyle( QFrame::StyledPanel | QFrame::Raised );
	setLineWidth( 2 );
	setMinimumSize( 128, 0 );
    } else {
	setFrameStyle( QFrame::StyledPanel | QFrame::Raised );
	setLineWidth( 2 );
	setMinimumSize( 128, 0 );
    }

    childWidget = window;
    if (!childWidget)
	return;

#ifndef QT_NO_WIDGET_TOPEXTRA
    setCaption( childWidget->caption() );
#endif

    QPoint p;
    QSize s;
    QSize cs;

    bool hasBeenResized = childWidget->testWState( WState_Resized );

    if ( !hasBeenResized )
	cs = childWidget->sizeHint().expandedTo( childWidget->minimumSizeHint() );
    else
	cs = childWidget->size();

    int th = titlebar ? titlebar->sizeHint().height() : 0;
    if ( titlebar ) {
#ifndef QT_NO_WIDGET_TOPEXTRA
	if( childWidget->icon() ) {
	    QPixmap pm(*childWidget->icon());
	    if(pm.width() != 14 || pm.height() != 14) {
		QImage im;
		im = pm;
		pm = im.smoothScale( 14, 14 );
	    }
	    titlebar->setIcon( pm );
	}
#endif
	p = QPoint( contentsRect().x(),
		    th + contentsRect().y() );
	s = QSize( cs.width() + 2*frameWidth(),
		   cs.height() + 2*frameWidth() + th + 2 );
    } else {
	p = QPoint( contentsRect().x(), contentsRect().y() );
	s = QSize( cs.width() + 2*frameWidth(),
		   cs.height() + 2*frameWidth() );
    }

    childWidget->reparent( this, p);
    resize( s );

    childWidget->installEventFilter( this );

    widgetResizeHandler = new QWidgetResizeHandlerP( this, window );
    widgetResizeHandler->setMovingEnabled( FALSE );
    widgetResizeHandler->setSizeProtection( !parent->scrollBarsEnabled() );
    connect( widgetResizeHandler, SIGNAL( activate() ),
	     this, SLOT( activate() ) );
    widgetResizeHandler->setExtraHeight( th + 2 );

    setBaseSize( baseSize() );
}

QWorkspacePChild::~QWorkspacePChild()
{
    if ( iconw )
	delete iconw->parentWidget();
}

void QWorkspacePChild::moveEvent( QMoveEvent * )
{
    ((QWorkspaceP*) parentWidget() )->updateWorkspace();
}

void QWorkspacePChild::resizeEvent( QResizeEvent * )
{
    QRect r = contentsRect();
    QRect cr;

    if ( titlebar ) {
	int th = titlebar->sizeHint().height();
	QRect tbrect( 0, 0, width(), th );
	if ( !style().styleHint( QStyle::SH_TitleBar_NoBorder ) )
	    tbrect = QRect( r.x(), r.y(), r.width(), th );
	titlebar->setGeometry( tbrect );
	cr = QRect( r.x(), r.y() + titlebar->height() + (shademode ? 5 : 1),
	    r.width(), r.height() - titlebar->height() - 2 );
    } else {
	cr = r;
    }

    if (!childWidget)
	return;

    windowSize = cr.size();
    childWidget->setGeometry( cr );
    ((QWorkspaceP*) parentWidget() )->updateWorkspace();
}

QSize QWorkspacePChild::baseSize() const
{
    int th = titlebar ? titlebar->sizeHint().height() : 0;
    return QSize( 2*frameWidth(), 2*frameWidth() + th + 2 );
}

QSize QWorkspacePChild::sizeHint() const
{
    if ( !childWidget )
	return QFrame::sizeHint() + baseSize();
    return childWidget->sizeHint().expandedTo( childWidget->minimumSizeHint() ) + baseSize();
}

QSize QWorkspacePChild::minimumSizeHint() const
{
    if ( !childWidget )
	return QFrame::minimumSizeHint() + baseSize();
    QSize s = childWidget->minimumSize();
    if ( s.isEmpty() )
	s = childWidget->minimumSizeHint();
    return s + baseSize();
}

void QWorkspacePChild::activate()
{
    ((QWorkspaceP*)parentWidget())->activateWindow( windowWidget() );
}

bool QWorkspacePChild::eventFilter( QObject * o, QEvent * e)
{
    if ( !isActive() && ( e->type() == QEvent::MouseButtonPress ||
	e->type() == QEvent::FocusIn ) ) {
	if ( iconw ) {
	    ((QWorkspaceP*)parentWidget())->normalizeWindow( windowWidget() );
	    if ( iconw ) {
		((QWorkspaceP*)parentWidget())->removeIcon( iconw->parentWidget() );
		delete iconw->parentWidget();
		iconw = 0;
	    }
	}
	activate();
    }

    // for all widgets except the window, we that's the only thing we
    // process, and if we have no childWidget we skip totally
    if ( o != childWidget || childWidget == 0 )
	return FALSE;

    switch ( e->type() ) {
    case QEvent::Show:
	if ( ((QWorkspaceP*)parentWidget())->d->focus.find( this ) < 0 )
	    ((QWorkspaceP*)parentWidget())->d->focus.append( this );
	if ( isVisibleTo( parentWidget() ) )
	    break;
	if (( (QShowEvent*)e)->spontaneous() )
	    break;
	// fall through
    case QEvent::ShowToParent:
	if ( windowWidget() && windowWidget()->testWFlags( WStyle_StaysOnTop ) ) {
	    internalRaise();
	    show();
	}
	((QWorkspaceP*)parentWidget())->showWindow( windowWidget() );
	break;
    case QEvent::ShowMaximized:
	if ( windowWidget()->maximumSize().isValid() &&
	     ( windowWidget()->maximumWidth() < parentWidget()->width() ||
	       windowWidget()->maximumHeight() < parentWidget()->height() ) ) {
	    windowWidget()->resize( windowWidget()->maximumSize() );
	    break;
	}
	if ( windowWidget()->testWFlags( WStyle_Maximize ) && !windowWidget()->testWFlags( WStyle_Tool ) )
	    ((QWorkspaceP*)parentWidget())->maximizeWindow( windowWidget() );
	else
	    ((QWorkspaceP*)parentWidget())->normalizeWindow( windowWidget() );
	break;
    case QEvent::ShowMinimized:
	((QWorkspaceP*)parentWidget())->minimizeWindow( windowWidget() );
	break;
    case QEvent::ShowNormal:
	((QWorkspaceP*)parentWidget())->normalizeWindow( windowWidget() );
	if (iconw) {
	    ((QWorkspaceP*)parentWidget())->removeIcon( iconw->parentWidget() );
	    delete iconw->parentWidget();
	}
	break;
    case QEvent::Hide:
    case QEvent::HideToParent:
	if ( !childWidget->isVisibleTo( this ) ) {
	    QWidget * w = iconw;
	    if ( w && ( w = w->parentWidget() ) ) {
		((QWorkspaceP*)parentWidget())->removeIcon( w );
		delete w;
	    }
	    hide();
	}
	break;
    case QEvent::CaptionChange:
#ifndef QT_NO_WIDGET_TOPEXTRA
	setCaption( childWidget->caption() );
	if ( iconw )
	    iconw->setCaption( childWidget->caption() );
#endif
	break;
    case QEvent::IconChange:
	{
	    QWorkspaceP* ws = (QWorkspaceP*)parentWidget();
	    if ( !titlebar )
		break;
#ifndef QT_NO_WIDGET_TOPEXTRA
	    if ( childWidget->icon() ) {
		titlebar->setIcon( *childWidget->icon() );
	    } else
#endif
	    {
		QPixmap pm;
		titlebar->setIcon( pm );
	    }

	    if ( ws->d->maxWindow != this )
		break;

	    if ( ws->d->maxtools ) {
#ifndef QT_NO_WIDGET_TOPEXTRA
		if ( childWidget->icon() ) {
		    QPixmap pm(*childWidget->icon());
		    if(pm.width() != 14 || pm.height() != 14) {
			QImage im;
			im = pm;
			pm = im.smoothScale( 14, 14 );
		    }
		    ws->d->maxtools->setPixmap( pm );
		} else
#endif
		{
		    QPixmap pm(14,14);
		    pm.fill( white );
		    ws->d->maxtools->setPixmap( pm );
		}
	    }
	}
	break;
    case QEvent::Resize:
	{
	    QResizeEvent* re = (QResizeEvent*)e;
	    if ( re->size() != windowSize && !shademode )
		resize( re->size() + baseSize() );
	}
	break;

    case QEvent::WindowDeactivate:
	if ( titlebar )
	    titlebar->setActive( FALSE );
	break;

    case QEvent::WindowActivate:
	if ( titlebar )
	    titlebar->setActive( act );
	break;

    default:
	break;
    }

    return QFrame::eventFilter(o, e);
}

bool QWorkspacePChild::focusNextPrevChild( bool next )
{
    QFocusData *f = focusData();

    QWidget *startingPoint = f->home();
    QWidget *candidate = 0;
    QWidget *w = next ? f->next() : f->prev();
    while( !candidate && w != startingPoint ) {
	if ( w != startingPoint &&
	     (w->focusPolicy() & TabFocus) == TabFocus
	     && w->isEnabled() &&!w->focusProxy() && w->isVisible() )
	    candidate = w;
	w = next ? f->next() : f->prev();
    }

    if ( candidate ) {
	QObjectList *ol = queryList();
	bool ischild = ol->findRef( candidate ) != -1;
	delete ol;
	if ( !ischild ) {
	    startingPoint = f->home();
	    QWidget *nw = next ? f->prev() : f->next();
	    QObjectList *ol2 = queryList();
	    QWidget *lastValid = 0;
	    candidate = startingPoint;
	    while ( nw != startingPoint ) {
		if ( ( candidate->focusPolicy() & TabFocus ) == TabFocus
		    && candidate->isEnabled() &&!candidate->focusProxy() && candidate->isVisible() )
		    lastValid = candidate;
		if ( ol2->findRef( nw ) == -1 ) {
		    candidate = lastValid;
		    break;
		}
		candidate = nw;
		nw = next ? f->prev() : f->next();
	    }
	    delete ol2;
	}
    }

    if ( !candidate )
	return FALSE;

    candidate->setFocus();
    return TRUE;
}

void QWorkspacePChild::childEvent( QChildEvent*  e)
{
    if ( e->type() == QEvent::ChildRemoved && e->child() == childWidget ) {
	childWidget = 0;
	if ( iconw ) {
	    ((QWorkspaceP*)parentWidget())->removeIcon( iconw->parentWidget() );
	    delete iconw->parentWidget();
	}
	close();
    }
}


void QWorkspacePChild::doResize()
{
    widgetResizeHandler->doResize();
}

void QWorkspacePChild::doMove()
{
    widgetResizeHandler->doMove();
}

void QWorkspacePChild::enterEvent( QEvent * )
{
}

void QWorkspacePChild::leaveEvent( QEvent * )
{
#ifndef QT_NO_CURSOR
    if ( !widgetResizeHandler->isButtonDown() )
	setCursor( arrowCursor );
#endif
}

void QWorkspacePChild::drawFrame( QPainter *p )
{
    QStyle::SFlags flags = QStyle::Style_Default;
    QStyleOption opt(lineWidth(),midLineWidth());

    if ( act )
	flags |= QStyle::Style_Active;

    style().drawPrimitive( QStyle::PE_WindowFrame, p, rect(), colorGroup(), flags, opt );
}

void QWorkspacePChild::styleChange( QStyle & )
{
    resizeEvent( 0 );
    if ( iconw ) {
	QVBox *vbox = (QVBox*)iconw->parentWidget()->qt_cast( "QVBox" );
	Q_ASSERT(vbox);
	if ( !style().styleHint( QStyle::SH_TitleBar_NoBorder ) ) {
	    vbox->setFrameStyle( QFrame::WinPanel | QFrame::Raised );
	    vbox->resize( 196+2*vbox->frameWidth(), 20 + 2*vbox->frameWidth() );
	} else {
	    vbox->resize( 196, 20 );
	}
    }
}

static bool isChildOf( QWidget * child, QWidget * parent )
{
    if ( !parent || !child )
	return FALSE;
    QWidget * w = child;
    while( w && w != parent )
	w = w->parentWidget();
    return w != 0;
}


void QWorkspacePChild::setActive( bool b )
{
    if ( !childWidget )
	return;

    if ( act == b )
	return;

    act = b;
    repaint( FALSE );

    if ( titlebar )
	titlebar->setActive( act );
    if ( iconw )
	iconw->setActive( act );

    QObjectList* ol = childWidget->queryList( "QWidget" );
    if ( act ) {
	QObject *o;
	for ( o = ol->first(); o; o = ol->next() )
	    o->removeEventFilter( this );
	bool hasFocus = isChildOf( focusWidget(), childWidget );
	if ( !hasFocus ) {
	    if ( lastfocusw && ol->contains( lastfocusw ) &&
		 lastfocusw->focusPolicy() != NoFocus ) {
		// this is a bug if lastfocusw has been deleted, a new
		// widget has been created, and the new one is a child
		// of the same window as the old one. but even though
		// it's a bug the behaviour is reasonable
		lastfocusw->setFocus();
	    } else if ( childWidget->focusPolicy() != NoFocus ) {
		childWidget->setFocus();
	    } else {
		// find something, anything, that accepts focus, and use that.
		o = ol->first();
		while( o && ((QWidget*)o)->focusPolicy() == NoFocus )
		    o = ol->next();
		if ( o )
		    ((QWidget*)o)->setFocus();
	    }
	}
    } else {
	lastfocusw = 0;
	if ( isChildOf( focusWidget(), childWidget ) )
	    lastfocusw = focusWidget();
	QObject * o;
	for ( o = ol->first(); o; o = ol->next() ) {
	    o->removeEventFilter( this );
	    o->installEventFilter( this );
	}
    }
    delete ol;
}

bool QWorkspacePChild::isActive() const
{
    return act;
}

QWidget* QWorkspacePChild::windowWidget() const
{
    return childWidget;
}


QWidget* QWorkspacePChild::iconWidget() const
{
    if ( !iconw ) {
	QWorkspacePChild* that = (QWorkspacePChild*) this;
	QVBox* vbox = new QVBox(0, "qt_vbox" );
	if ( !style().styleHint( QStyle::SH_TitleBar_NoBorder ) ) {
	    vbox->setFrameStyle( QFrame::WinPanel | QFrame::Raised );
	    vbox->resize( 196+2*vbox->frameWidth(), 20 + 2*vbox->frameWidth() );
	} else {
	    vbox->resize( 196, 20 );
	}
	QTitleBarP *tb = new QTitleBarP( windowWidget(), vbox, "_workspacechild_icon_");
	((QWorkspacePChild*)tb)->setWState( WState_Minimized );
	that->iconw = tb;
	iconw->setActive( isActive() );
	connect( iconw, SIGNAL( doActivate() ),
		 this, SLOT( activate() ) );
	connect( iconw, SIGNAL( doClose() ),
		 windowWidget(), SLOT( close() ) );
	connect( iconw, SIGNAL( doNormal() ),
		 this, SLOT( showNormal() ) );
	connect( iconw, SIGNAL( doMaximize() ),
		 this, SLOT( showMaximized() ) );
	connect( iconw, SIGNAL( popupOperationMenu( const QPoint& ) ),
		 this, SIGNAL( popupOperationMenu( const QPoint& ) ) );
	connect( iconw, SIGNAL( showOperationMenu() ),
		 this, SIGNAL( showOperationMenu() ) );
	connect( iconw, SIGNAL( doubleClicked() ),
		 this, SLOT( titleBarDoubleClicked() ) );
    }
#ifndef QT_NO_WIDGET_TOPEXTRA
    if ( windowWidget() ) {
	iconw->setCaption( windowWidget()->caption() );
	if ( windowWidget()->icon() )
	    iconw->setIcon( *windowWidget()->icon() );
    }
#endif
    return iconw->parentWidget();
}

void QWorkspacePChild::showMinimized()
{
    QApplication::postEvent( windowWidget(), new QEvent( QEvent::ShowMinimized ) );
}

void QWorkspacePChild::showMaximized()
{
    QApplication::postEvent( windowWidget(), new QEvent( QEvent::ShowMaximized ) );
}

void QWorkspacePChild::showNormal()
{
    QApplication::postEvent( windowWidget(), new QEvent( QEvent::ShowNormal ) );
}

void QWorkspacePChild::showShaded()
{
    if ( !titlebar)
	return;
    Q_ASSERT( windowWidget()->testWFlags( WStyle_MinMax ) && windowWidget()->testWFlags( WStyle_Tool ) );
    ((QWorkspaceP*)parentWidget())->activateWindow( windowWidget() );
    if ( shademode ) {
	QWorkspacePChild* fake = (QWorkspacePChild*)windowWidget();
	fake->clearWState( WState_Minimized );
	clearWState( WState_Minimized );

	shademode = FALSE;
	resize( shadeRestore );
	setMinimumSize( shadeRestoreMin );
    } else {
	shadeRestore = size();
	shadeRestoreMin = minimumSize();
	setMinimumHeight(0);
	shademode = TRUE;
	QWorkspacePChild* fake = (QWorkspacePChild*)windowWidget();
	fake->setWState( WState_Minimized );
	setWState( WState_Minimized );

	resize( width(), titlebar->height() + 2*lineWidth() + 1 );
    }
    titlebar->update();
}

void QWorkspacePChild::titleBarDoubleClicked()
{
    if ( !windowWidget() )
	return;
    if ( windowWidget()->testWFlags( WStyle_MinMax ) ) {
	if ( windowWidget()->testWFlags( WStyle_Tool ) )
	    showShaded();
	else if ( iconw )
	    showNormal();
	else if ( windowWidget()->testWFlags( WStyle_Maximize ) )
	    showMaximized();
    }
}

void QWorkspacePChild::adjustToFullscreen()
{
    qApp->sendPostedEvents( this, QEvent::Resize );
    qApp->sendPostedEvents( childWidget, QEvent::Resize );
    qApp->sendPostedEvents( childWidget, QEvent::Move );
    if(style().styleHint(QStyle::SH_Workspace_FillSpaceOnMaximize, this)) {
	setGeometry( 0, 0, parentWidget()->width(), parentWidget()->height());
    } else {
	setGeometry( -childWidget->x(), -childWidget->y(),
		     parentWidget()->width() + width() - childWidget->width(),
		     parentWidget()->height() + height() - childWidget->height() );
    }
    setWState( WState_Maximized );
    ((QWorkspacePChild*)childWidget)->setWState( WState_Maximized );
}

void QWorkspacePChild::adjustSize()
{
    if ( !testWState(WState_Polished) )
	polish();

    QSize prefSize = windowWidget()->sizeHint().expandedTo( windowWidget()->minimumSizeHint() );
    prefSize = prefSize.expandedTo( windowWidget()->minimumSize() ).boundedTo( windowWidget()->maximumSize() );
    prefSize += baseSize();

    resize( prefSize );
}

void QWorkspacePChild::setCaption( const QString& cap )
{
    if ( titlebar )
	titlebar->setCaption( cap );
#ifndef QT_NO_WIDGET_TOPEXTRA
    QWidget::setCaption( cap );
#endif
}

void QWorkspacePChild::internalRaise()
{
    setUpdatesEnabled( FALSE );
    if ( iconw )
	iconw->parentWidget()->raise();
    raise();

    if ( !windowWidget() || windowWidget()->testWFlags( WStyle_StaysOnTop ) ) {
	setUpdatesEnabled( TRUE );
	return;
    }

    QPtrListIterator<QWorkspacePChild> it( ((QWorkspaceP*)parent())->d->windows );
    while ( it.current () ) {
     	QWorkspacePChild* c = it.current();
	++it;
	if ( c->windowWidget() &&
	    !c->windowWidget()->isHidden() &&
	     c->windowWidget()->testWFlags( WStyle_StaysOnTop ) )
	     c->raise();
    }

    setUpdatesEnabled( TRUE );
}

void QWorkspacePChild::move( int x, int y )
{
    int nx = x;
    int ny = y;

    if ( windowWidget() && windowWidget()->testWFlags( WStyle_Tool ) ) {
	int dx = 10;
	int dy = 10;

	if ( QABS( x ) < dx )
	    nx = 0;
	if ( QABS( y ) < dy )
	    ny = 0;
	if ( QABS( x + width() - parentWidget()->width() ) < dx ) {
	    nx = parentWidget()->width() - width();
	    snappedRight = TRUE;
	} else
	    snappedRight = FALSE;

	if ( QABS( y + height() - parentWidget()->height() ) < dy ) {
	    ny = parentWidget()->height() - height();
	    snappedDown = TRUE;
	} else
	    snappedDown = FALSE;
    }
    QFrame::move( nx, ny );
}

bool QWorkspaceP::scrollBarsEnabled() const
{
    return d->vbar != 0;
}

/*! \property QWorkspaceP::scrollBarsEnabled
    \brief whether the workspace provides scrollbars

    If this property is set to TRUE, it is possible to resize child
    windows over the right or the bottom edge out of the visible area
    of the workspace. The workspace shows scrollbars to make it
    possible for the user to access those windows. If this property is
    set to FALSE (the default), resizing windows out of the visible
    area of the workspace is not permitted.
*/
void QWorkspaceP::setScrollBarsEnabled( bool enable )
{
    if ( (d->vbar != 0) == enable )
	return;

    d->xoffset = d->yoffset = 0;
    if ( enable ) {
	d->vbar = new QScrollBar( Vertical, this, "vertical scrollbar" );
	connect( d->vbar, SIGNAL( valueChanged(int) ), this, SLOT( scrollBarChanged() ) );
	d->hbar = new QScrollBar( Horizontal, this, "horizontal scrollbar" );
	connect( d->hbar, SIGNAL( valueChanged(int) ), this, SLOT( scrollBarChanged() ) );
	d->corner = new QWidget( this, "qt_corner" );
	updateWorkspace();
    } else {
	delete d->vbar;
	delete d->hbar;
	delete d->corner;
	d->vbar = d->hbar = 0;
	d->corner = 0;
    }

    QPtrListIterator<QWorkspacePChild> it( d->windows );
    while ( it.current () ) {
	QWorkspacePChild *child = it.current();
	++it;
	child->widgetResizeHandler->setSizeProtection( !enable );
    }
}

QRect QWorkspaceP::updateWorkspace()
{
    if ( !isUpdatesEnabled() )
	return rect();

    QRect cr( rect() );

    if ( scrollBarsEnabled() && !d->maxWindow ) {
	d->corner->raise();
	d->vbar->raise();
	d->hbar->raise();
	if ( d->maxWindow )
	    d->maxWindow->raise();

	QRect r( 0, 0, 0, 0 );
	QPtrListIterator<QWorkspacePChild> it( d->windows );
	while ( it.current () ) {
	    QWorkspacePChild *child = it.current();
	    ++it;
	    if ( !child->isHidden() )
		r = r.unite( child->geometry() );
	}
	d->vbar->blockSignals( TRUE );
	d->hbar->blockSignals( TRUE );

	int hsbExt = d->hbar->sizeHint().height();
	int vsbExt = d->vbar->sizeHint().width();


	bool showv = d->yoffset || d->yoffset + r.bottom() - height() + 1 > 0 || d->yoffset + r.top() < 0;
	bool showh = d->xoffset || d->xoffset + r.right() - width() + 1 > 0 || d->xoffset + r.left() < 0;

	if ( showh && !showv)
	    showv = d->yoffset + r.bottom() - height() + hsbExt + 1 > 0;
	if ( showv && !showh )
	    showh = d->xoffset + r.right() - width() + vsbExt  + 1 > 0;

	if ( !showh )
	    hsbExt = 0;
	if ( !showv )
	    vsbExt = 0;

	if ( showv ) {
	    d->vbar->setSteps( QMAX( height() / 12, 30 ), height()  - hsbExt );
	    d->vbar->setRange( QMIN( 0, d->yoffset + QMIN( 0, r.top() ) ), QMAX( 0, d->yoffset + QMAX( 0, r.bottom() - height() + hsbExt + 1) ) );
	    d->vbar->setGeometry( width() - vsbExt, 0, vsbExt, height() - hsbExt );
	    d->vbar->setValue( d->yoffset );
	    d->vbar->show();
	} else {
	    d->vbar->hide();
	}

	if ( showh ) {
	    d->hbar->setSteps( QMAX( width() / 12, 30 ), width() - vsbExt );
	    d->hbar->setRange( QMIN( 0, d->xoffset + QMIN( 0, r.left() ) ), QMAX( 0, d->xoffset + QMAX( 0, r.right() - width() + vsbExt  + 1) ) );
	    d->hbar->setGeometry( 0, height() - hsbExt, width() - vsbExt, hsbExt );
	    d->hbar->setValue( d->xoffset );
	    d->hbar->show();
	} else {
	    d->hbar->hide();
	}

	if ( showh && showv ) {
	    d->corner->setGeometry( width() - vsbExt, height() - hsbExt, vsbExt, hsbExt );
	    d->corner->show();
	} else {
	    d->corner->hide();
	}

	d->vbar->blockSignals( FALSE );
	d->hbar->blockSignals( FALSE );

	cr.setRect( 0, 0, width() - vsbExt, height() - hsbExt );
    }

    QPtrListIterator<QWidget> ii( d->icons );
    while ( ii.current() ) {
	QWorkspacePChild* w = (QWorkspacePChild*)ii.current();
	++ii;
	int x = w->x();
	int y = w->y();
	bool m = FALSE;
	if ( x+w->width() > cr.width() ) {
	    m = TRUE;
	    x =  cr.width() - w->width();
	}
	if ( y+w->height() >  cr.height() ) {
	    y =  cr.height() - w->height();
	    m = TRUE;
	}
	if ( m )
	    w->move( x, y );
    }

    return cr;

}

void QWorkspaceP::scrollBarChanged()
{
    int ver = d->yoffset - d->vbar->value();
    int hor = d->xoffset - d->hbar->value();
    d->yoffset = d->vbar->value();
    d->xoffset = d->hbar->value();

    QPtrListIterator<QWorkspacePChild> it( d->windows );
    while ( it.current () ) {
	QWorkspacePChild *child = it.current();
	++it;
	// we do not use move() due to the reimplementation in QWorkspacePChild
	child->setGeometry( child->x() + hor, child->y() + ver, child->width(), child->height() );
    }
    updateWorkspace();
}

#ifndef QT_NO_STYLE
/*!\reimp */
void QWorkspaceP::styleChange( QStyle &olds )
{
    int fs = style().styleHint(QStyle::SH_Workspace_FillSpaceOnMaximize, this);
    if ( isVisibleTo(0) && d->maxWindow &&
	 fs != olds.styleHint(QStyle::SH_Workspace_FillSpaceOnMaximize, this)) {
	if( fs )
	    hideMaximizeControls();
	else
	    showMaximizeControls();
    }
    QWidget::styleChange(olds);
}
#endif

#define RANGE 4

static bool resizeHorizontalDirectionFixed = FALSE;
static bool resizeVerticalDirectionFixed = FALSE;

QWidgetResizeHandlerP::QWidgetResizeHandlerP( QWidget *parent, QWidget *cw, const char *name )
    : QObject( parent, name ), widget( parent ), childWidget( cw ? cw : parent ),
      extrahei( 0 ), buttonDown( FALSE ), moveResizeMode( FALSE ), sizeprotect( TRUE ), moving( TRUE )
{
    mode = Nowhere;
    widget->setMouseTracking( TRUE );
    range = widget->inherits( "QFrame" ) ? ((QFrame*)widget)->frameWidth() : RANGE;
    range = QMAX( RANGE, range );
    active = TRUE;
    qApp->installEventFilter( this );
}

static QWidget *childOf( QWidget *w, QWidget *child )
{
    while ( child ) {
	if ( child == w )
	    return child;
	child = child->parentWidget();
    }
    return 0;
}

bool QWidgetResizeHandlerP::eventFilter( QObject *o, QEvent *ee )
{
    if ( !active || !o->isWidgetType() )
	return FALSE;

    QWidget *w = childOf( widget, (QWidget*)o );
    if ( !w || o->inherits( "QSizeGrip" ) )
	return FALSE;

    QMouseEvent *e = (QMouseEvent*)ee;
    switch ( e->type() ) {
    case QEvent::MouseButtonPress: {
	if ( w->isMaximized() )
	    break;
	if ( !widget->rect().contains( widget->mapFromGlobal( e->globalPos() ) ) )
	    return FALSE;
	if ( e->button() == LeftButton ) {
	    emit activate();
	    bool me = isMovingEnabled();
	    setMovingEnabled( me && o == widget );
	    mouseMoveEvent( e );
	    setMovingEnabled( me );
	    buttonDown = TRUE;
	    moveOffset = widget->mapFromGlobal( e->globalPos() );
	    invertedMoveOffset = widget->rect().bottomRight() - moveOffset;
	}
    } break;
    case QEvent::MouseButtonRelease:
	if ( w->isMaximized() )
	    break;
	if ( e->button() == LeftButton ) {
	    moveResizeMode = FALSE;
	    buttonDown = FALSE;
	    widget->releaseMouse();
	    widget->releaseKeyboard();
	}
	break;
    case QEvent::MouseMove: {
	if ( w->isMaximized() )
	    break;
	bool me = isMovingEnabled();
	setMovingEnabled( me && o == widget );
	mouseMoveEvent( e );
	setMovingEnabled( me );
	if ( buttonDown && mode != Center )
	    return TRUE;
    } break;
    case QEvent::KeyPress:
	keyPressEvent( (QKeyEvent*)e );
	break;
    case QEvent::AccelOverride:
	if ( buttonDown ) {
	    ((QKeyEvent*)ee)->accept();
	    return TRUE;
	}
	break;
    default:
	break;
    }
    return FALSE;
}

void QWidgetResizeHandlerP::mouseMoveEvent( QMouseEvent *e )
{
    QPoint pos = widget->mapFromGlobal( e->globalPos() );
    if ( !moveResizeMode && ( !buttonDown || ( e->state() & LeftButton ) == 0 ) ) {
	if ( pos.y() <= range && pos.x() <= range)
	    mode = TopLeft;
	else if ( pos.y() >= widget->height()-range && pos.x() >= widget->width()-range)
	    mode = BottomRight;
	else if ( pos.y() >= widget->height()-range && pos.x() <= range)
	    mode = BottomLeft;
	else if ( pos.y() <= range && pos.x() >= widget->width()-range)
	    mode = TopRight;
	else if ( pos.y() <= range )
	    mode = Top;
	else if ( pos.y() >= widget->height()-range )
	    mode = Bottom;
	else if ( pos.x() <= range )
	    mode = Left;
	else if (  pos.x() >= widget->width()-range )
	    mode = Right;
	else
	    mode = Center;

	if ( widget->isMinimized() )
	    mode = Center;
#ifndef QT_NO_CURSOR
	setMouseCursor( mode );
#endif
	return;
    }

    if ( buttonDown && !isMovingEnabled() && mode == Center && !moveResizeMode )
	return;

    if ( widget->testWState( WState_ConfigPending ) )
 	return;

    QPoint globalPos = widget->parentWidget( TRUE ) ?
		       widget->parentWidget( TRUE )->mapFromGlobal( e->globalPos() ) : e->globalPos();
    if ( widget->parentWidget( TRUE ) && !widget->parentWidget( TRUE )->rect().contains( globalPos ) ) {
	if ( globalPos.x() < 0 )
	    globalPos.rx() = 0;
	if ( globalPos.y() < 0 )
	    globalPos.ry() = 0;
	if ( sizeprotect && globalPos.x() > widget->parentWidget()->width() )
	    globalPos.rx() = widget->parentWidget()->width();
	if ( sizeprotect && globalPos.y() > widget->parentWidget()->height() )
	    globalPos.ry() = widget->parentWidget()->height();
    }

    QPoint p = globalPos + invertedMoveOffset;
    QPoint pp = globalPos - moveOffset;

    int fw = 0;
    int mw = QMAX( childWidget->minimumSizeHint().width(),
		   childWidget->minimumWidth() );
    int mh = QMAX( childWidget->minimumSizeHint().height(),
		   childWidget->minimumHeight() );
    if ( childWidget != widget ) {
	if ( widget->inherits( "QFrame" ) )
	    fw = ( (QFrame *) widget )->frameWidth();
	mw += 2 * fw;
	mh += 2 * fw + extrahei;
    }

    QSize mpsize( widget->geometry().right() - pp.x() + 1,
		  widget->geometry().bottom() - pp.y() + 1 );
    mpsize = mpsize.expandedTo( widget->minimumSize() ).expandedTo( QSize(mw, mh) );
    QPoint mp( widget->geometry().right() - mpsize.width() + 1,
	       widget->geometry().bottom() - mpsize.height() + 1 );

    QRect geom = widget->geometry();

    switch ( mode ) {
    case TopLeft:
	geom = QRect( mp, widget->geometry().bottomRight() ) ;
	break;
    case BottomRight:
	geom = QRect( widget->geometry().topLeft(), p ) ;
	break;
    case BottomLeft:
	geom = QRect( QPoint(mp.x(), widget->geometry().y() ), QPoint( widget->geometry().right(), p.y()) ) ;
	break;
    case TopRight:
	geom = QRect( QPoint( widget->geometry().x(), mp.y() ), QPoint( p.x(), widget->geometry().bottom()) ) ;
	break;
    case Top:
	geom = QRect( QPoint( widget->geometry().left(), mp.y() ), widget->geometry().bottomRight() ) ;
	break;
    case Bottom:
	geom = QRect( widget->geometry().topLeft(), QPoint( widget->geometry().right(), p.y() ) ) ;
	break;
    case Left:
	geom = QRect( QPoint( mp.x(), widget->geometry().top() ), widget->geometry().bottomRight() ) ;
	break;
    case Right:
	geom = QRect( widget->geometry().topLeft(), QPoint( p.x(), widget->geometry().bottom() ) ) ;
	break;
    case Center:
	if ( isMovingEnabled() || moveResizeMode )
	    geom.moveTopLeft( pp );
	break;
    default:
	break;
    }

    QSize maxsize( childWidget->maximumSize() );
    if ( childWidget != widget )
	maxsize += QSize( 2 * fw, 2 * fw + extrahei );

    geom = QRect( geom.topLeft(),
		  geom.size().expandedTo( widget->minimumSize() )
			     .expandedTo( QSize(mw, mh) )
			     .boundedTo( maxsize ) );

    if ( geom != widget->geometry() &&
	( widget->isTopLevel() || widget->parentWidget()->rect().intersects( geom ) ) ) {
	if ( widget->isMinimized() )
	    widget->move( geom.topLeft() );
	else
	    widget->setGeometry( geom );
    }

#if defined(Q_WS_WIN)
    MSG msg;
#if defined(UNICODE)
#  ifndef Q_OS_TEMP
    if ( qWinVersion() & WV_NT_based ) {
#  endif
	while(PeekMessageW( &msg, widget->winId(), WM_MOUSEMOVE, WM_MOUSEMOVE, PM_REMOVE ))
	    ;
#  ifndef Q_OS_TEMP
    } else
#  endif
#endif
#ifndef Q_OS_TEMP
    {
	while(PeekMessageA( &msg, widget->winId(), WM_MOUSEMOVE, WM_MOUSEMOVE, PM_REMOVE ))
	    ;
    }
#endif
#endif

    QApplication::syncX();
}

void QWidgetResizeHandlerP::setMouseCursor( MousePosition m )
{
#ifndef QT_NO_CURSOR
    switch ( m ) {
    case TopLeft:
    case BottomRight:
	widget->setCursor( sizeFDiagCursor );
	break;
    case BottomLeft:
    case TopRight:
	widget->setCursor( sizeBDiagCursor );
	break;
    case Top:
    case Bottom:
	widget->setCursor( sizeVerCursor );
	break;
    case Left:
    case Right:
	widget->setCursor( sizeHorCursor );
	break;
    default:
	widget->setCursor( arrowCursor );
	break;
    }
#endif
}

void QWidgetResizeHandlerP::keyPressEvent( QKeyEvent * e )
{
    if ( !isMove() && !isResize() )
	return;
    bool is_control = e->state() & ControlButton;
    int delta = is_control?1:8;
    QPoint pos = QCursor::pos();
    switch ( e->key() ) {
    case Key_Left:
	pos.rx() -= delta;
	if ( pos.x() <= QApplication::desktop()->geometry().left() ) {
	    if ( mode == TopLeft || mode == BottomLeft ) {
		moveOffset.rx() += delta;
		invertedMoveOffset.rx() += delta;
	    } else {
		moveOffset.rx() -= delta;
		invertedMoveOffset.rx() -= delta;
	    }
	}
	if ( isResize() && !resizeHorizontalDirectionFixed ) {
	    resizeHorizontalDirectionFixed = TRUE;
	    if ( mode == BottomRight )
		mode = BottomLeft;
	    else if ( mode == TopRight )
		mode = TopLeft;
#ifndef QT_NO_CURSOR
	    setMouseCursor( mode );
	    widget->grabMouse( widget->cursor() );
#else
	    widget->grabMouse();
#endif
	}
	break;
    case Key_Right:
	pos.rx() += delta;
	if ( pos.x() >= QApplication::desktop()->geometry().right() ) {
	    if ( mode == TopRight || mode == BottomRight ) {
		moveOffset.rx() += delta;
		invertedMoveOffset.rx() += delta;
	    } else {
		moveOffset.rx() -= delta;
		invertedMoveOffset.rx() -= delta;
	    }
	}
	if ( isResize() && !resizeHorizontalDirectionFixed ) {
	    resizeHorizontalDirectionFixed = TRUE;
	    if ( mode == BottomLeft )
		mode = BottomRight;
	    else if ( mode == TopLeft )
		mode = TopRight;
#ifndef QT_NO_CURSOR
	    setMouseCursor( mode );
	    widget->grabMouse( widget->cursor() );
#else
	    widget->grabMouse();
#endif
	}
	break;
    case Key_Up:
	pos.ry() -= delta;
	if ( pos.y() <= QApplication::desktop()->geometry().top() ) {
	    if ( mode == TopLeft || mode == TopRight ) {
		moveOffset.ry() += delta;
		invertedMoveOffset.ry() += delta;
	    } else {
		moveOffset.ry() -= delta;
		invertedMoveOffset.ry() -= delta;
	    }
	}
	if ( isResize() && !resizeVerticalDirectionFixed ) {
	    resizeVerticalDirectionFixed = TRUE;
	    if ( mode == BottomLeft )
		mode = TopLeft;
	    else if ( mode == BottomRight )
		mode = TopRight;
#ifndef QT_NO_CURSOR
	    setMouseCursor( mode );
	    widget->grabMouse( widget->cursor() );
#else
	    widget->grabMouse();
#endif
	}
	break;
    case Key_Down:
	pos.ry() += delta;
	if ( pos.y() >= QApplication::desktop()->geometry().bottom() ) {
	    if ( mode == BottomLeft || mode == BottomRight ) {
		moveOffset.ry() += delta;
		invertedMoveOffset.ry() += delta;
	    } else {
		moveOffset.ry() -= delta;
		invertedMoveOffset.ry() -= delta;
	    }
	}
	if ( isResize() && !resizeVerticalDirectionFixed ) {
	    resizeVerticalDirectionFixed = TRUE;
	    if ( mode == TopLeft )
		mode = BottomLeft;
	    else if ( mode == TopRight )
		mode = BottomRight;
#ifndef QT_NO_CURSOR
	    setMouseCursor( mode );
	    widget->grabMouse( widget->cursor() );
#else
	    widget->grabMouse();
#endif
	}
	break;
    case Key_Space:
    case Key_Return:
    case Key_Enter:
    case Key_Escape:
	moveResizeMode = FALSE;
	widget->releaseMouse();
	widget->releaseKeyboard();
	buttonDown = FALSE;
	break;
    default:
	return;
    }
    QCursor::setPos( pos );
}


void QWidgetResizeHandlerP::doResize()
{
    if ( !active )
	return;

    moveResizeMode = TRUE;
    buttonDown = TRUE;
    moveOffset = widget->mapFromGlobal( QCursor::pos() );
    if ( moveOffset.x() < widget->width()/2) {
	if ( moveOffset.y() < widget->height()/2)
	    mode = TopLeft;
	else
	    mode = BottomLeft;
    } else {
	if ( moveOffset.y() < widget->height()/2)
	    mode = TopRight;
	else
	    mode = BottomRight;
    }
    invertedMoveOffset = widget->rect().bottomRight() - moveOffset;
#ifndef QT_NO_CURSOR
    setMouseCursor( mode );
    widget->grabMouse( widget->cursor()  );
#else
    widget->grabMouse();
#endif
    widget->grabKeyboard();
    resizeHorizontalDirectionFixed = FALSE;
    resizeVerticalDirectionFixed = FALSE;
}

void QWidgetResizeHandlerP::doMove()
{
    if ( !active )
	return;

    mode = Center;
    moveResizeMode = TRUE;
    buttonDown = TRUE;
    moveOffset = widget->mapFromGlobal( QCursor::pos() );
    invertedMoveOffset = widget->rect().bottomRight() - moveOffset;
#ifndef QT_NO_CURSOR
    widget->grabMouse( SizeAllCursor );
#else
    widget->grabMouse();
#endif
    widget->grabKeyboard();
}

#ifndef QT_NO_TOOLTIP
class QTitleBarPTip : public QToolTip
{
public:
    QTitleBarPTip( QWidget * parent ) : QToolTip( parent ) { }

    void maybeTip( const QPoint &pos )
    {
	if ( !parentWidget()->inherits( "QTitleBarP" ) )
	    return;
	QTitleBarP *t = (QTitleBarP *)parentWidget();

	QString tipstring;
	QStyle::SubControl ctrl = t->style().querySubControl(QStyle::CC_TitleBar, t, pos);
	QSize controlSize = t->style().querySubControlMetrics(QStyle::CC_TitleBar, t, ctrl).size();

	QWidget *window = t->window();
	if ( window ) {
	    switch(ctrl) {
	    case QStyle::SC_TitleBarSysMenu:
		if ( t->testWFlags( WStyle_SysMenu ) )
		    tipstring = QTitleBarP::tr( "System Menu" );
		break;

	    case QStyle::SC_TitleBarShadeButton:
		if ( t->testWFlags( WStyle_Tool ) && t->testWFlags( WStyle_MinMax ) )
		    tipstring = QTitleBarP::tr( "Shade" );
		break;

	    case QStyle::SC_TitleBarUnshadeButton:
		if ( t->testWFlags( WStyle_Tool ) && t->testWFlags( WStyle_MinMax ) )
		    tipstring = QTitleBarP::tr( "Unshade" );
		break;

	    case QStyle::SC_TitleBarNormalButton:
	    case QStyle::SC_TitleBarMinButton:
		if ( !t->testWFlags( WStyle_Tool ) && t->testWFlags( WStyle_Minimize ) ) {
		    if( window->isMinimized() )
			tipstring = QTitleBarP::tr( "Normalize" );
		    else
			tipstring = QTitleBarP::tr( "Minimize" );
		}
		break;

	    case QStyle::SC_TitleBarMaxButton:
		if ( !t->testWFlags( WStyle_Tool ) && t->testWFlags( WStyle_Maximize ) )
		    tipstring = QTitleBarP::tr( "Maximize" );
		break;

	    case QStyle::SC_TitleBarCloseButton:
		if ( t->testWFlags( WStyle_SysMenu ) )
		    tipstring = QTitleBarP::tr( "Close" );
		break;

	    default:
		break;
	    }
	}
#ifndef QT_NO_WIDGET_TOPEXTRA
	if ( tipstring.isEmpty() ) {
	    if ( t->visibleText() != t->caption() )
		tipstring = t->caption();
	}
#endif
	if(!tipstring.isEmpty())
	    tip( QRect(pos, controlSize), tipstring );
    }
};
#endif

class QTitleBarPPrivate
{
public:
    QTitleBarPPrivate()
	: toolTip( 0 ), act( 0 ), window( 0 ), movable( 1 ), pressed( 0 ), autoraise(0)
    {
    }

    QStyle::SCFlags buttonDown;
    QPoint moveOffset;
    QToolTip *toolTip;
    bool act		    :1;
    QWidget* window;
    bool movable            :1;
    bool pressed            :1;
    bool autoraise          :1;
    QString cuttext;
};

QTitleBarP::QTitleBarP(QWidget* w, QWidget* parent, const char* name)
    : QWidget( parent, name, WStyle_Customize | WStyle_NoBorder | WResizeNoErase | WRepaintNoErase )
{
    d = new QTitleBarPPrivate();

#ifndef QT_NO_TOOLTIP
    d->toolTip = new QTitleBarPTip( this );
#endif
    d->window = w;
    d->buttonDown = QStyle::SC_None;
    d->act = 0;
    if ( w ) {
	setWFlags( ((QTitleBarP*)w)->getWFlags() | WResizeNoErase | WRepaintNoErase );
#ifndef QT_NO_WIDGET_TOPEXTRA
    	setCaption( w->caption() );
#endif
    } else {
	setWFlags( WStyle_Customize | WResizeNoErase | WRepaintNoErase );
    }

    readColors();
    setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
    setMouseTracking(TRUE);
}

QTitleBarP::~QTitleBarP()
{
#ifndef QT_NO_TOOLTIP
    delete d->toolTip;
#endif

    delete d;
    d = 0;
}

#ifdef Q_WS_WIN
extern QRgb qt_colorref2qrgb(COLORREF col);
#endif

void QTitleBarP::readColors()
{
    QPalette pal = palette();

#ifdef Q_WS_WIN // ask system properties on windows
#ifndef SPI_GETGRADIENTCAPTIONS
#define SPI_GETGRADIENTCAPTIONS 0x1008
#endif
#ifndef COLOR_GRADIENTACTIVECAPTION
#define COLOR_GRADIENTACTIVECAPTION 27
#endif
#ifndef COLOR_GRADIENTINACTIVECAPTION
#define COLOR_GRADIENTINACTIVECAPTION 28
#endif
    if ( QApplication::desktopSettingsAware() ) {
	pal.setColor( QPalette::Active, QColorGroup::Highlight, qt_colorref2qrgb(GetSysColor(COLOR_ACTIVECAPTION)) );
	pal.setColor( QPalette::Inactive, QColorGroup::Highlight, qt_colorref2qrgb(GetSysColor(COLOR_INACTIVECAPTION)) );
	pal.setColor( QPalette::Active, QColorGroup::HighlightedText, qt_colorref2qrgb(GetSysColor(COLOR_CAPTIONTEXT)) );
	pal.setColor( QPalette::Inactive, QColorGroup::HighlightedText, qt_colorref2qrgb(GetSysColor(COLOR_INACTIVECAPTIONTEXT)) );
	if ( qt_winver != Qt::WV_95 && qt_winver != WV_NT ) {
	    BOOL gradient;
#ifdef Q_OS_TEMP
	    SystemParametersInfo( SPI_GETGRADIENTCAPTIONS, 0, &gradient, 0 );
#else
#if defined(UNICODE)
	    if ( qt_winver & Qt::WV_NT_based )
		SystemParametersInfo( SPI_GETGRADIENTCAPTIONS, 0, &gradient, 0 );
	    else
#endif
		SystemParametersInfoA( SPI_GETGRADIENTCAPTIONS, 0, &gradient, 0 );
#endif
	    if ( gradient ) {
		pal.setColor( QPalette::Active, QColorGroup::Base, qt_colorref2qrgb(GetSysColor(COLOR_GRADIENTACTIVECAPTION)) );
		pal.setColor( QPalette::Inactive, QColorGroup::Base, qt_colorref2qrgb(GetSysColor(COLOR_GRADIENTINACTIVECAPTION)) );
	    } else {
		pal.setColor( QPalette::Active, QColorGroup::Base, palette().active().highlight() );
		pal.setColor( QPalette::Inactive, QColorGroup::Base, palette().inactive().highlight() );
	    }
	}
    } else
#endif // Q_WS_WIN
    {
	pal.setColor( QPalette::Active, QColorGroup::Highlight, palette().active().highlight() );
	pal.setColor( QPalette::Active, QColorGroup::Base, palette().active().highlight() );
	pal.setColor( QPalette::Inactive, QColorGroup::Highlight, palette().inactive().dark() );
	pal.setColor( QPalette::Inactive, QColorGroup::Base, palette().inactive().dark() );
	pal.setColor( QPalette::Inactive, QColorGroup::HighlightedText, palette().inactive().background() );
    }

    setPalette( pal );
    setActive( d->act );
}

void QTitleBarP::mousePressEvent( QMouseEvent * e)
{
    emit doActivate();
    if ( e->button() == LeftButton ) {
	d->pressed = TRUE;
	QStyle::SCFlags ctrl = style().querySubControl(QStyle::CC_TitleBar, this, e->pos());
	switch (ctrl) {
	case QStyle::SC_TitleBarSysMenu:
	    if ( testWFlags( WStyle_SysMenu ) && !testWFlags( WStyle_Tool ) ) {
		d->buttonDown = QStyle::SC_None;
		static QTime* t = 0;
		static QTitleBarP* tc = 0;
		if ( !t )
		    t = new QTime;
		if ( tc != this || t->elapsed() > QApplication::doubleClickInterval() ) {
		    emit showOperationMenu();
		    t->start();
		    tc = this;
		} else {
		    tc = 0;
		    emit doClose();
		    return;
		}
	    }
	    break;

	case QStyle::SC_TitleBarShadeButton:
	case QStyle::SC_TitleBarUnshadeButton:
	    if ( testWFlags( WStyle_MinMax ) && testWFlags( WStyle_Tool ) )
		d->buttonDown = ctrl;
	    break;

	case QStyle::SC_TitleBarNormalButton:
	    if( testWFlags( WStyle_Minimize ) && !testWFlags( WStyle_Tool ) )
		d->buttonDown = ctrl;
	    break;

	case QStyle::SC_TitleBarMinButton:
	    if( testWFlags( WStyle_Minimize ) && !testWFlags( WStyle_Tool ) )
		d->buttonDown = ctrl;
	    break;

	case QStyle::SC_TitleBarMaxButton:
	    if ( testWFlags( WStyle_Maximize ) && !testWFlags( WStyle_Tool ) )
		d->buttonDown = ctrl;
	    break;

	case QStyle::SC_TitleBarCloseButton:
	    if ( testWFlags( WStyle_SysMenu ) )
		d->buttonDown = ctrl;
	    break;

	case QStyle::SC_TitleBarLabel:
	    d->buttonDown = ctrl;
	    d->moveOffset = mapToParent( e->pos() );
	    break;
	
	default:
	    break;
	}
	repaint( FALSE );
    } else {
	d->pressed = FALSE;
    }
}

void QTitleBarP::contextMenuEvent( QContextMenuEvent *e )
{
    QStyle::SCFlags ctrl = style().querySubControl(QStyle::CC_TitleBar, this, e->pos());
    if( ctrl == QStyle::SC_TitleBarLabel || ctrl == QStyle::SC_TitleBarSysMenu ) {
	emit popupOperationMenu(e->globalPos());
	e->accept();
    }
}

void QTitleBarP::mouseReleaseEvent( QMouseEvent * e)
{
    if ( e->button() == LeftButton && d->pressed) {
	QStyle::SCFlags ctrl = style().querySubControl(QStyle::CC_TitleBar, this, e->pos());

	if (ctrl == d->buttonDown) {
	    switch(ctrl) {
	    case QStyle::SC_TitleBarShadeButton:
	    case QStyle::SC_TitleBarUnshadeButton:
		if( testWFlags( WStyle_MinMax ) && testWFlags( WStyle_Tool ) )
		    emit doShade();
		break;
	
	    case QStyle::SC_TitleBarNormalButton:
		if( testWFlags( WStyle_MinMax ) && !testWFlags( WStyle_Tool ) )
		    emit doNormal();
		break;

	    case QStyle::SC_TitleBarMinButton:
		if( testWFlags( WStyle_Minimize ) && !testWFlags( WStyle_Tool ) )
		    emit doMinimize();
		break;

	    case QStyle::SC_TitleBarMaxButton:
		if( d->window && testWFlags( WStyle_Maximize ) && !testWFlags( WStyle_Tool ) ) {
		    if(d->window->isMaximized())
			emit doNormal();
		    else
			emit doMaximize();
		}
		break;

	    case QStyle::SC_TitleBarCloseButton:
		if( testWFlags( WStyle_SysMenu ) ) {
		    d->buttonDown = QStyle::SC_None;
		    repaint(FALSE);
		    emit doClose();		
		    return;
		}
		break;

	    default:
		break;
	    }
	}
	d->buttonDown = QStyle::SC_None;
	repaint(FALSE);
	d->pressed = FALSE;
    }
}

void QTitleBarP::mouseMoveEvent( QMouseEvent * e)
{
    switch (d->buttonDown) {
    case QStyle::SC_None:
	if(autoRaise())
	    repaint( FALSE );
	break;
    case QStyle::SC_TitleBarSysMenu:
	break;
    case QStyle::SC_TitleBarShadeButton:
    case QStyle::SC_TitleBarUnshadeButton:
    case QStyle::SC_TitleBarNormalButton:
    case QStyle::SC_TitleBarMinButton:
    case QStyle::SC_TitleBarMaxButton:
    case QStyle::SC_TitleBarCloseButton:
	{
	    QStyle::SCFlags last_ctrl = d->buttonDown;
	    d->buttonDown = style().querySubControl(QStyle::CC_TitleBar, this, e->pos());
	    if( d->buttonDown != last_ctrl)
		d->buttonDown = QStyle::SC_None;
	    repaint(FALSE);
	    d->buttonDown = last_ctrl;
	}
	break;

    case QStyle::SC_TitleBarLabel:
	if ( d->buttonDown == QStyle::SC_TitleBarLabel && d->movable && d->pressed ) {
	    if ( (d->moveOffset - mapToParent( e->pos() ) ).manhattanLength() >= 4 ) {
		QPoint p = mapFromGlobal(e->globalPos());
#ifndef QT_NO_WORKSPACE
		if(d->window && d->window->parentWidget()->inherits("QWorkspacePChild")) {
		    QWidget *w = d->window->parentWidget()->parentWidget();
		    if(w && w->inherits("QWorkspaceP")) {
			QWorkspaceP *workspace = (QWorkspaceP*)w;
			p = workspace->mapFromGlobal( e->globalPos() );
			if ( !workspace->rect().contains(p) ) {
			    if ( p.x() < 0 )
				p.rx() = 0;
			    if ( p.y() < 0 )
				p.ry() = 0;
			    if ( p.x() > workspace->width() )
				p.rx() = workspace->width();
			    if ( p.y() > workspace->height() )
				p.ry() = workspace->height();
			}
		    }
		}
#endif
		QPoint pp = p - d->moveOffset;
		parentWidget()->move( pp );
	    }
	} else {
	    QStyle::SCFlags last_ctrl = d->buttonDown;
	    d->buttonDown = QStyle::SC_None;
	    if( d->buttonDown != last_ctrl)
		repaint(FALSE);
	}
	break;
    }
}

void QTitleBarP::resizeEvent( QResizeEvent *r)
{
    QWidget::resizeEvent(r);
    cutText();
}

void QTitleBarP::paintEvent(QPaintEvent *)
{
    QStyle::SCFlags ctrls = QStyle::SC_TitleBarLabel;
    if ( testWFlags( WStyle_SysMenu) ) {
	if ( testWFlags( WStyle_Tool ) ) {
	    ctrls |= QStyle::SC_TitleBarCloseButton;
	    if ( d->window && testWFlags( WStyle_MinMax ) ) {
		if ( d->window->isMinimized() )
		    ctrls |= QStyle::SC_TitleBarUnshadeButton;
		else
		    ctrls |= QStyle::SC_TitleBarShadeButton;
	    }
	} else {
	    ctrls |= QStyle::SC_TitleBarSysMenu | QStyle::SC_TitleBarCloseButton;
	    if ( d->window && testWFlags( WStyle_Minimize ) ) {
		if( d->window && d->window->isMinimized() )
		    ctrls |= QStyle::SC_TitleBarNormalButton;
		else
		    ctrls |= QStyle::SC_TitleBarMinButton;
	    }
	    if ( d->window && testWFlags( WStyle_Maximize ) && !d->window->isMaximized() )
		ctrls |= QStyle::SC_TitleBarMaxButton;
	}
    }

    QStyle::SCFlags under_mouse = QStyle::SC_None;
    if( autoRaise() && hasMouse() ) {
	QPoint p(mapFromGlobal(QCursor::pos()));
	under_mouse = style().querySubControl(QStyle::CC_TitleBar, this, p);
	ctrls ^= under_mouse;
    }

    // QSharedDoubleBuffer buffer( (bool)FALSE, (bool)FALSE );
    // buffer.begin( this, rect() );
    QSharedDoubleBuffer buffer( this, rect() );
    style().drawComplexControl(QStyle::CC_TitleBar, buffer.painter(), this, rect(),
			       colorGroup(),
			       isEnabled() ? QStyle::Style_Enabled :
			       QStyle::Style_Default, ctrls, d->buttonDown);
    if(under_mouse != QStyle::SC_None)
	style().drawComplexControl(QStyle::CC_TitleBar, buffer.painter(), this, rect(),
				   colorGroup(),
				   QStyle::Style_MouseOver |
				   (isEnabled() ? QStyle::Style_Enabled : 0),
				   under_mouse, d->buttonDown);
}

void QTitleBarP::mouseDoubleClickEvent( QMouseEvent *e )
{
    if ( e->button() != LeftButton )
	return;

    switch(style().querySubControl(QStyle::CC_TitleBar, this, e->pos())) {
    case QStyle::SC_TitleBarLabel:
	emit doubleClicked();
	break;

    case QStyle::SC_TitleBarSysMenu:
	if ( testWFlags( WStyle_SysMenu ) )
	    emit doClose();
	break;

    default:
	break;
    }
}

void QTitleBarP::cutText()
{
#ifndef QT_NO_WIDGET_TOPEXTRA
    QFontMetrics fm( font() );

    int maxw = style().querySubControlMetrics(QStyle::CC_TitleBar, this,
					      QStyle::SC_TitleBarLabel).width();
    if ( !d->window )
	maxw = width() - 20;
    const QString txt = caption();
    d->cuttext = txt;
    if ( fm.width( txt + "m" ) > maxw ) {
	int i = txt.length();
	int dotlength = fm.width( "..." );
	while ( i>0 && fm.width(txt.left( i )) + dotlength > maxw )
	    i--;
	if(i != (int)txt.length())
	    d->cuttext = txt.left( i ) + "...";
    }
#endif
}

void QTitleBarP::setCaption( const QString& title )
{
#ifndef QT_NO_WIDGET_TOPEXTRA
    if( caption() == title)
	return;
    QWidget::setCaption( title );
    cutText();

    update();
#endif
}


void QTitleBarP::setIcon( const QPixmap& icon )
{
#ifndef QT_NO_WIDGET_TOPEXTRA
#ifndef QT_NO_IMAGE_SMOOTHSCALE
    QRect menur = style().querySubControlMetrics(QStyle::CC_TitleBar, this,
						  QStyle::SC_TitleBarSysMenu);

    QPixmap theIcon;
    if (icon.width() > menur.width()) {
	// try to keep something close to the same aspect
	int aspect = (icon.height() * 100) / icon.width();
	int newh = (aspect * menur.width()) / 100;
	theIcon.convertFromImage( icon.convertToImage().smoothScale(menur.width(),
								   newh) );
    } else if (icon.height() > menur.height()) {
	// try to keep something close to the same aspect
	int aspect = (icon.width() * 100) / icon.height();
	int neww = (aspect * menur.height()) / 100;
	theIcon.convertFromImage( icon.convertToImage().smoothScale(neww,
								   menur.height()) );
    } else
	theIcon = icon;

    QWidget::setIcon( theIcon );
#else
    QWidget::setIcon( icon );
#endif

    update();
#endif
}

void QTitleBarP::leaveEvent( QEvent * )
{
    if(autoRaise() && !d->pressed)
	repaint( FALSE );
}

void QTitleBarP::enterEvent( QEvent * )
{
    if(autoRaise() && !d->pressed)
	repaint( FALSE );
    QEvent e( QEvent::Leave );
    QApplication::sendEvent( parentWidget(), &e );
}

void QTitleBarP::setActive( bool active )
{
    if ( d->act == active )
	return ;

    d->act = active;
    update();
}

bool QTitleBarP::isActive() const
{
    return d->act;
}

bool QTitleBarP::usesActiveColor() const
{
    return ( isActive() && isActiveWindow() ) ||
	   ( !window() && topLevelWidget()->isActiveWindow() );
}

QString QTitleBarP::visibleText() const
{
    return d->cuttext;
}

QWidget *QTitleBarP::window() const
{
    return d->window;
}

bool QTitleBarP::event( QEvent* e )
{
    if ( e->type() == QEvent::ApplicationPaletteChange ) {
	readColors();
	return TRUE;
    } else if ( e->type() == QEvent::WindowActivate ) {
	setActive( d->act );
    } else if ( e->type() == QEvent::WindowDeactivate ) {
	bool wasActive = d->act;
	setActive( FALSE );
	d->act = wasActive;
    }

    return QWidget::event( e );
}

void QTitleBarP::setMovable(bool b)
{
    d->movable = b;
}

bool QTitleBarP::isMovable() const
{
    return d->movable;
}

void QTitleBarP::setAutoRaise(bool b)
{
    d->autoraise = b;
}

bool QTitleBarP::autoRaise() const
{
    return d->autoraise;
}

QSize QTitleBarP::sizeHint() const
{
    constPolish();
    QRect menur = style().querySubControlMetrics(QStyle::CC_TitleBar, this,
						 QStyle::SC_TitleBarSysMenu);
    return QSize( menur.width(), style().pixelMetric( QStyle::PM_TitleBarHeight, this ) );
}

