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
//  File   : VTKViewer_Trihedron.cxx
//  Author : Alexey PETROV
//  Module : SALOME
//  $Header: 

#include "VTKViewer_Trihedron.h"
#include "utilities.h"
#include "SALOME_Actor.h"

// VTK Includes
#include <vtkMath.h>
#include <vtkMapper.h>
#include <vtkDataSet.h>
#include <vtkRenderer.h>
#include <vtkFollower.h>
#include <vtkRenderWindow.h>
#include <vtkObjectFactory.h>

#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkLineSource.h>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>

#include "VTKViewer_VectorText.h"

using namespace std;

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

vtkStandardNewMacro(VTKViewer_UnScaledActor);

VTKViewer_UnScaledActor::VTKViewer_UnScaledActor(){}

void VTKViewer_UnScaledActor::Render(vtkRenderer *theRenderer){
  if(theRenderer){
    float P[2][3] = {{-1.0, -1.0, 0.0},{+1.0, +1.0, 0.0}};
    theRenderer->ViewToWorld(P[0][0],P[0][1],P[0][2]);
    theRenderer->ViewToWorld(P[1][0],P[1][1],P[1][2]);
    float aWorldDiag = sqrt((P[1][0]-P[0][0])*(P[1][0]-P[0][0])+
			    (P[1][1]-P[0][1])*(P[1][1]-P[0][1])+
			    (P[1][2]-P[0][2])*(P[1][2]-P[0][2]));
    int* aSize = theRenderer->GetRenderWindow()->GetSize();
    float aWinDiag = sqrt(float(aSize[0]*aSize[0]+aSize[1]*aSize[1]));
    vtkDataSet* aDataSet = GetMapper()->GetInput();
    float aLength = aDataSet->GetLength();
    float aPrecision = 1.0E-3;
    float anOldScale = GetScale()[0];
    float aScale = mySize*aWorldDiag/aWinDiag/aLength*sqrt(float(aSize[0])/float(aSize[1]));
    if(fabs(aScale - anOldScale)/aScale > aPrecision){
      SetScale(aScale);
    }
  }
  vtkFollower::Render(theRenderer);
}

void VTKViewer_UnScaledActor::SetSize(int theSize){
  mySize = theSize;
}

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

vtkStandardNewMacro(VTKViewer_LineActor);

vtkCxxSetObjectMacro(VTKViewer_LineActor,LabelActor,VTKViewer_UnScaledActor);
vtkCxxSetObjectMacro(VTKViewer_LineActor,ArrowActor,VTKViewer_UnScaledActor);

void VTKViewer_LineActor::Render(vtkRenderer *theRenderer){
  if(LabelActor && LabelActor->GetVisibility()){
    LabelActor->Modified();
    LabelActor->Render(theRenderer);
  }
  if(ArrowActor && ArrowActor->GetVisibility()){
    ArrowActor->Modified();
    ArrowActor->Render(theRenderer);
  }
  vtkFollower::Render(theRenderer);
}

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
  
  virtual void AddToRender(vtkRenderer* theRenderer){
    //Order of the calls are important
    theRenderer->AddActor(myLineActor);
    theRenderer->AddActor(myLabelActor);
    theRenderer->AddActor(myArrowActor);
  }

  virtual void SetVisibility(VTKViewer_Trihedron::TVisibility theVis);
  virtual VTKViewer_Trihedron::TVisibility GetVisibility() { 
    return myVisibility;
  }

  virtual void SetCamera(vtkCamera* theCamera){
    myLabelActor->SetCamera(theCamera);
  }

  virtual void SetProperty(vtkProperty* theProperty){
    myLabelActor->SetProperty(theProperty);
    myArrowActor->SetProperty(theProperty);
    myLineActor->SetProperty(theProperty);
  }

  virtual void SetSize(float theSize);

  virtual VTKViewer_UnScaledActor* GetLabel(){
    return myLabelActor;
  }

  virtual VTKViewer_UnScaledActor* GetArrow(){
    return myArrowActor;
  }

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

