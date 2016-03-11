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

//  File   : SALOMEDS_AttributeExternalFileDef.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_AttributeExternalFileDef.hxx"
#include "SALOMEDS.hxx"
#include <string>

SALOMEDS_AttributeExternalFileDef::SALOMEDS_AttributeExternalFileDef(SALOMEDSImpl_AttributeExternalFileDef* theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeExternalFileDef::SALOMEDS_AttributeExternalFileDef(SALOMEDS::AttributeExternalFileDef_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeExternalFileDef::~SALOMEDS_AttributeExternalFileDef()
{}

std::string SALOMEDS_AttributeExternalFileDef::Value()
{
  std::string aValue;
  if (_isLocal) {
    SALOMEDS::Locker lock; 
    aValue = dynamic_cast<SALOMEDSImpl_AttributeExternalFileDef*>(_local_impl)->Value();
  }
  else aValue = SALOMEDS::AttributeExternalFileDef::_narrow(_corba_impl)->Value();
  return aValue;
}
 
void SALOMEDS_AttributeExternalFileDef::SetValue(const std::string& value)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock; 
    dynamic_cast<SALOMEDSImpl_AttributeExternalFileDef*>(_local_impl)->SetValue(value);
  }
  else SALOMEDS::AttributeExternalFileDef::_narrow(_corba_impl)->SetValue(value.c_str());
}
