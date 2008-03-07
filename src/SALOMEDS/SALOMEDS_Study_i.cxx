// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
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
//  File   : SALOMEDS_Study_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME


#include "utilities.h"
#include "SALOMEDS_Study_i.hxx"
#include "SALOMEDS_UseCaseIterator_i.hxx"
#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDS_AttributeStudyProperties_i.hxx"
#include "SALOMEDS_AttributeParameter_i.hxx"
#include "SALOMEDS_ChildIterator_i.hxx"
#include "SALOMEDS_Driver_i.hxx"
#include "SALOMEDS.hxx"

#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_SComponent.hxx"
#include "SALOMEDSImpl_UseCaseBuilder.hxx"
#include "SALOMEDSImpl_AttributeStudyProperties.hxx"
#include "SALOMEDSImpl_AttributeParameter.hxx"
#include "SALOMEDSImpl_ChildIterator.hxx"
#include "SALOMEDSImpl_IParameters.hxx"

#include "DF_Label.hxx"
#include "DF_Attribute.hxx"

#ifdef WIN32
#include <process.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif

#include "OpUtil.hxx"

using namespace std;

//============================================================================
/*! Function : SALOMEDS_Study_i
 *  Purpose  : SALOMEDS_Study_i constructor
 */
//============================================================================
SALOMEDS_Study_i::SALOMEDS_Study_i(SALOMEDSImpl_Study* theImpl,
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
  return CORBA::string_dup(_impl->GetPersistentReference().c_str());
}
//============================================================================
/*! Function : GetTransientReference
 *  Purpose  : Get IOR of the Study (registred in OCAF document in doc->Root)
 */
