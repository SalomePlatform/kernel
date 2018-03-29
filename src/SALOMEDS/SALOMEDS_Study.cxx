// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

//  File   : SALOMEDS_Study.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "utilities.h" 

#include "SALOMEDS_Study.hxx"

#include "SALOMEDS.hxx"
#include "SALOMEDS_SComponent.hxx"
#include "SALOMEDS_SObject.hxx"
#include "SALOMEDS_StudyBuilder.hxx"
#include "SALOMEDS_ChildIterator.hxx"
#include "SALOMEDS_SComponentIterator.hxx"
#include "SALOMEDS_AttributeStudyProperties.hxx"
#include "SALOMEDS_AttributeParameter.hxx"
#include "SALOMEDS_UseCaseBuilder.hxx"

#include "SALOMEDSImpl_SComponent.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_StudyBuilder.hxx"
#include "SALOMEDSImpl_ChildIterator.hxx"
#include "SALOMEDSImpl_SComponentIterator.hxx"
#include "SALOMEDSImpl_AttributeStudyProperties.hxx"
#include "SALOMEDSImpl_AttributeParameter.hxx"
#include "SALOMEDSImpl_GenericVariable.hxx"
#include "SALOMEDSImpl_UseCaseBuilder.hxx"

#include "SALOMEDS_Driver_i.hxx"
#include "SALOMEDS_Study_i.hxx"

#include "Utils_ORB_INIT.hxx" 
#include "Utils_SINGLETON.hxx" 

#include "Basics_Utils.hxx"

#ifdef WIN32
#include <process.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif

SALOMEDS_Study::SALOMEDS_Study(SALOMEDSImpl_Study* theStudy)
{
  _isLocal = true;
  _local_impl = theStudy;
  _corba_impl = SALOMEDS::Study::_nil();
  init_orb();
}

SALOMEDS_Study::SALOMEDS_Study(SALOMEDS::Study_ptr theStudy)
{
#ifdef WIN32
  long pid =  (long)_getpid();
#else
  long pid =  (long)getpid();
#endif  

  long addr = theStudy->GetLocalImpl(Kernel_Utils::GetHostname().c_str(), pid, _isLocal);
  if(_isLocal) {
    _local_impl = reinterpret_cast<SALOMEDSImpl_Study*>(addr);
    _corba_impl = SALOMEDS::Study::_duplicate(theStudy);
  }
  else {
    _local_impl = NULL;
    _corba_impl = SALOMEDS::Study::_duplicate(theStudy);
  }

  init_orb();
}

SALOMEDS_Study::~SALOMEDS_Study()
{
}

std::string SALOMEDS_Study::GetPersistentReference()
{
  std::string aRef;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aRef = _local_impl->GetPersistentReference();
  }
  else aRef = (CORBA::String_var)_corba_impl->GetPersistentReference();
  return aRef;
}

std::string SALOMEDS_Study::GetTransientReference()
{
  std::string aRef;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aRef = _local_impl->GetTransientReference();
  }
  else aRef = _corba_impl->GetTransientReference();
  return aRef;
}
 
bool SALOMEDS_Study::IsEmpty()
{
  bool ret;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    ret = _local_impl->IsEmpty();
  }
  else ret = _corba_impl->IsEmpty();
  return ret;
}

_PTR(SComponent) SALOMEDS_Study::FindComponent (const std::string& aComponentName)
{
  SALOMEDSClient_SComponent* aSCO = NULL;
  if (_isLocal) {
    SALOMEDS::Locker lock;

    SALOMEDSImpl_SComponent aSCO_impl = _local_impl->FindComponent(aComponentName);
    if (!aSCO_impl) return _PTR(SComponent)(aSCO);
    aSCO = new SALOMEDS_SComponent(aSCO_impl);
  }
  else {
    SALOMEDS::SComponent_var aSCO_impl = _corba_impl->FindComponent((char*)aComponentName.c_str());
    if (CORBA::is_nil(aSCO_impl)) return _PTR(SComponent)(aSCO);
    aSCO = new SALOMEDS_SComponent(aSCO_impl);
  }
  return _PTR(SComponent)(aSCO);
}
 
