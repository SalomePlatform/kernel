//  File   : SALOMEDS_SObject.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDS_SOBJECT_H__
#define __SALOMEDS_SOBJECT_H__

// std C++ headers
#include <vector>

#include <SALOMEDSClient.hxx>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

#include "SALOMEDS_SObject_i.hxx"
#include "SALOMEDSImpl_SObject.hxx"

class Standard_EXPORT SALOMEDS_SObject: public virtual SALOMEDSClient_SObject
{
protected:

  bool                         _isLocal;
  Handle(SALOMEDSImpl_SObject) _local_impl;
  SALOMEDS::SObject_var        _corba_impl;
  CORBA::ORB_var               _orb;

public:
  
  SALOMEDS_SObject(SALOMEDS::SObject_ptr theSObject);
  SALOMEDS_SObject(const Handle(SALOMEDSImpl_SObject)& theSObject);       
  virtual ~SALOMEDS_SObject();

  virtual std::string GetID();
  virtual _PTR(SComponent) GetFatherComponent();
  virtual _PTR(SObject)    GetFather();
  virtual bool FindAttribute(_PTR(GenericAttribute)& anAttribute, const std::string& aTypeOfAttribute);
  virtual bool ReferencedObject(_PTR(SObject)& theObject);
  virtual bool FindSubObject(int theTag, _PTR(SObject)& theObject);
  virtual _PTR(Study) GetStudy();
  virtual std::string Name();
  virtual void  Name(const std::string& theName);
  virtual vector<_PTR(GenericAttribute)> GetAllAttributes();
  virtual std::string GetName();
  virtual std::string GetComment();
  virtual std::string GetIOR();
  virtual int   Tag();
  virtual int   Depth();

  CORBA::Object_ptr GetObject();
  SALOMEDS::SObject_ptr GetSObject();

  SALOMEDS::SObject_ptr        GetCORBAImpl() { return SALOMEDS::SObject::_duplicate(_corba_impl); }
  Handle(SALOMEDSImpl_SObject) GetLocalImpl() { return _local_impl; } 

private:
  void init_orb();
};

#endif
