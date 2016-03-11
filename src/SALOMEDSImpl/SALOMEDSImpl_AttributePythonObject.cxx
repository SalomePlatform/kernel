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

//  File   : SALOMEDSImpl_AttributePythonObject.cxx
//  Author : Michael Ponikarov
//  Module : SALOME
//
#include "SALOMEDSImpl_AttributePythonObject.hxx"

const std::string& SALOMEDSImpl_AttributePythonObject::GetID() 
{
  static std::string SALOMEDSImpl_AttributePythonObjectID ("128371A3-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributePythonObjectID;
}

SALOMEDSImpl_AttributePythonObject* SALOMEDSImpl_AttributePythonObject::Set(const DF_Label& label) 
{
  SALOMEDSImpl_AttributePythonObject* A = NULL;
  if (!(A = (SALOMEDSImpl_AttributePythonObject*)label.FindAttribute(SALOMEDSImpl_AttributePythonObject::GetID()))) {
    A = new SALOMEDSImpl_AttributePythonObject();
    label.AddAttribute(A);
  }
  return A;
}

SALOMEDSImpl_AttributePythonObject::SALOMEDSImpl_AttributePythonObject()
:SALOMEDSImpl_GenericAttribute("AttributePythonObject")
{
    myIsScript = false;
}

void SALOMEDSImpl_AttributePythonObject::SetObject(const std::string& theSequence,
                                                   const bool theScript) 
{
  CheckLocked();
  Backup();
  mySequence = theSequence;
  myIsScript = theScript;

  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

std::string SALOMEDSImpl_AttributePythonObject::GetObject() const
{
  return mySequence;
}

bool SALOMEDSImpl_AttributePythonObject::IsScript() const
{
  return myIsScript;
}

int SALOMEDSImpl_AttributePythonObject::GetLength() const
{
  return mySequence.size();
}

const std::string& SALOMEDSImpl_AttributePythonObject::ID() const
{
  return GetID();
}

void SALOMEDSImpl_AttributePythonObject::Restore(DF_Attribute* with) 
{
  SALOMEDSImpl_AttributePythonObject* anObj = dynamic_cast<SALOMEDSImpl_AttributePythonObject*>(with);
  SetObject(anObj->GetObject(),anObj->IsScript());
}

DF_Attribute* SALOMEDSImpl_AttributePythonObject::NewEmpty() const
{
  return new SALOMEDSImpl_AttributePythonObject();
}

void SALOMEDSImpl_AttributePythonObject::Paste(DF_Attribute* into)
{
  SALOMEDSImpl_AttributePythonObject* anObj = dynamic_cast<SALOMEDSImpl_AttributePythonObject*>(into);
  anObj->SetObject(GetObject(),IsScript());
}


std::string SALOMEDSImpl_AttributePythonObject::Save() 
{
  std::string aString = GetObject();
  std::string aResult = IsScript()?"s":"n";
  aResult += aString;
  
  return aResult;
}
          
void SALOMEDSImpl_AttributePythonObject::Load(const std::string& value) 
{
  char* aString = (char*)value.c_str();
  SetObject(aString + 1, aString[0]=='s');
}   
