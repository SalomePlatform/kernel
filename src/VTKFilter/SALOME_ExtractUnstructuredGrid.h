//  VISU CONVERTOR :
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
//  File   : SALOME_ExtractUnstructuredGrid.hxx
//  Author : Alexey PETROV
//  Module : VISU

#ifndef SALOME_ExtractUnstructuredGrid_HeaderFile
#define SALOME_ExtractUnstructuredGrid_HeaderFile

#include <vtkUnstructuredGridToUnstructuredGridFilter.h>

#include <set>
#include <map>
#include <vector>

class SALOME_ExtractUnstructuredGrid : public vtkUnstructuredGridToUnstructuredGridFilter{
public:
  vtkTypeMacro(SALOME_ExtractUnstructuredGrid,vtkUnstructuredGridToUnstructuredGridFilter);

  // Description:
  // Construct with all types of clipping turned off.
  static SALOME_ExtractUnstructuredGrid *New();

  enum EExtraction{ eCells, ePoints};
  void SetModeOfExtraction(EExtraction theExtractionMode){
    myExtractionMode = theExtractionMode; Modified();
  }
  EExtraction GetModeOfExtraction(){ return myExtractionMode;}

  enum EChanging{ ePassAll, eAdding, eRemoving};
  void SetModeOfChanging(EChanging theChangeMode){
    myChangeMode = theChangeMode; 
    Modified();
  }
  EChanging GetModeOfChanging(){ return myChangeMode;}

  // Remove the cell from the output
  void RegisterCell(vtkIdType theCellId);
  int IsCellsRegistered() { return !myCellIds.empty();}
  void ClearRegisteredCells() { 
    myCellIds.clear();
    Modified();
  }
  
  // Remove every cells with the type from the output
  void RegisterCellsWithType(vtkIdType theCellType);
  int IsCellsWithTypeRegistered() { return !myCellTypes.empty();}
  void ClearRegisteredCellsWithType() { 
    myCellTypes.clear();
    Modified();
  }

  // Do the filter do some real work
  int IsChanging() { return IsCellsRegistered() || IsCellsWithTypeRegistered();}

  // Do it keep the mapping between input's and output's UnstructuredGrid
  void SetStoreMapping(int theStoreMapping);
  int GetStoreMapping(){ return myStoreMapping;}

  vtkIdType GetInputId(int theOutId) const;
  vtkIdType GetOutputId(int theInId) const;

  typedef std::vector<vtkIdType> TVectorId;
  typedef std::map<vtkIdType,vtkIdType> TMapId;

protected:
  SALOME_ExtractUnstructuredGrid();
  ~SALOME_ExtractUnstructuredGrid();

  void Execute();

  EExtraction myExtractionMode;
  
  EChanging myChangeMode;
  typedef std::set<vtkIdType> TSetId;
  TSetId myCellIds;
  TSetId myCellTypes;

  bool myStoreMapping;
  TVectorId myOut2InId;
  TMapId myIn2OutId;

private:
  SALOME_ExtractUnstructuredGrid(const SALOME_ExtractUnstructuredGrid&);  // Not implemented.
  void operator=(const SALOME_ExtractUnstructuredGrid&);  // Not implemented.
};


#endif


