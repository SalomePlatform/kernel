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

#include "SALOMEDSClient_ClientFactory.hxx"

#ifdef WIN32
#include <windows.h>
static HMODULE _libHandle = 0;
#define SALOMEDS_LIB_NAME "SalomeDS.dll"
#else
#include <dlfcn.h>
static void* _libHandle = NULL;
#define SALOMEDS_LIB_NAME "libSalomeDS.so"
#endif

#define SOBJECT_FACTORY      "SObjectFactory"
#define SCOMPONENT_FACTORY   "SComponentFactory"
#define STUDY_FACTORY        "StudyFactory"
#define BUILDER_FACTORY      "BuilderFactory"
#define STUDYMANAGER_FACTORY "StudyManagerFactory"
#define STUDYMANAGER_CREATE  "CreateStudyManager"
#define GET_PARAMETERS       "GetIParameters"
#define CONVERT_SOBJECT      "ConvertSObject"
#define CONVERT_STUDY        "ConvertStudy"
#define CONVERT_BUILDER      "ConvertBuilder"

typedef SALOMEDSClient_SObject* (*SOBJECT_FACTORY_FUNCTION) (SALOMEDS::SObject_ptr);
typedef SALOMEDSClient_SComponent* (*SCOMPONENT_FACTORY_FUNCTION) (SALOMEDS::SComponent_ptr);
typedef SALOMEDSClient_Study* (*STUDY_FACTORY_FUNCTION) (SALOMEDS::Study_ptr);
typedef SALOMEDSClient_StudyManager* (*STUDYMANAGER_FACTORY_FUNCTION) ();
typedef SALOMEDSClient_StudyBuilder* (*BUILDER_FACTORY_FUNCTION) (SALOMEDS::StudyBuilder_ptr);
typedef SALOMEDSClient_StudyManager* (*STUDYMANAGER_CREATE_FUNCTION) (CORBA::ORB_ptr, PortableServer::POA_ptr);
typedef SALOMEDSClient_IParameters* (*GET_PARAMETERS_FACTORY) (const _PTR(AttributeParameter)&);
typedef SALOMEDS::SObject_ptr (*CONVERT_SOBJECT_FUNCTION) (const _PTR(SObject)&);
typedef SALOMEDS::Study_ptr (*CONVERT_STUDY_FUNCTION) (const _PTR(Study)&);
typedef SALOMEDS::StudyBuilder_ptr (*CONVERT_BUILDER_FUNCTION) (const _PTR(StudyBuilder)&);


static SOBJECT_FACTORY_FUNCTION aSObjectFactory = NULL;
static SCOMPONENT_FACTORY_FUNCTION aSComponentFactory = NULL;
static STUDY_FACTORY_FUNCTION aStudyFactory = NULL;
static BUILDER_FACTORY_FUNCTION aBuilderFactory = NULL;
static STUDYMANAGER_FACTORY_FUNCTION aManagerFactory = NULL;
static STUDYMANAGER_CREATE_FUNCTION aCreateFactory = NULL;
static GET_PARAMETERS_FACTORY aGetIParameters = NULL;
static CONVERT_SOBJECT_FUNCTION aConvertSObject = NULL;
static CONVERT_STUDY_FUNCTION aConvertStudy = NULL;
static CONVERT_BUILDER_FUNCTION aConvertBuilder = NULL;

_PTR(SObject) ClientFactory::SObject(SALOMEDS::SObject_ptr theSObject)
{
  SALOMEDSClient_SObject* so = NULL;

#ifdef WIN32
  if(!_libHandle) _libHandle = ::LoadLibrary(SALOMEDS_LIB_NAME);
  if(!aSObjectFactory) aSObjectFactory = (SOBJECT_FACTORY_FUNCTION)::GetProcAddress(_libHandle, SOBJECT_FACTORY);
#else
  if(!_libHandle) _libHandle = dlopen(SALOMEDS_LIB_NAME, RTLD_LAZY | RTLD_GLOBAL);
  if(!aSObjectFactory) aSObjectFactory = (SOBJECT_FACTORY_FUNCTION) dlsym(_libHandle, SOBJECT_FACTORY);
#endif

  if(aSObjectFactory) so = aSObjectFactory(theSObject); 
  return _PTR(SObject)(so);
}

