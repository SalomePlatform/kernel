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
//  File   : OCCViewer_ViewPort.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef OCCViewer_ViewPort_H
#define OCCViewer_ViewPort_H

#include "QAD.h"
#include "QAD_Popup.h"

// QT Includes
#include <qrect.h>
#include <qcolor.h>
#include <qwidget.h>
#include <qcursor.h>
#include <qpainter.h>

class QAD_EXPORT OCCViewer_ViewPort: public QWidget, 
  public QAD_PopupClientServer
{
  Q_OBJECT

  /* Objects counter */
  static int		nCounter;				
	
  /* Cursors */
  static QCursor*	defCursor;
  static QCursor*	handCursor;
  static QCursor*	panCursor;
  static QCursor*	zoomCursor;
  static QCursor*	rotCursor;
  static QCursor*       glPanCursor;

  void	initialize();
  void	cleanup();

public:
  enum OperationType	{
    NOTHING,
    PANVIEW,
    ZOOMVIEW,
    ROTATE,
    PANGLOBAL,
    WINDOWFIT
  };

protected:
  static void createCursors();
  static void destroyCursors();
  
  static const QCursor* getDefaultCursor() { return defCursor; }
  static void setDefaultCursor(const QCursor& newCursor);
  static const QCursor* getHandCursor() { return handCursor; }
  static void setHandCursor(const QCursor& newCursor);
  static const QCursor* getPanCursor() { return panCursor; }
  static void setPanCursor(const QCursor& newCursor);
  static const QCursor* getZoomCursor() { return zoomCursor; }
  static void setZoomCursor(const QCursor& newCursor);
  static const QCursor* getRotCursor() { return rotCursor; }
  static void setRotCursor(const QCursor& newCursor);
  static const QCursor* getGlPanCursor() { return rotCursor; }
  static void setGlPanCursor(const QCursor& newCursor);
  
  /* Transformation selected but not started yet */
  bool		transformRequested() const { return ( myOperation != NOTHING ); }
  void		setTransformRequested ( OperationType op );

  /* Transformation is selected and already started */
  bool		transformInProcess() const { return myEventStarted; }
  void		setTransformInProcess( bool bOn ) { myEventStarted = bOn; }

public:
  OCCViewer_ViewPort( QWidget* parent );
  ~OCCViewer_ViewPort();
  
  /* Draw mode management */
  void	        enableDrawMode( bool bEnable );
  bool	        enableDrawMode() const;

  /* Cursors management */
  void	        setDefaultCursorOn();
  void	        setHandCursorOn();
  void	        setPanCursorOn();
  void	        setZoomCursorOn();
  void	        setRotCursorOn();
  void	        setGlPanCursorOn();

  void	        resetState();
  bool	        hasWindow() const;
  void	        windowChanged();
  void	        redrawPainters();
  void	        update(int x, int y, int w, int h);
  
  void	              setOriginalView( OCCViewer_ViewPort* view, const QRect& magnify );
  OCCViewer_ViewPort* getOriginalView() const;
  const QRect&        getMagnifyRect() const;
  const QRect&        getSelectionRect() const;

  /* View transformations */
  virtual void    activateZoom();	
  virtual void    activatePanning();
  virtual void    activateRotation();
  virtual void    activateWindowFit();
  virtual void    activateGlobalPanning();	
  
  virtual void	  fitAll( bool withZ = true ) = 0;
  virtual void	  reset() = 0;

  virtual void    incrementalPan   ( const int incrX, const int incrY ) = 0;
  virtual void    incrementalZoom  ( const int incr ) = 0;
  virtual void    incrementalRotate( const int incrX, const int incrY ) = 0;

  /* background color */
  virtual QColor  backgroundColor() const;
  virtual void    setBackgroundColor( const QColor& color) = 0;

protected: 
  virtual void    paintEvent(QPaintEvent *);
  virtual void	  resizeEvent(QResizeEvent *);
  
  virtual void    mouseMoveEvent(QMouseEvent *);
  virtual void    mouseReleaseEvent(QMouseEvent *);
  virtual void    mousePressEvent(QMouseEvent *);
  virtual void    mouseDoubleClickEvent(QMouseEvent *); 

  virtual void    keyPressEvent(QKeyEvent *);
  virtual void    keyReleaseEvent(QKeyEvent *);

  virtual void    focusInEvent(QFocusEvent *event);
  virtual void    focusOutEvent(QFocusEvent *event);

signals:
  void		  vpTransformationStarted (OCCViewer_ViewPort::OperationType type);
  void		  vpTransformationFinished (OCCViewer_ViewPort::OperationType type);
  
  void		  vpMousePress (QMouseEvent*);
  void		  vpMouseRelease (QMouseEvent*);
  void		  vpMouseMove (QMouseEvent*);
  void		  vpMouseDoubleClick (QMouseEvent*);
  
  void		  vpKeyPress (QKeyEvent*);
  void		  vpKeyRelease (QKeyEvent*);
  
  void		  vpFocusOut (QFocusEvent*);
  void		  vpFocusIn (QFocusEvent*);
  
  void		  vpPaint (QPaintEvent*);
  void		  vpDrawExternal (QPainter* painter);

  void            vpDumpView();

protected slots:
 virtual void	onChangeBackgroundColor() = 0;

protected:    
 virtual bool	setWindow() = 0;
 virtual void	windowResize() = 0;	
 virtual void	pan(int dx, int dy) = 0;	
 virtual void	setCenter(int x, int y) = 0;
 virtual void	fitWindow( const QRect& rect) = 0;
 virtual void	zoom(int x0, int y0, int x, int y) = 0;	
 virtual void	startRotation(int x0, int y0) = 0;
 virtual void	rotate(int x, int y) = 0;
 virtual void	endRotation() = 0;
        
protected:
 OCCViewer_ViewPort*	myOriginalViewport;
 OperationType	        myOperation;
 QCursor	        myCursor;
 
 QRect		        myMagnifyRect;
 QRect		        myRect;				
 bool		        myDrawRect;  /* set when a rect is used for selection or magnify */
	
 /* Mouse coordinates */
 int		        myStartX;	
 int		        myStartY;
 int		        myCurrX;
 int		        myCurrY;

 bool	                myHasWindow;
 bool		        myCursorIsHand;			
 bool		        myEnableDrawMode;
 bool		        myEventStarted;       /* set when transformation is in process */
 bool		        myPaintersRedrawing;  /* set to draw with external painters */
};

#endif

