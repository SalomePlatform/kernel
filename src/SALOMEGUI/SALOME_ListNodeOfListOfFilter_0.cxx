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
//  File   : SALOME_ListNodeOfListOfFilter_0.cxx
//  Module : SALOME

#include "SALOME_ListNodeOfListOfFilter.hxx"

#ifndef _Standard_TypeMismatch_HeaderFile
#include <Standard_TypeMismatch.hxx>
#endif

#ifndef _SALOME_Filter_HeaderFile
#include "SALOME_Filter.hxx"
#endif
#ifndef _SALOME_ListOfFilter_HeaderFile
#include "SALOME_ListOfFilter.hxx"
#endif
#ifndef _SALOME_ListIteratorOfListOfFilter_HeaderFile
#include "SALOME_ListIteratorOfListOfFilter.hxx"
#endif
using namespace std;
SALOME_ListNodeOfListOfFilter::~SALOME_ListNodeOfListOfFilter() {}
 


Standard_EXPORT Handle_Standard_Type& SALOME_ListNodeOfListOfFilter_Type_()
{

    static Handle_Standard_Type aType1 = STANDARD_TYPE(TCollection_MapNode);
  if ( aType1.IsNull()) aType1 = STANDARD_TYPE(TCollection_MapNode);
  static Handle_Standard_Type aType2 = STANDARD_TYPE(MMgt_TShared);
  if ( aType2.IsNull()) aType2 = STANDARD_TYPE(MMgt_TShared);
  static Handle_Standard_Type aType3 = STANDARD_TYPE(Standard_Transient);
  if ( aType3.IsNull()) aType3 = STANDARD_TYPE(Standard_Transient);
 

  static Handle_Standard_Transient _Ancestors[]= {aType1,aType2,aType3,NULL};
  static Handle_Standard_Type _aType = new Standard_Type("SALOME_ListNodeOfListOfFilter",
			                                 sizeof(SALOME_ListNodeOfListOfFilter),
			                                 1,
			                                 (Standard_Address)_Ancestors,
			                                 (Standard_Address)NULL);

  return _aType;
}


// DownCast method
//   allow safe downcasting
//
const Handle(SALOME_ListNodeOfListOfFilter) Handle(SALOME_ListNodeOfListOfFilter)::DownCast(const Handle(Standard_Transient)& AnObject) 
{
  Handle(SALOME_ListNodeOfListOfFilter) _anOtherObject;

  if (!AnObject.IsNull()) {
     if (AnObject->IsKind(STANDARD_TYPE(SALOME_ListNodeOfListOfFilter))) {
       _anOtherObject = Handle(SALOME_ListNodeOfListOfFilter)((Handle(SALOME_ListNodeOfListOfFilter)&)AnObject);
     }
  }

  return _anOtherObject ;
}
const Handle(Standard_Type)& SALOME_ListNodeOfListOfFilter::DynamicType() const 
{ 
  return STANDARD_TYPE(SALOME_ListNodeOfListOfFilter) ; 
}
Standard_Boolean SALOME_ListNodeOfListOfFilter::IsKind(const Handle(Standard_Type)& AType) const 
{ 
  return (STANDARD_TYPE(SALOME_ListNodeOfListOfFilter) == AType || TCollection_MapNode::IsKind(AType)); 
}
Handle_SALOME_ListNodeOfListOfFilter::~Handle_SALOME_ListNodeOfListOfFilter() {}
#define Item Handle_SALOME_Filter
#define Item_hxx "SALOME_Filter.hxx"
#define TCollection_ListNode SALOME_ListNodeOfListOfFilter
#define TCollection_ListNode_hxx "SALOME_ListNodeOfListOfFilter.hxx"
#define TCollection_ListIterator SALOME_ListIteratorOfListOfFilter
#define TCollection_ListIterator_hxx "SALOME_ListIteratorOfListOfFilter.hxx"
#define Handle_TCollection_ListNode Handle_SALOME_ListNodeOfListOfFilter
#define TCollection_ListNode_Type_() SALOME_ListNodeOfListOfFilter_Type_()
#define TCollection_List SALOME_ListOfFilter
#define TCollection_List_hxx "SALOME_ListOfFilter.hxx"
#include <TCollection_ListNode.gxx>