_PTR(SComponent) ClientFactory::SComponent(SALOMEDS::SComponent_ptr theSComponent)
{
  SALOMEDSClient_SComponent* sco = NULL; 

#ifdef WIN32
  if(!_libHandle) _libHandle = ::LoadLibrary(SALOMEDS_LIB_NAME);
  if(!aSComponentFactory) aSComponentFactory = (SCOMPONENT_FACTORY_FUNCTION)::GetProcAddress(_libHandle, SCOMPONENT_FACTORY);
#else
  if(!_libHandle) _libHandle = dlopen(SALOMEDS_LIB_NAME, RTLD_LAZY | RTLD_GLOBAL);
  if(!aSComponentFactory) aSComponentFactory = (SCOMPONENT_FACTORY_FUNCTION) dlsym(_libHandle, SCOMPONENT_FACTORY);
#endif

  if(aSComponentFactory) sco = aSComponentFactory(theSComponent); 
  return _PTR(SComponent)(sco);
}

_PTR(Study) ClientFactory::Study(SALOMEDS::Study_ptr theStudy)
{
  SALOMEDSClient_Study* study = NULL;

#ifdef WIN32
  if(!_libHandle) _libHandle = ::LoadLibrary(SALOMEDS_LIB_NAME);
  if(!aStudyFactory) aStudyFactory = (STUDY_FACTORY_FUNCTION)::GetProcAddress(_libHandle, STUDY_FACTORY);
#else
  if(!_libHandle) _libHandle = dlopen(SALOMEDS_LIB_NAME, RTLD_LAZY | RTLD_GLOBAL);
  if(!aStudyFactory) aStudyFactory = (STUDY_FACTORY_FUNCTION) dlsym(_libHandle, STUDY_FACTORY);
#endif

  if(aStudyFactory) study = aStudyFactory(theStudy); 
  return _PTR(Study)(study);
}

_PTR(StudyBuilder) ClientFactory::StudyBuilder(SALOMEDS::StudyBuilder_ptr theStudyBuilder)
{
  SALOMEDSClient_StudyBuilder* studyBuilder = NULL;

#ifdef WIN32
  if(!_libHandle) _libHandle = ::LoadLibrary(SALOMEDS_LIB_NAME);
  if(!aBuilderFactory) aBuilderFactory = (BUILDER_FACTORY_FUNCTION)::GetProcAddress(_libHandle, BUILDER_FACTORY);
#else
  if(!_libHandle) _libHandle = dlopen(SALOMEDS_LIB_NAME, RTLD_LAZY | RTLD_GLOBAL);
  if(!aBuilderFactory) aBuilderFactory = (BUILDER_FACTORY_FUNCTION) dlsym(_libHandle, BUILDER_FACTORY);
#endif

  if(aBuilderFactory) studyBuilder = aBuilderFactory(theStudyBuilder); 
  return _PTR(StudyBuilder)(studyBuilder);
}

_PTR(StudyManager) ClientFactory::StudyManager()
{
  SALOMEDSClient_StudyManager* manager = NULL;

#ifdef WIN32
  if(!_libHandle) _libHandle = ::LoadLibrary(SALOMEDS_LIB_NAME);
  if(!aManagerFactory) aManagerFactory = (STUDYMANAGER_FACTORY_FUNCTION)::GetProcAddress(_libHandle, STUDYMANAGER_FACTORY);
#else
  if(!_libHandle) _libHandle = dlopen(SALOMEDS_LIB_NAME, RTLD_LAZY | RTLD_GLOBAL);
  if(!aManagerFactory) aManagerFactory = (STUDYMANAGER_FACTORY_FUNCTION) dlsym(_libHandle, STUDYMANAGER_FACTORY);
#endif

  if(aManagerFactory) manager = aManagerFactory(); 
  return _PTR(StudyManager)(manager);
}

_PTR(StudyManager) ClientFactory::createStudyManager(CORBA::ORB_ptr orb, PortableServer::POA_ptr poa)
{
  SALOMEDSClient_StudyManager* manager = NULL;

#ifdef WIN32
  if(!_libHandle) _libHandle = ::LoadLibrary(SALOMEDS_LIB_NAME);
  if(!aCreateFactory) aCreateFactory = (STUDYMANAGER_CREATE_FUNCTION)::GetProcAddress(_libHandle, STUDYMANAGER_CREATE);
#else
  if(!_libHandle) _libHandle = dlopen(SALOMEDS_LIB_NAME, RTLD_LAZY | RTLD_GLOBAL);
  if(!aCreateFactory) aCreateFactory = (STUDYMANAGER_CREATE_FUNCTION) dlsym(_libHandle, STUDYMANAGER_CREATE);
#endif

  if(aCreateFactory)  manager = aCreateFactory(orb, poa);
  return _PTR(StudyManager)(manager);
}

