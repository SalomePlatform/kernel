#include"hdfi.h"
#include<stdlib.h>

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
