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
  File   : test2.c
  Module : SALOME
----------------------------------------------------------------------------*/

#include"hdfi.h"
#include<stdlib.h>

int main() {
  hdf_idt fid,gid,did,atid;
  hdf_err ret;
  hdf_size size[1];
  hdf_int32 data[10];
  int attr_val;
  int i;

  HDFerrorModeLock();
  
  if  ((fid = HDFfileOpen("titi.HDF",HDF_RDONLY)) < 0)
    return -1;
  MESSAGE("Ouverture du fichier")

  if ((gid = HDFgroupOpen(fid,"GROUP")) < 0)
    return -1;
  MESSAGE("Ouverture du goupe")

  if ((atid = HDFattrOpen(gid,"ATTR"))< 0)
    return -1;
  MESSAGE("Ouverture de l'attribut du goupe")

  if ((ret = HDFattrRead(atid,&attr_val)) < 0) 
    return -1;
  MESSAGE("Lecture de l'attribut du goupe : " << attr_val)

  if ((ret = HDFattrClose(atid))  < 0)
    return -1;
  MESSAGE("Fermeture de l'attribut du goupe")

  if ((did = HDFdatasetOpen(gid,"DATASET")) < 0)
    return -1;
  MESSAGE("Ouverture du dataset")

  if ((ret = HDFdatasetRead(did,data)) < 0)
    return -1;
  MESSAGE("Lecture du dataset")
  for (i=0;i<10;i++)
    MESSAGE(data[i])

  if ((ret = HDFdatasetClose(did)) < 0)
    return -1;
  MESSAGE("Fermeture du dataset")

  if ((ret = HDFgroupClose(gid)) < 0)
    return -1;
  MESSAGE("Fermeture du groupe")
  
  if ((ret = HDFfileClose(fid)) < 0)
    return ret;
  MESSAGE("Fermeture du fichier")
  
  return 0;
}
