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
  File   : HDFfileOpen.c
  Module : SALOME
----------------------------------------------------------------------------*/

#include "hdfi.h"
/*
 * - Name : HDFfileOpen
 * - Description : open an existing HDF file
 * - Parameters :
 *     - name (IN)        : file name
 *     - access_mode (IN) : file access mode 
 * - Result : 
 *     - success : file ID
 *     - failure : -1 
 */ 

hdf_idt HDFfileOpen(char *name,hdf_access_mode access_mode)
{ 
  hdf_idt fid;
  int mode;

  switch(access_mode)
    {
    case HDF_RDONLY:
      mode = H5F_ACC_RDONLY;
      break;

    case HDF_RDWR :
      mode = H5F_ACC_RDWR; 
      break;

    default :
      return -1;
    }  

  if ((fid = H5Fopen(name,mode,H5P_DEFAULT)) < 0)
    return -1;

  return fid;
}
