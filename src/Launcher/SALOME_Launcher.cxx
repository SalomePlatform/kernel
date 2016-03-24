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

#include "SALOME_Launcher.hxx"
#include "BatchTest.hxx"
#include "OpUtil.hxx"
#include "SALOME_ContainerManager.hxx"
#include "SALOME_NamingService.hxx"
#include "SALOME_ResourcesManager.hxx"
#include "Utils_CorbaException.hxx"


#include "Launcher_Job_Command.hxx"
#include "Launcher_Job_YACSFile.hxx"
#include "Launcher_Job_PythonSALOME.hxx"

#include "utilities.h"

#ifdef WIN32
# include <process.h>
#else
# include <unistd.h>
#endif
#include <sys/types.h>
#include <vector>
#include <list>

#include <stdio.h>
#include <sstream>

using namespace std;

const char *SALOME_Launcher::_LauncherNameInNS = "/SalomeLauncher";

//=============================================================================
/*! 
 *  Constructor
 *  \param orb
 */
//=============================================================================
SALOME_Launcher::SALOME_Launcher(CORBA::ORB_ptr orb, PortableServer::POA_var poa) : _l()
{
  MESSAGE("SALOME_Launcher constructor");
  _NS = new SALOME_NamingService(orb);
  _ResManager = new SALOME_ResourcesManager(orb,poa,_NS);
  _l.SetResourcesManager(_ResManager->GetImpl());
  _ContManager = new SALOME_ContainerManager(orb,poa,_NS);
  _ResManager->_remove_ref();
  _ContManager->_remove_ref();

  _orb = CORBA::ORB::_duplicate(orb) ;
  _poa = PortableServer::POA::_duplicate(poa) ;
  PortableServer::ObjectId_var id = _poa->activate_object(this);
  CORBA::Object_var obj = _poa->id_to_reference(id);
  Engines::SalomeLauncher_var refContMan = Engines::SalomeLauncher::_narrow(obj);

  _NS->Register(refContMan,_LauncherNameInNS);
  MESSAGE("SALOME_Launcher constructor end");
}

//=============================================================================
/*! 
 * destructor
 */
//=============================================================================
SALOME_Launcher::~SALOME_Launcher()
{
  MESSAGE("SALOME_Launcher destructor");
  delete _NS;
  MESSAGE("SALOME_Launcher destructor end");
}


