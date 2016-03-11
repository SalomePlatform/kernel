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

#include "SALOME_ResourcesCatalog_Parser.hxx"
#include <iostream>
#include <sstream>

using namespace std;

#define NULL_VALUE 0

unsigned int ResourceDataToSort::_nbOfProcWanted = NULL_VALUE;
unsigned int ResourceDataToSort::_nbOfNodesWanted = NULL_VALUE;
unsigned int ResourceDataToSort::_nbOfProcPerNodeWanted = NULL_VALUE;
unsigned int ResourceDataToSort::_CPUFreqMHzWanted = NULL_VALUE;
unsigned int ResourceDataToSort::_memInMBWanted = NULL_VALUE;

ResourceDataToSort::ResourceDataToSort()
{}

ResourceDataToSort::ResourceDataToSort(const std::string& name,
                                       unsigned int nbOfNodes,
                                       unsigned int nbOfProcPerNode,
                                       unsigned int CPUFreqMHz,
                                       unsigned int memInMB):
    _Name(name),
    _nbOfNodes(nbOfNodes),
    _nbOfProcPerNode(nbOfProcPerNode),
    _CPUFreqMHz(CPUFreqMHz),
    _memInMB(memInMB)
{}

//! Method used by list::sort to sort the resources used in SALOME_ResourcesManager::GetResourcesFitting
bool ResourceDataToSort::operator< (const ResourceDataToSort& other) const
  {
    unsigned int nbPts = GetNumberOfPoints();
    return nbPts < other.GetNumberOfPoints();
  }

unsigned int ResourceDataToSort::GetNumberOfPoints() const
  {
    unsigned int ret = 0;
    //priority 0 : Nb of proc

    if (_nbOfProcWanted != NULL_VALUE)
      {
        unsigned int nb_proc = _nbOfNodes * _nbOfProcPerNode;
        if (nb_proc == _nbOfProcWanted)
          ret += 30000;
        else if (nb_proc > _nbOfProcWanted)
          ret += 20000;
        else
          ret += 10000;
      }

    //priority 1 : Nb of nodes

    if (_nbOfNodesWanted != NULL_VALUE)
      {
        if (_nbOfNodes == _nbOfNodesWanted)
          ret += 3000;
        else if (_nbOfNodes > _nbOfNodesWanted)
          ret += 2000;
        else
          ret += 1000;
      }

    //priority 2 : Nb of proc by node
    if (_nbOfProcPerNodeWanted != NULL_VALUE)
      {
        if (_nbOfProcPerNode == _nbOfProcPerNodeWanted)
          ret += 300;
        else if (_nbOfProcPerNode > _nbOfProcPerNodeWanted)
          ret += 200;
        else
          ret += 100;
      }

    //priority 3 : Cpu freq
    if (_CPUFreqMHzWanted != NULL_VALUE)
      {
        if (_CPUFreqMHz == _CPUFreqMHzWanted)
          ret += 30;
        else if (_CPUFreqMHz > _CPUFreqMHzWanted)
          ret += 20;
        else
          ret += 10;
      }

    //priority 4 : memory
    if (_memInMBWanted != NULL_VALUE)
      {
        if (_memInMB == _memInMBWanted)
          ret += 3;
        else if (_memInMB > _memInMBWanted)
          ret += 2;
        else
          ret += 1;
      }

    //RES_MESSAGE("[GetNumberOfPoints] points number for resource: " << _Name << " " << ret);
    return ret;
  }

//! Method used for debug
void ResourceDataToSort::Print() const
  {
    std::cout << _nbOfNodes << std::endl;
    std::cout << _nbOfProcPerNode << std::endl;
    std::cout << _CPUFreqMHz << std::endl;
    std::cout << _memInMB << std::endl;
  }


ParserResourcesType::ParserResourcesType()
: type(single_machine),
  Protocol(ssh),
  ClusterInternalProtocol(ssh),
  Batch(none),
  mpi(nompi),
  nbOfProc(1),
  can_launch_batch_jobs(false),
  can_run_containers(false)
{
  DataForSort._Name = "";
  DataForSort._nbOfNodes = 1;
  DataForSort._nbOfProcPerNode = 1;
  DataForSort._CPUFreqMHz = 0;
  DataForSort._memInMB = 0;
}

