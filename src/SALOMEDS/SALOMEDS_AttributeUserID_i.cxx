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
//  File   : SALOMEDS_AttributeName_i.cxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

using namespace std;
#include "SALOMEDS_AttributeUserID_i.hxx"
#include <TCollection_ExtendedString.hxx>
#include "SALOMEDS_SObject_i.hxx"

char* SALOMEDS_AttributeUserID_i::Value() {
  char aGUID[40];
  Handle(TDataStd_UAttribute)::DownCast(_myAttr)->ID().ToCString(aGUID);
  CORBA::String_var c_s = CORBA::string_dup(aGUID);
  return c_s._retn();
}

void SALOMEDS_AttributeUserID_i::SetValue(const char* value) {
  CheckLocked();
  CORBA::String_var Str = CORBA::string_dup(value);
  Handle(TDataStd_UAttribute)::DownCast(_myAttr)->SetID(Standard_GUID(Standard_CString(Str)));
}

char* SALOMEDS_AttributeUserID_i::Store() {
  return "";
}

void SALOMEDS_AttributeUserID_i::Restore(const char* value) {
}
