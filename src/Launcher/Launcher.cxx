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

#include <list>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <time.h>

#ifdef WITH_LIBBATCH
#include <libbatch/BatchManagerCatalog.hxx>
#include <libbatch/FactBatchManager.hxx>
#include <libbatch/BatchManager.hxx>
#endif

#include "Basics_Utils.hxx"
#include "Basics_DirUtils.hxx"
#include "SALOME_Launcher_Handler.hxx"
#include "Launcher.hxx"
#include "Launcher_Job_Command.hxx"
#include "Launcher_XML_Persistence.hxx"

using namespace std;

//=============================================================================
/*! 
 *  Constructor
 *  \param orb
 *  Define a CORBA single thread policy for the server, which avoid to deal
 *  with non thread-safe usage like Change_Directory in SALOME naming service
 */
//=============================================================================
Launcher_cpp::Launcher_cpp()
{
  LAUNCHER_MESSAGE("Launcher_cpp constructor");
  _job_cpt = 0;
  _job_cpt_mutex = new pthread_mutex_t();
  pthread_mutex_init(_job_cpt_mutex, NULL);
}

//=============================================================================
/*! 
 * destructor
 */
//=============================================================================
Launcher_cpp::~Launcher_cpp()
{
  LAUNCHER_MESSAGE("Launcher_cpp destructor");
#ifdef WITH_LIBBATCH
  std::map<int, Launcher::Job *>::const_iterator it_job;
  for(it_job = _launcher_job_map.begin(); it_job != _launcher_job_map.end(); it_job++)
    delete it_job->second;
  std::map <int, Batch::BatchManager * >::const_iterator it1;
  for(it1=_batchmap.begin();it1!=_batchmap.end();it1++)
    delete it1->second;
#endif

  pthread_mutex_destroy(_job_cpt_mutex);
  delete _job_cpt_mutex;
}

#ifdef WITH_LIBBATCH

//=============================================================================
/*!
 * Add a job into the launcher - check resource and choose one 
 */ 
//=============================================================================
void 
Launcher_cpp::createJob(Launcher::Job * new_job)
{
  LAUNCHER_MESSAGE("Creating a new job");
  // Add job to the jobs map
  pthread_mutex_lock(_job_cpt_mutex);
  new_job->setNumber(_job_cpt);
  _job_cpt++;
  pthread_mutex_unlock(_job_cpt_mutex);
  std::map<int, Launcher::Job *>::const_iterator it_job = _launcher_job_map.find(new_job->getNumber());
  if (it_job == _launcher_job_map.end())
    _launcher_job_map[new_job->getNumber()] = new_job;
  else
  {
    LAUNCHER_INFOS("A job as already the same id: " << new_job->getNumber());
    delete new_job;
    throw LauncherException("A job as already the same id - job is not created !");
  }
  LAUNCHER_MESSAGE("New Job created");
}

//=============================================================================
/*!
 * Launch a job 
 */ 
//=============================================================================
void 
Launcher_cpp::launchJob(int job_id)
{
  LAUNCHER_MESSAGE("Launch a job");

  // Check if job exist
  std::map<int, Launcher::Job *>::const_iterator it_job = _launcher_job_map.find(job_id);
  if (it_job == _launcher_job_map.end())
  {
    LAUNCHER_INFOS("Cannot find the job, is it created ? job number: " << job_id);
    throw LauncherException("Cannot find the job, is it created ?");
  }

  Launcher::Job * job = it_job->second;

  // Check job state (cannot launch a job already launched...)
  if (job->getState() != "CREATED")
  {
    LAUNCHER_INFOS("Bad state of the job: " << job->getState());
    throw LauncherException("Bad state of the job: " + job->getState());
  }

  // Third step search batch manager for the job into the map -> instanciate one if does not exist
#ifdef WITH_LIBBATCH
  std::map<int, Batch::BatchManager *>::const_iterator it = _batchmap.find(job_id);
  if(it == _batchmap.end())
  {
    createBatchManagerForJob(job);
  }
#endif

  try {
    Batch::JobId batch_manager_job_id = _batchmap[job_id]->submitJob(*(job->getBatchJob()));
    job->setBatchManagerJobId(batch_manager_job_id);
    job->setState("QUEUED");
    job->setReference(batch_manager_job_id.getReference());
  }
  catch(const Batch::GenericException &ex)
  {
    LAUNCHER_INFOS("Job is not launched, exception in submitJob: " << ex.message);
    throw LauncherException(ex.message.c_str());
  }
  LAUNCHER_MESSAGE("Job launched");
}

