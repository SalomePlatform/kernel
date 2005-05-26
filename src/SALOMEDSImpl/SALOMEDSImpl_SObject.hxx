//  File   : SALOMEDSImpl_SObject.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSIMPL_SOBJECT_H__
#define __SALOMEDSIMPL_SOBJECT_H__

//Handle definition
#include <Handle_MMgt_TShared.hxx>
#include <Standard_DefineHandle.hxx>
DEFINE_STANDARD_HANDLE( SALOMEDSImpl_SObject, MMgt_TShared )

class SALOMEDSImpl_SComponent;
class Handle_SALOMEDSImpl_SComponent;
class SALOMEDSImpl_Study;
class Handle_SALOMEDSImpl_Study;

// Cascade headers
#include <TDF_Label.hxx>
#include <TDF_Tool.hxx>
#include <TDF_Attribute.hxx>
#include <Standard_GUID.hxx>
#include <Standard_NoSuchObject.hxx>
#include <TCollection_AsciiString.hxx>
#include <TColStd_HSequenceOfTransient.hxx>

class SALOMEDSImpl_SObject : public MMgt_TShared 
{
protected:
  TDF_Label                _lab;
  TCollection_AsciiString  _name;  
  TCollection_AsciiString  _value;
  TCollection_AsciiString  _type;

public:
  
  SALOMEDSImpl_SObject(const TDF_Label& theLabel);
  ~SALOMEDSImpl_SObject();
  
  virtual TCollection_AsciiString GetID();
  virtual Handle(SALOMEDSImpl_SComponent) GetFatherComponent();
  virtual Handle(SALOMEDSImpl_SObject) GetFather() ;
  virtual bool FindAttribute(Handle(TDF_Attribute)& theAttribute, const TCollection_AsciiString& theTypeOfAttribute);
  virtual bool ReferencedObject(Handle(SALOMEDSImpl_SObject)& theObject) ;
  virtual bool FindSubObject(int theTag, Handle(SALOMEDSImpl_SObject)& theObject);

  virtual Handle(SALOMEDSImpl_Study) GetStudy() ;
  virtual TCollection_AsciiString Name() { return _name; }
  virtual void Name(const TCollection_AsciiString& theName) { _name = theName; }
  virtual Handle(TColStd_HSequenceOfTransient) GetAllAttributes();

  virtual TCollection_AsciiString GetName();
  virtual TCollection_AsciiString GetComment();
  virtual TCollection_AsciiString GetIOR();

  virtual int Tag() { return _lab.Tag(); }
  virtual int Depth() { return _lab.Depth(); }

  virtual TDF_Label GetLabel() { return _lab; }   

  static Standard_GUID GetGUID(const TCollection_AsciiString& theTypeOfAttribute);

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_SObject )
};
#endif
