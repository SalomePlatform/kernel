//  SALOME OCCViewer : build OCC Viewer into Salome desktop
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
//  File   : OCCViewer_ViewPort.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include <stdlib.h>

#if !defined WNT
#define QT_CLEAN_NAMESPACE         /* avoid definition of INT32 and INT8 */
#endif

#include "OCCViewer_ViewPort.h"

#include "QAD.h"
#include "QAD_Tools.h"
#include "QAD_Desktop.h"
#include "QAD_MessageBox.h"

#if !defined WNT
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xmu/StdCmap.h>
#undef QT_CLEAN_NAMESPACE
#include <Xw_Window.hxx>
#include <Graphic3d_GraphicDevice.hxx>

#include <qpixmap.h>
#include <qintdict.h>

#include "utilities.h"

using namespace std;

/* XPM */
const char* imageZoomCursor[] = { 
"32 32 3 1",
". c None",
"a c #000000",
"# c #ffffff",
"................................",
"................................",
".#######........................",
"..aaaaaaa.......................",
"................................",
".............#####..............",
"...........##.aaaa##............",
"..........#.aa.....a#...........",
".........#.a.........#..........",
".........#a..........#a.........",
"........#.a...........#.........",
"........#a............#a........",
"........#a............#a........",
"........#a............#a........",
"........#a............#a........",
".........#...........#.a........",
".........#a..........#a.........",
".........##.........#.a.........",
"........#####.....##.a..........",
".......###aaa#####.aa...........",
"......###aa...aaaaa.......#.....",
".....###aa................#a....",
"....###aa.................#a....",
"...###aa...............#######..",
"....#aa.................aa#aaaa.",
".....a....................#a....",
"..........................#a....",
"...........................a....",
"................................",
"................................",
"................................",
"................................"};

const char* imageRotateCursor[] = { 
"32 32 3 1",
". c None",
"a c #000000",
"# c #ffffff",
"................................",
"................................",
"................................",
"................................",
"........#.......................",
".......#.a......................",
"......#######...................",
".......#aaaaa#####..............",
"........#..##.a#aa##........##..",
".........a#.aa..#..a#.....##.aa.",
".........#.a.....#...#..##.aa...",
".........#a.......#..###.aa.....",
"........#.a.......#a..#aa.......",
"........#a.........#..#a........",
"........#a.........#a.#a........",
"........#a.........#a.#a........",
"........#a.........#a.#a........",
".........#.........#a#.a........",
"........##a........#a#a.........",
"......##.a#.......#.#.a.........",
"....##.aa..##.....##.a..........",
"..##.aa.....a#####.aa...........",
"...aa.........aaa#a.............",
"................#.a.............",
"...............#.a..............",
"..............#.a...............",
"...............a................",
"................................",
"................................",
"................................",
"................................",
"................................"};

struct CMapEntry {
  CMapEntry();
  ~CMapEntry();
  Colormap cmap;
  bool alloc;
  XStandardColormap	scmap;
};

CMapEntry::CMapEntry()
{
  cmap = 0;
  alloc = false;
  scmap.colormap = 0;
}

CMapEntry::~CMapEntry()
{
  if ( alloc )
    XFreeColormap( QPaintDevice::x11AppDisplay(), cmap );
}

static QIntDict<CMapEntry> *cmap_dict = 0;
static bool mesa_gl = false;

static void cleanup_cmaps()
{
  if ( !cmap_dict )
    return;
  cmap_dict->setAutoDelete(true);
  delete cmap_dict;
  cmap_dict = 0;
}