//=============================================================================
/*!
 * Get job state
 */ 
//=============================================================================
const char *
Launcher_cpp::getJobState(int job_id)
{
  LAUNCHER_MESSAGE("Get job state");

  // Check if job exist
  std::map<int, Launcher::Job *>::const_iterator it_job = _launcher_job_map.find(job_id);
  if (it_job == _launcher_job_map.end())
  {
    LAUNCHER_INFOS("Cannot find the job, is it created ? job number: " << job_id);
    throw LauncherException("Cannot find the job, is it created ?");
  }

  Launcher::Job * job = it_job->second;

  std::string state;
  try
  {
    state = job->updateJobState();
  }
  catch(const Batch::GenericException &ex)
  {
    LAUNCHER_INFOS("getJobState failed, exception: " << ex.message);
    throw LauncherException(ex.message.c_str());
  }

  return state.c_str();
}

//=============================================================================
/*!
 * Get job assigned hostnames
 */
//=============================================================================
const char *
Launcher_cpp::getAssignedHostnames(int job_id)
{
  LAUNCHER_MESSAGE("Get job assigned hostnames");

  // Check if job exist
  std::map<int, Launcher::Job *>::const_iterator it_job = _launcher_job_map.find(job_id);
  if (it_job == _launcher_job_map.end())
  {
    LAUNCHER_INFOS("Cannot find the job, is it created ? job number: " << job_id);
    throw LauncherException("Cannot find the job, is it created ?");
  }

  Launcher::Job * job = it_job->second;
  std::string assigned_hostnames = job->getAssignedHostnames();

  return assigned_hostnames.c_str();
}

//=============================================================================
/*!
 * Get Job result - the result directory could be changed
 */ 
//=============================================================================
void
Launcher_cpp::getJobResults(int job_id, std::string directory)
{
  LAUNCHER_MESSAGE("Get Job results");

  // Check if job exist
  std::map<int, Launcher::Job *>::const_iterator it_job = _launcher_job_map.find(job_id);
  if (it_job == _launcher_job_map.end())
  {
    LAUNCHER_INFOS("Cannot find the job, is it created ? job number: " << job_id);
    throw LauncherException("Cannot find the job, is it created ?");
  }

  Launcher::Job * job = it_job->second;
  std::string resource_name = job->getResourceDefinition().Name;
  try 
  {
    if (directory != "")
      _batchmap[job_id]->importOutputFiles(*(job->getBatchJob()), directory);
    else
      _batchmap[job_id]->importOutputFiles(*(job->getBatchJob()), job->getResultDirectory());
  }
  catch(const Batch::GenericException &ex)
  {
    LAUNCHER_INFOS("getJobResult is maybe incomplete, exception: " << ex.message);
    throw LauncherException(ex.message.c_str());
  }
  LAUNCHER_MESSAGE("getJobResult ended");
}

//=============================================================================
/*!
 * Get Job dump state - the result directory could be changed
 */ 
//=============================================================================
bool
Launcher_cpp::getJobDumpState(int job_id, std::string directory)
{
  bool rtn;
  LAUNCHER_MESSAGE("Get Job dump state");

  // Check if job exist
  std::map<int, Launcher::Job *>::const_iterator it_job = _launcher_job_map.find(job_id);
  if (it_job == _launcher_job_map.end())
  {
    LAUNCHER_INFOS("Cannot find the job, is it created ? job number: " << job_id);
    throw LauncherException("Cannot find the job, is it created ?");
  }

  Launcher::Job * job = it_job->second;
  std::string resource_name = job->getResourceDefinition().Name;
  try 
  {
    if (directory != "")
      rtn = _batchmap[job_id]->importDumpStateFile(*(job->getBatchJob()), directory);
    else
      rtn = _batchmap[job_id]->importDumpStateFile(*(job->getBatchJob()), job->getResultDirectory());
  }
  catch(const Batch::GenericException &ex)
  {
    LAUNCHER_INFOS("getJobResult is maybe incomplete, exception: " << ex.message);
    throw LauncherException(ex.message.c_str());
  }
  LAUNCHER_MESSAGE("getJobResult ended");
  return rtn;
}

//=============================================================================
/*!
 * Get one file from the working directory - the result directory can be changed
 */