_PTR(SComponent) SALOMEDS_Study::FindComponentID(const std::string& aComponentID)
{  
  SALOMEDSClient_SComponent* aSCO = NULL;
  if (_isLocal) {
    SALOMEDS::Locker lock;

    SALOMEDSImpl_SComponent aSCO_impl = _local_impl->FindComponentID(aComponentID);
    if (!aSCO_impl) return _PTR(SComponent)(aSCO);
    aSCO = new SALOMEDS_SComponent(aSCO_impl);
  }
  else {
    SALOMEDS::SComponent_var aSCO_impl = _corba_impl->FindComponentID((char*)aComponentID.c_str());
    if(CORBA::is_nil(aSCO_impl)) return _PTR(SComponent)(aSCO);
    aSCO = new SALOMEDS_SComponent(aSCO_impl);
  }
  return _PTR(SComponent)(aSCO);
}
 
_PTR(SObject) SALOMEDS_Study::FindObject(const std::string& anObjectName)
{
  SALOMEDSClient_SObject* aSO = NULL;

  if (_isLocal) {
    SALOMEDS::Locker lock;

    SALOMEDSImpl_SObject aSO_impl = _local_impl->FindObject(anObjectName);
    if (!aSO_impl) return _PTR(SObject)(aSO);
    if(aSO_impl.IsComponent()) {
        SALOMEDSImpl_SComponent aSCO_impl = aSO_impl;
        return _PTR(SObject)(new SALOMEDS_SComponent(aSCO_impl));
    }   
    aSO = new SALOMEDS_SObject(aSO_impl);
  }
  else { 
    SALOMEDS::SObject_var aSO_impl = _corba_impl->FindObject((char*)anObjectName.c_str());
    if (CORBA::is_nil(aSO_impl)) return _PTR(SObject)(aSO);
    SALOMEDS::SComponent_var aSCO_impl = SALOMEDS::SComponent::_narrow(aSO_impl);
    if (!CORBA::is_nil(aSCO_impl)) return _PTR(SObject)(new SALOMEDS_SComponent(aSCO_impl));
    aSO = new SALOMEDS_SObject(aSO_impl);
  }

  return _PTR(SObject)(aSO);
}

std::vector<_PTR(SObject)> SALOMEDS_Study::FindObjectByName(const std::string& anObjectName, 
                                                            const std::string& aComponentName)   
{
  std::vector<_PTR(SObject)> aVector;
  int i, aLength = 0;

  if (_isLocal) {
    SALOMEDS::Locker lock;

    std::vector<SALOMEDSImpl_SObject> aSeq = _local_impl->FindObjectByName(anObjectName, aComponentName);
    aLength = aSeq.size();
    for (i = 0; i< aLength; i++) 
      aVector.push_back(_PTR(SObject)(new SALOMEDS_SObject(aSeq[i])));
  }
  else {
    SALOMEDS::Study::ListOfSObject_var aSeq = _corba_impl->FindObjectByName((char*)anObjectName.c_str(), 
                                                                            (char*)aComponentName.c_str());
    aLength = aSeq->length();
    for (i = 0; i< aLength; i++) aVector.push_back(_PTR(SObject)(new SALOMEDS_SObject(aSeq[i])));
  }

  return aVector;
}

_PTR(SObject) SALOMEDS_Study::FindObjectID(const std::string& anObjectID)
{
  SALOMEDSClient_SObject* aSO = NULL;
  if (_isLocal) {
    SALOMEDS::Locker lock;

    SALOMEDSImpl_SObject aSO_impl = _local_impl->FindObjectID(anObjectID);
    if(!aSO_impl) return _PTR(SObject)(aSO);
    return _PTR(SObject)(new SALOMEDS_SObject(aSO_impl));
  }
  else { 
    SALOMEDS::SObject_var aSO_impl = _corba_impl->FindObjectID((char*)anObjectID.c_str());
    if(CORBA::is_nil(aSO_impl)) return _PTR(SObject)(aSO);
    return _PTR(SObject)(new SALOMEDS_SObject(aSO_impl));
  }
  return _PTR(SObject)(aSO);
}
 
