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
//  File   : SALOMEDS_AttributeUserID.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeUserID.hxx"

#include <string>
#include <TCollection_AsciiString.hxx> 
#include <TCollection_ExtendedString.hxx>
#include <Standard_GUID.hxx>

SALOMEDS_AttributeUserID::SALOMEDS_AttributeUserID(const Handle(SALOMEDSImpl_AttributeUserID)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeUserID::SALOMEDS_AttributeUserID(SALOMEDS::AttributeUserID_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeUserID::~SALOMEDS_AttributeUserID()
{}

std::string SALOMEDS_AttributeUserID::Value()
{
  std::string aValue;
  if(_isLocal) {
    char guid[40];
    Handle(SALOMEDSImpl_AttributeUserID)::DownCast(_local_impl)->Value().ToCString(guid);
    aValue = std::string(guid);
  }
  else aValue = SALOMEDS::AttributeUserID::_narrow(_corba_impl)->Value();
  return aValue;
}
 
void SALOMEDS_AttributeUserID::SetValue(const std::string& value)
{
  CheckLocked();
  if(_isLocal) Handle(SALOMEDSImpl_AttributeUserID)::DownCast(_local_impl)->SetValue(Standard_GUID((char*)value.c_str()));
  else SALOMEDS::AttributeUserID::_narrow(_corba_impl)->SetValue(value.c_str());
}
