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
//  File   : VTKViewer_Utilities.cxx
//  Author : Alexey PETROV
//  Module : SALOME
//  $Header: 

#include "VTKViewer_Utilities.h"
#include "utilities.h"
#include "SALOME_Actor.h"

// VTK Includes
#include <vtkMath.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

using namespace std;

//see vtkRenderer::ResetCamera(float bounds[6]) method
void ResetCamera(vtkRenderer* theRenderer, int theUsingZeroFocalPoint){  
  if(!theRenderer) return;
  float bounds[6];
  int aCount = ComputeVisiblePropBounds(theRenderer,bounds);
  if(theUsingZeroFocalPoint || aCount){
    float aLength = bounds[1]-bounds[0];
    aLength = max((bounds[3]-bounds[2]),aLength);
    aLength = max((bounds[5]-bounds[4]),aLength);
    
    double vn[3];
    if ( theRenderer->GetActiveCamera() != NULL )
      theRenderer->GetActiveCamera()->GetViewPlaneNormal(vn);
    else{
      MESSAGE("Trying to reset non-existant camera");
      return;
    }
    
    float center[3] = {0.0, 0.0, 0.0};
    if(!theUsingZeroFocalPoint){
      center[0] = (bounds[0] + bounds[1])/2.0;
      center[1] = (bounds[2] + bounds[3])/2.0;
      center[2] = (bounds[4] + bounds[5])/2.0;
    }
    theRenderer->GetActiveCamera()->SetFocalPoint(center[0],center[1],center[2]);
    
    float width = sqrt((bounds[1]-bounds[0])*(bounds[1]-bounds[0]) +
		       (bounds[3]-bounds[2])*(bounds[3]-bounds[2]) +
		       (bounds[5]-bounds[4])*(bounds[5]-bounds[4]));
    
    double ang = theRenderer->GetActiveCamera()->GetViewAngle();
    float distance = 2.0*width/tan(ang*vtkMath::Pi()/360.0);
    
    // check view-up vector against view plane normal
    double *vup = theRenderer->GetActiveCamera()->GetViewUp();
    if ( fabs(vtkMath::Dot(vup,vn)) > 0.999 ){
      MESSAGE("Resetting view-up since view plane normal is parallel");
      theRenderer->GetActiveCamera()->SetViewUp(-vup[2], vup[0], vup[1]);
    }
    
    // update the camera
    theRenderer->GetActiveCamera()->SetPosition(center[0]+distance*vn[0],
						center[1]+distance*vn[1],
						center[2]+distance*vn[2]);
    // find size of the window
    int* winsize = theRenderer->GetSize();
    if(winsize[0] < winsize[1]) width *= float(winsize[1])/float(winsize[0]);
    
    if(theUsingZeroFocalPoint) width *= sqrt(2.0);
    
    theRenderer->GetActiveCamera()->SetParallelScale(width/2.0);
  }
  //workaround on VTK
  //theRenderer->ResetCameraClippingRange(bounds);
  ResetCameraClippingRange(theRenderer);
}


// Compute the bounds of the visible props
int ComputeVisiblePropBounds(vtkRenderer* theRenderer, float theBounds[6]){
  vtkProp    *prop;
  float      *bounds;
  int        aCount=0;

  theBounds[0] = theBounds[2] = theBounds[4] = VTK_LARGE_FLOAT;
  theBounds[1] = theBounds[3] = theBounds[5] = -VTK_LARGE_FLOAT;
  
  // loop through all props
  vtkActorCollection* aCollection = theRenderer->GetActors();
  aCollection->InitTraversal();
  while (vtkActor* prop = aCollection->GetNextActor()) {
    // if it's invisible, or has no geometry, we can skip the rest 
    if ( prop->GetVisibility() )
      {
      if(SALOME_Actor* anActor = SALOME_Actor::SafeDownCast(prop))
	if(anActor->IsInfinitive()) continue;
      bounds = prop->GetBounds();
      // make sure we haven't got bogus bounds
      if ( bounds != NULL &&
           bounds[0] > -VTK_LARGE_FLOAT && bounds[1] < VTK_LARGE_FLOAT &&
           bounds[2] > -VTK_LARGE_FLOAT && bounds[3] < VTK_LARGE_FLOAT &&
           bounds[4] > -VTK_LARGE_FLOAT && bounds[5] < VTK_LARGE_FLOAT )
        {
        aCount++;

        if (bounds[0] < theBounds[0])
          {
          theBounds[0] = bounds[0]; 
          }
        if (bounds[1] > theBounds[1])
          {
          theBounds[1] = bounds[1]; 
          }
        if (bounds[2] < theBounds[2])
          {
          theBounds[2] = bounds[2]; 
          }
        if (bounds[3] > theBounds[3])
          {
          theBounds[3] = bounds[3]; 
          }
        if (bounds[4] < theBounds[4])
          {
          theBounds[4] = bounds[4]; 
          }
        if (bounds[5] > theBounds[5])
          {
          theBounds[5] = bounds[5]; 
          }
        }//not bogus
      }
    }
  return aCount;
}


//see vtkRenderer::ResetCameraClippingRange(float bounds[6]) method
void ResetCameraClippingRange(vtkRenderer* theRenderer){
  if(!theRenderer || !theRenderer->VisibleActorCount()) return;

  vtkCamera* anActiveCamera = theRenderer->GetActiveCamera();
  if( anActiveCamera == NULL ){
    MESSAGE("Trying to reset clipping range of non-existant camera");
    return;
  }
  
  // Find the plane equation for the camera view plane
  double vn[3];
  anActiveCamera->GetViewPlaneNormal(vn);
  double  position[3];
  anActiveCamera->GetPosition(position);

  float bounds[6];
  theRenderer->ComputeVisiblePropBounds(bounds);

  double center[3];
  center[0] = (bounds[0] + bounds[1])/2.0;
  center[1] = (bounds[2] + bounds[3])/2.0;
  center[2] = (bounds[4] + bounds[5])/2.0;
  
  double width = sqrt((bounds[1]-bounds[0])*(bounds[1]-bounds[0]) +
		     (bounds[3]-bounds[2])*(bounds[3]-bounds[2]) +
		     (bounds[5]-bounds[4])*(bounds[5]-bounds[4]));

  double distance = sqrt((position[0]-center[0])*(position[0]-center[0]) +
			 (position[1]-center[1])*(position[1]-center[1]) +
			 (position[2]-center[2])*(position[2]-center[2]));

  float range[2] = {distance - width/2.0, distance + width/2.0};

  // Do not let the range behind the camera throw off the calculation.
  if (range[0] < 0.0) range[0] = 0.0;

  anActiveCamera->SetClippingRange( range );
}
