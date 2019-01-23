// Copyright (C) 2019  CEA/DEN, EDF R&D
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
#include "SALOME_ResourcesCatalog_Parser.hxx"
#include "Launcher_Job.hxx"
#include "Launcher_Job_SALOME.hxx"
#include "Launcher_Job_YACSFile.hxx"
#include "Launcher.hxx"
#include "ResourcesManager.hxx"

static PyObject *convertJob(Launcher::Job *job, int owner)
{
  PyObject *ret(nullptr);
  if(!job)
    {
      Py_XINCREF(Py_None);
      return Py_None;
    }
  if(dynamic_cast<Launcher::Job_YACSFile *>(job))
    return SWIG_NewPointerObj((void*)dynamic_cast<Launcher::Job_YACSFile *>(job),SWIGTYPE_p_Launcher__Job_YACSFile,owner);
  throw LauncherException("Not recognized type of job on downcast !");
}

%}

%include std_string.i

%typemap(out) const std::list<std::string>&
{
  std::size_t i;
  std::list<std::string>::const_iterator iL;
  $result = PyList_New($1->size());
  for (i=0, iL=$1->cbegin(); iL!=$1->cend(); i++, iL++)
    PyList_SetItem($result,i,PyUnicode_FromString((*iL).c_str())); 
}

%typemap(out) const std::map<std::string, std::string> &
{
  $result = PyDict_New();
  for(std::map<std::string, std::string>::const_iterator iL=$1->cbegin();iL!=$1->cend();iL++)
    {
      PyObject *a(PyUnicode_FromString((*iL).first.c_str()));
      PyObject *b(PyUnicode_FromString((*iL).second.c_str()));
      PyDict_SetItem($result,a,b);
      Py_DECREF(a); Py_DECREF(b);
    }
}

%typemap(out) std::vector<std::string> *
{
  std::size_t i;
  std::vector<std::string>::const_iterator iL;
  $result = PyList_New($1->size());
  for (i=0, iL=$1->cbegin(); iL!=$1->cend(); i++, iL++)
    PyList_SetItem($result,i,PyUnicode_FromString((*iL).c_str())); 
}

%typemap(out) Launcher::Job *
{
  $result=convertJob($1,$owner);
}
   
%exception
{
  try {
    $function
      }
  catch(LauncherException& e)
    {
      SWIG_exception_fail(SWIG_RuntimeError,e.msg.c_str());
    }
  catch (...)
    {
      SWIG_exception_fail(SWIG_UnknownError, "Unknown");
    }
}

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

class ResourcesManager_cpp
{
 public:
  ResourcesManager_cpp(const char *xmlFilePath);
};

class ParserResourcesType
{
public:
  ParserResourcesType();
  ~ParserResourcesType();
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
};

%feature("unref") Launcher::Job "$this->decrRef();"
%feature("unref") Launcher::Job_SALOME "$this->decrRef();"
%feature("unref") Launcher::Job_YACSFile "$this->decrRef();"

namespace Launcher
{
  class Job
  {
    public:
      Job();
      virtual ~Job();
      void setState(const std::string & state);
      std::string getState() const;
      std::string getAssignedHostnames();
      void setNumber(const int & number);
      int getNumber();
      virtual void setResourceDefinition(const ParserResourcesType & resource_definition);
      ParserResourcesType getResourceDefinition() const;
      // Common parameters
      void setJobName(const std::string & job_name);
      virtual void setJobFile(const std::string & job_file);
      void setPreCommand(const std::string & preCommand);
      void setWorkDirectory(const std::string & work_directory);
      void setLocalDirectory(const std::string & local_directory);
      void setResultDirectory(const std::string & result_directory);
      void add_in_file(const std::string & file);
      void add_out_file(const std::string & file);
      void setMaximumDuration(const std::string & maximum_duration);
      void setResourceRequiredParams(const resourceParams & resource_required_params);
      void setQueue(const std::string & queue);
      void setPartition(const std::string & partition);
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
      std::string getPreCommand() const;
      std::string getWorkDirectory() const;
      std::string getLocalDirectory() const;
      std::string getResultDirectory() const;
      const std::list<std::string> & get_in_files() const;
      const std::list<std::string> & get_out_files() const;
      std::string getMaximumDuration() const;
      resourceParams getResourceRequiredParams() const;
      std::string getQueue() const;
      std::string getPartition() const;
      std::string getEnvFile() const;
      std::string getJobType() const;
      bool getExclusive() const;
      std::string getExclusiveStr() const;
      unsigned long getMemPerCpu() const;
      std::string getWCKey() const;
      std::string getExtraParams() const;
      std::string getReference() const;
      std::string getLauncherFile() const;
      std::string getLauncherArgs() const;
      std::string updateJobState();

      void addSpecificParameter(const std::string & name, const std::string & value);
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
      virtual void update_job() = 0;
  };