CORBA::Long 
SALOME_Launcher::createJob(const Engines::JobParameters & job_parameters)
{
  std::string job_type = job_parameters.job_type.in();

  if (job_type != "command" && job_type != "yacs_file" && job_type != "python_salome")
  {
    std::string message("SALOME_Launcher::createJob: bad job type: ");
    message += job_type;
    THROW_SALOME_CORBA_EXCEPTION(message.c_str(), SALOME::INTERNAL_ERROR);
  }

  Launcher::Job * new_job; // It is Launcher_cpp that is going to destroy it

  if (job_type == "command")
    new_job = new Launcher::Job_Command();
  else if (job_type == "yacs_file")
    new_job = new Launcher::Job_YACSFile();
  else if (job_type == "python_salome")
    new_job = new Launcher::Job_PythonSALOME();

  // Name
  new_job->setJobName(job_parameters.job_name.in());

  // Directories
  std::string work_directory = job_parameters.work_directory.in();
  std::string local_directory = job_parameters.local_directory.in();
  std::string result_directory = job_parameters.result_directory.in();
  new_job->setWorkDirectory(work_directory);
  new_job->setLocalDirectory(local_directory);
  new_job->setResultDirectory(result_directory);

  // Parameters for COORM
  std::string launcher_file = job_parameters.launcher_file.in();
  std::string launcher_args = job_parameters.launcher_args.in();
  new_job->setLauncherFile(launcher_file);
  new_job->setLauncherArgs(launcher_args);

  // Job File
  std::string job_file = job_parameters.job_file.in();
  try
  {
    new_job->setJobFile(job_file);
  }
  catch(const LauncherException &ex)
  {
    INFOS(ex.msg.c_str());
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(),SALOME::INTERNAL_ERROR);
  }

  // Files
  std::string env_file = job_parameters.env_file.in();
  new_job->setEnvFile(env_file);
  for (CORBA::ULong i = 0; i < job_parameters.in_files.length(); i++)
    new_job->add_in_file(job_parameters.in_files[i].in());
  for (CORBA::ULong i = 0; i < job_parameters.out_files.length(); i++)
    new_job->add_out_file(job_parameters.out_files[i].in());

  // Expected During Time
  try
  {
    std::string maximum_duration = job_parameters.maximum_duration.in();
    new_job->setMaximumDuration(maximum_duration);
  }
  catch(const LauncherException &ex){
    INFOS(ex.msg.c_str());
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(),SALOME::INTERNAL_ERROR);
  }

  // Queue
  std::string queue = job_parameters.queue.in();
  new_job->setQueue(queue);

  // Exclusive
  new_job->setExclusive(job_parameters.exclusive);

  // Memory required per CPU
  new_job->setMemPerCpu(job_parameters.mem_per_cpu);

  // WC Key
  std::string wckey = job_parameters.wckey.in();
  new_job->setWCKey(wckey);

  // Extra params
  std::string extra_params = job_parameters.extra_params.in();
  new_job->setExtraParams(extra_params);

  // Resources requirements
  try
  {
    resourceParams p;
    p.name = job_parameters.resource_required.name;
    p.hostname = job_parameters.resource_required.hostname;
    p.OS = job_parameters.resource_required.OS;
    p.nb_proc = job_parameters.resource_required.nb_proc;
    p.nb_node = job_parameters.resource_required.nb_node;
    p.nb_proc_per_node = job_parameters.resource_required.nb_proc_per_node;
    p.cpu_clock = job_parameters.resource_required.cpu_clock;
    p.mem_mb = job_parameters.resource_required.mem_mb;
    new_job->setResourceRequiredParams(p);
  }
  catch(const LauncherException &ex){
    INFOS(ex.msg.c_str());
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(),SALOME::INTERNAL_ERROR);
  }

  // Adding specific parameters to the job
  for (CORBA::ULong i = 0; i < job_parameters.specific_parameters.length(); i++)
    new_job->addSpecificParameter(job_parameters.specific_parameters[i].name.in(),
                                  job_parameters.specific_parameters[i].value.in());
  try
  {
    new_job->checkSpecificParameters();
  }
  catch(const LauncherException &ex)
  {
    INFOS(ex.msg.c_str());
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(),SALOME::INTERNAL_ERROR);
  }

  try
  {
    _l.createJob(new_job);
    std::ostringstream job_id;
    job_id << new_job->getNumber();
    notifyObservers("NEW_JOB", job_id.str());
  }
  catch(const LauncherException &ex)
  {
    INFOS(ex.msg.c_str());
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(),SALOME::BAD_PARAM);
  }
  return new_job->getNumber();
}

void 
SALOME_Launcher::launchJob(CORBA::Long job_id)
{
  try
  {
    _l.launchJob(job_id);
  }
  catch(const LauncherException &ex)
  {
    INFOS(ex.msg.c_str());
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(),SALOME::BAD_PARAM);
  }
}

char *
SALOME_Launcher::getJobState(CORBA::Long job_id)
{
  std::string result;
  try
  {
    result = _l.getJobState(job_id);
  }
  catch(const LauncherException &ex)
  {
    INFOS(ex.msg.c_str());
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(),SALOME::BAD_PARAM);
  }
  return CORBA::string_dup(result.c_str());
}

// Get names or ids of hosts assigned to the job
char *
SALOME_Launcher::getAssignedHostnames(CORBA::Long job_id)
{
  std::string result;
  try
  {
    result = _l.getAssignedHostnames(job_id);
  }
  catch(const LauncherException &ex)
  {
    INFOS(ex.msg.c_str());
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(),SALOME::BAD_PARAM);
  }
  return CORBA::string_dup(result.c_str());
}

void
SALOME_Launcher::getJobResults(CORBA::Long job_id, const char * directory)
{
  try
  {
    _l.getJobResults(job_id, directory);
  }
  catch(const LauncherException &ex)
  {
    INFOS(ex.msg.c_str());
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(),SALOME::BAD_PARAM);
  }
}

CORBA::Boolean
SALOME_Launcher::getJobDumpState(CORBA::Long job_id, const char * directory)
{
  CORBA::Boolean rtn = false;
  try
  {
    rtn = _l.getJobDumpState(job_id, directory);
  }
  catch(const LauncherException &ex)
  {
    INFOS(ex.msg.c_str());
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(),SALOME::BAD_PARAM);
  }
  return rtn;
}

