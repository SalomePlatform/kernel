#include "hdfi.h"

/*
 * - Name : HDFerrorModeLock
 * - Description : lock HDF error mode
 * - Parameters : 
 * - Result : aucun
 */
void HDFerrorModeLock()
{
  H5Eset_auto(NULL,NULL);
}
