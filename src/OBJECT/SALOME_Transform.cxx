// File:	SALOME_Transform.cxx
// Created:	Wed Jun  4 09:46:59 2003
// Author:	Alexey PETROV
//		<apo@ivanox.nnov.matra-dtv.fr>


#include "SALOME_Transform.h"
#include "VTKViewer_Common.h"
#include <vtkObjectFactory.h>
#include <vtkGeometryFilter.h>
#include <vtkMatrix4x4.h>
using namespace std;

//=======================================================================

SALOME_Transform* SALOME_Transform::New(){
  vtkObject* ret = vtkObjectFactory::CreateInstance("SALOME_Transform");
  if(ret) return (SALOME_Transform*)ret;
  return new SALOME_Transform;
}

void SALOME_Transform::SetScale(float theScaleX, float theScaleY, float theScaleZ){ 
  double aMatrix[16] = {theScaleX,0,0,0, 
                        0,theScaleY,0,0, 
                        0,0,theScaleZ,0, 
                        0,0,0,1.0000000};
  vtkTransform::SetMatrix(aMatrix);
  //vtkTransform::Pop();
  //vtkTransform::Scale(theScale);
}

int SALOME_Transform::IsIdentity(){ 
  float* aScale = GetScale();
  return (aScale[0] == 1.0 && aScale[1] == 1.0 && aScale[2] == 1.0);
}

//=======================================================================

SALOME_TransformFilter* SALOME_TransformFilter::New(){
  vtkObject* ret = vtkObjectFactory::CreateInstance("SALOME_TransformFilter");
  if(ret) return (SALOME_TransformFilter*)ret;
  return new SALOME_TransformFilter;
}

void SALOME_TransformFilter::Execute(){
  vtkPoints *inPts;
  vtkPoints *newPts;
  int numPts, numCells;
  vtkPointSet *input = this->GetInput();
  vtkPointSet *output = this->GetOutput();
  vtkPointData *pd=input->GetPointData(), *outPD=output->GetPointData();
  vtkCellData *cd=input->GetCellData(), *outCD=output->GetCellData();
  output->CopyStructure( input );
  int anIdentity = 0;
  if(SALOME_Transform* aTransform = dynamic_cast<SALOME_Transform*>(this->Transform))
    anIdentity = aTransform->IsIdentity();
  if(!anIdentity && this->Transform != NULL){
    inPts = input->GetPoints();
    if(!inPts){
      vtkErrorMacro(<<"No input data");
      return;
    }
    numPts = inPts->GetNumberOfPoints();
    numCells = input->GetNumberOfCells();
    newPts = vtkPoints::New();
    newPts->Allocate(numPts);
    this->UpdateProgress(.2);
    this->Transform->TransformPoints(inPts,newPts);
    this->UpdateProgress(.8);
    output->SetPoints(newPts);
    newPts->Delete();
  }
  outPD->PassData(pd);
  outCD->PassData(cd);
}

//=======================================================================

SALOME_PassThroughFilter* SALOME_PassThroughFilter::New(){
  vtkObject* ret = vtkObjectFactory::CreateInstance("SALOME_PassThroughFilter");
  if(ret) return (SALOME_PassThroughFilter*)ret;
  return new SALOME_PassThroughFilter;
}

SALOME_PassThroughFilter::SALOME_PassThroughFilter(){
  myGeomFilter = vtkGeometryFilter::New();
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
