//  SALOME VTKViewer : build VTK viewer into Salome desktop
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
//  File   : VTKViewer_RectPicker.cxx
//  Author : Natalia KOPNOVA
//  Module : SALOME

#include <VTKViewer_RectPicker.h>

#include "vtkActor.h"
#include "vtkAssemblyNode.h"
#include "vtkAssemblyPath.h"
#include "vtkCamera.h"
#include "vtkCommand.h"
#include "vtkImageData.h"
#include "vtkLODProp3D.h"
#include "vtkMapper.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPoints.h"
#include "vtkProp3DCollection.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkTransform.h"
#include "vtkVertex.h"
#include "vtkVolume.h"
#include "vtkVolumeMapper.h"

using namespace std;

//----------------------------------------------------------------------------
vtkStandardNewMacro(VTKViewer_RectPicker);
//----------------------------------------------------------------------------

VTKViewer_RectPicker::VTKViewer_RectPicker()
{
}

//----------------------------------------------------------------------------
int VTKViewer_RectPicker::Pick(float selectionX1, float selectionY1, float selectionZ1,
			       float selectionX2, float selectionY2, float selectionZ2,
			       vtkRenderer *renderer)
{
  int k, i;
  vtkProp *prop;
  vtkCamera *camera;
  vtkAbstractMapper3D *mapper = NULL;
  float p1World[4][4], p2World[4][4], p1Mapper[4][4], p2Mapper[4][4];
  float c1[3], c2[3];
  int picked=0;
  int *winSize;
  float x, y, t, p;
  float *viewport;
  float cameraPos[4], cameraFP[4];
  float *displayCoords, *worldCoords;
  float pickPosition[4][3];
  double *clipRange;
  float ray[4][3], rayLength[4];
  int pickable;
  int LODId;
  float windowLowerLeft[4], windowUpperRight[4];
  float bounds[6], tol;
  float tF, tB;
  float hitPosition[3];
  float cameraDOP[3];
  
  //  Initialize picking process
  this->Initialize();
  this->Renderer = renderer;

  /* Selection point is not defined for the rectangle
  this->SelectionPoint[0] = 
  this->SelectionPoint[1] = 
  this->SelectionPoint[2] = 
  */

  // Invoke start pick method if defined
  this->InvokeEvent(vtkCommand::StartPickEvent,NULL);

  if ( renderer == NULL )
    {
    vtkErrorMacro(<<"Must specify renderer!");
    return 0;
    }

  // Get camera focal point and position. Convert to display (screen) 
  // coordinates. We need a depth value for z-buffer.
  //
  camera = renderer->GetActiveCamera();
  camera->GetPosition((float *)cameraPos); cameraPos[3] = 1.0;
  camera->GetFocalPoint((float *)cameraFP); cameraFP[3] = 1.0;

  renderer->SetWorldPoint(cameraFP);
  renderer->WorldToDisplay();
  displayCoords = renderer->GetDisplayPoint();
  selectionZ1 = selectionZ2 = displayCoords[2];

  // Convert the selection rectangle into world coordinates.
  //
  renderer->SetDisplayPoint(selectionX1, selectionY1, selectionZ1);
  renderer->DisplayToWorld();
  worldCoords = renderer->GetWorldPoint();
  if ( worldCoords[3] == 0.0 )
    {
    vtkErrorMacro(<<"Bad homogeneous coordinates");
    return 0;
    }
  for (i=0; i < 3; i++) 
    {
    pickPosition[0][i] = worldCoords[i] / worldCoords[3];
    }

  renderer->SetDisplayPoint(selectionX1, selectionY2, (selectionZ1+selectionZ2)/2);
  renderer->DisplayToWorld();
  worldCoords = renderer->GetWorldPoint();
  if ( worldCoords[3] == 0.0 )
    {
    vtkErrorMacro(<<"Bad homogeneous coordinates");
    return 0;
    }
  for (i=0; i < 3; i++) 
    {
    pickPosition[1][i] = worldCoords[i] / worldCoords[3];
    }
  
  renderer->SetDisplayPoint(selectionX2, selectionY2, selectionZ2);
  renderer->DisplayToWorld();
  worldCoords = renderer->GetWorldPoint();
  if ( worldCoords[3] == 0.0 )
    {
    vtkErrorMacro(<<"Bad homogeneous coordinates");
    return 0;
    }
  for (i=0; i < 3; i++) 
    {
    pickPosition[2][i] = worldCoords[i] / worldCoords[3];
    }

  renderer->SetDisplayPoint(selectionX2, selectionY1, (selectionZ1+selectionZ2)/2);
  renderer->DisplayToWorld();
  worldCoords = renderer->GetWorldPoint();
  if ( worldCoords[3] == 0.0 )
    {
    vtkErrorMacro(<<"Bad homogeneous coordinates");
    return 0;
    }
  for (i=0; i < 3; i++) 
    {
    pickPosition[3][i] = worldCoords[i] / worldCoords[3];
    }
  
  //  Compute the ray endpoints.  The ray is along the line running from
  //  the camera position to the selection point, starting where this line
  //  intersects the front clipping plane, and terminating where this
  //  line intersects the back clipping plane.
  for (k=0; k < 4; k++) {
    for (i=0; i<3; i++)
      {
	ray[k][i] = pickPosition[k][i] - cameraPos[i];
      }
  }
  for (i=0; i<3; i++)
    {
      cameraDOP[i] = cameraFP[i] - cameraPos[i];
    }

  vtkMath::Normalize(cameraDOP);

  for (k=0; k < 4; k++) {
    if (( rayLength[k] = vtkMath::Dot(cameraDOP,ray[k])) == 0.0 ) 
      {
	vtkWarningMacro("Cannot process points");
	return 0;
      } 
  }

  clipRange = camera->GetClippingRange();

  if ( camera->GetParallelProjection() )
    {
      for (k=0; k < 4; k++) {
	tF = clipRange[0] - rayLength[k];
	tB = clipRange[1] - rayLength[k];
	for (i=0; i<3; i++) 
	  {
	    p1World[k][i] = pickPosition[k][i] + tF*cameraDOP[i];
	    p2World[k][i] = pickPosition[k][i] + tB*cameraDOP[i];
	  }
	p1World[k][3] = p2World[k][3] = 1.0;
      }
    }
  else
    {
      for (k=0; k < 4; k++) {
	tF = clipRange[0] / rayLength[k];
	tB = clipRange[1] / rayLength[k];
	for (i=0; i<3; i++) 
	  {
	    p1World[k][i] = cameraPos[i] + tF*ray[k][i];
	    p2World[k][i] = cameraPos[i] + tB*ray[k][i];
	  }
	p1World[k][3] = p2World[k][3] = 1.0;
      }
    }

  // Compute the center points of ray rectangle
  for (i=0; i<3; i++) {
    c1[i] = c2[i] = 0;
    for (k=0; k<4; k++) {
      c1[i] += p1World[k][i];
      c2[i] += p2World[k][i];
    }
    c1[i] = c1[i]/4;
    c2[i] = c2[i]/4;
  }
  
  // Compute the tolerance in world coordinates.  Do this by
  // determining the world coordinates of the diagonal points of the
  // window, computing the width of the window in world coordinates, and 
  // multiplying by the tolerance.
  //
  viewport = renderer->GetViewport();
  winSize = renderer->GetRenderWindow()->GetSize();
  x = winSize[0] * viewport[0];
  y = winSize[1] * viewport[1];
  renderer->SetDisplayPoint(x, y, selectionZ1);
  renderer->DisplayToWorld();
  renderer->GetWorldPoint(windowLowerLeft);

  x = winSize[0] * viewport[2];
  y = winSize[1] * viewport[3];
  renderer->SetDisplayPoint(x, y, selectionZ2);
  renderer->DisplayToWorld();
  renderer->GetWorldPoint(windowUpperRight);

  for (tol=0.0,i=0; i<3; i++) 
    {
    tol += (windowUpperRight[i] - windowLowerLeft[i]) *
      (windowUpperRight[i] - windowLowerLeft[i]);
    }
  
  tol = sqrt (tol) * this->Tolerance;

  //  Loop over all props.  Transform ray (defined from position of
  //  camera to selection point) into coordinates of mapper (not
  //  transformed to actors coordinates!  Reduces overall computation!!!).
  //  Note that only vtkProp3D's can be picked by vtkPicker.
  //
  vtkPropCollection *props;
  vtkProp *propCandidate;
  if ( this->PickFromList ) 
    {
    props = this->GetPickList();
    }
  else 
    {
    props = renderer->GetProps();
    }

  vtkActor *actor;
  vtkLODProp3D *prop3D;
  vtkVolume *volume;
  vtkAssemblyPath *path;
  vtkProperty *tempProperty;
  this->Transform->PostMultiply();
  for ( props->InitTraversal(); (prop=props->GetNextProp()); )
    {
    for ( prop->InitPathTraversal(); (path=prop->GetNextPath()); )
      {
      pickable = 0;
      actor = NULL;
      propCandidate = path->GetLastNode()->GetProp();
      if ( propCandidate->GetPickable() && propCandidate->GetVisibility() )
        {
        pickable = 1;
        if ( (actor=vtkActor::SafeDownCast(propCandidate)) != NULL )
          {
          mapper = actor->GetMapper();
          if ( actor->GetProperty()->GetOpacity() <= 0.0 )
            {
            pickable = 0;
            }
          }
        else if ( (prop3D=vtkLODProp3D::SafeDownCast(propCandidate)) != NULL )
          {
          LODId = prop3D->GetPickLODID();
          mapper = prop3D->GetLODMapper(LODId);

          // if the mapper is a vtkMapper (as opposed to a vtkVolumeMapper), 
          // then check the transparency to see if the object is pickable
          if ( vtkMapper::SafeDownCast(mapper) != NULL)
            {
            prop3D->GetLODProperty(LODId, &tempProperty);
            if ( tempProperty->GetOpacity() <= 0.0 )
              {
              pickable = 0;
              }
            }
          }
        else if ( (volume=vtkVolume::SafeDownCast(propCandidate)) != NULL )
          {
          mapper = volume->GetMapper();
          }
        else
          {
          pickable = 0; //only vtkProp3D's (actors and volumes) can be picked
          }
        }
      //  If actor can be picked, get its composite matrix, invert it, and
      //  use the inverted matrix to transform the ray points into mapper
      //  coordinates. 
      if ( pickable  &&  mapper != NULL )
        {
        vtkMatrix4x4 *LastMatrix = path->GetLastNode()->GetMatrix();
        if (LastMatrix == NULL)
          {
          vtkErrorMacro (<< "Pick: Null matrix.");
          return 0;
          }
        this->Transform->SetMatrix(LastMatrix);
        this->Transform->Push();
        this->Transform->Inverse();

	for (k=0; k < 4; k++) {
	  this->Transform->TransformPoint(p1World[k],p1Mapper[k]);
	  this->Transform->TransformPoint(p2World[k],p2Mapper[k]);

	  for (i=0; i<3; i++) 
	    {
	      ray[k][i] = p2Mapper[k][i] - p1Mapper[k][i];
	    }
	}

        this->Transform->Pop();

        //  Have the ray endpoints in mapper space, now need to compare this
        //  with the mapper bounds to see whether intersection is possible.
        //
        //  Get the bounding box of the modeller.  Note that the tolerance is
        //  added to the bounding box to make sure things on the edge of the
        //  bounding box are picked correctly.
        mapper->GetBounds(bounds);
        bounds[0] -= tol; bounds[1] += tol; 
        bounds[2] -= tol; bounds[3] += tol; 
        bounds[4] -= tol; bounds[5] += tol; 
        if ( HitBBox(bounds, p1Mapper, ray) ) {
	  t = this->IntersectWithHex(p1Mapper, p2Mapper, tol, path, 
				     (vtkProp3D *)propCandidate, mapper);
	  if ( t >= 0.0 && t <= 1.0 /*t < VTK_LARGE_FLOAT*/ ) {
	    picked = 1;
	    this->Prop3Ds->AddItem((vtkProp3D *)prop);
	    this->PickedPositions->InsertNextPoint
	      ((1.0 - t)*c1[0] + t*c2[0],
	       (1.0 - t)*c1[1] + t*c2[1],
	       (1.0 - t)*c1[2] + t*c2[2]);
	    
	    // backwards compatibility: also add to this->Actors
	    if (actor) {
	      this->Actors->AddItem(actor);
	    }
	  }
	}

        }//if visible and pickable not transparent and has mapper
      }//for all parts
    }//for all actors
  
  // Invoke end pick method if defined
  this->InvokeEvent(vtkCommand::EndPickEvent,NULL);
  
  return picked;
}

