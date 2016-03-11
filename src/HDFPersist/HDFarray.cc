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
//  File   : HDFarray.cc
//  Module : SALOME
//
#include "HDFarray.hxx"
#include "HDFexception.hxx"
#include "hdfi.h"


HDFarray::HDFarray( HDFobject* father, hdf_type dataType, int ndim, hdf_size *dim )
  : HDFobject("")
{
  _father = father;
  _datatype = dataType;
  _ndim = ndim;
  _dim = dim;
}

HDFarray::HDFarray( HDFobject* father)
  : HDFobject("")
{
  _father = father;
  _datatype = HDF_NONE;
  _ndim = -1;
  _dim  = 0;

  if( father )
    _id = HDFarrayGetTypeId(father->GetId());
  
}


HDFarray::~HDFarray()
{
  if(_dim )
    delete [] _dim;
}

hdf_type HDFarray::GetDataType() {
  if (_datatype == HDF_NONE)
    if ((_datatype = HDFarrayGetType(_id)) == HDF_NONE )
      throw HDFexception("Can't determine the type of array data");
  return _datatype;
}


void HDFarray::CreateOnDisk() {
  if( (_id = HDFarrayCreate(_datatype,_ndim, _dim)) < 0 )
    throw HDFexception("Can't create HDF array");
}

void HDFarray::CloseOnDisk() {
  if( HDFarrayClose(_id) < 0 )
    throw HDFexception("Can't close HDF array");
}

int HDFarray::nDim() {
  if (_ndim == -1)
    if ((_ndim = HDFarrayGetnDim(_id)) < 0)
      throw HDFexception("Can't determine the array dimensions number");

  return _ndim;

}


void HDFarray::GetDim(hdf_size dim[]) {
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
      if ((ret = HDFarrayGetDim(_id,_dim)) < 0)
	throw HDFexception("Can't determine the size dimensions of the array ");
    }

  for (i=0;i<_ndim;i++)
    dim[i] = _dim[i];
}


hdf_object_type HDFarray::GetObjectType() {
  return HDF_ARRAY_TYPE;
}
