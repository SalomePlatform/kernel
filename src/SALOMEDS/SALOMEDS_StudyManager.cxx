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

//  File   : SALOMEDSClient_StudyManager.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_StudyManager.hxx"

#include "SALOMEDS.hxx"
#include "SALOMEDS_Study.hxx"
#include "SALOMEDS_SObject.hxx"
#include "SALOMEDS_Driver_i.hxx"

#include "SALOMEDSImpl_Study.hxx"

#include "Utils_ORB_INIT.hxx"
#include "Utils_SINGLETON.hxx"
#include "utilities.h"

#include "Basics_Utils.hxx"

#ifdef WIN32
#include <process.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif

SALOMEDS_Driver_i* GetDriver(const SALOMEDSImpl_SObject& theObject, CORBA::ORB_ptr orb);

SALOMEDS_StudyManager::SALOMEDS_StudyManager(SALOMEDS::StudyManager_ptr theManager)
{

#ifdef WIN32
  long pid =  (long)_getpid();
#else
  long pid =  (long)getpid();
#endif

  CORBA::LongLong addr = theManager->GetLocalImpl(Kernel_Utils::GetHostname().c_str(), pid, _isLocal);
  if(_isLocal) {
    _local_impl = reinterpret_cast<SALOMEDSImpl_StudyManager*>(addr);
    _corba_impl = SALOMEDS::StudyManager::_duplicate(theManager);
  }
  else {
    _local_impl = NULL;
    _corba_impl = SALOMEDS::StudyManager::_duplicate(theManager);
  }

  init_orb();
}

SALOMEDS_StudyManager::SALOMEDS_StudyManager()
{
  init_orb();

  SALOME_NamingService namingService(_orb);
  CORBA::Object_var obj = namingService.Resolve( "/myStudyManager" );
  SALOMEDS::StudyManager_var theManager = SALOMEDS::StudyManager::_narrow( obj );
  ASSERT( !CORBA::is_nil(theManager) );

#ifdef WIN32
  long pid =  (long)_getpid();
#else
  long pid =  (long)getpid();
#endif

  CORBA::LongLong addr = theManager->GetLocalImpl(Kernel_Utils::GetHostname().c_str(), pid, _isLocal);
  if(_isLocal) {
    _local_impl = reinterpret_cast<SALOMEDSImpl_StudyManager*>(addr);
    _corba_impl = SALOMEDS::StudyManager::_duplicate(theManager);
  }
  else {
    _local_impl = NULL;
    _corba_impl = SALOMEDS::StudyManager::_duplicate(theManager);
  }
}

SALOMEDS_StudyManager::~SALOMEDS_StudyManager()
{
}

_PTR(Study) SALOMEDS_StudyManager::NewStudy(const std::string& study_name)
{
  //SRN: Pure CORBA NewStudy as it does more initialization than the local one
  SALOMEDSClient_Study* aStudy = NULL;

  SALOMEDS::Study_var aStudy_impl = _corba_impl->NewStudy((char*)study_name.c_str());
  if(CORBA::is_nil(aStudy_impl)) return _PTR(Study)(aStudy);
  aStudy = new SALOMEDS_Study(aStudy_impl);

  return _PTR(Study)(aStudy);
}

_PTR(Study) SALOMEDS_StudyManager::Open(const std::string& theStudyUrl)
{
  //SRN: Pure CORBA Open as it does more initialization than the local one
  SALOMEDSClient_Study* aStudy = NULL;

  SALOMEDS::Study_var aStudy_impl = _corba_impl->Open((char*)theStudyUrl.c_str());
  if(CORBA::is_nil(aStudy_impl)) return  _PTR(Study)(aStudy);

  aStudy = new SALOMEDS_Study(aStudy_impl.in());

  return _PTR(Study)(aStudy);
}

