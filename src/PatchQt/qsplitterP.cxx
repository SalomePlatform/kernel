/////////////////////////////////////////////////////////////////////////////
// Module      : PatchQt
// File        : qsplitterP.cxx
// Description : the patch for Qt's QSplitter class (qsplitter.cpp)
/////////////////////////////////////////////////////////////////////////////

/****************************************************************************
** $Id$
**
**  Splitter widget
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

#include "qsplitterP.h"
#ifndef QT_NO_COMPLEXWIDGETS

#include <qglobal.h>
#include <qpainter.h>
#include <qdrawutil.h>
#include <qbitmap.h>
#include <private/qlayoutengine_p.h>
#if QT_VERSION < 300
  #include <qlist.h>
  #include <qarray.h>
#else
  #include <qptrlist.h>  //qt3.x
  #include <qmemarray.h> //qt3.x
  #include <qstyle.h> //qt3.x
#endif

#include <qobjectlist.h>
#include <qapplication.h> //sendPostedEvents
using namespace std;

#define SPLITTER_ICON_WIDTH 10
#define SPLITTER_ICON_HEIGHT 12

static int mouseOffset;
static int opaqueOldPos = -1; //### there's only one mouse, but this is a bit risky

static const char* splitter_left_xpm[] = {
  "5 5 2 1 0 0",
  " 	s none          m none  c none",
  ".	s iconColor1	m black	c black",
  "   ..",
  " ....",
  ".....",
  " ....",
  "   .."
};

static const char* splitter_right_xpm[] = {
  "5 5 2 1 0 0",
  " 	s iconColor1	m black	c black",
  ".	s none          m none  c none",
  "  ...",
  "    .",
  "     ",
  "    .",
  "  ..."
};

static const char* splitter_up_xpm[] = {
  "5 5 2 1 0 0",
  " 	s none          m none  c none",
  ".	s iconColor1	m black	c black",
  "  .  ",
  " ... ",
  " ... ",
  ".....",
  "....."
};

static const char* splitter_down_xpm[] = {
  "5 5 2 1 0 0",
  " 	s iconColor1	m black	c black",
  ".	s none          m none  c none",
  "     ",
  "     ",
  ".   .",
  ".   .",
  ".. .."
};

QSplitterPHandle::QSplitterPHandle( Qt::Orientation o,
				  QSplitterP *parent, const char * name )
    : QWidget( parent, name )
{
  s = parent;
  
  left  = new QPushButton(this);
  right = new QPushButton(this);
  unleft  = new QPushButton(this);
  unright = new QPushButton(this);
  
  setOrientation(o);
  
  left->setAutoDefault(false); unleft->setAutoDefault(false);	
  right->setAutoDefault(false); unright->setAutoDefault(false);
  left->setFlat(true); right->setFlat(true); unleft->setFlat(true); unright->setFlat(true);
  left->setCursor( arrowCursor );	right->setCursor( arrowCursor );
  unleft->setCursor( arrowCursor ); unright->setCursor( arrowCursor );

// VSR 21/11/02 -> ================================
  setMinimumSize( 1, 1 );
// VSR 21/11/02 <- ================================

  connect(left,    SIGNAL(clicked()), this, SLOT(onBeforeCompress()));
  connect(right,   SIGNAL(clicked()), this, SLOT(onAfterCompress()));
  connect(unleft,  SIGNAL(clicked()), this, SLOT(onBeforeUnCompress()));
  connect(unright, SIGNAL(clicked()), this, SLOT(onAfterUnCompress()));
}

void QSplitterPHandle::onBeforeCompress() {
  oldpos = s->pick(pos());
  int pos = s->pick(s->widgetBefore(id())->pos());
  s->compress(s->widgetBefore(id()));
  s->moveSplitter(pos, id());
  s->updateSplitterHandles();
}

void QSplitterPHandle::onBeforeUnCompress() {
  s->unCompress(s->widgetBefore(id()));
  s->moveSplitter(oldpos, id());
  s->updateSplitterHandles();
}

void QSplitterPHandle::onAfterCompress() {
  oldpos = s->pick(pos());
  int pos = s->pick(s->widgetAfter(id())->pos()) + s->pick(s->widgetAfter(id())->size()) - s->pick(size());
  s->compress(s->widgetAfter(id()));
  s->moveSplitter(pos, id());
  s->updateSplitterHandles();
}

void QSplitterPHandle::onAfterUnCompress() {
  s->unCompress(s->widgetAfter(id()));
  s->moveSplitter(oldpos, id());
  s->updateSplitterHandles();
}

// VSR 21/11/02 -> ================================
void QSplitterPHandle::compressAfter() 
{
  onAfterCompress();
}

void QSplitterPHandle::unCompressAfter() 
{
  onAfterUnCompress();
}

void QSplitterPHandle::compressBefore() 
{
  onBeforeCompress();
}

void QSplitterPHandle::unCompressBefore() 
{
  onBeforeUnCompress();
}
// VSR 21/11/02 <- ================================

QSizePolicy QSplitterPHandle::sizePolicy() const
{
    //### removeme 3.0
    return QWidget::sizePolicy();
}

QSize QSplitterPHandle::sizeHint() const
{
#if QT_VERSION < 300
  int sw = style().splitterWidth();
  return QSize(sw,sw).expandedTo( QApplication::globalStrut() );
#else
  int sw = style().pixelMetric(QStyle::PM_SplitterWidth, this);
  return (style().sizeFromContents(QStyle::CT_Splitter, s, QSize(sw, sw)).
	  expandedTo(QApplication::globalStrut()));
#endif
}

void QSplitterPHandle::setOrientation( Qt::Orientation o )
{
    orient = o;
#ifndef QT_NO_CURSOR
    if ( o == QSplitterP::Horizontal )
	setCursor( splitHCursor );
    else
	setCursor( splitVCursor );
#endif

	QPixmap lp, rp;
	if ( orient == QSplitterP::Horizontal )
	{
		lp = QPixmap(splitter_left_xpm);
		rp = QPixmap(splitter_right_xpm);
	}
	else
	{
		lp = QPixmap(splitter_up_xpm);
		rp = QPixmap(splitter_down_xpm);
	}
	left->setPixmap(lp);
	right->setPixmap(rp);
	unleft->setPixmap(rp);
	unright->setPixmap(lp);
}

void QSplitterPHandle::mouseMoveEvent( QMouseEvent *e )
{
    if ( !(e->state()&LeftButton) )
	return;
    QCOORD pos = s->pick(parentWidget()->mapFromGlobal(e->globalPos()))
		 - mouseOffset;
    if ( opaque() ) {
	s->moveSplitter( pos, id() );
    } else {
	int min = pos; int max = pos;
	s->getRange( id(), &min, &max );
	s->setRubberband( QMAX( min, QMIN(max, pos )));
    }
}

void QSplitterPHandle::mousePressEvent( QMouseEvent *e )
{
    if ( e->button() == LeftButton )
	mouseOffset = s->pick(e->pos());
}

void QSplitterPHandle::mouseReleaseEvent( QMouseEvent *e )
{
    if ( !opaque() && e->button() == LeftButton ) {
	QCOORD pos = s->pick(parentWidget()->mapFromGlobal(e->globalPos()));
	s->setRubberband( -1 );
	s->moveSplitter( pos, id() );
    }
}

void QSplitterPHandle::paintEvent( QPaintEvent * )
{
	updateButtonState();
    QPainter p( this );
    s->drawSplitter( &p, 0, 0, width(), height() );
}

void QSplitterPHandle::updateButtonState() {
	if (!s->isCompressEnabled()) {
		left->hide();
		right->hide();
		unleft->hide();
		unright->hide();
	}
	else {
		if ( orient == QSplitterP::Horizontal )
		{
			left->setGeometry( QRect( QPoint( (width() - SPLITTER_ICON_WIDTH)/2, 0 ),
									  QSize ( SPLITTER_ICON_WIDTH, SPLITTER_ICON_HEIGHT ) ) );
			right->setGeometry( QRect( QPoint( (width() - SPLITTER_ICON_WIDTH)/2, SPLITTER_ICON_HEIGHT ),
					                   QSize ( SPLITTER_ICON_WIDTH, SPLITTER_ICON_HEIGHT ) ) );
			unleft->setGeometry( left->geometry() );
			unright->setGeometry( right->geometry() );
		}
		else
		{
			left->setGeometry( QRect( QPoint( 0, (height() - SPLITTER_ICON_WIDTH)/2 ),
									  QSize ( SPLITTER_ICON_HEIGHT, SPLITTER_ICON_WIDTH ) ) );
			right->setGeometry( QRect( QPoint( SPLITTER_ICON_HEIGHT, (height() - SPLITTER_ICON_WIDTH)/2),
									   QSize ( SPLITTER_ICON_HEIGHT, SPLITTER_ICON_WIDTH ) ) );
			unleft->setGeometry( left->geometry() );
			unright->setGeometry( right->geometry() );
		}
		if ( s->isCompressed( s->widgetBefore( id() ) ) ) {
			left->hide();
			unleft->show();
			right->setEnabled(false);
		}
		else {
			unleft->hide();
			left->show();
			right->setEnabled(true);
		}
		if ( s->isCompressed( s->widgetAfter( id() ) ) ) {
			right->hide();
			unright->show();
			left->setEnabled(false);
		}
		else {
			unright->hide();
			right->show();
			left->setEnabled(true);
		}
	}
}

class QSplitterPLayoutStruct
{
public:
    QSplitterP::ResizeMode mode;
    QCOORD sizer;
    bool isSplitter;
    QWidget *wid;
};

class QSplitterPData
{
public:
    QSplitterPData() : opaque( FALSE ), firstShow( TRUE ) {}

    QList<QSplitterPLayoutStruct> list;
    bool opaque;
    bool firstShow;
};


// NOT REVISED
/*!
  \class QSplitter qsplitter.h
  \brief The QSplitter class implements a splitter widget.

  \ingroup organizers

  A splitter lets the user control the size of child widgets by
  dragging the boundary between the children. Any number of widgets
  may be controlled.

  To show a QListBox, a QListView and a QMultiLineEdit side by side:

  \code
    QSplitter *split = new QSplitter( parent );
    QListBox *lb = new QListBox( split );
    QListView *lv = new QListView( split );
    QMultiLineEdit *ed = new QMultiLineEdit( split );
  \endcode

  In QSplitter the boundary can be either horizontal or vertical.  The
  default is horizontal (the children are side by side) and you
  can use setOrientation( QSplitter::Vertical ) to set it to vertical.

  By default, all widgets can be as large or as small as the user
  wishes, down to \link QWidget::minimumSizeHint() minimumSizeHint()\endlink.
  You can naturally use setMinimumSize() and/or
  setMaximumSize() on the children. Use setResizeMode() to specify that
  a widget should keep its size when the splitter is resized.

  QSplitter normally resizes the children only at the end of a
  resize operation, but if you call setOpaqueResize( TRUE ), the
  widgets are resized as often as possible.

  The initial distribution of size between the widgets is determined
  by the initial size of each widget. You can also use setSizes() to
  set the sizes of all the widgets. The function sizes() returns the
  sizes set by the user.

  If you hide() a child, its space will be distributed among the other
  children. When you show() it again, it will be reinstated.

  <img src=qsplitter-m.png> <img src=qsplitter-w.png>

  \sa QTabBar
*/



