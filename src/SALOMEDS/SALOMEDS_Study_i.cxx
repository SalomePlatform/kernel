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

//  File   : SALOMEDS_Study_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "utilities.h"
#include <sstream>
#include "SALOMEDS_Study_i.hxx"
#include "SALOMEDS_StudyManager_i.hxx"
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
#include "SALOMEDSImpl_Callback.hxx"

#include "DF_Label.hxx"
#include "DF_Attribute.hxx"

#include "Basics_Utils.hxx"
#include "SALOME_KernelServices.hxx"

#ifdef WIN32
#include <process.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif

namespace SALOMEDS
{
  class Notifier: public SALOMEDSImpl_AbstractCallback
  {
  public:
    Notifier(CORBA::ORB_ptr orb)
    {
      _orb = CORBA::ORB::_duplicate(orb);
    }

    //============================================================================
    /*! Function : addSO_Notification
     *  Purpose  : This function tells all the observers that a SO has been added
     */
    //============================================================================

    virtual bool addSO_Notification(const SALOMEDSImpl_SObject& theSObject)
    {
      std::string anID=theSObject.GetID();
      const char* cID=anID.c_str();
      for (ObsListIter it (myObservers.begin()); it != myObservers.end(); ++it)
      {
        it->first->notifyObserverID(cID,1);
      }
      return true; // NGE return always true but can be modified if needed
    }

    //============================================================================
    /*! Function : removeSO_Notification
     *  Purpose  : This function tells all the observers that a SO has been removed
     */
    //============================================================================

    virtual bool removeSO_Notification(const SALOMEDSImpl_SObject& theSObject)
    {
      std::string anID=theSObject.GetID();
      const char* cID=anID.c_str();
      for (ObsListIter it (myObservers.begin()); it != myObservers.end(); ++it)
      {
        it->first->notifyObserverID(cID,2);
      }
      return true; // NGE return always true but can be modified if needed
    }

    //============================================================================
    /*! Function : modifySO_Notification
     *  Purpose  : This function tells all the observers that a SO has been modified
     */
    //============================================================================

    virtual bool modifySO_Notification(const SALOMEDSImpl_SObject& theSObject, int reason)
    {
      for (ObsListIter it (myObservers.begin()); it != myObservers.end(); ++it)
      {
        if(it->second)
        {
          std::string anID=theSObject.GetID();
          const char* cID=anID.c_str();
          it->first->notifyObserverID(cID,reason);
        }
      }
      return true; // NGE return always true but can be modified if needed
    }

    //============================================================================
    /*! Function : modifyNB_Notification
     *  Purpose  : This function tells all the observers that 
     *             a NoteBook variable has been added/modified/removed.
     */
    //============================================================================
    
    virtual bool modifyNB_Notification(const char* theVarName)
    {
      for (ObsListIter it (myObservers.begin()); it != myObservers.end(); ++it)
        {
          it->first->notifyObserverID(theVarName,6);
        }
      return true; // NGE return always true but can be modified if needed
    }

    //============================================================================
    /*! Function : attach
     *  Purpose  : register an Observer
     */
    //============================================================================

    virtual void attach(SALOMEDS::Observer_ptr theObs, bool modify)
    {
      myObservers.push_back(std::make_pair(SALOMEDS::Observer::_duplicate(theObs),modify));
    }

    //============================================================================
    /*! Function : detach
     *  Purpose  : unregister an Observer
     */
    //============================================================================

    virtual void detach(SALOMEDS::Observer_ptr theObs)
    {
      for (ObsListIter it (myObservers.begin()); it != myObservers.end(); ++it)
      {
	if ( it->first->_is_equivalent(theObs) ) {
	  myObservers.erase( it );
	  break;
	}
      }
    }

  private:
    typedef std::list< std::pair< SALOMEDS::Observer_var, bool > > ObsList;
    typedef ObsList::iterator ObsListIter;
    ObsList myObservers;
    CORBA::ORB_var                    _orb;
  };

