#include "hdfi.h"

/*
 * - Name: HDFattrOpen
 * - Description : open a HDF attribute
 * - Parameters :
 *     - pid (IN)  : father ID
 *     - name  (IN)  : attribute name 
 * - Result : 
 *     - if success : attribute ID
 *     - if failure : -1 
 */ 
hdf_idt HDFattrOpen(hdf_idt pid,char * name)
{
   hdf_idt aid;

   if ((aid = H5Aopen_name(pid,name)) < 0)
     return -1;

   return aid;
}