QSize QSplitterP::minSize( const QWidget *w ) const
{
 if ( isCompressed(w) && (data->list.count() > 1 ) ) return QSize(0,0);
 QSize min = w->minimumSize();
 QSize s;
 if ( min.height() <= 0 || min.width() <= 0 )
    s = w->minimumSizeHint();
 if ( min.height() > 0 )
    s.setHeight( min.height() );
 if ( min.width() > 0 )
    s.setWidth( min.width() );
 return s.expandedTo(QSize(0,0));
}

QSize QSplitterP::maxSize( const QWidget* w ) const
{
  if ( isCompressed(w) && (data->list.count() > 1 ) ) return QSize(0,0);
  else return w->maximumSize();
}

/*!
  Constructs a horizontal splitter.
*/

QSplitterP::QSplitterP( QWidget *parent, const char *name )
     :QFrame(parent,name,WPaintUnclipped)
{
  orient = Horizontal;
  init();
}


/*!
  Constructs splitter with orientation \a o.
*/

QSplitterP::QSplitterP( Orientation o, QWidget *parent, const char *name )
    :QFrame(parent,name,WPaintUnclipped)
{
  orient = o;
  init();
}


/*!
  Destructs the splitter.
*/

QSplitterP::~QSplitterP()
{
    data->list.setAutoDelete( TRUE );
    delete data;
}