CORBA::Boolean
SALOME_Launcher::getJobWorkFile(CORBA::Long job_id, const char * work_file, const char * directory)
{
  CORBA::Boolean rtn = false;
  try
  {
    rtn = _l.getJobWorkFile(job_id, work_file, directory);
  }
  catch(const LauncherException &ex)
  {
    INFOS(ex.msg.c_str());
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(),SALOME::BAD_PARAM);
  }
  return rtn;
}

void 
SALOME_Launcher::removeJob(CORBA::Long job_id)
{
  try
  {
    _l.removeJob(job_id);
    std::ostringstream job_id_str;
    job_id_str << job_id;
    notifyObservers("REMOVE_JOB", job_id_str.str());
  }
  catch(const LauncherException &ex)
  {
    INFOS(ex.msg.c_str());
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(),SALOME::BAD_PARAM);
  }
}

void 
SALOME_Launcher::stopJob(CORBA::Long job_id)
{
  try
  {
    _l.stopJob(job_id);
    std::ostringstream job_id_str;
    job_id_str << job_id;
    notifyObservers("UPDATE_JOB_STATE", job_id_str.str());
  }
  catch(const LauncherException &ex)
  {
    INFOS(ex.msg.c_str());
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(),SALOME::BAD_PARAM);
  }
}

//=============================================================================
/*! CORBA Method:
 *  Create a job in the launcher with a file
 *  \param xmlExecuteFile     : .xml to parse that contains job description
 *  \param clusterName        : machine choosed
 */
//=============================================================================
CORBA::Long 
SALOME_Launcher::createJobWithFile(const char * xmlExecuteFile,
                                   const char * clusterName)
{
  CORBA::Long jobId;
  try{
    jobId = _l.createJobWithFile(xmlExecuteFile, clusterName);
  }
  catch(const LauncherException &ex){
    INFOS(ex.msg.c_str());
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(),SALOME::INTERNAL_ERROR);
  }

  return jobId;
}

//=============================================================================
/*! CORBA Method:
 *  the test batch configuration 
 *  \param params             : The batch cluster
 */
//=============================================================================
CORBA::Boolean 
SALOME_Launcher::testBatch(const Engines::ResourceParameters& params)
{
  MESSAGE("BEGIN OF SALOME_Launcher::testBatch");
  CORBA::Boolean rtn = false;
  try
  {
    // Consider only resources that can run batch jobs
    Engines::ResourceParameters new_params(params);
    new_params.can_launch_batch_jobs = true;

    // find a resource matching the required parameters
    Engines::ResourceList *aMachineList = _ResManager->GetFittingResources(new_params);
    if (aMachineList->length() == 0)
      throw SALOME_Exception("No resources have been found with your parameters");

    const Engines::ResourceDefinition* p = _ResManager->GetResourceDefinition((*aMachineList)[0]);
        std::string resource_name(p->name);
    INFOS("Choose resource for test: " <<  resource_name);
    
    BatchTest t(*p);
    if (t.test()) 
    {
      rtn = true;
    }
  }
  catch(const LauncherException &ex){
    INFOS(ex.msg.c_str());
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(),SALOME::INTERNAL_ERROR);
  }
  return rtn;
}

//=============================================================================
/*! CORBA method:
 *  shutdown all the containers, then the ContainerManager servant
 */
//=============================================================================
void SALOME_Launcher::Shutdown()
{
  MESSAGE("Shutdown");
  _NS->Destroy_Name(_LauncherNameInNS);
  _ContManager->Shutdown();
  _ResManager->Shutdown();
  PortableServer::ObjectId_var oid = _poa->servant_to_id(this);
  _poa->deactivate_object(oid);
  if(!CORBA::is_nil(_orb))
    _orb->shutdown(0);
}

//=============================================================================
/*! CORBA Method:
 *  Returns the PID of the process
 */
//=============================================================================
CORBA::Long SALOME_Launcher::getPID()
{
  return 
#ifndef WIN32
    (CORBA::Long)getpid();
#else
    (CORBA::Long)_getpid();
#endif
}

//=============================================================================
/*! CORBA Method:
 *  Returns current launcher jobs list
 */
