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
//  File   : SALOMEDS_AttributeFileType_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME


#include "SALOMEDS_AttributeFileType_i.hxx"
#include "SALOMEDS.hxx"

using namespace std;

char* SALOMEDS_AttributeFileType_i::Value()
{
  SALOMEDS::Locker lock;
  CORBA::String_var c_s = 
    CORBA::string_dup(dynamic_cast<SALOMEDSImpl_AttributeFileType*>(_impl)->Value().c_str());
  return c_s._retn();
}

void SALOMEDS_AttributeFileType_i::SetValue(const char* value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  CORBA::String_var Str = CORBA::string_dup(value);
  string aValue((char*)Str.in());
  dynamic_cast<SALOMEDSImpl_AttributeFileType*>(_impl)->SetValue(aValue);
}
