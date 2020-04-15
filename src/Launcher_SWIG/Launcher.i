// Copyright (C) 2019-2020  CEA/DEN, EDF R&D
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

%module pylauncher

%{
#include "Launcher.hxx"
#include "ResourcesManager.hxx"

struct ResourceDefinition_cpp
{
public:
  std::string name;
  std::string hostname;
  std::string type;
  std::string protocol;
  std::string username;
  std::string applipath;
  std::string OS;
  int  mem_mb;
  int  cpu_clock;
  int  nb_node;
  int  nb_proc_per_node;
  std::string batch;
  std::string mpiImpl;
  std::string iprotocol;
  bool can_launch_batch_jobs;
  bool can_run_containers;
  std::string working_directory;
};
%}

%include "std_string.i"
%include "std_vector.i"
%include "std_list.i"
%include "std_map.i"

namespace std {
  %template(list_int) list<int>;
  %template(list_str) list<string>;
  %template(vector_str) vector<string>;
  %template(map_ss) map<string,string>;
};

// see ResourceParameters from SALOME_ResourcesManager.idl
// see resourceParams from ResourcesManager.hxx
%naturalvar JobParameters_cpp::componentList;
%naturalvar JobParameters_cpp::resourceList;
struct resourceParams
{
  resourceParams();

  std::string name;
  std::string hostname;
  bool can_launch_batch_jobs;
  bool can_run_containers;
  std::string OS;
  long nb_proc;
  long nb_node;
  long nb_proc_per_node;
  long cpu_clock;
  long mem_mb;
  std::vector<std::string> componentList;
  std::vector<std::string> resourceList;
};

// see JobParameters from SALOME_Launcher.idl
// see JobParameters_cpp from Launcher.hxx
%naturalvar JobParameters_cpp::in_files;
%naturalvar JobParameters_cpp::out_files;
%naturalvar JobParameters_cpp::specific_parameters;
struct JobParameters_cpp
{
public:
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

// see ResourceDefinition from SALOME_ResourcesManager.idl
// no other c++ equivalent. Convertion from ParserResourcesType
struct ResourceDefinition_cpp
{
public:
  std::string name;
  std::string hostname;
  std::string type;
  std::string protocol;
  std::string username;
  std::string applipath;
  std::string OS;
  int  mem_mb;
  int  cpu_clock;
  int  nb_node;
  int  nb_proc_per_node;
  std::string batch;
  std::string mpiImpl;
  std::string iprotocol;
  bool can_launch_batch_jobs;
  bool can_run_containers;
  std::string working_directory;
};

%exception
{
  try
  {
    $function
  }
  catch (ResourcesException& e)
  {
    SWIG_exception_fail(SWIG_RuntimeError, e.msg.c_str());
  }
  catch(...)
  {
    SWIG_exception_fail(SWIG_RuntimeError,"Unknown exception");
  }
}

%include <std_shared_ptr.i>
%shared_ptr(ResourcesManager_cpp)

class ResourcesManager_cpp
{
public:
  ResourcesManager_cpp(const char *xmlFilePath);
  std::vector<std::string> GetFittingResources(const resourceParams& params);
%extend
{
  ResourceDefinition_cpp GetResourceDefinition(const std::string& name)
  {
    ResourceDefinition_cpp swig_result;
    ParserResourcesType cpp_result = $self->GetResourcesDescr(name);

    swig_result.name = cpp_result.Name;
    swig_result.hostname = cpp_result.HostName;
    swig_result.type = cpp_result.getResourceTypeStr();
    swig_result.protocol = cpp_result.getAccessProtocolTypeStr();
    swig_result.username = cpp_result.UserName;
    swig_result.applipath = cpp_result.AppliPath;
    swig_result.OS = cpp_result.OS;
    swig_result.mem_mb = cpp_result.DataForSort._memInMB;
    swig_result.cpu_clock = cpp_result.DataForSort._CPUFreqMHz;
    swig_result.nb_node = cpp_result.DataForSort._nbOfNodes;
    swig_result.nb_proc_per_node = cpp_result.DataForSort._nbOfProcPerNode;
    swig_result.batch = cpp_result.getBatchTypeStr();
    swig_result.mpiImpl = cpp_result.getMpiImplTypeStr();
    swig_result.iprotocol = cpp_result.getClusterInternalProtocolStr();
    swig_result.can_launch_batch_jobs = cpp_result.can_launch_batch_jobs;
    swig_result.can_run_containers = cpp_result.can_run_containers;
    swig_result.working_directory = cpp_result.working_directory;

    return swig_result;
  }
}
};

%exception
{
  try
  {
    $function
  }
  catch (LauncherException& e)
  {
    SWIG_exception_fail(SWIG_RuntimeError, e.msg.c_str());
  }
  catch(...)
  {
    SWIG_exception_fail(SWIG_RuntimeError,"Unknown exception");
  }
}

class Launcher_cpp
{
public:
  Launcher_cpp();
  virtual ~Launcher_cpp();
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
  std::list<int> loadJobs(const char* jobs_file);
  void saveJobs(const char* jobs_file);
  long createJobWithFile(std::string xmlExecuteFile, std::string clusterName);
  void SetResourcesManager(std::shared_ptr<ResourcesManager_cpp>& rm );
};
