#include "hdfi.h"

/*
 * - Name : HDFgroupClose
 * - Description : close a HDF group 
 * - Parameters :
 *     - id     (IN)    : group ID
 * - Result : 
 *     - if success : returns  0
 *     - if failure : returns -1 
 */

hdf_err 
HDFgroupClose(hdf_idt id)
{
  hdf_err ret;

  if ((ret = H5Gclose(id)) < 0)
    return -1;
  
  return 0;
}
