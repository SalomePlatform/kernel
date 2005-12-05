// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/
//
//  File   : SALOMEDSImpl_AttributeGraphic.hxx
//  Author : Sergey LITONIN
//  Module : SALOME

#ifndef SALOMEDSImpl_AttributeGraphic_HeaderFile
#define SALOMEDSImpl_AttributeGraphic_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <TDF_Attribute.hxx>
#include <TColStd_DataMapOfIntegerInteger.hxx>
#include <TDF_Label.hxx>       
#include "SALOMEDSImpl_GenericAttribute.hxx"

class Standard_GUID;
class Handle(TDF_Attribute);
class Handle(TDF_RelocationTable);

/*
  Class       : SALOMEDSImpl_AttributeGraphic
  Description : This class is intended for storing information about
                graphic representation of objects in dirrent views
*/

DEFINE_STANDARD_HANDLE( SALOMEDSImpl_AttributeGraphic,  SALOMEDSImpl_GenericAttribute)



#ifndef WNT
class Standard_EXPORT SALOMEDSImpl_AttributeGraphic : public SALOMEDSImpl_GenericAttribute
#else
class SALOMEDSImpl_AttributeGraphic : public SALOMEDSImpl_GenericAttribute
#endif
{

public:
Standard_EXPORT                                    SALOMEDSImpl_AttributeGraphic();
Standard_EXPORT                                    ~SALOMEDSImpl_AttributeGraphic();
				  
Standard_EXPORT    static const                    Standard_GUID& GetID() ;

Standard_EXPORT    void                            SetVisibility( const Standard_Integer,
                                                 const Standard_Boolean );
Standard_EXPORT    Standard_Boolean                GetVisibility( const Standard_Integer ) const;
Standard_EXPORT    const Standard_GUID&            ID() const;
Standard_EXPORT    void                            Restore( const Handle(TDF_Attribute)& theWith );
Standard_EXPORT    Handle(TDF_Attribute)           NewEmpty() const;
Standard_EXPORT    void                            Paste( const Handle(TDF_Attribute)& theInto,
                                         const Handle(TDF_RelocationTable)& ) const;
Standard_EXPORT    virtual Standard_OStream&       Dump( Standard_OStream& anOS ) const;

Standard_EXPORT    void                            SetVisibility( const TColStd_DataMapOfIntegerInteger& );

Standard_EXPORT    const TColStd_DataMapOfIntegerInteger& GetVisibility();
  
private:
  TColStd_DataMapOfIntegerInteger myVisibility;

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_AttributeGraphic )
};

#endif
