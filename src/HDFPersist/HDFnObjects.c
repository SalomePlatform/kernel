#include "hdfi.h"
#include <stdlib.h>

/*
 * - Name : HDFnObjects
 * - Description : returns the number of HDF objets in a HDF group
 * - Parameters :
 *     - fid     (IN)     : HDF file ID
 *     - path    (IN)     : acces path to the HDF group in the HDF file
 * - Results :
 *     - if success : number of objects
 *     - if failure : -1 
 */ 

hdf_err Num(hdf_idt id,const char *name, void *data);

hdf_err
HDFnObjects(hdf_idt fid,char *path,int *n)
{
  int idx;
  int m = 0;

  if ((idx  = H5Giterate(fid,path,NULL,Num,(void*)&m)) < 0)
    return -1;

  *n = (int) m;

  return 0;
}

hdf_err Num(hdf_idt id,const char *name, void *data)
{
  int *count;
  
  count = (int *) data;
  (*count)++;

  return 0;
}