static Colormap choose_cmap(Display *dpy, XVisualInfo *vi)
{
  if ( !cmap_dict ) 
    {
      cmap_dict = new QIntDict<CMapEntry>;
      const char *v = glXQueryServerString( dpy, vi->screen, GLX_VERSION );
      mesa_gl = strstr(v,"Mesa") != 0;
      qAddPostRoutine( cleanup_cmaps );
    }
  
  CMapEntry *x = cmap_dict->find( (long)vi->visualid );
  if ( x )					// found colormap for visual
    return x->cmap;
  
  x = new CMapEntry();
  
  XStandardColormap *c;
  int n, i;
  
#ifdef DEBUG
  MESSAGE( "Choosing cmap for vID = " << vi->visualid );
#endif
  
  if ( vi->visualid == XVisualIDFromVisual( (Visual*)QPaintDevice::x11AppVisual() ) ) 
    {
#ifdef DEBUG
      MESSAGE( "Using x11AppColormap" );
#endif
      return QPaintDevice::x11AppColormap();
    }
  
  if ( mesa_gl ) 
    {	
      Atom hp_cmaps = XInternAtom( dpy, "_HP_RGB_SMOOTH_MAP_LIST", true );
      if ( hp_cmaps && vi->visual->c_class == TrueColor && vi->depth == 8 ) 
	{
	  if ( XGetRGBColormaps(dpy, RootWindow(dpy,vi->screen), &c, &n, hp_cmaps) ) 
	    {
	      i = 0;
	      while ( i < n && x->cmap == 0 ) 
		{
		  if ( c[i].visualid == vi->visual->visualid ) 
		    {
		      x->cmap = c[i].colormap;
		      x->scmap = c[i];
		      // Using HP_RGB scmap
		    }
		  i++;
		}
	      XFree( (char *)c );
	    }
	}
    }
  
#if !defined(_OS_SOLARIS_)
  if ( !x->cmap ) 
    {
      if ( XmuLookupStandardColormap(dpy,vi->screen,vi->visualid,vi->depth,
				     XA_RGB_DEFAULT_MAP,false,true) ) 
	{
	  if ( XGetRGBColormaps(dpy,RootWindow(dpy,vi->screen),&c,&n,
				XA_RGB_DEFAULT_MAP) ) 
	    {
	      i = 0;
	      while ( i < n && x->cmap == 0 ) 
		{
		  if ( c[i].visualid == vi->visualid ) 
		    {
		      x->cmap = c[i].colormap;
		      x->scmap = c[i];
		      // Using RGB_DEFAULT scmap
		    }
		  i++;
		}
	      XFree( (char *)c );
	    }
	}
    }
#endif
  if ( !x->cmap ) 
    {				
      // no shared cmap found
      x->cmap = XCreateColormap( dpy, RootWindow(dpy,vi->screen), vi->visual,
				 AllocNone );
      x->alloc = true;
      // Allocating cmap
    }
  
  cmap_dict->insert( (long)vi->visualid, x ); // associate cmap with visualid
  return x->cmap;
}	
#endif

/* statics */
int		OCCViewer_ViewPort::nCounter    = 0;
QCursor*	OCCViewer_ViewPort::defCursor   = NULL;
QCursor*	OCCViewer_ViewPort::handCursor  = NULL;
QCursor*	OCCViewer_ViewPort::panCursor   = NULL;
QCursor*	OCCViewer_ViewPort::zoomCursor  = NULL;
QCursor*	OCCViewer_ViewPort::rotCursor   = NULL;
QCursor*	OCCViewer_ViewPort::glPanCursor = NULL;

/*!
    Creates the necessary viewport cursors [ static ]
*/
void OCCViewer_ViewPort::createCursors ()
{
  defCursor    = new QCursor ( ArrowCursor );
  handCursor   = new QCursor ( PointingHandCursor );
  panCursor    = new QCursor ( SizeAllCursor  );
  zoomCursor   = new QCursor (QPixmap(imageZoomCursor));
  rotCursor    = new QCursor (QPixmap(imageRotateCursor));
  glPanCursor  = new QCursor (CrossCursor);
//  QAD_ResourceMgr* rmgr = QAD_Desktop::getResourceManager();
//  zoomCursor = new QCursor ( rmgr->loadPixmap( "QAD", tr("ICON_CURSOR_ZOOM") ));
//  rotCursor = new QCursor ( rmgr->loadPixmap( "QAD", tr("ICON_CURSOR_ROTATE") ));
}

