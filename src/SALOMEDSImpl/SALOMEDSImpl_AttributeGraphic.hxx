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



class Standard_EXPORT SALOMEDSImpl_AttributeGraphic : public SALOMEDSImpl_GenericAttribute
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
