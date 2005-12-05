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
// See http://www.salome-platform.org/
//
//  File   : SALOMEDS_AttributePersistentRef.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributePersistentRef.hxx"

#include <string>
#include <TCollection_AsciiString.hxx> 
#include <TCollection_ExtendedString.hxx>

SALOMEDS_AttributePersistentRef::SALOMEDS_AttributePersistentRef(const Handle(SALOMEDSImpl_AttributePersistentRef)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributePersistentRef::SALOMEDS_AttributePersistentRef(SALOMEDS::AttributePersistentRef_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributePersistentRef::~SALOMEDS_AttributePersistentRef()
{}

std::string SALOMEDS_AttributePersistentRef::Value()
{
  std::string aValue;
  if(_isLocal) 
    aValue = TCollection_AsciiString(Handle(SALOMEDSImpl_AttributePersistentRef)::DownCast(_local_impl)->Value()).ToCString();
  else aValue = SALOMEDS::AttributePersistentRef::_narrow(_corba_impl)->Value();
  return aValue;
}
 
void SALOMEDS_AttributePersistentRef::SetValue(const std::string& value)
{
  CheckLocked();
  if(_isLocal) Handle(SALOMEDSImpl_AttributePersistentRef)::DownCast(_local_impl)->SetValue((char*)value.c_str());
  else SALOMEDS::AttributePersistentRef::_narrow(_corba_impl)->SetValue(value.c_str());
}
