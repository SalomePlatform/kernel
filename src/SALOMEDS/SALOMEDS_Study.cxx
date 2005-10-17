//  File   : SALOMEDS_Study.cxx
//  Author : Sergey RUIN
//  Module : SALOME



#include "utilities.h" 

#include "SALOMEDS_Study.hxx"
#include "SALOMEDS_SComponent.hxx"
#include "SALOMEDSImpl_SComponent.hxx"
#include "SALOMEDS_SObject.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDS_StudyBuilder.hxx"
#include "SALOMEDSImpl_StudyBuilder.hxx"
#include "SALOMEDS_ChildIterator.hxx"
#include "SALOMEDSImpl_ChildIterator.hxx"
#include "SALOMEDS_SComponentIterator.hxx"
#include "SALOMEDSImpl_SComponentIterator.hxx"
#include "SALOMEDS_AttributeStudyProperties.hxx"
#include "SALOMEDSImpl_AttributeStudyProperties.hxx"
#include "SALOMEDS_UseCaseBuilder.hxx"
#include "SALOMEDSImpl_UseCaseBuilder.hxx"

#include "SALOMEDS_Driver_i.hxx"
#include "SALOMEDS_Study_i.hxx"

#include <TCollection_AsciiString.hxx> 
#include <TColStd_HSequenceOfAsciiString.hxx>
#include <TColStd_HSequenceOfTransient.hxx>

#include "Utils_ORB_INIT.hxx" 
#include "Utils_SINGLETON.hxx" 

#ifdef WIN32
#include <process.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif

#include "OpUtil.hxx"

using namespace std; 

SALOMEDS_Study::SALOMEDS_Study(const Handle(SALOMEDSImpl_Study)& theStudy)
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

  long addr = theStudy->GetLocalImpl(GetHostname().c_str(), pid, _isLocal);
  if(_isLocal) {
    _local_impl = ((SALOMEDSImpl_Study*)(addr));
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
  if(_isLocal) aRef = _local_impl->GetPersistentReference().ToCString();
  else aRef = _corba_impl->GetPersistentReference();
  return aRef;
}

std::string SALOMEDS_Study::GetTransientReference()
{
  std::string aRef;
  if(_isLocal) aRef = _local_impl->GetTransientReference().ToCString();
  else aRef = _corba_impl->GetTransientReference();
  return aRef;
}
 
bool SALOMEDS_Study::IsEmpty()
{
  bool ret;
  if(_isLocal) ret = _local_impl->IsEmpty();
  else ret = _corba_impl->IsEmpty();
  return ret;
}

_PTR(SComponent) SALOMEDS_Study::FindComponent (const std::string& aComponentName)
{
  SALOMEDSClient_SComponent* aSCO = NULL;
  if(_isLocal) {
    Handle(SALOMEDSImpl_SComponent) aSCO_impl =_local_impl->FindComponent((char*)aComponentName.c_str());
    if(aSCO_impl.IsNull()) return _PTR(SComponent)(aSCO);
    aSCO = new SALOMEDS_SComponent(aSCO_impl);
  }
  else {
    SALOMEDS::SComponent_var aSCO_impl = _corba_impl->FindComponent((char*)aComponentName.c_str());
    if(CORBA::is_nil(aSCO_impl)) return _PTR(SComponent)(aSCO);
    aSCO = new SALOMEDS_SComponent(aSCO_impl);
  }
  return _PTR(SComponent)(aSCO);
}
 
