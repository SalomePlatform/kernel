#include "hdfi.h"

/*
 * - Name: HDFobjectIdentify
 * - Description : find the name of the HDF object of rank "i" in the HDF
 *     group "path"
 * - Parameters :
 *     - fid     (IN)     : l'ID du fichier o
 *     - path  (IN)       : group access path
 *     - i (IN)           : access rank
 *     - name (OUT)       : expected name
 * - Result : 
 *     - if sucess : 0
 *     - if failure : -1
 */ 

hdf_err RankInfo(hdf_idt id, const char *name, void *data);

hdf_err HDFobjectIdentify(hdf_idt fid,char *path,int i,char *name)
{
  int idx;

  if ((idx = H5Giterate(fid,path,&i,RankInfo,
			name)) < 0)
    return -1;

  return 0;
}


hdf_err RankInfo(hdf_idt id, const char *name, void *data)
{
  if (data != NULL)
    strcpy(data,name);
  else 
    return -1;

  return 1;
}
