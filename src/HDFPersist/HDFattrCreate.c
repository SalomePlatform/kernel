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
  File   : HDFattrCreate.c
  Module : SALOME
----------------------------------------------------------------------------*/

#include "hdfi.h"

/*
 * - Name : HDFattrCreate
 * - Description : create a HDF attribute
 * - Parameters :
 *     - pid (IN) : father ID
 *     - name (IN) : attribute name
 *     - type (IN) : attribute type
 * - Result: 
 *     - if success : attribute ID 
 *     - if failure : -1
 */
hdf_idt HDFattrCreate(hdf_idt pid,char *name,hdf_type type, size_t size)
{
  hdf_idt aid,attr;
  hdf_err ret;
  int type_hdf;

  switch(type) {
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
    if((type_hdf = H5Tcopy(H5T_C_S1)) < 0)
      return -1;
    if((ret = H5Tset_size(type_hdf, size)) < 0)
      return -1;
    break;
    
  default :
    return -1;
  }

  if ((aid = H5Screate(H5S_SCALAR)) < 0)
    return -1;
  
  if ((attr = H5Acreate(pid,name,type_hdf,aid,H5P_DEFAULT)) < 0)
    return -1;
  
  if ((ret = H5Sclose(aid)) < 0)
    return -1;
  
  return attr;
}
