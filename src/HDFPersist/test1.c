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
  File   : test1.c
  Module : SALOME
----------------------------------------------------------------------------*/

#include"hdfi.h"
#include<stdlib.h>

#define MESSAGE(msg) printf(msg);
//__FILE__,":[",__LINE__,"]: ",

int main() {
  hdf_idt fid,gid,did,atid;
  hdf_err ret;
  hdf_size size[1];
  hdf_int32 data[10] = { 1,2,3,4,5,6,7,8,9,10};
  hdf_int32 attr_val = 2; 
  
  system("rm titi.HDF");

  HDFerrorModeLock();
  
  if  ((fid = HDFfileCreate("titi.HDF")) < 0)
    return -1;
  MESSAGE("Creation du fichier")

  if ((gid = HDFgroupCreate(fid,"GROUP")) < 0)
    return -1;
  MESSAGE("Creation d'un groupe")

  if ((atid = HDFattrCreate(gid,"ATTR",HDF_INT32))< 0)
    return -1;
  MESSAGE("Creation d'un attribut")

  if ((ret = HDFattrWrite(atid,&attr_val)) < 0)
    return -1;
  MESSAGE("Ecriture de l'attribut")
 
  if ((ret = HDFattrClose(atid))  < 0)
    return -1;
  MESSAGE("Fermeture de l'attribut")

  size[0] = 10;
  if ((did = HDFdatasetCreate(gid,"DATASET",HDF_INT32,size)) < 0)
    return -1;
  MESSAGE("Creation d'un dataset")

  if ((ret = HDFdatasetWrite(did,data)) < 0)
    return -1;
  MESSAGE("Ecriture du dataset")

  if ((ret = HDFdatasetClose(did)) < 0)
    return -1;
  MESSAGE("Fermeture du datset")

  if ((ret = HDFgroupClose(gid)) < 0)
    return -1;
  MESSAGE("Fermeture du groupe")

  if ((ret = HDFfileClose(fid)) < 0)
    return ret; 
  MESSAGE("Fermeture du fichier")

  if  ((fid = HDFfileCreate("toto.HDF")) < 0)
    return -1;
  MESSAGE("Creation du fichier")

  if ((ret = HDFfileClose(fid)) < 0)
    return ret; 
  MESSAGE("Fermeture du fichier")
  
  
  return 0;
}
