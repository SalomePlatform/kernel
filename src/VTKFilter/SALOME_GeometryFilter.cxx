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
//  File   : SALOME_GeometryFilter.cxx
//  Author : Michael ZORIN
//  Module : SALOME
//  $Header$

#include "SALOME_GeometryFilter.h"

#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkGenericCell.h>
#include <vtkHexahedron.h>
#include <vtkMergePoints.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPyramid.h>
#include <vtkStructuredGrid.h>
#include <vtkTetra.h>
#include <vtkUnsignedCharArray.h>
#include <vtkUnstructuredGrid.h>
#include <vtkVoxel.h>
#include <vtkWedge.h>

#ifdef _DEBUG_
static int MYDEBUG = 0;
static int MYDEBUGWITHFILES = 0;
#else
static int MYDEBUG = 0;
static int MYDEBUGWITHFILES = 0;
#endif

#if defined __GNUC__
  #if __GNUC__ == 2
    #define __GNUC_2__
  #endif
#endif

vtkCxxRevisionMacro(SALOME_GeometryFilter, "$Revision$");
vtkStandardNewMacro(SALOME_GeometryFilter);


SALOME_GeometryFilter::SALOME_GeometryFilter(): 
  myShowInside(0),
  myStoreMapping(0)
{}


SALOME_GeometryFilter::~SALOME_GeometryFilter()
{}


void SALOME_GeometryFilter::Execute()
{
  vtkDataSet *input= this->GetInput();
  vtkIdType numCells=input->GetNumberOfCells();

  if (numCells == 0)
    {
      return;
    }
  
  if (input->GetDataObjectType() == VTK_UNSTRUCTURED_GRID){
    this->UnstructuredGridExecute();
    return;
  }else
    vtkGeometryFilter::Execute();
}


void SALOME_GeometryFilter::SetStoreMapping(int theStoreMapping){
  myStoreMapping = theStoreMapping;
  this->Modified();
}


vtkIdType SALOME_GeometryFilter::GetElemObjId(int theVtkID){
  if(myVTK2ObjIds.empty() || theVtkID > myVTK2ObjIds.size()) return -1;
#if defined __GNUC_2__
  return myVTK2ObjIds[theVtkID];
#else
  return myVTK2ObjIds.at(theVtkID);
#endif
}


