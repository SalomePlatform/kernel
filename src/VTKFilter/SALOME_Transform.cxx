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
//  File   : SALOME_Transform.cxx
//  Author : Laurent CORNABE with help of Nicolas REJNERI
//  Module : SALOME


#include "SALOME_Transform.h"

#include <vtkObjectFactory.h>
#include <vtkMatrix4x4.h>

using namespace std;

static double EPS = 10e-4;

vtkStandardNewMacro(SALOME_Transform);

//void SALOME_Transform::SetScale(float theScaleX, float theScaleY, float theScaleZ){ 
//  double aMatrix[16] = {theScaleX,0,0,0, 
//                        0,theScaleY,0,0, 
//                        0,0,theScaleZ,0, 
//                        0,0,0,1.0000000};
//  vtkTransform::SetMatrix(aMatrix);
//}

/*!Sets matrix scale.*/
void SALOME_Transform::SetMatrixScale(double theScaleX, double theScaleY, double theScaleZ){ 
  double aMatrix[16] = {theScaleX,0,0,0, 
                        0,theScaleY,0,0, 
                        0,0,theScaleZ,0, 
                        0,0,0,1.0000000};
  this->SetMatrix(aMatrix);
}

/*!Gets matrix scale.*/
void SALOME_Transform::GetMatrixScale(double theScale[3]){
  vtkMatrix4x4 *aTMatrix=this->GetMatrix();
  const double aScaleX = aTMatrix->GetElement(0,0);
  const double aScaleY = aTMatrix->GetElement(1,1);
  const double aScaleZ = aTMatrix->GetElement(2,2);
  theScale[0] = aScaleX;
  theScale[1] = aScaleY;
  theScale[2] = aScaleZ;
}

int SALOME_Transform::IsIdentity(){ 
  //float* aScale = GetScale();
  //return (aScale[0] == 1.0 && aScale[1] == 1.0 && aScale[2] == 1.0);
  double aScale[3];
  this->GetMatrixScale(aScale);
  return (fabs(aScale[0] - 1.0) < EPS && 
	  fabs(aScale[1] - 1.0) < EPS && 
	  fabs(aScale[2] - 1.0) < EPS);
}
