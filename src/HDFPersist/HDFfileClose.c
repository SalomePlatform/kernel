#include "hdfi.h"

/*
 * - Name : HDFfileClose
 * - Description : closes a HDF file
 * - Parameters :
 *     - fid (IN) : file ID
 * - Result : 
 *     - success : 0
 *     - failure : -1 
 */ 

hdf_err HDFfileClose(hdf_idt fid)
{
  hdf_err ret;

  H5Fflush(fid,H5F_SCOPE_GLOBAL);

  if ((ret = H5Fclose(fid)) < 0)
    return -1;

  return 0;
}
