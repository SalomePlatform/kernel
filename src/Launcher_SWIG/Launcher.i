// Copyright (C) 2019-2025  CEA, EDF
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
#include "LauncherResourceDefinition.hxx"

#include <sstream>

std::shared_ptr<ResourcesManager_cpp> HandleToLocalInstance(const std::string& ptrInStringFrmt)
{
  std::istringstream iss(ptrInStringFrmt);
  void *zePtr(nullptr);
  iss >> zePtr;
  std::shared_ptr<ResourcesManager_cpp> *effPtr = reinterpret_cast<std::shared_ptr<ResourcesManager_cpp> *>(zePtr);
  std::shared_ptr<ResourcesManager_cpp> ret(*effPtr);
  delete effPtr;
  return ret;
}
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
  std::string verbose_py_log_level;
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
%include <LauncherResourceDefinition.hxx>

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
  void WriteInXmlFile(std::string xml_file);
  void DeleteAllResourcesInCatalog();
%extend
{
  ResourceDefinition_cpp GetResourceDefinition(const std::string& name)
  {
    ResourceDefinition_cpp swig_result;
    ParserResourcesType cpp_result = $self->GetResourcesDescr(name);
    swig_result.fromPRT( cpp_result );
    return swig_result;
  }

  void DeleteResourceInCatalog(const std::string& name)
  {
    $self->DeleteResourceInCatalog(name.c_str());
  }
  
  void AddResourceInCatalog (const ResourceDefinition_cpp& new_resource)
  {
    ParserResourcesType new_resource_cpp( new_resource.toPRT() );
    $self->AddResourceInCatalog(new_resource_cpp);
  }

  void AddResourceInCatalogNoQuestion (const ResourceDefinition_cpp& new_resource)
  {
    ParserResourcesType new_resource_cpp( new_resource.toPRT() );
    $self->AddResourceInCatalogNoQuestion(new_resource_cpp);
  }
  
  void ParseXmlFiles()
  {
    $self->ParseXmlFiles();
  }
  
  std::vector<std::string> GetListOfEntries() const
  {
    const MapOfParserResourcesType& allRes = $self->GetList();
    std::vector<std::string> ret;
    for(auto it : allRes)
      ret.push_back(it.first);
    return ret;
  }
}
};

%inline
{
  std::shared_ptr<ResourcesManager_cpp> HandleToLocalInstance(const std::string& ptrInStringFrmt);
}

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
  void         exportInputFiles(int job_id);
  void         getJobResults(int job_id, std::string directory);
  void         clearJobWorkingDir(int job_id);
  bool         getJobDumpState(int job_id, std::string directory);
  bool         getJobWorkFile(int job_id, std::string work_file, std::string directory);
  long         getMaximumDurationInSecond(int job_id);
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