  class GenObjRegister: public SALOMEDSImpl_AbstractCallback
  {
  public:
    GenObjRegister(CORBA::ORB_ptr orb)
    {
      _orb = CORBA::ORB::_duplicate(orb);
    }
    virtual void RegisterGenObj  (const std::string& theIOR)
    {
      try
      {
        CORBA::Object_var obj = _orb->string_to_object(theIOR.c_str());
        if ( obj->_non_existent() ) return;
        SALOME::GenericObj_var gobj = SALOME::GenericObj::_narrow(obj);
        if(! CORBA::is_nil(gobj) )
        {
          gobj->Register();
        }
      }
      catch(const CORBA::Exception& e)
      {
      }
    }
    virtual void UnRegisterGenObj(const std::string& theIOR)
    {
      try
      {
        CORBA::Object_var obj = _orb->string_to_object(theIOR.c_str());
        if ( obj->_non_existent() ) return;
        SALOME::GenericObj_var gobj = SALOME::GenericObj::_narrow(obj);
        if(! CORBA::is_nil(gobj) )
        {
          gobj->UnRegister();
        }
      }
      catch(const CORBA::Exception& e)
      {
      }
    }

  private:
    CORBA::ORB_var _orb;
  };

} // namespace SALOMEDS

std::map<SALOMEDSImpl_Study* , SALOMEDS_Study_i*> SALOMEDS_Study_i::_mapOfStudies;

//============================================================================
/*! Function : SALOMEDS_Study_i
 *  Purpose  : SALOMEDS_Study_i constructor
 */
//============================================================================
SALOMEDS_Study_i::SALOMEDS_Study_i(SALOMEDSImpl_Study* theImpl,
                                   CORBA::ORB_ptr      orb)
{
  _orb            = CORBA::ORB::_duplicate(orb);
  _impl           = theImpl;
  _builder        = new SALOMEDS_StudyBuilder_i(_impl->NewBuilder(), _orb);  
  _notifier       = new SALOMEDS::Notifier(_orb);
  _genObjRegister = new SALOMEDS::GenObjRegister(_orb);
  _closed         = false;

  theImpl->setNotifier(_notifier);
  theImpl->setGenObjRegister( _genObjRegister );

  // Notify GUI that study was created
  SALOME_NamingService *aNamingService = KERNEL::getNamingService();
  CORBA::Object_var obj = aNamingService->Resolve("/Kernel/Session");
  SALOME::Session_var aSession = SALOME::Session::_narrow(obj);
  if ( !CORBA::is_nil(aSession) ) {
    std::stringstream ss;
    ss << "studyCreated:" << theImpl->StudyId();
    std::string str = ss.str();
    SALOMEDS::unlock();
    aSession->emitMessageOneWay(str.c_str());
    SALOMEDS::lock();
  }
}
  
//============================================================================
/*! Function : ~SALOMEDS_Study_i
 *  Purpose  : SALOMEDS_Study_i destructor
 */
//============================================================================
SALOMEDS_Study_i::~SALOMEDS_Study_i()
{
  //delete the builder servant
  PortableServer::POA_var poa=_builder->_default_POA();
  PortableServer::ObjectId_var anObjectId = poa->servant_to_id(_builder);
  poa->deactivate_object(anObjectId.in());
  _builder->_remove_ref();
  
  _impl->setNotifier(0);
  delete _notifier;
  delete _genObjRegister;
  //delete implementation
  delete _impl;
  _mapOfStudies.erase(_impl);
}  

//============================================================================
/*! Function : GetPersistentReference
 *  Purpose  : Get persistent reference of study (idem URL())
 */