//=============================================================================
Engines::JobsList *
SALOME_Launcher::getJobsList()
{
  Engines::JobsList_var jobs_list = new Engines::JobsList();
  std::map<int, Launcher::Job *> cpp_jobs = _l.getJobs();
  std::map<int, Launcher::Job *>::const_iterator it_job;
  int list_id = 0;
  for(it_job = cpp_jobs.begin(); it_job != cpp_jobs.end(); it_job++)
  {
    int number          = it_job->first;
    try
    {
      // Prepare CORBA job description
      Engines::JobDescription_var job_descr = new Engines::JobDescription();
      Engines::JobParameters_var job_parameters = getJobParameters(number);
      job_descr->job_id = number;
      job_descr->job_parameters = job_parameters;

      // Add job description to the sequence
      jobs_list->length(list_id + 1);
      jobs_list[list_id] = job_descr;
      list_id++;
    }
    catch (...) {}
  }
  return jobs_list._retn();
}

//=============================================================================
/*! CORBA Method:
 * Returns the job description
 */
//=============================================================================
Engines::JobParameters *
SALOME_Launcher::getJobParameters(CORBA::Long job_id)
{
  std::map<int, Launcher::Job *> cpp_jobs = _l.getJobs();
  std::map<int, Launcher::Job *>::const_iterator it_job = cpp_jobs.find(job_id);
  if (it_job == cpp_jobs.end())
  {
    INFOS("Cannot find the job, is it created ? job number: " << job_id);
    THROW_SALOME_CORBA_EXCEPTION("Job does not exist", SALOME::INTERNAL_ERROR);
  }

  Launcher::Job * job = it_job->second;
  Engines::JobParameters_var job_parameters = new Engines::JobParameters;
  job_parameters->job_name         = CORBA::string_dup(job->getJobName().c_str());
  job_parameters->job_type         = CORBA::string_dup(job->getJobType().c_str());
  job_parameters->job_file         = CORBA::string_dup(job->getJobFile().c_str());
  job_parameters->env_file         = CORBA::string_dup(job->getEnvFile().c_str());
  job_parameters->work_directory   = CORBA::string_dup(job->getWorkDirectory().c_str());
  job_parameters->local_directory  = CORBA::string_dup(job->getLocalDirectory().c_str());
  job_parameters->result_directory = CORBA::string_dup(job->getResultDirectory().c_str());

  // Parameters for COORM
  job_parameters->launcher_file = CORBA::string_dup(job->getLauncherFile().c_str());
  job_parameters->launcher_args = CORBA::string_dup(job->getLauncherArgs().c_str());

  int i = 0;
  int j = 0;
  std::list<std::string> in_files  = job->get_in_files();
  std::list<std::string> out_files = job->get_out_files();
  job_parameters->in_files.length(in_files.size());
  for(std::list<std::string>::iterator it = in_files.begin(); it != in_files.end(); it++)
  {
    job_parameters->in_files[i] = CORBA::string_dup((*it).c_str());
    i++;
  }
  job_parameters->out_files.length(out_files.size());
  for(std::list<std::string>::iterator it = out_files.begin(); it != out_files.end(); it++)
  {
    job_parameters->out_files[j] = CORBA::string_dup((*it).c_str());
    j++;
  }

  job_parameters->maximum_duration = CORBA::string_dup(job->getMaximumDuration().c_str());
  job_parameters->queue            = CORBA::string_dup(job->getQueue().c_str());
  job_parameters->exclusive        = job->getExclusive();
  job_parameters->mem_per_cpu      = job->getMemPerCpu();
  job_parameters->wckey            = CORBA::string_dup(job->getWCKey().c_str());
  job_parameters->extra_params     = CORBA::string_dup(job->getExtraParams().c_str());

  resourceParams resource_params = job->getResourceRequiredParams();
  job_parameters->resource_required.name             = CORBA::string_dup(resource_params.name.c_str());
  job_parameters->resource_required.hostname         = CORBA::string_dup(resource_params.hostname.c_str());
  job_parameters->resource_required.OS               = CORBA::string_dup(resource_params.OS.c_str());
  job_parameters->resource_required.nb_proc          = resource_params.nb_proc;
  job_parameters->resource_required.nb_node          = resource_params.nb_node;
  job_parameters->resource_required.nb_proc_per_node = resource_params.nb_proc_per_node;
  job_parameters->resource_required.cpu_clock        = resource_params.cpu_clock;
  job_parameters->resource_required.mem_mb           = resource_params.mem_mb;

  std::map<std::string, std::string> specific_parameters = job->getSpecificParameters();
  if (!specific_parameters.empty())
  {
    job_parameters->specific_parameters.length(specific_parameters.size());
    std::map<std::string, std::string>::const_iterator it_specific;
    CORBA::ULong i = 0;
    for (it_specific = specific_parameters.begin() ; it_specific != specific_parameters.end(); it_specific++)
    {
      Engines::Parameter_var new_param = new Engines::Parameter;
      new_param->name  = CORBA::string_dup((it_specific->first).c_str());
      new_param->value = CORBA::string_dup((it_specific->second).c_str());
      job_parameters->specific_parameters[i] = new_param;
      i++;
    }
  }

  return job_parameters._retn();
}

