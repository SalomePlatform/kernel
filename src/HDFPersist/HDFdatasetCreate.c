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
  File   : HDFdatasetCreate.c
  Module : SALOME
----------------------------------------------------------------------------*/

#include "hdfi.h"

/*
 * - Name : HDFdatasetCreate
 * - Description : creates a HDF dataset
 * - Parameters :
 *     - pid  (IN)     : father ID
 *     - name (IN)     : dataset name
 *     - type (IN)     : dataset type (HDF_STRING,HDF_INT32,HDF_INT64,HDF_FLOAT64)
 *     - dimd (IN)     : dataset size
 *     - order(IN)     : byte order (H5T_ORDER_NONE, H5T_ORDER_LE, H5T_ORDER_BE)
 * - Result : 
 *     - if success : returns dataset ID
 *     - if failure : -1
 */ 

hdf_idt HDFdatasetCreate(hdf_idt pid,char *name,hdf_type type,
                         hdf_size *dimd, int ndim, hdf_byte_order order, hdf_idt arrayId)
{
  hdf_idt dataset, dataspace = 0;
  hdf_err ret;
  hdf_idt type_hdf, new_type_hdf = -1;

  switch(type)
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

    case HDF_STRING :           
      if((new_type_hdf = H5Tcopy(H5T_C_S1)) < 0)
        return -1;
      if((ret = H5Tset_size(new_type_hdf,1)) < 0)
        return -1;
      break;    

    case HDF_ARRAY :
      type_hdf = arrayId;
      break;
      
    default :
      return -1;
    }

  /* set order */
  if ( order != H5T_ORDER_ERROR && 
       order != H5T_ORDER_NONE && 
       type  != HDF_STRING )
    {
      if (( new_type_hdf = H5Tcopy( type_hdf )) < 0 )
        return -1;
      if (( ret = H5Tset_order (new_type_hdf, order )) < 0 )
        return -1;
    }

  if ((dataset = H5Dopen(pid,name)) < 0)
    {
      if ((dataspace = H5Screate_simple(ndim, dimd, NULL)) < 0)                                                         
        return -1;
      if ((dataset = H5Dcreate(pid,name,
                               new_type_hdf < 0 ? type_hdf : new_type_hdf,
                               dataspace, H5P_DEFAULT)) < 0)
        return -1;
    }
  else
    return -1;

  if ( ! (new_type_hdf < 0) && (ret = H5Tclose(new_type_hdf)) < 0)
    return -1;
  if ((ret = H5Sclose(dataspace)) < 0)
    return -1;

  return dataset;
}