//============================================================================
char* SALOMEDS_Study_i::GetPersistentReference()
{
  SALOMEDS::Locker lock; 
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  
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
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  
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
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  
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
  
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  SALOMEDS::SComponent_var sco;

  SALOMEDSImpl_SComponent aCompImpl = _impl->FindComponent(std::string(aComponentName));
  if (!aCompImpl.IsNull())
    sco = SALOMEDS_SComponent_i::New(aCompImpl, _orb);

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
  
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  SALOMEDS::SComponent_var sco;

  SALOMEDSImpl_SComponent aCompImpl = _impl->FindComponentID(std::string((char*)aComponentID));
  if (!aCompImpl.IsNull())
    sco = SALOMEDS_SComponent_i::New(aCompImpl, _orb);

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

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  SALOMEDS::SObject_var so;

  SALOMEDSImpl_SObject aSO = _impl->FindObject(std::string((char*)anObjectName));
  if (!aSO.IsNull()) {
    if (aSO.IsComponent()) {
      SALOMEDSImpl_SComponent aSCO = aSO;
      so = SALOMEDS_SComponent_i::New(aSCO, _orb);
    }
    else {
      so = SALOMEDS_SObject_i::New(aSO, _orb);
    }
  }

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

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  SALOMEDS::SObject_var so;

  SALOMEDSImpl_SObject aSO = _impl->FindObjectID(std::string((char*)anObjectID));
  if (!aSO.IsNull())
    so = SALOMEDS_SObject_i::New(aSO, _orb);

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

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  SALOMEDS::SObject_var so;

  if (anObjectID && strlen(anObjectID) > 0) {
    SALOMEDSImpl_SObject aSO = _impl->CreateObjectID((char*)anObjectID);
    if (!aSO.IsNull())
      so = SALOMEDS_SObject_i::New(aSO, _orb);
  }

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

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  std::vector<SALOMEDSImpl_SObject> aSeq = _impl->FindObjectByName(std::string((char*)anObjectName),
								   std::string((char*)aComponentName));

  SALOMEDS::Study::ListOfSObject_var listSO = new SALOMEDS::Study::ListOfSObject;
  int aLength = aSeq.size();
  listSO->length(aLength);
  for (int i = 0; i < aLength; i++) {
    SALOMEDS::SObject_var so = SALOMEDS_SObject_i::New(aSeq[i], _orb);
    listSO[i] = so;
  }
  
  return listSO._retn();
}

//============================================================================
/*! Function : FindObjectIOR
 *  Purpose  : Find an Object with IOR = anObjectIOR
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_Study_i::FindObjectIOR(const char* anObjectIOR)
{
  SALOMEDS::Locker lock; 

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  SALOMEDS::SObject_var so;

  SALOMEDSImpl_SObject aSO = _impl->FindObjectIOR(std::string((char*)anObjectIOR));
  if (!aSO.IsNull())
    so = SALOMEDS_SObject_i::New(aSO, _orb);

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

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  SALOMEDS::SObject_var so;

  SALOMEDSImpl_SObject aSO = _impl->FindObjectByPath(std::string((char*)thePath));
  if (!aSO.IsNull())
    so = SALOMEDS_SObject_i::New (aSO, _orb);

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

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  std::string aPath = "";

  if (!CORBA::is_nil(theObject)) {
    SALOMEDS::SObject_var aSObj = SALOMEDS::SObject::_narrow(theObject);
    SALOMEDSImpl_SObject aSO;

    if (!CORBA::is_nil(aSObj)) {
      aSO = _impl->FindObjectID(aSObj->GetID());
    }
    else {
      aSO = _impl->FindObjectIOR(_orb->object_to_string(theObject));
    }
    
    if (!aSO.IsNull()) {    
      aPath = _impl->GetObjectPath(aSO);
    }
  }

  return CORBA::string_dup(aPath.c_str());
}


//============================================================================
/*! Function : SetContext
 *  Purpose  : Sets the current context
 */