_PTR(SObject) SALOMEDS_Study::CreateObjectID(const std::string& anObjectID)
{
  SALOMEDSClient_SObject* aSO = NULL;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    SALOMEDSImpl_SObject aSO_impl = _local_impl->CreateObjectID(anObjectID);
    if(!aSO_impl) return _PTR(SObject)(aSO);
    aSO = new SALOMEDS_SObject(aSO_impl);
  }
  else { 
    SALOMEDS::SObject_var aSO_impl = _corba_impl->CreateObjectID((char*)anObjectID.c_str());
    if(CORBA::is_nil(aSO_impl)) return _PTR(SObject)(aSO);
    aSO = new SALOMEDS_SObject(aSO_impl);
  }
  return _PTR(SObject)(aSO);
}

_PTR(SObject) SALOMEDS_Study::FindObjectIOR(const std::string& anObjectIOR)
{
  SALOMEDSClient_SObject* aSO = NULL;
  if (_isLocal) {
    SALOMEDS::Locker lock;

    SALOMEDSImpl_SObject aSO_impl = _local_impl->FindObjectIOR(anObjectIOR);
    if (!aSO_impl) return _PTR(SObject)(aSO);
    aSO = new SALOMEDS_SObject(aSO_impl);
  }
  else { 
    SALOMEDS::SObject_var aSO_impl = _corba_impl->FindObjectIOR((char*)anObjectIOR.c_str());
    if (CORBA::is_nil(aSO_impl)) return _PTR(SObject)(aSO);
    aSO = new SALOMEDS_SObject(aSO_impl);
  }
  return _PTR(SObject)(aSO);
}

_PTR(SObject) SALOMEDS_Study::FindObjectByPath(const std::string& thePath)
{
  SALOMEDSClient_SObject* aSO = NULL;
  if (_isLocal) {
    SALOMEDS::Locker lock;

    SALOMEDSImpl_SObject aSO_impl = _local_impl->FindObjectByPath(thePath);
    if (!aSO_impl) return _PTR(SObject)(aSO);
    aSO = new SALOMEDS_SObject(aSO_impl);
  }
  else {
    SALOMEDS::SObject_var aSO_impl = _corba_impl->FindObjectByPath((char*)thePath.c_str());
    if (CORBA::is_nil(aSO_impl)) return _PTR(SObject)(aSO);
    aSO = new SALOMEDS_SObject(aSO_impl);
  }
  return _PTR(SObject)(aSO);
}

std::string SALOMEDS_Study::GetObjectPath(const _PTR(SObject)& theSO)
{
  if(!theSO) return "";
  SALOMEDS_SObject* aSO = dynamic_cast<SALOMEDS_SObject*>(theSO.get());
  std::string aPath;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aPath = _local_impl->GetObjectPath(*(aSO->GetLocalImpl()));
  }
  else aPath = _corba_impl->GetObjectPath(aSO->GetCORBAImpl());
  return aPath;
}

void SALOMEDS_Study::SetContext(const std::string& thePath)
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    _local_impl->SetContext(thePath);
  }
  else _corba_impl->SetContext((char*)thePath.c_str());
}

std::string SALOMEDS_Study::GetContext()  
{
  std::string aPath;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aPath = _local_impl->GetContext();
  }
  else aPath = _corba_impl->GetContext();
  return aPath;
}

std::vector<std::string> SALOMEDS_Study::GetObjectNames(const std::string& theContext)
{
  std::vector<std::string> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aVector = _local_impl->GetObjectNames(theContext);
  }
  else {
    SALOMEDS::ListOfStrings_var aSeq = _corba_impl->GetObjectNames((char*)theContext.c_str());
    aLength = aSeq->length();
    for (i = 0; i < aLength; i++) aVector.push_back(std::string((std::string)aSeq[i].in()));
  }
  return aVector;
}
 
