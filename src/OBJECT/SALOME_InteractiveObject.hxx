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

#include <string>

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
Standard_EXPORT   SALOME_InteractiveObject(const char* anEntry, 
					   const char* aComponentDataType,
					   const char* aName = "");
Standard_EXPORT   void setEntry(const char* anEntry) ;
Standard_EXPORT   const char* getEntry() ;
Standard_EXPORT   void setName(const char* aName) ;
Standard_EXPORT   const char* getName() ;
Standard_EXPORT   Standard_Boolean hasEntry() ;
Standard_EXPORT   virtual  Standard_Boolean isSame(const Handle(SALOME_InteractiveObject)& anIO) ;

Standard_EXPORT   Standard_Boolean hasReference() ;
Standard_EXPORT   const char* getReference() ;
Standard_EXPORT   void setReference(const char* aReference) ;

Standard_EXPORT   void setComponentDataType(const char* ComponentDataType) ;
Standard_EXPORT   const char* getComponentDataType() ;
Standard_EXPORT   Standard_Boolean isComponentType(const char* ComponentDataType) ; 
  //Standard_EXPORT   Standard_Boolean isComponentType(std::string  ComponentDataType) ; 
Standard_EXPORT   virtual ~SALOME_InteractiveObject();

// Must be defined for using by NCollection for ceratin of data map
friend Standard_Boolean IsEqual(const Handle(SALOME_InteractiveObject)& anIO1,
                                const Handle(SALOME_InteractiveObject)& anIO2)
{ return anIO1->isSame( anIO2 ); }





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
  std::string myReference;
  std::string myEntry;
  std::string myName;
  std::string myComponentDataType;

private: 

 // Methods PRIVATE
 // 


 // Fields PRIVATE
 //


};





// other Inline functions and methods (like "C++: function call" methods)
//


#endif
