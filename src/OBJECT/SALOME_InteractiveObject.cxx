//  SALOME OBJECT : implementation of interactive object visualization for OCC and VTK viewers
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : SALOME_InteractiveObject.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

/*!
  \class SALOME_InteractiveObject SALOME_InteractiveObject.hxx
  \brief ...
*/

#include "SALOME_InteractiveObject.ixx"
using namespace std;

SALOME_InteractiveObject::SALOME_InteractiveObject()
{
  myEntry = "";
  myName  = "";
  myComponentDataType = "";
  myReference = "";
}

SALOME_InteractiveObject::SALOME_InteractiveObject(const char* anEntry, 
						   const char* aComponentDataType,
						   const char* aName):
  myEntry(anEntry), 
  myName(aName), 
  myComponentDataType(aComponentDataType), 
  myReference("")
{}

void SALOME_InteractiveObject::setEntry(const char* anEntry){
  myEntry = anEntry;
}

const char* SALOME_InteractiveObject::getEntry(){
  return myEntry.c_str();
}

void SALOME_InteractiveObject::setComponentDataType(const char* aComponentDataType){
  myComponentDataType = aComponentDataType; 
}

const char* SALOME_InteractiveObject::getComponentDataType(){
  return myComponentDataType.c_str();
}

void SALOME_InteractiveObject::setName(const char* aName){
  myName = aName;
}

const char* SALOME_InteractiveObject::getName(){
  return myName.c_str();
}

Standard_Boolean SALOME_InteractiveObject::hasEntry(){
  return myEntry != "";
}

Standard_Boolean SALOME_InteractiveObject::isSame(const Handle(SALOME_InteractiveObject)& anIO ){
  if ( anIO->hasEntry() && this->hasEntry() ) {
    if ( myEntry == anIO->getEntry() )
      return Standard_True;
  }
  
  return Standard_False;
}

Standard_Boolean SALOME_InteractiveObject::isComponentType(const char* ComponentDataType){
  if ( myComponentDataType == ComponentDataType )
    return Standard_True;
  else
    return Standard_False;
}

Standard_Boolean SALOME_InteractiveObject::hasReference()
{
  return myReference != "";
}

const char* SALOME_InteractiveObject::getReference()
{
  return myReference.c_str();
}

void SALOME_InteractiveObject::setReference(const char* aReference)
{
  myReference = aReference;
}
