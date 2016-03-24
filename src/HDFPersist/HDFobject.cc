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
//  File   : HDFobject.cc
//  Module : SALOME
//
#include "HDFobject.hxx"
#include "hdfi.h"

#include <cstring>
#include <stdlib.h>

#ifdef WIN32
#define strdup _strdup
#endif

HDFobject::HDFobject(const char *name)
{
  HDFerrorModeLock();
  _name = strdup(name);
  _id = -1;
}

HDFobject::~HDFobject()
{
  free(_name);
}

hdf_idt HDFobject::GetId()
{
  return _id;
}

char *HDFobject::GetName()
{
  return _name;
}

hdf_object_type HDFobject::GetObjectType()
{
  return HDF_OBJECT;
}
