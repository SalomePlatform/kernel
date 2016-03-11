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

//  File   : SALOMEDS_AttributePixMap_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_AttributePixMap_i.hxx"
#include "SALOMEDS.hxx"

CORBA::Boolean SALOMEDS_AttributePixMap_i::HasPixMap()
{
  SALOMEDS::Locker lock;
  return dynamic_cast<SALOMEDSImpl_AttributePixMap*>(_impl)->HasPixMap();
}

char* SALOMEDS_AttributePixMap_i::GetPixMap()
{
  SALOMEDS::Locker lock;
  CORBA::String_var S =
    CORBA::string_dup(dynamic_cast<SALOMEDSImpl_AttributePixMap*>(_impl)->GetPixMap().c_str());
  return S._retn();
}

void SALOMEDS_AttributePixMap_i::SetPixMap(const char* value)
{
  SALOMEDS::Locker lock;
  CheckLocked();
  CORBA::String_var Str = CORBA::string_dup(value);
  dynamic_cast<SALOMEDSImpl_AttributePixMap*>(_impl)->SetPixMap(std::string(Str));
}
