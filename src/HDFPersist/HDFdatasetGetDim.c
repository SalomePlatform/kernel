#include <hdf5.h>
#include "hdfi.h"
#include <stdlib.h>

hdf_err 
HDFdatasetGetDim(hdf_idt id,hdf_size dim[])
{
  hdf_err ret = 0;
  hdf_size *tmp;
  int ndim;
  hdf_idt space_id;

  if ((space_id = H5Dget_space(id)) < 0)
    return -1;

  if ((ndim = H5Sget_simple_extent_ndims(space_id)) < 0)
    return -1;

  tmp = (hdf_size *) malloc(sizeof(hdf_size)*ndim);
  ret = H5Sget_simple_extent_dims(space_id, dim, tmp);
  ret =  H5Sclose(space_id);
  
 free (tmp);

  return ret;
}

				  
