//  SALOME VTKViewer : 
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
//  File   : VTKViewer_Trihedron.h
//  Author : Alexey PETROV
//  Module : SALOME
//  $Header: 

#ifndef VTKViewer_Trihedron_H
#define VTKViewer_Trihedron_H

#include <vtkObject.h>
#include <vtkFollower.h>

class vtkActorCollection;
class VTKViewer_Axis;
class vtkRenderer;
class vtkPolyDataMapper;
class vtkLineSource;
class vtkConeSource;
class VTKViewer_VectorText;
class vtkCamera;
class vtkProperty;

//==============================================================================

class VTKViewer_UnScaledActor: public vtkFollower{
  VTKViewer_UnScaledActor(const VTKViewer_UnScaledActor&);

public:
  vtkTypeMacro(VTKViewer_UnScaledActor,vtkFollower);
  static VTKViewer_UnScaledActor *New();

  virtual void SetSize(int theSize);
  virtual void Render(vtkRenderer *theRenderer);

protected:
  VTKViewer_UnScaledActor();
  ~VTKViewer_UnScaledActor(){}
  int mySize;
};

//==============================================================================

class VTKViewer_LineActor: public vtkFollower{
  VTKViewer_LineActor(const VTKViewer_LineActor&);

public:
  vtkTypeMacro(VTKViewer_LineActor,vtkFollower);
  static VTKViewer_LineActor *New();

  void SetLabelActor(VTKViewer_UnScaledActor* theLabelActor);
  void SetArrowActor(VTKViewer_UnScaledActor* theLabelActor);
  virtual void Render(vtkRenderer *theRenderer);

protected:
  VTKViewer_LineActor(){
    LabelActor = NULL;
    ArrowActor = NULL;
  }
  ~VTKViewer_LineActor(){
    SetLabelActor(NULL);
    SetArrowActor(NULL);
  }

  VTKViewer_UnScaledActor* LabelActor;
  VTKViewer_UnScaledActor* ArrowActor;
};

//================================================

class VTKViewer_Trihedron : public vtkObject{
 protected:
  VTKViewer_Trihedron();
  VTKViewer_Trihedron(const VTKViewer_Trihedron&);
  virtual ~VTKViewer_Trihedron();
 public:
  vtkTypeMacro(VTKViewer_Trihedron,vtkObject);
  static VTKViewer_Trihedron *New();
  
  virtual void SetSize(float theSize);
  virtual float GetSize() { return mySize;}

  enum TVisibility{eOff, eOn, eOnlyLineOn};
  virtual void SetVisibility(TVisibility theVis);
  virtual void VisibilityOff() { SetVisibility(eOff);}
  virtual void VisibilityOn() { SetVisibility(eOn);}
  virtual TVisibility GetVisibility();

  virtual void AddToRender(vtkRenderer* theRenderer);
  virtual void RemoveFromRender(vtkRenderer* theRenderer);

  virtual int GetVisibleActorCount(vtkRenderer* theRenderer);
  
protected:
  vtkActorCollection* myPresent;
  VTKViewer_Axis* myAxis[3];
  float mySize;
};

//==============================================================================
// The base class for concreate Axis
// Its only duty is to give correct initialization and destruction
//   of its pipe-lines
class VTKViewer_Axis : public vtkObject{
protected:
  VTKViewer_Axis();
  VTKViewer_Axis(const VTKViewer_Axis&);
  virtual ~VTKViewer_Axis();
 public:
  vtkTypeMacro(VTKViewer_Axis,vtkObject);

  virtual void AddToRender(vtkRenderer* theRenderer);

  virtual void SetVisibility(VTKViewer_Trihedron::TVisibility theVis);
  virtual VTKViewer_Trihedron::TVisibility GetVisibility() { return myVisibility; }

  virtual void SetCamera(vtkCamera* theCamera);
  virtual void SetProperty(vtkProperty* theProperty);
  virtual void SetSize(float theSize);

  virtual VTKViewer_UnScaledActor* GetLabel(){ return myLabelActor; }
  virtual VTKViewer_UnScaledActor* GetArrow(){ return myArrowActor; }

protected:
  VTKViewer_Trihedron::TVisibility myVisibility;
  float myDir[3], myRot[3];

  VTKViewer_LineActor *myLineActor;
  //vtkActor *myLineActor;
  VTKViewer_UnScaledActor *myArrowActor;
  VTKViewer_UnScaledActor *myLabelActor;

  vtkPolyDataMapper *myMapper[3];
  vtkLineSource *myLineSource;
  vtkConeSource *myConeSource;
  VTKViewer_VectorText* myVectorText;
};

#endif
