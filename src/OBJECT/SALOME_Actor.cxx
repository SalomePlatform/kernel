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
#include "SALOME_GeometryFilter.h"
#include "SALOME_TransformFilter.h"
#include "SALOME_PassThroughFilter.h"
 
// SALOME Includes
#include "utilities.h"

// VTK Includes
#include <vtkCell.h>
#include <vtkRenderer.h>
#include <vtkPolyData.h>
#include <vtkObjectFactory.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataMapper.h>

using namespace std;

#if defined __GNUC__
  #if __GNUC__ == 2
    #define __GNUC_2__
  #endif
#endif

int SALOME_POINT_SIZE = 3;


vtkStandardNewMacro(SALOME_Actor);


SALOME_Actor::SALOME_Actor(){
  myIsHighlighted = myIsPreselected = false;

  myRepresentation = 1;
  myDisplayMode = myRepresentation - 1;

  myProperty = vtkProperty::New();
  PreviewProperty = NULL;

  myIsInfinite = false;

  myIsResolveCoincidentTopology = true;

  vtkMapper::GetResolveCoincidentTopologyPolygonOffsetParameters(myPolygonOffsetFactor,
								 myPolygonOffsetUnits);
  myStoreMapping = false;
  myGeomFilter = SALOME_GeometryFilter::New();

  myTransformFilter = SALOME_TransformFilter::New();

  for(int i = 0; i < 6; i++)
    myPassFilter.push_back(SALOME_PassThroughFilter::New());
}


SALOME_Actor::~SALOME_Actor(){
  SetPreviewProperty(NULL);

  myGeomFilter->UnRegisterAllOutputs(); 
  myGeomFilter->Delete();

  myTransformFilter->UnRegisterAllOutputs();
  myTransformFilter->Delete();

  for(int i = 0, iEnd = myPassFilter.size(); i < iEnd; i++){
    if(myPassFilter[i]){
      myPassFilter[i]->UnRegisterAllOutputs(); 
      myPassFilter[i]->Delete();
    }
  }

  myProperty->Delete();
}


void SALOME_Actor::AddToRender(vtkRenderer* theRenderer){
  theRenderer->AddActor(this);
}

void SALOME_Actor::RemoveFromRender(vtkRenderer* theRenderer){
  theRenderer->RemoveActor(this);
}


void SALOME_Actor::SetTransform(SALOME_Transform* theTransform){
  myTransformFilter->SetTransform(theTransform);
}


void SALOME_Actor::SetMapper(vtkMapper* theMapper){
  InitPipeLine(theMapper);
}

void SALOME_Actor::InitPipeLine(vtkMapper* theMapper){
  if(theMapper){
    int anId = 0;
    myPassFilter[ anId ]->SetInput( theMapper->GetInput() );
    myPassFilter[ anId + 1]->SetInput( myPassFilter[ anId ]->GetOutput() );
    
    anId++; // 1
    myGeomFilter->SetStoreMapping( myStoreMapping );
    myGeomFilter->SetInput( myPassFilter[ anId ]->GetOutput() );

    anId++; // 2
    myPassFilter[ anId ]->SetInput( myGeomFilter->GetOutput() ); 
    myPassFilter[ anId + 1 ]->SetInput( myPassFilter[ anId ]->GetOutput() );

    anId++; // 3
    myTransformFilter->SetInput( myPassFilter[ anId ]->GetPolyDataOutput() );

    anId++; // 4
    myPassFilter[ anId ]->SetInput( myTransformFilter->GetOutput() );
    myPassFilter[ anId + 1 ]->SetInput( myPassFilter[ anId ]->GetOutput() );

    anId++; // 5
    if(vtkDataSetMapper* aMapper = dynamic_cast<vtkDataSetMapper*>(theMapper)){
      aMapper->SetInput(myPassFilter[anId]->GetOutput());
    }else if(vtkPolyDataMapper* aMapper = dynamic_cast<vtkPolyDataMapper*>(theMapper)){
      aMapper->SetInput(myPassFilter[anId]->GetPolyDataOutput());
    }
  }
  vtkLODActor::SetMapper(theMapper);
}


