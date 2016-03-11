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
//  File   : HDFgroup.cc
//  Module : SALOME
//
#include "hdfi.h"
#include <string.h>
#include "HDFgroup.hxx"
#include "HDFexception.hxx"

herr_t group_attr(hid_t loc_id, const char *attr_name, void *operator_data)
{
   *(char**)operator_data = new char[strlen(attr_name)+1];
   strcpy(*(char**)operator_data, attr_name);
   return 1;
}

HDFgroup::HDFgroup(const char *name, HDFcontainerObject *father)
  : HDFcontainerObject(name)
{
  _father = father;
  _fid = _father->GetId();
  _father->AddSon(this);
  _mid = -1;
  _attribute = NULL;
};

void HDFgroup::CreateOnDisk()
{
  if ((_id = HDFgroupCreate(_fid,_name)) < 0) 
    throw HDFexception("Can't create group");
}
  
void HDFgroup::OpenOnDisk()
{
  if ((_id = HDFgroupOpen(_fid,_name)) < 0)
    throw HDFexception("Can't open group");
}
  
void HDFgroup::CloseOnDisk()
{
  hdf_err ret;

  if ((ret = HDFgroupClose(_id)) < 0)
    throw HDFexception("Can't close group");
  _id = -1;
}

HDFcontainerObject *HDFgroup::GetFather()
{
  return _father;
}

hdf_object_type HDFgroup::GetObjectType()
{
  return HDF_GROUP;
}

int HDFgroup::nInternalObjects()
{
  int n = 0;
  hdf_err ret;   
  
  if ((ret = HDFnObjects(_fid,_name,&n)) < 0)
    throw HDFexception("Can't determine the number of internal objects");

  return  n;  
}

void HDFgroup::InternalObjectIndentify(int rank, char *object_name)
{
  hdf_err ret;

  if ((ret = HDFobjectIdentify(_fid,_name,rank,object_name)) < 0)
    throw HDFexception("Can't identify an internal object");
}

int HDFgroup::ExistInternalObject(const char *object_name)
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

hdf_object_type HDFgroup::InternalObjectType(char *object_name)
{
  hdf_object_type type;
  hdf_err ret;
  
  if ((ret = HDFobjectType(_id,object_name,&type)) < 0)
    throw HDFexception("Can't determine internal object type");

  return type;
}

void HDFgroup::FileMount(char *file,hdf_access_mode mode)
{
  hdf_err ret;

  if (_mid != -1)
    throw HDFexception("Can't mount the file");    

  if ((_mid = HDFfileOpen(file,mode)) < 0)
    throw HDFexception("Can't mount the file");

  if ((ret = HDFfileMount(_fid,_name,_mid)) < 0)
    throw HDFexception("Can't mount the file");
}

void HDFgroup::FileUnMount()
{
  hdf_err ret;

  if ((ret = HDFfileUmount(_fid,_name)) < 0)
        throw HDFexception("Can't unmount the file");

  if ((ret = HDFfileClose(_mid)) < 0)
        throw HDFexception("Can't unmount the file");

  _mid = -1;
}

int HDFgroup::nAttributes()
{
  int nbAttrs = H5Aget_num_attrs(_id);
  if(nbAttrs <= 0) nbAttrs = 0;
  return nbAttrs; 
}

char* HDFgroup::GetAttributeName(unsigned idx)
{
  int nbAttrs = nAttributes();
  if(nbAttrs == 0) return NULL;
  H5Aiterate(_id, &idx, group_attr, &_attribute);
  return _attribute;
}


 
