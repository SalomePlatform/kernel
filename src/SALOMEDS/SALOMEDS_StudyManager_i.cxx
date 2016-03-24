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

//  File   : SALOMEDS_StudyManager_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "utilities.h"
#include "SALOME_LifeCycleCORBA.hxx"
#include "SALOMEDS_StudyManager_i.hxx"
#include "SALOMEDS_Study_i.hxx"
#include "SALOMEDS_SComponent_i.hxx"
#include "SALOMEDS_Driver_i.hxx"
#include "SALOMEDS.hxx"

#include "SALOMEDSImpl_Study.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_SComponent.hxx"
#include "SALOMEDSImpl_AttributeIOR.hxx"

#include "Utils_CorbaException.hxx"
#include "Utils_ExceptHandlers.hxx"
#include "Basics_Utils.hxx"
#include "SALOME_GenericObj_i.hh"

#include <sstream>
#include <vector>
#include <map>

#ifdef WIN32
#include <process.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif

UNEXPECT_CATCH(SalomeException,SALOME::SALOME_Exception);
UNEXPECT_CATCH(LockProtection, SALOMEDS::StudyBuilder::LockProtection);

static SALOMEDS_Driver_i* GetDriver(const SALOMEDSImpl_SObject& theObject, CORBA::ORB_ptr orb);

static std::map<int, PortableServer::POA_ptr> _mapOfPOA;

//============================================================================
/*! Function : SALOMEDS_StudyManager_i
 *  Purpose  : SALOMEDS_StudyManager_i constructor
 */
//============================================================================
SALOMEDS_StudyManager_i::SALOMEDS_StudyManager_i(CORBA::ORB_ptr orb, PortableServer::POA_ptr thePOA)
{
  _orb = CORBA::ORB::_duplicate(orb);
  _poa = PortableServer::POA::_duplicate(thePOA);
  _name_service = new SALOME_NamingService(_orb);
  // Study directory creation in the naming service : to register all
  // open studies in the session
  _name_service->Create_Directory("/Study");
  _impl = new SALOMEDSImpl_StudyManager;
  _factory = new SALOMEDS_DriverFactory_i(_orb);
}

//============================================================================
/*! Function : ~SALOMEDS_StudyManager_i
 *  Purpose  : SALOMEDS_StudyManager_i destructor
 */
//============================================================================
SALOMEDS_StudyManager_i::~SALOMEDS_StudyManager_i()
{
  // Destroy directory to register open studies
  _name_service->Destroy_Directory("/Study");
  delete _name_service;
  delete _factory;
  delete _impl;
}

//============================================================================
/*! Function : register_name
 *  Purpose  : Register the study Manager in the naming service under the
 *             context name
 */
//============================================================================
void SALOMEDS_StudyManager_i::register_name(const char * name)
{
  SALOMEDS::StudyManager_var aManager(_this());
  _name_service->Register(aManager.in(), name);
}


//============================================================================
/*! Function : NewStudy
 *  Purpose  : Create a New Study of name study_name
 */
//============================================================================
SALOMEDS::Study_ptr SALOMEDS_StudyManager_i::NewStudy(const char* study_name)
     throw(SALOME::SALOME_Exception)
{
  SALOMEDS::Locker lock;

#ifndef ALLOW_MULTI_STUDIES
  std::vector<SALOMEDSImpl_Study*> anOpened = _impl->GetOpenStudies();
  int aLength = anOpened.size();

  if(aLength)
    {
      MESSAGE("There is already an active study in this session. Launch a new session, or close the study");
      THROW_SALOME_CORBA_EXCEPTION("Problem on New Study.\nThere is already an active study in this session.\nLaunch a new session, or close the study", SALOME::BAD_PARAM)
    }
#endif // !ALLOW_MULTI_STUDIES

  SALOMEDSImpl_Study* aStudyImpl = _impl->NewStudy(study_name);
  if(!aStudyImpl) {
    MESSAGE("NewStudy : Error : " << _impl->GetErrorCode());
    return SALOMEDS::Study::_nil();
  }

  MESSAGE("NewStudy : Creating the CORBA servant holding it... ");

  SALOMEDS_Study_i *Study_servant = SALOMEDS_Study_i::GetStudyServant(aStudyImpl, _orb);
  PortableServer::ObjectId_var servantid = _poa->activate_object(Study_servant); // to use poa registered in _mapOfPOA
  SALOMEDS::Study_var Study = Study_servant->_this();

  // Register study in the naming service
  // Path to acces the study
  if(!_name_service->Change_Directory("/Study"))
      MESSAGE( "Unable to access the study directory" )
  else
      _name_service->Register(Study, study_name);

  // Assign the value of the IOR in the study->root
  CORBA::String_var IORStudy = _orb->object_to_string(Study);

  aStudyImpl->SetTransientReference((char*)IORStudy.in());

  _mapOfPOA[Study->StudyId()] = _poa;

  return Study._retn();
}

