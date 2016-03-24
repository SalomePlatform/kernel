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
  File   : HDFdatasetRead.c
  Module : SALOME
----------------------------------------------------------------------------*/

#include "hdfi.h"

/*
 * - Name : _MEDdatasetRead
 * - Description : writes a HDF dataset
 * - Parameters :
 *     - id  (IN)     : dataset ID
 *     - val  (OUT)    : datset values
 * - Result : 
 *     - if success : 0
 *     - if failure : -1
 */ 
hdf_err HDFdatasetRead(hdf_idt id, void *val)
{
  hdf_idt datatype;
  hdf_err ret;

  if ((datatype = H5Dget_type(id)) < 0)
    return -1;

//#if defined (PCLINUX) || defined (PCLINUX64)
  if ((H5Tget_class(datatype) == H5T_INTEGER) && (H5Tget_size(datatype) == 4)) 
    datatype = H5T_NATIVE_INT;
//#endif 

  if ((ret = H5Dread(id,datatype,H5S_ALL,H5S_ALL,H5P_DEFAULT, val)) < 0)
    return -1;

  return 0;
}
