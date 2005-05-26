//  File   : SALOMEDSImpl_AttributeFlags.hxx
//  Author : Sergey LITONIN
//  Module : SALOME

#ifndef SALOMEDSImpl_AttributeFlags_HeaderFile
#define SALOMEDSImpl_AttributeFlags_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <TDF_Attribute.hxx>
#include <TDF_Label.hxx>       
#include "SALOMEDSImpl_GenericAttribute.hxx"

class Standard_GUID;
class Handle(TDF_Attribute);
class Handle(TDF_RelocationTable);


/*
  Class       : SALOMEDSImpl_AttributeFlags
  Description : This class is intended for storing different object attributes that
                have only two states (0 and 1).
*/

DEFINE_STANDARD_HANDLE( SALOMEDSImpl_AttributeFlags, SALOMEDSImpl_GenericAttribute )

class Standard_EXPORT SALOMEDSImpl_AttributeFlags :  public SALOMEDSImpl_GenericAttribute
{
private:
Standard_Integer myValue;

public:
Standard_EXPORT  static const Standard_GUID&            GetID() ;
Standard_EXPORT  static Handle(SALOMEDSImpl_AttributeFlags) Set(const TDF_Label&, const Standard_Integer );
                                                               
                                       SALOMEDSImpl_AttributeFlags();
Standard_EXPORT  virtual                                ~SALOMEDSImpl_AttributeFlags();

Standard_EXPORT  const Standard_GUID&                   ID() const;
Standard_EXPORT  void                                   Restore( const Handle(TDF_Attribute)& with );
Standard_EXPORT  Handle_TDF_Attribute                   NewEmpty() const;
Standard_EXPORT  void                                   Paste( const Handle(TDF_Attribute)& into,
                                              const Handle(TDF_RelocationTable)& RT ) const;
Standard_EXPORT  void                                   Set( const Standard_Integer );
Standard_EXPORT  Standard_Integer                       Get() const;

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_AttributeFlags )
};

#endif
