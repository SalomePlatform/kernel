using namespace std;
//  File      : SALOME_InteractiveObject.cxx
//  Created   : Wed Feb 20 17:24:59 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOME
//  Copyright : Open CASCADE 2002
//  $Header$

/*!
  \class SALOME_InteractiveObject SALOME_InteractiveObject.hxx
  \brief ...
*/

#include "SALOME_InteractiveObject.ixx"

SALOME_InteractiveObject::SALOME_InteractiveObject()
{
  myEntry = "";
  myName  = "";
  myComponentDataType = "";
  myReference = "";
}

SALOME_InteractiveObject::SALOME_InteractiveObject(const Standard_CString anEntry, 
						   const Standard_CString aComponentDataType,
						   const Standard_CString aName)
{
  myEntry = new char [strlen(anEntry)+1];
  strcpy( myEntry, anEntry);

  myName = new char [strlen(aName)+1];
  strcpy( myName, aName);

  myComponentDataType = new char [strlen(aComponentDataType)+1];
  strcpy( myComponentDataType, aComponentDataType);

  myReference = new char [strlen("")+1];
  strcpy( myReference, "");
}

void SALOME_InteractiveObject::setEntry(const Standard_CString anEntry){
  myEntry = new char [strlen(anEntry)+1];
  strcpy( myEntry, anEntry);
}

Standard_CString SALOME_InteractiveObject::getEntry(){
  return myEntry;
}

void SALOME_InteractiveObject::setComponentDataType(const Standard_CString aComponentDataType){
  myComponentDataType = new char [strlen(aComponentDataType)+1];
  strcpy( myComponentDataType, aComponentDataType);
}

Standard_CString SALOME_InteractiveObject::getComponentDataType(){
  return myComponentDataType;
}

void SALOME_InteractiveObject::setName(const Standard_CString aName){
  myName = new char [strlen(aName)+1];
  strcpy( myName, aName);
}

Standard_CString SALOME_InteractiveObject::getName(){
  return myName;
}

Standard_Boolean SALOME_InteractiveObject::hasEntry(){
  return !( strcmp(myEntry, "" ) == 0 );
}

Standard_Boolean SALOME_InteractiveObject::isSame(const Handle(SALOME_InteractiveObject)& anIO ){
  if ( anIO->hasEntry() && this->hasEntry() ) {
    if ( strcmp(myEntry, anIO->getEntry() ) == 0 )
      return Standard_True;
  }
  
  return Standard_False;
}

Standard_Boolean SALOME_InteractiveObject::isComponentType(const Standard_CString ComponentDataType){
  if ( strcmp( myComponentDataType, ComponentDataType) == 0 )
    return Standard_True;
  else
    return Standard_False;
}

Standard_Boolean SALOME_InteractiveObject::hasReference()
{
  return !( strcmp(myReference, "" ) == 0 );
}

Standard_CString SALOME_InteractiveObject::getReference()
{
  return myReference;
}

void SALOME_InteractiveObject::setReference(const Standard_CString aReference)
{
  myReference = new char [strlen(aReference)+1];
  strcpy( myReference, aReference);
}
