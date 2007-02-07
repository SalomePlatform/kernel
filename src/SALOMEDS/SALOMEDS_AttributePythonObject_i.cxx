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
//  File   : SALOMEDS_AttributePythonObject_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME


#include "SALOMEDS_AttributePythonObject_i.hxx"
#include "SALOMEDS.hxx"
#include <TCollection_ExtendedString.hxx>
#include <TColStd_HArray1OfCharacter.hxx>

using namespace std;

void SALOMEDS_AttributePythonObject_i::SetObject(const char* theSequence, CORBA::Boolean IsScript) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  char *aSeq = CORBA::string_dup(theSequence);
  Handle(SALOMEDSImpl_AttributePythonObject)::DownCast(_impl)->SetObject(aSeq, IsScript);
}

char* SALOMEDS_AttributePythonObject_i::GetObject() 
{
  SALOMEDS::Locker lock;
  TCollection_AsciiString aSeq(Handle(SALOMEDSImpl_AttributePythonObject)::
			       DownCast(_impl)->GetObject().ToCString());
  CORBA::String_var aStr = CORBA::string_dup(aSeq.ToCString());
  return aStr._retn();
}

CORBA::Boolean SALOMEDS_AttributePythonObject_i::IsScript() 
{
  SALOMEDS::Locker lock;
  return Handle(SALOMEDSImpl_AttributePythonObject)::DownCast(_impl)->IsScript();
}


