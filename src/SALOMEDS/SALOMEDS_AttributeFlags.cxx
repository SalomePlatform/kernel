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

//  File   : SALOMEDS_AttributeFlags.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_AttributeFlags.hxx"
#include "SALOMEDS.hxx"

SALOMEDS_AttributeFlags::SALOMEDS_AttributeFlags(SALOMEDSImpl_AttributeFlags* theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeFlags::SALOMEDS_AttributeFlags(SALOMEDS::AttributeFlags_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeFlags::~SALOMEDS_AttributeFlags()
{}

int SALOMEDS_AttributeFlags::GetFlags()
{
  int aValue;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aValue = dynamic_cast<SALOMEDSImpl_AttributeFlags*>(_local_impl)->Get();
  }
  else aValue = SALOMEDS::AttributeFlags::_narrow(_corba_impl)->GetFlags();
  return aValue;
}

void SALOMEDS_AttributeFlags::SetFlags(int theFlags)
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    dynamic_cast<SALOMEDSImpl_AttributeFlags*>(_local_impl)->Set(theFlags);
  }
  else SALOMEDS::AttributeFlags::_narrow(_corba_impl)->SetFlags(theFlags);
}

bool SALOMEDS_AttributeFlags::Get(int theFlag)
{
  return (GetFlags() & theFlag) ? true : false;
}
 
void SALOMEDS_AttributeFlags::Set(int theFlag, bool theValue)
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeFlags* anAttr = dynamic_cast<SALOMEDSImpl_AttributeFlags*>(_local_impl);
    if (theValue)
      anAttr->Set(anAttr->Get() | theFlag);
    else
      anAttr->Set(anAttr->Get() & ~theFlag);    
  }
  else SALOMEDS::AttributeFlags::_narrow(_corba_impl)->Set(theFlag, theValue);
}
