// Copyright (C) 2009-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

// Author: André RIBES - EDF R&D
//
#ifndef _LAUNCHER_JOB_HXX_
#define _LAUNCHER_JOB_HXX_

#include "Launcher_Utils.hxx"
#include "ResourcesManager.hxx"

#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>

#include <string>
#include <list>
#include <map>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <exception>

#ifdef WITH_LIBBATCH
#include <libbatch/Job.hxx>
#include <libbatch/JobId.hxx>
#include <libbatch/GenericException.hxx>
#endif

namespace Launcher
{
  class LAUNCHER_EXPORT Job
  {
    public:
      Job();
      virtual ~Job();

      // Launcher managing parameters
      // State of a Job: CREATED, IN_PROCESS, QUEUED, RUNNING, PAUSED, FINISHED, ERROR
      void setState(const std::string & state);
      std::string getState() const;
	  // Get names or ids of hosts assigned to the job
      std::string getAssignedHostnames();

      void setNumber(const int & number);
      int getNumber();

      virtual void setResourceDefinition(const ParserResourcesType & resource_definition);
      ParserResourcesType getResourceDefinition() const;

      // Common parameters
      void setJobName(const std::string & job_name);
      virtual void setJobFile(const std::string & job_file);
      void setWorkDirectory(const std::string & work_directory);
      void setLocalDirectory(const std::string & local_directory);
      void setResultDirectory(const std::string & result_directory);
      void add_in_file(const std::string & file);
      void add_out_file(const std::string & file);
      void setMaximumDuration(const std::string & maximum_duration);
      void setResourceRequiredParams(const resourceParams & resource_required_params);
      void setQueue(const std::string & queue);
      void setEnvFile(const std::string & env_file);
      void setExclusive(bool exclusive);
      void setExclusiveStr(const std::string & exclusiveStr);
      void setMemPerCpu(unsigned long mem_per_cpu);
      void setWCKey(const std::string & wckey);
      void setExtraParams(const std::string & extra_params);
      void setReference(const std::string & reference);
	  // For COORM
	  void setLauncherFile(const std::string & launcher_file);
	  void setLauncherArgs(const std::string & launcher_args);

      std::string getJobName() const;
      std::string getJobFile() const;
      std::string getWorkDirectory() const;
      std::string getLocalDirectory() const;
      std::string getResultDirectory() const;
      const std::list<std::string> & get_in_files() const;
      const std::list<std::string> & get_out_files() const;
      std::string getMaximumDuration() const;
      resourceParams getResourceRequiredParams() const;
      std::string getQueue() const;
      std::string getEnvFile() const;
      std::string getJobType() const;
      bool getExclusive() const;
      std::string getExclusiveStr() const;
      unsigned long getMemPerCpu() const;
      std::string getWCKey() const;
      std::string getExtraParams() const;
      std::string getReference() const;

	  // For COORM
	  std::string getLauncherFile() const;
	  std::string getLauncherArgs() const;

      std::string updateJobState();

      void addSpecificParameter(const std::string & name,
                                  const std::string & value);
      const std::map<std::string, std::string> & getSpecificParameters() const;
      virtual void checkSpecificParameters();

      // Checks
      void checkMaximumDuration(const std::string & maximum_duration);
      void checkResourceRequiredParams(const resourceParams & resource_required_params);

      // Helps
      long convertMaximumDuration(const std::string & maximum_duration);
      std::string getLaunchDate() const;

      void stopJob();
      void removeJob();

      // Abstract class
      virtual void update_job() = 0;

    protected:
      int _number;

      std::string _job_type;

      std::string _state;
      std::string _assigned_hostnames; // Assigned hostnames
      std::string _launch_date;
      std::string _env_file;

      ParserResourcesType _resource_definition;

      std::string _job_name;
      std::string _job_file;
      std::string _job_file_name;
      std::string _job_file_name_complete;

      std::string _work_directory;
      std::string _local_directory;
      std::string _result_directory;
      std::list<std::string> _in_files;
      std::list<std::string> _out_files;
      std::map<std::string, std::string> _specific_parameters;
      std::string _maximum_duration;
      long _maximum_duration_in_second;
      resourceParams _resource_required_params;
      std::string _queue;
      bool _exclusive;
      unsigned long _mem_per_cpu;
      std::string _wckey;
      std::string _extra_params;
      std::string _reference; //! Reference of the job for the batch manager

	  // Parameters for COORM
	  std::string _launcher_file;
	  std::string _launcher_args;

#ifdef WITH_LIBBATCH
    // Connection with LIBBATCH
    public:      
      Batch::Job * getBatchJob();
      Batch::Parametre common_job_params();
      void setBatchManagerJobId(Batch::JobId batch_manager_job_id);
      Batch::JobId getBatchManagerJobId() const;

    protected:
      Batch::Job * _batch_job;
      Batch::JobId _batch_job_id;
#endif
  };
}

#endif