//============================================================================
/*! Function : Open
 *  Purpose  : Open a Study from it's persistent reference
 */
//============================================================================
SALOMEDS::Study_ptr  SALOMEDS_StudyManager_i::Open(const char* aUrl)
     throw(SALOME::SALOME_Exception)
{
  SALOMEDS::Locker lock;

  Unexpect aCatch(SalomeException);
  MESSAGE("Begin of SALOMEDS_StudyManager_i::Open");

  #ifndef ALLOW_MULTI_STUDIES
  std::vector<SALOMEDSImpl_Study*> anOpened = _impl->GetOpenStudies();
  int aLength = anOpened.size();

  if(aLength)
    {
      MESSAGE("There is already an active study in this session. Launch a new session, or close the study.");
      THROW_SALOME_CORBA_EXCEPTION("Problem on Open Study.\nThere is already an active study in this session.\nLaunch a new session, or close the study.", SALOME::BAD_PARAM)
    }
#endif // ;ALLOW_MULTI_STUDIES

  SALOMEDSImpl_Study* aStudyImpl = _impl->Open(std::string(aUrl));

  if ( !aStudyImpl )
    THROW_SALOME_CORBA_EXCEPTION("Impossible to Open study from file", SALOME::BAD_PARAM)

  MESSAGE("Open : Creating the CORBA servant holding it... ");

  // Temporary aStudyUrl in place of study name
  SALOMEDS_Study_i * Study_servant = SALOMEDS_Study_i::GetStudyServant(aStudyImpl, _orb);
  PortableServer::ObjectId_var servantid = _poa->activate_object(Study_servant); // to use poa register in _mapOfPOA
  SALOMEDS::Study_var Study = Study_servant->_this();

  // Assign the value of the IOR in the study->root
  CORBA::String_var IORStudy = _orb->object_to_string(Study);
  aStudyImpl->SetTransientReference((char*)IORStudy.in());

  _mapOfPOA[Study->StudyId()] = _poa;

  // Register study in the naming service
  // Path to acces the study
  if(!_name_service->Change_Directory("/Study")) MESSAGE( "Unable to access the study directory" )
  else _name_service->Register(Study, aStudyImpl->Name().c_str());

  return Study._retn();
}



//============================================================================
/*! Function : Close
 *  Purpose  : Close a study.
 *             If the study hasn't been saved, ask the user to confirm the
 *             close action without saving
 */
//============================================================================
void SALOMEDS_StudyManager_i::Close(SALOMEDS::Study_ptr aStudy)
{
  SALOMEDS::Locker lock;

  if(aStudy->_is_nil()) return;

  // Destroy study name in the naming service
  if(_name_service->Change_Directory("/Study")){
    CORBA::String_var aString(aStudy->Name());
    _name_service->Destroy_Name(aString.in());
  }

  SALOMEDS::unlock();
  aStudy->Close();
  SALOMEDS::lock();

  //remove study servant
  PortableServer::POA_ptr poa=GetPOA(aStudy);
  PortableServer::ServantBase* aservant=poa->reference_to_servant(aStudy);
  PortableServer::ObjectId_var anObjectId = poa->servant_to_id(aservant);
  poa->deactivate_object(anObjectId.in());
  aservant->_remove_ref(); // decrement for the call to reference_to_servant
  aservant->_remove_ref(); // to delete the object
}

//============================================================================
/*! Function : Save
 *  Purpose  : Save a Study to it's persistent reference
 */
//============================================================================
CORBA::Boolean SALOMEDS_StudyManager_i::Save(SALOMEDS::Study_ptr aStudy, CORBA::Boolean theMultiFile)
{
  SALOMEDS::Locker lock;

  if(aStudy->_is_nil()) {
    MESSAGE("Save error: Study is null");
    return false;
  }

  SALOMEDSImpl_Study* aStudyImpl = _impl->GetStudyByID(aStudy->StudyId());
  return _impl->Save(aStudyImpl, _factory, theMultiFile);
}

CORBA::Boolean SALOMEDS_StudyManager_i::SaveASCII(SALOMEDS::Study_ptr aStudy, CORBA::Boolean theMultiFile)
{
  SALOMEDS::Locker lock;

  if(aStudy->_is_nil()) {
    MESSAGE("SaveASCII error: Study is null");
    return false;
  }

  SALOMEDSImpl_Study* aStudyImpl = _impl->GetStudyByID(aStudy->StudyId());
  return _impl->SaveASCII(aStudyImpl, _factory, theMultiFile);
}

//=============================================================================
/*! Function : SaveAs
 *  Purpose  : Save a study to the persistent reference aUrl
 */