//============================================================================
void SALOMEDS_Study_i::SetContext(const char* thePath) 
{
  SALOMEDS::Locker lock; 

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  _impl->SetContext(std::string((char*)thePath));
  if (_impl->IsError() && _impl->GetErrorCode() == "InvalidContext") 
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
  
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  if (!_impl->HasCurrentContext()) throw SALOMEDS::Study::StudyInvalidContext();

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

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  SALOMEDS::ListOfStrings_var aResult = new SALOMEDS::ListOfStrings;

  if (strlen(theContext) == 0 && !_impl->HasCurrentContext())
    throw SALOMEDS::Study::StudyInvalidContext();
  
  std::vector<std::string> aSeq = _impl->GetObjectNames(std::string((char*)theContext));
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

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  SALOMEDS::ListOfStrings_var aResult = new SALOMEDS::ListOfStrings;

  if (strlen(theContext) == 0 && !_impl->HasCurrentContext())
    throw SALOMEDS::Study::StudyInvalidContext();
  
  std::vector<std::string> aSeq = _impl->GetDirectoryNames(std::string((char*)theContext));
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

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  SALOMEDS::ListOfStrings_var aResult = new SALOMEDS::ListOfStrings;

  if (strlen(theContext) == 0 && !_impl->HasCurrentContext())
    throw SALOMEDS::Study::StudyInvalidContext();
  
  std::vector<std::string> aSeq = _impl->GetFileNames(std::string((char*)theContext));
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

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  SALOMEDS::ListOfStrings_var aResult = new SALOMEDS::ListOfStrings;

  std::vector<std::string> aSeq = _impl->GetComponentNames(std::string((char*)theContext));

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

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  CORBA::String_var anID = theSO->GetID();
  SALOMEDSImpl_SObject aSO = _impl->GetSObject(anID.in());
  SALOMEDSImpl_ChildIterator anItr(aSO);
  SALOMEDS_ChildIterator_i* it_servant = new SALOMEDS_ChildIterator_i(anItr, _orb);
  SALOMEDS::ChildIterator_var it = it_servant->_this();

  return it._retn();
}


//============================================================================
/*! Function : NewComponentIterator
 *  Purpose  : Create a SComponentIterator
 */
//============================================================================
SALOMEDS::SComponentIterator_ptr SALOMEDS_Study_i::NewComponentIterator()
{
  SALOMEDS::Locker lock; 

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  SALOMEDS_SComponentIterator_i* it_servant = new SALOMEDS_SComponentIterator_i(_impl->NewComponentIterator(), _orb);
  it_servant->Init();
  SALOMEDS::SComponentIterator_var it = it_servant->_this();

  return it._retn();
}


//============================================================================
/*! Function : NewBuilder
 *  Purpose  : Create a StudyBuilder
 */
//============================================================================
SALOMEDS::StudyBuilder_ptr SALOMEDS_Study_i::NewBuilder()
{
  SALOMEDS::Locker lock; 

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  SALOMEDS::StudyBuilder_var sb = SALOMEDS::StudyBuilder::_duplicate(_builder->_this());

  return sb._retn();
}
 
//============================================================================
/*! Function : Name
 *  Purpose  : get study name
 */
//============================================================================
char* SALOMEDS_Study_i::Name()
{
  SALOMEDS::Locker lock; 
  // Name is specified as IDL attribute: user exception cannot be raised
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
  // Name is specified as IDL attribute: user exception cannot be raised
  _impl->Name(std::string(name));
}

//============================================================================
/*! Function : IsSaved
 *  Purpose  : get if study has been saved
 */
//============================================================================
CORBA::Boolean SALOMEDS_Study_i::IsSaved()
{
  SALOMEDS::Locker lock; 
  // IsSaved is specified as IDL attribute: user exception cannot be raised
  return (!_closed) ? _impl->IsSaved() : false;
}

//============================================================================
/*! Function : IsSaved
 *  Purpose  : set if study has been saved
 */
//============================================================================
void SALOMEDS_Study_i::IsSaved(CORBA::Boolean save)
{
  SALOMEDS::Locker lock; 
  // IsSaved is specified as IDL attribute: user exception cannot be raised
  if (!_closed)
    _impl->IsSaved(save);
}

//============================================================================
/*! Function : IsModified
 *  Purpose  : Detect if a Study has been modified since it has been saved
 */
//============================================================================
CORBA::Boolean SALOMEDS_Study_i::IsModified()
{
  SALOMEDS::Locker lock; 

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  return _impl->IsModified();
}

//============================================================================
/*! Function : Modified
 *  Purpose  : Sets a Modified flag of a Study to True
 */
//============================================================================
void SALOMEDS_Study_i::Modified()
{
  SALOMEDS::Locker lock; 

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  _impl->Modify();
}

//============================================================================
/*! Function : URL
 *  Purpose  : get URL of the study (persistent reference of the study)
 */
