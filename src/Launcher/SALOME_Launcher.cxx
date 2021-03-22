// Copyright (C) 2007-2021  CEA/DEN, EDF R&D, OPEN CASCADE
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
#include "Launcher_Job_CommandSALOME.hxx"

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
SALOME_Launcher::SALOME_Launcher(CORBA::ORB_ptr orb, PortableServer::POA_var poa)
{
  MESSAGE("SALOME_Launcher constructor");
  _NS = new SALOME_NamingService(orb);
  init(orb,poa);
  MESSAGE("SALOME_Launcher constructor end");
}

SALOME_Launcher::SALOME_Launcher(CORBA::ORB_ptr orb, PortableServer::POA_var poa, SALOME_NamingService_Abstract *externalNS):_NS(externalNS)
{
  init(orb,poa);
}

void SALOME_Launcher::init(CORBA::ORB_ptr orb, PortableServer::POA_var poa)
{
  _ResManager = new SALOME_ResourcesManager(orb,poa,tradNS());
  _l.SetResourcesManager(_ResManager->GetImpl());
  _ContManager = new SALOME_ContainerManager(orb,poa,tradNS());
  _ResManager->_remove_ref();
  _ContManager->_remove_ref();

  _orb = CORBA::ORB::_duplicate(orb) ;
  _poa = PortableServer::POA::_duplicate(poa) ;
  PortableServer::ObjectId_var id = _poa->activate_object(this);
  CORBA::Object_var obj = _poa->id_to_reference(id);
  Engines::SalomeLauncher_var refContMan = Engines::SalomeLauncher::_narrow(obj);
  _NS->Register(refContMan,_LauncherNameInNS);
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
  JobParameters_cpp cpp_parameters = JobParameters_CORBA2CPP(job_parameters);
  CORBA::Long jobNumber = -1;
  try
  {
    jobNumber = _l.createJob(cpp_parameters);
    std::ostringstream job_id;
    job_id << jobNumber;
    notifyObservers("NEW_JOB", job_id.str());
  }
  catch(const LauncherException &ex)
  {
    INFOS(ex.msg.c_str());
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(),SALOME::BAD_PARAM);
  }
  return jobNumber;
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
SALOME_Launcher::exportInputFiles(CORBA::Long job_id)
{
  try
  {
    _l.exportInputFiles(job_id);
  }
  catch(const LauncherException &ex)
  {
    INFOS(ex.msg.c_str());
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(),SALOME::BAD_PARAM);
  }
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

void
SALOME_Launcher::clearJobWorkingDir(CORBA::Long job_id)
{
  try
  {
    _l.clearJobWorkingDir(job_id);
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

char *
SALOME_Launcher::dumpJob(CORBA::Long job_id)
{
  std::string result;
  try
  {
    result = _l.dumpJob(job_id);
  }
  catch(const LauncherException &ex)
  {
    INFOS(ex.msg.c_str());
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(),SALOME::BAD_PARAM);
  }
  return CORBA::string_dup(result.c_str());
}

CORBA::Long
SALOME_Launcher::restoreJob(const char * dumpedJob)
{
  CORBA::Long jobId;
  try{
    jobId = _l.restoreJob(dumpedJob);
    if(jobId >= 0)
    {
      std::ostringstream job_str;
      job_str << jobId;
      notifyObservers("NEW_JOB", job_str.str());
    }
  }
  catch(const LauncherException &ex){
    INFOS(ex.msg.c_str());
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(),SALOME::INTERNAL_ERROR);
  }

  return jobId;
}

