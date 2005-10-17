//  File   : SALOMEDSClient_SObject.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSCLIENT_SOBJECT_H__
#define __SALOMEDSCLIENT_SOBJECT_H__

// std C++ headers
#include <vector>
#include <string> 

#include "SALOMEDSClient_definitions.hxx"

class SALOMEDSClient_Study;
class SALOMEDSClient_SComponent;
class SALOMEDSClient_GenericAttribute;

class SALOMEDSClient_SObject
{
public:
  virtual ~SALOMEDSClient_SObject() {}

  virtual std::string GetID()  = 0;
  virtual _PTR(SComponent) GetFatherComponent() = 0;
  virtual _PTR(SObject)    GetFather() = 0;
  virtual bool FindAttribute(_PTR(GenericAttribute)& anAttribute, const std::string& aTypeOfAttribute) = 0;
  virtual bool ReferencedObject(_PTR(SObject)& theObject) = 0;
  virtual bool FindSubObject(int theTag, _PTR(SObject)& theObject) = 0;
  virtual _PTR(Study) GetStudy() = 0;
  virtual std::string Name() = 0;
  virtual void  Name(const std::string& theName)  = 0;
  virtual std::vector<_PTR(GenericAttribute)> GetAllAttributes() = 0;
  virtual std::string GetName() = 0;
  virtual std::string GetComment() = 0;
  virtual std::string GetIOR() = 0;
  virtual int   Tag() = 0;
  virtual int   Depth() = 0;
};

#endif
