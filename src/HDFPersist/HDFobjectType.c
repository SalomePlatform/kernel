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
  File   : HDFobjectType.c
  Module : SALOME
----------------------------------------------------------------------------*/

#include "hdfi.h"

hdf_err 
HDFobjectType(hdf_idt id, char *name, hdf_object_type *type)
{
  hdf_bool follow = 1;
  H5G_stat_t buf;
  hdf_err ret;

  if ((ret = H5Gget_objinfo(id,name,follow,&buf) < 0))
    return -1; 
  
  switch(buf.type)
    {
    case H5G_GROUP :
      *type = HDF_GROUP;
      break;

    case H5G_DATASET :
      *type = HDF_DATASET;
      break;
     
    default :
      return -1;
    }

  return 0;
}