%pythoncode %{
def CreateSSHContainerResource(hostname,applipath,nbOfNodes=1):
  return CreateContainerResource(hostname,applipath,"ssh",nbOfNodes)

def CreateSRUNContainerResource(hostname,applipath,nbOfNodes=1):
  return CreateContainerResource(hostname,applipath,"srun",nbOfNodes)

def CreateContainerResource(hostname,applipath,protocol,nbOfNodes=1):
  import getpass
  ret = ResourceDefinition_cpp()
  ret.name = hostname.split(".")[0]
  ret.hostname = ret.name
  ret.protocol = protocol
  ret.applipath = applipath
  ret.nb_node = nbOfNodes
  ret.nb_proc_per_node = 1
  ret.can_run_containers = True
  ret.can_launch_batch_jobs = False
  ret.mpiImpl = "no mpi"
  ret.iprotocol = protocol
  ret.type = "single_machine"
  ret.username = getpass.getuser()
  return ret

def ResourceDefinition_cpp_repr(self):
  pat0 = "{} = {}"
  pat1 = "{} = \"{}\""
  data = [("name","name",pat0),
  ("hostname","hostname",pat0),
  ("type","type",pat0),
  ("protocol","protocol",pat0),
  ("userName","username",pat0),
  ("appliPath","applipath",pat1),
  ("mpi","mpiImpl",pat0),
  ("nbOfNodes","nb_node",pat0),
  ("nbOfProcPerNode","nb_proc_per_node",pat0),
  ("canRunContainer","can_run_containers",pat0)
  ]
  ret = [c.format(a,getattr(self,b)) for a,b,c in data]
  return "\n".join( ret )

def ResourcesManager_cpp_GetList(self):
  return {name:self.GetResourceDefinition(name) for name in self.GetListOfEntries()}

def ResourcesManager_cpp___getitem__(self,name):
  return self.GetResourceDefinition(name)

def ResourcesManager_cpp___repr__(self):
  return str( self.GetList() )

ListOfAttrCommon = ['name', 'OS', 'applipath', 'batch', 'can_launch_batch_jobs', 'can_run_containers', 'componentList', 'cpu_clock', 'hostname', 'iprotocol', 'mem_mb', 'mpiImpl', 'nb_node', 'nb_proc_per_node', 'protocol', 'type', 'username', 'working_directory']

def ResourceDefinition_cpp_isEqual(self,other):
  return all( [getattr(self,k) == getattr(other,k) for k in ListOfAttrCommon] )

def RetrieveRMCppSingleton():
  from salome.kernel import KernelLauncher
  return HandleToLocalInstance( KernelLauncher.RetrieveInternalInstanceOfLocalCppResourcesManager() )

def GetPlayGroundInsideASlurmJob():
  import subprocess as sp
  cont = sp.check_output(["srun","hostname"])
  nodesMul = [elt for elt in cont.decode().split("\n") if elt != ""]
  from collections import defaultdict
  d = defaultdict(int)
  for elt in nodesMul:
      d[elt]+=1
  return d

def BuildCatalogFromScratch(protocol,appliPath):
  import os
  d = GetPlayGroundInsideASlurmJob()
  rmcpp = RetrieveRMCppSingleton()
  rmcpp.DeleteAllResourcesInCatalog()
  for k,v in d.items():
      contRes = CreateContainerResource(hostname=k,applipath=appliPath,protocol=protocol,nbOfNodes=v)
      rmcpp.AddResourceInCatalogNoQuestion(contRes)

def GetRequestForGiveContainer(hostname, contName):
  from salome.kernel import Engines
  import os
  rp=Engines.ResourceParameters(name=hostname,
                                hostname=hostname,
                                can_launch_batch_jobs=False,
                                can_run_containers=True,
                                OS="Linux",
                                componentList=[],
                                nb_proc=1,
                                mem_mb=1000,
                                cpu_clock=1000,
                                nb_node=1,
                                nb_proc_per_node=1,
                                policy="first",
                                resList=[])

  cp=Engines.ContainerParameters(container_name=contName,
                                  mode="start",
                                  workingdir=os.path.expanduser("~"),
                                  nb_proc=1,
                                  isMPI=False,
                                  parallelLib="",
                                  resource_params=rp)
  return cp


def FromEngineResourceDefinitionToCPP( corbaInstance ):
  """
  Convert Engine.ResourceDefinition to ResourceDefinition_cpp instance
  """
  ret = ResourceDefinition_cpp()
  for k in ListOfAttrCommon:
    setattr(ret,k,getattr(corbaInstance,k))
  return ret

def ToEngineResourceDefinitionFromCPP( cppInstance ):
  from salome.kernel import Engines
  return Engines.ResourceDefinition(**{k:getattr(cppInstance,k) for k in ListOfAttrCommon})

ResourceDefinition_cpp.repr = ResourceDefinition_cpp_repr
ResourceDefinition_cpp.__repr__ = ResourceDefinition_cpp_repr
ResourceDefinition_cpp.__eq__ = ResourceDefinition_cpp_isEqual

ResourcesManager_cpp.GetList = ResourcesManager_cpp_GetList
ResourcesManager_cpp.__getitem__ = ResourcesManager_cpp___getitem__
ResourcesManager_cpp.__repr__ = ResourcesManager_cpp___repr__
%}

