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
//  File   : VTKViewer_CellRectPicker.h
//  Author : Natalia KOPNOVA
//  Module : SALOME

#ifndef __VTKViewer_CellRectPicker_h
#define __VTKViewer_CellRectPicker_h

#include "VTKViewer_RectPicker.h"
#include <map>
#include <vector>

typedef struct {
  vtkIdType cellId;
  int subId;
  float depth;
  float p1[3];
  float p2[3];
} VTKViewer_CellData;

typedef std::vector<VTKViewer_CellData> VTKViewer_CellDataSet;
typedef std::map<vtkActor*, VTKViewer_CellDataSet> VTKViewer_ActorDataMap;

class vtkCell;
class vtkGenericCell;
class vtkQuad;

class VTK_EXPORT VTKViewer_CellRectPicker : public VTKViewer_RectPicker
{
public:
  static VTKViewer_CellRectPicker *New();
  vtkTypeMacro(VTKViewer_CellRectPicker,VTKViewer_RectPicker);

  // Description:
  // Get the id of the picked cells. 
  // (Notes: use GetActors() to get picked actors list)
  VTKViewer_CellDataSet GetCellData(vtkActor* actor) 
    { return this->ActorData[actor]; }

  // Description:
  // The method returns a non-zero value if the cells intersect each other 
  // in the direction dir[3]. Returned value is 1 if the first cell is top of 
  // the second one according to direction and 2 if the second cell is top.
  static int IntersectCells(vtkCell* c1, float d1, vtkCell* c2, float d2, float dir[3]);

  // Description:
  // The method returns a non-zero value if the point is inlide point data set
  static char PointInside(float point[3], vtkPoints* list);

protected:
  VTKViewer_CellRectPicker();
  ~VTKViewer_CellRectPicker();

  VTKViewer_ActorDataMap ActorData;

  virtual float IntersectWithHex(float p1[4][4], float p2[4][4], float tol, 
                          vtkAssemblyPath *path, vtkProp3D *p, 
                          vtkAbstractMapper3D *m);
  void Initialize();

  void IncludeCell(vtkDataSet* input, VTKViewer_CellData cellData, VTKViewer_CellDataSet& dalaList);
  void IncludeActor(vtkProp3D* prop, vtkDataSet* input, VTKViewer_CellDataSet& dataList);

private:
  vtkGenericCell *Cell;
  vtkQuad* Quad1;
  vtkQuad* Quad2;
};

#endif


