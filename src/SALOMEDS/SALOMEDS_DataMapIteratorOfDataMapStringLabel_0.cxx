//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
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
//  File   : SALOMEDS_DataMapIteratorOfDataMapStringLabel_0.cxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header: 

#include "SALOMEDS_DataMapIteratorOfDataMapStringLabel.hxx"

#ifndef _Standard_NoSuchObject_HeaderFile
#include <Standard_NoSuchObject.hxx>
#endif
#ifndef _TCollection_ExtendedString_HeaderFile
#include <TCollection_ExtendedString.hxx>
#endif
#ifndef _TDF_Label_HeaderFile
#include <TDF_Label.hxx>
#endif
#ifndef _SALOMEDS_DataMapStringLabel_HeaderFile
#include "SALOMEDS_DataMapStringLabel.hxx"
#endif
#ifndef _SALOMEDS_DataMapNodeOfDataMapStringLabel_HeaderFile
#include "SALOMEDS_DataMapNodeOfDataMapStringLabel.hxx"
#endif
using namespace std;
 

#define TheKey TCollection_ExtendedString
#define TheKey_hxx <TCollection_ExtendedString.hxx>
#define TheItem TDF_Label
#define TheItem_hxx <TDF_Label.hxx>
#define Hasher TCollection_ExtendedString
#define Hasher_hxx <TCollection_ExtendedString.hxx>
#define TCollection_DataMapNode SALOMEDS_DataMapNodeOfDataMapStringLabel
#define TCollection_DataMapNode_hxx "SALOMEDS_DataMapNodeOfDataMapStringLabel.hxx"
#define TCollection_DataMapIterator SALOMEDS_DataMapIteratorOfDataMapStringLabel
#define TCollection_DataMapIterator_hxx "SALOMEDS_DataMapIteratorOfDataMapStringLabel.hxx"
#define Handle_TCollection_DataMapNode Handle_SALOMEDS_DataMapNodeOfDataMapStringLabel
#define TCollection_DataMapNode_Type_() SALOMEDS_DataMapNodeOfDataMapStringLabel_Type_()
#define TCollection_DataMap SALOMEDS_DataMapStringLabel
#define TCollection_DataMap_hxx "SALOMEDS_DataMapStringLabel.hxx"
#include <TCollection_DataMapIterator.gxx>

