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
#define STUDYMANAGER_FACTORY "StudyManagerFactory"
#define STUDYMANAGER_CREATE  "CreateStudyManager"

typedef SALOMEDSClient_SObject* (*SOBJECT_FACTORY_FUNCTION) (SALOMEDS::SObject_ptr);
typedef SALOMEDSClient_SComponent* (*SCOMPONENT_FACTORY_FUNCTION) (SALOMEDS::SComponent_ptr);
typedef SALOMEDSClient_Study* (*STUDY_FACTORY_FUNCTION) (SALOMEDS::Study_ptr);
typedef SALOMEDSClient_StudyManager* (*STUDYMANAGER_FACTORY_FUNCTION) ();
typedef SALOMEDSClient_StudyManager* (*STUDYMANAGER_CREATE_FUNCTION) (CORBA::ORB_ptr, PortableServer::POA_ptr);

static SOBJECT_FACTORY_FUNCTION aSObjectFactory = NULL;
static SCOMPONENT_FACTORY_FUNCTION aSComponentFactory = NULL;
static STUDY_FACTORY_FUNCTION aStudyFactory = NULL;
static STUDYMANAGER_FACTORY_FUNCTION aManagerFactory = NULL;
static STUDYMANAGER_CREATE_FUNCTION aCreateFactory = NULL;

using namespace std;

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

