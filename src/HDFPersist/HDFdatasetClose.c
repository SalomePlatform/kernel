#include "hdfi.h"

/*
 * - Name : HDFdatasetClose
 * - Description : close a HDF datset 
 * - Parameters :
 *     - id     (IN)    : dateset ID
 * - Result : 
 *     - if success : returns 0
 *     - if failure : returns -1 
 */

hdf_err
HDFdatasetClose(hdf_idt id)
{
  hdf_err ret;

  if ((ret = H5Dclose(id)) < 0)
    return -1;
  
  return 0;
}
