//  File   : SALOMEDS_StudyManager.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDS_STUDYMANAGER_H__
#define __SALOMEDS_STUDYMANAGER_H__

#include <vector>
#include <string>

#include "SALOMEDSClient.hxx"
#include "SALOMEDSImpl_StudyManager.hxx"

// IDL headers

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

class Standard_EXPORT SALOMEDS_StudyManager: public SALOMEDSClient_StudyManager
{
private:
  bool                              _isLocal;
  Handle(SALOMEDSImpl_StudyManager) _local_impl;
  SALOMEDS::StudyManager_var        _corba_impl;  
  CORBA::ORB_var                    _orb;

public:

  SALOMEDS_StudyManager(SALOMEDS::StudyManager_ptr theManager);
  SALOMEDS_StudyManager();
  ~SALOMEDS_StudyManager();

  virtual _PTR(Study) NewStudy(const std::string& study_name);
  virtual _PTR(Study) Open(const std::string& theStudyUrl);
  virtual void Close(const _PTR(Study)& theStudy);
  virtual void Save(const _PTR(Study)& theStudy, bool theMultiFile);
  virtual void SaveASCII(const _PTR(Study)& theStudy, bool theMultiFile);
  virtual void SaveAs(const std::string& theUrl,  const _PTR(Study)& theStudy, bool theMultiFile);
  virtual void SaveAsASCII(const std::string& theUrl,  const _PTR(Study)& theStudy, bool theMultiFile);
  virtual std::vector<std::string> GetOpenStudies();
  virtual _PTR(Study) GetStudyByName(const std::string& theStudyName) ;
  virtual _PTR(Study) GetStudyByID(int theStudyID) ;
  virtual bool CanCopy(const _PTR(SObject)& theSO);
  virtual bool Copy(const _PTR(SObject)& theSO);
  virtual bool CanPaste(const _PTR(SObject)& theSO);
  virtual _PTR(SObject) Paste(const _PTR(SObject)& theSO); 

private:
  void init_orb();
};

#endif 
