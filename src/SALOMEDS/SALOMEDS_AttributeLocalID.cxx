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
//  File   : SALOMEDS_AttributeLocalID.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeLocalID.hxx"

#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>

SALOMEDS_AttributeLocalID::SALOMEDS_AttributeLocalID(const Handle(SALOMEDSImpl_AttributeLocalID)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeLocalID::SALOMEDS_AttributeLocalID(SALOMEDS::AttributeLocalID_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeLocalID::~SALOMEDS_AttributeLocalID()
{}

int SALOMEDS_AttributeLocalID::Value()
{
  int aValue;
  if(_isLocal) aValue = (bool)Handle(SALOMEDSImpl_AttributeLocalID)::DownCast(_local_impl)->Value();
  else aValue = SALOMEDS::AttributeLocalID::_narrow(_corba_impl)->Value();
  return aValue;
}
 
void SALOMEDS_AttributeLocalID::SetValue(int value)
{
  CheckLocked();
  if(_isLocal) Handle(SALOMEDSImpl_AttributeLocalID)::DownCast(_local_impl)->SetValue(value);
  else SALOMEDS::AttributeLocalID::_narrow(_corba_impl)->SetValue(value);
}
