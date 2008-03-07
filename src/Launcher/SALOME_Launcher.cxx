// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
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
#include "BatchLight_BatchManager_PBS.hxx"
#include "BatchLight_BatchManager_SLURM.hxx"
#include "BatchLight_Job.hxx"
#include "SALOME_Launcher.hxx"
#include "OpUtil.hxx"
#include <sys/types.h>
#ifndef WNT
#include <unistd.h>
#endif
#include <vector>
#include "Utils_CorbaException.hxx"
#include "Batch_Date.hxx"

#define TIME_OUT_TO_LAUNCH_CONT 21

using namespace std;

const char *SALOME_Launcher::_LauncherNameInNS = "/SalomeLauncher";

//=============================================================================
/*! 
 *  Constructor
 *  \param orb
 *  Define a CORBA single thread policy for the server, which avoid to deal
 *  with non thread-safe usage like Change_Directory in SALOME naming service
 */
//=============================================================================

SALOME_Launcher::SALOME_Launcher(CORBA::ORB_ptr orb, PortableServer::POA_var poa)
{
  MESSAGE("constructor");
  _NS = new SALOME_NamingService(orb);
  _ResManager = new SALOME_ResourcesManager(orb,poa,_NS);
  _ContManager = new SALOME_ContainerManager(orb,poa,_ResManager,_NS);
  _ResManager->_remove_ref();
  _ContManager->_remove_ref();

  _orb = CORBA::ORB::_duplicate(orb) ;
  _poa = PortableServer::POA::_duplicate(poa) ;
  PortableServer::ObjectId_var id = _poa->activate_object(this);
  CORBA::Object_var obj = _poa->id_to_reference(id);
  Engines::SalomeLauncher_var refContMan = Engines::SalomeLauncher::_narrow(obj);

  _NS->Register(refContMan,_LauncherNameInNS);
  MESSAGE("constructor end");
}

//=============================================================================
/*! 
 * destructor
 */
//=============================================================================

SALOME_Launcher::~SALOME_Launcher()
{
  MESSAGE("destructor");
  delete _NS;
  std::map < string, BatchLight::BatchManager * >::const_iterator it;
  for(it=_batchmap.begin();it!=_batchmap.end();it++)
    delete it->second;
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
  //_remove_ref();
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
  return (CORBA::Long)getpid();
}

//=============================================================================
/*! CORBA Method:
 *  Submit a batch job on a cluster and returns the JobId
 *  \param fileToExecute      : .py/.exe/.sh/... to execute on the batch cluster
 *  \param filesToExport      : to export on the batch cluster
 *  \param NumberOfProcessors : Number of processors needed on the batch cluster
 *  \param params             : Constraints for the choice of the batch cluster
 */
//=============================================================================
CORBA::Long SALOME_Launcher::submitSalomeJob( const char * fileToExecute ,
					      const Engines::FilesList& filesToExport ,
					      const Engines::FilesList& filesToImport ,
					      const Engines::BatchParameters& batch_params,
					      const Engines::MachineParameters& params)
{
  MESSAGE("BEGIN OF SALOME_Launcher::submitSalomeJob");
  CORBA::Long jobId;
  try{
    // find a cluster matching the structure params
    Engines::CompoList aCompoList ;
    Engines::MachineList *aMachineList = _ResManager->GetFittingResources(params, aCompoList);
    if (aMachineList->length() == 0)
      throw SALOME_Exception("No resources have been found with your parameters");

    const Engines::MachineParameters* p = _ResManager->GetMachineParameters((*aMachineList)[0]);
    string clustername(p->alias);
    INFOS("Choose cluster" <<  clustername);

    // search batch manager for that cluster in map or instanciate one
    std::map < string, BatchLight::BatchManager * >::const_iterator it = _batchmap.find(clustername);
    if(it == _batchmap.end())
    {
      _batchmap[clustername] = FactoryBatchManager(p);
      // TODO: Add a test for the cluster !
    }
    
    // create and submit job on cluster
    BatchLight::Job* job = new BatchLight::Job(fileToExecute, filesToExport, filesToImport, batch_params);
    bool res = job->check();
    if (!res) {
      delete job;
      throw SALOME_Exception("Job parameters are bad (see informations above)");
    }
    jobId = _batchmap[clustername]->submitJob(job);
  }
  catch(const SALOME_Exception &ex){
    INFOS(ex.what());
    THROW_SALOME_CORBA_EXCEPTION(ex.what(),SALOME::INTERNAL_ERROR);
  }
  return jobId;
}

//=============================================================================
/*! CORBA Method:
 *  Query a batch job on a cluster and returns the status of job
 *  \param jobId              : identification of Salome job
 *  \param params             : Constraints for the choice of the batch cluster
 */
