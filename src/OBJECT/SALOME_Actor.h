//  SALOME OBJECT : implementation of interactive object visualization for OCC and VTK viewers
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
//  File   : SALOME_Actor.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef SALOME_ACTOR_H
#define SALOME_ACTOR_H

// SALOME Includes
#include "VTKViewer_Common.h"
#include "SALOME_InteractiveObject.hxx"
#ifndef _Handle_SALOME_InteractiveObject_HeaderFile
#include "Handle_SALOME_InteractiveObject.hxx"
#endif
#include <vector.h>

class SALOME_Transform;
class SALOME_TransformFilter;
class SALOME_PassThroughFilter;

extern int SALOME_POINT_SIZE;

class SALOME_Actor : public vtkLODActor{
 protected:
  //constructor should be protected due to first implementation of this class
  //it was abstract class
  SALOME_Actor();
  ~SALOME_Actor();
 public:
  vtkTypeMacro(SALOME_Actor,vtkActor);

  // Description:
  // This causes the actor to be rendered. It, in turn, will render the actor`s
  // property and then mapper.
  virtual void Render(vtkRenderer *, vtkMapper *);

  // Description:
  // This method is used internally by the rendering process.
  // We overide the superclass method to properly set the estimated render time.
  virtual int RenderOpaqueGeometry(vtkViewport *viewport);

  // Description:
  // Release any graphics resources that are being consumed by this actor.
  // The parameter window could be used to determine which graphic
  // resources to release.
  virtual void ReleaseGraphicsResources(vtkWindow *);

  virtual Standard_Boolean hasIO() { return !myIO.IsNull(); }
  virtual Handle_SALOME_InteractiveObject getIO() { return myIO; } 
  virtual void setIO(const Handle(SALOME_InteractiveObject)& io) { myIO = io; }

  virtual Standard_CString getName()     { return myName; }
  virtual void setName(Standard_CString aName){
    myName = aName;
    if(hasIO())	myIO->setName(aName);
  }

  virtual int getDisplayMode() { return myDisplayMode; }
  virtual void setDisplayMode(int mode) { myDisplayMode = mode; }

  virtual bool hasHighlight() { return false; }  
  virtual void highlight(Standard_Boolean highlight) { ishighlighted = highlight; }  
  virtual Standard_Boolean isHighlighted() { return ishighlighted; }

  virtual void SetOpacity(float opa) { m_opacity = opa; }
  virtual float GetOpacity() { return m_opacity; }

  virtual void SetColor(float r,float g,float b) {};
  virtual void GetColor(float& r,float& g,float& b) {};

  vtkSetObjectMacro(PreviewProperty,vtkProperty);

  virtual void SetPreSelected(Standard_Boolean presel = Standard_False)   { ispreselected = presel; }

  // Used to obtain all dependent actors
  virtual void GetChildActors(vtkActorCollection*) {};
  
  virtual void AddToRender(vtkRenderer* theRenderer); 
  virtual void RemoveFromRender(vtkRenderer* theRenderer);

  virtual vtkPolyData* GetPolyDataInput(); 
  virtual void SetMapper(vtkMapper* theMapper); 
  virtual void SetTransform(SALOME_Transform* theTransform); 

  virtual void SetRepresentation(int theMode);
  virtual int GetRepresentation();
    
 protected:
  vtkProperty         *PreviewProperty;
  Standard_Boolean    ispreselected;

  Handle_SALOME_InteractiveObject myIO;
  Standard_CString myName;

  float   m_opacity;
  Standard_Boolean    ishighlighted;
  int myDisplayMode;

  SALOME_Transform *myTransform;
  std::vector<SALOME_PassThroughFilter*> myPassFilter;
  SALOME_TransformFilter *myTransformFilter;
  int myRepresentation;
};

#endif // SALOME_ACTOR_H