//=============================================================================
bool
Launcher_cpp::getJobWorkFile(int job_id,
                             std::string work_file,
                             std::string directory)
{
  bool rtn;
  LAUNCHER_MESSAGE("Get working file " << work_file);

  // Check if job exist
  std::map<int, Launcher::Job *>::const_iterator it_job = _launcher_job_map.find(job_id);
  if (it_job == _launcher_job_map.end())
  {
    LAUNCHER_INFOS("Cannot find the job, is it created ? job number: " << job_id);
    throw LauncherException("Cannot find the job, is it created ?");
  }

  Launcher::Job * job = it_job->second;
  std::string resource_name = job->getResourceDefinition().Name;
  try
  {
    if (directory != "")
      rtn = _batchmap[job_id]->importWorkFile(*(job->getBatchJob()), work_file, directory);
    else
      rtn = _batchmap[job_id]->importWorkFile(*(job->getBatchJob()), work_file, job->getResultDirectory());
  }
  catch(const Batch::GenericException &ex)
  {
    LAUNCHER_INFOS("getJobWorkFile is maybe incomplete, exception: " << ex.message);
    throw LauncherException(ex.message.c_str());
  }
  LAUNCHER_MESSAGE("getJobWorkFile ended");
  return rtn;
}

//=============================================================================
/*!
 * Remove the job - into the Launcher and its batch manager
 */ 
//=============================================================================
void
Launcher_cpp::removeJob(int job_id)
{
  LAUNCHER_MESSAGE("Remove Job");

  // Check if job exist
  std::map<int, Launcher::Job *>::iterator it_job = _launcher_job_map.find(job_id);
  if (it_job == _launcher_job_map.end())
  {
    LAUNCHER_INFOS("Cannot find the job, is it created ? job number: " << job_id);
    throw LauncherException("Cannot find the job, is it created ?");
  }

  it_job->second->removeJob();
  delete it_job->second;
  _launcher_job_map.erase(it_job);
}

//=============================================================================
/*!
 * stop the job
 */ 
//=============================================================================
void
Launcher_cpp::stopJob(int job_id)
{
  LAUNCHER_MESSAGE("Stop Job");

  // Check if job exist
  std::map<int, Launcher::Job *>::iterator it_job = _launcher_job_map.find(job_id);
  if (it_job == _launcher_job_map.end())
  {
    LAUNCHER_INFOS("Cannot find the job, is it created ? job number: " << job_id);
    throw LauncherException("Cannot find the job, is it created ?");
  }

  it_job->second->stopJob();
}

//=============================================================================
/*! 
 *  create a launcher job based on a file
 *  \param xmlExecuteFile     : to define the execution on the batch cluster
 */
//=============================================================================
long 
Launcher_cpp::createJobWithFile(const std::string xmlExecuteFile, 
                                const std::string clusterName)
{
  LAUNCHER_MESSAGE("Begin of Launcher_cpp::createJobWithFile");

  // Parsing xml file
  ParserLauncherType job_params = ParseXmlFile(xmlExecuteFile);

  // Creating a new job
  Launcher::Job_Command * new_job = new Launcher::Job_Command();

  std::string cmdFile = Kernel_Utils::GetTmpFileName();  
#ifndef WIN32
  cmdFile += ".sh";
#else
  cmdFile += ".bat";
#endif
  std::ofstream os;
  os.open(cmdFile.c_str(), std::ofstream::out );
  os << "#! /bin/sh" << std::endl;
  os << job_params.Command;
  os.close();

  new_job->setJobFile(cmdFile);
  new_job->setLocalDirectory(job_params.RefDirectory);
  new_job->setWorkDirectory(job_params.MachinesList[clusterName].WorkDirectory);
  new_job->setEnvFile(job_params.MachinesList[clusterName].EnvFile);

  for(int i=0; i < job_params.InputFile.size(); i++)
    new_job->add_in_file(job_params.InputFile[i]);
  for(int i=0; i < job_params.OutputFile.size();i++)
    new_job->add_out_file(job_params.OutputFile[i]);

  resourceParams p;
  p.hostname = clusterName;
  p.name = "";
  p.OS = "";
  p.nb_proc = job_params.NbOfProcesses;
  p.nb_node = 0;
  p.nb_proc_per_node = 0;
  p.cpu_clock = 0;
  p.mem_mb = 0;
  new_job->setResourceRequiredParams(p);

  createJob(new_job);
  return new_job->getNumber();
}

