#ifndef __SALOME_LOADRATEMANAGER_HXX__
#define __SALOME_LOADRATEMANAGER_HXX__

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_ContainerManager)
#include <string>


#if defined RESOURCESMANAGER_EXPORTS
#if defined WIN32
#define RESOURCESMANAGER_EXPORT __declspec( dllexport )
#else
#define RESOURCESMANAGER_EXPORT
#endif
#else
#if defined WNT
#define RESOURCESMANAGER_EXPORT __declspec( dllimport )
#else
#define RESOURCESMANAGER_EXPORT
#endif
#endif

class RESOURCESMANAGER_EXPORT SALOME_LoadRateManager
  {

  public:
    std::string FindBest(const Engines::MachineList& hosts);
  };

#endif