/*!
    Destroys the viewport cursors [ static ]
*/
void OCCViewer_ViewPort::destroyCursors ()
{
  if ( defCursor ) delete defCursor;
  defCursor = 0;
  if ( handCursor ) delete handCursor;
  handCursor = 0;
  if ( panCursor ) delete panCursor;
  panCursor = 0;
  if ( zoomCursor ) delete zoomCursor;
  zoomCursor = 0;
  if ( rotCursor ) delete rotCursor;
  rotCursor = 0;
  if ( glPanCursor ) delete glPanCursor;
  glPanCursor = 0;
}

/*!
    Sets new default cursor [ static ]
*/
void OCCViewer_ViewPort::setDefaultCursor(const QCursor& newCursor) 
{
  if ( !defCursor ) defCursor = new QCursor; 
  *defCursor = newCursor; 
}

/*!
    Sets new cursor for drawing rectangle in the viewport [ static ]
*/
void OCCViewer_ViewPort::setHandCursor(const QCursor& newCursor) 
{ 
  if ( !handCursor ) handCursor = new QCursor;
  *handCursor = newCursor; 
}

/*!
    Sets new cursor for panning [ static ]
*/
void OCCViewer_ViewPort::setPanCursor(const QCursor& newCursor) 
{ 
  if ( !panCursor ) panCursor = new QCursor;
  *panCursor = newCursor; 
}

/*!
    Sets new cursor for zooming [ static ]
*/
void OCCViewer_ViewPort::setZoomCursor(const QCursor& newCursor) 
{ 
  if ( !zoomCursor ) zoomCursor = new QCursor;
  *zoomCursor = newCursor; 
}

/*!
    Sets new cursor for rotating [ static ]
*/
void OCCViewer_ViewPort::setRotCursor(const QCursor& newCursor) 
{ 
  if ( !rotCursor ) rotCursor = new QCursor;
  *rotCursor = newCursor; 
}

/*!
    Sets new cursor for global panning [ static ]
*/
void OCCViewer_ViewPort::setGlPanCursor(const QCursor& newCursor) 
{ 
  if ( !glPanCursor ) glPanCursor = new QCursor;
  *glPanCursor = newCursor; 
}

/*!
    Constructor
*/
OCCViewer_ViewPort::OCCViewer_ViewPort(QWidget* parent) : 
  QWidget( parent, 0, WRepaintNoErase | WResizeNoErase)
{
  initialize();
}

/*!
    Destructor
*/
OCCViewer_ViewPort::~OCCViewer_ViewPort()
{
  cleanup();
}

