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
//  File   : SALOMEDS_AttributePixMap_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME


#include "SALOMEDS_AttributePixMap_i.hxx"
#include "SALOMEDS.hxx"

#include <TCollection_AsciiString.hxx>

using namespace std;

CORBA::Boolean SALOMEDS_AttributePixMap_i::HasPixMap()
{
  SALOMEDS::Locker lock;
  return Handle(SALOMEDSImpl_AttributePixMap)::DownCast(_impl)->HasPixMap();
}

char* SALOMEDS_AttributePixMap_i::GetPixMap()
{
  SALOMEDS::Locker lock;
  CORBA::String_var S =
    CORBA::string_dup(TCollection_AsciiString(Handle(SALOMEDSImpl_AttributePixMap)::
                                              DownCast(_impl)->GetPixMap()).ToCString());
  return S._retn();
}

void SALOMEDS_AttributePixMap_i::SetPixMap(const char* value)
{
  SALOMEDS::Locker lock;
  CheckLocked();
  CORBA::String_var Str = CORBA::string_dup(value);
  Handle(SALOMEDSImpl_AttributePixMap)::DownCast(_impl)->SetPixMap(TCollection_ExtendedString(Str));
}
