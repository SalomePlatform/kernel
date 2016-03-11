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

//  File   : SALOMEDS_AttributeName.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_AttributeName.hxx"
#include "SALOMEDS.hxx"

#include <string>

SALOMEDS_AttributeName::SALOMEDS_AttributeName(SALOMEDSImpl_AttributeName* theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeName::SALOMEDS_AttributeName(SALOMEDS::AttributeName_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeName::~SALOMEDS_AttributeName()
{}

std::string SALOMEDS_AttributeName::Value()
{
  std::string aValue;
  if (_isLocal) {
    SALOMEDS::Locker lock; 
    aValue = dynamic_cast<SALOMEDSImpl_AttributeName*>(_local_impl)->Value();
  }
  else aValue = (CORBA::String_var)((SALOMEDS::AttributeName_var)SALOMEDS::AttributeName::_narrow(_corba_impl))->Value();
  return aValue;
}

void SALOMEDS_AttributeName::SetValue(const std::string& value)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock; 
    dynamic_cast<SALOMEDSImpl_AttributeName*>(_local_impl)->SetValue(value);
  }
  else ((SALOMEDS::AttributeName_var)SALOMEDS::AttributeName::_narrow(_corba_impl))->SetValue(value.c_str());
}
