//  File      : VTKViewer_ViewFrame.h
//  Created   : Wed Mar 20 11:27:26 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : VTKViewer
//  Copyright : Open CASCADE 2002
//  $Header$

#ifndef VTKViewer_ViewFrame_H
#define VTKViewer_ViewFrame_H

#include "VTKViewer_RenderWindowInteractor.h"
#include "VTKViewer_RenderWindow.h"
#include "VTKViewer_NonIsometricTransform.h"

#include "QAD.h"
#include "QAD_ViewFrame.h"

// VTK Includes
#include <vtkAxes.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>

class QAD_EXPORT VTKViewer_ViewFrame : public QAD_ViewFrame
{
  Q_OBJECT

    public:
  VTKViewer_ViewFrame(QWidget* parent, const char* name=0 );
  ~VTKViewer_ViewFrame();

  ViewType                       getTypeView() const{ return VIEW_VTK;};
  QWidget*                       getViewWidget();
  vtkRenderer*                   getRenderer() {return m_Renderer;}

  void                               setRW(VTKViewer_RenderWindow* rw);
  VTKViewer_RenderWindow*	     getRW() {return m_RW;}
  VTKViewer_RenderWindowInteractor*  getRWInteractor() {return m_RWInteractor;}

  bool                           isTrihedronDisplayed();
  void                           SetTrihedronSize( int dim );

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
  void           Repaint();

  /* non-isometric transformation */
  void setNonIsometricTransform (VTKViewer_NonIsometricTransform *NonIsometricTransform)
    {
      m_NonIsometricTransform = NonIsometricTransform;
    }
  VTKViewer_NonIsometricTransform * getNonIsometricTransform () 
    { 
      if ( m_NonIsometricTransform == NULL )
	m_NonIsometricTransform = VTKViewer_NonIsometricTransform::New();
      return m_NonIsometricTransform; 
    }
  //apply existing transformation on adding SALOME_Actor
  void AddActor(SALOME_Actor*, bool update = false);

 private:
  void AddVector(float* o,float* p,vtkRenderer* renderer);
  void AddAxis(vtkRenderer* renderer);
 
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
 
 private:
  double                        m_ViewUp[3];
  double                        m_ViewNormal[3];

  void                          InitialSetup();

  vtkRenderer*                      m_Renderer;
  VTKViewer_RenderWindow*           m_RW;
  VTKViewer_RenderWindowInteractor* m_RWInteractor;

  Standard_Boolean              m_TriedronVisible;
  vtkActorCollection*           m_Triedron;  

  VTKViewer_NonIsometricTransform *m_NonIsometricTransform;

};
#endif
