#ifndef HDFI_H
#define HDFI_H

#include "HDFtypes.h"

/* HDF C INTERFACE */

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
hdf_idt HDFdatasetCreate(hdf_idt pid,char *name,hdf_type type,
			 hdf_size *dimd, int ndim);

extern
hdf_err HDFdatasetWrite(hdf_idt id, void *val);

extern
hdf_err HDFdatasetRead(hdf_idt id, void *val);

extern
hdf_type HDFdatasetGetType(hdf_idt id);

extern 
int HDFdatasetGetnDim(hdf_idt id);

extern 
hdf_err HDFdatasetGetDim(hdf_idt id,hdf_size dim[]);

extern
int HDFdatasetGetSize(hdf_idt id);

/* Attribute interface */
extern
hdf_idt HDFattrOpen(hdf_idt pid,char *name);

extern
hdf_err HDFattrClose(hdf_idt id);

extern
hdf_idt HDFattrCreate(hdf_idt pid,char *name,hdf_type type);

extern
hdf_err HDFattrWrite(hdf_idt id,void *val);

extern
hdf_err HDFattrRead(hdf_idt id,void *val);

/* HDF Tools */
extern
void HDFerrorModeLock();

extern
hdf_err
HDFnObjects(hdf_idt fid,char *path,int *n);

extern
hdf_err HDFobjectIdentify(hdf_idt fid,char *path,int i,char *name);

extern
hdf_err HDFobjectType(hdf_idt id, char *name, hdf_object_type *type);

#endif /* HDFI_H */
