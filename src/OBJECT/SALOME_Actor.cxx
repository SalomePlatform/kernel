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

using namespace std;
/*!
  \class SALOME_Actor SALOME_Actor.h
  \brief Abstract class of SALOME Objects in VTK.
*/

#include "SALOME_Actor.h"
 
// SALOME Includes
#include "utilities.h"

// VTK Includes
#include <vtkObjectFactory.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkGeometryFilter.h>
#include <vtkTransformPolyDataFilter.h>

void SALOME_Actor::Render(vtkRenderer *ren, vtkMapper *Mapper )
{
  if (this->Mapper == NULL) {
    MESSAGE ("No mapper for actor.")
    return;
  }
  
  vtkMapper *bestMapper;
  bestMapper = this->Mapper;

  /* render the property */
  if (!this->Property) {
    // force creation of a property
    this->GetProperty();
  }

  this->Property->Render(this, ren);
  if (this->BackfaceProperty) {
    this->BackfaceProperty->BackfaceRender(this, ren);
    this->Device->SetBackfaceProperty(this->BackfaceProperty);
  }
  this->Device->SetProperty(this->Property);
  
  
  /* render the texture */
  if (this->Texture) {
    this->Texture->Render(ren);
  }
  
  
  // Store information on time it takes to render.
  // We might want to estimate time from the number of polygons in mapper.
  this->Device->Render(ren,bestMapper);
  this->EstimatedRenderTime = bestMapper->GetTimeToDraw();
}

int SALOME_Actor::RenderOpaqueGeometry(vtkViewport *vp)
{
  int renderedSomething = 0; 
  vtkRenderer      *ren = (vtkRenderer *)vp;

  if ( ! this->Mapper ) {
    return 0;
  }

  // make sure we have a property
  if (!this->Property) {
    // force creation of a property
    this->GetProperty();
  }

  // is this actor opaque ?
  if (this->GetIsOpaque()) {
    this->Property->Render(this, ren);
    
    // render the backface property
    if (this->BackfaceProperty) {
      this->BackfaceProperty->BackfaceRender(this, ren);
    }
    
    // render the texture 
    if (this->Texture) {
      this->Texture->Render(ren);
    }
    this->Render(ren,this->Mapper);
    
    renderedSomething = 1;
  }
  
  return renderedSomething; 
}

void SALOME_Actor::ReleaseGraphicsResources(vtkWindow *renWin)
{
  vtkActor::ReleaseGraphicsResources(renWin);
  this->Mapper->ReleaseGraphicsResources(renWin);
}

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
    // myPassFilter[0]->Update();  -------- This and other three lines must be comment or removed to fix the regression in SMESH
    myPassFilter[1]->SetInput(myPassFilter[0]->GetPolyDataOutput());
    // myPassFilter[1]->Update();
    myTransformFilter->SetInput(myPassFilter[1]->GetPolyDataOutput());
    myPassFilter[2]->SetInput(myTransformFilter->GetOutput());
    // myPassFilter[2]->Update();
    myPassFilter[3]->SetInput(myPassFilter[2]->GetPolyDataOutput());
    // myPassFilter[3]->Update();
    if(vtkDataSetMapper* aMapper = dynamic_cast<vtkDataSetMapper*>(theMapper))
      aMapper->SetInput(myPassFilter[3]->GetOutput());
    else if(vtkPolyDataMapper* aMapper = dynamic_cast<vtkPolyDataMapper*>(theMapper))
      aMapper->SetInput(myPassFilter[3]->GetPolyDataOutput());
  }
  vtkLODActor::SetMapper(theMapper);
}

void SALOME_Actor::SetTransform(SALOME_Transform* theTransform){
  myTransformFilter->SetTransform(theTransform);
  myTransformFilter->Modified();
}

SALOME_Actor::SALOME_Actor(){
  PreviewProperty = NULL;
  myTransformFilter = SALOME_TransformFilter::New();
  myPassFilter.push_back(SALOME_PassThroughFilter::New());
  myPassFilter.push_back(SALOME_PassThroughFilter::New());
  myPassFilter.push_back(SALOME_PassThroughFilter::New());
  myPassFilter.push_back(SALOME_PassThroughFilter::New());
}

SALOME_Actor::~SALOME_Actor(){
  SetPreviewProperty(NULL);
  for(int i = 0, iEnd = myPassFilter.size(); i < iEnd; i++)
    if(myPassFilter[i] != NULL) 
      myPassFilter[i]->Delete();
}
