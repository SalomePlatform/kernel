/////////////////////////////////////////////////////////////////////////////
// Module      : PatchQt
// File        : qsplitterP.h
// Description : the patch for Qt's QSplitter class (qsplitter.h)
/////////////////////////////////////////////////////////////////////////////

/****************************************************************************
** $Id$
**
** Defintion of  QSplitter class
**
**  Created:  980105
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
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
#ifndef QSPLITTERP_H
#define QSPLITTERP_H

#ifndef QT_H
#include <qframe.h>
#include <qvaluelist.h>
#include <qwidgetlist.h>	// by STV
#endif // QT_H
#include <qpushbutton.h>

#ifndef QT_NO_COMPLEXWIDGETS

class QSplitterPHandle;
class QSplitterPData;
class QSplitterPLayoutStruct;

class Q_EXPORT QSplitterP : public QFrame
{
    Q_OBJECT
    Q_PROPERTY( Orientation orientation READ orientation WRITE setOrientation )
      
public:
    enum ResizeMode { Stretch, KeepSize, FollowSizeHint };

    QSplitterP( QWidget *parent=0, const char *name=0 );
    QSplitterP( Orientation, QWidget *parent=0, const char *name=0 );
    ~QSplitterP();

    virtual void setOrientation( Orientation );
    Orientation orientation() const { return orient; }

    virtual void setResizeMode( QWidget *w, ResizeMode );
    virtual void setOpaqueResize( bool = TRUE );
    bool opaqueResize() const;

    void moveToFirst( QWidget * );
    void moveToLast( QWidget * );

    void refresh() { recalc( TRUE ); }
    QSize sizeHint() const;
    QSize minimumSizeHint() const;
    QSizePolicy sizePolicy() const;

    QValueList<int> sizes() const;
    void setSizes( QValueList<int> );
    bool isCompressEnabled() const;
    void setCompressEnabled(bool on);

    void compress(QWidget*);
    void unCompress(QWidget*);

// VSR 21/11/02 -> ================================
    QSplitterPHandle* getHandleAfter(QWidget* w);
    QSplitterPHandle* getHandleBefore(QWidget* w);
// VSR 21/11/02 <- ================================

// ASL 07/09/04
   bool isCompressed( const QWidget* ) const;
// ASL 07/09/04

protected:
    void childEvent( QChildEvent * );

    bool event( QEvent * );
    void resizeEvent( QResizeEvent * );

    int idAfter( QWidget* ) const;
    QWidget* widgetBefore( int id ) const;
    QWidget* widgetAfter( int id ) const;

    QSize minSize( const QWidget *w ) const;
    QSize maxSize( const QWidget *w ) const;
    
    void moveSplitter( QCOORD pos, int id );
    virtual void drawSplitter( QPainter*, QCOORD x, QCOORD y,
			       QCOORD w, QCOORD h );
    void styleChange( QStyle& );
    int adjustPos( int , int );
    virtual void setRubberband( int );
    void getRange( int id, int*, int* );

private:
    void init();
    void recalc( bool update = FALSE );
    int hit( QPoint p );
    void doResize();
    void storeSizes();
    void processChildEvents();
    QSplitterPLayoutStruct *addWidget( QWidget*, bool first = FALSE );
    void recalcId();
    void moveBefore( int pos, int id, bool upLeft );
    void moveAfter( int pos, int id, bool upLeft );
    void setG( QWidget *w, int p, int s );
    void updateSplitterHandles() const;

    QCOORD pick( const QPoint &p ) const
    { return orient == Horizontal ? p.x() : p.y(); }
    QCOORD pick( const QSize &s ) const
    { return orient == Horizontal ? s.width() : s.height(); }

    QCOORD trans( const QPoint &p ) const
    { return orient == Vertical ? p.x() : p.y(); }
    QCOORD trans( const QSize &s ) const
    { return orient == Vertical ? s.width() : s.height(); }

    QSplitterPData *data;
    QWidgetList compressed_widgets;

    Orientation orient;
    bool compress_flag;
    friend class QSplitterPHandle;
private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QSplitterP( const QSplitterP & );
    QSplitterP& operator=( const QSplitterP & );
#endif
};

#endif // QT_NO_COMPLEXWIDGETS


class Q_EXPORT QSplitterPHandle : public QWidget
{
  Q_OBJECT
public:
    QSplitterPHandle( Qt::Orientation o,
		      QSplitterP *parent, const char* name=0 );
    void setOrientation( Qt::Orientation o );
    Qt::Orientation orientation() const { return orient; }
  
    bool opaque() const { return s->opaqueResize(); }

    QSize sizeHint() const;
    QSizePolicy sizePolicy() const;

    int id() const { return myId; } // data->list.at(id())->wid == this
    void setId( int i ) { myId = i; }
    void updateButtonState();

// VSR 21/11/02 -> ================================
    void compressAfter();
    void unCompressAfter();
    void compressBefore();
    void unCompressBefore();
// VSR 21/11/02 <- ================================

protected:
    void paintEvent( QPaintEvent * );
    void mouseMoveEvent( QMouseEvent * );
    void mousePressEvent( QMouseEvent * );
    void mouseReleaseEvent( QMouseEvent * );

protected slots:
    void onBeforeCompress();
    void onAfterCompress();
    void onBeforeUnCompress();
    void onAfterUnCompress();

private:
    Qt::Orientation orient;
    bool opaq;
    int myId;

    QSplitterP *s;
    int oldpos;
    QPushButton* left;
    QPushButton* right;
    QPushButton* unleft;
    QPushButton* unright;
};


#endif // QSPLITTERP_H
