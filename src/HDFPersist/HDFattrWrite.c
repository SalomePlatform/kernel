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
  File   : HDFattrWrite.c
  Module : SALOME
----------------------------------------------------------------------------*/

#include "hdfi.h"

/*
 * - Name : HDFattrWrite
 * - Description : write a value to a HDF attribute
 * - Parameters :
 *     - id (IN) : attribute ID
 *     - val  (IN) : attribute value
 * - Result : 
 *     - if success : 0
 *     - if failure : -1
 */
hdf_err HDFattrWrite(hdf_idt id, void *val)
{
  hdf_idt type_id;
  int ret = 0;
#if defined (PCLINUX) || defined (PCLINUX64)
  int isI32BE = 0;
#endif

  if ((type_id = H5Aget_type(id)) < 0)
    return -1;

#if defined (PCLINUX) || defined (PCLINUX64)
  if((H5Tget_class(type_id) == H5T_INTEGER) && (H5Tget_size(type_id) == 4)) {
    isI32BE = 1; /* See HDFattrCreate */
    if (H5Tconvert(H5T_NATIVE_INT,H5T_STD_I32BE,1,(void *)val,NULL,(hid_t)0) < 0)
      return -1;
  }
#endif

  ret = H5Awrite(id,type_id, val);


#if defined (PCLINUX) || defined (PCLINUX64)
  if (isI32BE && (H5Tconvert(H5T_STD_I32BE,H5T_NATIVE_INT,1,(void *)val,NULL,(hid_t)0) < 0))
    return -1;
#endif

  H5Tclose(type_id);

  return ret;
}
