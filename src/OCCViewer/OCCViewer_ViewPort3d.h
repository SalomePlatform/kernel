//  File      : OCCViewer_ViewPort3d.h
//  Created   : Wed Mar 20 10:48:51 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : OCCViewer
//  Copyright : Open CASCADE 2002
//  $Header$

#ifndef OCCViewer_ViewPort3d_H
#define OCCViewer_ViewPort3d_H

#include "OCCViewer_ViewPort.h"
#include "OCCViewer_VService.h"

#include "QAD.h"

// Open CASCADE Includes
#include <Standard.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>

// QT Includes
#include <qwidget.h>
#include <qrect.h>
#include <qcolor.h>
#include <qpainter.h>
#include <qapplication.h>

class QAD_EXPORT OCCViewer_ViewPort3d: public OCCViewer_ViewPort
{
  Q_OBJECT
	
 public:	
  OCCViewer_ViewPort3d( QWidget* parent, const Handle( V3d_Viewer)& viewer,
			V3d_TypeOfView  viewType = V3d_ORTHOGRAPHIC, 
			OCCViewer_ViewPort* prevView = 0, 
			const QRect* magnify = 0 );
  ~OCCViewer_ViewPort3d();
  
  /* view 3D management */
  Handle (V3d_View)	  setView( const Handle( V3d_View )& );
  Handle (V3d_View)       getView() const;
  Handle (V3d_Viewer)	  getViewer() const;
  V3d_TypeOfView          setViewType( V3d_TypeOfView type );

  /* popup management */
  void 	                  onCreatePopup();
  
  /* view transformations */
  void                    activateGlobalPanning();
  void	                  fitAll( bool withZ = true );
  void	                  reset();

  /* background */
  void		 	  setBackgroundColor( const QColor& color);	
  QColor		  backgroundColor() const;

  protected slots:
    void		  onChangeBackgroundColor();

 protected:	
  void			  paintEvent(QPaintEvent *ev);    

 private:
  bool	                setWindow(); 
  void	                windowResize();
  void	                pan(int dx, int dy);
  void	                setCenter(int x, int y);
  void	                fitWindow( const QRect& rect);
  void	                zoom(int x0, int y0, int x, int y);
  void	                startRotation(int x0, int y0);
  void	                rotate(int x, int y);
  void	                endRotation();

 private:       
  Handle (V3d_View)		myActiveView;    
  bool				myDegenerated;
  double				myCurScale;
};

#endif