_PTR(IParameters) ClientFactory::getIParameters(const _PTR(AttributeParameter)& ap)
{
  SALOMEDSClient_IParameters* interface = NULL;

#ifdef WIN32
  if(!_libHandle) _libHandle = ::LoadLibrary(SALOMEDS_LIB_NAME);
  if(!aGetIParameters) aGetIParameters = (GET_PARAMETERS_FACTORY)::GetProcAddress(_libHandle, GET_PARAMETERS);
#else
  if(!_libHandle) _libHandle = dlopen(SALOMEDS_LIB_NAME, RTLD_LAZY | RTLD_GLOBAL);
  if(!aGetIParameters) aGetIParameters = (GET_PARAMETERS_FACTORY) dlsym(_libHandle, GET_PARAMETERS);
#endif

  if(aGetIParameters) interface = aGetIParameters(ap); 
  return _PTR(IParameters)(interface);
}

SALOMEDS::SObject_ptr ClientFactory::crbSObject(const _PTR(SObject)& theSObject)
{
  SALOMEDS::SObject_var so;

#ifdef WIN32
  if(!_libHandle) _libHandle = ::LoadLibrary(SALOMEDS_LIB_NAME);
  if(!aConvertSObject) aConvertSObject = (CONVERT_SOBJECT_FUNCTION)::GetProcAddress(_libHandle, CONVERT_SOBJECT);
#else
  if(!_libHandle) _libHandle = dlopen(SALOMEDS_LIB_NAME, RTLD_LAZY | RTLD_GLOBAL);
  if(!aConvertSObject) aConvertSObject = (CONVERT_SOBJECT_FUNCTION) dlsym(_libHandle, CONVERT_SOBJECT);
#endif

  if(aConvertSObject) so = aConvertSObject(theSObject); 

  if(CORBA::is_nil(so)) return SALOMEDS::SObject::_nil();
  return so._retn();
}


SALOMEDS::Study_ptr ClientFactory::crbStudy(const _PTR(Study)& theStudy)
{
  SALOMEDS::Study_var study;

#ifdef WIN32
  if(!_libHandle) _libHandle = ::LoadLibrary(SALOMEDS_LIB_NAME);
  if(!aConvertStudy) aConvertStudy = (CONVERT_STUDY_FUNCTION)::GetProcAddress(_libHandle, CONVERT_STUDY);
#else
  if(!_libHandle) _libHandle = dlopen(SALOMEDS_LIB_NAME, RTLD_LAZY | RTLD_GLOBAL);
  if(!aConvertStudy) aConvertStudy = (CONVERT_STUDY_FUNCTION) dlsym(_libHandle, CONVERT_STUDY);
#endif

  if(aConvertStudy) study = aConvertStudy(theStudy); 
  
  if(CORBA::is_nil(study)) return SALOMEDS::Study::_nil();
  return study._retn();
}

SALOMEDS::StudyBuilder_ptr ClientFactory::crbStudyBuilder(const _PTR(StudyBuilder)& theStudyBuilder)
{
  SALOMEDS::StudyBuilder_var studyBuilder;

#ifdef WIN32
  if(!_libHandle) _libHandle = ::LoadLibrary(SALOMEDS_LIB_NAME);
  if(!aConvertBuilder)aConvertBuilder  = (CONVERT_BUILDER_FUNCTION)::GetProcAddress(_libHandle, CONVERT_BUILDER);
#else
  if(!_libHandle) _libHandle = dlopen(SALOMEDS_LIB_NAME, RTLD_LAZY | RTLD_GLOBAL);
  if(!aConvertBuilder) aConvertBuilder = (CONVERT_BUILDER_FUNCTION) dlsym(_libHandle, CONVERT_BUILDER);
#endif

  if(aConvertBuilder) studyBuilder = aConvertBuilder(theStudyBuilder); 
  
  if(CORBA::is_nil(studyBuilder)) return SALOMEDS::StudyBuilder::_nil();
  return studyBuilder._retn();
}