void QSplitterP::init()
{
    data = new QSplitterPData;
    if ( orient == Horizontal )
	setSizePolicy( QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Minimum) );
    else
    	setSizePolicy( QSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed) );
	compressed_widgets.clear();
	compress_flag = false;
}


/*!
  \fn void QSplitter::refresh()

  Updates the splitter state. You should not need to call this
  function during normal use of the splitter.
*/


/*!  Sets the orientation to \a o.  By default the orientation is
  horizontal (the widgets are side by side).

  \sa orientation()
*/

void QSplitterP::setOrientation( Orientation o )
{
    if ( orient == o )
	return;
    orient = o;

    if ( orient == Horizontal )
	setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Minimum ) );
    else
    	setSizePolicy( QSizePolicy( QSizePolicy::Minimum, QSizePolicy::Fixed ) );

    QSplitterPLayoutStruct *s = data->list.first();
    while ( s ) {
	if ( s->isSplitter )
	    ((QSplitterPHandle*)s->wid)->setOrientation( o );
	s = data->list.next();  // ### next at end of loop, no iterator
    }
    recalc( isVisible() );
}


/*!
   \fn Orientation QSplitter::orientation() const

   Returns the orientation (\c Horizontal or \c Vertical) of the splitter.
   \sa setOrientation()
*/

/*!
  \reimp
*/
void QSplitterP::resizeEvent( QResizeEvent * )
{
    doResize();
}


/*!
  Inserts the widget \a w at the end, or at the beginning if \a first is TRUE

  It is the responsibility of the caller of this function to make sure
  that \a w is not already in the splitter, and to call recalcId if
  needed.  (If \a first is TRUE, then recalcId is very probably
  needed.)
*/

