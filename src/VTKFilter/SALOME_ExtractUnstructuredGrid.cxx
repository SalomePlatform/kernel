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
// File:    VISU_ExtractUnstructuredGrid.cxx
// Author:  Alexey PETROV
// Module : VISU


#include "SALOME_ExtractUnstructuredGrid.h"
#include "utilities.h"

#include <vtkUnsignedCharArray.h>
#include <vtkUnstructuredGrid.h>
#include <vtkObjectFactory.h>
#include <vtkCellArray.h>
#include <vtkIntArray.h>
#include <vtkIdList.h>
#include <vtkCell.h>

using namespace std;

#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif

#if defined __GNUC__
  #if __GNUC__ == 2
    #define __GNUC_2__
  #endif
#endif

vtkStandardNewMacro(SALOME_ExtractUnstructuredGrid);


SALOME_ExtractUnstructuredGrid::SALOME_ExtractUnstructuredGrid():
  myExtractionMode(eCells), myChangeMode(ePassAll)
{}


SALOME_ExtractUnstructuredGrid::~SALOME_ExtractUnstructuredGrid(){}


void SALOME_ExtractUnstructuredGrid::RegisterCell(vtkIdType theCellId){
  if(0 && MYDEBUG) MESSAGE("RegisterCell - theCellId = "<<theCellId);
  myCellIds.insert(theCellId);
  Modified();
}


void SALOME_ExtractUnstructuredGrid::RegisterCellsWithType(vtkIdType theCellType){
  if(0 && MYDEBUG) MESSAGE("RegisterCellsWithType - theCellType = "<<theCellType);
  myCellTypes.insert(theCellType);
  Modified();
}


void SALOME_ExtractUnstructuredGrid::SetStoreMapping(int theStoreMapping){
  myStoreMapping = theStoreMapping;
  this->Modified();
}

vtkIdType SALOME_ExtractUnstructuredGrid::GetInputId(int theOutId) const{
  if(myCellIds.empty() && myCellTypes.empty()) return theOutId;
  if(myOut2InId.empty() || theOutId > myOut2InId.size()) return -1;
#if defined __GNUC_2__
  return myOut2InId[theOutId];
#else
  return myOut2InId.at(theOutId);
#endif
}

vtkIdType SALOME_ExtractUnstructuredGrid::GetOutputId(int theInId) const{
  if(myCellIds.empty() && myCellTypes.empty()) return theInId;
  TMapId::const_iterator anIter = myIn2OutId.find(theInId);
  if(anIter == myIn2OutId.end()) return -1;
  return anIter->second;
}


inline void InsertCell(vtkUnstructuredGrid *theInput,
		       vtkCellArray *theConnectivity, 
		       vtkUnsignedCharArray* theCellTypesArray,
		       vtkIdType theCellId, 
		       vtkIdList *theIdList,
		       bool theStoreMapping,
		       vtkIdType theOutId, 
		       SALOME_ExtractUnstructuredGrid::TVectorId& theOut2InId,
		       SALOME_ExtractUnstructuredGrid::TMapId& theIn2OutId)
{
  vtkCell *aCell = theInput->GetCell(theCellId);
  vtkIdList *aPntIds = aCell->GetPointIds();
  vtkIdType aNbIds = aPntIds->GetNumberOfIds();
  theIdList->SetNumberOfIds(aNbIds);
  for(vtkIdType i = 0; i < aNbIds; i++){
    theIdList->SetId(i,aPntIds->GetId(i));
  }
  theConnectivity->InsertNextCell(theIdList);

  vtkIdType aCellType = aCell->GetCellType();
  theCellTypesArray->InsertNextValue(aCellType);
  if(theStoreMapping){
    theOut2InId.push_back(theCellId);
    theIn2OutId[theCellId] = theOutId;
  }
}