//============================================================================
char* SALOMEDS_Study_i::GetTransientReference()
{
  SALOMEDS::Locker lock; 
  return CORBA::string_dup(_impl->GetTransientReference().c_str()); 
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
  
  SALOMEDSImpl_SComponent aCompImpl = _impl->FindComponent(string(aComponentName));
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
  
  SALOMEDSImpl_SComponent aCompImpl = _impl->FindComponentID(string((char*)aComponentID));
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

  SALOMEDSImpl_SObject aSO = _impl->FindObject(string((char*)anObjectName));
  if(aSO.IsNull()) return SALOMEDS::SObject::_nil();

  if(aSO.IsComponent()) {
    SALOMEDSImpl_SComponent aSCO = aSO;
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

  SALOMEDSImpl_SObject aSO = _impl->FindObjectID(string((char*)anObjectID));
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

  if(!anObjectID || strlen(anObjectID) == 0) return SALOMEDS::SObject::_nil();

  SALOMEDSImpl_SObject aSO = _impl->CreateObjectID((char*)anObjectID);
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

  vector<SALOMEDSImpl_SObject> aSeq = _impl->FindObjectByName(string((char*)anObjectName),
							       string((char*)aComponentName));
  int aLength = aSeq.size();
  SALOMEDS::Study::ListOfSObject_var listSO = new SALOMEDS::Study::ListOfSObject ;
  listSO->length(aLength);
  for(int i = 0; i<aLength; i++) {
    SALOMEDS::SObject_var so = SALOMEDS_SObject_i::New (aSeq[i], _orb);
    listSO[i] = so ;
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

  SALOMEDSImpl_SObject aSO = _impl->FindObjectIOR(string((char*)anObjectIOR));
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

  SALOMEDSImpl_SObject aSO = _impl->FindObjectByPath(string((char*)thePath));
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

  string aPath("");
  if(CORBA::is_nil(theObject)) return CORBA::string_dup(aPath.c_str());
  SALOMEDSImpl_SObject aSO;
  SALOMEDS::SObject_var aSObj = SALOMEDS::SObject::_narrow(theObject);

  if(!CORBA::is_nil(aSObj)) {
    aSO = _impl->FindObjectID(aSObj->GetID());
  }
  else {
    aSO  = _impl->FindObjectIOR(_orb->object_to_string(theObject));
  }
   
  if(aSO.IsNull()) return CORBA::string_dup(aPath.c_str());
  
  aPath = _impl->GetObjectPath(aSO);
  return  CORBA::string_dup(aPath.c_str());
}


//============================================================================
/*! Function : SetContext
 *  Purpose  : Sets the current context
 */
//============================================================================
void SALOMEDS_Study_i::SetContext(const char* thePath) 
{
  SALOMEDS::Locker lock; 

  _impl->SetContext(string((char*)thePath));
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
  return CORBA::string_dup(_impl->GetContext().c_str());
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

  if (strlen(theContext) == 0 && !_impl->HasCurrentContext())
    throw SALOMEDS::Study::StudyInvalidContext();

  vector<string> aSeq = _impl->GetObjectNames(string((char*)theContext));
  if (_impl->GetErrorCode() == "InvalidContext")
    throw SALOMEDS::Study::StudyInvalidContext();

  int aLength = aSeq.size();
  aResult->length(aLength);
  for (int anIndex = 0; anIndex < aLength; anIndex++) {
    aResult[anIndex] = CORBA::string_dup(aSeq[anIndex].c_str());
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

  if (strlen(theContext) == 0 && !_impl->HasCurrentContext())
    throw SALOMEDS::Study::StudyInvalidContext();

  vector<string> aSeq = _impl->GetDirectoryNames(string((char*)theContext));
  if (_impl->GetErrorCode() == "InvalidContext")
    throw SALOMEDS::Study::StudyInvalidContext();

  int aLength = aSeq.size();
  aResult->length(aLength);
  for (int anIndex = 0; anIndex < aLength; anIndex++) {
    aResult[anIndex] = CORBA::string_dup(aSeq[anIndex].c_str());
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

  if (strlen(theContext) == 0 && !_impl->HasCurrentContext())
    throw SALOMEDS::Study::StudyInvalidContext();

  vector<string> aSeq = _impl->GetFileNames(string((char*)theContext));
  if (_impl->GetErrorCode() == "InvalidContext")
    throw SALOMEDS::Study::StudyInvalidContext();

  int aLength = aSeq.size();
  aResult->length(aLength);
  for (int anIndex = 0; anIndex < aLength; anIndex++) {
    aResult[anIndex] = CORBA::string_dup(aSeq[anIndex].c_str());
  }

  return aResult._retn();
}

//============================================================================
/*! Function : GetComponentNames
 *  Purpose  : method to get all components names
 *  SRN:       Note, theContext can be any, it doesn't matter
 */
//============================================================================
SALOMEDS::ListOfStrings* SALOMEDS_Study_i::GetComponentNames(const char* theContext) 
{
  SALOMEDS::Locker lock; 

  SALOMEDS::ListOfStrings_var aResult = new SALOMEDS::ListOfStrings;

  vector<string> aSeq = _impl->GetComponentNames(string((char*)theContext));

  int aLength = aSeq.size();
  aResult->length(aLength);
  for(int anIndex = 0; anIndex < aLength; anIndex++) {
    aResult[anIndex] = CORBA::string_dup(aSeq[anIndex].c_str());
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

  SALOMEDSImpl_SObject aSO = _impl->GetSObject(theSO->GetID());
  SALOMEDSImpl_ChildIterator anItr(aSO);

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
  return CORBA::string_dup(_impl->Name().c_str());
}

//============================================================================
/*! Function : Name
 *  Purpose  : set study name
 */
//============================================================================
void SALOMEDS_Study_i::Name(const char* name)
{
  SALOMEDS::Locker lock;  
  _impl->Name(string((char*)name));
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
/*! Function : Modified
 *  Purpose  : Sets a Modified flag of a Study to True
 */
//============================================================================
void  SALOMEDS_Study_i::Modified()
{
  SALOMEDS::Locker lock; 
  return _impl->Modify();
}


//============================================================================
/*! Function : URL
 *  Purpose  : get URL of the study (persistent reference of the study)
 */
//============================================================================
char* SALOMEDS_Study_i::URL()
{
  SALOMEDS::Locker lock; 
  return CORBA::string_dup(_impl->URL().c_str());
}

//============================================================================
/*! Function : URL
 *  Purpose  : set URL of the study (persistent reference of the study)
 */
//============================================================================
void SALOMEDS_Study_i::URL(const char* url)
{
  SALOMEDS::Locker lock; 
  _impl->URL(string((char*)url));
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
  _impl->UpdateIORLabelMap(string((char*)anIOR), string((char*)anEntry));
}

SALOMEDS::Study_ptr SALOMEDS_Study_i::GetStudy(const DF_Label& theLabel, CORBA::ORB_ptr orb) 
{
  SALOMEDS::Locker lock; 

  SALOMEDSImpl_AttributeIOR* Att = NULL;
  if ((Att=(SALOMEDSImpl_AttributeIOR*)theLabel.Root().FindAttribute(SALOMEDSImpl_AttributeIOR::GetID()))){
    char* IOR = CORBA::string_dup(Att->Value().c_str());
    CORBA::Object_var obj = orb->string_to_object(IOR);
    SALOMEDS::Study_ptr aStudy = SALOMEDS::Study::_narrow(obj) ;
    ASSERT(!CORBA::is_nil(aStudy));
    return SALOMEDS::Study::_duplicate(aStudy);
  } else {
    MESSAGE("GetStudy: Problem to get study");
  }
  return SALOMEDS::Study::_nil();
}

void SALOMEDS_Study_i::IORUpdated(SALOMEDSImpl_AttributeIOR* theAttribute) 
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
  
  SALOMEDSImpl_AttributeStudyProperties* anAttr = _impl->GetProperties();
  SALOMEDS_AttributeStudyProperties_i* SP = new SALOMEDS_AttributeStudyProperties_i(anAttr, _orb);
  return SP->AttributeStudyProperties::_this();
}

char* SALOMEDS_Study_i::GetLastModificationDate() 
{
  SALOMEDS::Locker lock; 
  return CORBA::string_dup(_impl->GetLastModificationDate().c_str());
}

SALOMEDS::ListOfDates* SALOMEDS_Study_i::GetModificationsDate() 
{
  SALOMEDS::Locker lock; 
  
  vector<string> aSeq = _impl->GetModificationsDate();
  int aLength = aSeq.size();
  SALOMEDS::ListOfDates_var aDates = new SALOMEDS::ListOfDates;
  aDates->length(aLength);

  for(int anIndex = 0; anIndex < aLength; anIndex++) {
    aDates[anIndex] = CORBA::string_dup(aSeq[anIndex].c_str());
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
  //Not implemented
}

void SALOMEDS_Study_i::AddCreatedPostponed(const char* theIOR) 
{
  SALOMEDS::Locker lock; 
  //Not implemented
}

//============================================================================
/*! Function : RemovePostponed
 *  Purpose  : 
 */
//============================================================================
#ifndef WNT
void SALOMEDS_Study_i::RemovePostponed(const CORBA::Long /*theUndoLimit*/) 
#else
void SALOMEDS_Study_i::RemovePostponed(CORBA::Long /*theUndoLimit*/) 
#endif
{  
  SALOMEDS::Locker lock; 

  vector<string> anIORs = _impl->GetIORs();
  int i, aSize = (int)anIORs.size();

  for(i = 0; i < aSize; i++) {
    try {
      CORBA::Object_var obj = _orb->string_to_object(anIORs[i].c_str());
      SALOME::GenericObj_var aGeneric = SALOME::GenericObj::_narrow(obj);
      if (!CORBA::is_nil(aGeneric)) aGeneric->Destroy();
    } catch (...) {}
  }

  //Not implemented
}

//============================================================================
/*! Function : UndoPostponed
 *  Purpose  : 
 */
//============================================================================
#ifndef WNT
void SALOMEDS_Study_i::UndoPostponed(const CORBA::Long theWay) 
#else
void SALOMEDS_Study_i::UndoPostponed(CORBA::Long theWay) 
#endif
{
  SALOMEDS::Locker lock; 
  //Not implemented
}


//============================================================================
/*! Function : DumpStudy
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_Study_i::DumpStudy(const char* thePath, 
					   const char* theBaseName, 
					   CORBA::Boolean isPublished)
{
  SALOMEDS::Locker lock; 

  string aPath((char*)thePath), aBaseName((char*)theBaseName);
  SALOMEDS_DriverFactory_i* factory = new SALOMEDS_DriverFactory_i(_orb);
  CORBA::Boolean ret = _impl->DumpStudy(aPath, aBaseName, isPublished, factory);
  delete factory;
  return ret;
}

//============================================================================
/*! Function : GetCommonParameters
 *  Purpose  : 
 */
//============================================================================
SALOMEDS::AttributeParameter_ptr SALOMEDS_Study_i::GetCommonParameters(const char* theID, CORBA::Long theSavePoint)
{
  SALOMEDS::Locker lock; 
  
  SALOMEDSImpl_AttributeParameter* anAttr = _impl->GetCommonParameters(theID, theSavePoint);
  SALOMEDS_AttributeParameter_i* SP = new SALOMEDS_AttributeParameter_i(anAttr, _orb);
  return SP->AttributeParameter::_this();
}
 
//============================================================================
/*! Function : GetCommonModuleParameters
 *  Purpose  : 
 */
//============================================================================
SALOMEDS::AttributeParameter_ptr SALOMEDS_Study_i::GetModuleParameters(const char* theID, 
								       const char* theModuleName, 
								       CORBA::Long theSavePoint)
{
  SALOMEDS::Locker lock; 
  
  SALOMEDSImpl_AttributeParameter* anAttr = _impl->GetModuleParameters(theID, theModuleName, theSavePoint);
  SALOMEDS_AttributeParameter_i* SP = new SALOMEDS_AttributeParameter_i(anAttr, _orb);
  return SP->AttributeParameter::_this();
}

//============================================================================
/*! Function : SetStudyLock
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_Study_i::SetStudyLock(const char* theLockerID)
{
  SALOMEDS::Locker lock; 
  _impl->SetStudyLock(theLockerID);
}

//============================================================================
/*! Function : IsStudyLocked
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDS_Study_i::IsStudyLocked()
{
  SALOMEDS::Locker lock; 
  return _impl->IsStudyLocked();
}

//============================================================================
/*! Function : UnLockStudy
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_Study_i::UnLockStudy(const char* theLockerID)
{
  SALOMEDS::Locker lock; 
  _impl->UnLockStudy(theLockerID);
}

//============================================================================
/*! Function : GetLockerID
 *  Purpose  : 
 */
//============================================================================
SALOMEDS::ListOfStrings* SALOMEDS_Study_i::GetLockerID()
{
  SALOMEDS::Locker lock; 

  SALOMEDS::ListOfStrings_var aResult = new SALOMEDS::ListOfStrings;

  vector<string> aSeq = _impl->GetLockerID();

  int aLength = aSeq.size();
  aResult->length(aLength);
  for(int anIndex = 0; anIndex < aLength; anIndex++) {
    aResult[anIndex] = CORBA::string_dup(aSeq[anIndex].c_str());
  }
  return aResult._retn();
}

//============================================================================
/*! Function : GetDefaultScript
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_Study_i::GetDefaultScript(const char* theModuleName, const char* theShift)
{
  SALOMEDS::Locker lock; 

  string script = SALOMEDSImpl_IParameters::getDefaultScript(_impl, theModuleName, theShift);
  return CORBA::string_dup(script.c_str());
}

//============================================================================
/*! Function : EnableUseCaseAutoFilling
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_Study_i::EnableUseCaseAutoFilling(CORBA::Boolean isEnabled) 
{ 
  _impl->EnableUseCaseAutoFilling(isEnabled); 
  SALOMEDSImpl_StudyBuilder* builder = _builder->GetImpl();
  if(builder) {
    if(isEnabled) {
      builder->SetOnAddSObject(_impl->GetCallback());
      builder->SetOnRemoveSObject(_impl->GetCallback());
    }
    else {
      builder->SetOnAddSObject(NULL);
      builder->SetOnRemoveSObject(NULL);
    }
  }
}

//===========================================================================
//   PRIVATE FUNCTIONS
//===========================================================================
CORBA::LongLong SALOMEDS_Study_i::GetLocalImpl(const char* theHostname, CORBA::Long thePID, CORBA::Boolean& isLocal)
{
#ifdef WIN32
  long pid = (long)_getpid();
#else
  long pid = (long)getpid();
#endif  
  isLocal = (strcmp(theHostname, GetHostname().c_str()) == 0 && pid == thePID)?1:0;
  return ((CORBA::LongLong)_impl);
}
