#include "hdfi.h"

/*
 * - Name : HDFdatasetCreate
 * - Description : creates a HDF dataset
 * - Parameters :
 *     - pid  (IN)     : father ID
 *     - name (IN)     : dataset name
 *     - type (IN)     : dataset type (HDF_STRING,HDF_INT32,HDF_INT64,HDF_FLOAT64)
 *     - dimd (IN)     : dataset size
 * - Result : 
 *     - if success : returns dataset ID
 *     - if failure : -1
 */ 
hdf_idt HDFdatasetCreate(hdf_idt pid,char *name,hdf_type type,
			 hdf_size *dimd, int ndim)
{
  hdf_idt dataset, dataspace = 0;
  hdf_err ret;
  int type_hdf;

  switch(type)
    {
    case HDF_FLOAT64 :
      type_hdf = H5T_IEEE_F64LE;
      break;

    case HDF_INT32 :
      type_hdf = H5T_STD_I32LE;
      break;
 
    case HDF_INT64 :
      type_hdf = H5T_STD_I64LE;
      break;

    case HDF_STRING :           
      if((type_hdf = H5Tcopy(H5T_C_S1)) < 0)
	return -1;
      if((ret = H5Tset_size(type_hdf,1)) < 0)
	return -1;
      break;

    default :
      return -1;
    }

if ((dataset = H5Dopen(pid,name)) < 0)
    {

     if ((dataspace = H5Screate_simple(ndim,dimd,NULL)) < 0)
	return -1;
     if ((dataset = H5Dcreate(pid,name,type_hdf,dataspace,
			       H5P_DEFAULT)) < 0)
	return -1;
    }
  else
    return -1;

  if ((ret = H5Sclose(dataspace)) < 0)
    return -1;           

  return dataset;
}
