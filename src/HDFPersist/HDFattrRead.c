#include "hdfi.h"

/*
 * - Name : HDFattrWrite
 * - Description : write a value to a HDF attribute
 * - Parameters :
 *     - id   (IN) : attribute ID
 *     - val  (OUT) : attribute value
 * - Result : 
 *     - if success : 0
 *     - if failure : -1
 */
hdf_err HDFattrRead(hdf_idt id,void *val)
{
  hdf_err ret;
  int type_hdf;

  if ((type_hdf = H5Aget_type(id)) < 0)
    return -1;

  if ((ret = H5Aread(id,type_hdf,val)) < 0)
    return -1;

  return 0;
}
