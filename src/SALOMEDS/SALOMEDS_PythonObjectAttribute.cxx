using namespace std;
//  File      : SALOMEDS_PythonObjectAttribute.cxx
//  Created   : Tue Oct  8 10:13:30 2002
//  Author    : Michael Ponikarov
//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE
//  $Header$

#include "SALOMEDS_PythonObjectAttribute.ixx"
#include <string>

const Standard_GUID& SALOMEDS_PythonObjectAttribute::GetID() 
{
  static Standard_GUID SALOMEDS_PythonObjectAttributeID ("128371A3-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDS_PythonObjectAttributeID;
}

Handle(SALOMEDS_PythonObjectAttribute) SALOMEDS_PythonObjectAttribute::Set(const TDF_Label& label) 
{
  Handle(SALOMEDS_PythonObjectAttribute) anAttr;
  if (!label.FindAttribute(SALOMEDS_PythonObjectAttribute::GetID(),anAttr)) {
    anAttr = new SALOMEDS_PythonObjectAttribute();
    label.AddAttribute(anAttr);
  }
  return anAttr;
}

SALOMEDS_PythonObjectAttribute::SALOMEDS_PythonObjectAttribute()
{
}

void SALOMEDS_PythonObjectAttribute::SetObject(const Standard_CString theSequence,
					       const Standard_Boolean theScript) 
{
  Backup();
  if(mySequence != NULL) delete mySequence;
  if (theSequence == NULL) {
    mySequence = NULL;
  } else {
    mySequence = new char[strlen(theSequence)+1];
    strcpy(mySequence, theSequence);
  }

  myIsScript = theScript;
}

Standard_CString SALOMEDS_PythonObjectAttribute::GetObject() const
{
  return mySequence;
}

Standard_Boolean SALOMEDS_PythonObjectAttribute::IsScript() const
{
  return myIsScript;
}

Standard_Integer SALOMEDS_PythonObjectAttribute::GetLength() const
{
  if (mySequence == NULL) return 0;
  return (strlen(mySequence)+1);
}

const Standard_GUID& SALOMEDS_PythonObjectAttribute::ID() const
{
  return GetID();
}

void SALOMEDS_PythonObjectAttribute::Restore(const Handle(TDF_Attribute)& with) 
{
  Handle(SALOMEDS_PythonObjectAttribute) anObj = Handle(SALOMEDS_PythonObjectAttribute)::DownCast(with);
  SetObject(anObj->GetObject(),anObj->IsScript());
}

Handle(TDF_Attribute) SALOMEDS_PythonObjectAttribute::NewEmpty() const
{
  return new SALOMEDS_PythonObjectAttribute();
}

void SALOMEDS_PythonObjectAttribute::Paste(const Handle(TDF_Attribute)& into,
					   const Handle(TDF_RelocationTable)&) const
{
  Handle(SALOMEDS_PythonObjectAttribute) anObj = Handle(SALOMEDS_PythonObjectAttribute)::DownCast(into);
  anObj->SetObject(GetObject(),IsScript());
}

