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
//  File   : SALOMEDS_AttributeIOR.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeIOR.hxx"
#include "SALOMEDS.hxx"

#include <string>
#include <TCollection_AsciiString.hxx> 
#include <TCollection_ExtendedString.hxx>

SALOMEDS_AttributeIOR::SALOMEDS_AttributeIOR(const Handle(SALOMEDSImpl_AttributeIOR)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeIOR::SALOMEDS_AttributeIOR(SALOMEDS::AttributeIOR_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeIOR::~SALOMEDS_AttributeIOR()
{}

std::string SALOMEDS_AttributeIOR::Value()
{
  std::string aValue;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aValue = TCollection_AsciiString(Handle(SALOMEDSImpl_AttributeIOR)::
                                     DownCast(_local_impl)->Value()).ToCString();
  }
  else aValue = SALOMEDS::AttributeIOR::_narrow(_corba_impl)->Value();
  return aValue;
}

void SALOMEDS_AttributeIOR::SetValue(const std::string& value)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    Handle(SALOMEDSImpl_AttributeIOR)::DownCast(_local_impl)->SetValue((char*)value.c_str());
  }
  else SALOMEDS::AttributeIOR::_narrow(_corba_impl)->SetValue(value.c_str());
}