QSplitterPLayoutStruct *QSplitterP::addWidget( QWidget *w, bool first )
{
    QSplitterPLayoutStruct *s;
    QSplitterPHandle *newHandle = 0;
    if ( data->list.count() > 0 ) {
	s = new QSplitterPLayoutStruct;
	s->mode = KeepSize;
	newHandle = new QSplitterPHandle( orientation(), this );
	s->wid = newHandle;
	newHandle->setId(data->list.count());
	s->isSplitter = TRUE;
	s->sizer = pick( newHandle->sizeHint() );
	if ( first )
	    data->list.insert( 0, s );
	else
	    data->list.append( s );
    }
    s = new QSplitterPLayoutStruct;
    s->mode = Stretch;
    s->wid = w;
    if ( !testWState( WState_Resized ) && w->sizeHint().isValid() )
	s->sizer = pick( w->sizeHint() );
    else
	s->sizer = pick( w->size() );
    s->isSplitter = FALSE;
    if ( first )
	data->list.insert( 0, s );
    else
	data->list.append( s );
    if ( newHandle && isVisible() )
	newHandle->show(); //will trigger sending of post events
    return s;
}


/*!
  Tells the splitter that a child widget has been inserted/removed.
*/

void QSplitterP::childEvent( QChildEvent *c )
{
    if ( c->type() == QEvent::ChildInserted ) {
	if ( !c->child()->isWidgetType() )
	    return;

	if ( ((QWidget*)c->child())->testWFlags( WType_TopLevel ) )
	    return;

	QSplitterPLayoutStruct *s = data->list.first();
	while ( s ) {
	    if ( s->wid == c->child() )
		return;
	    s = data->list.next();
	}
	addWidget( (QWidget*)c->child() );
	recalc( isVisible() );

    } else if ( c->type() == QEvent::ChildRemoved ) {
	QSplitterPLayoutStruct *p = 0;
	if ( data->list.count() > 1 )
	    p = data->list.at(1); //remove handle _after_ first widget.
	QSplitterPLayoutStruct *s = data->list.first();
	while ( s ) {
	    if ( s->wid == c->child() ) {
		data->list.removeRef( s );
		delete s;
		if ( p && p->isSplitter ) {
		    data->list.removeRef( p );
		    delete p->wid; //will call childEvent
		    delete p;
		}
		recalcId();
		doResize();
		return;
	    }
	    p = s;
	    s = data->list.next();
	}
    }
}


/*!
  Shows a rubber band at position \a p. If \a p is negative, the
  rubber band is removed.
*/

void QSplitterP::setRubberband( int p )
{
    QPainter paint( this );
    paint.setPen( gray );
    paint.setBrush( gray );
    paint.setRasterOp( XorROP );
    QRect r = contentsRect();
    const int rBord = 3; //Themable????
#if QT_VERSION < 300
    const int sw = style().splitterWidth();
#else
    int sw = style().pixelMetric(QStyle::PM_SplitterWidth, this); //qt3.x
#endif
    if ( orient == Horizontal ) {
	if ( opaqueOldPos >= 0 )
	    paint.drawRect( opaqueOldPos + sw/2 - rBord , r.y(),
			    2*rBord, r.height() );
	if ( p >= 0 )
	    paint.drawRect( p  + sw/2 - rBord, r.y(), 2*rBord, r.height() );
    } else {
	if ( opaqueOldPos >= 0 )
	    paint.drawRect( r.x(), opaqueOldPos + sw/2 - rBord,
			    r.width(), 2*rBord );
	if ( p >= 0 )
	    paint.drawRect( r.x(), p + sw/2 - rBord, r.width(), 2*rBord );
    }
    opaqueOldPos = p;
}


/*! \reimp */

bool QSplitterP::event( QEvent *e )
{
    if ( e->type() == QEvent::LayoutHint || ( e->type() == QEvent::Show && data->firstShow ) ) {
	recalc( isVisible() );
	if ( e->type() == QEvent::Show )
	    data->firstShow = FALSE;
    }
    return QWidget::event( e );
}


/*!
  Draws the splitter handle in the rectangle described by \a x, \a y,
  \a w, \a h using painter \a p.
  \sa QStyle::drawSplitter
*/

void QSplitterP::drawSplitter( QPainter *p,
			      QCOORD x, QCOORD y, QCOORD w, QCOORD h )
{
#if QT_VERSION < 300
    style().drawSplitter( p, x, y, w, h, colorGroup(), orient );
#else
    style().drawPrimitive(QStyle::PE_Splitter, p, QRect(x, y, w, h), colorGroup(),
			  (orientation() == Qt::Horizontal ?
			   QStyle::Style_Horizontal : 0));
#endif
}


/*!
  Returns the id of the splitter to the right of or below the widget \a w,
  or 0 if there is no such splitter.
  (ie. it is either not in this QSplitter, or it is at the end).
*/

