// Copyright (C) 2024-2025  CEA, EDF
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

#include "LauncherResourceDefinition.hxx"

void ResourceDefinition_cpp::fromPRT(const ParserResourcesType& other)
{
  this->name = other.Name;
  this->hostname = other.HostName;
  this->type = other.getResourceTypeStr();
  this->protocol = other.getAccessProtocolTypeStr();
  this->username = other.UserName;
  this->applipath = other.AppliPath;
  this->OS = other.OS;
  this->mem_mb = other.DataForSort._memInMB;
  this->cpu_clock = other.DataForSort._CPUFreqMHz;
  this->nb_node = other.DataForSort._nbOfNodes;
  this->nb_proc_per_node = other.DataForSort._nbOfProcPerNode;
  this->batch = other.getBatchTypeStr();
  this->mpiImpl = other.getMpiImplTypeStr();
  this->iprotocol = other.getClusterInternalProtocolStr();
  this->can_launch_batch_jobs = other.can_launch_batch_jobs;
  this->can_run_containers = other.can_run_containers;
  this->working_directory = other.working_directory;
}

ParserResourcesType ResourceDefinition_cpp::toPRT() const
{
  ParserResourcesType new_resource_cpp;
  new_resource_cpp.Name = this->name;
  new_resource_cpp.HostName = this->hostname;
  new_resource_cpp.setResourceTypeStr( this->type );
  new_resource_cpp.setAccessProtocolTypeStr( this->protocol );
  new_resource_cpp.UserName = this->username;
  new_resource_cpp.AppliPath = this->applipath;
  new_resource_cpp.OS = this->OS;
  new_resource_cpp.DataForSort._Name = this->name;
  new_resource_cpp.DataForSort._memInMB = this->mem_mb;
  new_resource_cpp.DataForSort._CPUFreqMHz = this->cpu_clock;
  new_resource_cpp.DataForSort._nbOfNodes = this->nb_node;
  new_resource_cpp.DataForSort._nbOfProcPerNode = this->nb_proc_per_node;
  new_resource_cpp.setBatchTypeStr(this->batch);
  new_resource_cpp.setMpiImplTypeStr(this->mpiImpl);
  new_resource_cpp.setClusterInternalProtocolStr(this->iprotocol);
  new_resource_cpp.can_launch_batch_jobs = this->can_launch_batch_jobs;
  new_resource_cpp.can_run_containers = this->can_run_containers;
  new_resource_cpp.working_directory = this->working_directory;
  return new_resource_cpp;
}