void SALOMEDS_StudyManager::Close(const _PTR(Study)& theStudy)
{
  //SRN: Pure CORBA close as it does more cleaning than the local one
  SALOMEDS::Study_var aStudy = _corba_impl->GetStudyByID(theStudy->StudyId());
  _corba_impl->Close(aStudy);
}

bool SALOMEDS_StudyManager::Save(const _PTR(Study)& theStudy, bool theMultiFile)
{
  //SRN: Pure CORBA save as the save operation require CORBA in any case
  SALOMEDS::Study_var aStudy = _corba_impl->GetStudyByID(theStudy->StudyId());
  return _corba_impl->Save(aStudy, theMultiFile);
}

bool SALOMEDS_StudyManager::SaveASCII(const _PTR(Study)& theStudy, bool theMultiFile)
{
  //SRN: Pure CORBA save as the save operation require CORBA in any case
  SALOMEDS::Study_var aStudy = _corba_impl->GetStudyByID(theStudy->StudyId());
  return _corba_impl->SaveASCII(aStudy, theMultiFile);
}

bool SALOMEDS_StudyManager::SaveAs(const std::string& theUrl,  const _PTR(Study)& theStudy, bool theMultiFile)
{
  //SRN: Pure CORBA save as the save operation require CORBA in any case
  SALOMEDS::Study_var aStudy = _corba_impl->GetStudyByID(theStudy->StudyId());
  return _corba_impl->SaveAs((char*)theUrl.c_str(), aStudy, theMultiFile);
}

bool SALOMEDS_StudyManager::SaveAsASCII(const std::string& theUrl,  const _PTR(Study)& theStudy, bool theMultiFile)
{
  //SRN: Pure CORBA save as the save operation require CORBA in any case
  SALOMEDS::Study_var aStudy = _corba_impl->GetStudyByID(theStudy->StudyId());
  return _corba_impl->SaveAsASCII((char*)theUrl.c_str(), aStudy, theMultiFile);
}

std::vector<std::string> SALOMEDS_StudyManager::GetOpenStudies()
{
  std::vector<std::string> aVector;
  int aLength, i;

  if (_isLocal) {
    SALOMEDS::Locker lock;

    std::vector<SALOMEDSImpl_Study*> aSeq = _local_impl->GetOpenStudies();
    aLength = aSeq.size();
    for(i = 0; i < aLength; i++)
      aVector.push_back(aSeq[i]->Name());
  }
  else {
    SALOMEDS::ListOfOpenStudies_var aSeq = _corba_impl->GetOpenStudies();
    aLength = aSeq->length();
    for(i = 0; i < aLength; i++)
      aVector.push_back(aSeq[i].in());
  }
  return aVector;
}

_PTR(Study) SALOMEDS_StudyManager::GetStudyByName(const std::string& theStudyName)
{
  SALOMEDSClient_Study* aStudy = NULL;
  if (_isLocal) {
    SALOMEDS::Locker lock;

    SALOMEDSImpl_Study* aStudy_impl = _local_impl->GetStudyByName(theStudyName);
    if(!aStudy_impl) return _PTR(Study)(aStudy);
    aStudy = new SALOMEDS_Study(aStudy_impl);
  }
  else  {
    SALOMEDS::Study_var aStudy_impl = _corba_impl->GetStudyByName((char*)theStudyName.c_str());
    if(CORBA::is_nil(aStudy_impl)) return _PTR(Study)(aStudy);
    aStudy = new SALOMEDS_Study(aStudy_impl);
  }
  return _PTR(Study)(aStudy);
}

_PTR(Study) SALOMEDS_StudyManager::GetStudyByID(int theStudyID)
{
  SALOMEDSClient_Study* aStudy = NULL;
  if (_isLocal) {
    SALOMEDS::Locker lock;

    SALOMEDSImpl_Study* aStudy_impl = _local_impl->GetStudyByID(theStudyID);
    if(!aStudy_impl) return _PTR(Study)(aStudy);
    aStudy = new SALOMEDS_Study(aStudy_impl);
  }
  else {
    SALOMEDS::Study_var aStudy_impl = _corba_impl->GetStudyByID(theStudyID);
    if(CORBA::is_nil(aStudy_impl)) return _PTR(Study)(aStudy);
    aStudy = new SALOMEDS_Study(aStudy_impl);
  }
  return _PTR(Study)(aStudy);
}

