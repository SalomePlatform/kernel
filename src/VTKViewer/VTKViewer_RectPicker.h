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
//  File   : VTKViewer_RectPicker.h
//  Author : Natalia KOPNOVA
//  Module : SALOME

#ifndef __VTKViewer_RectPicker_h
#define __VTKViewer_RectPicker_h

#include <vtkPicker.h>

class VTK_EXPORT VTKViewer_RectPicker : public vtkPicker
{
public:
  static VTKViewer_RectPicker *New();
  vtkTypeMacro(VTKViewer_RectPicker,vtkPicker);
  
  // Description:
  // Perform pick operation with selection rectangle provided. Normally the 
  // first two values for the selection top-left and right-bottom points are 
  // x-y pixel coordinate, and the third value is =0. 
  // Return non-zero if something was successfully picked.
  virtual int Pick(float selectionX1, float selectionY1, float selectionZ1, 
		   float selectionX2, float selectionY2, float selectionZ2,
                   vtkRenderer *renderer);  

  // Description: 
  // Perform pick operation with selection rectangle provided. Normally the first
  // two values for the selection top-left and right-bottom points are x-y pixel 
  // coordinate, and the third value is =0. 
  // Return non-zero if something was successfully picked.
  int Pick(float selectionPt1[3], float selectionPt2[3], vtkRenderer *ren)
    {return this->Pick(selectionPt1[0], selectionPt1[1], selectionPt1[2], 
		       selectionPt2[0], selectionPt2[1], selectionPt2[2],
		       ren);};

  // Description:
  // Bounding box intersection with hexahedron. The method returns a non-zero value 
  // if the bounding box is hit. Origin[4][4] starts the ray from corner points, 
  // dir[4][3] is the vector components of the ray in the x-y-z directions. 
  // (Notes: the intersection ray dir[4][3] is NOT normalized.)
  static char HitBBox(float bounds[6], float origin[4][4], float dir[4][3]);

  // Description:
  // Position of point relative to hexahedron. The method returns a non-zero value 
  // if the point is inside. p1[4][4] is the corner points of top face, 
  // p2[4][4] is the corner points of bottom face. 
  static char PointInside(float point[3], float p1[4][4], float p2[4][4], float tol=0);

protected:
  VTKViewer_RectPicker();
  ~VTKViewer_RectPicker() {};

  virtual float IntersectWithHex(float p1[4][4], float p2[4][4], float tol, 
                          vtkAssemblyPath *path, vtkProp3D *p, 
                          vtkAbstractMapper3D *m);

private:
};

#endif


