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
//  File   : SALOMEDS_AttributeDrawable_i.cxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#include "SALOMEDS_AttributeDrawable_i.hxx"
#include "SALOMEDS.hxx"

using namespace std;

CORBA::Boolean SALOMEDS_AttributeDrawable_i::IsDrawable() {
  SALOMEDS::Locker lock;

  return (Handle(SALOMEDS_DrawableAttribute)::DownCast(_myAttr)->Get() == 1);
}
                                                           
void SALOMEDS_AttributeDrawable_i::SetDrawable(CORBA::Boolean value) {
  SALOMEDS::Locker lock;

  CheckLocked();
  Standard_Integer val = 0;
  if (value != 0) val = 1;
  Handle(TDataStd_Integer)::DownCast(_myAttr)->Set(val);
}

char* SALOMEDS_AttributeDrawable_i::Store() {
  SALOMEDS::Locker lock;

  return CORBA::string_dup(IsDrawable()?"1":"0");
}

void SALOMEDS_AttributeDrawable_i::Restore(const char* value) {
  SALOMEDS::Locker lock;

  SetDrawable(value[0] == '1');
}
