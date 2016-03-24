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

//  SALOME HDFPersist : implementation of HDF persitent ( save/ restore )
//  File   : HDFfile.cc
//  Module : SALOME
//
#include "hdfi.h"
#ifndef WIN32
#include <unistd.h>
#else
#include <io.h>
#define F_OK 0
#define access _access
#endif
#include <string.h>
#include <iostream>
#include "HDFfile.hxx"
#include "HDFexception.hxx"

herr_t file_attr(hid_t loc_id, const char *attr_name, void *operator_data)
{
   *(char**)operator_data = new char[strlen(attr_name)+1];
   strcpy(*(char**)operator_data, attr_name);
   return 1;
}

HDFfile::HDFfile(char *name)
  : HDFcontainerObject(name) 
{
  _attribute = NULL;
}

void HDFfile::CreateOnDisk()
{
  if ((_id = HDFfileCreate(_name)) < 0) 
    throw HDFexception("Can't create HDF file");
}

void HDFfile::OpenOnDisk(hdf_access_mode access_mode)
{
  _access_mode = access_mode;

  switch (_access_mode)
    {
    case HDF_RDWR :
      if (access(_name,F_OK))
	{
	  if ((_id = HDFfileCreate(_name)) < 0) 
	    throw HDFexception("Can't open HDF file");
	}
      else
	if ((_id = HDFfileOpen(_name,_access_mode)) < 0)
	  throw HDFexception("Can't open HDF file");
      break;
      
    case HDF_RDONLY :
      if ((_id = HDFfileOpen(_name,_access_mode)) < 0)
	throw HDFexception("Can't open HDF file");
      break;
      
    default :
      throw HDFexception("Can't open HDF file : bad acces option");
    }
}

void HDFfile::CloseOnDisk()
{
  hdf_err ret;

  if ((ret = HDFfileClose(_id)) < 0) 
    throw HDFexception("Can't close HDF file");
  _id = -1;
}
  

hdf_access_mode HDFfile::GetAccessMode()
{
  return _access_mode;
}

hdf_object_type HDFfile::GetObjectType()
{
  return HDF_FILE;
}

int HDFfile::nInternalObjects()
{
  int n;
  hdf_err ret;   

  if ((ret = HDFnObjects(_id,"/",&n)) < 0)
    throw HDFexception("Can't determine the number of internal objects");

  return  n;
}

void HDFfile::InternalObjectIndentify(int rank, char *object_name)
{
  hdf_err ret;

  if ((ret = HDFobjectIdentify(_id,"/",rank,object_name)) < 0)
    throw HDFexception("Can't identify an internal object");
}

int HDFfile::ExistInternalObject(const char *object_name)
{
  int n,i;
  int ret = 0;
  char name[HDF_NAME_MAX_LEN+1];

  n = this->nInternalObjects(); 
  for (i=0;i<n;i++) 
    {
      this->InternalObjectIndentify(i,name);
      if (!strcmp(name,object_name))
	{
	  ret = 1;
	  break;
	}
    }  
  return ret;
}

hdf_object_type HDFfile::InternalObjectType(char *object_name)
{
  hdf_object_type type;
  hdf_err ret;
  
  if ((ret = HDFobjectType(_id,object_name,&type)) < 0)
    throw HDFexception("Can't determine internal object type");

  return type;
}

int HDFfile::nAttributes()
{
  int nbAttrs = H5Aget_num_attrs(_id);
  if(nbAttrs <= 0) nbAttrs = 0;
  return nbAttrs; 
}

char* HDFfile::GetAttributeName(unsigned idx)
{
  int nbAttrs = nAttributes();
  if(nbAttrs == 0) return NULL;
  H5Aiterate(_id, &idx, file_attr, &_attribute);
  return _attribute;
}
