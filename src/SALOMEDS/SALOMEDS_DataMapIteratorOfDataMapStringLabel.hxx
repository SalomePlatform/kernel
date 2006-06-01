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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : SALOMEDS_DataMapIteratorOfDataMapStringLabel.hxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#ifndef _SALOMEDS_DataMapIteratorOfDataMapStringLabel_HeaderFile
#define _SALOMEDS_DataMapIteratorOfDataMapStringLabel_HeaderFile

#ifndef _TCollection_BasicMapIterator_HeaderFile
#include <TCollection_BasicMapIterator.hxx>
#endif
#ifndef _Handle_SALOMEDS_DataMapNodeOfDataMapStringLabel_HeaderFile
#include <Handle_SALOMEDS_DataMapNodeOfDataMapStringLabel.hxx>
#endif
class Standard_NoSuchObject;
class TCollection_ExtendedString;
class TDF_Label;
class SALOMEDS_DataMapStringLabel;
class SALOMEDS_DataMapNodeOfDataMapStringLabel;


#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

class SALOMEDS_DataMapIteratorOfDataMapStringLabel  : public TCollection_BasicMapIterator {

public:

    inline void* operator new(size_t,void* anAddress) 
      {
        return anAddress;
      }
    inline void* operator new(size_t size) 
      { 
        return Standard::Allocate(size); 
      }
    inline void  operator delete(void *anAddress) 
      { 
        if (anAddress) Standard::Free((Standard_Address&)anAddress); 
      }
//    inline void  operator delete(void *anAddress, size_t size) 
//      { 
//        if (anAddress) Standard::Free((Standard_Address&)anAddress,size); 
//      }
 // Methods PUBLIC
 // 
Standard_EXPORT SALOMEDS_DataMapIteratorOfDataMapStringLabel();
Standard_EXPORT SALOMEDS_DataMapIteratorOfDataMapStringLabel(const SALOMEDS_DataMapStringLabel& aMap);
Standard_EXPORT   void Initialize(const SALOMEDS_DataMapStringLabel& aMap) ;
Standard_EXPORT  const TCollection_ExtendedString& Key() const;
Standard_EXPORT  const TDF_Label& Value() const;





protected:

 // Methods PROTECTED
 // 


 // Fields PROTECTED
 //


private: 

 // Methods PRIVATE
 // 


 // Fields PRIVATE
 //


};





// other inline functions and methods (like "C++: function call" methods)
//


#endif
