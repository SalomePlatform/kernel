#include "hdfi.h"

/*
 * - Name : HDFgroupOpen
 * - Description : open an existing HDF group 
 * - Parameters :
 *     - pid     (IN)    : father ID
 *     - name    (IN)    : group name
 * - Result : 
 *     - if success : returns group ID
 *     - if failure : returns -1 
 */
 
hdf_idt
HDFgroupOpen(hdf_idt pid, char *name)
{
  hdf_idt id;

  if ((id = H5Gopen(pid,name)) < 0)
    return -1;

  return id;
}
