#include "hdfi.h"

/*
 * - Name : HDFgroupCreate
 * - Description : create a HDF group 
 * - Parameters :
 *     - pid     (IN)    : father ID
 *     - name    (IN)    : group name
 * - Result : 
 *     - if success : returns group ID
 *     - if failure : returns -1 
 */

hdf_idt
HDFgroupCreate(hdf_idt pid, char *name)
{
  hdf_idt id;

  if ((id = H5Gcreate(pid,name,0)) < 0)
    return -1;

  return id;
}
