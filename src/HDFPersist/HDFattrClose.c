#include "hdfi.h"

/*
 * - Name: HDFattrClose
 * - Description : close a HDF attribute
 * - Parameters :
 *     - id (IN)  : attribute ID
 * - Result : 
 *     - if success : 0 
 *     - if failure : -1
 */ 
hdf_err HDFattrClose(hdf_idt id)
{
  hdf_err ret;

  if ((ret = H5Aclose(id)) < 0)
    return -1;

  return 0;
}