//============================================================================
char* SALOMEDS_Study_i::URL()
{
  SALOMEDS::Locker lock; 
  // URL is specified as IDL attribute: user exception cannot be raised
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
  // URL is specified as IDL attribute: user exception cannot be raised
  _impl->URL(std::string((char*)url));
}

CORBA::Short SALOMEDS_Study_i::StudyId()
{
  SALOMEDS::Locker lock; 
  // StudyId is specified as IDL attribute: user exception cannot be raised
  return _impl->StudyId();
}

void SALOMEDS_Study_i::StudyId(CORBA::Short id)
{ 
  SALOMEDS::Locker lock; 
  // StudyId is specified as IDL attribute: user exception cannot be raised
  _impl->StudyId(id);
}

void SALOMEDS_Study_i::UpdateIORLabelMap(const char* anIOR, const char* anEntry) 
{
  SALOMEDS::Locker lock; 

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  _impl->UpdateIORLabelMap(std::string((char*)anIOR), std::string((char*)anEntry));
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

SALOMEDS_Study_i* SALOMEDS_Study_i::GetStudyServant(SALOMEDSImpl_Study* aStudyImpl, CORBA::ORB_ptr orb)
{
  if (_mapOfStudies.find(aStudyImpl) != _mapOfStudies.end()) 
    return _mapOfStudies[aStudyImpl];
  else
  {
    SALOMEDS_Study_i *Study_servant = new SALOMEDS_Study_i(aStudyImpl, orb);
    _mapOfStudies[aStudyImpl]=Study_servant;
    return Study_servant;
  }
}

void SALOMEDS_Study_i::IORUpdated(SALOMEDSImpl_AttributeIOR* theAttribute) 
{
  SALOMEDS::Locker lock; 
  SALOMEDSImpl_Study::IORUpdated(theAttribute);
}

SALOMEDS::Study::ListOfSObject* SALOMEDS_Study_i::FindDependances(SALOMEDS::SObject_ptr anObject) 
{
  SALOMEDS::Locker lock; 

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

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
  
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  SALOMEDSImpl_AttributeStudyProperties* anAttr = _impl->GetProperties();
  SALOMEDS_AttributeStudyProperties_i* SP = new SALOMEDS_AttributeStudyProperties_i(anAttr, _orb);
  SALOMEDS::AttributeStudyProperties_var aProp = SP->_this();
  return aProp._retn();
}

char* SALOMEDS_Study_i::GetLastModificationDate() 
{
  SALOMEDS::Locker lock; 

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  return CORBA::string_dup(_impl->GetLastModificationDate().c_str());
}

SALOMEDS::ListOfDates* SALOMEDS_Study_i::GetModificationsDate() 
{
  SALOMEDS::Locker lock; 
  
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  SALOMEDS::ListOfDates_var aDates = new SALOMEDS::ListOfDates;

  std::vector<std::string> aSeq = _impl->GetModificationsDate();

  int aLength = aSeq.size();
  aDates->length(aLength);
  for (int anIndex = 0; anIndex < aLength; anIndex++) {
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

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  SALOMEDS_UseCaseBuilder_i* UCBuilder = new SALOMEDS_UseCaseBuilder_i(_impl->GetUseCaseBuilder(), _orb);
  SALOMEDS::UseCaseBuilder_var uc = UCBuilder->_this();
  return uc._retn();
}


//============================================================================
/*! Function : Close
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_Study_i::Close()
{
  SALOMEDS::Locker lock; 

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  RemovePostponed(-1);
  
  SALOMEDS::SComponentIterator_var itcomponent = NewComponentIterator();
  for (; itcomponent->More(); itcomponent->Next()) {
    SALOMEDS::SComponent_var sco = itcomponent->Value();
    CORBA::String_var compodatatype=sco->ComponentDataType();
    MESSAGE ( "Look for an engine for data type :"<< compodatatype);
    // if there is an associated Engine call its method for closing
    CORBA::String_var IOREngine;
    if (sco->ComponentIOR(IOREngine)) {
      // we have found the associated engine to write the data 
      MESSAGE ( "We have found an engine for data type :"<< compodatatype);
      //_narrow can throw a corba exception
      try {
	CORBA::Object_var obj = _orb->string_to_object(IOREngine);
	if (!CORBA::is_nil(obj)) {
	  SALOMEDS::Driver_var anEngine = SALOMEDS::Driver::_narrow(obj) ;
	  if (!anEngine->_is_nil())  { 
	    SALOMEDS::unlock();
	    anEngine->Close(sco);
	    SALOMEDS::lock();
	  }
	}
      } 
      catch (CORBA::Exception&) {
      }
    }
    sco->UnRegister();
  }
  
  //Does not need any more this iterator
  itcomponent->UnRegister();
  
  // Notify GUI that study is closed
  SALOME_NamingService *aNamingService = KERNEL::getNamingService();
  CORBA::Object_ptr obj = aNamingService->Resolve("/Kernel/Session");
  SALOME::Session_var aSession = SALOME::Session::_narrow(obj);
  if ( !CORBA::is_nil(aSession) ) {
    std::stringstream ss;
    ss << "studyClosed:" << _impl->StudyId();
    std::string str = ss.str();
    SALOMEDS::unlock();
    aSession->emitMessageOneWay(str.c_str());
    SALOMEDS::lock();
  }
  
  _impl->Close();
  _closed = true;
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
void SALOMEDS_Study_i::RemovePostponed(CORBA::Long /*theUndoLimit*/) 
{  
  SALOMEDS::Locker lock; 

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  std::vector<std::string> anIORs = _impl->GetIORs();
  int i, aSize = (int)anIORs.size();
  
  for (i = 0; i < aSize; i++) {
    try {
      CORBA::Object_var obj = _orb->string_to_object(anIORs[i].c_str());
      SALOME::GenericObj_var aGeneric = SALOME::GenericObj::_narrow(obj);
      //rnv: To avoid double deletion of the Salome Generic Objects:
      //rnv: 1. First decrement of the reference count in the SALOMEDSImpl_AttributeIOR::~SALOMEDSImpl_AttributeIOR();
      //rnv: 2. Second decrement of the reference count in the next string : aGeneric->UnRegister();
      //if (!CORBA::is_nil(aGeneric)) aGeneric->UnRegister();
    } catch (...) {}
  }

  //Not implemented
}

//============================================================================
/*! Function : UndoPostponed
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_Study_i::UndoPostponed(CORBA::Long theWay) 
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
                                           CORBA::Boolean isPublished,
                                           CORBA::Boolean isMultiFile)
{
  SALOMEDS::Locker lock; 

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  std::string aPath((char*)thePath), aBaseName((char*)theBaseName);
  SALOMEDS_DriverFactory_i* factory = new SALOMEDS_DriverFactory_i(_orb);
  bool ret = _impl->DumpStudy(aPath, aBaseName, isPublished, isMultiFile, factory);
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
  
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  SALOMEDSImpl_AttributeParameter* anAttr = _impl->GetCommonParameters(theID, theSavePoint);
  SALOMEDS_AttributeParameter_i* SP = new SALOMEDS_AttributeParameter_i(anAttr, _orb);
  SALOMEDS::AttributeParameter_var aParam = SP->_this();

  return aParam._retn();
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
  
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  SALOMEDSImpl_AttributeParameter* anAttr = _impl->GetModuleParameters(theID, theModuleName, theSavePoint);
  SALOMEDS_AttributeParameter_i* SP = new SALOMEDS_AttributeParameter_i(anAttr, _orb);
  SALOMEDS::AttributeParameter_var aParam = SP->_this();

  return aParam._retn();
}

//============================================================================
/*! Function : SetStudyLock
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_Study_i::SetStudyLock(const char* theLockerID)
{
  SALOMEDS::Locker lock; 

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

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

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

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

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

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

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  SALOMEDS::ListOfStrings_var aResult = new SALOMEDS::ListOfStrings;

  std::vector<std::string> aSeq = _impl->GetLockerID();

  int aLength = aSeq.size();
  aResult->length(aLength);
  for(int anIndex = 0; anIndex < aLength; anIndex++) {
    aResult[anIndex] = CORBA::string_dup(aSeq[anIndex].c_str());
  }

  return aResult._retn();
}
//============================================================================
/*! Function : SetReal
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_Study_i::SetReal(const char* theVarName, CORBA::Double theValue)
{
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  


  _impl->SetVariable(std::string(theVarName), 
		     theValue,
		     SALOMEDSImpl_GenericVariable::REAL_VAR);
  if (_notifier)
    _notifier->modifyNB_Notification(theVarName);
}

//============================================================================
/*! Function : SetInteger
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_Study_i::SetInteger(const char* theVarName, CORBA::Long theValue)
{
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  _impl->SetVariable(std::string(theVarName), 
		     theValue,
		     SALOMEDSImpl_GenericVariable::INTEGER_VAR);
  if (_notifier)
    _notifier->modifyNB_Notification(theVarName);
}

//============================================================================
/*! Function : SetBoolean
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_Study_i::SetBoolean(const char* theVarName, CORBA::Boolean theValue)
{
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  _impl->SetVariable(std::string(theVarName), 
		     theValue,
		     SALOMEDSImpl_GenericVariable::BOOLEAN_VAR);
  if (_notifier)
    _notifier->modifyNB_Notification(theVarName);
}

//============================================================================
/*! Function : SetString
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_Study_i::SetString(const char* theVarName, const char* theValue)
{
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  _impl->SetStringVariable(std::string(theVarName), 
			   theValue,
			   SALOMEDSImpl_GenericVariable::STRING_VAR);
  if (_notifier)
    _notifier->modifyNB_Notification(theVarName);
}

//============================================================================
/*! Function : SetStringAsDouble
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_Study_i::SetStringAsDouble(const char* theVarName, CORBA::Double theValue)
{
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  _impl->SetStringVariableAsDouble(std::string(theVarName), 
				   theValue,
				   SALOMEDSImpl_GenericVariable::STRING_VAR);
}

//============================================================================
/*! Function : GetReal
 *  Purpose  : 
 */
//============================================================================
CORBA::Double SALOMEDS_Study_i::GetReal(const char* theVarName)
{
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  return _impl->GetVariableValue(std::string(theVarName));
}

//============================================================================
/*! Function : GetInteger
 *  Purpose  : 
 */
//============================================================================
CORBA::Long SALOMEDS_Study_i::GetInteger(const char* theVarName)
{
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  return (long)_impl->GetVariableValue(std::string(theVarName));
}

//============================================================================
/*! Function : GetBoolean
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_Study_i::GetBoolean(const char* theVarName)
{
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  return (bool)_impl->GetVariableValue(std::string(theVarName));
}

//============================================================================
/*! Function : GetString
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_Study_i::GetString(const char* theVarName)
{
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  return CORBA::string_dup(_impl->GetStringVariableValue(std::string(theVarName)).c_str());
}

//============================================================================
/*! Function : IsReal
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_Study_i::IsReal(const char* theVarName)
{
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  return _impl->IsTypeOf(std::string(theVarName),
			 SALOMEDSImpl_GenericVariable::REAL_VAR);
}

//============================================================================
/*! Function : IsInteger
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_Study_i::IsInteger(const char* theVarName)
{
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  return _impl->IsTypeOf(std::string(theVarName),
			 SALOMEDSImpl_GenericVariable::INTEGER_VAR);
}

//============================================================================
/*! Function : IsBoolean
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_Study_i::IsBoolean(const char* theVarName)
{
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  return _impl->IsTypeOf(std::string(theVarName),
			 SALOMEDSImpl_GenericVariable::BOOLEAN_VAR);
}

//============================================================================
/*! Function : IsString
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_Study_i::IsString(const char* theVarName)
{
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  return _impl->IsTypeOf(std::string(theVarName),
			 SALOMEDSImpl_GenericVariable::STRING_VAR);
}

//============================================================================
/*! Function : IsVariable
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_Study_i::IsVariable(const char* theVarName)
{
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  return _impl->IsVariable(std::string(theVarName));
}

//============================================================================
/*! Function : GetVariableNames
 *  Purpose  : 
 */
//============================================================================
SALOMEDS::ListOfStrings* SALOMEDS_Study_i::GetVariableNames()
{
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  SALOMEDS::ListOfStrings_var aResult = new SALOMEDS::ListOfStrings;

  std::vector<std::string> aVarNames = _impl->GetVariableNames();

  int aLen = aVarNames.size();
  aResult->length(aLen);
  for (int anInd = 0; anInd < aLen; anInd++)
    aResult[anInd] = CORBA::string_dup(aVarNames[anInd].c_str());
  
  return aResult._retn();
}

//============================================================================
/*! Function : RemoveVariable
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_Study_i::RemoveVariable(const char* theVarName)
{
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  bool res = _impl->RemoveVariable(std::string(theVarName));
  if (res && _notifier)
    _notifier->modifyNB_Notification(theVarName);

  return res;
}

//============================================================================
/*! Function : RenameVariable
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_Study_i::RenameVariable(const char* theVarName, const char* theNewVarName)
{
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  bool res = _impl->RenameVariable(std::string(theVarName), std::string(theNewVarName));
  if (res && _notifier)
    _notifier->modifyNB_Notification(theVarName);

  return res;
}

//============================================================================
/*! Function : IsVariableUsed
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_Study_i::IsVariableUsed(const char* theVarName)
{
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  return _impl->IsVariableUsed(std::string(theVarName));
}


//============================================================================
/*! Function : ParseVariables
 *  Purpose  : 
 */
//============================================================================
SALOMEDS::ListOfListOfStrings* SALOMEDS_Study_i::ParseVariables(const char* theVarName)
{
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  SALOMEDS::ListOfListOfStrings_var aResult = new SALOMEDS::ListOfListOfStrings;

  std::vector< std::vector<std::string> > aSections = _impl->ParseVariables(std::string(theVarName));

  int aSectionsLen = aSections.size();
  aResult->length(aSectionsLen);

  for (int aSectionInd = 0; aSectionInd < aSectionsLen; aSectionInd++) {
    std::vector<std::string> aVarNames = aSections[aSectionInd];

    SALOMEDS::ListOfStrings_var aList = new SALOMEDS::ListOfStrings;

    int aLen = aVarNames.size();
    aList->length(aLen);

    for (int anInd = 0; anInd < aLen; anInd++)
      aList[anInd] = CORBA::string_dup(aVarNames[anInd].c_str());

    aResult[aSectionInd] = aList;
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

  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  std::string script = SALOMEDSImpl_IParameters::getDefaultScript(_impl, theModuleName, theShift);
  return CORBA::string_dup(script.c_str());
}

//============================================================================
/*! Function : EnableUseCaseAutoFilling
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_Study_i::EnableUseCaseAutoFilling(CORBA::Boolean isEnabled) 
{ 
  if (_closed)
    throw SALOMEDS::Study::StudyInvalidReference();  

  _impl->EnableUseCaseAutoFilling(isEnabled); 
  SALOMEDSImpl_StudyBuilder* builder = _builder->GetImpl();
  if (builder) {
    if (isEnabled) {
      builder->SetOnAddSObject(_impl->GetCallback());
      builder->SetOnRemoveSObject(_impl->GetCallback());
    }
    else {
      builder->SetOnAddSObject(NULL);
      builder->SetOnRemoveSObject(NULL);
    }
  }
}

//============================================================================
/*! Function : attach
 *  Purpose  : This function attach an observer to the study
 */
//============================================================================
void SALOMEDS_Study_i::attach(SALOMEDS::Observer_ptr theObs, CORBA::Boolean modify)
{
  if (_notifier)
    static_cast<SALOMEDS::Notifier*>(_notifier)->attach(theObs, modify);
}


//============================================================================
/*! Function : detach
 *  Purpose  : This function detaches an observer from the study
 */
//============================================================================
void SALOMEDS_Study_i::detach(SALOMEDS::Observer_ptr theObs)
{
  if (_notifier)
    static_cast<SALOMEDS::Notifier*>(_notifier)->detach(theObs);
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
  isLocal = (strcmp(theHostname, Kernel_Utils::GetHostname().c_str()) == 0 && pid == thePID)?1:0;
  return reinterpret_cast<CORBA::LongLong>(_impl);
}
