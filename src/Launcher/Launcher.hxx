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

#include <pthread.h>

class MpiImpl;

namespace Batch{
  class BatchManager;
  class Job;
}

struct batchParams{
  std::string batch_directory;
  std::string expected_during_time;
  std::string mem;
  unsigned long nb_proc;

  // Parameters for COORM
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
  void         launchJob(int job_id);
  const char * getJobState(int job_id);
  const char * getAssignedHostnames(int job_id); // Get names or ids of hosts assigned to the job
  void         getJobResults(int job_id, std::string directory);
  bool         getJobDumpState(int job_id, std::string directory);
  bool         getJobWorkFile(int job_id, std::string work_file, std::string directory);
  void         stopJob(int job_id);
  void         removeJob(int job_id);

  /*! Load the jobs from the file "jobs_file" and add them to the Launcher.
   *  Return a list with the IDs of the jobs that were successfully loaded.
   */
  std::list<int> loadJobs(const char* jobs_file);

  //! Save the jobs of the Launcher to the file "jobs_file".
  void saveJobs(const char* jobs_file);

  // Useful methods
  long createJobWithFile(std::string xmlExecuteFile, std::string clusterName);
  std::map<int, Launcher::Job *> getJobs();
  void createBatchManagerForJob(Launcher::Job * job);
  void addJobDirectlyToMap(Launcher::Job * new_job);

  // Lib methods
  void SetResourcesManager( ResourcesManager_cpp* rm ) {_ResManager = rm;}

  // Used by SALOME_Launcher
  ResourcesManager_cpp *_ResManager;
protected:

  virtual void notifyObservers(const std::string & event_name, const std::string & event_data) {}

  // Methods used by user interface methods
#ifdef WITH_LIBBATCH
  Batch::BatchManager *FactoryBatchManager(ParserResourcesType& params);
  std::map <int, Batch::BatchManager*> _batchmap;
#endif
  ParserLauncherType ParseXmlFile(std::string xmlExecuteFile);

  std::map <int, Launcher::Job *> _launcher_job_map;  
  int _job_cpt; // job number counter
  pthread_mutex_t * _job_cpt_mutex; // mutex for job counter
};

#endif
