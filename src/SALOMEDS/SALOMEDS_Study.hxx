//  File   : SALOMEDS_Study.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDS_STUDY_H__
#define __SALOMEDS_STUDY_H__

#include <vector>
#include <string>

#include "SALOMEDSClient.hxx"
#include "SALOMEDSImpl_Study.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

class SALOMEDS_Study: public SALOMEDSClient_Study
{

private:
  bool                       _isLocal;
  Handle(SALOMEDSImpl_Study) _local_impl;
  SALOMEDS::Study_var        _corba_impl;
  CORBA::ORB_var             _orb;

public:

  SALOMEDS_Study(const Handle(SALOMEDSImpl_Study)& theStudy);
  SALOMEDS_Study(SALOMEDS::Study_ptr theStudy);
  ~SALOMEDS_Study();

  virtual std::string GetPersistentReference();
  virtual std::string GetTransientReference();
  virtual bool IsEmpty();
  virtual _PTR(SComponent) FindComponent (const std::string& aComponentName);
  virtual _PTR(SComponent) FindComponentID(const std::string& aComponentID);
  virtual _PTR(SObject) FindObject(const std::string& anObjectName);
  virtual std::vector<_PTR(SObject)> FindObjectByName( const std::string& anObjectName, const std::string& aComponentName ) ;  
  virtual _PTR(SObject) FindObjectID(const std::string& anObjectID);
  virtual _PTR(SObject) CreateObjectID(const std::string& anObjectID);
  virtual _PTR(SObject) FindObjectIOR(const std::string& anObjectIOR);
  virtual _PTR(SObject) FindObjectByPath(const std::string& thePath);
  virtual std::string GetObjectPath(const _PTR(SObject)& theSO);
  virtual void SetContext(const std::string& thePath);
  virtual std::string GetContext();  
  virtual std::vector<std::string> GetObjectNames(const std::string& theContext);
  virtual std::vector<std::string> GetDirectoryNames(const std::string& theContext);
  virtual std::vector<std::string> GetFileNames(const std::string& theContext);
  virtual std::vector<std::string> GetComponentNames(const std::string& theContext);
  virtual _PTR(ChildIterator) NewChildIterator(const _PTR(SObject)& theSO);
  virtual _PTR(SComponentIterator) NewComponentIterator();
  virtual _PTR(StudyBuilder) NewBuilder();
  virtual std::string Name();
  virtual void  Name(const std::string& name);
  virtual bool IsSaved();
  virtual void  IsSaved(bool save);
  virtual bool IsModified();
  virtual std::string URL();
  virtual void  URL(const std::string& url);
  virtual int StudyId();
  virtual void  StudyId(int id);
  virtual std::vector<_PTR(SObject)> FindDependances(const _PTR(SObject)& theSO);
  virtual _PTR(AttributeStudyProperties) GetProperties();
  virtual std::string GetLastModificationDate();
  virtual std::vector<std::string> GetModificationsDate();
  virtual _PTR(UseCaseBuilder) GetUseCaseBuilder();
  virtual void Close();
  virtual void EnableUseCaseAutoFilling(bool isEnabled);
  virtual bool DumpStudy(const std::string& thePath, const std::string& theBaseName, bool isPublished); 

  std::string ConvertObjectToIOR(CORBA::Object_ptr theObject);
  CORBA::Object_ptr ConvertIORToObject(const std::string& theIOR);     

  SALOMEDS::Study_ptr GetStudy();

private:
  void init_orb();

};
#endif
