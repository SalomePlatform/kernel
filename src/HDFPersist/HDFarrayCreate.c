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
  File   : HDFarrayCreate.c
  Module : SALOME
----------------------------------------------------------------------------*/

#include "hdfi.h"

/*
 * - Name : HDFarrayCreate
 * - Description : creates a HDF array
 * - Parameters :
 *     - dataType    (IN) : type of the data in the array (HDF_INT32, HDF_INT64, HDF_FLOAT64, HDF_CHAR).
 *     - ndim        (IN) : is the number of dimensions and the size of each dimension is specified in the array dims.
 *     - dim         (IN) : size of each array dimension.
 * - Result : 
 *     - if success : returns array ID
 *     - if failure : -1
 */ 

hdf_idt HDFarrayCreate(hdf_type dataType, int ndim, hdf_size *dim)
{
  hdf_idt type_hdf;
  switch(dataType)
      {
      case HDF_FLOAT64 :
#if defined (PCLINUX) || defined (PCLINUX64)
	type_hdf = H5T_IEEE_F64BE;
#else 
	type_hdf = H5T_IEEE_F64LE;
#endif
	break;

      case HDF_INT32 :
#if defined (PCLINUX) || defined (PCLINUX64)
	type_hdf = H5T_STD_I32BE;  
#else
	type_hdf = H5T_NATIVE_INT;
#endif
	break;
 
      case HDF_INT64 :
	type_hdf = H5T_NATIVE_LONG;
	break;
      case HDF_CHAR :
	type_hdf = H5T_NATIVE_CHAR;
	break;
      default:
	return -1;
	break;
      }
    
    return H5Tarray_create2( type_hdf, ndim, dim );
}
