//  SALOME SUPERVGraph : build Supervisor viewer into desktop
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
//  File   : SUPERVGraph_ViewFrame.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef SUPERVGraph_ViewFrame_H
#define SUPERVGraph_ViewFrame_H

#include "QAD.h"
#include "QAD_ViewFrame.h"
#include "QAD_Application.h"

#include "SALOME_InteractiveObject.hxx"
#include "SALOME_Selection.h" 

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
  
  void                           SetSelectionMode( Selection_Mode mode );

  void                           onAdjustTrihedron( );
  
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
