//  Copyright (C) 2007-2010  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

#include "SALOME_ResourcesCatalog_Parser.hxx"
#include <iostream>
#include <sstream>

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

void ParserResourcesType::Print()
{
  std::ostringstream oss;
  oss << std::endl <<
    "Name : " << Name << std::endl <<
    "HostName : " << HostName << std::endl << 
    "NbOfNodes : " << DataForSort._nbOfNodes << std::endl <<
    "NbOfProcPerNode : " << DataForSort._nbOfProcPerNode << std::endl <<
    "CPUFreqMHz : " << DataForSort._CPUFreqMHz << std::endl <<
    "MemInMB : " << DataForSort._memInMB << std::endl <<
    "Protocol : " << Protocol << std::endl <<
    "ClusterInternalProtocol : " << ClusterInternalProtocol << std::endl <<
    "Mode : " << Mode << std::endl <<
    "Batch : " << Batch << std::endl <<
    "mpi : " << mpi << std::endl <<
    "UserName : " << UserName << std::endl <<
    "AppliPath : " << AppliPath << std::endl <<
    "OS : " << OS << std::endl <<
    "batchQueue : " << batchQueue << std::endl <<
    "userCommands : " << userCommands << std::endl <<
    "use : " << use << std::endl <<
    "NbOfProc : " << nbOfProc << std::endl <<
    "Modules : " << std::endl <<
    "Components : " << std::endl;

  for(unsigned int i=0;i<ComponentsList.size();i++)
    oss << "Component " << i+1 << " called : " << ComponentsList[i] << std::endl;

  
  std::list<ParserResourcesClusterMembersType>::iterator it;
  for(it = ClusterMembersList.begin(); 
      it != ClusterMembersList.end();
      it++)
  {
    oss << "Cluster member  called : " << (*it).HostName << std::endl;
  }
  std::cout << oss.str() << std::endl;
}

std::string
ParserResourcesType::PrintAccessProtocolType() const
{
  if (Protocol == rsh)
    return "rsh";
  else
    return "ssh";
}

std::string
ParserResourcesType::PrintClusterInternalProtocol() const
{
  if (ClusterInternalProtocol == rsh)
    return "rsh";
  else
    return "ssh";
}

std::string 
ParserResourcesType::PrintAccessModeType() const
{
  if (Mode == interactive)
    return "interactive";
  else
    return "batch";
}

std::string 
ParserResourcesType::PrintBatchType() const
{
  if (Batch == none)
    return "none";
  else if (Batch == pbs)
    return "pbs";
  else if (Batch == lsf)
    return "lsf";
  else if (Batch == sge)
    return "sge";
  else if (Batch == ccc)
    return "ccc";
  else 
    return "ssh";
}

std::string 
ParserResourcesType::PrintMpiImplType() const
{
  if (mpi == nompi)
    return "no mpi";
  else if (mpi == lam)
    return "lam";
  else if (mpi == mpich1)
    return "mpich1";
  else if (mpi == mpich2)
    return "mpich2";
  else if (mpi == openmpi)
    return "openmpi";
  else if (mpi == slurm)
    return "slurm";
  else
    return "prun";
}

void ParserResourcesType::Clear()
{
  Name = "";
  HostName = "";
  Protocol = rsh;
  ClusterInternalProtocol = rsh;
  Mode = interactive;
  Batch = none;
  mpi = nompi;
  UserName = "";
  AppliPath = "";
  batchQueue = "";
  userCommands = "";
  ComponentsList.clear();
  OS = "";
  use = "";
  ClusterMembersList.clear();
  nbOfProc = 1;

  DataForSort._Name = "";
  DataForSort._nbOfNodes = 1;
  DataForSort._nbOfProcPerNode = 1;
  DataForSort._CPUFreqMHz = 0;
  DataForSort._memInMB = 0;
}