ParserResourcesType::~ParserResourcesType()
{
}

std::string ParserResourcesType::protocolToString(AccessProtocolType protocol)
{
  switch (protocol)
  {
  case sh:
    return "sh";
  case rsh:
    return "rsh";
  case ssh:
    return "ssh";
  case srun:
    return "srun";
  case pbsdsh:
    return "pbsdsh";
  case blaunch:
    return "blaunch";
  default:
    throw ResourcesException("Unknown protocol");
  }
}

AccessProtocolType ParserResourcesType::stringToProtocol(const std::string & protocolStr)
{
  if (protocolStr == "sh")
    return sh;
  else if (protocolStr == "rsh")
    return rsh;
  else if (protocolStr == "ssh")
    return ssh;
  else if (protocolStr == "srun")
    return srun;
  else if (protocolStr == "pbsdsh")
    return pbsdsh;
  else if (protocolStr == "blaunch")
    return blaunch;
  else
    throw ResourcesException((string("Unknown protocol ") + protocolStr).c_str());
}

ostream & operator<<(ostream &os, const ParserResourcesType &prt)
{
  os << "Name: " << prt.Name << endl <<
        "HostName: " << prt.HostName << endl <<
        "Type: " << prt.getResourceTypeStr() << endl <<
        "NbOfNodes: " << prt.DataForSort._nbOfNodes << endl <<
        "NbOfProcPerNode: " << prt.DataForSort._nbOfProcPerNode << endl <<
        "CPUFreqMHz: " << prt.DataForSort._CPUFreqMHz << endl <<
        "MemInMB: " << prt.DataForSort._memInMB << endl <<
        "Protocol: " << prt.getAccessProtocolTypeStr() << endl <<
        "ClusterInternalProtocol: " << prt.getClusterInternalProtocolStr() << endl <<
        "Batch: " << prt.getBatchTypeStr() << endl <<
        "mpi: " << prt.getMpiImplTypeStr() << endl <<
        "UserName: " << prt.UserName << endl <<
        "AppliPath: " << prt.AppliPath << endl <<
        "OS: " << prt.OS << endl <<
        "batchQueue: " << prt.batchQueue << endl <<
        "userCommands: " << prt.userCommands << endl <<
        "use: " << prt.use << endl <<
        "NbOfProc: " << prt.nbOfProc << endl <<
        "Can Launch Batch Jobs: " << prt.can_launch_batch_jobs << endl <<
        "Can Run Containers: " << prt.can_run_containers << endl <<
        "Working Directory: " << prt.working_directory << endl;

  for(unsigned int i=0 ; i<prt.ComponentsList.size() ; i++)
    os << "Component " << i+1 << " called: " << prt.ComponentsList[i] << endl;

  list<ParserResourcesType>::const_iterator it;
  for(it = prt.ClusterMembersList.begin() ; it != prt.ClusterMembersList.end() ; it++)
  {
    os << "Cluster member called: " << (*it).HostName << endl;
  }
  return os;
}

std::string
ParserResourcesType::getAccessProtocolTypeStr() const
{
  return protocolToString(Protocol);
}

std::string
ParserResourcesType::getClusterInternalProtocolStr() const
{
  return protocolToString(ClusterInternalProtocol);
}

std::string 
ParserResourcesType::getResourceTypeStr() const
{
  switch (type)
  {
  case cluster:
    return "cluster";
  case single_machine:
    return "single_machine";
  default:
    throw ResourcesException("Unknown resource type");
  }
}

std::string 
ParserResourcesType::getBatchTypeStr() const
{
  switch (Batch)
  {
  case none:
    return "none";
  case pbs:
    return "pbs";
  case lsf:
    return "lsf";
  case sge:
    return "sge";
  case ccc:
    return "ccc";
  case slurm:
    return "slurm";
  case ll:
    return "ll";
  case vishnu:
    return "vishnu";
  case oar:
    return "oar";
  case coorm:
    return "coorm";
  default:
    throw ResourcesException("Unknown batch type");
  }
}

