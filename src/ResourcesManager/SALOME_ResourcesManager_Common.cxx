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

#include "SALOME_ResourcesManager_Common.hxx"

using namespace std;

template <class T>
vector<string> strvec_CORBAtoCPP(const T & strvecCorba)
{
  vector<string> strvecCpp;
  for(unsigned int i=0; i<strvecCorba.length(); i++)
    strvecCpp.push_back(string(strvecCorba[i]));
  return strvecCpp;
}

template <class T>
typename T::_var_type strvec_CPPtoCORBA(const vector<string> & strvecCpp)
{
  typename T::_var_type strvecCorba = new T;
  strvecCorba->length(strvecCpp.size());
  for(unsigned int i=0;i<strvecCpp.size();i++)
    strvecCorba[i] = strvecCpp[i].c_str();
  return strvecCorba;
}

resourceParams resourceParameters_CORBAtoCPP(const Engines::ResourceParameters & params)
{
  resourceParams p;
  p.name = params.name;
  p.hostname = params.hostname;
  p.can_launch_batch_jobs = params.can_launch_batch_jobs;
  p.can_run_containers = params.can_run_containers;
  p.OS = params.OS;
  p.nb_proc = params.nb_proc;
  p.nb_node = params.nb_node;
  p.nb_proc_per_node = params.nb_proc_per_node;
  p.cpu_clock = params.cpu_clock;
  p.mem_mb = params.mem_mb;
  p.componentList = strvec_CORBAtoCPP<Engines::CompoList>(params.componentList);
  p.resourceList = resourceList_CORBAtoCPP(params.resList);
  return p;
}

Engines::ResourceParameters_var resourceParameters_CPPtoCORBA(const resourceParams & params)
{
  Engines::ResourceParameters_var p = new Engines::ResourceParameters;
  p->name = params.name.c_str();
  p->hostname = params.hostname.c_str();
  p->can_launch_batch_jobs = params.can_launch_batch_jobs;
  p->can_run_containers = params.can_run_containers;
  p->OS = params.OS.c_str();
  p->nb_proc = params.nb_proc;
  p->nb_node = params.nb_node;
  p->nb_proc_per_node = params.nb_proc_per_node;
  p->cpu_clock = params.cpu_clock;
  p->mem_mb = params.mem_mb;
  p->componentList = strvec_CPPtoCORBA<Engines::CompoList>(params.componentList);
  p->resList = resourceList_CPPtoCORBA(params.resourceList);
  return p;
}

vector<string> resourceList_CORBAtoCPP(const Engines::ResourceList & resList)
{
  return strvec_CORBAtoCPP<Engines::ResourceList>(resList);
}

Engines::ResourceList_var resourceList_CPPtoCORBA(const vector<string> & resList)
{
  return strvec_CPPtoCORBA<Engines::ResourceList>(resList);
}

ParserResourcesType resourceDefinition_CORBAtoCPP(const Engines::ResourceDefinition & resDef)
{
  ParserResourcesType resource;
  resource.Name = resDef.name;
  resource.HostName = resDef.hostname;
  resource.setResourceTypeStr(resDef.type.in());
  resource.OS = resDef.OS;
  resource.AppliPath = resDef.applipath;
  resource.DataForSort._memInMB = resDef.mem_mb;
  resource.DataForSort._CPUFreqMHz = resDef.cpu_clock;
  resource.DataForSort._nbOfNodes = resDef.nb_node;
  resource.DataForSort._nbOfProcPerNode = resDef.nb_proc_per_node;
  resource.UserName = resDef.username;
  resource.can_launch_batch_jobs = resDef.can_launch_batch_jobs;
  resource.can_run_containers = resDef.can_run_containers;
  resource.working_directory = resDef.working_directory;
  resource.setBatchTypeStr(resDef.batch.in());
  resource.setMpiImplTypeStr(resDef.mpiImpl.in());
  resource.setAccessProtocolTypeStr(resDef.protocol.in());
  resource.setClusterInternalProtocolStr(resDef.iprotocol.in());
  resource.ComponentsList = strvec_CORBAtoCPP<Engines::CompoList>(resDef.componentList);
  return resource;
}

Engines::ResourceDefinition_var resourceDefinition_CPPtoCORBA(const ParserResourcesType & resource)
{
  Engines::ResourceDefinition_var resCorba = new Engines::ResourceDefinition;
  resCorba->name = resource.Name.c_str();
  resCorba->hostname = resource.HostName.c_str();
  resCorba->type = resource.getResourceTypeStr().c_str();
  resCorba->protocol = resource.getAccessProtocolTypeStr().c_str();
  resCorba->iprotocol = resource.getClusterInternalProtocolStr().c_str();
  resCorba->username = resource.UserName.c_str();
  resCorba->applipath = resource.AppliPath.c_str();
  resCorba->componentList = strvec_CPPtoCORBA<Engines::CompoList>(resource.ComponentsList);
  resCorba->OS = resource.OS.c_str();
  resCorba->mem_mb = resource.DataForSort._memInMB;
  resCorba->cpu_clock = resource.DataForSort._CPUFreqMHz;
  resCorba->nb_proc_per_node = resource.DataForSort._nbOfProcPerNode;
  resCorba->nb_node = resource.DataForSort._nbOfNodes;
  resCorba->can_launch_batch_jobs = resource.can_launch_batch_jobs;
  resCorba->can_run_containers = resource.can_run_containers;
  resCorba->working_directory = resource.working_directory.c_str();
  resCorba->mpiImpl = resource.getMpiImplTypeStr().c_str();
  resCorba->batch = resource.getBatchTypeStr().c_str();
  return resCorba;
}
