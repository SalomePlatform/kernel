#ifndef __SALOME_CONTAINERMANAGER_HXX__
#define __SALOME_CONTAINERMANAGER_HXX__

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_Component)
#include CORBA_CLIENT_HEADER(SALOME_ContainerManager)
#include "SALOME_ResourcesManager.hxx"
#include "SALOME_LoadRateManager.hxx"

#include <string>

class SALOME_NamingService;

class SALOME_ContainerManager: public POA_Engines::ContainerManager,
			       public PortableServer::RefCountServantBase {
private:
  SALOME_ResourcesManager _ResManager;
  SALOME_LoadRateManager _LoadManager;
  SALOME_NamingService *_NS;
public:
  SALOME_ContainerManager(CORBA::ORB_ptr orb);
  ~SALOME_ContainerManager();
  Engines::Container_ptr FindOrStartContainer(const char *containerName, const Engines::MachineList& possibleComputer);
  Engines::MachineList *GetFittingResources(const Engines::MachineParameters& params, const char *componentName);
  char* FindBest(const Engines::MachineList& possibleComputers);
  void Shutdown();
  void ShutdownContainers();

  static const char *_ContainerManagerNameInNS;
private:
  Engines::Container_ptr FindContainer(const char *containerName,const Engines::MachineList& possibleComputers);
  Engines::Container_ptr FindContainer(const char *containerName,const char *theMachine);
  std::string BuildContainerNameInNS(const char *containerName,const char *machineName);
};

#endif
