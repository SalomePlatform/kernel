using namespace std;
//  File      : SALOME_TypeFilter.cxx
//  Created   : Wed Feb 20 17:24:59 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE 2002
//  $Header$

#include "SALOME_TypeFilter.ixx"
#include "SALOME_InteractiveObject.hxx"

SALOME_TypeFilter::SALOME_TypeFilter(const Standard_CString TheKind):
myKind(TheKind){}

Standard_Boolean SALOME_TypeFilter::IsOk(const Handle(SALOME_InteractiveObject)& anObj) const 
{
  return anObj->isComponentType( myKind );
}
