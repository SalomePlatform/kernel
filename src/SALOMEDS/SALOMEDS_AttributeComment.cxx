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
//  File   : SALOMEDS_AttributeComment.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeComment.hxx"

#include <string>
#include <TCollection_AsciiString.hxx> 
#include <TCollection_ExtendedString.hxx>

SALOMEDS_AttributeComment::SALOMEDS_AttributeComment(const Handle(SALOMEDSImpl_AttributeComment)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeComment::SALOMEDS_AttributeComment(SALOMEDS::AttributeComment_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeComment::~SALOMEDS_AttributeComment()
{}

std::string SALOMEDS_AttributeComment::Value()
{
  std::string aValue;
  if(_isLocal) 
    aValue = TCollection_AsciiString(Handle(SALOMEDSImpl_AttributeComment)::DownCast(_local_impl)->Value()).ToCString();
  else aValue = SALOMEDS::AttributeComment::_narrow(_corba_impl)->Value();
  return aValue;
}
 
void SALOMEDS_AttributeComment::SetValue(const std::string& value)
{
  CheckLocked();
  if(_isLocal) Handle(SALOMEDSImpl_AttributeComment)::DownCast(_local_impl)->SetValue((char*)value.c_str());
  else SALOMEDS::AttributeComment::_narrow(_corba_impl)->SetValue(value.c_str());
}
