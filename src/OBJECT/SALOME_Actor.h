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

#include <vector>

#include <vtkLODActor.h>
#include <vtkProperty.h>

class vtkCell;
class vtkDataSet;
class vtkPolyData;
class vtkCamera;

#include "SALOME_InteractiveObject.hxx"

class SALOME_Transform;
class SALOME_GeometryFilter;
class SALOME_TransformFilter;
class SALOME_PassThroughFilter;

extern int SALOME_POINT_SIZE;

class SALOME_Actor : public vtkLODActor{
 public:
  static SALOME_Actor* New();

  vtkTypeMacro(SALOME_Actor,vtkLODActor);

  virtual Standard_Boolean hasIO() { return !myIO.IsNull(); }
  virtual const Handle(SALOME_InteractiveObject)& getIO() { return myIO; } 
  virtual void setIO(const Handle(SALOME_InteractiveObject)& io) { myIO = io; }

  virtual const char* getName() { return myName.c_str(); }
  virtual void setName(const char* theName){
    if(hasIO())	myIO->setName(theName);
    myName = theName;
  }

  // To generate highlight automaticaly
  virtual bool hasHighlight() { return false; } 
  virtual void highlight(bool theHighlight) { myIsHighlighted = theHighlight; }  
  virtual bool isHighlighted() { return myIsHighlighted; }

  virtual void SetOpacity(float theOpacity);
  virtual float GetOpacity();

  virtual void SetColor(float r,float g,float b);
  virtual void GetColor(float& r,float& g,float& b);
  void SetColor(const float theRGB[3]){ 
    SetColor(theRGB[0],theRGB[1],theRGB[2]);
  }

  vtkSetObjectMacro(PreviewProperty,vtkProperty);

  virtual void SetPreSelected(bool thePreselect = false) { myIsPreselected = thePreselect;}


  // Used to obtain all dependent actors
  virtual void GetChildActors(vtkActorCollection*) {};
  
  virtual void AddToRender(vtkRenderer* theRenderer); 
  virtual void RemoveFromRender(vtkRenderer* theRenderer);


  // For selection mapping purpose
  virtual int GetNodeObjId(int theVtkID) { return theVtkID;}
  virtual float* GetNodeCoord(int theObjID);

  virtual int GetElemObjId(int theVtkID) { return theVtkID;}
  virtual vtkCell* GetElemCell(int theObjID);

  virtual int GetObjDimension( const int theObjId );

  virtual void SetMapper(vtkMapper* theMapper); 
  virtual vtkDataSet* GetInput(); 


  virtual void SetTransform(SALOME_Transform* theTransform); 
  virtual unsigned long int GetMTime();

  virtual void SetRepresentation(int theMode);
  virtual int GetRepresentation();

  virtual int getDisplayMode();
  virtual void setDisplayMode(int theMode);

  // Infinitive means actor without size (point for example),
  // which is not taken into account in calculation of boundaries of the scene
  void SetInfinitive(bool theIsInfinite) { myIsInfinite = theIsInfinite; }
  virtual bool IsInfinitive();
    
  void SetResolveCoincidentTopology(bool theIsResolve);
  void SetPolygonOffsetParameters(float factor, float units);
  void GetPolygonOffsetParameters(float& factor, float& units);

  virtual void Render(vtkRenderer *, vtkMapper *);

  virtual float GetShrinkFactor() { return 1.0;}

  virtual bool IsShrunkable() { return false;}
  virtual bool IsShrunk() { return false;}

  virtual void SetShrink() {} 
  virtual void UnShrink() {}

  virtual bool IsSetCamera() const { return false; }
  virtual bool IsResizable() const { return false; }
  virtual void SetSize( const float ) {}
  virtual void SetCamera( vtkCamera* ) {}

 protected:
  bool myIsResolveCoincidentTopology;
  float myPolygonOffsetFactor;
  float myPolygonOffsetUnits;

  Handle(SALOME_InteractiveObject) myIO;
  std::string myName;

  vtkProperty *PreviewProperty;
  bool myIsPreselected;

  float myOpacity;
  bool myIsHighlighted;
  int myDisplayMode;
  bool myIsInfinite;

  bool myStoreMapping;
  SALOME_GeometryFilter *myGeomFilter;
  SALOME_TransformFilter *myTransformFilter;
  std::vector<SALOME_PassThroughFilter*> myPassFilter;

  int myRepresentation;
  vtkProperty *myProperty;

  void InitPipeLine(vtkMapper* theMapper); 

  SALOME_Actor();
  ~SALOME_Actor();
};


#endif // SALOME_ACTOR_H