int QSplitterP::idAfter( QWidget* w ) const
{
    QSplitterPLayoutStruct *s = data->list.first();
    bool seen_w = FALSE;
    while ( s ) {
	if ( s->isSplitter && seen_w )
	    return data->list.at();
	if ( !s->isSplitter && s->wid == w )
	    seen_w = TRUE;
	s = data->list.next();
    }
    return 0;
}

// VSR 21/11/02 -> ================================
QSplitterPHandle* QSplitterP::getHandleAfter(QWidget* w)
{
  QSplitterPLayoutStruct *s = data->list.first();
  bool seen_w = FALSE;
  while ( s ) {
    if ( s->isSplitter && seen_w )
      return (QSplitterPHandle*)s->wid;
    if ( !s->isSplitter && s->wid == w )
      seen_w = TRUE;
    s = data->list.next();
  }
  return 0;
}

QSplitterPHandle* QSplitterP::getHandleBefore(QWidget* w)
{
  QSplitterPLayoutStruct *s = data->list.first();
  QSplitterPHandle* h = 0;
  while ( s ) {
    if ( s->isSplitter )
      h = (QSplitterPHandle*)s;
    if ( !s->isSplitter && s->wid == w )
      return h;
    s = data->list.next();
  }
  return 0;
}
// VSR 21/11/02 <- ================================

QWidget* QSplitterP::widgetBefore( int id ) const
{
    QSplitterPLayoutStruct *s = data->list.first();
	QWidget* w;
    while ( s ) {
		if ( !s->isSplitter ) {
			w = s->wid;
			if (idAfter(w) == id) return w;
		}
		s = data->list.next();
    }
    return 0;
}

QWidget* QSplitterP::widgetAfter( int id ) const
{
    QSplitterPLayoutStruct *s = data->list.first();
	bool seen_s = FALSE;
    while ( s ) {
	if ( !s->isSplitter && seen_s )
	    return s->wid;
	if ( s->isSplitter && data->list.at() == id )
	    seen_s = TRUE;
	s = data->list.next();
    }
    return 0;
}

void QSplitterP::unCompress(QWidget* w) {
   compressed_widgets.remove(w);
}

void QSplitterP::compress(QWidget* w)
{
  if (!isCompressed(w)) compressed_widgets.append(w);
}

bool QSplitterP::isCompressed(const QWidget* w) const {
  QWidget* t = (QWidget*)w;
  if (compressed_widgets.containsRef(t)==0) return false;
  else return true;
}

/*!
  Moves the left/top edge of the splitter handle with id \a id as
  close as possible to \a p which is the distance from the left (or
  top) edge of the widget.

  \sa idAfter()
*/
void QSplitterP::moveSplitter( QCOORD p, int id )
{
    p = adjustPos( p, id );

    QSplitterPLayoutStruct *s = data->list.at(id);
    int oldP = orient == Horizontal? s->wid->x() : s->wid->y();
    bool upLeft = p < oldP;

    moveAfter( p, id, upLeft );
    moveBefore( p-1, id-1, upLeft );

    storeSizes();
}


void QSplitterP::setG( QWidget *w, int p, int s )
{
    if ( orient == Horizontal )
	w->setGeometry( p, contentsRect().y(), s, contentsRect().height() );
    else
	w->setGeometry( contentsRect().x(), p, contentsRect().width(), s );
}


/*!
  Places the right/bottom edge of the widget at \a id at position \a pos.

  \sa idAfter()
*/

void QSplitterP::moveBefore( int pos, int id, bool upLeft )
{
    QSplitterPLayoutStruct *s = data->list.at(id);
    if ( !s )
	return;
    QWidget *w = s->wid;
    if ( w->isHidden() ) {
	moveBefore( pos, id-1, upLeft );
    } else if ( s->isSplitter ) {
	int dd = s->sizer;
	if ( upLeft ) {
	    setG( w, pos-dd+1, dd );
	    moveBefore( pos-dd, id-1, upLeft );
	} else {
	    moveBefore( pos-dd, id-1, upLeft );
	    setG( w, pos-dd+1, dd );
	}
    } else {
	int left = pick( w->pos() );
	int dd = pos - left + 1;
	dd = QMAX( pick(minSize(w)), QMIN(dd, pick(maxSize(w))));
	int newLeft = pos-dd+1;
	setG( w, newLeft, dd );
	if ( left != newLeft )
	    moveBefore( newLeft-1, id-1, upLeft );
    }
}


/*!
  Places the left/top edge of the widget at \a id at position \a pos.

  \sa idAfter()
*/

