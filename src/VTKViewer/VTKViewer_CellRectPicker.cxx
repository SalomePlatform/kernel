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
//  File   : VTKViewer_CellRectPicker.cxx
//  Author : Natalia KOPNOVA
//  Module : SALOME

#include <VTKViewer_CellRectPicker.h>

#include "vtkGenericCell.h"
#include "vtkAssemblyNode.h"
#include "vtkAssemblyPath.h"
#include "vtkObjectFactory.h"
#include "vtkImageData.h"
#include "vtkMapper.h"
#include "vtkVolumeMapper.h"
#include "vtkMath.h"
#include "vtkLine.h"
#include "vtkQuad.h"
#include "vtkPoints.h"
#include "vtkPlane.h"
#include "vtkActor.h"

using namespace std;

//----------------------------------------------------------------------------
vtkStandardNewMacro(VTKViewer_CellRectPicker);
//----------------------------------------------------------------------------

VTKViewer_CellRectPicker::VTKViewer_CellRectPicker()
{
  this->Cell = vtkGenericCell::New();
  this->Quad1 = vtkQuad::New();
  this->Quad2 = vtkQuad::New();
}

VTKViewer_CellRectPicker::~VTKViewer_CellRectPicker()
{
  this->Cell->Delete();
  this->Quad1->Delete();
  this->Quad2->Delete();
}

void VTKViewer_CellRectPicker::Initialize()
{
  ActorData.clear();
  this->VTKViewer_RectPicker::Initialize();
}

//----------------------------------------------------------------------------
float VTKViewer_CellRectPicker::IntersectWithHex(float p1[4][4], float p2[4][4], float tol, 
						 vtkAssemblyPath *path, vtkProp3D *prop3D, 
						 vtkAbstractMapper3D *m)
{
  vtkDataSet *input;
  vtkMapper *mapper;
  vtkVolumeMapper *volumeMapper;

  // Get the underlying dataset
  if ( (mapper=vtkMapper::SafeDownCast(m)) != NULL ) {
    input = mapper->GetInput();
  }
  else if ( (volumeMapper=vtkVolumeMapper::SafeDownCast(m)) != NULL ) {
    input = volumeMapper->GetInput();
  }
  else {
    return VTK_LARGE_FLOAT;
  }

  vtkIdType numCells;
  if ( (numCells = input->GetNumberOfCells()) < 1 ) {
    return 2.0;
  }

  int i, k, subId;
  float bounds[6], center[3], cp1[3], cp2[3];
  float pcoord[3], weight[4], dist;
  float t1[2], t2[2], x[3], t;

  for (k = 0; k < 4; k++) {
    this->Quad1->PointIds->SetId(k, k);
    this->Quad1->Points->SetPoint(k, p1[k]);
    this->Quad2->PointIds->SetId(k, k);
    this->Quad2->Points->SetPoint(k, p2[k]);
  }

  /*
  cout << "---> Selection area:"   << endl;
  for (k = 0; k < 4; k++)
    cout << "\t(" << p1[k][0] << ", " << p1[k][1] << ", " << p1[k][2] << ")";
  cout << endl;
  for (k = 0; k < 4; k++)
    cout << "\t(" << p2[k][0] << ", " << p2[k][1] << ", " << p2[k][2] << ")";
  cout << endl;
  */

  vtkIdType cellId;
  VTKViewer_CellData cellData;
  VTKViewer_CellDataSet dataList;

  char inside; int n; float *point;
  float tMin = VTK_LARGE_FLOAT, xMin[3];
  for (cellId = 0; cellId < numCells; cellId++) {
    input->GetCell(cellId, this->Cell);

    this->Cell->GetBounds(bounds);
    for (i = 0; i < 3; i++)
      center[i] = (bounds[2*i]+bounds[2*i+1])/2;

    /*
    if (!VTKViewer_RectPicker::PointInside(center, p1, p2, 0.01))
      continue;
    */

    inside = 1;
    n = this->Cell->Points->GetNumberOfPoints();
    //cout << "---> Info cell " << cellId << " (" << n << "): " << endl;
    for (k = 0; k < n; k++) {
      point = this->Cell->Points->GetPoint(k);
      //cout << "  P (" << point[0] << ", " << point[1] << ", " << point[2] << ")";
      if (!VTKViewer_RectPicker::PointInside(point, p1, p2)) {
	inside = 0; break;
      }
    }
    //cout << endl;
    if (!inside) continue;

    //cout << "---> Inside cell " << cellId << endl;

    this->Quad1->EvaluatePosition(center, 0, subId, pcoord, dist, weight);
    t1[0] = pcoord[0]; t1[1] = pcoord[1];
    this->Quad2->EvaluatePosition(center, 0, subId, pcoord, dist, weight);
    t2[0] = pcoord[0]; t2[1] = pcoord[1];

    pcoord[0] = (t1[0]+t2[0])/2; pcoord[1] = (t1[1]+t2[1])/2; pcoord[2] = 0;

    this->Quad1->EvaluateLocation(subId, pcoord, cp1, weight);
    this->Quad2->EvaluateLocation(subId, pcoord, cp2, weight);

    if (this->Cell->IntersectWithLine(cp1, cp2, tol, t, x, pcoord, subId)) {
      cellData.cellId = cellId;
      cellData.subId = subId;
      cellData.depth = t;
      for (i = 0; i < 3; i++) {
	cellData.p1[i] = cp1[i];
	cellData.p2[i] = cp2[i];
      }
      /*
      cout << "---> Include cell " << cellId << ", depth = " << t   << endl;
      cout << "     P1 = (" << cp1[0] << ", " << cp1[1] << ", " << cp1[2] << ")"  << endl;
      cout << "     P2 = (" << cp2[0] << ", " << cp2[1] << ", " << cp2[2] << ")"  << endl;
      */
      IncludeCell(input, cellData, dataList);
      if (t < tMin) {
	tMin = t;
	for (i = 0; i < 3; i++)
	  xMin[i] = x[i];
      }
    }
  }

  if (!dataList.empty()) {
    // compare against other actors
    IncludeActor(prop3D, input, dataList);
    if (tMin < this->GlobalTMin) {
      this->MarkPicked(path, prop3D, m, tMin, xMin);
    }
  }

  return tMin;
}


