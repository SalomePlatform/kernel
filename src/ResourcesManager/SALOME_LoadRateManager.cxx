#include "SALOME_LoadRateManager.hxx"

using namespace std;

string SALOME_LoadRateManager::FindBest(const Engines::MachineList& hosts)
{
  // for the moment then "maui" will be used for dynamic selection ...
  return string(hosts[0]);
}