_PTR(SComponent) SALOMEDS_Study::FindComponentID(const std::string& aComponentID)
{  
  SALOMEDSClient_SComponent* aSCO = NULL;
  if(_isLocal) {
    Handle(SALOMEDSImpl_SComponent) aSCO_impl =_local_impl->FindComponentID((char*)aComponentID.c_str());
    if(aSCO_impl.IsNull()) return _PTR(SComponent)(aSCO);
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

  if(_isLocal) {
    Handle(SALOMEDSImpl_SObject) aSO_impl = _local_impl->FindObject((char*)anObjectName.c_str());
    if(aSO_impl.IsNull()) return _PTR(SObject)(aSO);
    Handle(SALOMEDSImpl_SComponent) aSCO_impl = Handle(SALOMEDSImpl_SComponent)::DownCast(aSO_impl);
    if(!aSCO_impl.IsNull()) return _PTR(SObject)(new SALOMEDS_SComponent(aSCO_impl));
    aSO = new SALOMEDS_SObject(aSO_impl);
  }
  else { 
    SALOMEDS::SObject_var aSO_impl = _corba_impl->FindObject((char*)anObjectName.c_str());
    if(CORBA::is_nil(aSO_impl)) return _PTR(SObject)(aSO);
    SALOMEDS::SComponent_var aSCO_impl = SALOMEDS::SComponent::_narrow(aSO_impl);
    if(!CORBA::is_nil(aSCO_impl)) return _PTR(SObject)(new SALOMEDS_SComponent(aSCO_impl));
    aSO = new SALOMEDS_SObject(aSO_impl);
  }

  return _PTR(SObject)(aSO);
}
 
std::vector<_PTR(SObject)> SALOMEDS_Study::FindObjectByName(const std::string& anObjectName, 
								      const std::string& aComponentName)   
{
  std::vector<_PTR(SObject)> aVector;
  int i, aLength = 0;
  
  if(_isLocal) {
    Handle(TColStd_HSequenceOfTransient) aSeq = _local_impl->FindObjectByName((char*)anObjectName.c_str(), (char*)aComponentName.c_str());
    aLength = aSeq->Length();
    for(i = 1; i<= aLength; i++) 
      aVector.push_back(_PTR(SObject)(new SALOMEDS_SObject(Handle(SALOMEDSImpl_SObject)::DownCast(aSeq->Value(i)))));
  }
  else {
    SALOMEDS::Study::ListOfSObject_var aSeq = _corba_impl->FindObjectByName((char*)anObjectName.c_str(), 
									    (char*)aComponentName.c_str());
    aLength = aSeq->length();
    for(i = 0; i< aLength; i++) aVector.push_back(_PTR(SObject)(new SALOMEDS_SObject(aSeq[i])));
  }

  return aVector;
}
 
_PTR(SObject) SALOMEDS_Study::FindObjectID(const std::string& anObjectID)
{
  SALOMEDSClient_SObject* aSO = NULL;
  if(_isLocal) {
    Handle(SALOMEDSImpl_SObject) aSO_impl = _local_impl->FindObjectID((char*)anObjectID.c_str());
    if(aSO_impl.IsNull()) return _PTR(SObject)(aSO);
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
  if(_isLocal) aSO = new SALOMEDS_SObject(_local_impl->CreateObjectID((char*)anObjectID.c_str()));
  else aSO = new SALOMEDS_SObject(_corba_impl->CreateObjectID((char*)anObjectID.c_str())); 
  return _PTR(SObject)(aSO);
}
 
_PTR(SObject) SALOMEDS_Study::FindObjectIOR(const std::string& anObjectIOR)
{
  SALOMEDSClient_SObject* aSO = NULL;
  if(_isLocal) {
    Handle(SALOMEDSImpl_SObject) aSO_impl = _local_impl->FindObjectIOR((char*)anObjectIOR.c_str());
    if(aSO_impl.IsNull()) return _PTR(SObject)(aSO);
    aSO = new SALOMEDS_SObject(aSO_impl);
  }
  else { 
    SALOMEDS::SObject_var aSO_impl = _corba_impl->FindObjectIOR((char*)anObjectIOR.c_str());
    if(CORBA::is_nil(aSO_impl)) return _PTR(SObject)(aSO);
    aSO = new SALOMEDS_SObject(aSO_impl);
  }
  return _PTR(SObject)(aSO);
}

_PTR(SObject) SALOMEDS_Study::FindObjectByPath(const std::string& thePath)
{
  SALOMEDSClient_SObject* aSO = NULL;
  if(_isLocal) {
    Handle(SALOMEDSImpl_SObject) aSO_impl = _local_impl->FindObjectByPath((char*)thePath.c_str());
    if(aSO_impl.IsNull()) return _PTR(SObject)(aSO);
    aSO = new SALOMEDS_SObject(aSO_impl);
  }
  else {
    SALOMEDS::SObject_var aSO_impl = _corba_impl->FindObjectByPath((char*)thePath.c_str());
    if(CORBA::is_nil(aSO_impl)) return _PTR(SObject)(aSO);
    aSO = new SALOMEDS_SObject(aSO_impl);
  }
  return _PTR(SObject)(aSO);
}

std::string SALOMEDS_Study::GetObjectPath(const _PTR(SObject)& theSO)
{
  SALOMEDS_SObject* aSO = dynamic_cast<SALOMEDS_SObject*>(theSO.get());
  std::string aPath;
  if(_isLocal) aPath = _local_impl->GetObjectPath(aSO->GetLocalImpl()).ToCString();
  else aPath = _corba_impl->GetObjectPath(aSO->GetCORBAImpl());
  return aPath;
}

void SALOMEDS_Study::SetContext(const std::string& thePath)
{
  if(_isLocal) _local_impl->SetContext((char*)thePath.c_str());
  else _corba_impl->SetContext((char*)thePath.c_str());
}

std::string SALOMEDS_Study::GetContext()  
{
  std::string aPath;
  if(_isLocal) aPath = _local_impl->GetContext().ToCString();
  else aPath = _corba_impl->GetContext();
  return aPath;
}

std::vector<std::string> SALOMEDS_Study::GetObjectNames(const std::string& theContext)
{
  std::vector<std::string> aVector;
  int aLength, i;
  if(_isLocal) {
    Handle(TColStd_HSequenceOfAsciiString) aSeq = _local_impl->GetObjectNames((char*)theContext.c_str());
    aLength = aSeq->Length();
    for(i = 1; i<=aLength; i++) aVector.push_back(aSeq->Value(i).ToCString());
  }
  else {
    SALOMEDS::ListOfStrings_var aSeq = _corba_impl->GetObjectNames((char*)theContext.c_str());
    aLength = aSeq->length();
    for(i = 0; i<aLength; i++) aVector.push_back(std::string((std::string)aSeq[i].in()));
  }
  return aVector;
}
 
std::vector<std::string> SALOMEDS_Study::GetDirectoryNames(const std::string& theContext)
{
  std::vector<std::string> aVector;
  int aLength, i;
  if(_isLocal) {
    Handle(TColStd_HSequenceOfAsciiString) aSeq = _local_impl->GetDirectoryNames((char*)theContext.c_str());
    aLength = aSeq->Length();
    for(i = 1; i<=aLength; i++) aVector.push_back(aSeq->Value(i).ToCString());
  }
  else {
    SALOMEDS::ListOfStrings_var aSeq = _corba_impl->GetDirectoryNames((char*)theContext.c_str());
    aLength = aSeq->length();
    for(i = 0; i<aLength; i++) aVector.push_back((char*)aSeq[i].in());
  }
  return aVector;
}
 
std::vector<std::string> SALOMEDS_Study::GetFileNames(const std::string& theContext)
{
  std::vector<std::string> aVector;
  int aLength, i;
  if(_isLocal) {
    Handle(TColStd_HSequenceOfAsciiString) aSeq = _local_impl->GetFileNames((char*)theContext.c_str());
    aLength = aSeq->Length();
    for(i = 1; i<=aLength; i++) aVector.push_back(aSeq->Value(i).ToCString());
  }
  else {
    SALOMEDS::ListOfStrings_var aSeq = _corba_impl->GetFileNames((char*)theContext.c_str());
    aLength = aSeq->length();

    for(i = 0; i<aLength; i++) aVector.push_back((char*)aSeq[i].in());
  }
  return aVector;
}
 
std::vector<std::string> SALOMEDS_Study::GetComponentNames(const std::string& theContext)
{
  std::vector<std::string> aVector;
  int aLength, i;
  if(_isLocal) {
    Handle(TColStd_HSequenceOfAsciiString) aSeq = _local_impl->GetComponentNames((char*)theContext.c_str());
    aLength = aSeq->Length();
    for(i = 1; i<=aLength; i++) aVector.push_back(aSeq->Value(i).ToCString());
  }
  else {
    SALOMEDS::ListOfStrings_var aSeq = _corba_impl->GetComponentNames((char*)theContext.c_str());
    aLength = aSeq->length();
    for(i = 0; i<aLength; i++) aVector.push_back((char*)aSeq[i].in());
  }
  return aVector;
}

_PTR(ChildIterator) SALOMEDS_Study::NewChildIterator(const _PTR(SObject)& theSO)
{
  SALOMEDS_SObject* aSO = dynamic_cast<SALOMEDS_SObject*>(theSO.get());
  SALOMEDSClient_ChildIterator* aCI = NULL; 
  if(_isLocal) {
    Handle(SALOMEDSImpl_ChildIterator) aCIimpl = _local_impl->NewChildIterator(aSO->GetLocalImpl());
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
  if(_isLocal) {
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
  if(_isLocal) {
    Handle(SALOMEDSImpl_StudyBuilder) aSBimpl = _local_impl->NewBuilder();
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
  if(_isLocal) aName = _local_impl->Name().ToCString();
  else aName = _corba_impl->Name();
  return aName;
}
 
void SALOMEDS_Study::Name(const std::string& theName)
{
  if(_isLocal) _local_impl->Name((char*)theName.c_str());
  else _corba_impl->Name((char*)theName.c_str());
}

bool SALOMEDS_Study::IsSaved()
{
  bool isSaved;
  if(_isLocal) isSaved = _local_impl->IsSaved();
  else isSaved = _corba_impl->IsSaved();
  return isSaved;
}

void SALOMEDS_Study::IsSaved(bool save)
{
  if(_isLocal) _local_impl->IsSaved(save);
  else _corba_impl->IsSaved(save);
}

bool SALOMEDS_Study::IsModified()
{
  bool isModified;
  if(_isLocal) isModified = _local_impl->IsModified();
  else isModified = _corba_impl->IsModified();
  return isModified;
}
 
std::string SALOMEDS_Study::URL()
{
  std::string aURL;
  if(_isLocal) aURL = _local_impl->URL().ToCString();
  else aURL = _corba_impl->URL();
  return aURL;
}

void SALOMEDS_Study::URL(const std::string& url)
{
  if(_isLocal) _local_impl->URL((char*)url.c_str());
  else _corba_impl->URL((char*)url.c_str());
}

int SALOMEDS_Study::StudyId()
{
  int anID;
  if(_isLocal) anID = _local_impl->StudyId();
  else anID = _corba_impl->StudyId();
  return anID;
}
 
void SALOMEDS_Study::StudyId(int id) 
{
  if(_isLocal) _local_impl->StudyId(id);
  else _corba_impl->StudyId(id);  
}

std::vector<_PTR(SObject)> SALOMEDS_Study::FindDependances(const _PTR(SObject)& theSO)
{
  std::vector<_PTR(SObject)> aVector;
  SALOMEDS_SObject* aSO = dynamic_cast<SALOMEDS_SObject*>(theSO.get());
  int aLength, i;
  if(_isLocal) {
    Handle(TColStd_HSequenceOfTransient) aSeq = _local_impl->FindDependances(aSO->GetLocalImpl());
    if ( !aSeq.IsNull() )
    {
      aLength = aSeq->Length();
      for(i=1; i<=aLength; i++) 
        aVector.push_back(_PTR(SObject)(
          new SALOMEDS_SObject(Handle(SALOMEDSImpl_SObject)::DownCast(aSeq->Value(i)))));
    }
  }
  else {
    SALOMEDS::Study::ListOfSObject_var aSeq = _corba_impl->FindDependances(aSO->GetCORBAImpl());
    aLength = aSeq->length();
    for(i=0; i<aLength; i++) aVector.push_back(_PTR(SObject)(new SALOMEDS_SObject(aSeq[i])));
  }
  return aVector;
}
 
_PTR(AttributeStudyProperties) SALOMEDS_Study::GetProperties()
{
  SALOMEDSClient_AttributeStudyProperties* aProp;
  if(_isLocal) aProp = new SALOMEDS_AttributeStudyProperties(_local_impl->GetProperties());
  else aProp = new SALOMEDS_AttributeStudyProperties(_corba_impl->GetProperties());
  return _PTR(AttributeStudyProperties)(aProp);
}
 
std::string SALOMEDS_Study::GetLastModificationDate() 
{
  std::string aDate;
  if(_isLocal) aDate = _local_impl->GetLastModificationDate().ToCString();
  else aDate = _corba_impl->GetLastModificationDate();
  return aDate;
}

std::vector<std::string> SALOMEDS_Study::GetModificationsDate()
{
  std::vector<std::string> aVector;
  int aLength, i;
  if(_isLocal) {
    Handle(TColStd_HSequenceOfAsciiString) aSeq = _local_impl->GetModificationsDate();
    aLength = aSeq->Length();
    for(i=1; i<=aLength; i++) aVector.push_back(aSeq->Value(i).ToCString());
  }
  else {
    SALOMEDS::ListOfDates_var aSeq = _corba_impl->GetModificationsDate();
    aLength = aSeq->length();
    for(i=0; i<aLength; i++) aVector.push_back((char*)aSeq[i].in());
  }
  return aVector;
}

_PTR(UseCaseBuilder) SALOMEDS_Study::GetUseCaseBuilder()
{
  SALOMEDSClient_UseCaseBuilder* aUB = NULL;
  if(_isLocal) {
    Handle(SALOMEDSImpl_UseCaseBuilder) aUBimpl = _local_impl->GetUseCaseBuilder();
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
  if(_isLocal) _local_impl->Close();
  else _corba_impl->Close();
}

void SALOMEDS_Study::EnableUseCaseAutoFilling(bool isEnabled)
{
  if(_isLocal) _local_impl->EnableUseCaseAutoFilling(isEnabled);
  else _corba_impl->EnableUseCaseAutoFilling(isEnabled);
}

bool SALOMEDS_Study::DumpStudy(const std::string& thePath, const std::string& theBaseName, bool isPublished)
{
  bool ret;
  if(_isLocal) {
    SALOMEDS_DriverFactory_i* aFactory = new SALOMEDS_DriverFactory_i(_orb);
    ret = _local_impl->DumpStudy((char*)thePath.c_str(), (char*)theBaseName.c_str(), isPublished, aFactory);
    delete aFactory;
  }
  else ret = _corba_impl->DumpStudy((char*)thePath.c_str(), (char*)theBaseName.c_str(), isPublished);
  return ret;
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
  if(_isLocal) {
    if(!CORBA::is_nil(_corba_impl)) return _corba_impl;
    std::string anIOR = _local_impl->GetTransientReference().ToCString();
    SALOMEDS::Study_var aStudy;
    if(!_local_impl->IsError() && anIOR != "") {
      aStudy = SALOMEDS::Study::_narrow(_orb->string_to_object(anIOR.c_str()));
    }
    else {
      SALOMEDS_Study_i *aStudy_servant = new SALOMEDS_Study_i(_local_impl, _orb);
      aStudy = aStudy_servant->_this();
      _local_impl->SetTransientReference(_orb->object_to_string(aStudy));
    }
    return aStudy._retn();
  }
  else {
    return _corba_impl;
  }
   
  return SALOMEDS::Study::_nil();
}
