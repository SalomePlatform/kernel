#ifndef __SALOME_CONTAINERMANAGER_HXX__
#define __SALOME_CONTAINERMANAGER_HXX__

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_Component)
#include CORBA_CLIENT_HEADER(SALOME_ContainerManager)
#include "SALOME_ResourcesManager.hxx"
#include "SALOME_LoadRateManager.hxx"

#include <string>

class SALOME_NamingService;

#if defined CONTAINER_EXPORTS
#if defined WIN32
#define CONTAINER_EXPORT __declspec( dllexport )
#else
#define CONTAINER_EXPORT
#endif
#else
#if defined WNT
#define CONTAINER_EXPORT __declspec( dllimport )
#else
#define CONTAINER_EXPORT
#endif
#endif

class CONTAINER_EXPORT SALOME_ContainerManager:
  public POA_Engines::ContainerManager,
  public PortableServer::RefCountServantBase
{

public:
  SALOME_ContainerManager(CORBA::ORB_ptr orb);
  ~SALOME_ContainerManager();

  Engines::Container_ptr
  FindOrStartContainer(const Engines::MachineParameters& params,
		       const Engines::MachineList& possibleComputer);

  Engines::MachineList *
  GetFittingResources(const Engines::MachineParameters& params,
		      const char *componentName);

  char* FindBest(const Engines::MachineList& possibleComputers);

  void Shutdown();
  void ShutdownContainers();

  static const char *_ContainerManagerNameInNS;

private:
  Engines::Container_ptr
  FindContainer(const Engines::MachineParameters& params,
		const Engines::MachineList& possibleComputers);

  Engines::Container_ptr
  FindContainer(const Engines::MachineParameters& params,
		const char *theMachine);

  long GetIdForContainer(void);
  long _id;

  SALOME_ResourcesManager *_ResManager;
  SALOME_NamingService *_NS;
};

#endif
