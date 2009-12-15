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
  
  if (job_type != "command" and job_type != "yacs_file" and job_type != "python_salome")
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
  if (queue != "")
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

  try
  {
    _l.createJob(new_job);
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

void 
SALOME_Launcher::removeJob(CORBA::Long job_id)
{
  try
  {
    _l.removeJob(job_id);
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
    // find a cluster matching the structure params
    Engines::ResourceList *aMachineList = _ResManager->GetFittingResources(params);
    if (aMachineList->length() == 0)
      throw SALOME_Exception("No resources have been found with your parameters");

    const Engines::ResourceDefinition* p = _ResManager->GetResourceDefinition((*aMachineList)[0]);
    string resource_name(p->name);
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
