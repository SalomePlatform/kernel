// Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
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

//  SALOME ResourcesCatalog : implementation of catalog resources parsing (SALOME_ModuleCatalog.idl)
//  File   : SALOME_ResourcesCatalog_Parser.hxx
//  Author : Estelle Deville
//  Module : SALOME
//$Header$
//
#ifndef SALOME_RESOURCES_CATALOG_PARSER
#define SALOME_RESOURCES_CATALOG_PARSER

#include "ResourcesManager_Defs.hxx"

#include <string>
#include <vector>
#include <list>
#include <map>
#include <iostream>

#ifdef WNT
#pragma warning(disable:4251) // Warning DLL Interface ...
#endif

enum AccessProtocolType {rsh, ssh, srun, pbsdsh, blaunch};

enum AccessModeType {interactive, batch};

enum BatchType {none, pbs, lsf, sge, ssh_batch, ccc, ll, slurm, vishnu};

enum MpiImplType {nompi, lam, mpich1, mpich2, openmpi, ompi, slurmmpi, prun};

class RESOURCESMANAGER_EXPORT ResourceDataToSort
  {

  public:
    std::string _Name;
    unsigned int _nbOfNodes;
    unsigned int _nbOfProcPerNode;
    unsigned int _CPUFreqMHz;
    unsigned int _memInMB;
    static unsigned int _nbOfProcWanted;
    static unsigned int _nbOfNodesWanted;
    static unsigned int _nbOfProcPerNodeWanted;
    static unsigned int _CPUFreqMHzWanted;
    static unsigned int _memInMBWanted;

  public:
    ResourceDataToSort();
    ResourceDataToSort(const std::string& name,
                       unsigned int nbOfNodes,
                       unsigned int nbOfProcPerNode,
                       unsigned int CPUFreqMHz,
                       unsigned int memInMB);
    bool operator< (const ResourceDataToSort& other) const;
    void Print() const;

  private:
    unsigned int GetNumberOfPoints() const;
  };

struct RESOURCESMANAGER_EXPORT ParserResourcesClusterMembersType
{
  std::string HostName;
  AccessProtocolType Protocol;
  AccessProtocolType ClusterInternalProtocol;
  std::string UserName;
  std::string AppliPath;
  ResourceDataToSort DataForSort;
};

struct RESOURCESMANAGER_EXPORT ParserResourcesType
{
  ResourceDataToSort DataForSort;
  std::string Name;
  std::string HostName;
  AccessProtocolType Protocol;
  AccessProtocolType ClusterInternalProtocol;
  AccessModeType Mode;
  BatchType Batch;
  MpiImplType mpi;
  std::string UserName;
  std::string AppliPath;
  std::vector<std::string> ComponentsList;
  std::vector<std::string> ModulesList;
  std::string OS;
  std::string batchQueue;
  std::string userCommands;
  std::string use;
  std::list<ParserResourcesClusterMembersType> ClusterMembersList;
  unsigned int nbOfProc;
  bool is_cluster_head;
  std::string working_directory;

  void Print();
  void Clear();

  static std::string protocolToString(AccessProtocolType protocol);
  static AccessProtocolType stringToProtocol(const std::string & protocolStr);

  std::string PrintAccessProtocolType() const;
  std::string PrintAccessModeType() const;
  std::string PrintBatchType() const;
  std::string PrintMpiImplType() const;
  std::string PrintClusterInternalProtocol() const;
};

typedef std::map<std::string, ParserResourcesType> MapOfParserResourcesType;
typedef std::map<std::string, ParserResourcesType>::iterator MapOfParserResourcesType_it;

#endif //SALOME_RESOURCES_CATALOG_PARSER