VTKViewer_Axis::VTKViewer_Axis(){
  // Initialize the Line pipe-line representation
  myLineSource = vtkLineSource::New();
  myLineSource->SetPoint1(0.0,0.0,0.0);

  myMapper[0] = vtkPolyDataMapper::New();
  myMapper[0]->SetInput(myLineSource->GetOutput());

  myLineActor = VTKViewer_LineActor::New();
  myLineActor->SetMapper(myMapper[0]);
  myLineActor->PickableOff();

  // Initialize the Arrow pipe-line representation
  myConeSource =  vtkConeSource::New();
  myConeSource->SetResolution(2);
  myConeSource->SetAngle(10);

  myMapper[1] = vtkPolyDataMapper::New();
  myMapper[1]->SetInput(myConeSource->GetOutput());
  
  myArrowActor = VTKViewer_UnScaledActor::New();
  myArrowActor->SetMapper(myMapper[1]);
  static int aArrowActorSize = 24;
  myArrowActor->SetSize(aArrowActorSize);
  myArrowActor->PickableOff();

  myLineActor->SetArrowActor(myArrowActor);

  // Initialize the Label pipe-line representation
  myVectorText = VTKViewer_VectorText::New();

  myMapper[2] = vtkPolyDataMapper::New();
  myMapper[2]->SetInput(myVectorText->GetOutput());

  myLabelActor = VTKViewer_UnScaledActor::New();
  myLabelActor->SetMapper(myMapper[2]);
  static int aLabelActorSize = 12;
  myLabelActor->SetSize(aLabelActorSize);
  myLabelActor->PickableOff();
  //myLabelActor->DebugOn();

  myLineActor->SetLabelActor(myLabelActor);

  // Initialise visibility param.
  myVisibility = VTKViewer_Trihedron::eOn;
}

VTKViewer_Axis::~VTKViewer_Axis(){
  // Destroy of the Label pipe-line representation
  myLabelActor->Delete();

  myMapper[2]->RemoveAllInputs();
  myMapper[2]->Delete();

  myVectorText->Delete();

  // Destroy of the Arrow pipe-line representation
  myArrowActor->Delete();

  myMapper[1]->RemoveAllInputs();
  myMapper[1]->Delete();

  myConeSource->Delete();

  // Destroy of the Arrow pipe-line representation
  myLineActor->Delete();

  myMapper[2]->RemoveAllInputs();
  myMapper[2]->Delete();

  myLineSource->Delete();
}

void VTKViewer_Axis::SetVisibility(VTKViewer_Trihedron::TVisibility theVis){
  switch(theVis){
  case VTKViewer_Trihedron::eOff:
  case VTKViewer_Trihedron::eOn:
    myLabelActor->SetVisibility(theVis);
    myArrowActor->SetVisibility(theVis);
    myLineActor->SetVisibility(theVis);
    break;
  case VTKViewer_Trihedron::eOnlyLineOn:
    myLabelActor->VisibilityOff();
    myArrowActor->VisibilityOff();
    myLineActor->VisibilityOn();
    break;
  default:
    return;
  }
  myVisibility = theVis;
}

void VTKViewer_Axis::SetSize(float theSize){
  float aPosition[3] = {myDir[0]*theSize, myDir[1]*theSize, myDir[2]*theSize};
  myLineSource->SetPoint2(aPosition);

  myArrowActor->SetPosition(0.0,0.0,0.0);
  myArrowActor->AddPosition(aPosition);
  myArrowActor->SetOrientation(myRot);

  myLabelActor->SetPosition(0.0,0.0,0.0);
  myLabelActor->AddPosition(aPosition);
}

//==============================================================================
class VTKViewer_XAxis : public VTKViewer_Axis{
protected:
  VTKViewer_XAxis();
  VTKViewer_XAxis(const VTKViewer_XAxis&);
public:
  vtkTypeMacro(VTKViewer_XAxis,VTKViewer_Axis);
  static VTKViewer_XAxis *New();
};

vtkStandardNewMacro(VTKViewer_XAxis);

VTKViewer_XAxis::VTKViewer_XAxis(){ 
  myDir[0] = 1.0; myDir[1] = 0.0; myDir[2] = 0.0;
  myRot[0] = 0.0; myRot[1] = 0.0; myRot[2] = 0.0;
  myVectorText->SetText("X");
  vtkProperty* aProperty = vtkProperty::New();
  aProperty->SetColor(1.0,0.0,0.0);
  SetProperty(aProperty);
  aProperty->Delete();
}

