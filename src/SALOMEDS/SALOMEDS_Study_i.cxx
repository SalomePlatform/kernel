//  File   : SALOMEDS_Study_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "utilities.h"
#include "SALOMEDS_Study_i.hxx"
#include "SALOMEDS_UseCaseIterator_i.hxx"
#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDS_AttributeStudyProperties_i.hxx"
#include "SALOMEDS_ChildIterator_i.hxx"
#include "SALOMEDS_Driver_i.hxx"
#include "SALOMEDS.hxx"

#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_SComponent.hxx"
#include "SALOMEDSImpl_UseCaseBuilder.hxx"
#include "SALOMEDSImpl_AttributeStudyProperties.hxx"
#include "SALOMEDSImpl_ChildIterator.hxx"

#include <TColStd_SequenceOfExtendedString.hxx>
#include <TColStd_HSequenceOfAsciiString.hxx>
#include <TColStd_HSequenceOfTransient.hxx>
#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>

#include <TDF_Label.hxx>
#include <TDF_Attribute.hxx>

#ifdef WIN32
#include <process.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif

#include "OpUtil.hxx"

//============================================================================
/*! Function : SALOMEDS_Study_i
 *  Purpose  : SALOMEDS_Study_i constructor
 */
//============================================================================
SALOMEDS_Study_i::SALOMEDS_Study_i(const Handle(SALOMEDSImpl_Study) theImpl,
				   CORBA::ORB_ptr orb)
{
  _orb = CORBA::ORB::_duplicate(orb);
  _impl = theImpl;

  _builder = new SALOMEDS_StudyBuilder_i(_impl->NewBuilder(), _orb);  
}
  
//============================================================================
/*! Function : ~SALOMEDS_Study_i
 *  Purpose  : SALOMEDS_Study_i destructor
 */
//============================================================================
SALOMEDS_Study_i::~SALOMEDS_Study_i()
{
}  

//============================================================================
/*! Function : GetPersistentReference
 *  Purpose  : Get persistent reference of study (idem URL())
 */
//============================================================================
char* SALOMEDS_Study_i::GetPersistentReference()
{
  SALOMEDS::Locker lock; 
  return CORBA::string_dup(_impl->GetPersistentReference().ToCString());
}
//============================================================================
/*! Function : GetTransientReference
 *  Purpose  : Get IOR of the Study (registred in OCAF document in doc->Root)
 */
//============================================================================
char* SALOMEDS_Study_i::GetTransientReference()
{
  SALOMEDS::Locker lock; 
  return CORBA::string_dup(_impl->GetTransientReference().ToCString()); 
}

//============================================================================
/*! Function : IsEmpty
 *  Purpose  : Detect if study is empty
 */
//============================================================================
CORBA::Boolean SALOMEDS_Study_i::IsEmpty()
{
  SALOMEDS::Locker lock; 
  return _impl->IsEmpty();
}

//============================================================================
/*! Function : FindComponent
 *  Purpose  : Find a Component with ComponentDataType = aComponentName
 */
//============================================================================
SALOMEDS::SComponent_ptr SALOMEDS_Study_i::FindComponent (const char* aComponentName)
{
  SALOMEDS::Locker lock; 
  
  Handle(SALOMEDSImpl_SComponent) aCompImpl = _impl->FindComponent(TCollection_AsciiString((char*)aComponentName));
  if(aCompImpl.IsNull()) return SALOMEDS::SComponent::_nil();

  SALOMEDS::SComponent_var sco = SALOMEDS_SComponent_i::New (aCompImpl, _orb);
  return sco._retn();
}

//============================================================================
/*! Function : FindComponentID
 *  Purpose  : Find a Component from it's ID
 */