void SALOME_GeometryFilter::UnstructuredGridExecute()
{
  vtkUnstructuredGrid *input= (vtkUnstructuredGrid *)this->GetInput();
  vtkCellArray *Connectivity = input->GetCells();
  if (Connectivity == NULL) {return;}
  vtkIdType cellId;
  int i;
  int allVisible;
  vtkIdType npts = 0;
  vtkIdType *pts = 0;
  vtkPoints *p = input->GetPoints();
  vtkIdType numCells=input->GetNumberOfCells();
  vtkPointData *pd = input->GetPointData();
  vtkCellData *cd = input->GetCellData();
  vtkPolyData *output = this->GetOutput();
  vtkPointData *outputPD = output->GetPointData();
  
  vtkCellData *outputCD = output->GetCellData();
  vtkCellArray *Verts, *Lines, *Polys, *Strips;
  vtkIdList *cellIds, *faceIds;
  char *cellVis;
  vtkIdType newCellId;
  int faceId, *faceVerts, numFacePts;
  float *x;
  int PixelConvert[4], aNewPts[VTK_CELL_SIZE];
  // ghost cell stuff
  unsigned char  updateLevel = (unsigned char)(output->GetUpdateGhostLevel());
  unsigned char  *cellGhostLevels = 0;  
  
  PixelConvert[0] = 0;
  PixelConvert[1] = 1;
  PixelConvert[2] = 3;
  PixelConvert[3] = 2;
  
  vtkDebugMacro(<<"Executing geometry filter for unstructured grid input");

  vtkDataArray* temp = 0;
  if (cd)
    {
    temp = cd->GetArray("vtkGhostLevels");
    }
  if ( (!temp) || (temp->GetDataType() != VTK_UNSIGNED_CHAR)
    || (temp->GetNumberOfComponents() != 1))
    {
    vtkDebugMacro("No appropriate ghost levels field available.");
    }
  else
    {
    cellGhostLevels = ((vtkUnsignedCharArray*)temp)->GetPointer(0);
    }
  
  // Check input
  if ( Connectivity == NULL )
    {
    vtkDebugMacro(<<"Nothing to extract");
    return;
    }

  // Determine nature of what we have to do
  cellIds = vtkIdList::New();
  faceIds = vtkIdList::New();
  if ( (!this->CellClipping) && (!this->PointClipping) &&
       (!this->ExtentClipping) )
    {
    allVisible = 1;
    cellVis = NULL;
    }
  else
    {
    allVisible = 0;
    cellVis = new char[numCells];
    }

  // Just pass points through, never merge
  output->SetPoints(input->GetPoints());
  outputPD->PassData(pd);

  outputCD->CopyAllocate(cd,numCells,numCells/2);

  output->Allocate(numCells/4+1,numCells);
  //Verts = vtkCellArray::New();
  //Verts->Allocate(numCells/4+1,numCells);
  //Lines = vtkCellArray::New();
  //Lines->Allocate(numCells/4+1,numCells);
  //Polys = vtkCellArray::New();
  //Polys->Allocate(numCells/4+1,numCells);
  //Strips = vtkCellArray::New();
  //Strips->Allocate(numCells/4+1,numCells);
  
  // Loop over the cells determining what's visible
  if (!allVisible)
    {
    for (cellId=0, Connectivity->InitTraversal(); 
         Connectivity->GetNextCell(npts,pts); 
         cellId++)
      {
      cellVis[cellId] = 1;
      if ( this->CellClipping && cellId < this->CellMinimum ||
           cellId > this->CellMaximum )
        {
        cellVis[cellId] = 0;
        }
      else
        {
        for (i=0; i < npts; i++) 
          {
          x = p->GetPoint(pts[i]);
          if ( (this->PointClipping && (pts[i] < this->PointMinimum ||
                                        pts[i] > this->PointMaximum) ) ||
               (this->ExtentClipping && 
                (x[0] < this->Extent[0] || x[0] > this->Extent[1] ||
                 x[1] < this->Extent[2] || x[1] > this->Extent[3] ||
                 x[2] < this->Extent[4] || x[2] > this->Extent[5] )) )
            {
            cellVis[cellId] = 0;
            break;
            }//point/extent clipping
          }//for each point
        }//if point clipping needs checking
      }//for all cells
    }//if not all visible
  
  // Loop over all cells now that visibility is known
  // (Have to compute visibility first for 3D cell boundarys)
  int progressInterval = numCells/20 + 1;
  if(myStoreMapping){
    myVTK2ObjIds.clear();
    myVTK2ObjIds.reserve(numCells);
  }
  for (cellId=0, Connectivity->InitTraversal(); 
       Connectivity->GetNextCell(npts,pts); 
       cellId++)
    {
    //Progress and abort method support
    if ( !(cellId % progressInterval) )
      {
      vtkDebugMacro(<<"Process cell #" << cellId);
      this->UpdateProgress ((float)cellId/numCells);
      }

    // Handle ghost cells here.  Another option was used cellVis array.
    if (cellGhostLevels && cellGhostLevels[cellId] > updateLevel)
      { // Do not create surfaces in outer ghost cells.
      continue;
      }
    
    if (allVisible || cellVis[cellId])  //now if visible extract geometry
      {
      //special code for nonlinear cells - rarely occurs, so right now it
      //is slow.
      vtkIdType aCellType = input->GetCellType(cellId);
      switch (aCellType)
        {
        case VTK_EMPTY_CELL:
          break;

        case VTK_VERTEX:
        case VTK_POLY_VERTEX:
          newCellId = output->InsertNextCell(aCellType,npts,pts);
	  if(myStoreMapping){
	    myVTK2ObjIds.push_back(cellId); //apo
	  }
          outputCD->CopyData(cd,cellId,newCellId);
          break;

        case VTK_LINE: 
        case VTK_POLY_LINE:
          newCellId = output->InsertNextCell(VTK_LINE,npts,pts);
	  if(myStoreMapping){
	    myVTK2ObjIds.push_back(cellId); //apo
	  }
          outputCD->CopyData(cd,cellId,newCellId);
          break;

        case VTK_TRIANGLE:
        case VTK_QUAD:
        case VTK_POLYGON:
          newCellId = output->InsertNextCell(aCellType,npts,pts);
	  if(myStoreMapping){
	    myVTK2ObjIds.push_back(cellId); //apo
	  }
          outputCD->CopyData(cd,cellId,newCellId);
          break;

        case VTK_TRIANGLE_STRIP:
          newCellId = output->InsertNextCell(aCellType,npts,pts);
	  if(myStoreMapping){
	    myVTK2ObjIds.push_back(cellId); //apo
	  }
          outputCD->CopyData(cd,cellId,newCellId);
          break;

        case VTK_PIXEL:
          newCellId = output->InsertNextCell(aCellType,npts,pts);
	  if(myStoreMapping){
	    myVTK2ObjIds.push_back(cellId); //apo
	  }
	  outputCD->CopyData(cd,cellId,newCellId);
          break;

        case VTK_TETRA: {
          for (faceId = 0; faceId < 4; faceId++)
            {
            faceIds->Reset();
            faceVerts = vtkTetra::GetFaceArray(faceId);
            faceIds->InsertNextId(pts[faceVerts[0]]);
            faceIds->InsertNextId(pts[faceVerts[1]]);
            faceIds->InsertNextId(pts[faceVerts[2]]);
            numFacePts = 3;
	    aCellType = VTK_TRIANGLE;
            input->GetCellNeighbors(cellId, faceIds, cellIds);
            if ( cellIds->GetNumberOfIds() <= 0 || myShowInside == 1 ||
                 (!allVisible && !cellVis[cellIds->GetId(0)]) )
              {
              for ( i=0; i < numFacePts; i++)
                {
                aNewPts[i] = pts[faceVerts[i]];
                }
              newCellId = output->InsertNextCell(aCellType,numFacePts,aNewPts);
	      if(myStoreMapping){
		myVTK2ObjIds.push_back(cellId); //apo
	      }
              outputCD->CopyData(cd,cellId,newCellId);
              }
            }
          break;
	}
        case VTK_VOXEL: {
          for (faceId = 0; faceId < 6; faceId++)
            {
            faceIds->Reset();
            faceVerts = vtkVoxel::GetFaceArray(faceId);
            faceIds->InsertNextId(pts[faceVerts[0]]);
            faceIds->InsertNextId(pts[faceVerts[1]]);
            faceIds->InsertNextId(pts[faceVerts[2]]);
            faceIds->InsertNextId(pts[faceVerts[3]]);
            numFacePts = 4;
	    aCellType = VTK_QUAD;
            input->GetCellNeighbors(cellId, faceIds, cellIds);
            if ( cellIds->GetNumberOfIds() <= 0 || myShowInside == 1 || 
                 (!allVisible && !cellVis[cellIds->GetId(0)]) )
              {
              for ( i=0; i < numFacePts; i++)
                {
                aNewPts[i] = pts[faceVerts[PixelConvert[i]]];
                }
              newCellId = output->InsertNextCell(aCellType,numFacePts,aNewPts);
	      if(myStoreMapping){
		myVTK2ObjIds.push_back(cellId); //apo
	      }
              outputCD->CopyData(cd,cellId,newCellId);
              }
            }
          break;
	}
        case VTK_HEXAHEDRON: {
          for (faceId = 0; faceId < 6; faceId++)
            {
            faceIds->Reset();
            faceVerts = vtkHexahedron::GetFaceArray(faceId);
            faceIds->InsertNextId(pts[faceVerts[0]]);
            faceIds->InsertNextId(pts[faceVerts[1]]);
            faceIds->InsertNextId(pts[faceVerts[2]]);
            faceIds->InsertNextId(pts[faceVerts[3]]);
            numFacePts = 4;
	    aCellType = VTK_QUAD;
            input->GetCellNeighbors(cellId, faceIds, cellIds);
            if ( cellIds->GetNumberOfIds() <= 0 || myShowInside == 1 ||
                 (!allVisible && !cellVis[cellIds->GetId(0)]) )
              {
              for ( i=0; i < numFacePts; i++)
                {
                aNewPts[i] = pts[faceVerts[i]];
                }
              newCellId = output->InsertNextCell(aCellType,numFacePts,aNewPts);
	      if(myStoreMapping){
		myVTK2ObjIds.push_back(cellId); //apo
	      }
              outputCD->CopyData(cd,cellId,newCellId);
              }
            }
          break;
	}
        case VTK_WEDGE: {
          for (faceId = 0; faceId < 5; faceId++)
            {
            faceIds->Reset();
            faceVerts = vtkWedge::GetFaceArray(faceId);
            faceIds->InsertNextId(pts[faceVerts[0]]);
            faceIds->InsertNextId(pts[faceVerts[1]]);
            faceIds->InsertNextId(pts[faceVerts[2]]);
            numFacePts = 3;
	    aCellType = VTK_TRIANGLE;
            if (faceVerts[3] >= 0)
              {
              faceIds->InsertNextId(pts[faceVerts[3]]);
              numFacePts = 4;
	      aCellType = VTK_QUAD;
              }
            input->GetCellNeighbors(cellId, faceIds, cellIds);
            if ( cellIds->GetNumberOfIds() <= 0 || myShowInside == 1 || 
                 (!allVisible && !cellVis[cellIds->GetId(0)]) )
              {
              for ( i=0; i < numFacePts; i++)
                {
                aNewPts[i] = pts[faceVerts[i]];
                }
              newCellId = output->InsertNextCell(aCellType,numFacePts,aNewPts);
	      if(myStoreMapping){
		myVTK2ObjIds.push_back(cellId); //apo
	      }
              outputCD->CopyData(cd,cellId,newCellId);
              }
            }
          break;
	}
        case VTK_PYRAMID: {
          for (faceId = 0; faceId < 5; faceId++)
            {
            faceIds->Reset();
            faceVerts = vtkPyramid::GetFaceArray(faceId);
            faceIds->InsertNextId(pts[faceVerts[0]]);
            faceIds->InsertNextId(pts[faceVerts[1]]);
            faceIds->InsertNextId(pts[faceVerts[2]]);
            numFacePts = 3;
	    aCellType = VTK_TRIANGLE;
            if (faceVerts[3] >= 0)
              {
              faceIds->InsertNextId(pts[faceVerts[3]]);
              numFacePts = 4;
	      aCellType = VTK_QUAD;
              }
            input->GetCellNeighbors(cellId, faceIds, cellIds);
            if ( cellIds->GetNumberOfIds() <= 0 || myShowInside == 1 || 
                 (!allVisible && !cellVis[cellIds->GetId(0)]) )
              {
              for ( i=0; i < numFacePts; i++)
                {
                aNewPts[i] = pts[faceVerts[i]];
                }
              newCellId = output->InsertNextCell(aCellType,numFacePts,aNewPts);
	      if(myStoreMapping){
		myVTK2ObjIds.push_back(cellId); //apo
	      }
              outputCD->CopyData(cd,cellId,newCellId);
              }
            }
          break;
	}
        //Quadratic cells
        case VTK_QUADRATIC_EDGE:
        case VTK_QUADRATIC_TRIANGLE:
        case VTK_QUADRATIC_QUAD:
        case VTK_QUADRATIC_TETRA:
        case VTK_QUADRATIC_HEXAHEDRON:
          
          break; //done with quadratic cells
          
        } //switch
      } //if visible
    } //for all cells
  
  if(MYDEBUG && myStoreMapping){
    for(int i = 0, iEnd = myVTK2ObjIds.size(); i < iEnd; i++){
      cout<<myVTK2ObjIds[i]<<", ";
    }
    cout<<"\n";
  }

  // Update ourselves and release memory
  //
  //output->SetVerts(Verts);
  //Verts->Delete();
  //output->SetLines(Lines);
  //Lines->Delete();
  //output->SetPolys(Polys);
  //Polys->Delete();
  //output->SetStrips(Strips);
  //Strips->Delete();
  
  output->Squeeze();

  vtkDebugMacro(<<"Extracted " << input->GetNumberOfPoints() << " points,"
  << output->GetNumberOfCells() << " cells.");

  cellIds->Delete();
  faceIds->Delete();
  if ( cellVis )
    {
    delete [] cellVis;
    }
}


void SALOME_GeometryFilter::SetInside(int theShowInside){
  if(myShowInside == theShowInside) return;
  myShowInside = theShowInside;
  this->Modified();
}
int SALOME_GeometryFilter::GetInside(){
  return myShowInside;
}
