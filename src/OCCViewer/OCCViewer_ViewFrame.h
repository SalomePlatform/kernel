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
//  File   : OCCViewer_ViewFrame.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef OCCViewer_ViewFrame_H
#define OCCViewer_ViewFrame_H

#include "OCCViewer_ViewPort.h"
#include "QAD_ViewFrame.h"
#include "SALOME_Selection.h"

class OCCViewer_Viewer3d;
class QAD_EXPORT OCCViewer_ViewFrame : public QAD_ViewFrame
{
  Q_OBJECT

 public:
  OCCViewer_ViewFrame(QWidget* parent, const QString& title);
  OCCViewer_ViewFrame(QWidget* parent = 0);
  virtual ~OCCViewer_ViewFrame();

 protected:
  virtual void	    initViewPort();
  void              initialize();
  void              cleanup();

 public:
  ViewType          getTypeView() const{ return VIEW_OCC;};
  QWidget*          getViewWidget();
  void              setViewPort(OCCViewer_ViewPort* view);
  OCCViewer_ViewPort*	    getViewPort() const;

  void	            setViewer(OCCViewer_Viewer3d* viewer);
  OCCViewer_Viewer3d*     getViewer() const;

  void              setVisible( bool isVisible = true );

  void              setBackgroundColor( const QColor& );
  QColor            backgroundColor() const;

  void              setCursor( const QCursor& );
  QCursor           cursor() const;

  void              SetSelectionMode( Selection_Mode mode ) {};
  
  /*  popup management */
  void              setPopupServer( QAD_Application* );


  /*  interactive object management */
  void              highlight( const Handle(SALOME_InteractiveObject)& IObject, 
			       bool highlight, bool update = true );
  void              unHighlightAll();
  void              rename( const Handle(SALOME_InteractiveObject)& IObject,
			    QString newName );
  bool              isInViewer( const Handle(SALOME_InteractiveObject)& IObject );
  bool              isVisible( const Handle(SALOME_InteractiveObject)& IObject );

  /*  undo/redo management */
  void              undo(SALOMEDS::Study_var aStudy,
			 const char* StudyFrameEntry);
  void              redo(SALOMEDS::Study_var aStudy,
			 const char* StudyFrameEntry);

  /* selection */
  Handle(SALOME_InteractiveObject) FindIObject(const char* Entry);

  /* display */		
  void           Display(const Handle(SALOME_InteractiveObject)& IObject, bool update = true);
  void           DisplayOnly(const Handle(SALOME_InteractiveObject)& IObject);
  void           Erase(const Handle(SALOME_InteractiveObject)& IObject, bool update = true);
  void           DisplayAll();
  void           EraseAll();
  void           Repaint();

 protected:	
  void              closeEvent(QCloseEvent* e);
  void              resizeEvent(QResizeEvent* e);
  void              keyPressEvent( QKeyEvent *k );

 signals:
  void              vfTransformationStarted ( OCCViewer_ViewPort::OperationType );
  void              vfTransformationFinished( OCCViewer_ViewPort::OperationType );
  void              vfDrawExternal(QPainter* painter);
  void              vfMousePress(QMouseEvent*);
  void              vfMouseRelease(QMouseEvent*);
  void              vfMouseMove(QMouseEvent*);
  void              vfMouseDoubleClick(QMouseEvent*);
  void              vfKeyPress(QKeyEvent*);
  void              vfKeyRelease(QKeyEvent*);
  void              vfResize(QResizeEvent*);
  void              vfPrint(QPaintEvent*);
  void              vfViewClosing(QCloseEvent*);
  void              vfMoved (QMoveEvent*);

  public slots:

  void           onViewPan(); 
  void           onViewZoom();
  void           onViewFitAll();
  void           onViewFitArea();
  void           onViewGlobalPan(); 
  void           onViewRotate();
  void           onViewReset();     
  void           onViewFront(); 
  void           onViewBack(); 
  void           onViewRight(); 
  void           onViewLeft();     
  void           onViewBottom();
  void           onViewTop();
  void           onViewTrihedron(); 
  void           onAdjustTrihedron();
 
 protected:
  OCCViewer_Viewer3d*      myViewer;	 // my owner
  OCCViewer_ViewPort*      myViewPort; // child viewport
};

#endif

