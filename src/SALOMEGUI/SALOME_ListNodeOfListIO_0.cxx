//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
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
//  File   : SALOME_ListNodeOfListIO_0.cxx
//  Module : SALOME

#include "SALOME_ListNodeOfListIO.hxx"

#ifndef _Standard_TypeMismatch_HeaderFile
#include <Standard_TypeMismatch.hxx>
#endif

#ifndef _SALOME_InteractiveObject_HeaderFile
#include "SALOME_InteractiveObject.hxx"
#endif
#ifndef _SALOME_ListIO_HeaderFile
#include "SALOME_ListIO.hxx"
#endif
#ifndef _SALOME_ListIteratorOfListIO_HeaderFile
#include "SALOME_ListIteratorOfListIO.hxx"
#endif
using namespace std;


SALOME_ListNodeOfListIO::~SALOME_ListNodeOfListIO() {}
 


Standard_EXPORT Handle_Standard_Type& SALOME_ListNodeOfListIO_Type_()
{

    static Handle_Standard_Type aType1 = STANDARD_TYPE(TCollection_MapNode);
  if ( aType1.IsNull()) aType1 = STANDARD_TYPE(TCollection_MapNode);
  static Handle_Standard_Type aType2 = STANDARD_TYPE(MMgt_TShared);
  if ( aType2.IsNull()) aType2 = STANDARD_TYPE(MMgt_TShared);
  static Handle_Standard_Type aType3 = STANDARD_TYPE(Standard_Transient);
  if ( aType3.IsNull()) aType3 = STANDARD_TYPE(Standard_Transient);
 

  static Handle_Standard_Transient _Ancestors[]= {aType1,aType2,aType3,NULL};
  static Handle_Standard_Type _aType = new Standard_Type("SALOME_ListNodeOfListIO",
			                                 sizeof(SALOME_ListNodeOfListIO),
			                                 1,
			                                 (Standard_Address)_Ancestors,
			                                 (Standard_Address)NULL);

  return _aType;
}


// DownCast method
//   allow safe downcasting
//
const Handle(SALOME_ListNodeOfListIO) Handle(SALOME_ListNodeOfListIO)::DownCast(const Handle(Standard_Transient)& AnObject) 
{
  Handle(SALOME_ListNodeOfListIO) _anOtherObject;

  if (!AnObject.IsNull()) {
     if (AnObject->IsKind(STANDARD_TYPE(SALOME_ListNodeOfListIO))) {
       _anOtherObject = Handle(SALOME_ListNodeOfListIO)((Handle(SALOME_ListNodeOfListIO)&)AnObject);
     }
  }

  return _anOtherObject ;
}
const Handle(Standard_Type)& SALOME_ListNodeOfListIO::DynamicType() const 
{ 
  return STANDARD_TYPE(SALOME_ListNodeOfListIO) ; 
}
Standard_Boolean SALOME_ListNodeOfListIO::IsKind(const Handle(Standard_Type)& AType) const 
{ 
  return (STANDARD_TYPE(SALOME_ListNodeOfListIO) == AType || TCollection_MapNode::IsKind(AType)); 
}
Handle_SALOME_ListNodeOfListIO::~Handle_SALOME_ListNodeOfListIO() {}
#define Item Handle_SALOME_InteractiveObject
#define Item_hxx "SALOME_InteractiveObject.hxx"
#define TCollection_ListNode SALOME_ListNodeOfListIO
#define TCollection_ListNode_hxx "SALOME_ListNodeOfListIO.hxx"
#define TCollection_ListIterator SALOME_ListIteratorOfListIO
#define TCollection_ListIterator_hxx "SALOME_ListIteratorOfListIO.hxx"
#define Handle_TCollection_ListNode Handle_SALOME_ListNodeOfListIO
#define TCollection_ListNode_Type_() SALOME_ListNodeOfListIO_Type_()
#define TCollection_List SALOME_ListIO
#define TCollection_List_hxx "SALOME_ListIO.hxx"
#include <TCollection_ListNode.gxx>

