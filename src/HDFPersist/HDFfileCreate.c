#include "hdfi.h"

/*
 * - Name : HDFfileCreate
 * - Description : creates a HDF file
 * - Parameters :
 *     - name (IN) : file name
 * - Result : 
 *     - success : file ID
 *     - failure : -1 
 */ 
hdf_idt HDFfileCreate(char *name)
{
  hdf_idt fid;

  if ((fid = H5Fcreate(name,H5F_ACC_TRUNC,
			       H5P_DEFAULT,H5P_DEFAULT)) < 0)
    return -1;

  return fid;
}
