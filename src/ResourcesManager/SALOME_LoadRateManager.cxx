#include "SALOME_LoadRateManager.hxx"
#include "utilities.h"
#include <iostream>

using namespace std;

string SALOME_LoadRateManager::FindBest(const Engines::MachineList& hosts)
{
  // for the moment then "maui" will be used for dynamic selection ...
  MESSAGE("SALOME_LoadRateManager::FindBest " << hosts.length());

  if (hosts.length() == 0)
    return string("");

  return string(hosts[0]);
}