inline void InsertPointCell(vtkCellArray *theConnectivity, 
			    vtkUnsignedCharArray* theCellTypesArray,
			    vtkIdType theCellId, 
			    vtkIdList *theIdList,
			    bool theStoreMapping,
			    vtkIdType theOutId, 
			    SALOME_ExtractUnstructuredGrid::TVectorId& theOut2InId,
			    SALOME_ExtractUnstructuredGrid::TMapId& theIn2OutId)
{
  theIdList->SetId(0,theCellId);
  theConnectivity->InsertNextCell(theIdList);
  theCellTypesArray->InsertNextValue(VTK_VERTEX);
  if(theStoreMapping){
    theOut2InId.push_back(theCellId);
    theIn2OutId[theCellId] = theOutId;
  }
}

void SALOME_ExtractUnstructuredGrid::Execute(){
  vtkUnstructuredGrid *anInput = this->GetInput();
  vtkUnstructuredGrid *anOutput = this->GetOutput();
  myOut2InId.clear();  myIn2OutId.clear();

  if(MYDEBUG){
    MESSAGE("Execute - anInput->GetNumberOfCells() = "<<anInput->GetNumberOfCells());
    MESSAGE("Execute - myCellTypes.size() = "<<myCellTypes.size());
    MESSAGE("Execute - myCellIds.size() = "<<myCellIds.size());
    MESSAGE("Execute - myExtractionMode = "<<myExtractionMode);
    MESSAGE("Execute - myChangeMode = "<<myChangeMode);
  }
  if(myExtractionMode == eCells){
    if(myChangeMode == ePassAll || myCellIds.empty() && myCellTypes.empty() && myChangeMode == eRemoving){
      if(vtkIdType aNbElems = anInput->GetNumberOfCells()){
	if(myStoreMapping) myOut2InId.reserve(aNbElems);
	anOutput->ShallowCopy(anInput);
	for(vtkIdType aCellId = 0, anOutId = 0; aCellId < aNbElems; aCellId++,anOutId++){
	  if(myStoreMapping){
	    myOut2InId.push_back(aCellId);
	    myIn2OutId[aCellId] = anOutId;
	  }
	}
      }
    }else{
      vtkIdList *anIdList = vtkIdList::New();
      vtkCellArray *aConnectivity = vtkCellArray::New();
      vtkIdType aNbElems = anInput->GetNumberOfCells();
      aConnectivity->Allocate(2*aNbElems,0);
      vtkUnsignedCharArray* aCellTypesArray = vtkUnsignedCharArray::New();
      aCellTypesArray->SetNumberOfComponents(1);
      aCellTypesArray->Allocate(aNbElems*aCellTypesArray->GetNumberOfComponents());
      if(!myCellIds.empty() && myCellTypes.empty()){
	if(myStoreMapping) myOut2InId.reserve(myCellIds.size());
	if(myChangeMode == eAdding){
	  for(vtkIdType aCellId = 0, anOutId = 0; aCellId < aNbElems; aCellId++,anOutId++){
	    if(myCellIds.find(aCellId) != myCellIds.end()){
	      InsertCell(anInput,aConnectivity,aCellTypesArray,aCellId,anIdList,
			 myStoreMapping,anOutId,myOut2InId,myIn2OutId);
	    }
	  }
	}else{
	  for(vtkIdType aCellId = 0, anOutId = 0; aCellId < aNbElems; aCellId++,anOutId++){
	    if(myCellIds.find(aCellId) == myCellIds.end()){
	      InsertCell(anInput,aConnectivity,aCellTypesArray,aCellId,anIdList,
			 myStoreMapping,anOutId,myOut2InId,myIn2OutId);
	    }
	  }
	}
      }else if(myCellIds.empty() && !myCellTypes.empty()){
	if(myChangeMode == eAdding){
	  for(vtkIdType aCellId = 0, anOutId = 0; aCellId < aNbElems; aCellId++,anOutId++){
	    vtkIdType aType = anInput->GetCellType(aCellId);
	    if(myCellTypes.find(aType) != myCellTypes.end()){
	      InsertCell(anInput,aConnectivity,aCellTypesArray,aCellId,anIdList,
			 myStoreMapping,anOutId,myOut2InId,myIn2OutId);
	    }
	  }
	}else{
	  for(vtkIdType aCellId = 0, anOutId = 0; aCellId < aNbElems; aCellId++,anOutId++){
	    vtkIdType aType = anInput->GetCellType(aCellId);
	    if(myCellTypes.find(aType) == myCellTypes.end()){
	      InsertCell(anInput,aConnectivity,aCellTypesArray,aCellId,anIdList,
			 myStoreMapping,anOutId,myOut2InId,myIn2OutId);
	    }
	  }
	}
      }else if(!myCellIds.empty() && !myCellTypes.empty()){
	if(myChangeMode == eAdding){
	  for(vtkIdType aCellId = 0, anOutId = 0; aCellId < aNbElems; aCellId++,anOutId++){
	    vtkIdType aType = anInput->GetCellType(aCellId);
	    if(myCellTypes.find(aType) != myCellTypes.end()){
	      if(myCellIds.find(aCellId) != myCellIds.end()){
		InsertCell(anInput,aConnectivity,aCellTypesArray,aCellId,anIdList,
			   myStoreMapping,anOutId,myOut2InId,myIn2OutId);
	      }
	    }
	  }
	}else{
	  for(vtkIdType aCellId = 0, anOutId = 0; aCellId < aNbElems; aCellId++,anOutId++){
	    vtkIdType aType = anInput->GetCellType(aCellId);
	    if(myCellTypes.find(aType) == myCellTypes.end()){
	      if(myCellIds.find(aCellId) == myCellIds.end()){
		InsertCell(anInput,aConnectivity,aCellTypesArray,aCellId,anIdList,
			   myStoreMapping,anOutId,myOut2InId,myIn2OutId);
	      }
	    }
	  }
	}
      }
      if((aNbElems = aConnectivity->GetNumberOfCells())){
	vtkIntArray* aCellLocationsArray = vtkIntArray::New();
	aCellLocationsArray->SetNumberOfComponents(1);
	aCellLocationsArray->SetNumberOfTuples(aNbElems);
	aConnectivity->InitTraversal();
	for(vtkIdType i = 0, *pts, npts; aConnectivity->GetNextCell(npts,pts); i++){
	  aCellLocationsArray->SetValue(i,aConnectivity->GetTraversalLocation(npts));
	}
	anOutput->SetCells(aCellTypesArray,aCellLocationsArray,aConnectivity);
	anOutput->SetPoints(anInput->GetPoints());
	aCellLocationsArray->Delete();
      }
      aCellTypesArray->Delete();
      aConnectivity->Delete();
      anIdList->Delete();
    }
  }else{
    vtkIdList *anIdList = vtkIdList::New();
    anIdList->SetNumberOfIds(1);
    vtkCellArray *aConnectivity = vtkCellArray::New();
    vtkIdType aNbElems = anInput->GetNumberOfPoints();
    aConnectivity->Allocate(2*aNbElems,0);
    vtkUnsignedCharArray* aCellTypesArray = vtkUnsignedCharArray::New();
    aCellTypesArray->SetNumberOfComponents(1);
    aCellTypesArray->Allocate(aNbElems*aCellTypesArray->GetNumberOfComponents());
    if(myChangeMode == ePassAll || myCellIds.empty() && myCellTypes.empty() && myChangeMode == eRemoving){
      if(myStoreMapping) myOut2InId.reserve(aNbElems);
      for(vtkIdType aCellId = 0, anOutId = 0; aCellId < aNbElems; aCellId++,anOutId++){
	InsertPointCell(aConnectivity,aCellTypesArray,aCellId,anIdList,
			myStoreMapping,anOutId,myOut2InId,myIn2OutId);
      }
    }else if(!myCellIds.empty() && myCellTypes.empty()){
      if(myStoreMapping) myOut2InId.reserve(myCellIds.size());
      if(myChangeMode == eAdding){
	for(vtkIdType aCellId = 0, anOutId = 0; aCellId < aNbElems; aCellId++,anOutId++){
	  if(myCellIds.find(aCellId) != myCellIds.end()){
	    InsertPointCell(aConnectivity,aCellTypesArray,aCellId,anIdList,
			    myStoreMapping,anOutId,myOut2InId,myIn2OutId);
	  }
	}
      }else{
	for(vtkIdType aCellId = 0, anOutId = 0; aCellId < aNbElems; aCellId++,anOutId++){
	  if(myCellIds.find(aCellId) == myCellIds.end()){
	    InsertPointCell(aConnectivity,aCellTypesArray,aCellId,anIdList,
			    myStoreMapping,anOutId,myOut2InId,myIn2OutId);
	  }
	}
      }
    }else if(myCellIds.empty() && !myCellTypes.empty()){
      if(myChangeMode == eAdding){
	for(vtkIdType aCellId = 0, anOutId = 0; aCellId < aNbElems; aCellId++,anOutId++){
	  vtkIdType aType = anInput->GetCellType(aCellId);
	  if(myCellTypes.find(aType) != myCellTypes.end()){
	    InsertPointCell(aConnectivity,aCellTypesArray,aCellId,anIdList,
			    myStoreMapping,anOutId,myOut2InId,myIn2OutId);
	  }
	}
      }else{
	for(vtkIdType aCellId = 0, anOutId = 0; aCellId < aNbElems; aCellId++,anOutId++){
	  vtkIdType aType = anInput->GetCellType(aCellId);
	  if(myCellTypes.find(aType) == myCellTypes.end()){
	    InsertPointCell(aConnectivity,aCellTypesArray,aCellId,anIdList,
			    myStoreMapping,anOutId,myOut2InId,myIn2OutId);
	  }
	}
      }
    }else if(!myCellIds.empty() && !myCellTypes.empty()){
      if(myChangeMode == eAdding){
	for(vtkIdType aCellId = 0, anOutId = 0; aCellId < aNbElems; aCellId++,anOutId++){
	  vtkIdType aType = anInput->GetCellType(aCellId);
	  if(myCellTypes.find(aType) != myCellTypes.end()){
	    if(myCellIds.find(aCellId) != myCellIds.end()){
	      InsertPointCell(aConnectivity,aCellTypesArray,aCellId,anIdList,
			      myStoreMapping,anOutId,myOut2InId,myIn2OutId);
	    }
	  }
	}
      }else{
	for(vtkIdType aCellId = 0, anOutId = 0; aCellId < aNbElems; aCellId++,anOutId++){
	  vtkIdType aType = anInput->GetCellType(aCellId);
	  if(myCellTypes.find(aType) == myCellTypes.end()){
	    if(myCellIds.find(aCellId) == myCellIds.end()){
	      InsertPointCell(aConnectivity,aCellTypesArray,aCellId,anIdList,
			      myStoreMapping,anOutId,myOut2InId,myIn2OutId);
	    }
	  }
	}
      }
    }
    if((aNbElems = aConnectivity->GetNumberOfCells())){
      vtkIntArray* aCellLocationsArray = vtkIntArray::New();
      aCellLocationsArray->SetNumberOfComponents(1);
      aCellLocationsArray->SetNumberOfTuples(aNbElems);
      aConnectivity->InitTraversal();
      for(vtkIdType i = 0, *pts, npts; aConnectivity->GetNextCell(npts,pts); i++){
	aCellLocationsArray->SetValue(i,aConnectivity->GetTraversalLocation(npts));
      }
      anOutput->SetCells(aCellTypesArray,aCellLocationsArray,aConnectivity);
      anOutput->SetPoints(anInput->GetPoints());
      aCellLocationsArray->Delete();
    }
    aCellTypesArray->Delete();
    aConnectivity->Delete();
    anIdList->Delete();
  }
  if(MYDEBUG){
    MESSAGE("Execute - anOutput->GetNumberOfCells() = "<<anOutput->GetNumberOfCells());
    if(myStoreMapping){
      MESSAGE("Execute - myOut2InId.size() = "<<myOut2InId.size());
      MESSAGE("Execute - myIn2OutId.size() = "<<myIn2OutId.size());
    }
  }
}