//=============================================================================
/*!
 *  Factory to instanciate the good batch manager for choosen cluster.
 */ 
//=============================================================================
Batch::BatchManager *
Launcher_cpp::FactoryBatchManager(ParserResourcesType& params)
{
  std::string mpi;
  Batch::CommunicationProtocolType protocol;
  Batch::FactBatchManager * fact;

  std::string hostname = params.HostName;

  switch(params.Protocol)
  {
    case sh:
      protocol = Batch::SH;
      break;
    case rsh:
      protocol = Batch::RSH;
      break;
    case ssh:
      protocol = Batch::SSH;
      break;
    default:
      throw LauncherException("Unknown protocol for this resource");
      break;
  }

  switch(params.mpi)
  {
    case lam:
      mpi = "lam";
      break;
    case mpich1:
      mpi = "mpich1";
      break;
    case mpich2:
      mpi = "mpich2";
      break;
    case openmpi:
      mpi = "openmpi";
      break;
    case ompi:
      mpi = "ompi";
      break;
    case slurmmpi:
      mpi = "slurmmpi";
      break;
    case prun:
      mpi = "prun";
      break;
    default:
      mpi = "nompi";
  }

  const char * bmType;
  switch( params.Batch )
  {
    case pbs:
      bmType = "PBS";
      break;
    case lsf:
      bmType = "LSF";
      break;
    case sge:
      bmType = "SGE";
      break;
    case ccc:
      bmType = "CCC";
      break;
    case slurm:
      bmType = "SLURM";
      break;
    case none:
      bmType = "LOCAL";
      break;
    case ll:
      bmType = "LL";
      break;
    case vishnu:
      bmType = "VISHNU";
      break;
    case oar:
      bmType = "OAR";
      break;
    case coorm:
      bmType = "COORM";
      break;
    default:
      LAUNCHER_MESSAGE("Bad batch description of the resource: Batch = " << params.Batch);
      throw LauncherException("No batchmanager for that cluster - Bad batch description of the resource");
  }
  Batch::BatchManagerCatalog & cata = Batch::BatchManagerCatalog::getInstance();
  fact = dynamic_cast<Batch::FactBatchManager*>(cata(bmType));
  if (fact == NULL) {
    LAUNCHER_MESSAGE("Cannot find batch manager factory for " << bmType << ". Check your version of libBatch.");
    throw LauncherException("Cannot find batch manager factory");
  }
  LAUNCHER_MESSAGE("Instanciation of batch manager of type: " << bmType);
  Batch::BatchManager * batch_client = (*fact)(hostname.c_str(), params.UserName.c_str(),
                                               protocol, mpi.c_str());
  return batch_client;
}

//----------------------------------------------------------
// Without LIBBATCH - Launcher_cpp do nothing...
//----------------------------------------------------------
#else

void 
Launcher_cpp::createJob(Launcher::Job * new_job)
{
  LAUNCHER_INFOS("Launcher compiled without LIBBATCH - cannot create a job !!!");
  delete new_job;
  throw LauncherException("Method Launcher_cpp::createJob is not available "
                          "(libBatch was not present at compilation time)");
}

void 
Launcher_cpp::launchJob(int job_id)
{
  LAUNCHER_INFOS("Launcher compiled without LIBBATCH - cannot launch a job !!!");
  throw LauncherException("Method Launcher_cpp::launchJob is not available "
                          "(libBatch was not present at compilation time)");
}

const char *
Launcher_cpp::getJobState(int job_id)
{
  LAUNCHER_INFOS("Launcher compiled without LIBBATCH - cannot get job state!!!");
  throw LauncherException("Method Launcher_cpp::getJobState is not available "
                          "(libBatch was not present at compilation time)");
}

const char *
Launcher_cpp::getAssignedHostnames(int job_id)
{
  LAUNCHER_INFOS("Launcher compiled without LIBBATCH - cannot get job assigned hostnames!!!");
  throw LauncherException("Method Launcher_cpp::getAssignedHostnames is not available "
                          "(libBatch was not present at compilation time)");
}

void
Launcher_cpp::getJobResults(int job_id, std::string directory)
{
  LAUNCHER_INFOS("Launcher compiled without LIBBATCH - cannot get job results!!!");
  throw LauncherException("Method Launcher_cpp::getJobResults is not available "
                          "(libBatch was not present at compilation time)");
}

