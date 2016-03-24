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
//  File   : HDFcontainerObject.cc
//  Module : SALOME
//
#include "hdfi.h"
#include "HDFcontainerObject.hxx"
#include "HDFexception.hxx"

HDFcontainerObject::HDFcontainerObject(const char *name)
  : HDFinternalObject(name)
{
  _nsons = 0;
  _firstson = NULL;
  _lastson = NULL;
}

HDFcontainerObject::~HDFcontainerObject()
{
  HDFinternalObject *sonToDelete = 0;
  while(_firstson)
    {
      sonToDelete = _firstson;
      _firstson = _firstson->GetNextBrother();
      delete sonToDelete;
    }
}

int HDFcontainerObject::nInternalObjects()
{
  return 0;
}

void HDFcontainerObject::InternalObjectIndentify(int rank, char *object_name)
{
  object_name = NULL;
}

void HDFcontainerObject::AddSon(HDFinternalObject *son)
{
  if (_nsons == 0)
    {
      _firstson = son;
      _lastson = _firstson;
    }
  else
    {
      _lastson->SetNextBrother(son);
      son->SetPreviousBrother(_lastson);
      _lastson = son;
    }
  _nsons ++;
}

HDFinternalObject *HDFcontainerObject::GetFirstSon()
{
  return _firstson;
}

HDFinternalObject *HDFcontainerObject::GetLastSon()
{
  return _lastson;
}

int HDFcontainerObject::nSons()
{
  return _nsons;
}