#define SIDE_LEFT 0
#define SIDE_RIGHT 1
#define SIDE_MIDDLE 2

float GetParameterValue(float start, float end, float point)
{
  if (start == end) return -VTK_LARGE_FLOAT;
  return (point-start)/(end-start);
}

void GetPointCoord(const float start[3], const float end[3], float t, float point[3])
{
  int i;
  for (i = 0; i < 3; i++) {
    point[i] = start[i] + t*(end[i]-start[i]);
  }
}

char GetIntersectionPoint(const float start[3], const float end[3], 
			  const int& index, const float p, float point[3])
{
  float t = GetParameterValue(start[index], end[index], p);
  char result = 0;
  if (t >= 0.0 && t <= 1.0) {
    result = 1;
    GetPointCoord(start, end, t, point);
  }
  return result;
}

//----------------------------------------------------------------------------
char VTKViewer_RectPicker::HitBBox (float bounds[6], float origin[4][4], float dir[4][3])
{
  int i, j, k, n;
  float endray[4][3];

  for (k = 0; k < 4; k++) {
    for (i = 0; i < 3; i++) {
      endray[k][i] = origin[k][i] + dir[k][i];
    }
  }

  // Compute hex bounding box, center point and center direction
  float hbounds[6], center[3], ray[3];
  for (i = 0; i < 3; i++) {
    hbounds[2*i] = hbounds[2*i+1] = origin[0][i];
    center[i] = ray[i] = 0;
    for (k = 0; k < 4; k++) {
      center[i] += origin[k][i];
      ray[i] += endray[k][i];
      if (origin[k][i] < hbounds[2*i]) {
	hbounds[2*i] = origin[k][i];
      }
      else if (origin[k][i] > hbounds[2*i+1])
	hbounds[2*i+1] = origin[k][i];
      if (endray[k][i] < hbounds[2*i])
	hbounds[2*i] = endray[k][i];
      else if (endray[k][i] > hbounds[2*i+1])
	hbounds[2*i+1] = endray[k][i];
    }
    center[i] = center[i]/4;
    ray[i] = ray[i]/4;
    ray[i] = ray[i] - center[i];
  }

  // Check for intersection between bouning boxes
  for (i = 0; i < 3; i++) {
    if (bounds[2*i+1] < hbounds[2*i] || bounds[2*i] > hbounds[2*i+1])
      return 0;
  }

  // Check if one of the origin point lays inside bbox
  char inside;
  for (k = 0; k < 4; k++) {
    inside = 1;
    for (i = 0; i < 3; i++) {
      if (origin[k][i] < bounds[2*i] || origin[k][i] > bounds[2*i+1]) {
	inside = 0;
	break;
      }
    }
    if (inside) return 1;
  }

  // Find the closest coord plane for the center point
  char side[3];
  float coordPlane[3];
  inside = 1;
  for (i = 0; i < 3; i++) {
    if (center[i] < bounds[2*i]) {
      inside = 0;
      coordPlane[i] = bounds[2*i];
      side[i] = SIDE_LEFT;
    }
    else if (center[i] > bounds[2*i+1]) {
      inside = 0;
      coordPlane[i] = bounds[2*i+1];
      side[i] = SIDE_RIGHT;
    }
    else {
      coordPlane[i] = (ray[i]<0.0) ? bounds[2*i] : bounds[2*i+1];
      side[i] = SIDE_MIDDLE;
    }
  }
  if (inside) return 1;

  // Calculate parametric distances to the planes and find the max
  float maxT[3];
  int whichPlane = 0;
  char defined = 0;
  for (i = 0; i < 3; i++) {
    if (side[i] != SIDE_MIDDLE && ray[i] != 0.0) {
      maxT[i] = (coordPlane[i]-center[i])/ray[i];
      defined = 1;
    }
    else
      maxT[i] = -1.0;
  }
  for (i = 0; i < 3; i++) {
    if (maxT[whichPlane] < maxT[i])
      whichPlane = i;
  }

  // Check for intersection along the center ray
  float coord;
  if (maxT[whichPlane] <= 1.0 && maxT[whichPlane] >= 0.0) {
    inside = 1;
    for (i = 0; i < 3; i++) {
      if (i != whichPlane) {
	coord = center[i] + maxT[whichPlane]*ray[i];
	if (coord < bounds[2*i] || coord > bounds[2*i+1])
	  inside = 0;
      }
    }
    if (inside) return 1;
  }

  // Define the intersection plane
  if (!defined) {
    for (i = 0; i < 3; i++) {
      if (ray[i] != 0.0) {
	maxT[i] = (coordPlane[i]-center[i])/ray[i];
      }
      else 
	maxT[i] = VTK_LARGE_FLOAT;
    }
    for (i = 0; i < 3; i++) {
      if (maxT[whichPlane] > maxT[i])
	whichPlane = i;
    }
  }

  // Compute the intersection between hex and coord plane
  float t[4];
  for (k = 0; k < 4; k++) {
    if (dir[k][whichPlane] != 0.0) {
      t[k] = (coordPlane[whichPlane]-origin[k][whichPlane])/dir[k][whichPlane];
    }
    else {
      t[k] = VTK_LARGE_FLOAT;
    }
  }

  vtkPoints* aPoints = vtkPoints::New();
  float p[3], q[3], t1;
  for (k = 0; k < 4; k++) {
    n = (k+1)%4; // next point
    if (t[k] > 1.0) {
      if (t[n] < 1.0) {
	// find intersection point
	t1 = GetParameterValue(endray[k][whichPlane], endray[n][whichPlane], coordPlane[whichPlane]);
	if (t1 > 0.0 && t1 < 1.0) {
	  GetPointCoord(endray[k], endray[n], t1, p);
	  aPoints->InsertNextPoint(p[0], p[1], p[2]);
	}
      }
      if (t[n] < 0.0) {
	// find second intersection point
	t1 = GetParameterValue(origin[k][whichPlane], origin[n][whichPlane], coordPlane[whichPlane]);
	if (t1 > 0.0 && t1 < 1.0) {
	  GetPointCoord(origin[k], origin[n], t1, p);
	  aPoints->InsertNextPoint(p[0], p[1], p[2]);
	}
      }
    }
    else if (t[k] < 0.0) {
      if (t[n] > 0.0) {
	// find intersection point
	t1 = GetParameterValue(origin[k][whichPlane], origin[n][whichPlane], coordPlane[whichPlane]);
	if (t1 > 0.0 && t1 < 1.0) {
	  GetPointCoord(origin[k], origin[n], t1, p);
	  aPoints->InsertNextPoint(p[0], p[1], p[2]);
	}
      }
    }
    else {
      // find intersection point
      GetPointCoord(origin[k], endray[k], t[k], p);
      aPoints->InsertNextPoint(p[0], p[1], p[2]);

      if (t[n] < 0.0) {
	// find second intersection point
	t1 = GetParameterValue(origin[k][whichPlane], origin[n][whichPlane], coordPlane[whichPlane]);
	if (t1 > 0.0 && t1 < 1.0) {
	  GetPointCoord(origin[k], origin[n], t1, p);
	  aPoints->InsertNextPoint(p[0], p[1], p[2]);
	}
      }
      else if (t[n] > 1.0) {
	// find second intersection point
	t1 = GetParameterValue(endray[k][whichPlane], endray[n][whichPlane], coordPlane[whichPlane]);
	if (t1 > 0.0 && t1 < 1.0) {
	  GetPointCoord(endray[k], endray[n], t1, p);
	  aPoints->InsertNextPoint(p[0], p[1], p[2]);
	}
      }
    }
  }
  n = aPoints->GetNumberOfPoints();
  if (n == 0) {
    aPoints->Delete();
    return 0;
  }

  if (n == 1) {
    aPoints->GetPoint(0, p);
    inside = 1;
    for (i = 0; i < 3; i++) {
      if (i != whichPlane) {
	if (p[i] < bounds[2*i] || p[i] > bounds[2*i+1]) {
	  inside = 0; break;
	}
      }
    }
    aPoints->Delete();
    return inside;
  }

  // Analize intersection
  int nearPlane, boundPlane = -1;
  float boundCoord, boundMin, boundMax;
  char intersect = 0;
  for (k = 0; k < n; k++) {
    aPoints->GetPoint(k, p);
    j = k+1; if (j == n) j = 0;
    aPoints->GetPoint(j, q);
    inside = 1;
    nearPlane = 0;
    // if the point is inside bbox
    for (i = 0; i < 3; i++) {
      if (i != whichPlane) {
	if (p[i] < bounds[2*i]) {
	  side[i] = SIDE_LEFT;
	  maxT[i] = GetParameterValue(p[i], q[i], bounds[2*i]);
	  inside = 0; 
	}
	else if (p[i] > bounds[2*i+1]) {
	  side[i] = SIDE_RIGHT;
	  maxT[i] = GetParameterValue(p[i], q[i], bounds[2*i+1]);
	  inside = 0; 
	}
	else {
	  side[i] = SIDE_MIDDLE;
	  maxT[i] = -1.0;
	}
      }
      else maxT[i] = -1.0;
      if (maxT[i] > maxT[nearPlane]) nearPlane = i;
    }
    if (inside) break;
    // if segment intersects bbox
    if (maxT[nearPlane] >= 0.0 && maxT[nearPlane] <= 1.0) {
      for (i = 0; i < 3; i++) {
	if (i != whichPlane && i != nearPlane) {
	  coord = p[i] + maxT[nearPlane]*(q[i]-p[i]);
	  if (coord >= bounds[2*i] && coord <= bounds[2*i+1]) {
	    intersect = 1; break;
	  }
	}
      }
      // intersect with boundPlane
      if (boundPlane == -1) {
	boundCoord = p[nearPlane] + maxT[nearPlane]*(q[nearPlane]-p[nearPlane]);
	boundPlane = nearPlane;
	for (i = 0; i < 3; i++) {
	  if (i != whichPlane && i != boundPlane) {
	    coord = p[i] + maxT[nearPlane]*(q[i]-p[i]);
	    boundMin = boundMax = coord;
	  }
	}
      }
      else {
	t1 = GetParameterValue(p[boundPlane], q[boundPlane], boundCoord);
	if (t1 >= 0.0 && t1 <= 1.0) {
	  for (i = 0; i < 3; i++) {
	    if (i != whichPlane && i != boundPlane) {
	      coord = p[i] + t1*(q[i]-p[i]);
	      if (coord < boundMin) boundMin = coord;
	      if (coord > boundMax) boundMax = coord;
	    }
	  }
	}
      }
    }
    if (intersect) break;
  }
  aPoints->Delete();
  if (inside || intersect) {
    return 1;
  }

  inside = 1;
  for (i = 0; i < 3; i++) {
    if (i != whichPlane && i != boundPlane) {
      if (boundMin > bounds[2*i+1] || boundMax < bounds[2*i])
	inside = 0;
    }
  }

  return inside;
}

