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