void QSplitterP::moveAfter( int pos, int id, bool upLeft )
{
    QSplitterPLayoutStruct *s = id < int(data->list.count()) ?
			       data->list.at(id) : 0;
    if ( !s )
	return;
    QWidget *w = s->wid;
    if ( w->isHidden() ) {
	moveAfter( pos, id+1, upLeft );
    } else if ( pick( w->pos() ) == pos ) {
	//No need to do anything if it's already there.
	return;
    } else if ( s->isSplitter ) {
	int dd = s->sizer;
	if ( upLeft ) {
	    setG( w, pos, dd );
	    moveAfter( pos+dd, id+1, upLeft );
	} else {
	    moveAfter( pos+dd, id+1, upLeft );
	    setG( w, pos, dd );
	}
    } else {
	int right = pick( w->geometry().bottomRight() );

       	int dd = right - pos + 1;
	dd = QMAX( pick(minSize(w)), QMIN(dd, pick(maxSize(w))));
	int newRight = pos+dd-1;
	setG( w, pos, dd );
	moveAfter( newRight+1, id+1, upLeft );
    }
}


/*!
  Returns the valid range of the splitter with id \a id in \a min and \a max.

  \sa idAfter()
*/

void QSplitterP::getRange( int id, int *min, int *max )
{
    int minB = 0;	//before
    int maxB = 0;
    int minA = 0;
    int maxA = 0;	//after
    int n = data->list.count();
    if ( id < 0 || id >= n )
	return;
    int i;
    for ( i = 0; i < id; i++ ) {
	QSplitterPLayoutStruct *s = data->list.at(i);
	if ( s->isSplitter ) {
	    minB += s->sizer;
	    maxB += s->sizer;
	} else {
	    minB += pick( minSize(s->wid) );
	    maxB += pick( maxSize(s->wid) );
	}
    }
    for ( i = id; i < n; i++ ) {
	QSplitterPLayoutStruct *s = data->list.at(i);
	if ( s->isSplitter ) {
	    minA += s->sizer;
	    maxA += s->sizer;
	} else {
	    minA += pick( minSize(s->wid) );
	    maxA += pick( maxSize(s->wid) );
	}
    }
    QRect r = contentsRect();
    if ( min )
	*min = pick(r.topLeft()) + QMAX( minB, pick(r.size())-maxA );
    if ( max )
	*max = pick(r.topLeft()) + QMIN( maxB, pick(r.size())-minA );

}


/*!
  Returns the legal position closest to \a p of the splitter with id \a id.

  \sa idAfter()
*/

int QSplitterP::adjustPos( int p, int id )
{
    int min = 0;
    int max = 0;
    getRange( id, &min, &max );
    p = QMAX( min, QMIN( p, max ) );

    return p;
}


void QSplitterP::doResize()
{
    QRect r = contentsRect();
    int i;
    int n = data->list.count();
    QArray<QLayoutStruct> a( n );
    for ( i = 0; i< n; i++ ) {
	a[i].init();
	QSplitterPLayoutStruct *s = data->list.at(i);
	if ( s->wid->isHidden() ) {
	    a[i].stretch = 0;
	    a[i].sizeHint = a[i].minimumSize = 0;
	    a[i].maximumSize = 0;
	} else if ( s->isSplitter ) {
	    a[i].stretch = 0;
	    a[i].sizeHint = a[i].minimumSize = a[i].maximumSize = s->sizer;
	    a[i].empty = FALSE;
	} else if ( s->mode == KeepSize ) {
	    a[i].stretch = 0;
	    a[i].minimumSize = pick( minSize(s->wid) );
	    a[i].sizeHint = s->sizer;
	    a[i].maximumSize = pick( maxSize(s->wid) );
	    a[i].empty = FALSE;
	} else if ( s->mode == FollowSizeHint ) {
	    a[i].stretch = 0;
	    a[i].minimumSize = a[i].sizeHint = pick( s->wid->sizeHint() );
	    a[i].maximumSize = pick( maxSize(s->wid) );
	    a[i].empty = FALSE;
	} else { //proportional
	    a[i].stretch = s->sizer;
	    a[i].maximumSize = pick( maxSize(s->wid) );
	    a[i].sizeHint = a[i].minimumSize = pick( minSize(s->wid) );
	    a[i].empty = FALSE;
	}
    }

    qGeomCalc( a, 0, n, pick( r.topLeft() ), pick( r.size() ), 0 );
    for ( i = 0; i< n; i++ ) {
	QSplitterPLayoutStruct *s = data->list.at(i);
	if ( orient == Horizontal )
	    s->wid->setGeometry( a[i].pos, r.top(), a[i].size, r.height() );
	else
	    s->wid->setGeometry( r.left(), a[i].pos, r.width(), a[i].size );
    }

}