//----------------------------------------------------------------------------
char VTKViewer_RectPicker::PointInside (float p[3], float p1[4][4], float p2[4][4], float tol)
{
  int i, j, k;
  float t, coord[3];

  // Fix one coordinate (x, for example) and 
  // compute intersection with coordinate plane
  vtkPoints* aPoints = vtkPoints::New();
  int mode = 0;
  for (k = 0; k < 4; k++) {
    j = k+1; if (j == 4) j = 0;
    switch (mode) {
    case 0:
      if (GetIntersectionPoint(p1[k], p1[j], 0, p[0], coord)) {
	aPoints->InsertNextPoint(coord[0], coord[1], coord[2]);
	mode = 0;
      }
      if (GetIntersectionPoint(p1[k], p2[k], 0, p[0], coord)) {
	aPoints->InsertNextPoint(coord[0], coord[1], coord[2]);
	mode = 1;
      }
      if (GetIntersectionPoint(p2[k], p2[j], 0, p[0], coord)) {
	aPoints->InsertNextPoint(coord[0], coord[1], coord[2]);
	mode = 2;
      }
      /*
      if ((p1[k][0]-p[0])*(p2[k][0]-p[0]) <= 0) {
	t = GetParameterValue(p1[k][0], p2[k][0], p[0]);
	if (t >= 0.0 && t <= 1.0) {
	  GetPointCoord(p1[k], p2[k], t, coord);
	  aPoints->InsertNextPoint(coord[0], coord[1], coord[2]);
	}
      }
      */
      break;
    case 1:
      if (GetIntersectionPoint(p1[k], p2[k], 0, p[0], coord)) {
	aPoints->InsertNextPoint(coord[0], coord[1], coord[2]);
	mode = 1;
      }
      if (GetIntersectionPoint(p2[k], p2[j], 0, p[0], coord)) {
	aPoints->InsertNextPoint(coord[0], coord[1], coord[2]);
	mode = 2;
      }
      if (GetIntersectionPoint(p1[k], p1[j], 0, p[0], coord)) {
	aPoints->InsertNextPoint(coord[0], coord[1], coord[2]);
	mode = 0;
      }
      /*
      if ((p1[k][0]-p[0])*(p1[j][0]-p[0]) <= 0) {
	t = GetParameterValue(p1[k][0], p1[j][0], p[0]);
	if (t > 0.0 && t < 1.0) {
	  GetPointCoord(p1[k], p1[j], t, coord);
	  aPoints->InsertNextPoint(coord[0], coord[1], coord[2]);
	}
      }
      */
      break;
    case 2:
      if (GetIntersectionPoint(p2[k], p2[j], 0, p[0], coord)) {
	aPoints->InsertNextPoint(coord[0], coord[1], coord[2]);
	mode = 2;
      }
      if (GetIntersectionPoint(p1[k], p2[k], 0, p[0], coord)) {
	aPoints->InsertNextPoint(coord[0], coord[1], coord[2]);
	mode = 1;
      }
      if (GetIntersectionPoint(p1[k], p1[j], 0, p[0], coord)) {
	aPoints->InsertNextPoint(coord[0], coord[1], coord[2]);
	mode = 0;
      }
      /*
      if ((p2[k][0]-p[0])*(p2[j][0]-p[0]) <= 0) {
	t = GetParameterValue(p2[k][0], p2[j][0], p[0]);
	if (t > 0.0 && t < 1.0) {
	  GetPointCoord(p2[k], p2[j], t, coord);
	  aPoints->InsertNextPoint(coord[0], coord[1], coord[2]);
	}
      }
      */
      break;
    }
  }
  int n = aPoints->GetNumberOfPoints();
  //cout << "---> Points in X projection " << n << endl;
  if (n == 0) {
    aPoints->Delete();
    return 0;
  }

  // Fix the second coord and define bounds
  float zMin = VTK_LARGE_FLOAT, zMax = -VTK_LARGE_FLOAT, z, ncoord[3];
  char inside = 0;
  for (k = 0; k < n; k++) {
    aPoints->GetPoint(k, coord);
    //cout << "  P" << k << " (" << coord[0] << ", " << coord[1] << ", " << coord[2] << ")";
    j = k+1; if (j == n) j = 0;
    if (j == k) {
      if (p[1] == coord[1] && p[2] == coord[2]) {
	inside = 1;
      }
      break;
    }
    aPoints->GetPoint(j, ncoord);
    t = GetParameterValue(coord[1], ncoord[1], p[1]);
    if (t >= 0.0 && t <= 1) {
      z = coord[2] + t*(ncoord[2]-coord[2]);
      if (z < zMin) zMin = z;
      if (z > zMax) zMax = z;
    }
  }
  //cout << endl << " Zmin = " << zMin << ", Zmax = " << zMax  << endl;
  if (!inside) {
    if (p[2] <= (zMax+tol) && p[2] >= (zMin-tol))
      inside = 1;
  }
  
  aPoints->Delete();
  return inside;
}

