/*----------------------------------------------------------------------------
SALOME HDFPersist : implementation of HDF persitent ( save/ restore )

 Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
 CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 

 This library is free software; you can redistribute it and/or 
 modify it under the terms of the GNU Lesser General Public 
 License as published by the Free Software Foundation; either 
 version 2.1 of the License. 

 This library is distributed in the hope that it will be useful, 
 but WITHOUT ANY WARRANTY; without even the implied warranty of 
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 Lesser General Public License for more details. 

 You should have received a copy of the GNU Lesser General Public 
 License along with this library; if not, write to the Free Software 
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 

 See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 



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
 * - Result : 
 *     - if success : returns dataset ID
 *     - if failure : -1
 */ 

hdf_idt HDFdatasetCreate(hdf_idt pid,char *name,hdf_type type,
			 hdf_size *dimd, int ndim)
{
  hdf_idt dataset, dataspace = 0;
  hdf_err ret;
  int type_hdf;

  switch(type)
    {
    case HDF_FLOAT64 :
#ifdef PCLINUX
      type_hdf = H5T_IEEE_F64BE;
#else 
      type_hdf = H5T_IEEE_F64LE;
#endif
      break;

    case HDF_INT32 :
#ifdef PCLINUX
      type_hdf = H5T_STD_I32BE;  
#else
      type_hdf = H5T_NATIVE_INT;
#endif
      break;
 
    case HDF_INT64 :
      type_hdf = H5T_NATIVE_LONG;
      break;

    case HDF_STRING :           
      if((type_hdf = H5Tcopy(H5T_C_S1)) < 0)
	return -1;
      if((ret = H5Tset_size(type_hdf,1)) < 0)
	return -1;
      break;

    default :
      return -1;
    }

  if ((dataset = H5Dopen(pid,name)) < 0)
    {
      if ((dataspace = H5Screate_simple(ndim, dimd, NULL)) < 0)								
	return -1;
      if ((dataset = H5Dcreate(pid,name,type_hdf,dataspace, H5P_DEFAULT)) < 0)
	return -1;
    }
  else
    return -1;

  if ((ret = H5Sclose(dataspace)) < 0)
    return -1;           

  return dataset;
}
