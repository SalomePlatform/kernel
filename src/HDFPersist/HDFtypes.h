/*----------------------------------------------------------------------------------
  SALOME HDFPersist : implementation of HDF persitent ( save/ restore )

  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
 
  This library is free software; you can redistribute it and/or 
  modify it under the terms of the GNU Lesser General Public 
  License as published by the Free Software Foundation; either 
  version 2.1 of the License. 
 
  This library is distributed in the hope that it will be useful, 
  but WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
  Lesser General Public License for more details. 
 
  You should have received a copy of the GNU Lesser General Public 
  License along with this library; if not, write to the Free Software 
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
 
  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 



  File   : HDFtypes.h
  Module : SALOME
----------------------------------------------------------------------------------*/

#ifndef HDFTYPES_H
#define HDFTYPES_H

#include <hdf5.h>

/* max length of a HDF object */
#define HDF_NAME_MAX_LEN  100

/* HDF 5 types */
typedef size_t         hdf_size_type;
typedef H5T_class_t    hdf_class_type;
typedef hsize_t        hdf_size;
typedef hid_t          hdf_idt;
typedef herr_t         hdf_err;
typedef hbool_t        hdf_bool;
typedef H5T_order_t    hdf_byte_order;

typedef int            hdf_int32;
typedef long           hdf_int64;
typedef double         hdf_float64;

/* Access Mode */
typedef enum {HDF_RDONLY,HDF_RDWR} hdf_access_mode; 

/* Values types for HDF datasets and attributes */
typedef enum {HDF_NONE,HDF_STRING, HDF_INT32, HDF_INT64, HDF_FLOAT64} hdf_type;
/* - HDF_STRING   : C string 
   - HDF_INT32    : 32 bits integer 
   - HDF_INT64    : 64 bits integer
   - HDF_FLOAT64  : IEEE  64 bits float
*/

/* HDF object types */
typedef enum {HDF_OBJECT,HDF_FILE,HDF_GROUP,HDF_DATASET,
	      HDF_ATTRIBUTE} hdf_object_type;

#endif /* HDFTYPES_H */
