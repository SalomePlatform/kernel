#include "hdfi.h"

hdf_err 
HDFobjectType(hdf_idt id, char *name, hdf_object_type *type)
{
  hdf_object_type rep;
  hdf_bool follow = 1;
  H5G_stat_t buf;
  hdf_err ret;

  if ((ret = H5Gget_objinfo(id,name,follow,&buf) < 0))
    return -1; 
  
  switch(buf.type)
    {
    case H5G_GROUP :
      *type = HDF_GROUP;
      break;

    case H5G_DATASET :
      *type = HDF_DATASET;
      break;
     
    default :
      return -1;
    }

  return 0;
}
