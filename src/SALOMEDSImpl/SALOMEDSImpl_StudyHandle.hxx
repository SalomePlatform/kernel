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

class Standard_EXPORT SALOMEDSImpl_StudyHandle : public TDF_Attribute
{

public:
                                  SALOMEDSImpl_StudyHandle();
                                  ~SALOMEDSImpl_StudyHandle() { myHandle.Nullify(); }

  static Handle(SALOMEDSImpl_StudyHandle) Set(const TDF_Label& theLabel, const Handle(SALOMEDSImpl_Study)& theStudy); 
  static const                    Standard_GUID& GetID() ;

  void                            SetHandle(const Handle(SALOMEDSImpl_Study)& theStudy) { myHandle = theStudy; }
  Handle(SALOMEDSImpl_Study)      GetHandle() { return myHandle; }
  const Standard_GUID&            ID() const;
  void                            Restore( const Handle(TDF_Attribute)& theWith );
  Handle(TDF_Attribute)           NewEmpty() const;
  void                            Paste( const Handle(TDF_Attribute)& theInto,
                                         const Handle(TDF_RelocationTable)& ) const;
  
private:
  Handle(SALOMEDSImpl_Study) myHandle;

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_StudyHandle )
};

#endif
