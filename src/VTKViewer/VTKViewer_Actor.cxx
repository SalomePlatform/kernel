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

#include "VTKViewer_Actor.h"

#include "SALOME_PassThroughFilter.h"

// VTK Includes
#include <vtkObjectFactory.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkRenderer.h>

#include <vtkCell.h>
#include <vtkPolyData.h>
#include <vtkShrinkFilter.h>

#include "utilities.h"
 
using namespace std;

#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif


static void CopyPoints(vtkUnstructuredGrid* theGrid, vtkDataSet *theSourceDataSet){
  vtkPoints *aPoints = vtkPoints::New();
  vtkIdType iEnd = theSourceDataSet->GetNumberOfPoints();
  aPoints->SetNumberOfPoints(iEnd);
  for(vtkIdType i = 0; i < iEnd; i++){
    aPoints->SetPoint(i,theSourceDataSet->GetPoint(i));
  }
  theGrid->SetPoints(aPoints);
  aPoints->Delete();
}


//=======================================================================

vtkStandardNewMacro(VTKViewer_Actor);

VTKViewer_Actor::VTKViewer_Actor()
{
  myRenderer = NULL;
  myIsInfinite = true;

  Visibility = Pickable = false;

  myUnstructuredGrid = vtkUnstructuredGrid::New();
  myUnstructuredGrid->Allocate();

  myIsShrunk = false;
  myIsShrinkable = true;
  myShrinkFilter = vtkShrinkFilter::New();

  myMapper = vtkDataSetMapper::New();

  myMapper->SetInput(myUnstructuredGrid);
  Superclass::InitPipeLine(myMapper);

  SetResolveCoincidentTopology(false);
}


void VTKViewer_Actor::SetShrinkFactor(float theValue){
  myShrinkFilter->SetShrinkFactor(theValue);
  Modified();
}


void VTKViewer_Actor::SetShrink()
{
  if ( !myIsShrinkable ) return;
  if ( vtkDataSet* aDataSet = myPassFilter[0]->GetOutput() )
  {
    myShrinkFilter->SetInput( aDataSet );
    myPassFilter[1]->SetInput( myShrinkFilter->GetOutput() );
    myIsShrunk = true;
  }
}

void VTKViewer_Actor::UnShrink()
{
  if ( !myIsShrunk ) return;
  if ( vtkDataSet* aDataSet = myPassFilter[0]->GetOutput() )
  {
    myPassFilter[1]->SetInput( aDataSet );
    myPassFilter[1]->Modified();
    myIsShrunk = false;
    Modified();
  }
}


//----------------------------------------------------------------------------
VTKViewer_Actor::~VTKViewer_Actor()
{
  if(MYDEBUG) INFOS("VTKViewer_Actor::~VTKViewer_Actor()");

  myMapper->RemoveAllInputs();
  myMapper->Delete();

  myShrinkFilter->UnRegisterAllOutputs();
  myShrinkFilter->Delete();

  myUnstructuredGrid->Delete();
}


//----------------------------------------------------------------------------
void VTKViewer_Actor::MapCells(SALOME_Actor* theMapActor, 
			       const TColStd_IndexedMapOfInteger& theMapIndex)
{
  myUnstructuredGrid->Reset();

  vtkDataSet *aSourceDataSet = theMapActor->GetInput();
  CopyPoints(myUnstructuredGrid,aSourceDataSet);

  int aNbOfParts = theMapIndex.Extent();
  for(int ind = 1; ind <= aNbOfParts; ind++){
    int aPartId = theMapIndex( ind );
    vtkCell* aCell = theMapActor->GetElemCell(aPartId);
    myUnstructuredGrid->InsertNextCell(aCell->GetCellType(),aCell->GetPointIds());
    //for (int i = 0, iEnd = aCell->GetNumberOfEdges(); i < iEnd; i++){
    //  vtkCell* anEdgeCell = aCell->GetEdge(i);
    //  myUnstructuredGrid->InsertNextCell(VTK_LINE,anEdgeCell->GetPointIds());
    //}
  }

  UnShrink();
  if(theMapActor->IsShrunk()){
    SetShrinkFactor(theMapActor->GetShrinkFactor());
    SetShrink();
  }
}


//----------------------------------------------------------------------------
void VTKViewer_Actor::MapPoints(SALOME_Actor* theMapActor, 
				const TColStd_IndexedMapOfInteger& theMapIndex)
{
  myUnstructuredGrid->Reset();
  if(int aNbOfParts = theMapIndex.Extent()){
    vtkPoints *aPoints = vtkPoints::New();
    aPoints->SetNumberOfPoints(aNbOfParts);
    for(int i = 0; i < aNbOfParts; i++){
      int aPartId = theMapIndex( i+1 );
      float* aCoord = theMapActor->GetNodeCoord(aPartId);
      aPoints->SetPoint(i,aCoord);
      myUnstructuredGrid->InsertNextCell(VTK_VERTEX,1,&i);
    }
    myUnstructuredGrid->SetPoints(aPoints);
    aPoints->Delete();
  }

  UnShrink();
}


//----------------------------------------------------------------------------
void VTKViewer_Actor::MapEdge(SALOME_Actor* theMapActor, 
			      const TColStd_IndexedMapOfInteger& theMapIndex)
{
  myUnstructuredGrid->Reset();

  vtkDataSet *aSourceDataSet = theMapActor->GetInput();
  CopyPoints(myUnstructuredGrid,aSourceDataSet);

  int iEnd = theMapIndex.Extent();
  int aCellId = -1, aCellCounter = 0;
  for(int i = 1; i <= iEnd; i++){
    int anId = theMapIndex( i );
    if(anId > 0) {
      aCellCounter++;
      aCellId = anId;
    }
  }

  if(aCellCounter == 1){
    vtkCell* aCell = theMapActor->GetElemCell(aCellId);
    if(aCell->GetCellType() <= VTK_LINE){
      myUnstructuredGrid->InsertNextCell(aCell->GetCellType(),aCell->GetPointIds());
    }else{
      int aNbOfParts = aCell->GetNumberOfEdges();
      for(int i = 1; i <= iEnd; i++){
        int aPartId = theMapIndex(i);
	if( aPartId < 0){
          aPartId = -aPartId-1;
	  if(0 > aPartId || aPartId >= aNbOfParts) break;
	  vtkCell* anEdgeCell = aCell->GetEdge(aPartId);
	  myUnstructuredGrid->InsertNextCell(VTK_LINE,anEdgeCell->GetPointIds());
	}
      }
    }
  }else{
    int aNbOfParts = aSourceDataSet->GetNumberOfCells();
    for(int i = 1; i <= iEnd; i++){
      int aPartId = theMapIndex( i );
      if(aPartId > 0){
	if(aPartId >= aNbOfParts) break;
	vtkCell* aCell = aSourceDataSet->GetCell(aPartId);
	myUnstructuredGrid->InsertNextCell(aCell->GetCellType(),aCell->GetPointIds());
      }
    }
  }

  UnShrink();
  if(theMapActor->IsShrunk()){
    SetShrinkFactor(theMapActor->GetShrinkFactor());
    SetShrink();
  }
}

//----------------------------------------------------------------------------
