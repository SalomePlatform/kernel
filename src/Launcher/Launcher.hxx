// Copyright (C) 2007-2020  CEA/DEN, EDF R&D, OPEN CASCADE
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

#ifndef __LAUNCHER_HXX__
#define __LAUNCHER_HXX__

#include "Launcher_Utils.hxx"
#include "Launcher_Job.hxx"

#include "ResourcesManager.hxx"
#include <SALOME_ResourcesCatalog_Parser.hxx>

#include "SALOME_Launcher_Parser.hxx"

#include <string>
#include <vector>
#include <list>
#include <memory>

class MpiImpl;

namespace Batch{
  class BatchManager;
  class Job;
}

struct LAUNCHER_EXPORT JobParameters_cpp
{
  std::string job_name;
  std::string job_type;
  std::string job_file;
  std::string pre_command;
  std::string env_file;
  std::list<std::string> in_files;
  std::list<std::string> out_files;
  std::string work_directory;
  std::string local_directory;
  std::string result_directory;
  std::string maximum_duration;
  resourceParams resource_required;
  std::string queue;
  std::string partition;
  bool exclusive;
  unsigned int mem_per_cpu;
  std::string wckey;
  std::string extra_params;
  std::map<std::string, std::string> specific_parameters;
  std::string launcher_file;
  std::string launcher_args;
};

class LAUNCHER_EXPORT Launcher_cpp
{
public:
  Launcher_cpp();
  virtual ~Launcher_cpp();

  // Main interface
  void         createJob(Launcher::Job * new_job);
  int          createJob(const JobParameters_cpp& job_parameters);
  void         launchJob(int job_id);
  std::string  getJobState(int job_id);
  std::string  getAssignedHostnames(int job_id); // Get names or ids of hosts assigned to the job
  void         getJobResults(int job_id, std::string directory);
  void         clearJobWorkingDir(int job_id);
  bool         getJobDumpState(int job_id, std::string directory);
  bool         getJobWorkFile(int job_id, std::string work_file, std::string directory);
  void         stopJob(int job_id);
  void         removeJob(int job_id);
  std::string  dumpJob(int job_id);
  int restoreJob(const std::string& dumpedJob);
  JobParameters_cpp getJobParameters(int job_id);

  /*! Load the jobs from the file "jobs_file" and add them to the Launcher.
   *  Return a list with the IDs of the jobs that were successfully loaded.
   */
  std::list<int> loadJobs(const char* jobs_file);

  //! Save the jobs of the Launcher to the file "jobs_file".
  void saveJobs(const char* jobs_file);

  // Useful methods
  long createJobWithFile(std::string xmlExecuteFile, std::string clusterName);
  std::map<int, Launcher::Job *> getJobs();
  void addJobDirectlyToMap(Launcher::Job * new_job);
  Launcher::Job * findJob(int job_id);

  // Lib methods
  void SetResourcesManager( std::shared_ptr<ResourcesManager_cpp>& rm ) {_ResManager = rm;}

protected:

  // Used by SALOME_Launcher
  std::shared_ptr<ResourcesManager_cpp> _ResManager;

  virtual void notifyObservers(const std::string & event_name, const std::string & event_data) {}
  int addJob(Launcher::Job * new_job);

  // Methods used by user interface methods
#ifdef WITH_LIBBATCH
  Batch::BatchManager *FactoryBatchManager(ParserResourcesType& params);
  std::map <int, Batch::BatchManager*> _batchmap;
  Batch::BatchManager* getBatchManager(Launcher::Job * job);
#endif
  ParserLauncherType ParseXmlFile(std::string xmlExecuteFile);

  std::map <int, Launcher::Job *> _launcher_job_map;  
  int _job_cpt; // job number counter
};

#endif