//============================================================================
CORBA::Boolean SALOMEDS_StudyManager_i::SaveAs(const char* aUrl, SALOMEDS::Study_ptr aStudy, CORBA::Boolean theMultiFile)
{
  SALOMEDS::Locker lock;

  if(aStudy->_is_nil()) {
    MESSAGE("SaveASCII error: Study is null");
    return false;
  }

  SALOMEDSImpl_Study* aStudyImpl = _impl->GetStudyByID(aStudy->StudyId());
  return _impl->SaveAs(std::string(aUrl), aStudyImpl, _factory, theMultiFile);
}

CORBA::Boolean SALOMEDS_StudyManager_i::SaveAsASCII(const char* aUrl, SALOMEDS::Study_ptr aStudy, CORBA::Boolean theMultiFile)
{
  SALOMEDS::Locker lock;

  if(aStudy->_is_nil()) {
    MESSAGE("SaveASCII error: Study is null");
    return false;
  }

  SALOMEDSImpl_Study* aStudyImpl = _impl->GetStudyByID(aStudy->StudyId());
  return _impl->SaveAsASCII(std::string(aUrl), aStudyImpl, _factory, theMultiFile);
}

//============================================================================
/*! Function : GetOpenStudies
 *  Purpose  : Get name list of open studies in the session
 */
//============================================================================
SALOMEDS::ListOfOpenStudies*  SALOMEDS_StudyManager_i::GetOpenStudies()
{
  SALOMEDS::Locker lock;

  std::vector<SALOMEDSImpl_Study*> anOpened = _impl->GetOpenStudies();
  int aLength = anOpened.size();

  SALOMEDS::ListOfOpenStudies_var _list_open_studies = new SALOMEDS::ListOfOpenStudies;
  _list_open_studies->length(aLength);

  if(!aLength)
    {
      MESSAGE("No active study in this session");
    }
  else
    {
      for (unsigned int ind=0; ind < aLength; ind++)
        {
          _list_open_studies[ind] = CORBA::string_dup(anOpened[ind]->Name().c_str());
          SCRUTE(_list_open_studies[ind]) ;
        }
    }
  return _list_open_studies._retn();
}

//============================================================================
/*! Function : GetStudyByName
 *  Purpose  : Get a study from its name
 */
//============================================================================
SALOMEDS::Study_ptr SALOMEDS_StudyManager_i::GetStudyByName(const char* aStudyName)
{
  SALOMEDS::Locker lock;

  SALOMEDSImpl_Study* aStudyImpl = _impl->GetStudyByName(std::string(aStudyName));

  if (!aStudyImpl)
  {
    MESSAGE(_impl->GetErrorCode().c_str());
    return SALOMEDS::Study::_nil();
  }

  SALOMEDS_Study_i* aStudy_servant = SALOMEDS_Study_i::GetStudyServant(aStudyImpl, _orb);
  return aStudy_servant->_this();
}

//============================================================================
/*! Function : GetStudyByID
 *  Purpose  : Get a study from its ID
 */
//============================================================================
SALOMEDS::Study_ptr SALOMEDS_StudyManager_i::GetStudyByID(CORBA::Short aStudyID)
{
  SALOMEDS::Locker lock;

  SALOMEDSImpl_Study* aStudyImpl = _impl->GetStudyByID(aStudyID);

  if (!aStudyImpl)
  {
    MESSAGE(_impl->GetErrorCode().c_str());
    return SALOMEDS::Study::_nil();
  }

  SALOMEDS_Study_i* aStudy_servant = SALOMEDS_Study_i::GetStudyServant(aStudyImpl, _orb);
  return aStudy_servant->_this();
}


//============================================================================
/*! Function : CanCopy
 *  Purpose  :
 */
//============================================================================
CORBA::Boolean SALOMEDS_StudyManager_i::CanCopy(SALOMEDS::SObject_ptr theObject)
{
  SALOMEDS::Locker lock;

  SALOMEDS::Study_var aStudy = theObject->GetStudy();
  SALOMEDSImpl_Study* aStudyImpl = _impl->GetStudyByID(aStudy->StudyId());
  CORBA::String_var anID = theObject->GetID();
  SALOMEDSImpl_SObject anObject = aStudyImpl->GetSObject(anID.in());

  SALOMEDS_Driver_i* aDriver = GetDriver(anObject, _orb);
  bool ret = _impl->CanCopy(anObject, aDriver);
  delete aDriver;
  return ret;
}

//============================================================================
/*! Function : Copy
 *  Purpose  :
 */