//----------------------------------------------------------------------------
float VTKViewer_RectPicker::IntersectWithHex(float p1[4][4], float p2[4][4], float tol, 
					     vtkAssemblyPath *path, vtkProp3D *prop3D, 
					     vtkAbstractMapper3D *mapper)
{
  int i, k;
  float *center, p0[3], ray[3], rayFactor, t;

  // Get the data from the modeler
  //
  center = mapper->GetCenter();

  if (!PointInside(center, p1, p2)) {
    return 2.0;
  }

  //   Determine appropriate info
  //
  for (i = 0; i < 3; i++) {
    p0[i] = ray[i] = 0;
    for (k = 0; k < 4; k++) {
      p0[i] += p1[k][i];
      ray[i] += p2[k][i];
    }
    p0[i] = p0[i]/4;
    ray[i] = ray[i]/4;
    ray[i] = ray[i] - p0[i];
  }
  if (( rayFactor = vtkMath::Dot(ray,ray)) == 0.0 ) {
    vtkErrorMacro("Cannot process points");
    return 2.0;
  }

  // Project the center point onto the ray and determine its parametric value
  //
  t = (ray[0]*(center[0]-p0[0]) + ray[1]*(center[1]-p0[1])
       + ray[2]*(center[2]-p0[2])) / rayFactor;

  if ( t >= 0.0 && t <= 1.0 && t < this->GlobalTMin ) {
    this->MarkPicked(path, prop3D, mapper, t, center);
  }
  return t;
}