//============================================================================
SALOMEDS::SComponent_ptr SALOMEDS_Study_i::FindComponentID(const char* aComponentID)
{
  SALOMEDS::Locker lock; 
  
  Handle(SALOMEDSImpl_SComponent) aCompImpl = _impl->FindComponentID(TCollection_AsciiString((char*)aComponentID));
  if(aCompImpl.IsNull()) return SALOMEDS::SComponent::_nil();

  SALOMEDS::SComponent_var sco = SALOMEDS_SComponent_i::New (aCompImpl, _orb);
  return sco._retn();
}

//============================================================================
/*! Function : FindObject
 *  Purpose  : Find an Object with SALOMEDS::Name = anObjectName
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_Study_i::FindObject(const char* anObjectName)
{
  SALOMEDS::Locker lock; 

  Handle(SALOMEDSImpl_SObject) aSO = _impl->FindObject(TCollection_AsciiString((char*)anObjectName));
  if(aSO.IsNull()) return SALOMEDS::SObject::_nil();

  if(aSO->DynamicType() == STANDARD_TYPE(SALOMEDSImpl_SComponent)) {
    Handle(SALOMEDSImpl_SComponent) aSCO = Handle(SALOMEDSImpl_SComponent)::DownCast(aSO);
    SALOMEDS::SComponent_var sco = SALOMEDS_SComponent_i::New (aSCO, _orb);
    return sco._retn();
  }
   
  SALOMEDS::SObject_var so = SALOMEDS_SObject_i::New (aSO, _orb);
 
 return so._retn();
}

//============================================================================
/*! Function : FindObjectID
 *  Purpose  : Find an Object with ID = anObjectID
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_Study_i::FindObjectID(const char* anObjectID)
{
  SALOMEDS::Locker lock; 

  Handle(SALOMEDSImpl_SObject) aSO = _impl->FindObjectID(TCollection_AsciiString((char*)anObjectID));
  if(aSO.IsNull()) return SALOMEDS::SObject::_nil();
  SALOMEDS::SObject_var so = SALOMEDS_SObject_i::New (aSO, _orb);
  return so._retn();
}

//============================================================================
/*! Function : CreateObjectID
 *  Purpose  : Creates an Object with ID = anObjectID
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_Study_i::CreateObjectID(const char* anObjectID)
{
  SALOMEDS::Locker lock; 

  Handle(SALOMEDSImpl_SObject) aSO = _impl->CreateObjectID((char*)anObjectID);
  if(aSO.IsNull()) return SALOMEDS::SObject::_nil();

  SALOMEDS::SObject_var so = SALOMEDS_SObject_i::New (aSO, _orb);
  return so._retn();
}

//============================================================================
/*! Function : FindObjectByName
 *  Purpose  : Find Objects with SALOMEDS::Name = anObjectName in a Component
 *           : with ComponentDataType = aComponentName
 */
//============================================================================
SALOMEDS::Study::ListOfSObject* SALOMEDS_Study_i::FindObjectByName( const char* anObjectName,
								    const char* aComponentName )
{
  SALOMEDS::Locker lock; 

  Handle(TColStd_HSequenceOfTransient) aSeq = _impl->FindObjectByName(TCollection_AsciiString((char*)anObjectName),
								      TCollection_AsciiString((char*)aComponentName));
  int aLength = aSeq->Length();
  SALOMEDS::Study::ListOfSObject_var listSO = new SALOMEDS::Study::ListOfSObject ;
  listSO->length(aLength);
  for(int i = 1; i<=aLength; i++) {
    Handle(SALOMEDSImpl_SObject) aSO = Handle(SALOMEDSImpl_SObject)::DownCast(aSeq->Value(i));
    SALOMEDS::SObject_var so = SALOMEDS_SObject_i::New (aSO, _orb);
    listSO[i-1] = so ;
  }
  return listSO._retn() ;
}

