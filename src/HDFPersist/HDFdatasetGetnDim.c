#include "hdfi.h"
#include <hdf5.h>

int 
HDFdatasetGetnDim(hdf_idt id)
{
  int ndim;
  hdf_idt space_id;
  hdf_err ret;

  if ((space_id = H5Dget_space(id)) < 0)
    return -1;

  if ((ndim = H5Sget_simple_extent_ndims(space_id)) < 0)
    return -1;

  if ((ret = H5Sclose(space_id)) < 0)
    return -1;

  return ndim;
}
