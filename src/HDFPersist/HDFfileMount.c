#include <hdf5.h>
#include "hdfi.h"

hdf_err
HDFfileMount(hdf_idt id,const char *name,hdf_idt fid)
{
  hdf_err ret = 0;

  ret = H5Fmount(id, name, fid,H5P_DEFAULT);
    
  return ret;
} 