std::vector<std::string> SALOMEDS_Study::GetDirectoryNames(const std::string& theContext)
{
  std::vector<std::string> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aVector = _local_impl->GetDirectoryNames(theContext);
  }
  else {
    SALOMEDS::ListOfStrings_var aSeq = _corba_impl->GetDirectoryNames((char*)theContext.c_str());
    aLength = aSeq->length();
    for (i = 0; i < aLength; i++) aVector.push_back((char*)aSeq[i].in());
  }
  return aVector;
}
 
std::vector<std::string> SALOMEDS_Study::GetFileNames(const std::string& theContext)
{
  std::vector<std::string> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aVector = _local_impl->GetFileNames(theContext);
  }
  else {
    SALOMEDS::ListOfStrings_var aSeq = _corba_impl->GetFileNames((char*)theContext.c_str());
    aLength = aSeq->length();

    for (i = 0; i < aLength; i++) aVector.push_back((char*)aSeq[i].in());
  }
  return aVector;
}
 
std::vector<std::string> SALOMEDS_Study::GetComponentNames(const std::string& theContext)
{
  std::vector<std::string> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aVector = _local_impl->GetComponentNames(theContext);
  }
  else {
    SALOMEDS::ListOfStrings_var aSeq = _corba_impl->GetComponentNames((char*)theContext.c_str());
    aLength = aSeq->length();
    for (i = 0; i < aLength; i++) aVector.push_back((char*)aSeq[i].in());
  }
  return aVector;
}

_PTR(ChildIterator) SALOMEDS_Study::NewChildIterator(const _PTR(SObject)& theSO)
{
  SALOMEDS_SObject* aSO = dynamic_cast<SALOMEDS_SObject*>(theSO.get());
  SALOMEDSClient_ChildIterator* aCI = NULL; 
  if (_isLocal) {
    SALOMEDS::Locker lock;
    SALOMEDSImpl_ChildIterator aCIimpl = _local_impl->NewChildIterator(*(aSO->GetLocalImpl()));
    aCI = new SALOMEDS_ChildIterator(aCIimpl);
  }
  else {
    SALOMEDS::ChildIterator_var aCIimpl = _corba_impl->NewChildIterator(aSO->GetCORBAImpl());
    aCI = new SALOMEDS_ChildIterator(aCIimpl);
  }

  return _PTR(ChildIterator)(aCI);
}

_PTR(SComponentIterator) SALOMEDS_Study::NewComponentIterator()
{
  SALOMEDSClient_SComponentIterator* aCI = NULL; 
  if (_isLocal) {
    SALOMEDS::Locker lock;

    SALOMEDSImpl_SComponentIterator aCIimpl = _local_impl->NewComponentIterator();
    aCI = new SALOMEDS_SComponentIterator(aCIimpl);
  }
  else {
    SALOMEDS::SComponentIterator_var aCIimpl = _corba_impl->NewComponentIterator();
    aCI = new SALOMEDS_SComponentIterator(aCIimpl);
  }

  return _PTR(SComponentIterator)(aCI);
}

_PTR(StudyBuilder) SALOMEDS_Study::NewBuilder()
{
  SALOMEDSClient_StudyBuilder* aSB = NULL; 
  if (_isLocal) {
    SALOMEDS::Locker lock;

    SALOMEDSImpl_StudyBuilder* aSBimpl = _local_impl->NewBuilder();
    aSB = new SALOMEDS_StudyBuilder(aSBimpl);
  }
  else {
    SALOMEDS::StudyBuilder_var aSBimpl = _corba_impl->NewBuilder();
    aSB = new SALOMEDS_StudyBuilder(aSBimpl);
  }

  return _PTR(StudyBuilder)(aSB);
}

std::string SALOMEDS_Study::Name()
{
  std::string aName;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aName = _local_impl->Name();
  }
  else aName = _corba_impl->Name();
  return aName;
}

void SALOMEDS_Study::Name(const std::string& theName)
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    _local_impl->Name(theName);
  }
  else _corba_impl->Name((char*)theName.c_str());
}

bool SALOMEDS_Study::IsSaved()
{
  bool isSaved;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    isSaved = _local_impl->IsSaved();
  }
  else isSaved = _corba_impl->IsSaved();
  return isSaved;
}

