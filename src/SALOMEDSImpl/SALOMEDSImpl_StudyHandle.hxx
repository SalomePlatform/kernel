//  File   : SALOMEDSImpl_StudyHandle.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDSImpl_StudyHandle_HeaderFile
#define SALOMEDSImpl_StudyHandle_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <TDF_Attribute.hxx>
#include <TDF_Label.hxx>

class Standard_GUID;
class Handle(TDF_Attribute);
class Handle(TDF_RelocationTable);

/*
  Class       : SALOMEDSImpl_StudyHandle
  Description : PRIVATE: This class is intended for storing of the study handle 
*/

DEFINE_STANDARD_HANDLE( SALOMEDSImpl_StudyHandle, TDF_Attribute )

#include "SALOMEDSImpl_Study.hxx"

#ifndef WNT
class Standard_EXPORT SALOMEDSImpl_StudyHandle : public TDF_Attribute
#else
class SALOMEDSImpl_StudyHandle : public TDF_Attribute
#endif
{

public:
Standard_EXPORT                                  SALOMEDSImpl_StudyHandle();
Standard_EXPORT                                  ~SALOMEDSImpl_StudyHandle() { myHandle.Nullify(); }

Standard_EXPORT  static Handle(SALOMEDSImpl_StudyHandle) Set(const TDF_Label& theLabel, const Handle(SALOMEDSImpl_Study)& theStudy); 
Standard_EXPORT  static const                    Standard_GUID& GetID() ;

Standard_EXPORT  void                            SetHandle(const Handle(SALOMEDSImpl_Study)& theStudy) { myHandle = theStudy; }
Standard_EXPORT  Handle(SALOMEDSImpl_Study)      GetHandle() { return myHandle; }
Standard_EXPORT  const Standard_GUID&            ID() const;
Standard_EXPORT  void                            Restore( const Handle(TDF_Attribute)& theWith );
Standard_EXPORT  Handle(TDF_Attribute)           NewEmpty() const;
Standard_EXPORT  void                            Paste( const Handle(TDF_Attribute)& theInto,
							                            const Handle(TDF_RelocationTable)& ) const;
  
private:
  Handle(SALOMEDSImpl_Study) myHandle;

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_StudyHandle )
};

#endif
