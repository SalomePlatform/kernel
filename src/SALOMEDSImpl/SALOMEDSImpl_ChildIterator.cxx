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

//  File   : SALOMEDSImpl_ChildIterator.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_ChildIterator.hxx"
#include "SALOMEDSImpl_Study.hxx"

SALOMEDSImpl_ChildIterator::SALOMEDSImpl_ChildIterator(const SALOMEDSImpl_SObject& theSO)
{
  _so  = theSO;
  _it = DF_ChildIterator(_so.GetLabel());
}

SALOMEDSImpl_ChildIterator::SALOMEDSImpl_ChildIterator(const DF_Label& theLabel)
{
  _so  = SALOMEDSImpl_Study::SObject(theLabel);
  _it = DF_ChildIterator(theLabel);
}

void SALOMEDSImpl_ChildIterator::Init()
{
  if(_so) _it.Init(_so.GetLabel(), false);
}

void SALOMEDSImpl_ChildIterator::Init(const DF_Label& theLabel)
{
  _it.Init(theLabel, false);
}

void SALOMEDSImpl_ChildIterator::InitEx(bool theAllLevels)
{
  if(_so) _it.Init(_so.GetLabel(), theAllLevels);
}

void SALOMEDSImpl_ChildIterator::InitEx(const DF_Label& theLabel, bool theAllLevels)
{
  _it.Init(theLabel, theAllLevels);
}

bool SALOMEDSImpl_ChildIterator::More()
{
  return _it.More();
}

void SALOMEDSImpl_ChildIterator::Next() 
{
  _it.Next();
}

SALOMEDSImpl_SObject SALOMEDSImpl_ChildIterator::Value()
{
  if(!_so) return SALOMEDSImpl_SObject();
  return SALOMEDSImpl_Study::SObject(_it.Value());
}

DF_Label SALOMEDSImpl_ChildIterator::Label()
{
  return _it.Value();
}

SALOMEDSImpl_ChildIterator* SALOMEDSImpl_ChildIterator::GetPersistentCopy() const
{
  SALOMEDSImpl_ChildIterator* itr = new SALOMEDSImpl_ChildIterator();
  itr->_it = _it;
  itr->_so = _so;
  return itr;
}