//==============================================================================
class VTKViewer_YAxis : public VTKViewer_Axis{
protected:
  VTKViewer_YAxis();
  VTKViewer_YAxis(const VTKViewer_YAxis&);
public:
  vtkTypeMacro(VTKViewer_YAxis,VTKViewer_Axis);
  static VTKViewer_YAxis *New();
};

vtkStandardNewMacro(VTKViewer_YAxis);

VTKViewer_YAxis::VTKViewer_YAxis(){ 
  myDir[0] = 0.0; myDir[1] = 1.0; myDir[2] = 0.0;
  myRot[0] = 0.0; myRot[1] = 0.0; myRot[2] = 90.;
  myVectorText->SetText("Y");
  vtkProperty* aProperty = vtkProperty::New();
  aProperty->SetColor(0.0,1.0,0.0);
  SetProperty(aProperty);
  aProperty->Delete();
}

//==============================================================================
class VTKViewer_ZAxis : public VTKViewer_Axis{
protected:
  VTKViewer_ZAxis();
  VTKViewer_ZAxis(const VTKViewer_ZAxis&);
public:
  vtkTypeMacro(VTKViewer_ZAxis,VTKViewer_Axis);
  static VTKViewer_ZAxis *New();
};

vtkStandardNewMacro(VTKViewer_ZAxis);

VTKViewer_ZAxis::VTKViewer_ZAxis(){ 
  myDir[0] = 0.0; myDir[1] = 0.0; myDir[2] = 1.0;
  myRot[0] = 0.0; myRot[1] = -90; myRot[2] = 0.0;
  myVectorText->SetText("Z");
  vtkProperty* aProperty = vtkProperty::New();
  aProperty->SetColor(0.0,0.0,1.0);
  SetProperty(aProperty);
  aProperty->Delete();
}

//==============================================================================

vtkStandardNewMacro(VTKViewer_Trihedron);

VTKViewer_Trihedron::VTKViewer_Trihedron(){
  myPresent = vtkActorCollection::New();
  myAxis[0] = VTKViewer_XAxis::New();
  myAxis[1] = VTKViewer_YAxis::New();
  myAxis[2] = VTKViewer_ZAxis::New();
  static float aSize = 100;
  SetSize(aSize);
}

VTKViewer_Trihedron::~VTKViewer_Trihedron(){
  myPresent->RemoveAllItems();
  myPresent->Delete();
  for(int i = 0; i < 3; i++)
    myAxis[i]->Delete();
}

void VTKViewer_Trihedron::SetSize(float theSize){
  mySize = theSize;
  for(int i = 0; i < 3; i++)
    myAxis[i]->SetSize(theSize);
}

void VTKViewer_Trihedron::SetVisibility(TVisibility theVis){
  for(int i = 0; i < 3; i++)
    myAxis[i]->SetVisibility(theVis);
}

VTKViewer_Trihedron::TVisibility VTKViewer_Trihedron::GetVisibility(){
  return myAxis[0]->GetVisibility();
}

void VTKViewer_Trihedron::AddToRender(vtkRenderer* theRenderer){
  vtkCamera* aCamera = theRenderer->GetActiveCamera();
  for(int i = 0; i < 3; i++){
    myAxis[i]->AddToRender(theRenderer);
    myAxis[i]->SetCamera(aCamera);
  }
}

void VTKViewer_Trihedron::RemoveFromRender(vtkRenderer* theRenderer){
  myPresent->InitTraversal();
  while(vtkActor* anActor = myPresent->GetNextActor())
    theRenderer->RemoveActor(anActor);
}

int VTKViewer_Trihedron::GetVisibleActorCount(vtkRenderer* theRenderer){
  //TVisibility aVis = GetVisibility();
  //SetVisibility(eOff);
  vtkActorCollection* aCollection = theRenderer->GetActors();
  aCollection->InitTraversal();
  int aCount = 0;
  while(vtkActor* prop = aCollection->GetNextActor())
    if( prop->GetVisibility())
      if(SALOME_Actor* anActor = SALOME_Actor::SafeDownCast(prop))
	if(!anActor->IsInfinitive()) 
	  aCount++;
  //int aCount = theRenderer->VisibleActorCount();
  //SetVisibility(aVis);
  return aCount;
}