//============================================================================
/*! Function : FindObjectIOR
 *  Purpose  : Find an Object with IOR = anObjectIOR
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_Study_i::FindObjectIOR(const char* anObjectIOR)
{
  SALOMEDS::Locker lock; 

  Handle(SALOMEDSImpl_SObject) aSO = _impl->FindObjectIOR(TCollection_AsciiString((char*)anObjectIOR));
  if(aSO.IsNull()) return SALOMEDS::SObject::_nil();

  SALOMEDS::SObject_var so = SALOMEDS_SObject_i::New (aSO, _orb);
  return so._retn();
}

//============================================================================
/*! Function : FindObjectByPath
 *  Purpose  : Find an Object by its path = thePath
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_Study_i::FindObjectByPath(const char* thePath)
{
  SALOMEDS::Locker lock; 

  Handle(SALOMEDSImpl_SObject) aSO = _impl->FindObjectByPath(TCollection_AsciiString((char*)thePath));
  if(aSO.IsNull()) return SALOMEDS::SObject::_nil();

  SALOMEDS::SObject_var so = SALOMEDS_SObject_i::New (aSO, _orb);
  return so._retn();
}

//============================================================================
/*! Function : GetObjectPath
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_Study_i::GetObjectPath(CORBA::Object_ptr theObject)
{
  SALOMEDS::Locker lock; 

  TCollection_AsciiString aPath("");
  if(CORBA::is_nil(theObject)) return CORBA::string_dup(aPath.ToCString());
  Handle(SALOMEDSImpl_SObject) aSO;
  SALOMEDS::SObject_var aSObj = SALOMEDS::SObject::_narrow(theObject);

  if(!CORBA::is_nil(aSObj)) {
    aSO = _impl->FindObjectID(aSObj->GetID());
  }
  else {
    aSO  = _impl->FindObjectIOR(_orb->object_to_string(theObject));
  }
   
  if(aSO.IsNull()) return CORBA::string_dup(aPath.ToCString());
  
  aPath = _impl->GetObjectPath(aSO);
  return  CORBA::string_dup(aPath.ToCString());
}


//============================================================================
/*! Function : SetContext
 *  Purpose  : Sets the current context
 */
//============================================================================
void SALOMEDS_Study_i::SetContext(const char* thePath) 
{
  SALOMEDS::Locker lock; 

  _impl->SetContext(TCollection_AsciiString((char*)thePath));
  if(_impl->IsError() && _impl->GetErrorCode() == "InvalidContext") 
    throw SALOMEDS::Study::StudyInvalidContext();  
}

//============================================================================
/*! Function : GetContext
 *  Purpose  : Gets the current context
 */
//============================================================================
char* SALOMEDS_Study_i::GetContext() 
{
  SALOMEDS::Locker lock; 

  if(!_impl->HasCurrentContext()) throw SALOMEDS::Study::StudyInvalidContext();   
  return CORBA::string_dup(_impl->GetContext().ToCString());
}

//============================================================================
/*! Function : GetObjectNames
 *  Purpose  : method to get all object names in the given context (or in the current context, if 'theContext' is empty)
 */
//============================================================================
SALOMEDS::ListOfStrings* SALOMEDS_Study_i::GetObjectNames(const char* theContext) 
{
  SALOMEDS::Locker lock; 

  SALOMEDS::ListOfStrings_var aResult = new SALOMEDS::ListOfStrings;
  if (strlen(theContext) == 0 && !_impl->HasCurrentContext()) throw SALOMEDS::Study::StudyInvalidContext();   
  Handle(TColStd_HSequenceOfAsciiString) aSeq = _impl->GetObjectNames(TCollection_AsciiString((char*)theContext));
  int aLength = aSeq->Length();
  aResult->length(aLength);
  for(int anIndex = 1; anIndex <= aLength; anIndex++) {
    aResult[anIndex-1] = CORBA::string_dup(TCollection_AsciiString(aSeq->Value(anIndex)).ToCString());
  }
  return aResult._retn();
}

//============================================================================
/*! Function : GetDirectoryNames
 *  Purpose  : method to get all directory names in the given context (or in the current context, if 'theContext' is empty)
 */
