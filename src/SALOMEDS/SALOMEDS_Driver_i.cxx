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

#include "SALOMEDS_Driver_i.hxx"
#include "SALOMEDS_TMPFile_i.hxx"
#include "utilities.h"
#include "SALOMEDS_SObject_i.hxx"
#include "SALOMEDS_SComponent_i.hxx"
#include "SALOMEDS_Study_i.hxx"
#include "SALOMEDS.hxx"
#include <SALOMEDSImpl_IParameters.hxx>
#include <stdlib.h>

#include CORBA_CLIENT_HEADER(SALOME_Session)

SALOMEDS_Driver_i::SALOMEDS_Driver_i(Engines::EngineComponent_ptr theEngine, CORBA::ORB_ptr theORB)
{
  // engine should not be null - component is supposed to be inherited from Engines::EngineComponent
  _engine = Engines::EngineComponent::_duplicate(theEngine);
  // driver can be null - if component interface does not inherit SALOMEDS::Driver
  _driver = SALOMEDS::Driver::_narrow(theEngine);
  _orb = CORBA::ORB::_duplicate(theORB);
}

SALOMEDS_Driver_i::SALOMEDS_Driver_i(SALOMEDS::Driver_ptr theDriver, CORBA::ORB_ptr theORB)
{
  // driver can be null - if component interface does not inherit SALOMEDS::Driver
  _driver = SALOMEDS::Driver::_duplicate(theDriver);
  // engine can be null - since it is narrowed from SALOMEDS::Driver ptr which can be null
  _engine = Engines::EngineComponent::_narrow(theDriver);
  _orb = CORBA::ORB::_duplicate(theORB);
}

SALOMEDS_Driver_i::~SALOMEDS_Driver_i()
{
}

std::string SALOMEDS_Driver_i::GetIOR()
{
  std::string ior = "";
  if ( !CORBA::is_nil(_engine) ) {
    CORBA::String_var cior = _orb->object_to_string(_engine);
    ior = cior;
  }
  return ior;
}

SALOMEDSImpl_TMPFile* SALOMEDS_Driver_i::Save(const SALOMEDSImpl_SComponent& theComponent,
                                              const std::string& theURL,
                                              long& theStreamLength,
                                              bool isMultiFile)
{
  SALOMEDS::SComponent_var sco = SALOMEDS_SComponent_i::New (theComponent, _orb);
  CORBA::String_var url = CORBA::string_dup(theURL.c_str());

  SALOMEDS::unlock();
  SALOMEDS::TMPFile_var aStream;

  if ( !CORBA::is_nil(_driver) )
    aStream = _driver->Save(sco.in(), url, isMultiFile);

  SALOMEDSImpl_TMPFile* aTMPFile = new SALOMEDS_TMPFile_i(aStream._retn());
  theStreamLength = aTMPFile->Size();

  sco->UnRegister();
  SALOMEDS::lock();

  return aTMPFile;
}

SALOMEDSImpl_TMPFile* SALOMEDS_Driver_i::SaveASCII(const SALOMEDSImpl_SComponent& theComponent,
                                                   const std::string& theURL,
                                                   long& theStreamLength,
                                                   bool isMultiFile)
{
  SALOMEDS::SComponent_var sco = SALOMEDS_SComponent_i::New (theComponent, _orb);
  CORBA::String_var url = CORBA::string_dup(theURL.c_str());

  SALOMEDS::unlock();
  SALOMEDS::TMPFile_var aStream;

  if ( !CORBA::is_nil(_driver) )
    aStream = _driver->SaveASCII(sco.in(), url, isMultiFile);

  SALOMEDSImpl_TMPFile* aTMPFile = new SALOMEDS_TMPFile_i(aStream._retn());
  theStreamLength = aTMPFile->Size();

  sco->UnRegister();
  SALOMEDS::lock();

  return aTMPFile;
}

bool SALOMEDS_Driver_i::Load(const SALOMEDSImpl_SComponent& theComponent,
                             const unsigned char* theStream,
                             const long theStreamLength,
                             const std::string& theURL,
                             bool isMultiFile)
{
  SALOMEDS::SComponent_var sco = SALOMEDS_SComponent_i::New (theComponent, _orb);
  CORBA::String_var url = CORBA::string_dup(theURL.c_str());
  CORBA::Octet* anOctetBuf = (CORBA::Octet*)theStream;

  SALOMEDS::TMPFile_var aStream;
  if (theStreamLength > 0)
    aStream = new SALOMEDS::TMPFile(theStreamLength, theStreamLength, anOctetBuf, 0);
  else
    aStream = new SALOMEDS::TMPFile(0);

  SALOMEDS::unlock();
  bool isOk = false;

  if ( !CORBA::is_nil(_driver) )
    isOk = _driver->Load(sco.in(), aStream.in(), url, isMultiFile);

  sco->UnRegister();
  SALOMEDS::lock();
  return isOk;
}