bool
Launcher_cpp::getJobDumpState(int job_id, std::string directory)
{
  LAUNCHER_INFOS("Launcher compiled without LIBBATCH - cannot get job dump state!!!");
  throw LauncherException("Method Launcher_cpp::getJobDumpState is not available "
                          "(libBatch was not present at compilation time)");
}

bool
Launcher_cpp::getJobWorkFile(int job_id, std::string work_file, std::string directory)
{
  LAUNCHER_INFOS("Launcher compiled without LIBBATCH - cannot get job dump state!!!");
  throw LauncherException("Method Launcher_cpp::getJobWorkFile is not available "
                          "(libBatch was not present at compilation time)");
}

void
Launcher_cpp::removeJob(int job_id)
{
  LAUNCHER_INFOS("Launcher compiled without LIBBATCH - cannot remove job!!!");
  throw LauncherException("Method Launcher_cpp::removeJob is not available "
                          "(libBatch was not present at compilation time)");
}

void
Launcher_cpp::stopJob(int job_id)
{
  throw LauncherException("Method Launcher_cpp::stopJob is not available "
                          "(libBatch was not present at compilation time)");
}

long 
Launcher_cpp::createJobWithFile( const std::string xmlExecuteFile, std::string clusterName)
{
  throw LauncherException("Method Launcher_cpp::createJobWithFile is not available "
                          "(libBatch was not present at compilation time)");
  return 0;
}

#endif

ParserLauncherType 
Launcher_cpp::ParseXmlFile(std::string xmlExecuteFile)
{
  ParserLauncherType job_params;
  SALOME_Launcher_Handler * handler = new SALOME_Launcher_Handler(job_params);

  const char* aFilePath = xmlExecuteFile.c_str();
  FILE* aFile = fopen(aFilePath, "r");
  if (aFile != NULL)
  {
    xmlDocPtr aDoc = xmlReadFile(aFilePath, NULL, 0);
    if (aDoc != NULL)
      handler->ProcessXmlDocument(aDoc);
    else
    {
      std::string message = "ResourcesManager_cpp: could not parse file: " + xmlExecuteFile;
      LAUNCHER_MESSAGE(message);
      delete handler;
      throw LauncherException(message);
    }
    // Free the document
    xmlFreeDoc(aDoc);
    fclose(aFile);
  }
  else
  {
    std::string message = "ResourcesManager_cpp: file is not readable: " + xmlExecuteFile;
    LAUNCHER_MESSAGE(message);
    delete handler;
    throw LauncherException(message);
  }

  // Return
  delete handler;
  return job_params;
}

std::map<int, Launcher::Job *>
Launcher_cpp::getJobs()
{
  return _launcher_job_map;
}

void 
Launcher_cpp::createBatchManagerForJob(Launcher::Job * job)
{
  int job_id = job->getNumber();

  // Select a ressource for the job
  std::vector<std::string> ResourceList;
  resourceParams params = job->getResourceRequiredParams();
  // Consider only resources that can launch batch jobs
  params.can_launch_batch_jobs = true;
  try
  {
    ResourceList = _ResManager->GetFittingResources(params);
  }
  catch(const ResourcesException &ex)
  {
    throw LauncherException(ex.msg.c_str());
  }
  if (ResourceList.size() == 0)
  {
    LAUNCHER_INFOS("No adequate resource found for the job, number " << job->getNumber());
    job->setState("ERROR");
    throw LauncherException("No resource found the job");
  }

  // Configure the job with the resource selected - the first of the list
  ParserResourcesType resource_definition = _ResManager->GetResourcesDescr(ResourceList[0]);

  // Set resource definition to the job
  // The job will check if the definitions needed
  try
  {
    job->setResourceDefinition(resource_definition);
  }
  catch(const LauncherException &ex)
  {
    LAUNCHER_INFOS("Error in the definition of the resource, mess: " << ex.msg);
    job->setState("ERROR");
    throw ex;
  }

  // Step 2: We can now add a Factory if the resource is correctly define
#ifdef WITH_LIBBATCH
  std::map<int, Batch::BatchManager *>::const_iterator it = _batchmap.find(job_id);
  if(it == _batchmap.end())
  {
    try
    {
      // Warning cannot write on one line like this, because map object is constructed before
      // the method is called...
      //_batchmap[job_id] = FactoryBatchManager(resource_definition);
      Batch::BatchManager * batch_client = FactoryBatchManager(resource_definition);
      _batchmap[job_id] = batch_client;
    }
    catch(const LauncherException &ex)
    {
      LAUNCHER_INFOS("Error during creation of the batch manager of the job, mess: " << ex.msg);
      throw ex;
    }
    catch(const Batch::GenericException &ex)
    {
      LAUNCHER_INFOS("Error during creation of the batch manager of the job, mess: " << ex.message);
      throw LauncherException(ex.message);
    }
  }
#endif
}