//============================================================================
SALOMEDS::ListOfStrings* SALOMEDS_Study_i::GetDirectoryNames(const char* theContext) 
{
  SALOMEDS::Locker lock; 

  SALOMEDS::ListOfStrings_var aResult = new SALOMEDS::ListOfStrings;
  if (strlen(theContext) == 0 && !_impl->HasCurrentContext()) throw SALOMEDS::Study::StudyInvalidContext();   
  Handle(TColStd_HSequenceOfAsciiString) aSeq = _impl->GetDirectoryNames(TCollection_AsciiString((char*)theContext));
  int aLength = aSeq->Length();
  aResult->length(aLength);
  for(int anIndex = 1; anIndex <= aLength; anIndex++) {
    aResult[anIndex-1] = CORBA::string_dup(TCollection_AsciiString(aSeq->Value(anIndex)).ToCString());
  }
  return aResult._retn();
}

//============================================================================
/*! Function : GetFileNames
 *  Purpose  : method to get all file names in the given context (or in the current context, if 'theContext' is empty)
 */
//============================================================================
SALOMEDS::ListOfStrings* SALOMEDS_Study_i::GetFileNames(const char* theContext) 
{
  SALOMEDS::Locker lock; 

  SALOMEDS::ListOfStrings_var aResult = new SALOMEDS::ListOfStrings;
  if (strlen(theContext) == 0 && !_impl->HasCurrentContext()) throw SALOMEDS::Study::StudyInvalidContext();   
  Handle(TColStd_HSequenceOfAsciiString) aSeq = _impl->GetFileNames(TCollection_AsciiString((char*)theContext));
  int aLength = aSeq->Length();
  aResult->length(aLength);
  for(int anIndex = 1; anIndex <= aLength; anIndex++) {
    aResult[anIndex-1] = CORBA::string_dup(TCollection_AsciiString(aSeq->Value(anIndex)).ToCString());
  }
  return aResult._retn();
}

//============================================================================
/*! Function : GetComponentNames
 *  Purpose  : method to get all components names
 */
//============================================================================
SALOMEDS::ListOfStrings* SALOMEDS_Study_i::GetComponentNames(const char* theContext) 
{
  SALOMEDS::Locker lock; 

  SALOMEDS::ListOfStrings_var aResult = new SALOMEDS::ListOfStrings;
  if (strlen(theContext) == 0 && !_impl->HasCurrentContext()) throw SALOMEDS::Study::StudyInvalidContext();   
  Handle(TColStd_HSequenceOfAsciiString) aSeq = _impl->GetComponentNames(TCollection_AsciiString((char*)theContext));
  int aLength = aSeq->Length();
  aResult->length(aLength);
  for(int anIndex = 1; anIndex <= aLength; anIndex++) {
    aResult[anIndex-1] = CORBA::string_dup(TCollection_AsciiString(aSeq->Value(anIndex)).ToCString());
  }
  return aResult._retn();
}

//============================================================================
/*! Function : NewChildIterator
 *  Purpose  : Create a ChildIterator from an SObject
 */
//============================================================================
SALOMEDS::ChildIterator_ptr SALOMEDS_Study_i::NewChildIterator(SALOMEDS::SObject_ptr theSO)
{
  SALOMEDS::Locker lock; 

  Handle(SALOMEDSImpl_SObject) aSO = _impl->GetSObject(theSO->GetID());
  Handle(SALOMEDSImpl_ChildIterator) anItr = new SALOMEDSImpl_ChildIterator(aSO);

  //Create iterator
  SALOMEDS_ChildIterator_i* it_servant = new SALOMEDS_ChildIterator_i(anItr, _orb);
  SALOMEDS::ChildIterator_var it = SALOMEDS::ChildIterator::_narrow(it_servant->_this()); 

  return it;
}


//============================================================================
/*! Function : NewComponentIterator
 *  Purpose  : Create a SComponentIterator
 */