bool SALOMEDS_Driver_i::LoadASCII(const SALOMEDSImpl_SComponent& theComponent,
                                  const unsigned char* theStream,
                                  const long theStreamLength,
                                  const std::string& theURL,
                                  bool isMultiFile)
{
  SALOMEDS::SComponent_var sco = SALOMEDS_SComponent_i::New (theComponent, _orb);
  CORBA::String_var url = CORBA::string_dup(theURL.c_str());
  CORBA::Octet* anOctetBuf = (CORBA::Octet*)theStream;

  SALOMEDS::TMPFile_var aStream;
  if(theStreamLength > 0)
    aStream = new SALOMEDS::TMPFile(theStreamLength, theStreamLength, anOctetBuf, 0);
  else
    aStream = new SALOMEDS::TMPFile(0);

  SALOMEDS::unlock();
  bool isOk = false;

  if ( !CORBA::is_nil(_driver) )
    isOk = _driver->LoadASCII(sco.in(), aStream.in(), url, isMultiFile);

  sco->UnRegister();
  SALOMEDS::lock();
  return isOk;
}

void SALOMEDS_Driver_i::Close(const SALOMEDSImpl_SComponent& theComponent)
{
  SALOMEDS::SComponent_var sco = SALOMEDS_SComponent_i::New (theComponent, _orb);

  SALOMEDS::unlock();
  if ( !CORBA::is_nil(_driver) )
    _driver->Close(sco.in());
  sco->UnRegister();
  SALOMEDS::lock();
}

std::string SALOMEDS_Driver_i::ComponentDataType()
{
  std::string dtype = "";
  if ( !CORBA::is_nil(_driver) ) {
    CORBA::String_var type = _driver->ComponentDataType();
    dtype = type;
  }
  return dtype;
}

std::string SALOMEDS_Driver_i::Version()
{
  return !CORBA::is_nil( _engine ) ? _engine->getVersion() : std::string("");
}

std::string SALOMEDS_Driver_i::IORToLocalPersistentID(const SALOMEDSImpl_SObject& theSObject,
                                                      const std::string& IORString,
                                                      bool isMultiFile,
                                                      bool isASCII)
{
  SALOMEDS::SObject_var so = SALOMEDS_SObject_i::New (theSObject, _orb);
  SALOMEDS::unlock();

  std::string pstr = "";

  if ( !CORBA::is_nil(_driver) ) {
    CORBA::String_var pers_string =_driver->IORToLocalPersistentID(so.in(), IORString.c_str(),
                                                                   isMultiFile, isASCII);
    if ( pers_string.in() )
      pstr = pers_string;
  }

  so->UnRegister();
  SALOMEDS::lock();

  return pstr;
}


std::string SALOMEDS_Driver_i::LocalPersistentIDToIOR(const SALOMEDSImpl_SObject& theObject,
                                                      const std::string& aLocalPersistentID,
                                                      bool isMultiFile,
                                                      bool isASCII)
{
  SALOMEDS::SObject_var so = SALOMEDS_SObject_i::New (theObject, _orb);
  CORBA::String_var pers_string = CORBA::string_dup(aLocalPersistentID.c_str());
  SALOMEDS::unlock();

  std::string ior = "";

  if ( !CORBA::is_nil(_driver) ) {
    CORBA::String_var IOR = _driver->LocalPersistentIDToIOR(so.in(), pers_string.in(), isMultiFile, isASCII);
    if ( IOR.in() )
      ior = IOR;
  }
  so->UnRegister();
  SALOMEDS::lock();
  return ior;
}

bool SALOMEDS_Driver_i::CanCopy(const SALOMEDSImpl_SObject& theObject)
{
  SALOMEDS::SObject_var so = SALOMEDS_SObject_i::New (theObject, _orb);

  SALOMEDS::unlock();
  bool isOk = false;

  if ( !CORBA::is_nil(_driver) )
    isOk = _driver->CanCopy(so.in());

  so->UnRegister();
  SALOMEDS::lock();

  return isOk;
}


SALOMEDSImpl_TMPFile* SALOMEDS_Driver_i::CopyFrom(const SALOMEDSImpl_SObject& theObject,
                                                  int& theObjectID,
                                                  long& theStreamLength)
{
  SALOMEDS::SObject_var so = SALOMEDS_SObject_i::New (theObject, _orb);

  SALOMEDS::unlock();
  CORBA::Long anObjectID;
  SALOMEDS::TMPFile_var aStream;

  if ( !CORBA::is_nil(_driver) )
    aStream = _driver->CopyFrom(so.in(), anObjectID);

  SALOMEDSImpl_TMPFile* aTMPFile = new SALOMEDS_TMPFile_i(aStream._retn());
  theStreamLength = aTMPFile->Size();
  theObjectID = anObjectID;

  so->UnRegister();
  SALOMEDS::lock();

  return aTMPFile;
}