/*!
    Initializes viewport
*/
void OCCViewer_ViewPort::initialize()
{
  /* initialize cursors */
  if ( nCounter++ == 0 ) createCursors();

#if !defined WNT

  XVisualInfo* pVisualInfo;
  if ( x11Display() ) 
    {
      /* Initialization with the default VisualID
       */
      //NRI Visual *v = DefaultVisual(x11Display(), DefaultScreen(x11Display()));
      //NRI int visualID = XVisualIDFromVisual(v);

	  /*  Here we use the settings from 
	      Optimizer_ViewInfo::TxglCreateWindow()
	  */
      int visualAttr[] = { GLX_RGBA, GLX_DEPTH_SIZE, 1, 
			   GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1,
			   GLX_BLUE_SIZE, 1, GLX_DOUBLEBUFFER,
			   None };

      pVisualInfo = ::glXChooseVisual (x11Display(), DefaultScreen(x11Display()), visualAttr);
	  
      if ( isVisible() ) hide();
	  
      XSetWindowAttributes a;
	  
      a.colormap = choose_cmap( x11Display(), pVisualInfo );	    /* find best colormap */
      a.background_pixel = backgroundColor().pixel();
      a.border_pixel = black.pixel();
      Window p = RootWindow( x11Display(), DefaultScreen(x11Display()) ); 
      if ( parentWidget() ) p = parentWidget()->winId();
	  
      Window w = XCreateWindow( x11Display(), p,  x(), y(), width(), height(),
				0, pVisualInfo->depth, InputOutput,  pVisualInfo->visual,
				CWBackPixel | CWBorderPixel | CWColormap, &a );
      Window *cmw;
      Window *cmwret;
      int count;
      if ( XGetWMColormapWindows( x11Display(), topLevelWidget()->winId(), &cmwret, &count ) )
	{
	  cmw = new Window[count+1];
	  memcpy( (char *)cmw, (char *)cmwret, sizeof(Window)*count );
	  XFree( (char *)cmwret );
	  int i;
	      
	  for (i = 0; i < count; i++) 
	    {
	      if ( cmw[i] == winId() ) /* replace old window */
		{		
		  cmw[i] = w;
		  break;
		}
	    }
	      
	  if ( i >= count )			 /* append new window */
	    cmw[count++] = w;
	} 
      else 
	{
	  count = 1;
	  cmw = new Window[count];
	  cmw[0] = w;
	}
	  
      /* Creating new window (with good VisualID) for this widget
	   */
      create(w);
      XSetWMColormapWindows( x11Display(), topLevelWidget()->winId(), cmw, count );
      delete [] cmw;
	  
      if ( isVisible() ) show();
	  
      if ( pVisualInfo ) {
	XFree( (char *)pVisualInfo );
      }
	  
      XFlush(x11Display());
      //		XSync(x11Display(), false);
      //		XSynchronize(x11Display(), true);
    } 
#endif	// !defined WNT
  
  myOriginalViewport = NULL;	
  myCursorIsHand = false;
  myCursor = *defCursor;
  myHasWindow = false;
  myDrawRect = false;
  myStartX = myStartY = myCurrX = myCurrY =0;
  myPaintersRedrawing = false;
  myEnableDrawMode = true;
  
  setTransformRequested ( NOTHING );
  setTransformInProcess ( false );
  
  setMouseTracking( true );
  setBackgroundMode(NoBackground);

  setFocus();
}

/*!
    Cleans up the viewport
*/
void OCCViewer_ViewPort::cleanup()
{
  if ( --nCounter == 0 ) 
    destroyCursors();
}

/*!
    Sets the original view for the viewport   
*/
void OCCViewer_ViewPort::setOriginalView( OCCViewer_ViewPort* viewport, 
					  const QRect& magnify )
{
  myOriginalViewport = viewport;
  myMagnifyRect = magnify;
}

/*!
    Returns the original view or null
*/
OCCViewer_ViewPort* OCCViewer_ViewPort::getOriginalView() const
{
  return myOriginalViewport;
}

/*!
    Returns the 'magnify' rect ( used for 'magnify' operation )
*/
const QRect& OCCViewer_ViewPort::getMagnifyRect() const
{
  return myMagnifyRect;
}

/*!
    Returns the sketched rect ( used for multiple selection )
*/
const QRect& OCCViewer_ViewPort::getSelectionRect() const
{
  return myRect;
}

/*!
    Returns 'true' if the viewport has a native window
*/
bool OCCViewer_ViewPort::hasWindow() const
{
  return myHasWindow;
}

/*!
    Must be called if native window was changed 
*/
void OCCViewer_ViewPort::windowChanged() 
{
  myHasWindow = false;
}

/*!
    Sets the default cursor active    
*/
void OCCViewer_ViewPort::setDefaultCursorOn()
{
  setCursor ( *OCCViewer_ViewPort::defCursor );
}

/*!
    Sets the 'hand' cursor active    
*/
void OCCViewer_ViewPort::setHandCursorOn()
{
  setCursor ( *OCCViewer_ViewPort::handCursor );
}