//============================================================================
SALOMEDS::SComponentIterator_ptr SALOMEDS_Study_i::NewComponentIterator()
{
  SALOMEDS::Locker lock; 
  SALOMEDS_SComponentIterator_i* _it = new SALOMEDS_SComponentIterator_i(_impl->NewComponentIterator(), _orb);
  _it->Init();
  return _it->_this();
}


//============================================================================
/*! Function : NewBuilder
 *  Purpose  : Create a StudyBuilder
 */
//============================================================================
SALOMEDS::StudyBuilder_ptr SALOMEDS_Study_i::NewBuilder()
{
  SALOMEDS::Locker lock; 
  return _builder->_this();
}
 
//============================================================================
/*! Function : Name
 *  Purpose  : get study name
 */
//============================================================================
char* SALOMEDS_Study_i::Name()
{
  SALOMEDS::Locker lock; 
  return CORBA::string_dup(_impl->Name().ToCString());
}

//============================================================================
/*! Function : Name
 *  Purpose  : set study name
 */
//============================================================================
void SALOMEDS_Study_i::Name(const char* name)
{
  SALOMEDS::Locker lock;  
  _impl->Name(TCollection_AsciiString((char*)name));
}

//============================================================================
/*! Function : IsSaved
 *  Purpose  : get if study has been saved
 */
//============================================================================
CORBA::Boolean  SALOMEDS_Study_i::IsSaved()
{
  SALOMEDS::Locker lock; 
  return _impl->IsSaved();
}

//============================================================================
/*! Function : IsSaved
 *  Purpose  : set if study has been saved
 */
//============================================================================
void SALOMEDS_Study_i::IsSaved(CORBA::Boolean save)
{
  SALOMEDS::Locker lock; 
  _impl->IsSaved(save);
}

//============================================================================
/*! Function : IsModified
 *  Purpose  : Detect if a Study has been modified since it has been saved
 */
//============================================================================
CORBA::Boolean  SALOMEDS_Study_i::IsModified()
{
  SALOMEDS::Locker lock; 
  return _impl->IsModified();
}

//============================================================================
/*! Function : URL
 *  Purpose  : get URL of the study (persistent reference of the study)
 */
//============================================================================
char* SALOMEDS_Study_i::URL()
{
  SALOMEDS::Locker lock; 
  return CORBA::string_dup(_impl->URL().ToCString());
}

//============================================================================
/*! Function : URL
 *  Purpose  : set URL of the study (persistent reference of the study)
 */
//============================================================================
void SALOMEDS_Study_i::URL(const char* url)
{
  SALOMEDS::Locker lock; 
  _impl->URL(TCollection_AsciiString((char*)url));
}


CORBA::Short SALOMEDS_Study_i::StudyId()
{
  SALOMEDS::Locker lock; 
  return _impl->StudyId();
}

void SALOMEDS_Study_i::StudyId(CORBA::Short id)
{ 
  SALOMEDS::Locker lock; 
  _impl->StudyId(id);
}

void SALOMEDS_Study_i::UpdateIORLabelMap(const char* anIOR,const char* anEntry) 
{
  SALOMEDS::Locker lock; 
  _impl->UpdateIORLabelMap(TCollection_AsciiString((char*)anIOR), TCollection_AsciiString((char*)anEntry));
}

SALOMEDS::Study_ptr SALOMEDS_Study_i::GetStudy(const TDF_Label theLabel, CORBA::ORB_ptr orb) 
{
  SALOMEDS::Locker lock; 

  Handle(SALOMEDSImpl_AttributeIOR) Att;
  if (theLabel.Root().FindAttribute(SALOMEDSImpl_AttributeIOR::GetID(),Att)){
    char* IOR = CORBA::string_dup(TCollection_AsciiString(Att->Value()).ToCString());
    CORBA::Object_var obj = orb->string_to_object(IOR);
    SALOMEDS::Study_ptr aStudy = SALOMEDS::Study::_narrow(obj) ;
    ASSERT(!CORBA::is_nil(aStudy));
    return SALOMEDS::Study::_duplicate(aStudy);
  } else {
    MESSAGE("GetStudy: Problem to get study");
  }
  return SALOMEDS::Study::_nil();
}

