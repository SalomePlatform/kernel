#include <hdf5.h>
#include "hdfi.h"

hdf_err
HDFfileUmount(hdf_idt id,const char *name)
{
  hdf_err ret;

  ret = H5Funmount(id,name);

  return ret;
}