//----------------------------------------------------------------------------
void VTKViewer_CellRectPicker::IncludeCell(vtkDataSet* input, 
					   VTKViewer_CellData cellData, 
					   VTKViewer_CellDataSet& dataList)
{
  vtkGenericCell* cell1 = vtkGenericCell::New();
  vtkGenericCell* cell2 = vtkGenericCell::New();
  vtkCell* sub1 = 0;
  vtkCell* sub2 = 0;
  
  input->GetCell(cellData.cellId, cell1);
  if (cell1->GetNumberOfFaces() > 0)
    sub1 = cell1->GetFace(cellData.subId);

  int i, result;
  float p1[3], p2[3], dir[3];
  char add = 1;

  VTKViewer_CellData curData;
  VTKViewer_CellDataSet::iterator it;
  for (it = dataList.begin(); it != dataList.end();) {
    curData = *it;
    for (i = 0; i < 3; i++) {
      p1[i] = (cellData.p1[i]+curData.p1[i])/2;
      p2[i] = (cellData.p2[i]+curData.p2[i])/2;
      dir[i] = p2[i] - p1[i];
    }

    input->GetCell(curData.cellId, cell2);
    sub2 = 0;
    if (cell2->GetNumberOfFaces() > 0)
      sub2 = cell2->GetFace(curData.subId);

    if (sub1) {
      if (sub2)
	result = IntersectCells(sub1, cellData.depth, 
				sub2, curData.depth, dir);
      else 
	result = IntersectCells(sub1, cellData.depth, 
				cell2, curData.depth, dir);
    }
    else {
      if (sub2)
	result = IntersectCells(cell1, cellData.depth, 
				sub2, curData.depth, dir);
      else
	result = IntersectCells(cell1, cellData.depth, 
				cell2, curData.depth, dir);
    }
    /*
    cout << "     Direction = (" << dir[0] << ", " << dir[1] << ", " << dir[2] << ")";
    cout << "     depth = " << cellData.depth << ", " << curData.depth << ", " << result << endl;
    */
    if (result > 0) {
      if (result == 1) {
	it = dataList.erase(it);
	continue;
      } 
      else {
	add = 0;
	break;
      }
    }
    ++it;
  }
  if (add) {
    //cout << "     add "   << endl;
    dataList.push_back(cellData);
  }

  cell1->Delete();
  cell2->Delete();
}

