#include <hdf5.h>
#include "hdfi.h"

int HDFdatasetGetSize(hdf_idt id)
{
  hdf_size size;

  if ((size = H5Dget_storage_size(id)) < 0)
    return -1;

  return (int) size;
}