void SALOMEDS_Study_i::IORUpdated(const Handle(SALOMEDSImpl_AttributeIOR) theAttribute) 
{
  SALOMEDS::Locker lock; 
  SALOMEDSImpl_Study::IORUpdated(theAttribute);
}

SALOMEDS::Study::ListOfSObject* SALOMEDS_Study_i::FindDependances(SALOMEDS::SObject_ptr anObject) 
{
  SALOMEDS::Locker lock; 

  SALOMEDS::GenericAttribute_ptr aTarget;
  if (anObject->FindAttribute(aTarget,"AttributeTarget")) {
    return SALOMEDS::AttributeTarget::_narrow(aTarget)->Get();
  }
  SALOMEDS::Study::ListOfSObject* aList = new SALOMEDS::Study::ListOfSObject;
  aList->length(0);
  return aList;
}


SALOMEDS::AttributeStudyProperties_ptr SALOMEDS_Study_i::GetProperties() 
{
  SALOMEDS::Locker lock; 
  
  Handle(SALOMEDSImpl_AttributeStudyProperties) anAttr = _impl->GetProperties();
  SALOMEDS_AttributeStudyProperties_i* SP = new SALOMEDS_AttributeStudyProperties_i(anAttr, _orb);
  return SP->AttributeStudyProperties::_this();
}

char* SALOMEDS_Study_i::GetLastModificationDate() 
{
  SALOMEDS::Locker lock; 
  return CORBA::string_dup(_impl->GetLastModificationDate().ToCString());
}

SALOMEDS::ListOfDates* SALOMEDS_Study_i::GetModificationsDate() 
{
  SALOMEDS::Locker lock; 
  
  Handle(TColStd_HSequenceOfAsciiString) aSeq = _impl->GetModificationsDate();
  int aLength = aSeq->Length();
  SALOMEDS::ListOfDates_var aDates = new SALOMEDS::ListOfDates;
  aDates->length(aLength);

  for(int anIndex = 1; anIndex <= aLength; anIndex++) {
    aDates[anIndex-1] = CORBA::string_dup(aSeq->Value(anIndex).ToCString());
  }
  return aDates._retn();
}



//============================================================================
/*! Function : GetUseCaseBuilder
 *  Purpose  : Returns a UseCase builder
 */
//============================================================================
SALOMEDS::UseCaseBuilder_ptr SALOMEDS_Study_i::GetUseCaseBuilder() 
{
  SALOMEDS::Locker lock; 
  SALOMEDS_UseCaseBuilder_i* UCBuilder = new SALOMEDS_UseCaseBuilder_i(_impl->GetUseCaseBuilder(), _orb);
  return UCBuilder->_this();
}


//============================================================================
/*! Function : Close
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_Study_i::Close()
{
  SALOMEDS::Locker lock; 
  
  RemovePostponed(-1);

  SALOMEDS::SComponentIterator_var itcomponent = NewComponentIterator();

  for (; itcomponent->More(); itcomponent->Next()) {
    SALOMEDS::SComponent_var sco = itcomponent->Value();
	  
    MESSAGE ( "Look for an engine for data type :"<< sco->ComponentDataType());
    // if there is an associated Engine call its method for closing
    CORBA::String_var IOREngine;
    if (sco->ComponentIOR(IOREngine)) {
      // we have found the associated engine to write the data 
      MESSAGE ( "We have found an engine for data type :"<< sco->ComponentDataType());
      CORBA::Object_var obj = _orb->string_to_object(IOREngine);
      if (!CORBA::is_nil(obj)) {
	SALOMEDS::Driver_var anEngine = SALOMEDS::Driver::_narrow(obj) ;

	if (!anEngine->_is_nil()) { 
	  SALOMEDS::unlock();
	  anEngine->Close(sco);
	  SALOMEDS::lock();
	}
      }
    }
  }
  _impl->Close();
}

//============================================================================
/*! Function : AddPostponed
 *  Purpose  : 
 */
 //============================================================================
