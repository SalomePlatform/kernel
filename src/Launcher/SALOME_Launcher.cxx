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
 *  Define a CORBA single thread policy for the server, which avoid to deal
 *  with non thread-safe usage like Change_Directory in SALOME naming service
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
  MESSAGE("destructor");
  delete _NS;
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
  return 
#ifndef WIN32
    (CORBA::Long)getpid();
#else
    (CORBA::Long)_getpid();
#endif

}

//=============================================================================
/*! CORBA Method:
 *  Submit a batch job on a cluster and returns the JobId
 *  \param xmlExecuteFile     : .xml to parse to execute on the batch cluster
 *  \param clusterName        : cluster name
 */
//=============================================================================
CORBA::Long SALOME_Launcher::submitJob(const char * xmlExecuteFile,
				       const char * clusterName)
{
  CORBA::Long jobId;

  try{
    jobId = _l.submitJob(xmlExecuteFile,clusterName);
  }
  catch(const LauncherException &ex){
    INFOS(ex.msg.c_str());
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(),SALOME::INTERNAL_ERROR);
  }
  return jobId;
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
  
  machineParams p;
  p.hostname = params.hostname;
  p.OS = params.OS;
  p.nb_node = params.nb_node;
  p.nb_proc_per_node = params.nb_proc_per_node;
  p.cpu_clock = params.cpu_clock;
  p.mem_mb = params.mem_mb;

  batchParams bp;
  bp.batch_directory = batch_params.batch_directory;
  bp.expected_during_time = batch_params.expected_during_time;
  bp.mem = batch_params.mem;
  bp.nb_proc = batch_params.nb_proc;

  vector<string> efl;
  for(int i=0;i<filesToExport.length();i++)
    efl.push_back(string(filesToExport[i]));

  vector<string> ifl;
  for(int i=0;i<filesToImport.length();i++)
    ifl.push_back(string(filesToImport[i]));

  try{
    jobId = _l.submitSalomeJob(fileToExecute,efl,ifl,bp,p);
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
SALOME_Launcher::testBatch(const Engines::MachineParameters& params)
{
  MESSAGE("BEGIN OF SALOME_Launcher::testBatch");
  CORBA::Boolean rtn = false;
  try
  {
    // find a cluster matching the structure params
    Engines::CompoList aCompoList ;
    Engines::MachineList *aMachineList = _ResManager->GetFittingResources(params, aCompoList);
    if (aMachineList->length() == 0)
      throw SALOME_Exception("No resources have been found with your parameters");

    const Engines::MachineParameters* p = _ResManager->GetMachineParameters((*aMachineList)[0]);
    string clustername(p->alias);
    INFOS("Choose cluster" <<  clustername);
    
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
/*! CORBA Method:
 *  Query a batch job on a cluster and returns the status of job
 *  \param jobId              : identification of Salome job
 *  \param params             : Constraints for the choice of the batch cluster
 */
//=============================================================================
char* SALOME_Launcher::queryJob( const CORBA::Long jobId, 
				 const Engines::MachineParameters& params)
{
  string status;
  machineParams p;
  p.hostname = params.hostname;
  p.OS = params.OS;
  p.nb_node = params.nb_node;
  p.nb_proc_per_node = params.nb_proc_per_node;
  p.cpu_clock = params.cpu_clock;
  p.mem_mb = params.mem_mb;

  try{
    status =  _l.queryJob(jobId,p);
  }
  catch(const LauncherException &ex){
    INFOS(ex.msg.c_str());
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(),SALOME::BAD_PARAM);
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
void SALOME_Launcher::deleteJob( const CORBA::Long jobId, 
				 const Engines::MachineParameters& params)
{
  machineParams p;
  p.hostname = params.hostname;
  p.OS = params.OS;
  p.nb_node = params.nb_node;
  p.nb_proc_per_node = params.nb_proc_per_node;
  p.cpu_clock = params.cpu_clock;
  p.mem_mb = params.mem_mb;

  try{
    _l.deleteJob(jobId,p);
  }
  catch(const LauncherException &ex){
    INFOS("Caught exception.");
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(),SALOME::BAD_PARAM);
  }
}

//=============================================================================
/*! CORBA Method:
 *  Get result files of job on a cluster
 *  \param jobId              : identification of Salome job
 *  \param params             : Constraints for the choice of the batch cluster
 */
//=============================================================================
void SALOME_Launcher::getResultsJob( const char *directory,
				     const CORBA::Long jobId, 
				     const Engines::MachineParameters& params)
{
  machineParams p;
  p.hostname = params.hostname;
  p.OS = params.OS;
  p.nb_node = params.nb_node;
  p.nb_proc_per_node = params.nb_proc_per_node;
  p.cpu_clock = params.cpu_clock;
  p.mem_mb = params.mem_mb;

  try{
    _l.getResultsJob( directory, jobId, p );
  }
  catch(const LauncherException &ex){
    INFOS("Caught exception.");
    THROW_SALOME_CORBA_EXCEPTION(ex.msg.c_str(),SALOME::BAD_PARAM);
  }
}

