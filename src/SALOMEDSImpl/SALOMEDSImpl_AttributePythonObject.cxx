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
//  File   : SALOMEDSImpl_AttributePythonObject.cxx
//  Author : Michael Ponikarov
//  Module : SALOME

#include "SALOMEDSImpl_AttributePythonObject.hxx"
#include <Standard_GUID.hxx>
#include <string>

using namespace std;

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributePythonObject, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributePythonObject, SALOMEDSImpl_GenericAttribute )

const Standard_GUID& SALOMEDSImpl_AttributePythonObject::GetID() 
{
  static Standard_GUID SALOMEDSImpl_AttributePythonObjectID ("128371A3-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributePythonObjectID;
}

Handle(SALOMEDSImpl_AttributePythonObject) SALOMEDSImpl_AttributePythonObject::Set(const TDF_Label& label) 
{
  Handle(SALOMEDSImpl_AttributePythonObject) anAttr;
  if (!label.FindAttribute(SALOMEDSImpl_AttributePythonObject::GetID(),anAttr)) {
    anAttr = new SALOMEDSImpl_AttributePythonObject();
    label.AddAttribute(anAttr);
  }
  return anAttr;
}

SALOMEDSImpl_AttributePythonObject::SALOMEDSImpl_AttributePythonObject()
:SALOMEDSImpl_GenericAttribute("AttributePythonObject")
{
}

void SALOMEDSImpl_AttributePythonObject::SetObject(const TCollection_AsciiString& theSequence,
					           const bool theScript) 
{
  CheckLocked();    
  Backup();
  mySequence = theSequence;
  myIsScript = theScript;

  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

TCollection_AsciiString SALOMEDSImpl_AttributePythonObject::GetObject() const
{
  return mySequence;
}

bool SALOMEDSImpl_AttributePythonObject::IsScript() const
{
  return myIsScript;
}

int SALOMEDSImpl_AttributePythonObject::GetLength() const
{
  return mySequence.Length();
}

const Standard_GUID& SALOMEDSImpl_AttributePythonObject::ID() const
{
  return GetID();
}

void SALOMEDSImpl_AttributePythonObject::Restore(const Handle(TDF_Attribute)& with) 
{
  Handle(SALOMEDSImpl_AttributePythonObject) anObj = Handle(SALOMEDSImpl_AttributePythonObject)::DownCast(with);
  SetObject(anObj->GetObject(),anObj->IsScript());
}

Handle(TDF_Attribute) SALOMEDSImpl_AttributePythonObject::NewEmpty() const
{
  return new SALOMEDSImpl_AttributePythonObject();
}

void SALOMEDSImpl_AttributePythonObject::Paste(const Handle(TDF_Attribute)& into,
					       const Handle(TDF_RelocationTable)&) const
{
  Handle(SALOMEDSImpl_AttributePythonObject) anObj = Handle(SALOMEDSImpl_AttributePythonObject)::DownCast(into);
  anObj->SetObject(GetObject(),IsScript());
}


TCollection_AsciiString SALOMEDSImpl_AttributePythonObject::Save() 
{
  char* aString = (char*)GetObject().ToCString();
  char* aResult = new char[strlen(aString) + 2];
  aResult[0] = IsScript()?'s':'n';
  strcpy(aResult+1, aString);
  TCollection_AsciiString ret(aResult);
  
  delete aResult;
  return ret;
}
	  
void SALOMEDSImpl_AttributePythonObject::Load(const TCollection_AsciiString& value) 
{
  char* aString = value.ToCString();
  SetObject(aString + 1, aString[0]=='s');
}   