//============================================================================
CORBA::Boolean SALOMEDS_StudyManager_i::Copy(SALOMEDS::SObject_ptr theObject)
{
  SALOMEDS::Locker lock;

  SALOMEDS::Study_var aStudy = theObject->GetStudy();
  SALOMEDSImpl_Study* aStudyImpl = _impl->GetStudyByID(aStudy->StudyId());
  CORBA::String_var anID = theObject->GetID();
  SALOMEDSImpl_SObject anObject = aStudyImpl->GetSObject(anID.in());

  SALOMEDS_Driver_i* aDriver = GetDriver(anObject, _orb);
  bool ret = _impl->Copy(anObject, aDriver);
  delete aDriver;
  return ret;
}

//============================================================================
/*! Function : CanPaste
 *  Purpose  :
 */
//============================================================================
CORBA::Boolean SALOMEDS_StudyManager_i::CanPaste(SALOMEDS::SObject_ptr theObject)
{
  SALOMEDS::Locker lock;

  SALOMEDS::Study_var aStudy = theObject->GetStudy();
  SALOMEDSImpl_Study* aStudyImpl = _impl->GetStudyByID(aStudy->StudyId());
  CORBA::String_var anID = theObject->GetID();
  SALOMEDSImpl_SObject anObject = aStudyImpl->GetSObject(anID.in());

  SALOMEDS_Driver_i* aDriver = GetDriver(anObject, _orb);
  bool ret = _impl->CanPaste(anObject, aDriver);
  delete aDriver;
  return ret;
}

//============================================================================
/*! Function : Paste
 *  Purpose  :
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_StudyManager_i::Paste(SALOMEDS::SObject_ptr theObject)
     throw(SALOMEDS::StudyBuilder::LockProtection)
{
  SALOMEDS::Locker lock;

  Unexpect aCatch(LockProtection);
  SALOMEDS::Study_var aStudy = theObject->GetStudy();

  SALOMEDSImpl_Study* aStudyImpl = _impl->GetStudyByID(aStudy->StudyId());
  CORBA::String_var anID = theObject->GetID();
  SALOMEDSImpl_SObject anObject = aStudyImpl->GetSObject(anID.in());
  SALOMEDSImpl_SObject aNewSO;

  try {
    SALOMEDS_Driver_i* aDriver = GetDriver(anObject, _orb);
    aNewSO =  _impl->Paste(anObject, aDriver);
    delete aDriver;
  }
  catch (...) {
    throw SALOMEDS::StudyBuilder::LockProtection();
  }

  SALOMEDS::SObject_var so = SALOMEDS_SObject_i::New (aNewSO, _orb);
  return so._retn();
}


SALOMEDS_Driver_i* GetDriver(const SALOMEDSImpl_SObject& theObject, CORBA::ORB_ptr orb)
{
  SALOMEDS_Driver_i* driver = NULL;

  SALOMEDSImpl_SComponent aSCO = theObject.GetFatherComponent();
  if(!aSCO.IsNull()) {
    std::string IOREngine = aSCO.GetIOR();
    if(!IOREngine.empty()) {
      CORBA::Object_var obj = orb->string_to_object(IOREngine.c_str());
      Engines::EngineComponent_var Engine = Engines::EngineComponent::_narrow(obj) ;
      driver = new SALOMEDS_Driver_i(Engine, orb);
    }
  }

  return driver;
}

PortableServer::POA_ptr SALOMEDS_StudyManager_i::GetPOA(const SALOMEDS::Study_ptr theStudy) {
  if (_mapOfPOA.find(theStudy->StudyId()) != _mapOfPOA.end()) return _mapOfPOA[theStudy->StudyId()];
  return PortableServer::POA::_nil();
}

CORBA::Long SALOMEDS_StudyManager_i::getPID()
{
#ifdef WIN32
  return (CORBA::Long)_getpid();
#else
  return (CORBA::Long)getpid();
#endif
}

void SALOMEDS_StudyManager_i::ShutdownWithExit()
{
  exit( EXIT_SUCCESS );
}

//===========================================================================
//   PRIVATE FUNCTIONS
//===========================================================================
CORBA::LongLong SALOMEDS_StudyManager_i::GetLocalImpl(const char* theHostname, CORBA::Long thePID, CORBA::Boolean& isLocal)
{
#ifdef WIN32
  long pid = (long)_getpid();
#else
  long pid = (long)getpid();
#endif
  isLocal = (strcmp(theHostname, Kernel_Utils::GetHostname().c_str()) == 0 && pid == thePID)?1:0;
  return reinterpret_cast<CORBA::LongLong>(_impl);
}

//===========================================================================
namespace SALOMEDS
{
  PortableServer::ServantBase_var
  GetServant(CORBA::Object_ptr theObject, PortableServer::POA_ptr thePOA)
  {
    if(CORBA::is_nil(theObject))
      return NULL;
    try{
      return thePOA->reference_to_servant(theObject);
    }catch(...){
      return NULL;
    }
  }

}

//===========================================================================
