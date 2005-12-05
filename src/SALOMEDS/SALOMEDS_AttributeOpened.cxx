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
//  File   : SALOMEDS_AttributeOpened.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeOpened.hxx"

#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>

SALOMEDS_AttributeOpened::SALOMEDS_AttributeOpened(const Handle(SALOMEDSImpl_AttributeOpened)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeOpened::SALOMEDS_AttributeOpened(SALOMEDS::AttributeOpened_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeOpened::~SALOMEDS_AttributeOpened()
{}

bool SALOMEDS_AttributeOpened::IsOpened()
{
  bool aValue;
  if(_isLocal) aValue = (bool)Handle(SALOMEDSImpl_AttributeOpened)::DownCast(_local_impl)->IsOpened();
  else aValue = SALOMEDS::AttributeOpened::_narrow(_corba_impl)->IsOpened();
  return aValue;
}
 
void SALOMEDS_AttributeOpened::SetOpened(bool value)
{
  if(_isLocal) Handle(SALOMEDSImpl_AttributeOpened)::DownCast(_local_impl)->SetOpened((int)value);
  else SALOMEDS::AttributeOpened::_narrow(_corba_impl)->SetOpened(value);
}
