//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
#include "SALOMEDS_Driver_i.hxx"
#include "SALOMEDS_TMPFile_i.hxx"
#include "utilities.h"
#include "SALOMEDS_SObject_i.hxx"
#include "SALOMEDS_SComponent_i.hxx"
#include "SALOMEDS_Study_i.hxx"
#include "SALOMEDS.hxx"
#include <stdlib.h>

using namespace std;  

SALOMEDS_Driver_i::~SALOMEDS_Driver_i()
{
}

SALOMEDSImpl_TMPFile* SALOMEDS_Driver_i::Save(const SALOMEDSImpl_SComponent& theComponent,
				      const string& theURL,
				      long& theStreamLength,
				      bool isMultiFile)
{  
  SALOMEDS::SComponent_var sco = SALOMEDS_SComponent_i::New (theComponent, _orb);
  CORBA::String_var url = CORBA::string_dup(theURL.c_str());

  SALOMEDS::unlock();
  SALOMEDS::TMPFile_var aStream = _driver->Save(sco.in(), url, isMultiFile);
  SALOMEDSImpl_TMPFile* aTMPFile = new SALOMEDS_TMPFile_i(aStream._retn());
  theStreamLength = aTMPFile->Size();
  SALOMEDS::lock();

  return aTMPFile;
}

SALOMEDSImpl_TMPFile* SALOMEDS_Driver_i::SaveASCII(const SALOMEDSImpl_SComponent& theComponent,
 					           const string& theURL,
					           long& theStreamLength,
					           bool isMultiFile)
{
  SALOMEDS::SComponent_var sco = SALOMEDS_SComponent_i::New (theComponent, _orb);
  CORBA::String_var url = CORBA::string_dup(theURL.c_str());

  SALOMEDS::unlock();
  SALOMEDS::TMPFile_var aStream = _driver->SaveASCII(sco.in(), url, isMultiFile);
  SALOMEDSImpl_TMPFile* aTMPFile = new SALOMEDS_TMPFile_i(aStream._retn());
  theStreamLength = aTMPFile->Size();
  SALOMEDS::lock();

  return aTMPFile;
}
  
bool SALOMEDS_Driver_i::Load(const SALOMEDSImpl_SComponent& theComponent,
			     const unsigned char* theStream,
			     const long theStreamLength,
			     const string& theURL,
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
  bool isOk = _driver->Load(sco.in(), aStream.in(), url, isMultiFile);
  SALOMEDS::lock();

  return isOk;
}

bool SALOMEDS_Driver_i::LoadASCII(const SALOMEDSImpl_SComponent& theComponent,
				  const unsigned char* theStream,
				  const long theStreamLength,
				  const string& theURL,
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
  bool isOk = _driver->LoadASCII(sco.in(), aStream.in(), url, isMultiFile);
  SALOMEDS::lock();

  return isOk;
}

void SALOMEDS_Driver_i::Close(const SALOMEDSImpl_SComponent& theComponent)
{
  SALOMEDS::SComponent_var sco = SALOMEDS_SComponent_i::New (theComponent, _orb);

  SALOMEDS::unlock();
  _driver->Close(sco.in());
  SALOMEDS::lock();
}



string SALOMEDS_Driver_i::IORToLocalPersistentID(const SALOMEDSImpl_SObject& theSObject,
						 const string& IORString,
					         bool isMultiFile,
						 bool isASCII)
{
  SALOMEDS::SObject_var so = SALOMEDS_SObject_i::New (theSObject, _orb);
  CORBA::String_var ior = CORBA::string_dup(IORString.c_str());

  SALOMEDS::unlock();
  CORBA::String_var pers_string =_driver->IORToLocalPersistentID(so.in(), ior.in(), isMultiFile, isASCII);
  SALOMEDS::lock();

  return string(pers_string);
}


string SALOMEDS_Driver_i::LocalPersistentIDToIOR(const SALOMEDSImpl_SObject& theObject,
					         const string& aLocalPersistentID,
					 	 bool isMultiFile,
						 bool isASCII)
{
  SALOMEDS::SObject_var so = SALOMEDS_SObject_i::New (theObject, _orb);
  CORBA::String_var pers_string = CORBA::string_dup(aLocalPersistentID.c_str());
  SALOMEDS::unlock();
  CORBA::String_var IOR = _driver->LocalPersistentIDToIOR(so.in(), pers_string.in(), isMultiFile, isASCII);
  SALOMEDS::lock();
  return string(IOR);
}