bool SALOMEDS_Driver_i::CanPaste(const std::string& theComponentName, int theObjectID)
{
  SALOMEDS::unlock();
  bool canPaste = false;

  if ( !CORBA::is_nil(_driver) )
    canPaste = _driver->CanPaste(theComponentName.c_str(), theObjectID);

  SALOMEDS::lock();
  return canPaste;
}

std::string SALOMEDS_Driver_i::PasteInto(const unsigned char* theStream,
                                         const long theStreamLength,
                                         int theObjectID,
                                         const SALOMEDSImpl_SObject& theObject)
{
  SALOMEDS::SObject_var so = SALOMEDS_SObject_i::New (theObject, _orb);
  CORBA::Octet* anOctetBuf =  (CORBA::Octet*)theStream;

  SALOMEDS::TMPFile_var aStream;
  if(theStreamLength > 0)
    aStream = new SALOMEDS::TMPFile(theStreamLength, theStreamLength, anOctetBuf, 0);
  else
    aStream = new SALOMEDS::TMPFile(0);

  SALOMEDS::unlock();

  std::string entry = "";

  if ( !CORBA::is_nil(_driver) ) {
    SALOMEDS::SObject_var ret_so = _driver->PasteInto(aStream.in(), theObjectID, so.in());
    entry = ret_so->GetID();
  }

  so->UnRegister();
  SALOMEDS::lock();

  return entry;
}

SALOMEDSImpl_TMPFile* SALOMEDS_Driver_i::DumpPython(SALOMEDSImpl_Study* theStudy,
                                                    bool isPublished,
                                                    bool isMultiFile,
                                                    bool& isValidScript,
                                                    long& theStreamLength)
{
  SALOMEDS_Study_i *  st_servant = SALOMEDS_Study_i::GetStudyServant(theStudy, _orb);//new SALOMEDS_Study_i (theStudy, _orb);
  SALOMEDS::Study_var st  = SALOMEDS::Study::_narrow(st_servant->_this());

  SALOMEDS::unlock();

  Engines::TMPFile_var aStream;
  CORBA::Boolean aValidScript = true; // VSR: maybe should be false by default ???

  if ( !CORBA::is_nil( _engine ) )
    aStream = _engine->DumpPython(st.in(), isPublished, isMultiFile, aValidScript);

  SALOMEDSImpl_TMPFile* aTMPFile = new Engines_TMPFile_i(aStream._retn());
  theStreamLength = aTMPFile->Size();
  isValidScript = aValidScript;

  SALOMEDS::lock();

  return aTMPFile;
}

//###############################################################################################################
//                                          SALOMEDS_DriverFactory
//###############################################################################################################

SALOMEDS_DriverFactory_i::SALOMEDS_DriverFactory_i(CORBA::ORB_ptr theORB)
{
  _orb = CORBA::ORB::_duplicate(theORB);
  _name_service = new SALOME_NamingService(_orb);
}


SALOMEDS_DriverFactory_i::~SALOMEDS_DriverFactory_i()
{
  delete _name_service;
}

SALOMEDSImpl_Driver* SALOMEDS_DriverFactory_i::GetDriverByType(const std::string& theComponentType)
{

  if ( theComponentType == SALOMEDSImpl_IParameters::getDefaultVisualComponent() )
    return NULL; // skip the "Interface Applicative" component

  SALOMEDSImpl_Driver* driver = 0;

  CORBA::Object_var obj = SALOME_LifeCycleCORBA(_name_service).FindOrLoad_Component("FactoryServer", theComponentType.c_str());

  if (!CORBA::is_nil(obj)) {
    Engines::EngineComponent_var anEngine = Engines::EngineComponent::_narrow(obj);
    driver = new SALOMEDS_Driver_i(anEngine, _orb);
  }
  else {
    // It can be "light" module
    obj = _name_service->Resolve("/Kernel/Session");
    if (!CORBA::is_nil(obj)) {
      SALOME::Session_var session = SALOME::Session::_narrow(obj);
      if (!CORBA::is_nil(session)) {
	Engines::EngineComponent_var anEngine = session->GetComponent(theComponentType.c_str());
	if (!CORBA::is_nil(anEngine))
	  driver = new SALOMEDS_Driver_i(anEngine, _orb);
      }
    }
  }

  return driver;
}

SALOMEDSImpl_Driver* SALOMEDS_DriverFactory_i::GetDriverByIOR(const std::string& theIOR)
{
  CORBA::Object_var obj;
  obj = _orb->string_to_object(theIOR.c_str());

  if (!CORBA::is_nil(obj)) {
    Engines::EngineComponent_var anEngine = Engines::EngineComponent::_narrow(obj);
    return new SALOMEDS_Driver_i(anEngine, _orb);
  }

  return NULL;
}
