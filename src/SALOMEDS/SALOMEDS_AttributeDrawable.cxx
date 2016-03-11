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

//  File   : SALOMEDS_AttributeDrawable.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_AttributeDrawable.hxx"
#include "SALOMEDS.hxx"

SALOMEDS_AttributeDrawable::SALOMEDS_AttributeDrawable(SALOMEDSImpl_AttributeDrawable* theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeDrawable::SALOMEDS_AttributeDrawable(SALOMEDS::AttributeDrawable_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeDrawable::~SALOMEDS_AttributeDrawable()
{}

bool SALOMEDS_AttributeDrawable::IsDrawable()
{
  bool aValue;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aValue = (bool)dynamic_cast<SALOMEDSImpl_AttributeDrawable*>(_local_impl)->IsDrawable();
  }
  else aValue = SALOMEDS::AttributeDrawable::_narrow(_corba_impl)->IsDrawable();
  return aValue;
}

void SALOMEDS_AttributeDrawable::SetDrawable(bool value)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    dynamic_cast<SALOMEDSImpl_AttributeDrawable*>(_local_impl)->SetDrawable((int)value);
  }
  else SALOMEDS::AttributeDrawable::_narrow(_corba_impl)->SetDrawable(value);
}