//=============================================================================
/*! CORBA Method:
 *  Loads jobs saved in jobs_file
 */
//=============================================================================
void
SALOME_Launcher::loadJobs(const char* jobs_file)
{
  list<int> new_jobs_id_list;
  try
  {
    // Load the jobs in Launcher
    new_jobs_id_list = _l.loadJobs(jobs_file);
  }
  catch (const LauncherException & ex)
  {
    INFOS(ex.msg.c_str());
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(), SALOME::INTERNAL_ERROR);
  }

  // Notify observers of the new jobs
  list<int>::const_iterator it_jobs_id;
  for (it_jobs_id = new_jobs_id_list.begin(); it_jobs_id != new_jobs_id_list.end(); it_jobs_id++)
  {
    ostringstream job_id_sstr;
    job_id_sstr << *it_jobs_id;
    notifyObservers("NEW_JOB", job_id_sstr.str());
  }
  notifyObservers("LOAD_JOBS", jobs_file);
}

//=============================================================================
/*! CORBA Method:
 *  Save jobs of Launcher (in any steps) in file jobs_file
 */
//=============================================================================
void
SALOME_Launcher::saveJobs(const char* jobs_file)
{
  _l.saveJobs(jobs_file);
  notifyObservers("SAVE_JOBS", jobs_file);
}

//=============================================================================
/*! CORBA Method:
 *  Add a new observer to the launcher
 */
//=============================================================================
void
SALOME_Launcher::addObserver(Engines::SalomeLauncherObserver_ptr observer)
{
  bool new_observer = true;
  std::list<Engines::SalomeLauncherObserver_var>::iterator iter = _observers.begin();
  while(iter != _observers.end())
  {
    if (std::string(_orb->object_to_string(*iter)) ==
        std::string(_orb->object_to_string(observer)))
    {
      new_observer = false;
      break;
    }
    iter++;
  }
  if (new_observer)
    _observers.push_back(Engines::SalomeLauncherObserver::_duplicate(observer));

  // We notify the new observer with all jobs that are currently in the Launcher
  std::map<int, Launcher::Job *> cpp_jobs = _l.getJobs();
  std::map<int, Launcher::Job *>::const_iterator it_job;
  for(it_job = cpp_jobs.begin(); it_job != cpp_jobs.end(); it_job++)
  {
    int number = it_job->first;
    std::ostringstream job_id;
    job_id << number;
    try
    {
      observer->notify("NEW_JOB", job_id.str().c_str());
    }
    catch (...) 
    {
       MESSAGE("Notify Observer, exception catch");
    }

  }
}

//=============================================================================
/*! CORBA Method:
 *  Add a new observer to the launcher
 */
//=============================================================================
void
SALOME_Launcher::removeObserver(Engines::SalomeLauncherObserver_ptr observer)
{
  std::list<Engines::SalomeLauncherObserver_var>::iterator iter = _observers.begin();
  while(iter != _observers.end())
  {
    if (std::string(_orb->object_to_string(*iter)) ==
        std::string(_orb->object_to_string(observer)))
    {
      // Observer found
      iter =_observers.erase(iter++);
    }
    else
    {
      iter++;
    }
  }
}

//=============================================================================
/*! Internal Method:
 *  Notify observers on a new event
 */
//=============================================================================
void
SALOME_Launcher::notifyObservers(const std::string & event_name,
                                 const std::string & event_data)
{
  std::list<Engines::SalomeLauncherObserver_var>::iterator iter = _observers.begin();
  while(iter != _observers.end())
  {
    try
    {
      (*iter)->notify(CORBA::string_dup(event_name.c_str()),
                      CORBA::string_dup(event_data.c_str()));
    }
    catch (...) 
    {
       MESSAGE("Notify Observer, exception catch");
    }
    iter++;
  }

}
