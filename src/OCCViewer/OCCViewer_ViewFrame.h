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

/*
  Class       : OCCViewer_ViewFrame
  Description : View frame for Open CASCADE view
*/

class QAD_EXPORT OCCViewer_ViewFrame : public QAD_ViewFrame
{
  Q_OBJECT

public:

                              OCCViewer_ViewFrame( QWidget* parent, const QString& title );
                              OCCViewer_ViewFrame( QWidget* parent = 0 );
  virtual                     ~OCCViewer_ViewFrame();

  /*
    Reimplemented from QAD_ViewFrame
  */

  virtual ViewType            getTypeView() const { return VIEW_OCC; }
  virtual QWidget*            getViewWidget();
  virtual void                setBackgroundColor( const QColor& );
  virtual QColor              backgroundColor() const;
  virtual void                SetSelectionMode( Selection_Mode mode ) {};

  /*  popup management */
  
  virtual void                setPopupServer( QAD_Application* );

  /*  interactive object management */

  virtual void                highlight( const Handle(SALOME_InteractiveObject)&,
                                         const bool highlight,
                                         const bool update = true );
  virtual void                unHighlightAll();
  virtual void                rename( const Handle(SALOME_InteractiveObject)&,
                                      QString );
  virtual bool                isInViewer( const Handle(SALOME_InteractiveObject)& );
  virtual bool                isVisible( const Handle(SALOME_InteractiveObject)& );

  /*  undo/redo management */
  
  void                        undo( QAD_Study* aStudy, const char* StudyFrameEntry );
  void                        redo( QAD_Study* aStudy, const char* StudyFrameEntry );

  /* selection */
  
  virtual Handle(SALOME_InteractiveObject) FindIObject( const char* Entry );

  /* display */
  virtual void                Display( const Handle(SALOME_InteractiveObject)&,
                                       bool update = true);
  virtual void                DisplayOnly( const Handle(SALOME_InteractiveObject)& );
  virtual void                Erase( const Handle(SALOME_InteractiveObject)&,
                                     bool update = true);
  virtual void                DisplayAll();
  virtual void                EraseAll();
  virtual void                Repaint();

  void                        AdjustTrihedrons( const bool forced  = false );

  /* Reimplemented from SALOME_View */
  
  virtual void                Display( const SALOME_OCCPrs* );
  virtual void                Erase( const SALOME_OCCPrs*, const bool = false );
  virtual SALOME_Prs*         CreatePrs( const char* entry = 0 );
  virtual void                BeforeDisplay( SALOME_Displayer* d );
  virtual void                AfterDisplay ( SALOME_Displayer* d );
  virtual void                LocalSelection( const SALOME_OCCPrs*, const int );
  virtual void                GlobalSelection( const bool = false ) const;

  /* Own public methods */
  
  void                        setViewPort( OCCViewer_ViewPort* );
  OCCViewer_ViewPort*         getViewPort() const;

  void                        setViewer( OCCViewer_Viewer3d* );
  OCCViewer_Viewer3d*         getViewer() const;

  void                        setVisible( const bool isVisible = true );

  void                        setCursor( const QCursor& );
  QCursor                     cursor() const;

signals:

  void                        vfTransformationStarted ( OCCViewer_ViewPort::OperationType );
  void                        vfTransformationFinished( OCCViewer_ViewPort::OperationType );
  void                        vfDrawExternal( QPainter* painter );
  void                        vfMousePress( QMouseEvent* );
  void                        vfMouseRelease( QMouseEvent* );
  void                        vfMouseMove( QMouseEvent* );
  void                        vfMouseDoubleClick( QMouseEvent* );
  void                        vfKeyPress( QKeyEvent* );
  void                        vfKeyRelease( QKeyEvent* );
  void                        vfResize( QResizeEvent* );
  void                        vfPrint( QPaintEvent* );
  void                        vfViewClosing( QCloseEvent* );
  void                        vfMoved ( QMoveEvent* );

public slots:

  void                        onViewPan();
  void                        onViewZoom();
  void                        onViewFitAll();
  void                        onViewFitArea();
  void                        onViewGlobalPan();
  void                        onViewRotate();
  void                        onViewReset();
  void                        onViewFront();
  void                        onViewBack();
  void                        onViewRight();
  void                        onViewLeft();
  void                        onViewBottom();
  void                        onViewTop();
  void                        onViewTrihedron();
  void                        onAdjustTrihedron();

protected:

  void                        initViewPort();
  void                        initialize();
  void                        cleanup();
  void                        closeEvent( QCloseEvent* );
  void                        resizeEvent( QResizeEvent* );
  void                        keyPressEvent( QKeyEvent * );
private:

  bool                        getTrihedronSize( double& theNewSize, double& theSize );
  void                        redisplayAll( QAD_Study*, const bool = true );
 
private:

  OCCViewer_Viewer3d*         myViewer;	 // my owner
  OCCViewer_ViewPort*         myViewPort; // child viewport
};

#endif

