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
//  File   : SALOME_InteractiveObject.hxx
//  Module : SALOME

#ifndef _SALOME_InteractiveObject_HeaderFile
#define _SALOME_InteractiveObject_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Handle_SALOME_InteractiveObject_HeaderFile
#include <Handle_SALOME_InteractiveObject.hxx>
#endif

#ifndef _Standard_CString_HeaderFile
#include <Standard_CString.hxx>
#endif
#ifndef _MMgt_TShared_HeaderFile
#include <MMgt_TShared.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif


class SALOME_InteractiveObject : public MMgt_TShared {

public:

    void* operator new(size_t,void* anAddress) 
      {
        return anAddress;
      }
    void* operator new(size_t size) 
      { 
        return Standard::Allocate(size); 
      }
    void  operator delete(void *anAddress) 
      { 
        if (anAddress) Standard::Free((Standard_Address&)anAddress); 
      }
 // Methods PUBLIC
 // 
Standard_EXPORT   SALOME_InteractiveObject();
Standard_EXPORT   SALOME_InteractiveObject(const Standard_CString anEntry, 
					   const Standard_CString aComponentDataType,
					   const Standard_CString aName = "");
Standard_EXPORT   void setEntry(const Standard_CString anEntry) ;
Standard_EXPORT   Standard_CString getEntry() ;
Standard_EXPORT   void setName(const Standard_CString aName) ;
Standard_EXPORT   Standard_CString getName() ;
Standard_EXPORT   Standard_Boolean hasEntry() ;
Standard_EXPORT   virtual  Standard_Boolean isSame(const Handle(SALOME_InteractiveObject)& anIO) ;

Standard_EXPORT   Standard_Boolean hasReference() ;
Standard_EXPORT   Standard_CString getReference() ;
Standard_EXPORT   void setReference(const Standard_CString aReference) ;

Standard_EXPORT   void setComponentDataType(const Standard_CString ComponentDataType) ;
Standard_EXPORT   Standard_CString getComponentDataType() ;
Standard_EXPORT   Standard_Boolean isComponentType(const Standard_CString ComponentDataType) ;

Standard_EXPORT   ~SALOME_InteractiveObject();




 // Type management
 //
 Standard_EXPORT friend Handle_Standard_Type& SALOME_InteractiveObject_Type_();
 Standard_EXPORT const Handle(Standard_Type)& DynamicType() const;
 Standard_EXPORT Standard_Boolean	       IsKind(const Handle(Standard_Type)&) const;

protected:

 // Methods PROTECTED
 // 


 // Fields PROTECTED
 //
Standard_CString myReference;
Standard_CString myEntry;
Standard_CString myName;
Standard_CString myComponentDataType;

private: 

 // Methods PRIVATE
 // 


 // Fields PRIVATE
 //


};





// other Inline functions and methods (like "C++: function call" methods)
//


#endif