//=============================================================================
/*! CORBA Method:
 *  Create a job in the launcher with a file
 *  \param xmlExecuteFile     : .xml to parse that contains job description
 *  \param clusterName        : machine chosen
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
  if(!_NS)
    return;
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
  Engines::JobParameters_var job_parameters;
  try
  {
    JobParameters_cpp cpp_parameters = _l.getJobParameters(job_id);
    job_parameters = JobParameters_CPP2CORBA(cpp_parameters);
  }
  catch(const LauncherException &ex)
  {
    INFOS(ex.msg.c_str());
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(),SALOME::BAD_PARAM);
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

JobParameters_cpp
SALOME_Launcher::JobParameters_CORBA2CPP(
                                   const Engines::JobParameters& job_parameters)
{
  JobParameters_cpp result;

  result.job_name = job_parameters.job_name.in();
  result.job_type = job_parameters.job_type.in();
  result.job_file = job_parameters.job_file.in();
  result.pre_command = job_parameters.pre_command.in();
  result.env_file = job_parameters.env_file.in();

  result.in_files.clear();
  for (CORBA::ULong i = 0; i < job_parameters.in_files.length(); i++)
    result.in_files.push_back(job_parameters.in_files[i].in());
  result.out_files.clear();
  for (CORBA::ULong i = 0; i < job_parameters.out_files.length(); i++)
    result.out_files.push_back(job_parameters.out_files[i].in());

  result.work_directory = job_parameters.work_directory.in();
  result.local_directory = job_parameters.local_directory.in();
  result.result_directory = job_parameters.result_directory.in();
  result.maximum_duration = job_parameters.maximum_duration.in();

  result.resource_required = resourceParameters_CORBAtoCPP(job_parameters.resource_required);

  result.queue = job_parameters.queue.in();
  result.partition = job_parameters.partition.in();
  result.exclusive = job_parameters.exclusive;
  result.mem_per_cpu = job_parameters.mem_per_cpu;
  result.wckey = job_parameters.wckey.in();
  result.extra_params = job_parameters.extra_params.in();

  result.specific_parameters.clear();
  for (CORBA::ULong i = 0; i < job_parameters.specific_parameters.length(); i++)
    result.specific_parameters[job_parameters.specific_parameters[i].name.in()]
                             = job_parameters.specific_parameters[i].value.in();

  result.launcher_file = job_parameters.launcher_file.in();
  result.launcher_args = job_parameters.launcher_args.in();
  return result;
}

Engines::JobParameters_var
SALOME_Launcher::JobParameters_CPP2CORBA(const JobParameters_cpp& job_parameters)
{
  Engines::JobParameters_var result = new Engines::JobParameters;
  result->job_name = CORBA::string_dup(job_parameters.job_name.c_str());
  result->job_type = CORBA::string_dup(job_parameters.job_type.c_str());
  result->job_file = CORBA::string_dup(job_parameters.job_file.c_str());
  result->pre_command = CORBA::string_dup(job_parameters.pre_command.c_str());
  result->env_file = CORBA::string_dup(job_parameters.env_file.c_str());
  result->in_files.length((CORBA::ULong)job_parameters.in_files.size()); //!< TODO: conversion from size_t to CORBA::ULong

  int i = 0;
  for(const std::string& it : job_parameters.in_files)
  {
    result->in_files[i] = CORBA::string_dup(it.c_str());
    i++;
  }
  result->out_files.length((CORBA::ULong)job_parameters.out_files.size()); //!< TODO: conversion from size_t to CORBA::ULong
  i = 0;
  for(const std::string& it : job_parameters.out_files)
  {
    result->out_files[i] = CORBA::string_dup(it.c_str());
    i++;
  }

  result->work_directory = CORBA::string_dup(job_parameters.work_directory.c_str());
  result->local_directory = CORBA::string_dup(job_parameters.local_directory.c_str());
  result->result_directory = CORBA::string_dup(job_parameters.result_directory.c_str());
  result->maximum_duration = CORBA::string_dup(job_parameters.maximum_duration.c_str());

  result->resource_required = resourceParameters_CPPtoCORBA(job_parameters.resource_required);

  result->queue = CORBA::string_dup(job_parameters.queue.c_str());
  result->partition = CORBA::string_dup(job_parameters.partition.c_str());
  result->exclusive = job_parameters.exclusive;
  result->mem_per_cpu = job_parameters.mem_per_cpu;
  result->wckey = CORBA::string_dup(job_parameters.wckey.c_str());
  result->extra_params = CORBA::string_dup(job_parameters.extra_params.c_str());

  const std::map<std::string, std::string>& specific_parameters
                                       = job_parameters.specific_parameters;
  if (!specific_parameters.empty())
  {
    result->specific_parameters.length((CORBA::ULong)specific_parameters.size()); //!< TODO: conversion from size_t to CORBA::ULong
    CORBA::ULong i = 0;
    for (const auto& it_specific : specific_parameters)
    {
      Engines::Parameter_var new_param = new Engines::Parameter;
      new_param->name  = CORBA::string_dup(it_specific.first.c_str());
      new_param->value = CORBA::string_dup(it_specific.second.c_str());
      result->specific_parameters[i] = new_param;
      i++;
    }
  }

  result->launcher_file = CORBA::string_dup(job_parameters.launcher_file.c_str());
  result->launcher_args = CORBA::string_dup(job_parameters.launcher_args.c_str());
  return result;
}

static SALOME_Launcher *_launcher_singleton_ssl = nullptr;

#include "SALOME_Fake_NamingService.hxx"

SALOME_Launcher *KERNEL::getLauncherSA()
{
  if(!_launcher_singleton_ssl)
  {
    int argc(0);
    CORBA::ORB_var orb = CORBA::ORB_init(argc,nullptr);
    PortableServer::POA_var root_poa=PortableServer::POA::_the_root_poa();
    SALOME_Fake_NamingService *ns=new SALOME_Fake_NamingService(orb);
    _launcher_singleton_ssl = new SALOME_Launcher(orb,root_poa,ns);//3rd arg is important to skip NS !
  }
  return _launcher_singleton_ssl;
}
