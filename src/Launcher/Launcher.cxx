//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

#ifdef WITH_LIBBATCH
#include <Batch/Batch_Date.hxx>
#include <Batch/Batch_FactBatchManager_eLSF.hxx>
#include <Batch/Batch_FactBatchManager_ePBS.hxx>
#include <Batch/Batch_BatchManager_eClient.hxx>
#include <Batch/Batch_FactBatchManager_eSGE.hxx>
#include <Batch/Batch_FactBatchManager_eSSH.hxx>
#endif

#include "SALOME_Launcher_Handler.hxx"
#include "Launcher.hxx"
#include "Launcher_Job_Command.hxx"
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <time.h>

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
  std::map < string, Batch::BatchManager_eClient * >::const_iterator it1;
  for(it1=_batchmap.begin();it1!=_batchmap.end();it1++)
    delete it1->second;
  std::map<int, Launcher::Job *>::const_iterator it_job;
  for(it_job = _launcher_job_map.begin(); it_job != _launcher_job_map.end(); it_job++)
    delete it_job->second;
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
  
  // First step take a resource
  std::vector<std::string> ResourceList;
  resourceParams params = new_job->getResourceRequiredParams();
  try{
    ResourceList = _ResManager->GetFittingResources(params);
  }
  catch(const ResourcesException &ex){
    throw LauncherException(ex.msg.c_str());
  }
  if (ResourceList.size() == 0)
  {
    LAUNCHER_INFOS("No adequate resource found for the job, number " << new_job->getNumber() << " - deleting it");
    delete new_job;
    throw LauncherException("No resource found the job");
  }

  // Second step configure the job with the resource selected - the first of the list
  ParserResourcesType resource_definition = _ResManager->GetResourcesDescr(ResourceList[0]);

  // Set resource definition to the job
  // The job will check if the definitions needed
  try 
  {
    new_job->setResourceDefinition(resource_definition);
  }
  catch(const LauncherException &ex)
  {
    LAUNCHER_INFOS("Error in the definition of the resource, mess: " << ex.msg);
    delete new_job;
    throw ex;
  }

  // Third step search batch manager for the resource into the map -> instanciate one if does not exist
  std::string resource_name = resource_definition.Name;
  std::map<std::string, Batch::BatchManager_eClient *>::const_iterator it = _batchmap.find(resource_name);
  if(it == _batchmap.end())
  {
    try 
    {
      _batchmap[resource_name] = FactoryBatchManager(resource_definition);
    }
    catch(const LauncherException &ex)
    {
      LAUNCHER_INFOS("Error during creation of the batch manager of the resource, mess: " << ex.msg);
      delete new_job;
      throw ex;
    }
    catch(const Batch::EmulationException &ex)
    {
      LAUNCHER_INFOS("Error during creation of the batch manager of the resource, mess: " << ex.message);
      delete new_job;
      throw LauncherException(ex.message);
    }
  }


  // Final step - add job to the jobs map
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

  std::string resource_name = job->getResourceDefinition().Name;
  try {
    Batch::JobId batch_manager_job_id = _batchmap[resource_name]->submitJob(*(job->getBatchJob()));
    job->setBatchManagerJobId(batch_manager_job_id);
    job->setState("QUEUED");
  }
  catch(const Batch::EmulationException &ex)
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
  std::string state = job->updateJobState();

  return state.c_str();
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
      _batchmap[resource_name]->importOutputFiles(*(job->getBatchJob()), directory);
    else
      _batchmap[resource_name]->importOutputFiles(*(job->getBatchJob()), job->getResultDirectory());
  }
  catch(const Batch::EmulationException &ex)
  {
    LAUNCHER_INFOS("getJobResult is maybe incomplete, exception: " << ex.message);
    throw LauncherException(ex.message.c_str());
  }
  LAUNCHER_MESSAGE("getJobResult ended");
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

  delete it_job->second;
  _launcher_job_map.erase(it_job);
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
  new_job->setJobFile(job_params.Command);
  new_job->setLocalDirectory(job_params.RefDirectory);
  new_job->setWorkDirectory(job_params.MachinesList[clusterName].WorkDirectory);
  new_job->setEnvFile(job_params.MachinesList[clusterName].EnvFile);

  for(int i=0; i < job_params.InputFile.size(); i++)
    new_job->add_in_file(job_params.InputFile[i]);
  for(int i=0; i < job_params.OutputFile.size();i++)
    new_job->add_out_file(job_params.OutputFile[i]);

  resourceParams p;
  p.hostname = clusterName;
  p.nb_proc = job_params.NbOfProcesses;
  new_job->setResourceRequiredParams(p);

  createJob(new_job);
  return new_job->getNumber();
}

//=============================================================================
/*!
 *  Factory to instanciate the good batch manager for choosen cluster.
 */ 
//=============================================================================
Batch::BatchManager_eClient *
Launcher_cpp::FactoryBatchManager(ParserResourcesType& params)
{
  std::string mpi;
  Batch::CommunicationProtocolType protocol;
  Batch::FactBatchManager_eClient* fact;

  int nb_proc_per_node = params.DataForSort._nbOfProcPerNode;
  std::string hostname = params.HostName;

  switch(params.Protocol)
  {
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
    case slurm:
      mpi = "slurm";
      break;
    case prun:
      mpi = "prun";
      break;
    default:
      mpi = "nompi";
  }

  std::string message = "Instanciation of batch manager of type: ";
  switch( params.Batch )
  {
    case pbs:
      message += "ePBS";
      fact = new Batch::FactBatchManager_ePBS;
      break;
    case lsf:
      message += "eLSF";
      fact = new Batch::FactBatchManager_eLSF;
      break;
    case sge:
      message += "eSGE";
      fact = new Batch::FactBatchManager_eSGE;
      break;
    case ssh_batch:
      message += "eSSH";
      fact = new Batch::FactBatchManager_eSSH;
      break;
    default:
      LAUNCHER_MESSAGE("Bad batch description of the resource: Batch = " << params.Batch);
      throw LauncherException("No batchmanager for that cluster - Bad batch description of the resource");
  }
  LAUNCHER_MESSAGE(message);
  return (*fact)(hostname.c_str(), protocol, mpi.c_str(), nb_proc_per_node);
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

void
Launcher_cpp::getJobResults(int job_id, std::string directory)
{
  LAUNCHER_INFOS("Launcher compiled without LIBBATCH - cannot get job results!!!");
  throw LauncherException("Method Launcher_cpp::getJobResults is not available "
                          "(libBatch was not present at compilation time)");
}

void
Launcher_cpp::removeJob(int job_id)
{
  LAUNCHER_INFOS("Launcher compiled without LIBBATCH - cannot remove job!!!");
  throw LauncherException("Method Launcher_cpp::removeJob is not available "
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
Launcher_cpp::ParseXmlFile(string xmlExecuteFile)
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