  class Job_SALOME : public Job
  {
  private:
    Job_SALOME();
  public:
    virtual ~Job_SALOME();
    virtual void setResourceDefinition(const ParserResourcesType & resource_definition);
    virtual void update_job();
#ifdef WITH_LIBBATCH
  public:
    std::string buildSalomeScript(Batch::Parametre params);
#endif
  };

  class Job_YACSFile : public Job_SALOME
  {
  public:
    Job_YACSFile();
    virtual ~Job_YACSFile();
    virtual void setJobFile(const std::string & job_file);
    virtual void checkSpecificParameters();
  };
}

class Launcher_cpp
{
public:
  Launcher_cpp();
  virtual ~Launcher_cpp();
  void         createJob(Launcher::Job * new_job);
  void         launchJob(int job_id);
  const char * getJobState(int job_id);
  const char * getAssignedHostnames(int job_id); // Get names or ids of hosts assigned to the job
  void         getJobResults(int job_id, std::string directory);
  void         clearJobWorkingDir(int job_id);
  bool         getJobDumpState(int job_id, std::string directory);
  bool         getJobWorkFile(int job_id, std::string work_file, std::string directory);
  void         stopJob(int job_id);
  void         removeJob(int job_id);
  std::string  dumpJob(int job_id);
  int restoreJob(const std::string& dumpedJob);
  std::list<int> loadJobs(const char* jobs_file);
  void saveJobs(const char* jobs_file);
  long createJobWithFile(std::string xmlExecuteFile, std::string clusterName);
  std::map<int, Launcher::Job *> getJobs();
  void addJobDirectlyToMap(Launcher::Job * new_job);
  void SetResourcesManager( ResourcesManager_cpp *rm );
  Launcher::Job * findJob(int job_id);
};

%pythoncode %{
def sendJobToSession(self, job_id, sessionId=None):
    """Send job specified by its job_id in self to a remote SALOME session.
    Doing so, it's possible to follow the job created locally into the JobManager module of the target SALOME session.
    SALOME session is specified by the file pointed by the content of OMNIORB_CONFIG environement var. The content of this var is called sessionId.
    If sessionId is let untouched, the current OMNIORB_CONFIG environement var is used.
    If this method fails to connect to the target SALOME session a RuntimeError exception will be thrown.
    """
    def job_type_traducer(jyf):
        dico = {'Job_YACSFile' : 'yacs_file'}
        st = type(jyf).__name__
        if st not in dico:
            raise RuntimeError("Not recognized type %s !"%st)
        return dico[st]
    #
    def resource_required_func(jyf):
        import Engines
        rp =jyf.getResourceRequiredParams()
        l12 = [('name', None), ('hostname', None), ('can_launch_batch_jobs', None), ('can_run_containers', None), ('OS', None), ('componentList', None), ('nb_proc', None), ('mem_mb', None), ('cpu_clock', None), ('nb_node', None), ('nb_proc_per_node', None), ('policy', lambda x: 'cycl'), ('resList', lambda x: x.resourceList)]
        kw2={}
        for a,b in l12:
            if a and not b:
                kw2[a]=getattr(rp,a)
            else:
                if a and b:
                    kw2[a]=b(rp)
        return Engines.ResourceParameters(**kw2)
    #
    filest = self.dumpJob(job_id);
    # Connect to SALOME session a retrieve its SalomeLauncher object
    import os
    if sessionId is not None:
      os.environ["OMNIORB_CONFIG"]=sessionId
    import Engines
    import orbmodule
    try:
      clt=orbmodule.client()
      sl = clt.Resolve("SalomeLauncher")
    except:
      raise RuntimeError("Fail to connect to the remote SALOME session.")
    # swig to CORBA translation
    # Job_YACSFile -> Engines.JobParameters and resourceParams -> Engines.ResourceParameters()
    l21= [('job_name', None), ('job_type', job_type_traducer), ('job_file', None), ('pre_command', None), ('env_file', None), ('in_files', lambda x: x.get_in_files()), ('out_files', lambda x: x.get_out_files()), ('work_directory', None), ('local_directory', None), ('result_directory', None), ('maximum_duration', None), ('resource_required',resource_required_func) , ('queue', None), ('partition', None), ('exclusive', None), ('mem_per_cpu', None), ('wckey', lambda x: x.getWCKey() ), ('extra_params', None), ('specific_parameters', lambda x: list(x.getSpecificParameters().items())), ('launcher_file', None), ('launcher_args', None)]
    kw={}
    jyf = self.findJob(job_id)
    for a,b in l21:
        if not b and a:
            kw[a]=eval("jyf.get%s()"%"".join(["%s%s"%(elt2[0].upper(),elt2[1:]) for elt2 in a.split("_")]),{"jyf" : jyf})
        else:
            if a and b:
                kw[a]=b(jyf)
    jyc = Engines.JobParameters(**kw)
    ########################
    bpc = sl.createJob(jyc)
    sl.restoreJob(filest)

Launcher_cpp.sendJobToSession = sendJobToSession
del sendJobToSession
%}
