// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

/*----------------------------------------------------------------------------
SALOME HDFPersist : implementation of HDF persitent ( save/ restore )
  File   : HDFnObjects.c
  Module : SALOME
----------------------------------------------------------------------------*/

#include "hdfi.h"
#include <stdlib.h>

/*
 * - Name : HDFnObjects
 * - Description : returns the number of HDF objets in a HDF group
 * - Parameters :
 *     - fid     (IN)     : HDF file ID
 *     - path    (IN)     : acces path to the HDF group in the HDF file
 * - Results :
 *     - if success : number of objects
 *     - if failure : -1 
 */ 

hdf_err Num(hdf_idt id,const char *name, void *data);

hdf_err
HDFnObjects(hdf_idt fid,const char *path,int *n)
{
  int idx;
  int m = 0;

  if ((idx  = H5Giterate(fid,path,NULL,Num,(void*)&m)) < 0)
    return -1;

  *n = (int) m;

  return 0;
}

hdf_err Num(hdf_idt id,const char *name, void *data)
{
  int *count;
  
  count = (int *) data;
  (*count)++;

  return 0;
}

