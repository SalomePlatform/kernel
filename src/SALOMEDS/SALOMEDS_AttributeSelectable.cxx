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

//  File   : SALOMEDS_AttributeSelectable.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_AttributeSelectable.hxx"
#include "SALOMEDS.hxx"

SALOMEDS_AttributeSelectable::SALOMEDS_AttributeSelectable(SALOMEDSImpl_AttributeSelectable* theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeSelectable::SALOMEDS_AttributeSelectable(SALOMEDS::AttributeSelectable_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeSelectable::~SALOMEDS_AttributeSelectable()
{}

bool SALOMEDS_AttributeSelectable::IsSelectable()
{
  bool aValue;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aValue = (bool)dynamic_cast<SALOMEDSImpl_AttributeSelectable*>(_local_impl)->IsSelectable();
  }
  else aValue = SALOMEDS::AttributeSelectable::_narrow(_corba_impl)->IsSelectable();
  return aValue;
}

void SALOMEDS_AttributeSelectable::SetSelectable(bool value)
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    dynamic_cast<SALOMEDSImpl_AttributeSelectable*>(_local_impl)->SetSelectable((int)value);
  }
  else SALOMEDS::AttributeSelectable::_narrow(_corba_impl)->SetSelectable(value);
}