void 
Launcher_cpp::addJobDirectlyToMap(Launcher::Job * new_job)
{
  // Step 0: Calculated job_id
  pthread_mutex_lock(_job_cpt_mutex);
  int job_id = _job_cpt;
  _job_cpt++;
  new_job->setNumber(job_id);
  pthread_mutex_unlock(_job_cpt_mutex);

  // Step 1: check if resource is already in the map
  createBatchManagerForJob(new_job);

  // Step 2: add the job to the batch manager
#ifdef WITH_LIBBATCH
  try
  {
    Batch::JobId batch_manager_job_id = _batchmap[job_id]->addJob(*(new_job->getBatchJob()),
                                                                  new_job->getReference());
    new_job->setBatchManagerJobId(batch_manager_job_id);
  }
  catch(const Batch::GenericException &ex)
  {
    LAUNCHER_INFOS("Job cannot be added, exception in addJob: " << ex.message);
    throw LauncherException(ex.message.c_str());
  }

  // Step 3: add job to launcher map
  std::map<int, Launcher::Job *>::const_iterator it_job = _launcher_job_map.find(new_job->getNumber());
  if (it_job == _launcher_job_map.end())
    _launcher_job_map[new_job->getNumber()] = new_job;
  else
  {
    LAUNCHER_INFOS("A job as already the same id: " << new_job->getNumber());
    delete new_job;
    throw LauncherException("A job as already the same id - job is not created !");
  }
  LAUNCHER_MESSAGE("New job added");
#endif
}

list<int>
Launcher_cpp::loadJobs(const char* jobs_file)
{
  list<int> new_jobs_id_list;

  // Load the jobs from XML file
  list<Launcher::Job *> jobs_list = Launcher::XML_Persistence::loadJobs(jobs_file);

  // Create each job in the launcher
  list<Launcher::Job *>::const_iterator it_job;
  for (it_job = jobs_list.begin(); it_job != jobs_list.end(); it_job++)
  {
    Launcher::Job * new_job = *it_job;
    string job_state = new_job->getState();

    try
    {
      if (job_state == "CREATED")
      {
        // In this case, we ignore run_part informations
        createJob(new_job);
        new_jobs_id_list.push_back(new_job->getNumber());
      }
      else if (job_state == "QUEUED"     ||
               job_state == "RUNNING"    ||
               job_state == "IN_PROCESS" ||
               job_state == "PAUSED")
      {
        addJobDirectlyToMap(new_job);
        new_jobs_id_list.push_back(new_job->getNumber());

        // Step 4: We check that the BatchManager could resume
        // the job
#ifdef WITH_LIBBATCH
        if (new_job->getBatchManagerJobId().getReference() != new_job->getReference())
        {
          LAUNCHER_INFOS("BatchManager type cannot resume a job - job state is set to ERROR");
          new_job->setState("ERROR");
        }
#endif
      }
      else if (job_state == "FINISHED" ||
               job_state == "FAILED"   ||
               job_state == "ERROR")
      {
        // Step 2: We add run_part informations
        addJobDirectlyToMap(new_job);
        new_jobs_id_list.push_back(new_job->getNumber());
      }
      else
      {
        LAUNCHER_INFOS("A bad job is found, state unknown " << job_state);
        delete new_job;
      }
    }
    catch(const LauncherException &ex)
    {
      LAUNCHER_INFOS("Cannot load the job. Exception: " << ex.msg.c_str());
      delete new_job;
    }
  }

  return new_jobs_id_list;
}

void
Launcher_cpp::saveJobs(const char* jobs_file)
{
  // Create a sorted list from the internal job map
  list<const Launcher::Job *> jobs_list;
  for (int i=0; i<_job_cpt; i++)
  {
    map<int, Launcher::Job *>::const_iterator it_job = _launcher_job_map.find(i);
    if (it_job != _launcher_job_map.end())
      jobs_list.push_back(it_job->second);
  }

  // Save the jobs in XML file
  Launcher::XML_Persistence::saveJobs(jobs_file, jobs_list);
}
