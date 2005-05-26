//  File   : SALOMEDSClient_StudyManager.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSClient_STUDYMANAGER_H__
#define __SALOMEDSClient_STUDYMANAGER_H__

#include <vector>
#include <string>

#include "SALOMEDSClient_definitions.hxx"
#include "SALOMEDSClient_Study.hxx"

class SALOMEDSClient_StudyManager
{
public:
  virtual ~SALOMEDSClient_StudyManager() {}

  virtual _PTR(Study) NewStudy(const std::string& study_name) = 0;
  virtual _PTR(Study) Open(const std::string& theStudyUrl) = 0;
  virtual void Close(const _PTR(Study)& theStudy) = 0;
  virtual void Save(const _PTR(Study)& theStudy, bool theMultiFile) = 0;
  virtual void SaveASCII(const _PTR(Study)& theStudy, bool theMultiFile) = 0;
  virtual void SaveAs(const std::string& theUrl,  const _PTR(Study)& theStudy, bool theMultiFile) = 0;
  virtual void SaveAsASCII(const std::string& theUrl,  const _PTR(Study)& theStudy, bool theMultiFile) = 0;
  virtual std::vector<std::string> GetOpenStudies() = 0;
  virtual _PTR(Study) GetStudyByName(const std::string& theStudyName) = 0;
  virtual _PTR(Study) GetStudyByID(int theStudyID) = 0;
  virtual bool CanCopy(const _PTR(SObject)& theSO) = 0;
  virtual bool Copy(const _PTR(SObject)& theSO) = 0;
  virtual bool CanPaste(const _PTR(SObject)& theSO) = 0;
  virtual _PTR(SObject) Paste(const _PTR(SObject)& theSO) = 0;
  
};

#endif 
