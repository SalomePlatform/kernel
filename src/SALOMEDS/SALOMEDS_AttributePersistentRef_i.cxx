//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : SALOMEDS_AttributePersistentRef_i.cxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#include "SALOMEDS_AttributePersistentRef_i.hxx"
#include "SALOMEDS.hxx"

#include <TCollection_ExtendedString.hxx>
#include <TCollection_AsciiString.hxx>

using namespace std;

char* SALOMEDS_AttributePersistentRef_i::Value()
{
  SALOMEDS::Locker lock;

  TCollection_ExtendedString S = Handle(SALOMEDS_PersRefAttribute)::DownCast(_myAttr)->Get();
  CORBA::String_var c_s = CORBA::string_dup(TCollection_AsciiString(S).ToCString());
  return c_s._retn();
}

void SALOMEDS_AttributePersistentRef_i::SetValue(const char* value) 
{
  SALOMEDS::Locker lock;

  CheckLocked();
  CORBA::String_var Str = CORBA::string_dup(value);
  Handle(TDataStd_Comment)::DownCast(_myAttr)->Set(TCollection_ExtendedString(Str));
}

char* SALOMEDS_AttributePersistentRef_i::Store() {
  SALOMEDS::Locker lock;

  return Value();
}

void SALOMEDS_AttributePersistentRef_i::Restore(const char* value) {
  SALOMEDS::Locker lock;

  SetValue(value);
}
