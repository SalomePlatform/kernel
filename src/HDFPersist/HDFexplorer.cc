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
//  File   : HDFexplorer.cc
//  Module : SALOME
//
#include "HDFcontainerObject.hxx"
#include "HDFexception.hxx"
#include "HDFinternalObject.hxx"
#include "HDFexplorer.hxx"

HDFexplorer::HDFexplorer(HDFcontainerObject *container)
{
  _container = container;
  _size = _container->nSons();
  _head = _container->GetFirstSon();
  _queue = _container->GetLastSon();
  _current = _head;
}

HDFexplorer::~HDFexplorer()
{
}

void HDFexplorer::Init()
{
  _current = _head;
}

void HDFexplorer::Next()
{
  _current = _current->GetNextBrother(); 
}

int HDFexplorer::More()
{
  if (_current != NULL)
    return 1;
  else
    return 0;
}

HDFinternalObject *HDFexplorer::Value()
{
  return _current;
}

void HDFexplorer::Reset(HDFcontainerObject *new_container)
{
  _container = new_container;
  _size = _container->nSons();
  _head = _container->GetFirstSon();
  _queue = _container->GetLastSon();
  _current = _head;
}
