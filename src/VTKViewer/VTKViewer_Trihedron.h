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

class vtkActorCollection;
class VTKViewer_Axis;
class vtkRenderer;

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
  
  virtual void  Render(vtkRenderer *theRenderer);
  
protected:
  vtkActorCollection* myPresent;
  VTKViewer_Axis* myAxis[3];
  float mySize;
};

#endif
