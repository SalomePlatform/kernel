//  SALOME FILTER : interactive object for VISU entities implementation
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
//  File   : SALOME_PassThroughFilter.cxx
//  Author : Laurent CORNABE with help of Nicolas REJNERI
//  Module : SALOME


#include "SALOME_PassThroughFilter.h"
#include "SALOME_GeometryFilter.h"

#include <vtkObjectFactory.h>
#include <vtkPointSet.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkPoints.h>
#include <vtkMatrix4x4.h>

using namespace std;

vtkStandardNewMacro(SALOME_PassThroughFilter);

SALOME_PassThroughFilter::SALOME_PassThroughFilter(){
  this->vtkProcessObject::SetNthInput(0, NULL);
  myGeomFilter = SALOME_GeometryFilter::New();
}

SALOME_PassThroughFilter::~SALOME_PassThroughFilter(){
  myGeomFilter->Delete();
}

void SALOME_PassThroughFilter::Execute(){
  vtkDataSet *input = static_cast<vtkDataSet*>(this->GetInput());
  vtkDataSet *output = static_cast<vtkDataSet*>(this->GetOutput());
  output->CopyStructure( input );
  output->GetPointData()->PassData( input->GetPointData() );
  output->GetCellData()->PassData( input->GetCellData() );
}


void SALOME_PassThroughFilter::SetInput(vtkDataSet *input){
  myGeomFilter->SetInput(input);
  vtkDataSet *oldInput = this->GetInput();
  if(oldInput != NULL)
    if(input == NULL || oldInput->GetDataObjectType() != input->GetDataObjectType()){
      vtkWarningMacro("Changing input type.  Deleting output");
      this->SetOutput(NULL);
    }
  if (input != NULL && this->vtkSource::GetOutput(0) == NULL){
    this->vtkSource::SetNthOutput(0, input->NewInstance());
    this->Outputs[0]->ReleaseData();
    this->Outputs[0]->Delete();
  }
  this->vtkProcessObject::SetNthInput(0, input);
}
vtkPolyData *SALOME_PassThroughFilter::GetPolyDataOutput() {
  vtkDataSet *ds = this->GetOutput();
  if(!ds) return NULL;
  if(ds->GetDataObjectType() == VTK_POLY_DATA) return (vtkPolyData *)ds;
  myGeomFilter->SetInput(this->GetOutput());
  return myGeomFilter->GetOutput();
}


void SALOME_PassThroughFilter::SetInside(int theShowInside){
  myGeomFilter->SetInside(theShowInside);
  Modified();
}
int SALOME_PassThroughFilter::GetInside(){
  return myGeomFilter->GetInside();
}