void SALOMEDS_Study::IsSaved(bool save)
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    _local_impl->IsSaved(save);
  }
  else _corba_impl->IsSaved(save);
}

bool SALOMEDS_Study::IsModified()
{
  bool isModified;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    isModified = _local_impl->IsModified();
  }
  else isModified = _corba_impl->IsModified();
  return isModified;
}

void SALOMEDS_Study::Modified()
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    _local_impl->Modify();
  }
  else _corba_impl->Modified();
}

 
std::string SALOMEDS_Study::URL()
{
  std::string aURL;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aURL = _local_impl->URL();
  }
  else aURL = _corba_impl->URL();
  return aURL;
}

void SALOMEDS_Study::URL(const std::string& url)
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    _local_impl->URL(url);
  }
  else _corba_impl->URL((char*)url.c_str());
}

int SALOMEDS_Study::StudyId()
{
  int anID;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    anID = _local_impl->StudyId();
  }
  else anID = _corba_impl->StudyId();
  return anID;
}
 
void SALOMEDS_Study::StudyId(int id) 
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    _local_impl->StudyId(id);
  }
  else _corba_impl->StudyId(id);  
}

std::vector<_PTR(SObject)> SALOMEDS_Study::FindDependances(const _PTR(SObject)& theSO)
{
  std::vector<_PTR(SObject)> aVector;
  SALOMEDS_SObject* aSO = dynamic_cast<SALOMEDS_SObject*>(theSO.get());
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock;

    std::vector<SALOMEDSImpl_SObject> aSeq = _local_impl->FindDependances(*(aSO->GetLocalImpl()));
    if (aSeq.size()) {
      aLength = aSeq.size();
      for (i = 0; i < aLength; i++) 
        aVector.push_back(_PTR(SObject)(new SALOMEDS_SObject(aSeq[i])));
    }
  }
  else {
    SALOMEDS::Study::ListOfSObject_var aSeq = _corba_impl->FindDependances(aSO->GetCORBAImpl());
    aLength = aSeq->length();
    for (i = 0; i < aLength; i++) aVector.push_back(_PTR(SObject)(new SALOMEDS_SObject(aSeq[i])));
  }
  return aVector;
}
 
_PTR(AttributeStudyProperties) SALOMEDS_Study::GetProperties()
{
  SALOMEDSClient_AttributeStudyProperties* aProp;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aProp = new SALOMEDS_AttributeStudyProperties(_local_impl->GetProperties());
  }
  else aProp = new SALOMEDS_AttributeStudyProperties((SALOMEDS::AttributeStudyProperties_var)_corba_impl->GetProperties());
  return _PTR(AttributeStudyProperties)(aProp);
}
 
std::string SALOMEDS_Study::GetLastModificationDate() 
{
  std::string aDate;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aDate = _local_impl->GetLastModificationDate();
  }
  else aDate = _corba_impl->GetLastModificationDate();
  return aDate;
}

std::vector<std::string> SALOMEDS_Study::GetModificationsDate()
{
  std::vector<std::string> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aVector = _local_impl->GetModificationsDate();
  }
  else {
    SALOMEDS::ListOfDates_var aSeq = _corba_impl->GetModificationsDate();
    aLength = aSeq->length();
    for (i = 0; i < aLength; i++) aVector.push_back((char*)aSeq[i].in());
  }
  return aVector;
}

_PTR(UseCaseBuilder) SALOMEDS_Study::GetUseCaseBuilder()
{
  SALOMEDSClient_UseCaseBuilder* aUB = NULL;
  if (_isLocal) {
    SALOMEDS::Locker lock;

    SALOMEDSImpl_UseCaseBuilder* aUBimpl = _local_impl->GetUseCaseBuilder();
    aUB = new SALOMEDS_UseCaseBuilder(aUBimpl);
  }
  else {
    SALOMEDS::UseCaseBuilder_var aUBimpl = _corba_impl->GetUseCaseBuilder();
    aUB = new SALOMEDS_UseCaseBuilder(aUBimpl);
  }

  return _PTR(UseCaseBuilder)(aUB);
}

void SALOMEDS_Study::Close()
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    _local_impl->Close();
  }
  else _corba_impl->Close();
}