//=============================================================================
char* SALOME_Launcher::querySalomeJob( const CORBA::Long jobId, 
				       const Engines::MachineParameters& params)
{
  string status;
  try{
    // find a cluster matching params structure
    Engines::CompoList aCompoList ;
    Engines::MachineList * aMachineList = _ResManager->GetFittingResources( params , aCompoList ) ;
    const Engines::MachineParameters* p = _ResManager->GetMachineParameters((*aMachineList)[0]);
    string clustername(p->alias);
    
    // search batch manager for that cluster in map
    std::map < string, BatchLight::BatchManager * >::const_iterator it = _batchmap.find(clustername);
    if(it == _batchmap.end())
      throw SALOME_Exception("no batchmanager for that cluster");
    
    status = _batchmap[clustername]->queryJob(jobId);
  }
  catch(const SALOME_Exception &ex){
    INFOS("Caught exception.");
    THROW_SALOME_CORBA_EXCEPTION(ex.what(),SALOME::BAD_PARAM);
  }
  return CORBA::string_dup(status.c_str());
}

//=============================================================================
/*! CORBA Method:
 *  Delete a batch job on a cluster 
 *  \param jobId              : identification of Salome job
 *  \param params             : Constraints for the choice of the batch cluster
 */
//=============================================================================
void SALOME_Launcher::deleteSalomeJob( const CORBA::Long jobId, 
				       const Engines::MachineParameters& params)
{
  try{
    // find a cluster matching params structure
    Engines::CompoList aCompoList ;
    Engines::MachineList *aMachineList = _ResManager->GetFittingResources( params , aCompoList ) ;
    const Engines::MachineParameters* p = _ResManager->GetMachineParameters((*aMachineList)[0]);
    string clustername(p->alias);
    
    // search batch manager for that cluster in map
    std::map < string, BatchLight::BatchManager * >::const_iterator it = _batchmap.find(clustername);
    if(it == _batchmap.end())
      throw SALOME_Exception("no batchmanager for that cluster");
    
    _batchmap[clustername]->deleteJob(jobId);
  }
  catch(const SALOME_Exception &ex){
    INFOS("Caught exception.");
    THROW_SALOME_CORBA_EXCEPTION(ex.what(),SALOME::BAD_PARAM);
  }
}

//=============================================================================
/*! CORBA Method:
 *  Get result files of job on a cluster
 *  \param jobId              : identification of Salome job
 *  \param params             : Constraints for the choice of the batch cluster
 */
//=============================================================================
void SALOME_Launcher::getResultSalomeJob( const char *directory,
					  const CORBA::Long jobId, 
					  const Engines::MachineParameters& params)
{
  try{
    // find a cluster matching params structure
    Engines::CompoList aCompoList ;
    Engines::MachineList *aMachineList = _ResManager->GetFittingResources( params , aCompoList ) ;
    const Engines::MachineParameters* p = _ResManager->GetMachineParameters((*aMachineList)[0]);
    string clustername(p->alias);
    
    // search batch manager for that cluster in map
    std::map < string, BatchLight::BatchManager * >::const_iterator it = _batchmap.find(clustername);
    if(it == _batchmap.end())
      throw SALOME_Exception("no batchmanager for that cluster");
    
    _batchmap[clustername]->importOutputFiles( directory, jobId );
  }
  catch(const SALOME_Exception &ex){
    INFOS("Caught exception.");
    THROW_SALOME_CORBA_EXCEPTION(ex.what(),SALOME::BAD_PARAM);
  }
}

//=============================================================================
/*!
 *  Factory to instanciate the good batch manager for choosen cluster.
 */ 
//=============================================================================

BatchLight::BatchManager *SALOME_Launcher::FactoryBatchManager( const Engines::MachineParameters* params ) throw(SALOME_Exception)
{
  // Fill structure for batch manager
  BatchLight::batchParams p;
  p.hostname = params->alias;
  p.protocol = params->protocol;
  p.username = params->username;
  p.applipath = params->applipath;
  for(int i=0;i<params->modList.length();i++)
    p.modulesList.push_back((const char*)params->modList[i]);
  p.nbnodes = params->nb_node;
  p.nbprocpernode = params->nb_proc_per_node;
  p.mpiImpl = params->mpiImpl;

  string sb = (const char*)params->batch;
  if(sb == "pbs")
    return new BatchLight::BatchManager_PBS(p);
  else if(sb == "slurm")
    return new BatchLight::BatchManager_SLURM(p);
  else{
    MESSAGE("BATCH = " << params->batch);
    throw SALOME_Exception("no batchmanager for that cluster");
  }
}

