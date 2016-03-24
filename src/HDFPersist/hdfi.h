// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

/*----------------------------------------------------------------------
  SALOME HDFPersist : implementation of HDF persitent ( save/ restore )
  File   : hdfi.h
  Module : SALOME
  ----------------------------------------------------------------------*/

#ifndef HDFI_H
#define HDFI_H

#include "HDFtypes.h"

/* HDF C INTERFACE */

#ifdef __cplusplus
extern "C" {
#endif
/* File Interface */
extern
hdf_idt HDFfileCreate(char *name);

extern
hdf_err HDFfileClose(hdf_idt fid);

extern
hdf_idt HDFfileOpen(char *name,hdf_access_mode access_mode);

extern
hdf_err HDFfileMount(hdf_idt id,const char *name,hdf_idt fid);

extern
hdf_err HDFfileUmount(hdf_idt id,const char *name);

/* Group Interface */
extern
hdf_idt HDFgroupCreate(hdf_idt pid, char *name);

extern
hdf_idt HDFgroupOpen(hdf_idt pid, char *name);

extern 
hdf_err HDFgroupClose(hdf_idt id);

/* DataSet Interface */
extern 
hdf_idt HDFdatasetOpen(hdf_idt pid,char *name);

extern
hdf_err HDFdatasetClose(hdf_idt id);

extern
hdf_idt HDFdatasetCreate(hdf_idt pid,char *name, hdf_type type,
                         hdf_size *dimd, int ndim, hdf_byte_order order, hdf_idt arrayId);

extern
hdf_err HDFdatasetWrite(hdf_idt id, void *val);

extern
hdf_err HDFdatasetRead(hdf_idt id, void *val);

extern
hdf_type HDFdatasetGetType(hdf_idt id);

extern
hdf_type HDFarrayGetType(hdf_idt id);

extern 
hdf_idt HDFarrayGetTypeId(hdf_idt type_id);

extern 
hdf_err HDFarrayClose(hdf_idt id);

extern 
int HDFarrayGetnDim(hdf_idt id);

extern 
hdf_err HDFarrayGetDim(hdf_idt id, hdf_size dim[]);


extern 
hdf_idt HDFarrayCreate(hdf_type dataType,int ndim,hdf_size *dim);

extern 
int HDFdatasetGetnDim(hdf_idt id);

extern 
hdf_err HDFdatasetGetDim(hdf_idt id,hdf_size dim[]);

extern
int HDFdatasetGetSize(hdf_idt id);

extern 
hdf_byte_order HDFdatasetGetOrder(hdf_idt id);

/* Attribute interface */
extern
hdf_idt HDFattrOpen(hdf_idt pid,char *name);

extern
hdf_err HDFattrClose(hdf_idt id);

extern
hdf_idt HDFattrCreate(hdf_idt pid,char *name,hdf_type type, size_t size);

extern
hdf_err HDFattrWrite(hdf_idt id,void *val);

extern
hdf_err HDFattrRead(hdf_idt id,void *val);

extern
hdf_type HDFattrGetType(hdf_idt id);

extern
hdf_type HDFattrGetSize(hdf_idt id);

/* HDF Tools */
extern
void HDFerrorModeLock();

extern
hdf_err
HDFnObjects(hdf_idt fid,const char *path,int *n);

extern
hdf_err HDFobjectIdentify(hdf_idt fid,const char *path,int i,char *name);

extern
hdf_err HDFobjectType(hdf_idt id, char *name, hdf_object_type *type);
#ifdef __cplusplus
}
#endif

#endif /* HDFI_H */