void SALOMEDS_Study::EnableUseCaseAutoFilling(bool isEnabled)
{
  if(_isLocal) _local_impl->EnableUseCaseAutoFilling(isEnabled);
  else _corba_impl->EnableUseCaseAutoFilling(isEnabled);
}

bool SALOMEDS_Study::DumpStudy(const std::string& thePath,
                               const std::string& theBaseName,
                               bool isPublished,
                               bool isMultiFile)
{
  //SRN: Pure CORBA DumpStudy as it does more cleaning than the local one
  if(CORBA::is_nil(_corba_impl)) GetStudy(); //If CORBA implementation is null then retrieve it
  bool ret = _corba_impl->DumpStudy(thePath.c_str(), theBaseName.c_str(), isPublished, isMultiFile);
  return ret;
}     

void SALOMEDS_Study::SetStudyLock(const std::string& theLockerID)
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    _local_impl->SetStudyLock(theLockerID.c_str());
  }
  else _corba_impl->SetStudyLock((char*)theLockerID.c_str());
}
 
bool SALOMEDS_Study::IsStudyLocked()
{
  bool isLocked;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    isLocked = _local_impl->IsStudyLocked();
  }
  else isLocked = _corba_impl->IsStudyLocked();
  return isLocked;
}
 
void SALOMEDS_Study::UnLockStudy(const std::string& theLockerID)
{
  if(_isLocal) _local_impl->UnLockStudy(theLockerID.c_str());
  else _corba_impl->UnLockStudy((char*)theLockerID.c_str());
}

std::vector<std::string> SALOMEDS_Study::GetLockerID()
{
  std::vector<std::string> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aVector = _local_impl->GetLockerID();
  }
  else {
    SALOMEDS::ListOfStrings_var aSeq = _corba_impl->GetLockerID();
    aLength = aSeq->length();
    for (i = 0; i < aLength; i++) aVector.push_back((char*)aSeq[i].in());
  }
  return aVector;
}


void SALOMEDS_Study::SetReal(const std::string& theVarName, const double theValue)
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    _local_impl->SetVariable(theVarName,
                             theValue,
                             SALOMEDSImpl_GenericVariable::REAL_VAR);
  }
  else 
    _corba_impl->SetReal((char*)theVarName.c_str(),theValue);
}

void SALOMEDS_Study::SetInteger(const std::string& theVarName, const int theValue)
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    _local_impl->SetVariable(theVarName,
                             theValue,
                             SALOMEDSImpl_GenericVariable::INTEGER_VAR);
  }
  else 
    _corba_impl->SetInteger((char*)theVarName.c_str(),theValue);
}

void SALOMEDS_Study::SetBoolean(const std::string& theVarName, const bool theValue)
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    _local_impl->SetVariable(theVarName,
                             theValue,
                             SALOMEDSImpl_GenericVariable::BOOLEAN_VAR);
  }
  else 
    _corba_impl->SetBoolean((char*)theVarName.c_str(),theValue);
}

void SALOMEDS_Study::SetString(const std::string& theVarName, const std::string& theValue)
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    _local_impl->SetStringVariable(theVarName,
                                   theValue,
                                   SALOMEDSImpl_GenericVariable::STRING_VAR);
  }
  else 
    _corba_impl->SetString((char*)theVarName.c_str(),(char*)theValue.c_str());
}

void SALOMEDS_Study::SetStringAsDouble(const std::string& theVarName, const double theValue)
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    _local_impl->SetStringVariableAsDouble(theVarName,
                                           theValue,
                                           SALOMEDSImpl_GenericVariable::STRING_VAR);
  }
  else 
    _corba_impl->SetStringAsDouble((char*)theVarName.c_str(),theValue);
}

double SALOMEDS_Study::GetReal(const std::string& theVarName)
{
  double aResult;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aResult = _local_impl->GetVariableValue(theVarName);
  }
  else 
    aResult = _corba_impl->GetReal((char*)theVarName.c_str());
  return aResult;
}

