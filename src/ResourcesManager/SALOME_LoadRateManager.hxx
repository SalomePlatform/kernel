#ifndef __SALOME_LOADRATEMANAGER_HXX__
#define __SALOME_LOADRATEMANAGER_HXX__

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_ContainerManager)
#include <string>

class SALOME_LoadRateManager
{
public:
  std::string FindBest(const Engines::MachineList& hosts);
};

#endif