bool SALOMEDS_Driver_i::CanCopy(const SALOMEDSImpl_SObject& theObject)
{
  SALOMEDS::SObject_var so = SALOMEDS_SObject_i::New (theObject, _orb);

  SALOMEDS::unlock();
  bool isOk = _driver->CanCopy(so.in());
  so->Destroy();
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
  SALOMEDS::TMPFile_var aStream = _driver->CopyFrom(so.in(), anObjectID);
  SALOMEDSImpl_TMPFile* aTMPFile = new SALOMEDS_TMPFile_i(aStream._retn());
  theStreamLength = aTMPFile->Size();
  theObjectID = anObjectID;
  SALOMEDS::lock();

  return aTMPFile;
}

bool SALOMEDS_Driver_i::CanPaste(const string& theComponentName, int theObjectID)
{
  SALOMEDS::unlock();
  bool canPaste = _driver->CanPaste(theComponentName.c_str(), theObjectID);
  SALOMEDS::lock();
  return canPaste;
}

string SALOMEDS_Driver_i::PasteInto(const unsigned char* theStream,
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
  SALOMEDS::SObject_var ret_so = _driver->PasteInto(aStream.in(), theObjectID, so.in());
  SALOMEDS::lock();

  return string(ret_so->GetID());
}

SALOMEDSImpl_TMPFile* SALOMEDS_Driver_i::DumpPython(SALOMEDSImpl_Study* theStudy, 
					            bool isPublished, 
					            bool& isValidScript,
					            long& theStreamLength)
{
  SALOMEDS_Study_i *  st_servant = new SALOMEDS_Study_i (theStudy, _orb);
  SALOMEDS::Study_var st  = SALOMEDS::Study::_narrow(st_servant->_this());
  Engines::Component_ptr aComponent = Engines::Component::_narrow(_driver);

  SALOMEDS::unlock();
  CORBA::Boolean aValidScript, aPublished = isPublished;
  Engines::TMPFile_var aStream = aComponent->DumpPython(st.in(), aPublished, aValidScript);
  SALOMEDSImpl_TMPFile* aTMPFile = new Engines_TMPFile_i(aStream._retn());
  theStreamLength = aTMPFile->Size();
  isValidScript = aValidScript;
  SALOMEDS::lock();

  return aTMPFile;
}

//###############################################################################################################
//                                          SALOMEDS_DriverFactory
//###############################################################################################################

SALOMEDSImpl_Driver* SALOMEDS_DriverFactory_i::GetDriverByType(const string& theComponentType)
{
  CORBA::Object_var obj;

  string aFactoryType;
  if (theComponentType == "SUPERV") aFactoryType = "SuperVisionContainer";
  else aFactoryType = "FactoryServer";

  SALOMEDS::unlock();
  obj = SALOME_LifeCycleCORBA(_name_service).FindOrLoad_Component(aFactoryType.c_str(), theComponentType.c_str());
  SALOMEDS::lock();

  if (CORBA::is_nil(obj)) {
    obj = SALOME_LifeCycleCORBA(_name_service).FindOrLoad_Component("FactoryServerPy", theComponentType.c_str());
  }

  if (!CORBA::is_nil(obj)) {
    SALOMEDS::Driver_var aDriver = SALOMEDS::Driver::_narrow(obj);
    return new SALOMEDS_Driver_i(aDriver, _orb);
  }

  return NULL;
}

SALOMEDSImpl_Driver* SALOMEDS_DriverFactory_i::GetDriverByIOR(const string& theIOR)
{
  CORBA::Object_var obj;
  obj = _orb->string_to_object(theIOR.c_str());
 
  if (!CORBA::is_nil(obj)) {
    SALOMEDS::Driver_var aDriver = SALOMEDS::Driver::_narrow(obj);
    return new SALOMEDS_Driver_i(aDriver, _orb);
  }

  return NULL;
}
