// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
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

//  File   : SALOMEDS_AttributePythonObject.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_AttributePythonObject.hxx"
#include "SALOMEDS.hxx"

#include <string>

SALOMEDS_AttributePythonObject::SALOMEDS_AttributePythonObject(SALOMEDSImpl_AttributePythonObject* theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributePythonObject::SALOMEDS_AttributePythonObject(SALOMEDS::AttributePythonObject_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributePythonObject::~SALOMEDS_AttributePythonObject()
{}

bool SALOMEDS_AttributePythonObject::IsScript()
{
  bool ret;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    ret = dynamic_cast<SALOMEDSImpl_AttributePythonObject*>(_local_impl)->IsScript();
  }
  else ret = SALOMEDS::AttributePythonObject::_narrow(_corba_impl)->IsScript();
  return ret;
}

std::string SALOMEDS_AttributePythonObject::GetObject()
{
  std::string aValue;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aValue = dynamic_cast<SALOMEDSImpl_AttributePythonObject*>(_local_impl)->GetObject().c_str();
  }
  else aValue = SALOMEDS::AttributePythonObject::_narrow(_corba_impl)->GetObject();
  return aValue;
}

void SALOMEDS_AttributePythonObject::SetObject(const std::string& theSequence, bool IsScript)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    dynamic_cast<SALOMEDSImpl_AttributePythonObject*>(_local_impl)->SetObject(theSequence, IsScript);
  }
  else SALOMEDS::AttributePythonObject::_narrow(_corba_impl)->SetObject(theSequence.c_str(), IsScript);
}
