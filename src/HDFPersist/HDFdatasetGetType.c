#include "hdfi.h"
#include <hdf5.h>

hdf_type
HDFdatasetGetType(hdf_idt id)
{
  hdf_idt type_id;
  hdf_type type;
  hdf_class_type hdf_type;
  hdf_size_type size;

  if ((type_id = H5Dget_type(id)) < 0)
    return HDF_NONE;

  hdf_type = H5Tget_class(type_id);
  switch (hdf_type)
    {
    case H5T_INTEGER :
      size = H5Tget_size(type_id);
      if (size == 4)
	type = HDF_INT32;
      else
	type = HDF_INT64;
      break;

    case H5T_FLOAT :
      type = HDF_FLOAT64;
      break;

    case H5T_STRING :
      type = HDF_STRING;
      break;

    default :
      type = HDF_NONE;
    }

  return type;
}
