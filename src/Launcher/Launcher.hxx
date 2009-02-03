//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
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
#ifndef __LAUNCHER_HXX__
#define __LAUNCHER_HXX__

#ifdef WIN32
# ifdef LAUNCHER_EXPORTS
#  define LAUNCHER_EXPORT __declspec(dllexport)
# else
#  define LAUNCHER_EXPORT __declspec(dllimport)
# endif
#else
# define LAUNCHER_EXPORT
#endif

#include <SALOME_ResourcesCatalog_Parser.hxx>
#include "ResourcesManager.hxx"
#include "SALOME_Launcher_Parser.hxx"

#include <string>
#include <vector>

class MpiImpl;

namespace Batch{
  class BatchManager_eClient;
  class Job;
}

struct batchParams{
  std::string batch_directory;
  std::string expected_during_time;
  std::string mem;
  unsigned long nb_proc;
};

class LAUNCHER_EXPORT LauncherException
{
public:
  const std::string msg;

  LauncherException(const std::string m) : msg(m) {}
};

class LAUNCHER_EXPORT Launcher_cpp
{

public:
  Launcher_cpp();
  ~Launcher_cpp();

  long submitJob(const std::string xmlExecuteFile,
		 const std::string clusterName) throw(LauncherException);

  long submitSalomeJob(const std::string fileToExecute ,
		       const std::vector<std::string>& filesToExport ,
		       const std::vector<std::string>& filesToImport ,
		       const batchParams& batch_params,
		       const machineParams& params) throw(LauncherException);

  std::string queryJob( const long jobId, const machineParams& params) throw(LauncherException);
  std::string queryJob( const long jobId, const std::string clusterName);
  void deleteJob( const long jobId, const machineParams& params) throw(LauncherException);
  void deleteJob( const long jobId, const std::string clusterName);
  void getResultsJob( const std::string directory, const long jobId, const machineParams& params ) throw(LauncherException);
  void getResultsJob( const std::string directory, const long jobId, const std::string clusterName );

  void SetResourcesManager( ResourcesManager_cpp* rm ) { _ResManager = rm; }

protected:

  std::string buildSalomeCouplingScript(const std::string fileToExecute, const std::string dirForTmpFiles, const ParserResourcesType& params);
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
  void ParseXmlFile(std::string xmlExecuteFile);

  //! will contain the informations on the data type catalog(after parsing)
  ParserLauncherType _launch;

};

#endif