/*!
    Sets the panning cursor active    
*/
void OCCViewer_ViewPort::setPanCursorOn()
{
  setCursor( *OCCViewer_ViewPort::panCursor );
}

/*!
    Sets the zooming cursor active    
*/
void OCCViewer_ViewPort::setZoomCursorOn()
{
  setCursor( *OCCViewer_ViewPort::zoomCursor );
}

/*!
    Sets the rotating cursor active    
*/
void OCCViewer_ViewPort::setRotCursorOn()
{
  setCursor( *OCCViewer_ViewPort::rotCursor );
}

/*!
    Sets the global panning cursor active    
*/
void OCCViewer_ViewPort::setGlPanCursorOn()
{
  setCursor( *OCCViewer_ViewPort::glPanCursor );
}

/*!
    Returns the default background color
*/
QColor OCCViewer_ViewPort::backgroundColor() const
{
  return Qt::white;
}

/*!
    Activates 'zoom' transformation
*/
void OCCViewer_ViewPort::activateZoom()
{
  if ( !transformRequested() && !myCursorIsHand )
    myCursor = cursor();	        /* save old cursor */
  
  if ( myOperation != ZOOMVIEW ) {
    setTransformRequested ( ZOOMVIEW );		
    setCursor( *zoomCursor );
  }
}

/*!
    Activates 'panning' transformation
*/
void OCCViewer_ViewPort::activatePanning()
{
  if ( !transformRequested() && !myCursorIsHand )
    myCursor = cursor();	        /* save old cursor */
  
  if ( myOperation != PANVIEW ) {
    setTransformRequested ( PANVIEW );
    setCursor( *panCursor );
  }
}

/*!
    Activates 'rotation' transformation
*/
void OCCViewer_ViewPort::activateRotation()
{
  if ( !transformRequested() && !myCursorIsHand )
    myCursor = cursor();	        /* save old cursor */
  
  if ( myOperation != ROTATE ) {
    setTransformRequested ( ROTATE );
    setCursor( *rotCursor );	
  }
}

/*!
    Activates 'fit' transformation
*/
void OCCViewer_ViewPort::activateWindowFit()
{
  if ( !transformRequested() && !myCursorIsHand )
    myCursor = cursor();	        /* save old cursor */

  if ( myOperation != WINDOWFIT ) {
    setTransformRequested ( WINDOWFIT );		
    setCursor ( *handCursor );
    myCursorIsHand = true;
  }
//  setTransformInProcess( true );		
//  emit vpTransformationStarted ( WINDOWFIT );
}

/*!
    Activates 'global panning' transformation
*/
void OCCViewer_ViewPort::activateGlobalPanning()
{
  if ( !transformRequested() && !myCursorIsHand )
    myCursor = cursor();	        /* save old cursor */
  
  if ( myOperation != PANGLOBAL ) 
    {
      fitAll(); /* fits view before selecting a new scene center */
      setTransformRequested ( PANGLOBAL );
      setCursor( *glPanCursor );
    }
//  setTransformInProcess( true );		
//  emit vpTransformationStarted ( PANGLOBAL );
}

/*!
    Sets the viewport to its initial state
    ( no transformations in process etc. )
*/
void OCCViewer_ViewPort::resetState()
{
  myDrawRect = false;
  
  /* make rectangle empty (left > right) */
  myRect.setLeft(2);
  myRect.setRight(0);
  
  if ( transformRequested() || myCursorIsHand ) 
    setCursor( myCursor );
  myCursorIsHand = false;
  
  if ( transformRequested() ) 
    emit vpTransformationFinished (myOperation);
  
  setTransformInProcess( false );		
  setTransformRequested ( NOTHING );	
  QAD_Application::getDesktop()->putInfo( tr("INF_READY") );
}

/*!
    Enable/disable user's ability to sketch a rect in the viewport
*/
void OCCViewer_ViewPort::enableDrawMode(bool bEnable )
{
  myEnableDrawMode = bEnable;
}

