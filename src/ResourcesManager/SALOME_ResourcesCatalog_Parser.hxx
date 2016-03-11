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

#ifdef WIN32
#pragma warning(disable:4251) // Warning DLL Interface ...
#endif

enum AccessProtocolType {sh, rsh, ssh, srun, pbsdsh, blaunch};

enum ResourceType {cluster, single_machine};

enum BatchType {none, pbs, lsf, sge, ccc, ll, slurm, vishnu, oar, coorm};

enum MpiImplType {nompi, lam, mpich1, mpich2, openmpi, ompi, slurmmpi, prun};

class RESOURCESMANAGER_EXPORT ResourcesException
{
public:
  const std::string msg;

  ResourcesException(const std::string m) : msg(m) {}
};

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

class RESOURCESMANAGER_EXPORT ParserResourcesType
{
public:
  ParserResourcesType();
  virtual ~ParserResourcesType();

  std::string getAccessProtocolTypeStr() const;
  std::string getResourceTypeStr() const;
  std::string getBatchTypeStr() const;
  std::string getMpiImplTypeStr() const;
  std::string getClusterInternalProtocolStr() const;
  std::string getCanLaunchBatchJobsStr() const;
  std::string getCanRunContainersStr() const;

  void setAccessProtocolTypeStr(const std::string & protocolTypeStr);
  void setResourceTypeStr(const std::string & resourceTypeStr);
  void setBatchTypeStr(const std::string & batchTypeStr);
  void setMpiImplTypeStr(const std::string & mpiImplTypeStr);
  void setClusterInternalProtocolStr(const std::string & internalProtocolTypeStr);
  void setCanLaunchBatchJobsStr(const std::string & canLaunchBatchJobsStr);
  void setCanRunContainersStr(const std::string & canRunContainersStr);

  ResourceDataToSort DataForSort;
  std::string Name;
  std::string HostName;
  AccessProtocolType Protocol;
  AccessProtocolType ClusterInternalProtocol;
  ResourceType type;
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
  std::list<ParserResourcesType> ClusterMembersList;
  unsigned int nbOfProc;
  bool can_launch_batch_jobs;
  bool can_run_containers;
  std::string working_directory;

protected:

  static std::string protocolToString(AccessProtocolType protocol);
  static AccessProtocolType stringToProtocol(const std::string & protocolStr);

  RESOURCESMANAGER_EXPORT friend std::ostream & operator<<(std::ostream &os,
                                                           const ParserResourcesType &rt);

};

typedef std::map<std::string, ParserResourcesType> MapOfParserResourcesType;
typedef std::map<std::string, ParserResourcesType>::iterator MapOfParserResourcesType_it;

#endif //SALOME_RESOURCES_CATALOG_PARSER
