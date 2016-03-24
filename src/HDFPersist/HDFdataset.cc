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
//  File   : HDFdataset.cc
//  Module : SALOME
//
#include "hdfi.h"
#include <string.h>
#include "HDFdataset.hxx"
#include "HDFcontainerObject.hxx"
#include "HDFexception.hxx"

#include <iostream>

herr_t dataset_attr(hid_t loc_id, const char *attr_name, void *operator_data)
{
  *(char**)operator_data = new char[strlen(attr_name)+1];
  strcpy(*(char**)operator_data, attr_name);
  return 1;
}

HDFdataset::HDFdataset(const char *name, HDFcontainerObject *father,hdf_type type, 
		       hdf_size dim[], int dimsize, hdf_byte_order order)
  : HDFinternalObject(name)
{
  int i;

  _father = father;
  _fid = _father->GetId();
  _father->AddSon(this);
  _type = type;
  _ndim = dimsize;
  _dim = new hdf_size[dimsize];
  _byte_order = order;
  _size = 1;
  _attribute = NULL;
  for (i=0;i<dimsize;i++)
    {
      _dim[i] = dim[i];
      _size = _size * _dim[i];
    }
  _arrayId = -1;
}


HDFdataset::HDFdataset(const char *name,HDFcontainerObject *father)
  : HDFinternalObject(name)
{
  _father = father;
  _fid = _father->GetId();
  _father->AddSon(this);
  _type = HDF_NONE;
  _ndim = -1;
  _dim = 0;
  _byte_order = H5T_ORDER_ERROR;
  _size = -1;
  _attribute = NULL;
  _arrayId = -1;
}

HDFdataset::~HDFdataset()
{
  delete [] _dim;
}

void HDFdataset::CreateOnDisk()
{
  if ((_id = HDFdatasetCreate(_fid,_name,_type,_dim,_ndim,_byte_order,_arrayId)) < 0)
    throw HDFexception("Can't create dataset");
}

void HDFdataset::OpenOnDisk()
{
  if ((_id = HDFdatasetOpen(_fid,_name)) < 0)
    throw HDFexception("Can't open dataset");
}

void HDFdataset::CloseOnDisk()
{
  hdf_err ret;

  if ((ret = HDFdatasetClose(_id)) < 0)
    throw HDFexception("Can't close dataset");
  _id = -1;
}

void HDFdataset::WriteOnDisk(void *values)
{
  hdf_err ret;

  if ((ret = HDFdatasetWrite(_id,values)) < 0)
    throw HDFexception("Can't write dataset");
 
}

void HDFdataset::ReadFromDisk(void *values)
{
  hdf_err ret;

  if ((ret = HDFdatasetRead(_id,values)) < 0)
      throw HDFexception("Can't read dataset");
}

HDFcontainerObject *HDFdataset::GetFather()
{
  return _father;
}

hdf_type HDFdataset::GetType()
{
  if (_type == HDF_NONE)
    if ((_type = HDFdatasetGetType(_id)) == HDF_NONE)
      throw HDFexception("Can't determine the type of data in the dataset");
  
  return _type;
}

int HDFdataset::nDim()
{
  if (_ndim == -1)
    if ((_ndim = HDFdatasetGetnDim(_id)) < 0)
      throw HDFexception("Can't determine the dataset dimensions number");

  return _ndim;
}

void HDFdataset::GetDim(hdf_size dim[])
{
  int i;
  int ndim;
  hdf_err ret;

  if (_dim == 0)
    {
      if (_ndim == -1)
	ndim = HDFdatasetGetnDim(_id);
      else
	ndim = _ndim;
      _dim = new hdf_size[ndim];
      if ((ret = HDFdatasetGetDim(_id,_dim)) < 0)
	throw HDFexception("Can't determine the size dimensions of the dataset ");
    }

  for (i=0;i<_ndim;i++)
    dim[i] = _dim[i];
}

hdf_size HDFdataset::GetSize()
{
  int size_type;

  if (_size == -1)
    {
      if ((_size = HDFdatasetGetSize(_id)) < 0)
	throw HDFexception("Can't determine the size of the dataset");
      
      if (_type == HDF_NONE)
	if ((_type = HDFdatasetGetType(_id)) == HDF_NONE)
	  throw HDFexception("Can't determine the size of the dataset");
      
      switch (_type)
	{
	case HDF_INT32 : 
	  size_type = 4;
	  break;
	  
	case HDF_INT64 :
	case HDF_FLOAT64 :
	  size_type = 8;
	  break;
	  
	default :
	  size_type = 1;
	}
      _size = _size / size_type;
    }

  return _size;
}

hdf_byte_order HDFdataset::GetOrder()
{
  if (_byte_order < 0 )
    if ((_byte_order = HDFdatasetGetOrder( _id )) < 0)
      throw HDFexception("Can't determine the byte order of the dataset");
  return _byte_order;
}

hdf_object_type HDFdataset::GetObjectType()
{
  return HDF_DATASET;
}


int HDFdataset::nAttributes()
{
  int nbAttrs = H5Aget_num_attrs(_id);
  if(nbAttrs <= 0) nbAttrs = 0;
  return nbAttrs; 
}


char* HDFdataset::GetAttributeName(unsigned idx)
{
  int nbAttrs = nAttributes();
  if(nbAttrs == 0) return NULL;
  H5Aiterate(_id, &idx, dataset_attr, &_attribute);
  return _attribute;
}

void HDFdataset::SetArrayId(hdf_idt arrayId) {
  _arrayId = arrayId;
}