void SALOME_Actor::Render(vtkRenderer *ren, vtkMapper* m){
  if(myIsResolveCoincidentTopology){
    int aResolveCoincidentTopology = vtkMapper::GetResolveCoincidentTopology();
    float aFactor, aUnit; 
    vtkMapper::GetResolveCoincidentTopologyPolygonOffsetParameters(aFactor,aUnit);
    
    vtkMapper::SetResolveCoincidentTopologyToPolygonOffset();
    vtkMapper::SetResolveCoincidentTopologyPolygonOffsetParameters(myPolygonOffsetFactor,
								   myPolygonOffsetUnits);
    vtkLODActor::Render(ren,m);
    
    vtkMapper::SetResolveCoincidentTopologyPolygonOffsetParameters(aFactor,aUnit);
    vtkMapper::SetResolveCoincidentTopology(aResolveCoincidentTopology);
  }else{
    vtkLODActor::Render(ren,m);
  }
}


void SALOME_Actor::SetResolveCoincidentTopology(bool theIsResolve) {
  myIsResolveCoincidentTopology = theIsResolve;
}

void SALOME_Actor::SetPolygonOffsetParameters(float factor, float units){
  myPolygonOffsetFactor = factor;
  myPolygonOffsetUnits = units;
}

void SALOME_Actor::GetPolygonOffsetParameters(float& factor, float& units){
  factor = myPolygonOffsetFactor;
  units = myPolygonOffsetUnits;
}


vtkDataSet* SALOME_Actor::GetInput(){
  return myPassFilter.front()->GetOutput();
}


unsigned long int SALOME_Actor::GetMTime(){
  unsigned long mTime = this->Superclass::GetMTime();
  unsigned long time = myTransformFilter->GetMTime();
  mTime = ( time > mTime ? time : mTime );
  if(vtkDataSet *aDataSet = myPassFilter[0]->GetInput()){
    time = aDataSet->GetMTime();
    mTime = ( time > mTime ? time : mTime );
  }
  return mTime;
}


void SALOME_Actor::SetRepresentation(int theMode) { 
  switch(myRepresentation){
  case VTK_POINTS : 
  case VTK_SURFACE : 
    myProperty->DeepCopy(GetProperty());
  }    
  switch(theMode){
  case VTK_POINTS : 
  case VTK_SURFACE : 
    GetProperty()->DeepCopy(myProperty);
    break;
  default:
    GetProperty()->SetAmbient(1.0);
    GetProperty()->SetDiffuse(0.0);
    GetProperty()->SetSpecular(0.0);
  }
  switch(theMode){
  case 3 : 
    myGeomFilter->SetInside(true);
    GetProperty()->SetRepresentation(1);
    break;
  case VTK_POINTS : 
    GetProperty()->SetPointSize(SALOME_POINT_SIZE);  
  default :
    GetProperty()->SetRepresentation(theMode);
    myGeomFilter->SetInside(false);
  }
  myRepresentation = theMode;
}

int SALOME_Actor::GetRepresentation(){ 
  return myRepresentation;
}


vtkCell* SALOME_Actor::GetElemCell(int theObjID){
  return GetInput()->GetCell(theObjID);
}


float* SALOME_Actor::GetNodeCoord(int theObjID){
  return GetInput()->GetPoint(theObjID);
}


//=================================================================================
// function : GetObjDimension
// purpose  : Return object dimension.
//            Virtual method shoulb be redifined by derived classes
//=================================================================================
int SALOME_Actor::GetObjDimension( const int theObjId )
{
  if ( vtkCell* aCell = GetElemCell(theObjId) )
    return aCell->GetCellDimension();
  return 0;
}


bool SALOME_Actor::IsInfinitive(){ 
  return myIsInfinite; 
}


void SALOME_Actor::SetOpacity(float theOpacity){ 
  myOpacity = theOpacity;
  GetProperty()->SetOpacity(theOpacity);
}

float SALOME_Actor::GetOpacity(){
  return myOpacity;
}


void SALOME_Actor::SetColor(float r,float g,float b){
  GetProperty()->SetColor(r,g,b);
}

void SALOME_Actor::GetColor(float& r,float& g,float& b){
  float aColor[3];
  GetProperty()->GetColor(aColor);
  r = aColor[0];
  g = aColor[1];
  b = aColor[2];
}


int SALOME_Actor::getDisplayMode(){ 
  return myDisplayMode; 
}

void SALOME_Actor::setDisplayMode(int theMode){ 
  SetRepresentation(theMode+1); 
  myDisplayMode = GetRepresentation() - 1;
}
