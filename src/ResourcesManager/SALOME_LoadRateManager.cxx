// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
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
#include "SALOME_LoadRateManager.hxx"
#include "utilities.h"
#include <iostream>
#include <map>

using namespace std;

string SALOME_LoadRateManager::FindFirst(const Engines::MachineList& hosts)
{
  MESSAGE("SALOME_LoadRateManager::FindFirst " << hosts.length());

  if (hosts.length() == 0)
    return string("");

  return string(hosts[0]);
}

string SALOME_LoadRateManager::FindNext(const Engines::MachineList& hosts,MapOfParserResourcesType& resList,SALOME_NamingService *ns)
{
  MESSAGE("SALOME_LoadRateManager::FindNext " << hosts.length());
  map<string, int> machines;

  if (hosts.length() == 0)
    return string("");

  for(int i=0;i<hosts.length();i++)
    machines[string(hosts[i])] = 0;

  ns->Change_Directory("/Containers");
  vector<string> vec = ns->list_directory_recurs();
  for(vector<string>::iterator iter = vec.begin();iter!=vec.end();iter++){
    CORBA::Object_var obj=ns->Resolve((*iter).c_str());
    Engines::Container_var cont=Engines::Container::_narrow(obj);
    if(!CORBA::is_nil(cont)){
      string mach = cont->getHostName();
      machines[mach]++;
    }
  }

  int imin = 0;
  ParserResourcesType resource = resList[string(hosts[0])];
  int nbproc = resource.DataForSort._nbOfProcPerNode * resource.DataForSort._nbOfNodes;
  int min = machines[string(hosts[0])]/nbproc;
  for(int i=1;i<hosts.length();i++){
    resource = resList[string(hosts[i])];
    nbproc = resource.DataForSort._nbOfProcPerNode * resource.DataForSort._nbOfNodes;
    if( machines[string(hosts[i])]/nbproc < min ){
      imin = i;
      min = machines[string(hosts[i])]/nbproc;
    }
  }

  return string(hosts[imin]);
}

string SALOME_LoadRateManager::FindBest(const Engines::MachineList& hosts) throw (SALOME_Exception)
{
  // for the moment then "maui" will be used for dynamic selection ...
  MESSAGE("SALOME_LoadRateManager::FindBest " << hosts.length());
  throw(SALOME_Exception(LOCALIZED("not yet implemented")));
  return string("");
}
