#include "hdfi.h"

/*
 * - Name : HDFdatasetOpen
 * - Description : open a HDF datset 
 * - Parameters :
 *     - pid     (IN)    : father ID
 *     - name    (IN)    : dataset name
 * - Result : 
 *     - if success : returns dateset ID
 *     - if failure : returns -1 
 */

hdf_idt
HDFdatasetOpen(hdf_idt pid,char *name)
{
  hdf_idt id;

  if ((id = H5Dopen(pid,name)) < 0)
    return -1;

  return id;
}