bool SALOMEDS_StudyManager::CanCopy(const _PTR(SObject)& theSO)
{
  SALOMEDS_SObject* aSO = dynamic_cast<SALOMEDS_SObject*>(theSO.get());
  bool ret;

  if (_isLocal) {
    SALOMEDS::Locker lock;

    SALOMEDSImpl_SObject aSO_impl = *(aSO->GetLocalImpl());
    SALOMEDS_Driver_i* aDriver = GetDriver(aSO_impl, _orb);
    ret = _local_impl->CanCopy(aSO_impl, aDriver);
    delete aDriver;
  }
  else {
    ret = _corba_impl->CanCopy(aSO->GetCORBAImpl());
  }

  return ret;
}

bool SALOMEDS_StudyManager::Copy(const _PTR(SObject)& theSO)
{
  SALOMEDS_SObject* aSO = dynamic_cast<SALOMEDS_SObject*>(theSO.get());
  bool ret;
  if (_isLocal) {
    SALOMEDS::Locker lock;

    SALOMEDSImpl_SObject aSO_impl = *(aSO->GetLocalImpl());
    SALOMEDS_Driver_i* aDriver = GetDriver(aSO_impl, _orb);
    ret = _local_impl->Copy(aSO_impl, aDriver);
    delete aDriver;
  }
  else {
    ret = _corba_impl->Copy(aSO->GetCORBAImpl());
  }
  return ret;
}

bool SALOMEDS_StudyManager::CanPaste(const _PTR(SObject)& theSO)
{
  SALOMEDS_SObject* aSO = dynamic_cast<SALOMEDS_SObject*>(theSO.get());
  bool ret;

  if (_isLocal) {
    SALOMEDS::Locker lock;

    SALOMEDSImpl_SObject aSO_impl = *(aSO->GetLocalImpl());
    SALOMEDS_Driver_i* aDriver = GetDriver(aSO_impl, _orb);
    ret = _local_impl->CanPaste(aSO_impl, aDriver);
    delete aDriver;
  }
  else {
    ret = _corba_impl->CanPaste(aSO->GetCORBAImpl());
  }

  return ret;
}

_PTR(SObject) SALOMEDS_StudyManager::Paste(const _PTR(SObject)& theSO)
{
  SALOMEDS_SObject* aSO = dynamic_cast<SALOMEDS_SObject*>(theSO.get());
  SALOMEDSClient_SObject* aResult = NULL;

  if (_isLocal) {
    SALOMEDS::Locker lock;

    SALOMEDSImpl_SObject aSO_impl = *(aSO->GetLocalImpl());
    SALOMEDS_Driver_i* aDriver = GetDriver(aSO_impl, _orb);
    SALOMEDSImpl_SObject aNewSO = _local_impl->Paste(aSO_impl, aDriver);
    delete aDriver;
    if(aNewSO.IsNull()) return _PTR(SObject)(aResult);
    aResult = new SALOMEDS_SObject(aNewSO);
  }
  else {
    SALOMEDS::SObject_ptr aNewSO = _corba_impl->Paste(aSO->GetCORBAImpl());
    if(CORBA::is_nil(aNewSO)) return _PTR(SObject)(aResult);
    aResult = new SALOMEDS_SObject(aNewSO);
  }

  return _PTR(SObject)(aResult);
}


void SALOMEDS_StudyManager::init_orb()
{
  ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance();
  ASSERT(SINGLETON_<ORB_INIT>::IsAlreadyExisting());
  _orb = init(0 , 0 );
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
