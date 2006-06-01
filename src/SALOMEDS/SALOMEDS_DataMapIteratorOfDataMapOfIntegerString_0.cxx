//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOMEDS_DataMapIteratorOfDataMapOfIntegerString_0.cxx
//  Author : Sergey Ruin
//  Module : SALOME

#include <SALOMEDS_DataMapIteratorOfDataMapOfIntegerString.hxx>

#ifndef _Standard_NoSuchObject_HeaderFile
#include <Standard_NoSuchObject.hxx>
#endif
#ifndef _TCollection_ExtendedString_HeaderFile
#include <TCollection_ExtendedString.hxx>
#endif
#ifndef _TColStd_MapIntegerHasher_HeaderFile
#include <TColStd_MapIntegerHasher.hxx>
#endif
#ifndef _SALOMEDS_DataMapOfIntegerString_HeaderFile
#include <SALOMEDS_DataMapOfIntegerString.hxx>
#endif
#ifndef _SALOMEDS_DataMapNodeOfDataMapOfIntegerString_HeaderFile
#include <SALOMEDS_DataMapNodeOfDataMapOfIntegerString.hxx>
#endif
using namespace std;
 

#define TheKey Standard_Real
#define TheKey_hxx <Standard_Real.hxx>
#define TheItem TCollection_ExtendedString
#define TheItem_hxx <TCollection_ExtendedString.hxx>
#define Hasher TColStd_MapIntegerHasher
#define Hasher_hxx <TColStd_MapIntegerHasher.hxx>
#define TCollection_DataMapNode SALOMEDS_DataMapNodeOfDataMapOfIntegerString
#define TCollection_DataMapNode_hxx <SALOMEDS_DataMapNodeOfDataMapOfIntegerString.hxx>
#define TCollection_DataMapIterator SALOMEDS_DataMapIteratorOfDataMapOfIntegerString
#define TCollection_DataMapIterator_hxx <SALOMEDS_DataMapIteratorOfDataMapOfIntegerString.hxx>
#define Handle_TCollection_DataMapNode Handle_SALOMEDS_DataMapNodeOfDataMapOfIntegerString
#define TCollection_DataMapNode_Type_() SALOMEDS_DataMapNodeOfDataMapOfIntegerString_Type_()
#define TCollection_DataMap SALOMEDS_DataMapOfIntegerString
#define TCollection_DataMap_hxx <SALOMEDS_DataMapOfIntegerString.hxx>
#include <TCollection_DataMapIterator.gxx>

