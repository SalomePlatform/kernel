//  File      : OCCViewer_Viewer3d.h
//  Created   : Wed Mar 20 11:00:02 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : OCCViewer
//  Copyright : Open CASCADE 2002
//  $Header$


#ifndef OCCVIEWER_VIEWER3D_H
#define OCCVIEWER_VIEWER3D_H

#include "OCCViewer_ViewFrame.h"
#include "OCCViewer_AISSelector.h"

#include "QAD.h"
#include "QAD_Popup.h"
#include "QAD_ViewFrame.h"
#include "SALOME_InteractiveObject.hxx"

// Open CASCADE Includes
#include <AIS_InteractiveContext.hxx>
#include <AIS_Trihedron.hxx>

class QAD_EXPORT OCCViewer_Viewer3d : public QObject, public QAD_PopupClientServer
{
  Q_OBJECT

public:
  OCCViewer_Viewer3d(OCCViewer_ViewFrame* vf);
  ~OCCViewer_Viewer3d();

signals:
  void		vwKeyPress( QKeyEvent* );
  void		vwKeyRelease( QKeyEvent* );
  void		vwDrawExternal( QPainter* );

public slots:
 void		onKeyPress(QKeyEvent*);

 void		onMouseMove(QMouseEvent*);
 void		onMousePress(QMouseEvent*);
 void		onMouseRelease(QMouseEvent*);
 void		onMouseDoubleClick(QMouseEvent*);

 void		onSelectionDone( bool bAdded );
 void		onSelectionCancel( bool bAdded );

 void		onTransformationStarted( OCCViewer_ViewPort::OperationType );
 void		onTransformationFinished( OCCViewer_ViewPort::OperationType );

signals:
 void		vw3dSelectionDone (bool added);
 void		vw3dSelectionCancel();
 void		vw3dSelectionEnabled( bool );

public:
 void		    createViewer3d();
 Handle(V3d_Viewer) getViewer3d() const;

 void               setTrihedronSize(float size);
 bool               isTrihedronDisplayed();

  /* popup management */
  void		    onCreatePopup();

  void		    setCursor( const QCursor& cursor);
  const QCursor&    cursor() const;
  void		    setIcon( const QPixmap& icon);
  const QPixmap&    icon() const;
  void		    setBackgroundColor( const QColor& aColor);
  const QColor&	    backgroundColor() const;

  /* Selection management */
  bool		highlight( const Handle(SALOME_InteractiveObject)& IObject,
			   bool highlight, bool update=true); 
  bool		unHighlightAll(); 
  bool          isInViewer( const Handle(SALOME_InteractiveObject)& IObject,
			    bool onlyInViewer = false );
  bool          isVisible( const Handle(SALOME_InteractiveObject)& IObject );
  void          rename( const Handle(SALOME_InteractiveObject)& IObject,
			QString newName );

  bool		enabledSelection() const;
  void		enableSelection( bool );
  void		enableMultipleSelection( bool );

  void          SetColor(const Handle(SALOME_InteractiveObject)& IObject,
			 QColor thecolor);
  void          SwitchRepresentation(const Handle(SALOME_InteractiveObject)& IObject,
				     int mode);
  void          SetTransparency(const Handle(SALOME_InteractiveObject)& IObject,
				float transp);

  /* interactive context */
  Handle (AIS_InteractiveContext) getAISContext() const;
  OCCViewer_AISSelector*	  getAISSelector() const;

  /* trihedron */
  Handle(AIS_Trihedron)           getTrihedron() const;

protected:
 bool	     eventFilter( QObject *, QEvent * );

protected:
 OCCViewer_ViewFrame*		 myViewFrame;

 Handle (AIS_InteractiveContext) myAISContext;
 OCCViewer_AISSelector*		 myAISSelector;
 bool				 myEnableSelection;
 Handle(AIS_Trihedron)		 myTrihedron;
 Handle(V3d_Viewer)		 myV3dViewer;
 Handle(V3d_Viewer)		 myV3dCollector;

 /* attributes for all the views */
 QPixmap			 myIcon;	/* icon */
 QCursor			 myCursor;	/* current cursor */
 QColor				 myBackColor;	/* background */

};

#endif

