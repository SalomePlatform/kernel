// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
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


#include "SALOMEDSImpl_ChildIterator.hxx"
#include "SALOMEDSImpl_Study.hxx"

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_ChildIterator, MMgt_TShared )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_ChildIterator, MMgt_TShared )

SALOMEDSImpl_ChildIterator::SALOMEDSImpl_ChildIterator(const Handle(SALOMEDSImpl_SObject)& theSO)
{
  _so  = theSO;
  _it = TDF_ChildIterator(_so->GetLabel());
}

SALOMEDSImpl_ChildIterator::SALOMEDSImpl_ChildIterator(const TDF_Label& theLabel)
{
  _so  = SALOMEDSImpl_Study::SObject(theLabel);
  _it = TDF_ChildIterator(theLabel);
}

void SALOMEDSImpl_ChildIterator::Init()
{
  _it.Initialize(_so->GetLabel(), Standard_False);
}

void SALOMEDSImpl_ChildIterator::Init(const TDF_Label& theLabel)
{
  _it.Initialize(theLabel, Standard_False);
}

void SALOMEDSImpl_ChildIterator::InitEx(bool theAllLevels)
{
  _it.Initialize(_so->GetLabel(), theAllLevels);
}

void SALOMEDSImpl_ChildIterator::InitEx(const TDF_Label& theLabel, bool theAllLevels)
{
  _it.Initialize(theLabel, theAllLevels);
}

bool SALOMEDSImpl_ChildIterator::More()
{
  return _it.More();
}

void SALOMEDSImpl_ChildIterator::Next() 
{
  _it.Next();
}

Handle(SALOMEDSImpl_SObject) SALOMEDSImpl_ChildIterator::Value()
{
  return SALOMEDSImpl_Study::SObject(_it.Value());
}

TDF_Label SALOMEDSImpl_ChildIterator::Label()
{
  return _it.Value();
}