int SALOMEDS_Study::GetInteger(const std::string& theVarName)
{
  int aResult;  
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aResult = (int) _local_impl->GetVariableValue(theVarName);
  }
  else 
    aResult = _corba_impl->GetInteger((char*)theVarName.c_str());
  return aResult;
}

bool SALOMEDS_Study::GetBoolean(const std::string& theVarName)
{
  bool aResult;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aResult = (bool) _local_impl->GetVariableValue(theVarName);
  }
  else 
    aResult = _corba_impl->GetBoolean((char*)theVarName.c_str());
  return aResult;
}

std::string SALOMEDS_Study::GetString(const std::string& theVarName)
{
  std::string aResult;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aResult = _local_impl->GetStringVariableValue(theVarName);
  }
  else 
    aResult = _corba_impl->GetString((char*)theVarName.c_str());
  return aResult;
}

bool SALOMEDS_Study::IsReal(const std::string& theVarName)
{
  bool aResult;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aResult = _local_impl->IsTypeOf(theVarName, 
                                   SALOMEDSImpl_GenericVariable::REAL_VAR);
  }
  else
    aResult = _corba_impl->IsReal((char*)theVarName.c_str());
  return aResult;
}

bool SALOMEDS_Study::IsInteger(const std::string& theVarName)
{
  bool aResult;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aResult = _local_impl->IsTypeOf(theVarName, 
                                   SALOMEDSImpl_GenericVariable::INTEGER_VAR);
  }
  else
    aResult = _corba_impl->IsInteger((char*)theVarName.c_str());
  return aResult;
}

bool SALOMEDS_Study::IsBoolean(const std::string& theVarName)
{
  bool aResult;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aResult = _local_impl->IsTypeOf(theVarName, 
                                   SALOMEDSImpl_GenericVariable::BOOLEAN_VAR);
  }
  else
    aResult = _corba_impl->IsBoolean((char*)theVarName.c_str());
  return aResult;
}

bool SALOMEDS_Study::IsString(const std::string& theVarName)
{
  bool aResult;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aResult = _local_impl->IsTypeOf(theVarName, 
                                    SALOMEDSImpl_GenericVariable::STRING_VAR);
  }
  else
    aResult = _corba_impl->IsString((char*)theVarName.c_str());
  return aResult;
}

bool SALOMEDS_Study::IsVariable(const std::string& theVarName)
{
  bool aResult;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aResult = _local_impl->IsVariable(theVarName);
  }
  else
    aResult = _corba_impl->IsVariable((char*)theVarName.c_str());
  return aResult;
}

std::vector<std::string> SALOMEDS_Study::GetVariableNames()
{
  std::vector<std::string> aVector;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aVector = _local_impl->GetVariableNames();
  }
  else {
    SALOMEDS::ListOfStrings_var aSeq = _corba_impl->GetVariableNames();
    int aLength = aSeq->length();
    for (int i = 0; i < aLength; i++) 
      aVector.push_back( std::string(aSeq[i].in()) );
  }
  return aVector;
}

bool SALOMEDS_Study::RemoveVariable(const std::string& theVarName)
{
  bool aResult;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aResult = _local_impl->RemoveVariable(theVarName);
  }
  else
    aResult = _corba_impl->RemoveVariable((char*)theVarName.c_str());
  return aResult;
}

bool SALOMEDS_Study::RenameVariable(const std::string& theVarName, const std::string& theNewVarName)
{
  bool aResult;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aResult = _local_impl->RenameVariable(theVarName, theNewVarName);
  }
  else
    aResult = _corba_impl->RenameVariable((char*)theVarName.c_str(), (char*)theNewVarName.c_str());
  return aResult;
}

bool SALOMEDS_Study::IsVariableUsed(const std::string& theVarName)
{
  bool aResult;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aResult = _local_impl->IsVariableUsed(theVarName);
  }
  else
    aResult = _corba_impl->IsVariableUsed((char*)theVarName.c_str());
  return aResult;
}

