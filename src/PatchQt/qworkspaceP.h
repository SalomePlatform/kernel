/////////////////////////////////////////////////////////////////////////////
// Module      : PatchQt
// File        : qworkspaceP.h
// Description : the patch for Qt's QWorkspace class (qworkspace.h)
/////////////////////////////////////////////////////////////////////////////

/****************************************************************************
** $Id$
**
** Definition of the QWorkspace class
**
** Created : 990210
**
** Copyright (C) 1999-2000 Trolltech AS.  All rights reserved.
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

#ifndef QWORKSPACEP_H
#define QWORKSPACEP_H

#ifndef QT_H
#include <qobject.h>
#include <qwidget.h>
#include <qwidgetlist.h>
#include <qframe.h>
#include <qguardedptr.h>
#include <qbutton.h>
#include <qlabel.h>
#endif // QT_H

class QWorkspacePChild;
class QShowEvent;
class QWorkspacePPrivate;
class QPopupMenu;
class QWidgetResizeHandlerP;
class QTitleBarP;

class Q_EXPORT QWorkspaceP : public QWidget
{
    Q_OBJECT
    Q_PROPERTY( bool scrollBarsEnabled READ scrollBarsEnabled WRITE setScrollBarsEnabled )
public:
    QWorkspaceP( QWidget* parent=0, const char* name=0 );
    ~QWorkspaceP();

    QWidget* activeWindow() const;
    QWidgetList windowList() const;

    QSize sizeHint() const;

    bool scrollBarsEnabled() const;
    void setScrollBarsEnabled( bool enable );

    void setPaletteBackgroundColor( const QColor & );
    void setPaletteBackgroundPixmap( const QPixmap & );

signals:
    void windowActivated( QWidget* w);

public slots:
    void cascade();
    void tile();

protected:
#ifndef QT_NO_STYLE
    void styleChange( QStyle& );
#endif
    void childEvent( QChildEvent * );
    void resizeEvent( QResizeEvent * );
    bool eventFilter( QObject *, QEvent * );
    void showEvent( QShowEvent *e );
    void hideEvent( QHideEvent *e );
#ifndef QT_NO_WHEELEVENT
    void wheelEvent( QWheelEvent *e );
#endif

private slots:
    void closeActiveWindow();
    void closeAllWindows();
    void normalizeActiveWindow();
    void minimizeActiveWindow();
    void showOperationMenu();
    void popupOperationMenu( const QPoint& );
    void operationMenuActivated( int );
    void operationMenuAboutToShow();
    void toolMenuAboutToShow();
    void scrollBarChanged();
public slots:
    void activateNextWindow();
    void activatePreviousWindow();

public:
    void activateWindow( QWidget* w, bool change_focus = TRUE );

private:
    void insertIcon( QWidget* w);
    void removeIcon( QWidget* w);
    void place( QWidget* );

    QWorkspacePChild* findChild( QWidget* w);
    void showMaximizeControls();
    void hideMaximizeControls();
    void showWindow( QWidget* w);
    void maximizeWindow( QWidget* w);
    void minimizeWindow( QWidget* w);
    void normalizeWindow( QWidget* w);

    QRect updateWorkspace();

    QPopupMenu* popup;
    QWorkspacePPrivate* d;

    friend class QWorkspacePChild;

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QWorkspaceP( const QWorkspaceP & );
    QWorkspaceP& operator=( const QWorkspaceP & );
#endif
};

class QWorkspacePChild : public QFrame
{
    Q_OBJECT

    friend class QWorkspaceP;
    friend class QTitleBarP;

public:
    QWorkspacePChild( QWidget* window,
		     QWorkspaceP* parent=0, const char* name=0 );
    ~QWorkspacePChild();

    void setActive( bool );
    bool isActive() const;

    void adjustToFullscreen();
    void adjustSize();

    QWidget* windowWidget() const;
    QWidget* iconWidget() const;

    void doResize();
    void doMove();

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    QSize baseSize() const;

signals:
    void showOperationMenu();
    void popupOperationMenu( const QPoint& );

public slots:
    void activate();
    void showMinimized();
    void showMaximized();
    void showNormal();
    void showShaded();
    void setCaption( const QString& );
    void internalRaise();
    void titleBarDoubleClicked();

    void move( int x, int y );

protected:
    void enterEvent( QEvent * );
    void leaveEvent( QEvent * );
    void childEvent( QChildEvent* );
    void resizeEvent( QResizeEvent * );
    void moveEvent( QMoveEvent * );
    bool eventFilter( QObject *, QEvent * );

    bool focusNextPrevChild( bool );

    void drawFrame( QPainter * );
    void styleChange( QStyle & );

private:
    QWidget* childWidget;
    QWidget* lastfocusw;
    QWidgetResizeHandlerP *widgetResizeHandler;
    QTitleBarP* titlebar;
    QGuardedPtr<QTitleBarP> iconw;
    QSize windowSize;
    QSize shadeRestore;
    QSize shadeRestoreMin;
    bool act		    :1;
    bool shademode	    :1;
    bool snappedRight	    :1;
    bool snappedDown	    :1;
};

#ifndef QT_H
#endif // QT_H
class QMouseEvent;
class QKeyEvent;

class Q_EXPORT QWidgetResizeHandlerP : public QObject
{
    Q_OBJECT

public:
    QWidgetResizeHandlerP( QWidget *parent, QWidget *cw = 0, const char *name = 0 );
    void setActive( bool b ) { active = b; if ( !active ) setMouseCursor( Nowhere ); }
    bool isActive() const { return active; }
    void setMovingEnabled( bool b ) { moving = b; }
    bool isMovingEnabled() const { return moving; }

    bool isButtonDown() const { return buttonDown; }

    void setExtraHeight( int h ) { extrahei = h; }
    void setSizeProtection( bool b ) { sizeprotect = b; }

    void doResize();
    void doMove();

signals:
    void activate();

protected:
    bool eventFilter( QObject *o, QEvent *e );
    void mouseMoveEvent( QMouseEvent *e );
    void keyPressEvent( QKeyEvent *e );

private:
    enum MousePosition {
	Nowhere,
	TopLeft, BottomRight, BottomLeft, TopRight,
	Top, Bottom, Left, Right,
	Center
    };

    QWidget *widget;
    QWidget *childWidget;
    QPoint moveOffset;
    QPoint invertedMoveOffset;
    MousePosition mode;
    int extrahei;
    int range;
    uint buttonDown	    :1;
    uint moveResizeMode	    :1;
    uint active		    :1;
    uint sizeprotect	    :1;
    uint moving		    :1; 

    void setMouseCursor( MousePosition m );
    bool isMove() const {
	return moveResizeMode && mode == Center;
    }
    bool isResize() const {
	return moveResizeMode && !isMove();
    }

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QWidgetResizeHandlerP( const QWidgetResizeHandlerP & );
    QWidgetResizeHandlerP& operator=( const QWidgetResizeHandlerP & );
#endif

};

class QToolTip;
class QTitleBarPPrivate;
class QPixmap;

class Q_EXPORT QTitleBarP : public QWidget
{
    Q_OBJECT
    Q_PROPERTY( bool autoRaise READ autoRaise WRITE setAutoRaise )
    Q_PROPERTY( bool movable READ isMovable WRITE setMovable )

public:
    QTitleBarP (QWidget* w, QWidget* parent, const char* name=0);
    ~QTitleBarP();

    bool isActive() const;
    bool usesActiveColor() const;
    virtual QString visibleText() const;

    bool isMovable() const;
    void setMovable(bool);

    bool autoRaise() const;
    void setAutoRaise(bool);

    QWidget *window() const;

    QSize sizeHint() const;

public slots:
    void setActive( bool );
    void setCaption( const QString& title );
    void setIcon( const QPixmap& icon );

signals:
    void doActivate();
    void doNormal();
    void doClose();
    void doMaximize();
    void doMinimize();
    void doShade();
    void showOperationMenu();
    void popupOperationMenu( const QPoint& );
    void doubleClicked();

protected:
    bool event( QEvent *);
    void resizeEvent( QResizeEvent *);
    void contextMenuEvent( QContextMenuEvent * );
    void mousePressEvent( QMouseEvent * );
    void mouseDoubleClickEvent( QMouseEvent * );
    void mouseReleaseEvent( QMouseEvent * );
    void mouseMoveEvent( QMouseEvent * );
    void enterEvent( QEvent *e );
    void leaveEvent( QEvent *e );
    void paintEvent( QPaintEvent *p );

    virtual void cutText();

private:
    void readColors();

    QTitleBarPPrivate *d;
};

#endif // QWORKSPACEP_H