//----------------------------------------------------------------------------
void VTKViewer_CellRectPicker::IncludeActor(vtkProp3D* prop, 
					    vtkDataSet* input, 
					    VTKViewer_CellDataSet& dataList)
{
  vtkGenericCell* cell1 = vtkGenericCell::New();
  vtkGenericCell* cell2 = vtkGenericCell::New();

  int i, result;
  float p1[3], p2[3], dir[3];
  char removed;

  VTKViewer_CellDataSet actorData;
  VTKViewer_CellData curData, cellData;
  VTKViewer_CellDataSet::iterator it1, it2;
  VTKViewer_ActorDataMap::iterator it;

  for (it1 = dataList.begin(); it1 != dataList.end();) {
    cellData = *it1;
    input->GetCell(cellData.cellId, cell1);
    removed = 0;

    for (it = ActorData.begin(); it != ActorData.end(); ++it) {
      vtkActor* actor = (*it).first;
      actorData = (*it).second;

      for (it2 = actorData.begin(); it2 != actorData.end();) {
	curData = *it2;
	for (i = 0; i < 3; i++) {
	  p1[i] = (cellData.p1[i]+curData.p1[i])/2;
	  p2[i] = (cellData.p2[i]+curData.p2[i])/2;
	  dir[i] = p2[i] - p1[i];
	}

	actor->GetMapper()->GetInput()->GetCell(curData.cellId, cell2);
	result = IntersectCells(cell1, cellData.depth,
				cell2, curData.depth, dir);
	if (result > 0) {
	  if (result == 1) {
	    it2 = actorData.erase(it2);
	    continue;
	  }
	  else {
	    removed = 1;
	    it1 = dataList.erase(it1);
	    break;
	  }
	  break;
	}
	++it2;
      }
      if (removed) break;
    } // by actors
    if (!removed) ++it1;
  }

  if (!dataList.empty()) {
    vtkActor* actor;
    if ((actor = vtkActor::SafeDownCast(prop)) != NULL)
      ActorData[actor] = dataList;
  }

  cell1->Delete();
  cell2->Delete();
}

int VTKViewer_CellRectPicker::IntersectCells(vtkCell* c1, float d1, vtkCell* c2, float d2, float dir[3])
{
  int i, k;
  float *orig, cp[3], bounds[6];
  vtkCell* cell = 0;
  vtkCell* tcell = 0;

  // define the central point and cell for projection
  if (fabs(d1-d2) < 1.e-7) return 0;

  if (d1 < d2) {
    orig = c1->Points->GetPoint(0);
    cell = c1;
    tcell = c2;
    c2->GetBounds(bounds);
    for (i = 0; i < 3; i++)
      cp[i] = (bounds[2*i] + bounds[2*i+1])/2;
  }
  else if (d1 > d2) {
    orig = c2->Points->GetPoint(0);
    cell = c2;
    tcell = c1;
    c1->GetBounds(bounds);
    for (i = 0; i < 3; i++)
      cp[i] = (bounds[2*i] + bounds[2*i+1])/2;
  }

  float proj[3];
  vtkPlane::GeneralizedProjectPoint(cp, orig, dir, proj);

  float *p, pp[3];
  vtkPoints* aPoints = vtkPoints::New();
  for (k = 0; k < cell->Points->GetNumberOfPoints(); k++) {
    p = cell->Points->GetPoint(k);
    vtkPlane::GeneralizedProjectPoint(p, orig, dir, pp);
    aPoints->InsertNextPoint(pp[0], pp[1], pp[2]);
  }

  int result = 0;
  if (PointInside(proj, aPoints)) {
    result = (d1<d2) ? 1 : 2;
  }

  if (result == 0) {
    // test for boundary cells
    for (k = 0; k < tcell->Points->GetNumberOfPoints(); k++) {
      p = tcell->Points->GetPoint(k);
      vtkPlane::GeneralizedProjectPoint(p, orig, dir, pp);
      if (PointInside(pp, aPoints)) {
	result = 1; break;
      }
    }
    if (result && d2<d1) result = 2;
  }

  aPoints->Delete();
  return result;
}

char VTKViewer_CellRectPicker::PointInside(float point[3], vtkPoints* list)
{
  int i, k, l, n;
  float x[3], *a1, *a2, v[3], p[3], q[3];

  n = list->GetNumberOfPoints();
  if (n < 3) return 0;

  for (k = 0; k < n; k++) {
    l = k+1; if (l == n) l = 0;
    a1 = list->GetPoint(k);
    a2 = list->GetPoint(l);

    // compute vectors
    for (i = 0; i < 3; i++) {
      x[i] = point[i] - a1[i];
      v[i] = a2[i]-a1[i];
    }

    // check if point coincides with vertex
    if (x[0]*x[0] + x[1]*x[1] + x[2]*x[2] < 1.e-7) return 0;

    // compute normal
    vtkMath::Cross(x, v, p);

    // check if point lays on edge
    if (p[0]*p[0] + p[1]*p[1] + p[2]*p[2] < 1.e-7) return 0;

    // check if normals are in the same direction
    if (k > 0) {
      if (vtkMath::Dot(p, q) < 0.0) return 0;
    }

    // remember normal direction
    for (i = 0; i < 3; i++) {
      q[i] = p[i];
    }
  }

  return 1;
}