std::vector< std::vector<std::string> > SALOMEDS_Study::ParseVariables(const std::string& theVars)
{
  std::vector< std::vector<std::string> > aResult;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aResult = _local_impl->ParseVariables(theVars);
  }
  else {
    SALOMEDS::ListOfListOfStrings_var aSeq = _corba_impl->ParseVariables(theVars.c_str());
    for (int i = 0, n = aSeq->length(); i < n; i++) {
      std::vector<std::string> aVector;
      SALOMEDS::ListOfStrings aSection = aSeq[i];
      for (int j = 0, m = aSection.length(); j < m; j++) {
        aVector.push_back( std::string(aSection[j].in()) );
      }
      aResult.push_back( aVector );
    }
  }
  return aResult;
}

std::string SALOMEDS_Study::ConvertObjectToIOR(CORBA::Object_ptr theObject) 
{
  return _orb->object_to_string(theObject); 
}

CORBA::Object_ptr SALOMEDS_Study::ConvertIORToObject(const std::string& theIOR) 
{ 
  return _orb->string_to_object(theIOR.c_str()); 
} 

void SALOMEDS_Study::init_orb()
{
  ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance() ;
  ASSERT(SINGLETON_<ORB_INIT>::IsAlreadyExisting()); 
  _orb = init(0 , 0 ) ;     
}

SALOMEDS::Study_ptr SALOMEDS_Study::GetStudy()
{
  if (_isLocal) {
    SALOMEDS::Locker lock;

    if (!CORBA::is_nil(_corba_impl)) return SALOMEDS::Study::_duplicate(_corba_impl);
    std::string anIOR = _local_impl->GetTransientReference();
    SALOMEDS::Study_var aStudy;
    if (!_local_impl->IsError() && anIOR != "") {
      aStudy = SALOMEDS::Study::_narrow(_orb->string_to_object(anIOR.c_str()));
    }
    else {
      SALOMEDS_Study_i *aStudy_servant = new SALOMEDS_Study_i(_local_impl, _orb);
      aStudy = aStudy_servant->_this();
      _local_impl->SetTransientReference(_orb->object_to_string(aStudy));
    }
    _corba_impl = SALOMEDS::Study::_duplicate(aStudy);
    return aStudy._retn();
  }
  else {
    return SALOMEDS::Study::_duplicate(_corba_impl);
  }

  return SALOMEDS::Study::_nil();
}


_PTR(AttributeParameter) SALOMEDS_Study::GetCommonParameters(const std::string& theID, int theSavePoint)
{
  SALOMEDSClient_AttributeParameter* AP = NULL;
  if(theSavePoint >= 0) {
    if (_isLocal) {
      SALOMEDS::Locker lock;
      AP = new SALOMEDS_AttributeParameter(_local_impl->GetCommonParameters(theID.c_str(), theSavePoint));
    }
    else {
      AP = new SALOMEDS_AttributeParameter(_corba_impl->GetCommonParameters(theID.c_str(), theSavePoint));
    }
  }
  return _PTR(AttributeParameter)(AP);
}

_PTR(AttributeParameter) SALOMEDS_Study::GetModuleParameters(const std::string& theID, 
                                                             const std::string& theModuleName, int theSavePoint)
{
  SALOMEDSClient_AttributeParameter* AP = NULL;
  if(theSavePoint > 0) {
    if (_isLocal) {
      SALOMEDS::Locker lock;
      AP = new SALOMEDS_AttributeParameter(_local_impl->GetModuleParameters(theID.c_str(), theModuleName.c_str(), theSavePoint));
    }
    else {
      AP = new SALOMEDS_AttributeParameter(_corba_impl->GetModuleParameters(theID.c_str(), theModuleName.c_str(), theSavePoint));
    }
  }
  return _PTR(AttributeParameter)(AP);
}

void SALOMEDS_Study::attach(SALOMEDS::Observer_ptr theObserver,bool modify)
{
  if(CORBA::is_nil(_corba_impl)) GetStudy(); //If CORBA implementation is null then retrieve it
  _corba_impl->attach(theObserver,modify);
}

void SALOMEDS_Study::detach(SALOMEDS::Observer_ptr theObserver)
{
  if(CORBA::is_nil(_corba_impl)) GetStudy(); //If CORBA implementation is null then retrieve it
  _corba_impl->detach(theObserver);
}
