//  SALOME OBJECT : kernel of SALOME component
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
//  File   : SALOME_GeometryFilter.h
//  Author : Michael ZORIN
//  Module : SALOME
//  $Header$


#ifndef __SALOME_GeometryFilter_h
#define __SALOME_GeometryFilter_h

#include <vtkGeometryFilter.h>
#include <vector>

class SALOME_GeometryFilter : public vtkGeometryFilter 
{
public:
  static SALOME_GeometryFilter *New();
  vtkTypeRevisionMacro(SALOME_GeometryFilter, vtkGeometryFilter);
  void SetInside(int theShowInside);
  int GetInside();

  void SetStoreMapping(int theStoreMapping);
  int GetStoreMapping(){ return myStoreMapping;}

  typedef std::vector<vtkIdType> TVectorId;
  virtual vtkIdType GetObjId(int theVtkID);
  virtual TVectorId GetVtkId(int theObjID);

protected:
  SALOME_GeometryFilter();
  ~SALOME_GeometryFilter();
  
  void Execute();
  void UnstructuredGridExecute();
    
private:
  int myShowInside;   

  int myStoreMapping;   
  TVectorId myVTK2ObjIds;
  typedef std::vector<TVectorId> TVecVectorId;
  TVecVectorId myObj2VTKIds;
};

#endif
