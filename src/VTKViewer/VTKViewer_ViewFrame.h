//  SALOME VTKViewer : build VTK viewer into Salome desktop
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
//  File   : VTKViewer_ViewFrame.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef VTKViewer_ViewFrame_H
#define VTKViewer_ViewFrame_H

#include "QAD.h"
#include "QAD_ViewFrame.h"

#include "VTKViewer_RenderWindowInteractor.h"
#include "VTKViewer_RenderWindow.h"

// VTK Includes
class vtkRenderer;
class vtkTransform;
class VTKViewer_Trihedron;

class QAD_EXPORT VTKViewer_ViewFrame : public QAD_ViewFrame{
  Q_OBJECT;
 public:
  VTKViewer_ViewFrame(QWidget* parent, const char* name=0 );
  ~VTKViewer_ViewFrame();
  
  ViewType                       getTypeView() const{ return VIEW_VTK;};
  QWidget*                       getViewWidget();
  vtkRenderer*                   getRenderer() {return m_Renderer;}

  VTKViewer_RenderWindow*	 getRW() {return m_RW;}
  VTKViewer_RenderWindowInteractor* getRWInteractor() {return m_RWInteractor;}

  bool                           isTrihedronDisplayed();
 
  void                           setBackgroundColor( const QColor& );
  QColor                         backgroundColor() const;
  
  void                           SetSelectionMode( int mode );

  /*  popup management */
  void                           setPopupServer( QAD_Application* );

  /*  interactive object management */
  void                           highlight( const Handle(SALOME_InteractiveObject)& IObject, 
					    bool highlight, bool immediatly = true );
  void                           unHighlightAll();
  void                           rename( const Handle(SALOME_InteractiveObject)& IObject,
					 QString newName );
  bool                           isInViewer( const Handle(SALOME_InteractiveObject)& IObject );
  bool                           isVisible( const Handle(SALOME_InteractiveObject)& IObject );

  /*  undo/redo management */
  void              undo(SALOMEDS::Study_var aStudy,
			 const char* StudyFrameEntry);
  void              redo(SALOMEDS::Study_var aStudy,
			 const char* StudyFrameEntry);


  /* selection */
  Handle(SALOME_InteractiveObject) FindIObject(const char* Entry);
  
  /* display */		
  void           Display(const Handle(SALOME_InteractiveObject)& IObject, bool immediatly = true);
  void           DisplayOnly(const Handle(SALOME_InteractiveObject)& IObject);
  void           Erase(const Handle(SALOME_InteractiveObject)& IObject, bool immediatly = true);
  void           DisplayAll();
  void           EraseAll();
  void           Repaint(bool theUpdateTrihedron);
  void           Repaint() { Repaint(true); }

  //apply existing transformation on adding SALOME_Actor
  void SetScale(double theScale[3]);
  void GetScale(double theScale[3]);
  void AddActor(SALOME_Actor*, bool update = false);
  void RemoveActor(SALOME_Actor*, bool update = false);

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
 
 private:
  double                        m_ViewUp[3];
  double                        m_ViewNormal[3];
  
  void                          InitialSetup();

  vtkRenderer*                      m_Renderer;
  VTKViewer_RenderWindow*           m_RW;
  VTKViewer_RenderWindowInteractor* m_RWInteractor;

  VTKViewer_Trihedron *m_Triedron;  
  SALOME_Transform *m_Transform;
};
#endif
