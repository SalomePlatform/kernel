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
//  File   : SALOME_ListIteratorOfListOfFilter_0.cxx
//  Module : SALOME

#include "SALOME_ListIteratorOfListOfFilter.hxx"

#ifndef _Standard_NoMoreObject_HeaderFile
#include <Standard_NoMoreObject.hxx>
#endif
#ifndef _Standard_NoSuchObject_HeaderFile
#include <Standard_NoSuchObject.hxx>
#endif
#ifndef _SALOME_ListOfFilter_HeaderFile
#include "SALOME_ListOfFilter.hxx"
#endif
#ifndef _SALOME_Filter_HeaderFile
#include "SALOME_Filter.hxx"
#endif
#ifndef _SALOME_ListNodeOfListOfFilter_HeaderFile
#include "SALOME_ListNodeOfListOfFilter.hxx"
#endif
using namespace std;
 

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
#include <TCollection_ListIterator.gxx>