void QSplitterP::recalc( bool update )
{
    int fi = 2*frameWidth();
    int maxl = fi;
    int minl = fi;
    int maxt = QWIDGETSIZE_MAX;
    int mint = fi;
    int n = data->list.count();
    bool first = TRUE;
    /*
      The splitter before a hidden widget is always hidden.
      The splitter before the first visible widget is hidden.
      The splitter before any other visible widget is visible.
    */
    for ( int i = 0; i< n; i++ ) {
	QSplitterPLayoutStruct *s = data->list.at(i);
	if ( !s->isSplitter ) {
	    QSplitterPLayoutStruct *p = (i > 0) ? p = data->list.at( i-1 ) : 0;
	    if ( p && p->isSplitter )
		if ( first || s->wid->isHidden() )
		    p->wid->hide(); //may trigger new recalc
		else
		    p->wid->show(); //may trigger new recalc
	    if ( !s->wid->isHidden() )
		first = FALSE;
	}
    }

    bool empty=TRUE;
    for ( int j = 0; j< n; j++ ) {
	QSplitterPLayoutStruct *s = data->list.at(j);
	if ( !s->wid->isHidden() ) {
	    empty = FALSE;
	    if ( s->isSplitter ) {
		minl += s->sizer;
		maxl += s->sizer;
	    } else {
		QSize minS = minSize(s->wid);
		minl += pick( minS );
		maxl += pick( maxSize(s->wid) );
		mint = QMAX( mint, trans( minS ));
		int tm = trans( maxSize(s->wid) );
		if ( tm > 0 )
		    maxt = QMIN( maxt, tm );
	    }
	}
    }
    if ( empty )
	maxl = maxt = 0;
    else
	maxl = QMIN( maxl, QWIDGETSIZE_MAX );
    if ( maxt < mint )
	maxt = mint;

    if ( orient == Horizontal ) {
	setMaximumSize( maxl, maxt );
	setMinimumSize( minl, mint );
    } else {
	setMaximumSize( maxt, maxl );
	setMinimumSize( mint, minl );
    }
    if ( update )
	doResize();
}

/*! \enum QSplitter::ResizeMode

  This enum type describes how QSplitter will resize each of its child widgets.  The currently defined values are: <ul>

  <li> \c Stretch - the widget will be resized when the splitter
  itself is resized.

  <li> \c KeepSize - QSplitter will try to keep this widget's size
  unchanged.

  <li> \c FollowSizeHint - QSplitter will resize the widget when its
  size hint changes.

  </ul>

*/

/*!
  Sets resize mode of \a w to \a mode.

  \sa ResizeMode
*/

void QSplitterP::setResizeMode( QWidget *w, ResizeMode mode )
{
    processChildEvents();
    QSplitterPLayoutStruct *s = data->list.first();
    while ( s ) {
	if ( s->wid == w  ) {
	    s->mode = mode;
	    return;
	}
	s = data->list.next();
    }
    s = addWidget( w, TRUE );
    s->mode = mode;
}


/*!
  Returns TRUE if opaque resize is on, FALSE otherwise.

  \sa setOpaqueResize()
*/

bool QSplitterP::opaqueResize() const
{
    return data->opaque;
}


/*!
  Sets opaque resize to \a on. Opaque resize is initially turned off.

  \sa opaqueResize()
*/

void QSplitterP::setOpaqueResize( bool on )
{
    data->opaque = on;
}


/*!
  Moves \a w to the leftmost/top position.
*/

void QSplitterP::moveToFirst( QWidget *w )
{
    processChildEvents();
    bool found = FALSE;
    QSplitterPLayoutStruct *s = data->list.first();
    while ( s ) {
	if ( s->wid == w  ) {
	    found = TRUE;
	    QSplitterPLayoutStruct *p = data->list.prev();
	    if ( p ) { // not already at first place
		data->list.take(); //take p
		data->list.take(); // take s
		data->list.insert( 0, p );
		data->list.insert( 0, s );
	    }
	    break;
	}
	s = data->list.next();
    }
     if ( !found )
	addWidget( w, TRUE );
     recalcId();
}


/*!
  Moves \a w to the rightmost/bottom position.
*/

void QSplitterP::moveToLast( QWidget *w )
{
    processChildEvents();
    bool found = FALSE;
    QSplitterPLayoutStruct *s = data->list.first();
    while ( s ) {
	if ( s->wid == w  ) {
	    found = TRUE;
	    data->list.take(); // take s
	    QSplitterPLayoutStruct *p = data->list.current();
	    if ( p ) { // the splitter handle after s
		data->list.take(); //take p
		data->list.append( p );
	    }
	    data->list.append( s );
	    break;
	}
	s = data->list.next();
    }
     if ( !found )
	addWidget( w);
     recalcId();
}


void QSplitterP::recalcId()
{
    int n = data->list.count();
    for ( int i = 0; i < n; i++ ) {
	QSplitterPLayoutStruct *s = data->list.at(i);
	if ( s->isSplitter )
	    ((QSplitterPHandle*)s->wid)->setId(i);
    }
}


/*!\reimp
*/
QSize QSplitterP::sizeHint() const
{
    constPolish();
    int l = 0;
    int t = 0;
    if ( children() ) {
	const QObjectList * c = children();
	QObjectListIt it( *c );
	QObject * o;

	while( (o=it.current()) != 0 ) {
	    ++it;
	    if ( o->isWidgetType() &&
		 !((QWidget*)o)->isHidden() ) {
		QSize s = ((QWidget*)o)->sizeHint();
		if ( s.isValid() ) {
		    l += pick( s );
		    t = QMAX( t, trans( s ) );
		}
	    }
	}
    }
    return orientation() == Horizontal ? QSize( l, t ) : QSize( t, l );
}


