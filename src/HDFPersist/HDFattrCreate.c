#include "hdfi.h"

/*
 * - Name : HDFattrCreate
 * - Description : create a HDF attribute
 * - Parameters :
 *     - pid (IN) : father ID
 *     - name (IN) : attribute name
 *     - type (IN) : attribute type
 * - Result: 
 *     - if success : attribute ID 
 *     - if failure : -1
 */
hdf_idt HDFattrCreate(hdf_idt pid,char *name,hdf_type type)
{
  hdf_idt aid,attr;
  hdf_err ret;
  int type_hdf;

  switch(type) {
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

  if ((aid = H5Screate(H5S_SCALAR)) < 0)
    return -1;
  
  if ((attr = H5Acreate(pid,name,type_hdf,aid,H5P_DEFAULT)) < 0)
    return -1;
  
  if ((ret = H5Sclose(aid)) < 0)
    return -1;
  
  return attr;
}