void SALOMEDS_Study_i::AddPostponed(const char* theIOR) 
{
  SALOMEDS::Locker lock; 
 
  CORBA::Object_var obj = _orb->string_to_object(theIOR);
  if (!CORBA::is_nil(obj)) {
    SALOME::GenericObj_var aGeneric = SALOME::GenericObj::_narrow(obj) ;
    if (!CORBA::is_nil(aGeneric)) _impl->AddPostponed((char*)theIOR);     
  }
}

void SALOMEDS_Study_i::AddCreatedPostponed(const char* theIOR) 
{
  SALOMEDS::Locker lock; 

  CORBA::Object_var obj = _orb->string_to_object(theIOR);
  if (!CORBA::is_nil(obj)) {
    SALOME::GenericObj_var aGeneric = SALOME::GenericObj::_narrow(obj) ;
    if (!CORBA::is_nil(aGeneric)) _impl->AddCreatedPostponed((char*)theIOR);
  }
}

//============================================================================
/*! Function : RemovePostponed
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_Study_i::RemovePostponed(const CORBA::Long theUndoLimit) 
{  
  SALOMEDS::Locker lock; 

  Handle(TColStd_HSequenceOfAsciiString) aSeq = _impl->RemovePostponed(theUndoLimit);
  int aLegth = aSeq->Length();
  for(int i = 1; i <= aLegth; i++) {
    TCollection_AsciiString anIOR = aSeq->Value(i);
    if (anIOR.Value(1) == 'c') {
      CORBA::Object_var obj = _orb->string_to_object(anIOR.Split(1).ToCString());
      SALOME::GenericObj_var aGeneric = SALOME::GenericObj::_narrow(obj);
      if (!CORBA::is_nil(aGeneric)) aGeneric->Destroy();
    }
    else if (anIOR.Value(1) == 'd') {
      CORBA::Object_var obj = _orb->string_to_object(anIOR.Split(1).ToCString());
      SALOME::GenericObj_var aGeneric = SALOME::GenericObj::_narrow(obj);
      if (!CORBA::is_nil(aGeneric)) aGeneric->Destroy();
    }
    else {
      try {
	CORBA::Object_var obj = _orb->string_to_object(anIOR.ToCString());
	SALOME::GenericObj_var aGeneric = SALOME::GenericObj::_narrow(obj);
	if (!CORBA::is_nil(aGeneric)) aGeneric->Destroy();
      } catch (...) {}
    }
  }
}

//============================================================================
/*! Function : UndoPostponed
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_Study_i::UndoPostponed(const CORBA::Long theWay) 
{
  SALOMEDS::Locker lock; 

  _impl->UndoPostponed(theWay);
}


//============================================================================
/*! Function : DumpStudy
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_Study_i::DumpStudy(const char* thePath, const char* theBaseName, CORBA::Boolean isPublished)
{
  SALOMEDS::Locker lock; 

  TCollection_AsciiString aPath((char*)thePath), aBaseName((char*)theBaseName);
  SALOMEDS_DriverFactory_i* factory = new SALOMEDS_DriverFactory_i(_orb);
  CORBA::Boolean ret = _impl->DumpStudy(aPath, aBaseName, isPublished, factory);
  delete factory;
  return ret;
}

//===========================================================================
//   PRIVATE FUNCTIONS
//===========================================================================
long SALOMEDS_Study_i::GetLocalImpl(const char* theHostname, CORBA::Long thePID, CORBA::Boolean& isLocal)
{
#ifdef WIN32
  long pid = (long)_getpid();
#else
  long pid = (long)getpid();
#endif  
  isLocal = (strcmp(theHostname, GetHostname().c_str()) == 0 && pid == thePID)?1:0;
  SALOMEDSImpl_Study* local_impl = _impl.operator->();
  return ((long)local_impl);
}
