#include "hdfi.h"

/*
 * - Name : _MEDdatasetRead
 * - Description : writes a HDF dataset
 * - Parameters :
 *     - id  (IN)     : dataset ID
 *     - val  (OUT)    : datset values
 * - Result : 
 *     - if success : 0
 *     - if failure : -1
 */ 
hdf_err HDFdatasetRead(hdf_idt id, void *val)
{
  hdf_idt datatype;
  hdf_err ret;

  if ((datatype = H5Dget_type(id)) < 0)
    return -1;

  if ((ret = H5Dread(id,datatype,H5S_ALL,H5S_ALL,
		      H5P_DEFAULT, val)) < 0)
    return -1;

  return 0;
}
