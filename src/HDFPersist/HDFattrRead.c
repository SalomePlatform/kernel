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
  File   : HDFattrRead.c
  Module : SALOME
----------------------------------------------------------------------------*/

#include "hdfi.h"

/*
 * - Name : HDFattrWrite
 * - Description : write a value to a HDF attribute
 * - Parameters :
 *     - id   (IN) : attribute ID
 *     - val  (OUT) : attribute value
 * - Result : 
 *     - if success : 0
 *     - if failure : -1
 */
hdf_err HDFattrRead(hdf_idt id,void *val)
{
  hdf_err ret = -1;
  hdf_idt type_hdf;
  
  if ((type_hdf = H5Aget_type(id)) >= 0) {
#if defined (PCLINUX) || defined (PCLINUX64)
    if ((H5Tget_class(type_hdf) == H5T_INTEGER) && (H5Tget_size(type_hdf) == 4)) 
      type_hdf = H5T_NATIVE_INT;
#endif 
    ret = H5Aread(id, type_hdf, val);
  }

  H5Tclose(type_hdf);
  return ret;
}
