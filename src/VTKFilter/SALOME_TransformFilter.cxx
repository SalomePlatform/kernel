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
//  File   : SALOME_TransformFilter.h
//  Author : Laurent CORNABE with help of Nicolas REJNERI
//  Module : SALOME


#include "SALOME_TransformFilter.h"
#include "SALOME_Transform.h"

#include <vtkObjectFactory.h>
#include <vtkPointSet.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkPoints.h>

using namespace std;

vtkStandardNewMacro(SALOME_TransformFilter);

void SALOME_TransformFilter::Execute(){
  vtkPoints *inPts;
  vtkPoints *newPts;
  int numPts, numCells;
  vtkPointSet *input = this->GetInput();
  vtkPointSet *output = this->GetOutput();
  vtkPointData *pd=input->GetPointData(), *outPD=output->GetPointData();
  vtkCellData *cd=input->GetCellData(), *outCD=output->GetCellData();
  output->CopyStructure( input );
  if(Transform){
    bool anIsIdentity = true;
    if(SALOME_Transform* aTransform = dynamic_cast<SALOME_Transform*>(Transform))
      anIsIdentity = aTransform->IsIdentity();
    inPts = input->GetPoints();
    if(!anIsIdentity && inPts){
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
  }
  outPD->PassData(pd);
  outCD->PassData(cd);
}
