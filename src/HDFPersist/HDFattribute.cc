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
//  File   : HDFattribute.cc
//  Module : SALOME
//
#include "hdfi.h"
#include "HDFexception.hxx"
#include "HDFattribute.hxx"
#include "HDFinternalObject.hxx"

HDFattribute::HDFattribute(char *name,HDFinternalObject *father,hdf_type type, size_t size)
  : HDFobject(name)
{
  _father = father;
  _fid = _father->GetId();
  _type = type;
  _size = size;
}

HDFattribute::HDFattribute(char *name,HDFinternalObject *father)
  : HDFobject(name)
{
  _father = father;
  _fid = _father->GetId();
  _type = HDF_NONE;
  _size = 0;
}

void HDFattribute::CreateOnDisk()
{
  if ((_id = HDFattrCreate(_fid,_name, _type, _size)) < 0)
    throw HDFexception("Can't create attribute");
}
void HDFattribute::OpenOnDisk()
{
  if ((_id = HDFattrOpen(_fid,_name)) < 0)
    throw HDFexception("Can't open attribute");
}
void HDFattribute::CloseOnDisk()
{
  hdf_err ret;

  if ((ret = HDFattrClose(_id)) < 0)
      throw HDFexception("Can't close attribute");
}

void HDFattribute::WriteOnDisk(void *values)
{
  hdf_err ret;

  if ((ret = HDFattrWrite(_id,values)) < 0)
    throw HDFexception("Can't write attribute");
}

void HDFattribute::ReadFromDisk(void *values)
{
  hdf_err ret; 

  if ((ret = HDFattrRead(_id,values)) < 0)
    throw HDFexception("Can't read attribute");
}

HDFinternalObject * HDFattribute::GetFather()
{
  return _father;
}

hdf_type HDFattribute::GetType()
{
 if (_type == HDF_NONE)
    if ((_type = HDFattrGetType(_id)) == HDF_NONE)
      throw HDFexception("Can't determine the type of data in the attribute");

  return _type;
}

size_t HDFattribute::GetSize()
{
  if(_size == 0) {
    if((_size = HDFattrGetSize(_id)) < 0)
      throw HDFexception("Can't determine the size of data in the attribute");
  }
  return _size;
}
