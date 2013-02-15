// Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
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
#include "Utils_CorbaException.hxx"


#include "Launcher_Job_Command.hxx"
#include "Launcher_Job_YACSFile.hxx"
#include "Launcher_Job_PythonSALOME.hxx"

#ifdef WIN32
# include <process.h>
#else
# include <unistd.h>
#endif
#include <sys/types.h>
#include <vector>

#include <libxml/parser.h>
#include <stdio.h>
#include <sstream>

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
  _ContManager = new SALOME_ContainerManager(orb,poa,_ResManager,_NS);
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
  // Step 1: check jobs_file read access
  FILE* xml_file = fopen(jobs_file, "r");
  if (xml_file == NULL)
  {
    std::string error = "Error opening jobs_file in SALOME_Launcher::loadJobs: " + std::string(jobs_file);
    INFOS(error);
    THROW_SALOME_CORBA_EXCEPTION(error.c_str(), SALOME::INTERNAL_ERROR);
  }

  // Step 2: read xml file
  xmlDocPtr doc = xmlReadFile(jobs_file, NULL, 0);
  if (doc == NULL)
  {
    std::string error = "Error in xmlReadFile in SALOME_Launcher::loadJobs, could not parse file: " + std::string(jobs_file);
    INFOS(error);
    fclose(xml_file);
    THROW_SALOME_CORBA_EXCEPTION(error.c_str(), SALOME::INTERNAL_ERROR);
  }

  // Step 3: Find jobs
  xmlNodePtr root_node = xmlDocGetRootElement(doc);
  xmlNodePtr xmlCurrentNode = root_node->xmlChildrenNode;
  if (!xmlStrcmp(root_node->name, xmlCharStrdup("jobs")))
  {
    while(xmlCurrentNode != NULL)
    {
      if (!xmlStrcmp(xmlCurrentNode->name, xmlCharStrdup("job")))
      {
        INFOS("A job is found");
        Launcher::Job * new_job; // It is Launcher_cpp that is going to destroy it
        xmlNodePtr job_node = xmlCurrentNode;

        // Begin Job
        if (!xmlHasProp(job_node, xmlCharStrdup("type"))  ||
            !xmlHasProp(job_node, xmlCharStrdup("name")))
        {
          INFOS("A bad job is found, type or name not found");
          break;
        }
        xmlChar* type = xmlGetProp(job_node, xmlCharStrdup("type"));
        xmlChar* name = xmlGetProp(job_node, xmlCharStrdup("name"));
        std::string job_type((const char*) type);
        if (job_type == "command")
          new_job = new Launcher::Job_Command();
        else if (job_type == "yacs_file")
          new_job = new Launcher::Job_YACSFile();
        else if (job_type == "python_salome")
          new_job = new Launcher::Job_PythonSALOME();
        new_job->setJobName(std::string((const char *)name));
        xmlFree(type);
        xmlFree(name);

        xmlNodePtr user_node = xmlFirstElementChild(job_node);
        xmlNodePtr run_node = xmlNextElementSibling(user_node);
        if (user_node == NULL || run_node == NULL)
        {
          INFOS("A bad job is found, user_part or run_part not found");
          delete new_job;
          break;
        }
        if (xmlStrcmp(user_node->name, xmlCharStrdup("user_part")) ||
            xmlStrcmp(run_node->name, xmlCharStrdup("run_part")))
        {
          INFOS("A bad job is found, cannot get a correct user_part or run_part node");
          delete new_job;
          break;
        }

        // Add user part

        // Get job_file env_file work_directory local_directory result_directory
        xmlNodePtr job_file_node         = xmlFirstElementChild(user_node);
        xmlNodePtr env_file_node         = xmlNextElementSibling(job_file_node);
        xmlNodePtr work_directory_node   = xmlNextElementSibling(env_file_node);
        xmlNodePtr local_directory_node  = xmlNextElementSibling(work_directory_node);
        xmlNodePtr result_directory_node = xmlNextElementSibling(local_directory_node);
        if (job_file_node         == NULL ||
            env_file_node         == NULL ||
            work_directory_node   == NULL ||
            local_directory_node  == NULL ||
            result_directory_node == NULL
           )
        {
          INFOS("A bad job is found, some user_part are not found");
          delete new_job;
          break;
        }
        if (xmlStrcmp(job_file_node->name,         xmlCharStrdup("job_file"))         ||
            xmlStrcmp(env_file_node->name,         xmlCharStrdup("env_file"))         ||
            xmlStrcmp(work_directory_node->name,   xmlCharStrdup("work_directory"))   ||
            xmlStrcmp(local_directory_node->name,  xmlCharStrdup("local_directory"))  ||
            xmlStrcmp(result_directory_node->name, xmlCharStrdup("result_directory"))
           )
        {
          INFOS("A bad job is found, some user part node are not in the rigth or does not have a correct name");
          delete new_job;
          break;
        }
        xmlChar* job_file         = xmlNodeGetContent(job_file_node);
        try
        {
          new_job->setJobFile(std::string((const char *)job_file));
        }
        catch(const LauncherException &ex)
        {
          INFOS("Exception receice for job_file, cannot add the job" << ex.msg.c_str());
          delete new_job;
          xmlFree(job_file);
          break;
        }
        xmlChar* env_file         = xmlNodeGetContent(env_file_node);
        xmlChar* work_directory   = xmlNodeGetContent(work_directory_node);
        xmlChar* local_directory  = xmlNodeGetContent(local_directory_node);
        xmlChar* result_directory = xmlNodeGetContent(result_directory_node);
        new_job->setEnvFile(std::string((const char *)env_file));
        new_job->setWorkDirectory(std::string((const char *)work_directory));
        new_job->setLocalDirectory(std::string((const char *)local_directory));
        new_job->setResultDirectory(std::string((const char *)result_directory));
        xmlFree(job_file);
        xmlFree(env_file);
        xmlFree(work_directory);
        xmlFree(local_directory);
        xmlFree(result_directory);

        // Get in and out files
        xmlNodePtr files_node = xmlNextElementSibling(result_directory_node);
        if (files_node == NULL)
        {
          INFOS("A bad job is found, user_part files is not found");
          delete new_job;
          break;
        }
        if (xmlStrcmp(files_node->name, xmlCharStrdup("files")))
        {
          INFOS("A bad job is found, files node are not in the rigth place or does not have a correct name or does not exist");
          delete new_job;
          break;
        }
        xmlNodePtr file_node = xmlFirstElementChild(files_node);
        while (file_node != NULL)
        {
          if (!xmlStrcmp(file_node->name, xmlCharStrdup("in_file")))
          {
            xmlChar* in_file = xmlNodeGetContent(file_node);
            new_job->add_in_file(std::string((const char *)in_file));
            xmlFree(in_file);
          }
          else if (!xmlStrcmp(file_node->name, xmlCharStrdup("out_file")))
          {
            xmlChar* out_file = xmlNodeGetContent(file_node);
            new_job->add_out_file(std::string((const char *)out_file));
            xmlFree(out_file);
          }
          file_node = xmlNextElementSibling(file_node);
        }

        // Get resource part
        xmlNodePtr res_node = xmlNextElementSibling(files_node);
        xmlNodePtr maximum_duration_node = xmlNextElementSibling(res_node);
        xmlNodePtr queue_node = xmlNextElementSibling(maximum_duration_node);
        if (res_node              == NULL ||
            maximum_duration_node == NULL ||
            queue_node            == NULL
           )
        {
          INFOS("A bad job is found, some user_part are not found");
          delete new_job;
          break;
        }
        if (xmlStrcmp(res_node->name,              xmlCharStrdup("resource_params"))  ||
            xmlStrcmp(maximum_duration_node->name, xmlCharStrdup("maximum_duration")) ||
            xmlStrcmp(queue_node->name,            xmlCharStrdup("queue"))
           )
        {
          INFOS("A bad job is found, some user part node are not in the rigth or does not have a correct name");
          delete new_job;
          break;
        }
        xmlChar* maximum_duration = xmlNodeGetContent(maximum_duration_node);
        try
        {
          new_job->setMaximumDuration(std::string((const char *)maximum_duration));
        }
        catch(const LauncherException &ex)
        {
          INFOS("Exception receice for maximum_duration, cannot add the job" << ex.msg.c_str());
          delete new_job;
          xmlFree(maximum_duration);
          break;
        }
        xmlChar* queue            = xmlNodeGetContent(queue_node);
        new_job->setQueue(std::string((const char *)queue));
        xmlFree(maximum_duration);
        xmlFree(queue);

        xmlNodePtr specific_node = xmlNextElementSibling(queue_node);
        if (specific_node == NULL)
        {
          INFOS("A bad job is found, specific_parameters part is not found");
          delete new_job;
          break;
        }
        xmlNodePtr parameter_node = xmlFirstElementChild(specific_node);
        while (parameter_node != NULL)
        {
          if (!xmlStrcmp(parameter_node->name, xmlCharStrdup("specific_parameter")))
          {
            xmlNodePtr name_node = xmlFirstElementChild(parameter_node);
            xmlNodePtr value_node = xmlNextElementSibling(name_node);
            if (name_node == NULL ||
                value_node == NULL)
            {
              INFOS("A bad job is found, specific_parameter parts are not found");
              delete new_job;
              break;
            }
            if (xmlStrcmp(name_node->name, xmlCharStrdup("name")) ||
                xmlStrcmp(value_node->name, xmlCharStrdup("value")))
            {
              INFOS("A bad job is found, specific_parameter bad parts are found");
              delete new_job;
              break;
            }

            xmlChar* name  = xmlNodeGetContent(name_node);
            xmlChar* value = xmlNodeGetContent(value_node);
            try
            {
              new_job->addSpecificParameter(std::string((const char*)name), std::string((const char*)value));
              xmlFree(name);
              xmlFree(value);
            }
            catch(const LauncherException &ex)
            {
              INFOS("Exception receice for a specific parameter, cannot add the job" << ex.msg.c_str());
              delete new_job;
              xmlFree(name);
              xmlFree(value);
              break;
            }
          }
          else
          {
            INFOS("A bad job is found, specific_parameters part is bad, a node that is not a specific parameter is found");
            delete new_job;
            break;
          }
          parameter_node = xmlNextElementSibling(parameter_node);
        }

        xmlNodePtr res_name_node             = xmlFirstElementChild(res_node);
        xmlNodePtr res_hostname_node         = xmlNextElementSibling(res_name_node);
        xmlNodePtr res_os_node               = xmlNextElementSibling(res_hostname_node);
        xmlNodePtr res_nb_proc_node          = xmlNextElementSibling(res_os_node);
        xmlNodePtr res_nb_node_node          = xmlNextElementSibling(res_nb_proc_node);
        xmlNodePtr res_nb_proc_per_node_node = xmlNextElementSibling(res_nb_node_node);
        xmlNodePtr res_cpu_clock_node        = xmlNextElementSibling(res_nb_proc_per_node_node);
        xmlNodePtr res_mem_mb_node           = xmlNextElementSibling(res_cpu_clock_node);
        if (res_name_node             == NULL ||
            res_hostname_node         == NULL ||
            res_os_node               == NULL ||
            res_nb_proc_node          == NULL ||
            res_nb_node_node          == NULL ||
            res_nb_proc_per_node_node == NULL ||
            res_cpu_clock_node        == NULL ||
            res_mem_mb_node           == NULL
           )
        {
          INFOS("A bad job is found, some resource_params user_part are not found");
          delete new_job;
          break;
        }
        if (xmlStrcmp(res_name_node->name,             xmlCharStrdup("name"))             ||
            xmlStrcmp(res_hostname_node->name,         xmlCharStrdup("hostname"))         ||
            xmlStrcmp(res_os_node->name,               xmlCharStrdup("OS"))               ||
            xmlStrcmp(res_nb_proc_node->name,          xmlCharStrdup("nb_proc"))          ||
            xmlStrcmp(res_nb_node_node->name,          xmlCharStrdup("nb_node"))          ||
            xmlStrcmp(res_nb_proc_per_node_node->name, xmlCharStrdup("nb_proc_per_node")) ||
            xmlStrcmp(res_cpu_clock_node->name,        xmlCharStrdup("cpu_clock"))        ||
            xmlStrcmp(res_mem_mb_node->name,           xmlCharStrdup("mem_mb"))
           )
        {
          INFOS("A bad job is found, some resource_params user_part node are not in the rigth or does not have a correct name");
          delete new_job;
          break;
        }
        xmlChar* res_name     = xmlNodeGetContent(res_name_node);
        xmlChar* res_hostname = xmlNodeGetContent(res_hostname_node);
        xmlChar* res_os       = xmlNodeGetContent(res_os_node);
        resourceParams p;
        p.name     = std::string((const char*) res_name);
        p.hostname = std::string((const char*) res_hostname);
        p.OS       = std::string((const char*) res_os);
        xmlFree(res_name);
        xmlFree(res_hostname);
        xmlFree(res_os);
        xmlChar* res_nb_proc          = xmlNodeGetContent(res_nb_proc_node);
        xmlChar* res_nb_node          = xmlNodeGetContent(res_nb_node_node);
        xmlChar* res_nb_proc_per_node = xmlNodeGetContent(res_nb_proc_per_node_node);
        xmlChar* res_cpu_clock        = xmlNodeGetContent(res_cpu_clock_node);
        xmlChar* res_mem_mb           = xmlNodeGetContent(res_mem_mb_node);
        bool import_value = true;
        std::istringstream nb_proc_stream((const char *) res_nb_proc);
        if (!(nb_proc_stream >> p.nb_proc))
          import_value = false;
        std::istringstream nb_node_stream((const char *) res_nb_node);
        if (!(nb_node_stream >> p.nb_node))
          import_value = false;
        std::istringstream nb_proc_per_node_stream((const char *) res_nb_proc_per_node);
        if (!(nb_proc_per_node_stream >> p.nb_proc_per_node))
          import_value = false;
        std::istringstream cpu_clock_stream((const char *) res_cpu_clock);
        if (!(cpu_clock_stream >> p.cpu_clock))
          import_value = false;
        std::istringstream mem_mb_stream((const char *) res_mem_mb);
        if (!(mem_mb_stream >> p.mem_mb))
          import_value = false;
        xmlFree(res_nb_proc);
        xmlFree(res_nb_node);
        xmlFree(res_nb_proc_per_node);
        xmlFree(res_cpu_clock);
        xmlFree(res_mem_mb);
        if (!import_value)
        {
          INFOS("A bad job is found, some resource_params value are not correct");
          delete new_job;
          break;
        }
        try
        {
          new_job->setResourceRequiredParams(p);
        }
        catch(const LauncherException &ex)
        {
          INFOS("A bad job is found, an error when inserting resource_params:" << ex.msg.c_str());
          delete new_job;
          break;
        }

        // We finally get run part to figure out what to do
        xmlNodePtr job_state_node             = xmlFirstElementChild(run_node);
        xmlNodePtr resource_choosed_name_node = xmlNextElementSibling(job_state_node);
        xmlNodePtr job_reference_node         = xmlNextElementSibling(resource_choosed_name_node);
        if (job_state_node             == NULL ||
            resource_choosed_name_node == NULL ||
            job_reference_node         == NULL
           )
        {
          INFOS("A bad job is found, some run_part are not found");
          delete new_job;
          break;
        }
        if (xmlStrcmp(job_state_node->name,             xmlCharStrdup("job_state"))             ||
            xmlStrcmp(resource_choosed_name_node->name, xmlCharStrdup("resource_choosed_name")) ||
            xmlStrcmp(job_reference_node->name,         xmlCharStrdup("job_reference"))
           )
        {
          INFOS("A bad job is found, some run_part nodes are not in the rigth or does not have a correct name");
          delete new_job;
          break;
        }
        xmlChar* job_state_xml             = xmlNodeGetContent(job_state_node);
        xmlChar* resource_choosed_name_xml = xmlNodeGetContent(resource_choosed_name_node);
        xmlChar* job_reference_xml         = xmlNodeGetContent(job_reference_node);
        std::string job_state((const char *) job_state_xml);
        std::string resource_choosed_name((const char *) resource_choosed_name_xml);
        std::string job_reference((const char *) job_reference_xml);
        xmlFree(job_state_xml);
        xmlFree(resource_choosed_name_xml);
        xmlFree(job_reference_xml);

        if (job_state == "CREATED")
        {
          // In this case, we ignore run_part informations
          try
          {
            _l.createJob(new_job);
            std::ostringstream job_id;
            job_id << new_job->getNumber();
            notifyObservers("NEW_JOB", job_id.str());
          }
          catch(const LauncherException &ex)
          {
            INFOS("Load failed: " << ex.msg.c_str());
          }
        }
        else if (job_state == "QUEUED"     ||
                 job_state == "RUNNING"    ||
                 job_state == "IN_PROCESS" ||
                 job_state == "PAUSED")
        {
          try
          {
            new_job->setState(job_state);
            _l.addJobDirectlyToMap(new_job, job_reference);

            // Step 4: We check that the BatchManager could resume
            // the job
#ifdef WITH_LIBBATCH
            if (new_job->getBatchManagerJobId().getReference() != job_reference)
            {
              INFOS("BatchManager type cannot resume a job - job state is set to ERROR");
              new_job->setState("ERROR");
            }
#endif
            std::ostringstream job_id;
            job_id << new_job->getNumber();
            notifyObservers("NEW_JOB", job_id.str());
          }
          catch(const LauncherException &ex)
          {
            INFOS("Cannot load the job! Exception: " << ex.msg.c_str());
            delete new_job;
          }
        }
        else if (job_state == "FINISHED" ||
                 job_state == "FAILED"   ||
                 job_state == "ERROR")
        {
          try
          {
            // Step 2: We add run_part informations
            new_job->setState(job_state);
            _l.addJobDirectlyToMap(new_job, job_reference);
            std::ostringstream job_id;
            job_id << new_job->getNumber();
            notifyObservers("NEW_JOB", job_id.str());
          }
          catch(const LauncherException &ex)
          {
            INFOS("Cannot load the job! Exception: " << ex.msg.c_str());
            delete new_job;
          }
        }
        else
        {
          INFOS("A bad job is found, state unknown " << job_state);
          delete new_job;
        }

      }
      xmlCurrentNode = xmlCurrentNode->next;
    }
  }
  else
  {
    xmlFreeDoc(doc);
    fclose(xml_file);
    std::string error = "Error in xml file, could not find root_node named jobs: " + std::string(jobs_file);
    INFOS(error);
    THROW_SALOME_CORBA_EXCEPTION(error.c_str(), SALOME::INTERNAL_ERROR);
  }

  // Clean
  xmlFreeDoc(doc);
  fclose(xml_file);
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

  // Step 1: check jobs_file write access
  FILE* xml_file = fopen(jobs_file, "w");
  if (xml_file == NULL)
  {
    std::string error = "Error opening jobs_file in SALOME_Launcher::saveJobs: " + std::string(jobs_file);
    INFOS(error);
    THROW_SALOME_CORBA_EXCEPTION(error.c_str(), SALOME::INTERNAL_ERROR);
  }

  // Step 2: First lines
  xmlKeepBlanksDefault(0);
  xmlDocPtr doc = xmlNewDoc(xmlCharStrdup("1.0"));
  xmlNodePtr root_node = xmlNewNode(NULL, xmlCharStrdup("jobs"));
  xmlDocSetRootElement(doc, root_node);
  xmlNodePtr doc_comment = xmlNewDocComment(doc, xmlCharStrdup("SALOME Launcher save jobs file"));
  xmlAddPrevSibling(root_node, doc_comment);

  // Step 3: For each job write it on the xml document
  // We could put a mutex but are not foing to do that currently
  std::map<int, Launcher::Job *> jobs_list = _l.getJobs();
  std::map<int, Launcher::Job *>::const_iterator it_job;
  for(it_job = jobs_list.begin(); it_job != jobs_list.end(); it_job++)
  {
    it_job->second->addToXmlDocument(root_node);
  }

  // Final step: write file
  int isOk = xmlSaveFormatFile(jobs_file, doc, 1);
  if (!isOk)
  {
    std::string error = "Error during xml file saving in SALOME_Launcher::saveJobs: " + std::string(jobs_file);
    INFOS(error);
    xmlFreeDoc(doc);
    fclose(xml_file);
    THROW_SALOME_CORBA_EXCEPTION(error.c_str(), SALOME::INTERNAL_ERROR);
  }

  // Clean
  xmlFreeDoc(doc);
  fclose(xml_file);
  MESSAGE("SALOME_Launcher::saveJobs : WRITING DONE!");
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
