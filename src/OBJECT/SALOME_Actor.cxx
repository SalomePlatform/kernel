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
//  File   : SALOME_Actor.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

/*!
  \class SALOME_Actor SALOME_Actor.h
  \brief Abstract class of SALOME Objects in VTK.
*/

#include "SALOME_Actor.h"
#include "SALOME_Transform.h"
#include "SALOME_TransformFilter.h"
#include "SALOME_PassThroughFilter.h"
#include "SALOME_GeometryFilter.h"
 
// SALOME Includes
#include "utilities.h"

// VTK Includes
#include <vtkObjectFactory.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkTransformPolyDataFilter.h>

// For test
#include <vtkMapperCollection.h>
#include "vtkTimerLog.h"

using namespace std;

int SALOME_POINT_SIZE = 3;


vtkStandardNewMacro(SALOME_Actor);

void SALOME_Actor::AddToRender(vtkRenderer* theRenderer){
  theRenderer->AddActor(this);
}
void SALOME_Actor::RemoveFromRender(vtkRenderer* theRenderer){
  theRenderer->RemoveActor(this);
}


vtkPolyData* SALOME_Actor::GetPolyDataInput(){
  return myPassFilter[3]->GetPolyDataOutput();
}

void SALOME_Actor::SetMapper(vtkMapper* theMapper){
  if(theMapper){
    myPassFilter[0]->SetInput(theMapper->GetInput());
    myPassFilter[1]->SetInput(myPassFilter[0]->GetPolyDataOutput());
    myTransformFilter->SetInput(myPassFilter[1]->GetPolyDataOutput());
    myPassFilter[2]->SetInput(myTransformFilter->GetOutput());
    myPassFilter[3]->SetInput(myPassFilter[2]->GetPolyDataOutput());
    if(vtkDataSetMapper* aMapper = dynamic_cast<vtkDataSetMapper*>(theMapper))
      aMapper->SetInput(myPassFilter[3]->GetPolyDataOutput());
    else if(vtkPolyDataMapper* aMapper = dynamic_cast<vtkPolyDataMapper*>(theMapper))
      aMapper->SetInput(myPassFilter[3]->GetPolyDataOutput());
  }
  vtkLODActor::SetMapper(theMapper);
}

void SALOME_Actor::SetTransform(SALOME_Transform* theTransform){
  myTransformFilter->SetTransform(theTransform);
}


unsigned long int SALOME_Actor::GetMTime(){
  unsigned long mTime = this->Superclass::GetMTime();
  unsigned long time = myTransformFilter->GetMTime();
  mTime = ( time > mTime ? time : mTime );
  return mTime;
}


void SALOME_Actor::SetRepresentation(int theMode) { 
  switch(myRepresentation){
  case 0 : 
  case 2 : 
    myProperty->DeepCopy(GetProperty());
  }    
  switch(theMode){
  case 0 : 
  case 2 : 
    GetProperty()->DeepCopy(myProperty);
    break;
  default:
    GetProperty()->SetAmbient(1.0);
    GetProperty()->SetDiffuse(0.0);
    GetProperty()->SetSpecular(0.0);
  }
  switch(theMode){
  case 3 : 
    myPassFilter[0]->SetInside(true);
    GetProperty()->SetRepresentation(1);
    break;
  case 0 : 
    GetProperty()->SetPointSize(SALOME_POINT_SIZE);  
  default :
    GetProperty()->SetRepresentation(theMode);
    myPassFilter[0]->SetInside(false);
  }
  myRepresentation = theMode;
}
int SALOME_Actor::GetRepresentation(){ 
  return myRepresentation;
}


SALOME_Actor::SALOME_Actor(){
  PreviewProperty = NULL;
  ispreselected = Standard_False;
  myProperty = vtkProperty::New();
  myRepresentation = 2;
  myTransformFilter = SALOME_TransformFilter::New();
  myPassFilter.push_back(SALOME_PassThroughFilter::New());
  myPassFilter.push_back(SALOME_PassThroughFilter::New());
  myPassFilter.push_back(SALOME_PassThroughFilter::New());
  myPassFilter.push_back(SALOME_PassThroughFilter::New());
}

SALOME_Actor::~SALOME_Actor(){
  myTransformFilter->Delete();
  SetPreviewProperty(NULL);
  for(int i = 0, iEnd = myPassFilter.size(); i < iEnd; i++)
    if(myPassFilter[i] != NULL){
      myPassFilter[i]->UnRegisterAllOutputs(); 
      myPassFilter[i]->Delete();
    }
  myProperty->Delete();
}
