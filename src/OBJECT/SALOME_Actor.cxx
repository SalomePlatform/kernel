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
  PreviewProperty = NULL;
  ispreselected = Standard_False;
  myProperty = vtkProperty::New();

  myRepresentation = 2;

  myIsInfinite = false;

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
  if(theMapper){
    int anId = 0;
#if defined __GNUC_2__
    myPassFilter[anId]->SetInput(theMapper->GetInput());
#else
    myPassFilter.at(anId)->SetInput(theMapper->GetInput());
#endif
    myGeomFilter->SetStoreMapping(myStoreMapping);
#if defined __GNUC_2__
    myGeomFilter->SetInput(myPassFilter[anId]->GetOutput());
#else
    myGeomFilter->SetInput(myPassFilter.at(anId)->GetOutput());
#endif

    anId++;

#if defined __GNUC_2__
    myPassFilter[anId]->SetInput(myGeomFilter->GetOutput());
    myPassFilter[anId+1]->SetInput(myPassFilter[anId]->GetOutput());
#else
    myPassFilter.at(anId)->SetInput(myGeomFilter->GetOutput());
    myPassFilter.at(anId+1)->SetInput(myPassFilter.at(anId)->GetOutput());
#endif

    anId++;
#if defined __GNUC_2__
    myPassFilter[anId+1]->SetInput(myPassFilter[anId]->GetOutput());
#else
    myPassFilter.at(anId+1)->SetInput(myPassFilter.at(anId)->GetOutput());
#endif

    anId++;
#if defined __GNUC_2__
    myTransformFilter->SetInput(myPassFilter[anId]->GetPolyDataOutput());
#else
    myTransformFilter->SetInput(myPassFilter.at(anId)->GetPolyDataOutput());
#endif

    anId++;
#if defined __GNUC_2__
    myPassFilter[anId]->SetInput(myTransformFilter->GetOutput());
    myPassFilter[anId+1]->SetInput(myPassFilter[anId]->GetOutput());
#else
    myPassFilter.at(anId)->SetInput(myTransformFilter->GetOutput());
    myPassFilter.at(anId+1)->SetInput(myPassFilter.at(anId)->GetOutput());
#endif

    anId++;
    if(vtkDataSetMapper* aMapper = dynamic_cast<vtkDataSetMapper*>(theMapper))
#if defined __GNUC_2__
      aMapper->SetInput(myPassFilter[anId]->GetOutput());
#else
      aMapper->SetInput(myPassFilter.at(anId)->GetOutput());
#endif
    else if(vtkPolyDataMapper* aMapper = dynamic_cast<vtkPolyDataMapper*>(theMapper))
#if defined __GNUC_2__
      aMapper->SetInput(myPassFilter[anId]->GetPolyDataOutput());
#else
      aMapper->SetInput(myPassFilter.at(anId)->GetPolyDataOutput());
#endif
  }
  vtkLODActor::SetMapper(theMapper);
}

vtkPolyData* SALOME_Actor::GetPolyDataInput(){
  return myPassFilter.back()->GetPolyDataOutput();
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
    break;
    GetProperty()->SetAmbient(1.0);
    GetProperty()->SetDiffuse(0.0);
    GetProperty()->SetSpecular(0.0);
  }
  switch(theMode){
  case 3 : 
    myGeomFilter->SetInside(true);
    GetProperty()->SetRepresentation(1);
    break;
  case 0 : 
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

//=================================================================================
// function : GetObjDimension
// purpose  : Return object dimension.
//            Virtual method shoulb be redifined by derived classes
//=================================================================================
int SALOME_Actor::GetObjDimension( const int theObjId )
{
  if ( theObjId < 0 )
    return 0;
    
  std::vector<int> aVtkList = GetVtkId( theObjId );
  int nbVtk = aVtkList.size();
  
  if ( nbVtk == 0 )
    return 0;
  else if ( nbVtk > 1 )
    return 3;
  else //  nbVtk = 1
  {
    if ( vtkDataSet* aGrid = GetMapper()->GetInput() )
    {
      if ( vtkCell* aCell = aGrid->GetCell( aVtkList.front() ) )
        return aCell->GetCellDimension();
    }
  }
  return 0;
}


















