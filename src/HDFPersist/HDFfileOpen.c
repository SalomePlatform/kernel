#include "hdfi.h"
/*
 * - Name : HDFfileOpen
 * - Description : open an existing HDF file
 * - Parameters :
 *     - name (IN)        : file name
 *     - access_mode (IN) : file access mode 
 * - Result : 
 *     - success : file ID
 *     - failure : -1 
 */ 

hdf_idt HDFfileOpen(char *name,hdf_access_mode access_mode)
{ 
  hdf_idt fid;
  int mode;

  switch(access_mode)
    {
    case HDF_RDONLY:
      mode = H5F_ACC_RDONLY;
      break;

    case HDF_RDWR :
      mode = H5F_ACC_RDWR; 
      break;

    default :
      return -1;
    }  

  if ((fid = H5Fopen(name,mode,H5P_DEFAULT)) < 0)
    return -1;

  return fid;
}
