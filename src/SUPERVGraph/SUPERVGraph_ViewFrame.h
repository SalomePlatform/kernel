//  File      : SUPERVGraph_ViewFrame.h
//  Created   : Wed Mar 20 14:22:27 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SUPERVGraph
//  Copyright : Open CASCADE 2002
//  $Header$


#ifndef SUPERVGraph_ViewFrame_H
#define SUPERVGraph_ViewFrame_H

#include "QAD.h"
#include "QAD_ViewFrame.h"
#include "QAD_Application.h"

#include "SALOME_InteractiveObject.hxx"

class QAD_EXPORT SUPERVGraph_View: public QWidget, public QAD_PopupClientServer{
  Q_OBJECT;
 public:
  SUPERVGraph_View(QWidget* theParent);
  SUPERVGraph_View(SUPERVGraph_View* theParent);

  virtual void ActivatePanning() = 0;
  virtual void ResetView() = 0;
 protected:
  void init(QWidget* theParent);
  void onCreatePopup();
};

class QAD_EXPORT SUPERVGraph_ViewFrame : public QAD_ViewFrame {
  Q_OBJECT

    public:
  SUPERVGraph_ViewFrame(QWidget* parent, const char* name=0 );
  ~SUPERVGraph_ViewFrame();

  ViewType                       getTypeView() const{ return VIEW_GRAPHSUPERV;};
  SUPERVGraph_View*              getViewWidget();
  void                           setViewWidget(SUPERVGraph_View* theView);

  void                           setBackgroundColor( const QColor& );
  QColor                         backgroundColor() const;
  
  void                           SetSelectionMode( int mode );

  void                           SetTrihedronSize( int dim );
  
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
  Handle(SALOME_InteractiveObject) FindIObject(const char* Entry) { };

  /* display */		
  void           Display(const Handle(SALOME_InteractiveObject)& IObject, bool immediatly = true){};
  void           DisplayOnly(const Handle(SALOME_InteractiveObject)& IObject){};
  void           Erase(const Handle(SALOME_InteractiveObject)& IObject, bool immediatly = true){};
  void           DisplayAll(){};
  void           EraseAll(){};
  void           Repaint() {};

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
  SUPERVGraph_View* myView;
  QAD_Application* myApp;
}; 
#endif
