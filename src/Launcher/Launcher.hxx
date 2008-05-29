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
#ifndef __LAUNCHER_HXX__
#define __LAUNCHER_HXX__

#include "Batch_BatchManager_eClient.hxx"
#include "ResourcesManager.hxx"

#include <string>

struct batchParams{
  std::string batch_directory;
  std::string expected_during_time;
  std::string mem;
  unsigned long nb_proc;
};

class LauncherException
{
public:
  const std::string msg;

  LauncherException(const std::string m) : msg(m) {}
};

class Launcher_cpp
{

public:
  Launcher_cpp();
  ~Launcher_cpp();

  long submitSalomeJob(const std::string fileToExecute ,
		       const std::vector<std::string>& filesToExport ,
		       const std::vector<std::string>& filesToImport ,
		       const batchParams& batch_params,
		       const machineParams& params) throw(LauncherException);

  std::string querySalomeJob( const long jobId, const machineParams& params) throw(LauncherException);
  void deleteSalomeJob( const long jobId, const machineParams& params) throw(LauncherException);
  void getResultSalomeJob( const std::string directory, const long jobId, const machineParams& params ) throw(LauncherException);

  void SetResourcesManager( ResourcesManager_cpp* rm ) { _ResManager = rm; }

protected:

  std::string buildSalomeCouplingScript(const string fileToExecute, const string dirForTmpFiles, const ParserResourcesType& params);
  MpiImpl *FactoryMpiImpl(MpiImplType mpiImpl) throw(LauncherException);
  Batch::BatchManager_eClient *FactoryBatchManager( const ParserResourcesType& params ) throw(LauncherException);
  std::string getTmpDirForBatchFiles();
  std::string getRemoteFile( std::string remoteDir, std::string localFile );
  std::string getHomeDir(const ParserResourcesType& p, const std::string & tmpdir);  

  std::map <std::string,Batch::BatchManager_eClient*> _batchmap;
  std::map < std::pair<std::string,long> , Batch::Job* > _jobmap;
  ResourcesManager_cpp *_ResManager;
  bool check(const batchParams& batch_params);
  long getWallTime(std::string edt);
  long getRamSize(std::string mem);
};

#endif
