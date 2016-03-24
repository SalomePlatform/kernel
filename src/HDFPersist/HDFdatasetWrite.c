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
  File   : HDFdatasetWrite.c
  Module : SALOME
----------------------------------------------------------------------------*/

#include "hdfi.h"
#include <stdlib.h>

/*
 * - Name : _MEDdatasetWrite
 * - Description : writes a HDF dataset
 * - Parameters :
 *     - id  (IN)     : dataset ID
 *     - val  (IN)    : datset values
 * - Result : 
 *     - if success : 0
 *     - if failure : -1
 */ 
hdf_err HDFdatasetWrite(hdf_idt id, void *val)
{
  hdf_idt datatype;
  hdf_err ret;
#if defined (PCLINUX) || defined (PCLINUX64)
  int isI32BE = 0;
  int size = 0;
#endif

  if ((datatype = H5Dget_type(id)) < 0)
    return -1;

#if defined (PCLINUX) || defined (PCLINUX64)
  if((H5Tget_class(datatype) == H5T_INTEGER) && (H5Tget_size(datatype) == 4)) {
    isI32BE = 1; /* See HDFdatasetCreate */

    /*SRN : bug IPAL9619:  replaced the method of getting the size of INT32 dataset */ 
    int i, ndim = HDFdatasetGetnDim(id);
    if(ndim < 0) return -1;
    
    hdf_size *dim = (hdf_size *) malloc(sizeof(hdf_size)*ndim);
    if ((ret = HDFdatasetGetDim(id, dim)) < 0)  return -1;
        
    for(i=0; i<ndim; i++) size+=dim[i];    
    free(dim);
    /*SRN : end of the fix */
    
    if(size == 0) 
      return -1;
    if(H5Tconvert(H5T_NATIVE_INT, H5T_STD_I32BE, size, (void *)val, NULL, (hid_t)0) < 0) 
      return -1;
  }
#endif

  if ((ret = H5Dwrite(id, datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, val)) < 0) 
    return -1;

#if defined (PCLINUX) || defined (PCLINUX64)
  if (isI32BE && (H5Tconvert(H5T_STD_I32BE, H5T_NATIVE_INT, size, (void *)val, NULL, (hid_t)0) < 0)) 
    return -1;
#endif

  return 0;
}