/*!
\reimp
*/

QSize QSplitterP::minimumSizeHint() const
{
    constPolish();
    int l = 0;
    int t = 0;
    if ( children() ) {
	const QObjectList * c = children();
	QObjectListIt it( *c );
	QObject * o;

	while( (o=it.current()) != 0 ) {
	    ++it;
	    if ( o->isWidgetType() &&
		 !((QWidget*)o)->isHidden() ) {
		QSize s = minSize((QWidget*)o);
		if ( s.isValid() ) {
		    l += pick( s );
		    t = QMAX( t, trans( s ) );
		}
	    }
	}
    }
    return orientation() == Horizontal ? QSize( l, t ) : QSize( t, l );
}



/*!\reimp
*/
QSizePolicy QSplitterP::sizePolicy() const
{
    return QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
}


/*!
  Calculates stretch parameters from current sizes
*/

void QSplitterP::storeSizes()
{
    QSplitterPLayoutStruct *s = data->list.first();
    while ( s ) {
	if ( !s->isSplitter )
	    s->sizer = pick( s->wid->size() );
	s = data->list.next();
    }
}


#if 0 // ### remove this code ASAP

/*!
  Hides \a w if \a hide is TRUE, and updates the splitter.

  \warning Due to a limitation in the current implementation,
  calling QWidget::hide() will not work.
*/

void QSplitterP::setHidden( QWidget *w, bool hide )
{
    if ( w == w1 ) {
	w1show = !hide;
    } else if ( w == w2 ) {
	w2show = !hide;
    } else {
#ifdef CHECK_RANGE
	qWarning( "QSplitterP::setHidden(), unknown widget" );
#endif
	return;
    }
    if ( hide )
	w->hide();
    else
	w->show();
    recalc( TRUE );
}


/*!
  Returns the hidden status of \a w
*/

bool QSplitterP::isHidden( QWidget *w ) const
{
    if ( w == w1 )
	return !w1show;
     else if ( w == w2 )
	return !w2show;
#ifdef CHECK_RANGE
    else
	qWarning( "QSplitterP::isHidden(), unknown widget" );
#endif
    return FALSE;
}
#endif


/*!
  Returns a list of the size parameters of all the widgets in this
  splitter.

  Giving the values to setSizes() will give a splitter with the same
  layout as this one.
*/

QValueList<int> QSplitterP::sizes() const
{
    if ( !testWState(WState_Polished) ) {
	QWidget* that = (QWidget*) this;
	that->polish();
    }
    QValueList<int> list;
    QSplitterPLayoutStruct *s = data->list.first();
    while ( s ) {
	if ( !s->isSplitter )
	    list.append( s->sizer );
	s = data->list.next();
    }
    return list;
}



/*!
  Sets the size parameters to the values given in \a list.
  If the splitterP is horizontal, the values set the sizes from
  left to right. If it is vertical, the sizes are applied from
  top to bottom.
  Extra values in \a list are ignored.

  If \a list contains too few values, the result is undefined
  but the program will still be well-behaved.
*/

void QSplitterP::setSizes( QValueList<int> list )
{
    processChildEvents();
    QValueList<int>::Iterator it = list.begin();
    QSplitterPLayoutStruct *s = data->list.first();
    while ( s && it != list.end() ) {
	if ( !s->isSplitter ) {
	    s->sizer = *it;
	    ++it;
	}
	s = data->list.next();
    }
    doResize();
}


/*!
  Gets all posted child events, ensuring that the internal state of
  the splitter is consistent with the programmer's idea.
*/

void QSplitterP::processChildEvents()
{
    QApplication::sendPostedEvents( this, QEvent::ChildInserted );
}


/*!
  \reimp
*/

void QSplitterP::styleChange( QStyle& old )
{
#if QT_VERSION < 300
    int sw = style().splitterWidth();
#else
    int sw = style().pixelMetric(QStyle::PM_SplitterWidth, this); //qt3.x
#endif
    QSplitterPLayoutStruct *s = data->list.first();
    while ( s ) {
	if ( s->isSplitter )
	    s->sizer = sw;
	s = data->list.next();
    }
    doResize();
    QFrame::styleChange( old );
}
#endif

bool QSplitterP::isCompressEnabled() const
{ 
	return compress_flag; 
}

void QSplitterP::setCompressEnabled(bool on) {
	compress_flag = on;
}

void QSplitterP::updateSplitterHandles() const {
    QSplitterPLayoutStruct *s = data->list.first();
    while ( s ) {
	if ( s->isSplitter )
	    ((QSplitterPHandle*)s->wid)->updateButtonState();
	s = data->list.next();
    }
}