/*!
    Returns 'true' if user can sketch a rect in the viewport
*/
bool OCCViewer_ViewPort::enableDrawMode() const
{
  return myEnableDrawMode;
}

/*!
    Sets the active operation 'op'
*/
void OCCViewer_ViewPort::setTransformRequested ( OperationType op )
{    
  myOperation = op;
  setMouseTracking ( myOperation == NOTHING );  
}

/*!
    Mouse event handler
*/
void OCCViewer_ViewPort::mousePressEvent (QMouseEvent* event)
{
  myStartX = event->x();
  myStartY = event->y();
  switch ( myOperation )
    {
    case WINDOWFIT:
      if ( event->button() == Qt::LeftButton )
        emit vpTransformationStarted ( WINDOWFIT );
      break;    
//      return;
    case PANGLOBAL:
      if ( event->button() == Qt::LeftButton )
        emit vpTransformationStarted ( PANGLOBAL );
      break;    
//      return;
      
    case ZOOMVIEW:
      if ( event->button() == Qt::LeftButton )
	emit vpTransformationStarted ( ZOOMVIEW );
      break;
      
    case PANVIEW:
      if ( event->button() == Qt::LeftButton )
	emit vpTransformationStarted ( PANVIEW );
      break;

    case ROTATE:
      if ( event->button() == Qt::LeftButton )
	{
	  startRotation(myStartX, myStartY);
	  emit vpTransformationStarted ( ROTATE );
	}
      break;
      
    default:
      
      /*	Try to activate a transformation
       */
      if ( (event->state() == Qt::ControlButton) &&
	   (event->button() == Qt::LeftButton) )
	{
				/* MB1 + CTRL = Zooming */
	  activateZoom();
	}
      else if ( (event->state() == Qt::ControlButton) &&
		(event->button() == Qt::MidButton) )
	{
				/* MB2 + CTRL = Panning */
	  activatePanning();
	}
      else if ( (event->state() == Qt::ControlButton) &&
		(event->button() == Qt::RightButton) )
	{
				/* MB3 + CTRL = Rotation */
	  activateRotation();
	  startRotation(myStartX, myStartY);
	}
      
      /* notify that we start a transformation */
      if ( transformRequested() ) 
	emit vpTransformationStarted ( myOperation );
    }
  
  if ( transformRequested() ) 
    setTransformInProcess( true );		
  
  /*  NOTE: this signal must be emitted at the end 
      because we must to detect a transformation first
  */
  emit vpMousePress (event);
}

/*!
    Mouse event handler 
*/
void OCCViewer_ViewPort::mouseMoveEvent (QMouseEvent* event)
{
  myCurrX = event->x();
  myCurrY = event->y();
  switch (myOperation)
    {
    case ROTATE:
      rotate(myCurrX, myCurrY);
      break;
      
    case ZOOMVIEW:
      zoom(myStartX, myStartY, myCurrX, myCurrY);
      myStartX = myCurrX;
      myStartY = myCurrY;
      break;
      
    case PANVIEW:
      pan(myCurrX - myStartX, myStartY - myCurrY);
      myStartX = myCurrX;
      myStartY = myCurrY;
      break;
      
/*    case WINDOWFIT:
      myDrawRect = true;
      repaint();
      break;
*/      
    case PANGLOBAL:
      break;
      
    default:
      if ( event->state() == Qt::LeftButton ||
	   event->state() == ( Qt::LeftButton | Qt::ShiftButton) )
	{
	  myDrawRect = myEnableDrawMode;
	  if ( myDrawRect ) 
	    {
	      repaint( visibleRect(), false);
	      if ( !myCursorIsHand )
		{   /* we are going to sketch a rectangle */					    
		  myCursorIsHand = true;		
		  myCursor = cursor();
		  setCursor ( *handCursor );
		}
	    }
	}
    }
  emit vpMouseMove( event );			
}