std::string 
ParserResourcesType::getMpiImplTypeStr() const
{
  switch (mpi)
  {
  case nompi:
    return "no mpi";
  case lam:
    return "lam";
  case mpich1:
    return "mpich1";
  case mpich2:
    return "mpich2";
  case openmpi:
    return "openmpi";
  case ompi:
    return "ompi";
  case slurmmpi:
    return "slurmmpi";
  case prun:
    return "prun";
  default:
    throw ResourcesException("Unknown MPI implementation type");
  }
}

string ParserResourcesType::getCanLaunchBatchJobsStr() const
{
  return can_launch_batch_jobs ? "true" : "false";
}

string ParserResourcesType::getCanRunContainersStr() const
{
  return can_run_containers ? "true" : "false";
}

void ParserResourcesType::setAccessProtocolTypeStr(const string & protocolTypeStr)
{
  Protocol = stringToProtocol(protocolTypeStr);
}

void ParserResourcesType::setResourceTypeStr(const string & resourceTypeStr)
{
  if (resourceTypeStr == "cluster")
    type = cluster;
  else if (resourceTypeStr == "single_machine")
    type = single_machine;
  else
    throw ResourcesException((string("Unknown resource type ") + resourceTypeStr).c_str());
}

void ParserResourcesType::setBatchTypeStr(const string & batchTypeStr)
{
  if (batchTypeStr == "pbs")
    Batch = pbs;
  else if (batchTypeStr == "lsf")
    Batch = lsf;
  else if (batchTypeStr == "sge")
    Batch = sge;
  else if (batchTypeStr == "slurm")
    Batch = slurm;
  else if (batchTypeStr == "ccc")
    Batch = ccc;
  else if (batchTypeStr == "ll")
    Batch = ll;
  else if (batchTypeStr == "vishnu")
    Batch = vishnu;
  else if (batchTypeStr == "oar")
    Batch = oar;
  else if (batchTypeStr == "coorm")
    Batch = coorm;
  else if (batchTypeStr == "" || batchTypeStr == "none" || batchTypeStr == "ssh_batch")
    Batch = none;
  else
    throw ResourcesException((string("Unknown batch type ") + batchTypeStr).c_str());
}

void ParserResourcesType::setMpiImplTypeStr(const string & mpiImplTypeStr)
{
  if (mpiImplTypeStr == "lam")
    mpi = lam;
  else if (mpiImplTypeStr == "mpich1")
    mpi = mpich1;
  else if (mpiImplTypeStr == "mpich2")
    mpi = mpich2;
  else if (mpiImplTypeStr == "openmpi")
    mpi = openmpi;
  else if (mpiImplTypeStr == "ompi")
    mpi = ompi;
  else if (mpiImplTypeStr == "slurmmpi")
    mpi = slurmmpi;
  else if (mpiImplTypeStr == "prun")
    mpi = prun;
  else if (mpiImplTypeStr == "" || mpiImplTypeStr == "no mpi")
    mpi = nompi;
  else
    throw ResourcesException((string("Unknown MPI implementation type ") + mpiImplTypeStr).c_str());
}

void ParserResourcesType::setClusterInternalProtocolStr(const string & internalProtocolTypeStr)
{
  ClusterInternalProtocol = stringToProtocol(internalProtocolTypeStr);
}

void ParserResourcesType::setCanLaunchBatchJobsStr(const string & canLaunchBatchJobsStr)
{
  if (canLaunchBatchJobsStr == "true")
    can_launch_batch_jobs = true;
  else if (canLaunchBatchJobsStr == "false")
    can_launch_batch_jobs = false;
  else
    throw ResourcesException((string("Invalid boolean value for can_launch_batch_jobs: ") +
                            canLaunchBatchJobsStr).c_str());
}

void ParserResourcesType::setCanRunContainersStr(const string & canRunContainersStr)
{
  if (canRunContainersStr == "true")
    can_run_containers = true;
  else if (canRunContainersStr == "false")
    can_run_containers = false;
  else
    throw ResourcesException((string("Invalid boolean value for can_run_containers: ") +
                            canRunContainersStr).c_str());
}