/*!
    Mouse event handler 
*/
void OCCViewer_ViewPort::mouseReleaseEvent (QMouseEvent* event)
{
  if ( !transformRequested() && (event->button() == Qt::RightButton) )
    {
      QPopupMenu* popup = createPopup();
      if ( popup ) {
	QAD_Tools::checkPopup( popup );
	if ( popup->count()>0 ) {
	  popup->exec( QCursor::pos() );
	}
	destroyPopup();
      }
      return;
    }
  
  switch ( myOperation )
    {
    case NOTHING:
      break;
    case ROTATE:
      endRotation();
      resetState();
      break;
      
    case PANVIEW:
    case ZOOMVIEW:
      resetState();
      break;
      
    case PANGLOBAL:
      if ( event->button() == Qt::LeftButton )
	{
	  setCenter( event->x(), event->y() );
	  resetState();
	}
      break;
      
    case WINDOWFIT:
      if ( event->state() == Qt::LeftButton )
	{
	  myCurrX = event->x();
	  myCurrY = event->y();
	  QRect rect = QAD_Tools::makeRect(myStartX, myStartY, myCurrX, myCurrY);
	  if ( !rect.isEmpty() ) fitWindow(rect);
	  resetState();
	}
      break;
    }
  
  // NOTE: viewer 3D detects a rectangle of selection using this event
  // so we must emit it BEFORE resetting the selection rectangle
  emit vpMouseRelease  (event);		
  
  if ( event->button() == Qt::LeftButton && myDrawRect )
    {
      myDrawRect = false;
      repaint(visibleRect(), false);
      resetState(); 
    }
}

/*!
    Mouse event handler 
*/
void OCCViewer_ViewPort::mouseDoubleClickEvent(QMouseEvent *event)
{
  emit vpMouseDoubleClick (event);
}

/*!
    Key event handler 
*/
void OCCViewer_ViewPort::keyPressEvent(QKeyEvent *event)
{
  emit vpKeyPress (event);
}

/*!
    Key event handler 
*/
void OCCViewer_ViewPort::keyReleaseEvent(QKeyEvent *event)
{
  emit vpKeyRelease (event);
}

/*!
    Called when the viewport gets the focus
*/
void OCCViewer_ViewPort::focusInEvent(QFocusEvent *event)
{
  emit vpFocusIn (event);
}

/*!
    Called when the viewport loses the focus
*/
void OCCViewer_ViewPort::focusOutEvent(QFocusEvent *event)
{
  emit vpFocusOut (event);
}
	
/*!
    Resizes the viewport
*/
void OCCViewer_ViewPort::resizeEvent (QResizeEvent* event)
{
  windowResize();
}

/*!
    Updates the viewport
*/
void OCCViewer_ViewPort::update(int x, int y, int w, int h)
{
  if ( !myHasWindow )
    myHasWindow = setWindow();
  if ( myHasWindow) 
    repaint(x, y, w, h, true);
}

/*!
    Repaints the viewport    
*/
void OCCViewer_ViewPort::paintEvent (QPaintEvent *ev)
{
  emit vpPaint (ev);
  if ( myDrawRect )	
    {
      QPainter thePainter(this);
      thePainter.setRasterOp(Qt::XorROP);
      thePainter.setPen(Qt::white);
      QRect aRect = QAD_Tools::makeRect(myStartX, myStartY, myCurrX, myCurrY);
      if ( !myRect.isEmpty() )
	thePainter.drawRect( myRect );
      thePainter.drawRect(aRect);
      myRect = aRect;
    }

  if ( myPaintersRedrawing )
    {
      QPainter thePainter(this);
      emit vpDrawExternal  (&thePainter);
      myPaintersRedrawing = false;
    }
}

/*!
  Forces to redraw the viewport by an external painter     
*/
void OCCViewer_ViewPort::redrawPainters()
{
  myPaintersRedrawing = true;
  repaint();
}
