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
#endif

#include "SALOME_Launcher_Handler.hxx"
#include "Launcher.hxx"
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
#if defined(_DEBUG_) || defined(_DEBUG)
  cerr << "Launcher_cpp constructor" << endl;
#endif
}

//=============================================================================
/*! 
 * destructor
 */
//=============================================================================

Launcher_cpp::~Launcher_cpp()
{
#if defined(_DEBUG_) || defined(_DEBUG)
  cerr << "Launcher_cpp destructor" << endl;
#endif

#ifdef WITH_LIBBATCH
  std::map < string, Batch::BatchManager_eClient * >::const_iterator it1;
  for(it1=_batchmap.begin();it1!=_batchmap.end();it1++)
    delete it1->second;
  std::map < std::pair<std::string,long> , Batch::Job* >::const_iterator it2;
  for(it2=_jobmap.begin();it2!=_jobmap.end();it2++)
    delete it2->second;
#endif
}

//=============================================================================
/*! CORBA Method:
 *  Submit a batch job on a cluster and returns the JobId
 *  \param xmlExecuteFile     : to define the execution on the batch cluster
 *  \param clusterName        : name of the batch cluster
 */
//=============================================================================
long Launcher_cpp::submitJob( const std::string xmlExecuteFile,
                  const std::string clusterName) throw(LauncherException)
{
#ifdef WITH_LIBBATCH
#if defined(_DEBUG_) || defined(_DEBUG)
  cout << "BEGIN OF Launcher_cpp::submitJob" << endl;
#endif
  long jobId;
  vector<string> aMachineList;

  if(!_ResManager)
    throw LauncherException("You must set Resources Manager to Launcher!!");

  // verify if cluster is in resources catalog
  machineParams params;
  params.hostname = clusterName;
  try{
    aMachineList = _ResManager->GetFittingResources(params);
  }
  catch(const ResourcesException &ex){
    throw LauncherException(ex.msg.c_str());
  }
  if (aMachineList.size() == 0)
    throw LauncherException("This cluster is not in resources catalog");

  // Parsing xml file
  ParseXmlFile(xmlExecuteFile);

  // verify if clustername is in xml file
  map<std::string,MachineParameters>::const_iterator it1 = _launch.MachinesList.find(clusterName);
  if(it1 == _launch.MachinesList.end())
    throw LauncherException("This cluster is not in xml file");

  ParserResourcesType p = _ResManager->GetResourcesList(aMachineList[0]);
  string cname(p.Alias);
#if defined(_DEBUG_) || defined(_DEBUG)
  cout << "Choose cluster: " <<  cname << endl;
#endif

  // search batch manager for that cluster in map or instanciate one
  map < string, Batch::BatchManager_eClient * >::const_iterator it2 = _batchmap.find(cname);
  if(it2 == _batchmap.end())
    {
      _batchmap[cname] = FactoryBatchManager(p);
      // TODO: Add a test for the cluster !
    }
    
  try{

    // directory on cluster to put files to execute
    string remotedir = _launch.MachinesList[clusterName].WorkDirectory;
    // local directory to get files to execute and to put results
    string localdir = _launch.RefDirectory;

    int idx1 = xmlExecuteFile.find_last_of("/");
    if(idx1 == string::npos) idx1 = -1;
    int idx2 = xmlExecuteFile.find(".xml");
    string logfile = xmlExecuteFile.substr(idx1+1,idx2-idx1-1);
    string ologfile = logfile + ".output.log";
    string elogfile = logfile + ".error.log";

    // create and submit job on cluster
    Batch::Parametre param;
    param[USER] = p.UserName;
    param[EXECUTABLE] = "";
    for(int i=0; i<_launch.InputFile.size();i++)
      param[INFILE] += Batch::Couple( localdir + "/" + _launch.InputFile[i], remotedir + "/" + _launch.InputFile[i] );
    for(int i=0; i<_launch.OutputFile.size();i++)
      param[OUTFILE] += Batch::Couple( localdir + "/" + _launch.OutputFile[i], remotedir + "/" + _launch.OutputFile[i] );
    param[OUTFILE] += Batch::Couple( localdir + "/" + ologfile, remotedir + "/" + ologfile );
    param[OUTFILE] += Batch::Couple( localdir + "/" + elogfile, remotedir + "/" + elogfile );
    param[NBPROC] = _launch.NbOfProcesses;
    param[WORKDIR] = remotedir;
    param[TMPDIR] = remotedir;
    param[MAXWALLTIME] = getWallTime("");
    param[MAXRAMSIZE] = getRamSize("");
    param[HOMEDIR] = "";

    Batch::Environnement env;
    env["COMMAND"] = _launch.Command;
    env["SOURCEFILE"] = _launch.MachinesList[clusterName].EnvFile;
    env["LOGFILE"] = logfile;

    Batch::Job* job = new Batch::Job(param,env);

    // submit job on cluster
    Batch::JobId jid = _batchmap[cname]->submitJob(*job);

    // get job id in long
    istringstream iss(jid.getReference());
    iss >> jobId;

    _jobmap[ pair<string,long>(cname,jobId) ] = job;
  }
  catch(const Batch::EmulationException &ex){
    throw LauncherException(ex.message.c_str());
  }

  return jobId;
#else
  throw LauncherException("Method Launcher_cpp::submitJob is not available "
                          "(libBatch was not present at compilation time)");
#endif
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
long Launcher_cpp::submitSalomeJob( const string fileToExecute ,
				    const vector<string>& filesToExport ,
				    const vector<string>& filesToImport ,
				    const batchParams& batch_params,
				    const machineParams& params) throw(LauncherException)
{
#ifdef WITH_LIBBATCH
#if defined(_DEBUG_) || defined(_DEBUG)
  cerr << "BEGIN OF Launcher_cpp::submitSalomeJob" << endl;
#endif
  long jobId;
  vector<string> aMachineList;

  if(!_ResManager)
    throw LauncherException("You must set Resources Manager to Launcher!!");

  // check batch params
  if ( !check(batch_params) )
    throw LauncherException("Batch parameters are bad (see informations above)");

  // find a cluster matching the structure params
  try{
    aMachineList = _ResManager->GetFittingResources(params);
  }
  catch(const ResourcesException &ex){
    throw LauncherException(ex.msg.c_str());
  }
  if (aMachineList.size() == 0)
    throw LauncherException("No resources have been found with your parameters");

  ParserResourcesType p = _ResManager->GetResourcesList(aMachineList[0]);
  string clustername(p.Alias);
#if defined(_DEBUG_) || defined(_DEBUG)
  cerr << "Choose cluster: " <<  clustername << endl;
#endif
  
  // search batch manager for that cluster in map or instanciate one
  map < string, Batch::BatchManager_eClient * >::const_iterator it = _batchmap.find(clustername);
  if(it == _batchmap.end())
    {
      _batchmap[clustername] = FactoryBatchManager(p);
      // TODO: Add a test for the cluster !
    }
    
  try{
    // tmp directory on cluster to put files to execute
    string tmpdir = getTmpDirForBatchFiles();

    // create and submit job on cluster
    Batch::Parametre param;
    param[USER] = p.UserName;
    param[EXECUTABLE] = buildSalomeCouplingScript(fileToExecute,tmpdir,p);
    param[INFILE] = Batch::Couple( fileToExecute, getRemoteFile(tmpdir,fileToExecute) );
    for(int i=0;i<filesToExport.size();i++)
      param[INFILE] += Batch::Couple( filesToExport[i], getRemoteFile(tmpdir,filesToExport[i]) );

    ostringstream file_name_output;
    file_name_output << "~/" << tmpdir << "/" << "output.log*";
    ostringstream file_name_error;
    file_name_error << "~/" << tmpdir << "/" << "error.log*";
    ostringstream file_container_log;
    file_container_log << "~/" << tmpdir << "/" << "YACS_Server*";
    param[OUTFILE] = Batch::Couple( "", file_name_output.str());
    param[OUTFILE] += Batch::Couple( "", file_name_error.str());
    param[OUTFILE] += Batch::Couple( "", file_container_log.str());

    for(int i=0;i<filesToImport.size();i++)
      param[OUTFILE] += Batch::Couple( "", filesToImport[i] );

    param[NBPROC] = batch_params.nb_proc;
    param[WORKDIR] = batch_params.batch_directory;
    param[TMPDIR] = tmpdir;
    param[MAXWALLTIME] = getWallTime(batch_params.expected_during_time);
    param[MAXRAMSIZE] = getRamSize(batch_params.mem);
    param[HOMEDIR] = getHomeDir(p, tmpdir);
    param[QUEUE] = p.batchQueue;

    Batch::Environnement env;

    Batch::Job* job = new Batch::Job(param,env);

    // submit job on cluster
    Batch::JobId jid = _batchmap[clustername]->submitJob(*job);

    // get job id in long
    istringstream iss(jid.getReference());
    iss >> jobId;

    _jobmap[ pair<string,long>(clustername,jobId) ] = job;    
  }
  catch(const Batch::EmulationException &ex){
    throw LauncherException(ex.message.c_str());
  }

  return jobId;
#else
  throw LauncherException("Method Launcher_cpp::submitSalomeJob is not available "
                          "(libBatch was not present at compilation time)");
#endif
}

//=============================================================================
/*! CORBA Method:
 *  Query a batch job on a cluster and returns the status of job
 *  \param jobId              : identification of Salome job
 *  \param params             : Constraints for the choice of the batch cluster
 */
//=============================================================================
string Launcher_cpp::queryJob( long id, 
			       const machineParams& params) throw(LauncherException)
{
#ifdef WITH_LIBBATCH
  if(!_ResManager)
    throw LauncherException("You must set Resources Manager to Launcher!!");

  // find a cluster matching params structure
  vector<string> aMachineList = _ResManager->GetFittingResources( params ) ;
  ParserResourcesType p = _ResManager->GetResourcesList(aMachineList[0]);
  string clustername(p.Alias);
    
  // search batch manager for that cluster in map
  std::map < string, Batch::BatchManager_eClient * >::const_iterator it = _batchmap.find(clustername);
  if(it == _batchmap.end())
    throw LauncherException("no batchmanager for that cluster");
    
  Batch::Parametre par;
  try{
    ostringstream oss;
    oss << id;
    Batch::JobId jobId( _batchmap[clustername], oss.str() );

    Batch::JobInfo jinfo = jobId.queryJob();
    par = jinfo.getParametre();
  }
  catch(const Batch::EmulationException &ex){
    throw LauncherException(ex.message.c_str());
  }

  return par[STATE];
#else
  throw LauncherException("Method Launcher_cpp::queryJob is not available "
                          "(libBatch was not present at compilation time)");
#endif
}

string Launcher_cpp::queryJob( long id, 
			       const std::string clusterName) throw (LauncherException)
{
  machineParams params;
  params.hostname = clusterName;
  return queryJob(id,params);
}

//=============================================================================
/*! CORBA Method:
 *  Delete a batch job on a cluster 
 *  \param jobId              : identification of Salome job
 *  \param params             : Constraints for the choice of the batch cluster
 */
//=============================================================================
void Launcher_cpp::deleteJob( const long id, 
			      const machineParams& params) throw(LauncherException)
{
#ifdef WITH_LIBBATCH
  if(!_ResManager)
    throw LauncherException("You must set Resources Manager to Launcher!!");

  // find a cluster matching params structure
  vector<string> aMachineList = _ResManager->GetFittingResources( params ) ;
  ParserResourcesType p = _ResManager->GetResourcesList(aMachineList[0]);
  string clustername(p.Alias);
    
  // search batch manager for that cluster in map
  map < string, Batch::BatchManager_eClient * >::const_iterator it = _batchmap.find(clustername);
  if(it == _batchmap.end())
    throw LauncherException("no batchmanager for that cluster");
  
  ostringstream oss;
  oss << id;
  Batch::JobId jobId( _batchmap[clustername], oss.str() );

  jobId.deleteJob();
#else
  throw LauncherException("Method Launcher_cpp::deleteJob is not available "
                          "(libBatch was not present at compilation time)");
#endif
}

void Launcher_cpp::deleteJob( long id, 
			      const std::string clusterName) throw (LauncherException)
{
  machineParams params;
  params.hostname = clusterName;
  deleteJob(id,params);
}

//=============================================================================
/*! CORBA Method:
 *  Get result files of job on a cluster
 *  \param jobId              : identification of Salome job
 *  \param params             : Constraints for the choice of the batch cluster
 */
//=============================================================================
void Launcher_cpp::getResultsJob( const string directory,
				  const long id, 
				  const machineParams& params) throw(LauncherException)
{
#ifdef WITH_LIBBATCH
  if(!_ResManager)
    throw LauncherException("You must set Resources Manager to Launcher!!");

  vector<string> aMachineList = _ResManager->GetFittingResources( params ) ;
  ParserResourcesType p = _ResManager->GetResourcesList(aMachineList[0]);
  string clustername(p.Alias);
    
  // search batch manager for that cluster in map
  map < string, Batch::BatchManager_eClient * >::const_iterator it = _batchmap.find(clustername);
  if(it == _batchmap.end())
    throw LauncherException("no batchmanager for that cluster");
    
  Batch::Job* job = _jobmap[ pair<string,long>(clustername,id) ];

  _batchmap[clustername]->importOutputFiles( *job, directory );
#else
  throw LauncherException("Method Launcher_cpp::getResultsJob is not available "
                          "(libBatch was not present at compilation time)");
#endif
}

void Launcher_cpp::getResultsJob( const std::string directory, 
				  long id, 
				  const std::string clusterName) throw (LauncherException)
{
  machineParams params;
  params.hostname = clusterName;
  getResultsJob(directory,id,params);
}

//=============================================================================
/*!
 *  Factory to instanciate the good batch manager for choosen cluster.
 */ 
//=============================================================================

Batch::BatchManager_eClient *Launcher_cpp::FactoryBatchManager( const ParserResourcesType& params ) throw(LauncherException)
{
#ifdef WITH_LIBBATCH
  std::string hostname, mpi;
  Batch::CommunicationProtocolType protocol;
  Batch::FactBatchManager_eClient* fact;

  hostname = params.Alias;
  switch(params.Protocol){
  case rsh:
    protocol = Batch::RSH;
    break;
  case ssh:
    protocol = Batch::SSH;
    break;
  default:
    throw LauncherException("unknown protocol");
    break;
  }
  switch(params.mpi){
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
  case nompi:
    throw LauncherException("you must specified an mpi implementation for batch manager");
    break;
  default:
    throw LauncherException("unknown mpi implementation");
    break;
  }    
#if defined(_DEBUG_) || defined(_DEBUG)
  cerr << "Instanciation of batch manager" << endl;
#endif
  switch( params.Batch ){
  case pbs:
#if defined(_DEBUG_) || defined(_DEBUG)
    cerr << "Instantiation of PBS batch manager" << endl;
#endif
    fact = new Batch::FactBatchManager_ePBS;
    break;
  case lsf:
#if defined(_DEBUG_) || defined(_DEBUG)
    cerr << "Instantiation of LSF batch manager" << endl;
#endif
    fact = new Batch::FactBatchManager_eLSF;
    break;
  case sge:
#if defined(_DEBUG_) || defined(_DEBUG)
    cout << "Instantiation of SGE batch manager" << endl;
#endif
    fact = new Batch::FactBatchManager_eSGE;
    break;
  default:
#if defined(_DEBUG_) || defined(_DEBUG)
    cerr << "BATCH = " << params.Batch << endl;
#endif
    throw LauncherException("no batchmanager for that cluster");
  }
  return (*fact)(hostname.c_str(), protocol, mpi.c_str());
#else
  throw LauncherException("Method Launcher_cpp::FactoryBatchManager is not available "
                          "(libBatch was not present at compilation time)");
#endif
}

string Launcher_cpp::buildSalomeCouplingScript(const string fileToExecute, const string dirForTmpFiles, const ParserResourcesType& params)
{
#ifdef WITH_LIBBATCH
#ifndef WIN32 //TODO: need for porting on Windows
  int idx = dirForTmpFiles.find("Batch/");
  std::string filelogtemp = dirForTmpFiles.substr(idx+6, dirForTmpFiles.length());

  string::size_type p1 = fileToExecute.find_last_of("/");
  string::size_type p2 = fileToExecute.find_last_of(".");
  std::string fileNameToExecute = fileToExecute.substr(p1+1,p2-p1-1);
  std::string TmpFileName = "/tmp/runSalome_" + fileNameToExecute + ".sh";

  MpiImpl* mpiImpl = FactoryMpiImpl(params.mpi);

  ofstream tempOutputFile;
  tempOutputFile.open(TmpFileName.c_str(), ofstream::out );

  // Begin
  tempOutputFile << "#! /bin/sh -f" << endl ;
  tempOutputFile << "cd " ;
  tempOutputFile << params.AppliPath << endl ;
  tempOutputFile << "export SALOME_BATCH=1\n";
  tempOutputFile << "export PYTHONPATH=~/" ;
  tempOutputFile << dirForTmpFiles ;
  tempOutputFile << ":$PYTHONPATH" << endl ;

  // Adding user script
  std::string script = params.userCommands;
  if (script != "")
    tempOutputFile << script << endl;
  // Test node rank
  tempOutputFile << "if test \"" ;
  tempOutputFile << mpiImpl->rank() ;
  tempOutputFile << "\" = \"0\"; then" << endl ;

  // -----------------------------------------------
  // Code for rank 0 : launch runAppli and a container
  // RunAppli
  if(params.ModulesList.size()>0)
    tempOutputFile << "  ./runAppli --terminal --modules=" ;
  else
    tempOutputFile << "  ./runAppli --terminal ";
  for ( int i = 0 ; i < params.ModulesList.size() ; i++ ) {
    tempOutputFile << params.ModulesList[i] ;
    if ( i != params.ModulesList.size()-1 )
      tempOutputFile << "," ;
  }
  tempOutputFile << " --standalone=registry,study,moduleCatalog --ns-port-log="
		 << filelogtemp 
		 << " &\n";

  // Wait NamingService
  tempOutputFile << "  current=0\n"
		 << "  stop=20\n" 
		 << "  while ! test -f " << filelogtemp << "\n"
		 << "  do\n"
		 << "    sleep 2\n"
		 << "    let current=current+1\n"
		 << "    if [ \"$current\" -eq \"$stop\" ] ; then\n"
		 << "      echo Error Naming Service failed ! >&2"
		 << "      exit\n"
		 << "    fi\n"
		 << "  done\n"
		 << "  port=`cat " << filelogtemp << "`\n";
    
  // Wait other containers
  tempOutputFile << "  for ((ip=1; ip < ";
  tempOutputFile << mpiImpl->size();
  tempOutputFile << " ; ip++))" << endl;
  tempOutputFile << "  do" << endl ;
  tempOutputFile << "    arglist=\"$arglist YACS_Server_\"$ip" << endl ;
  tempOutputFile << "  done" << endl ;
  tempOutputFile << "  sleep 5" << endl ;
  tempOutputFile << "  ./runSession waitContainers.py $arglist" << endl ;
  
  // Launch user script
  tempOutputFile << "  ./runSession python ~/" << dirForTmpFiles << "/" << fileNameToExecute << ".py" << endl;

  // Stop application
  tempOutputFile << "  rm " << filelogtemp << "\n"
		 << "  ./runSession shutdownSalome.py" << endl;

  // -------------------------------------
  // Other nodes launch a container
  tempOutputFile << "else" << endl ;

  // Wait NamingService
  tempOutputFile << "  current=0\n"
		 << "  stop=20\n" 
		 << "  while ! test -f " << filelogtemp << "\n"
		 << "  do\n"
		 << "    sleep 2\n"
		 << "    let current=current+1\n"
		 << "    if [ \"$current\" -eq \"$stop\" ] ; then\n"
		 << "      echo Error Naming Service failed ! >&2"
		 << "      exit\n"
		 << "    fi\n"
		 << "  done\n"
		 << "  port=`cat " << filelogtemp << "`\n";

  // Launching container
  tempOutputFile << "  ./runSession SALOME_Container YACS_Server_";
  tempOutputFile << mpiImpl->rank()
		 << " > ~/" << dirForTmpFiles << "/YACS_Server_" 
		 << mpiImpl->rank() << "_container_log." << filelogtemp
		 << " 2>&1\n";
  tempOutputFile << "fi" << endl ;
  tempOutputFile.flush();
  tempOutputFile.close();
  chmod(TmpFileName.c_str(), 0x1ED);
#if defined(_DEBUG_) || defined(_DEBUG)
  cerr << TmpFileName.c_str() << endl;
#endif

  delete mpiImpl;

  return TmpFileName;
#else
  return "";
#endif
    
#else
  throw LauncherException("Method Launcher_cpp::buildSalomeCouplingScript is not available "
                          "(libBatch was not present at compilation time)");
#endif
}

MpiImpl *Launcher_cpp::FactoryMpiImpl(MpiImplType mpi) throw(LauncherException)
{
#ifdef WITH_LIBBATCH
  switch(mpi){
  case lam:
    return new MpiImpl_LAM();
  case mpich1:
    return new MpiImpl_MPICH1();
  case mpich2:
    return new MpiImpl_MPICH2();
  case openmpi:
    return new MpiImpl_OPENMPI();
  case slurm:
    return new MpiImpl_SLURM();
  case prun:
    return new MpiImpl_PRUN();
  case nompi:
    throw LauncherException("you must specify an mpi implementation for batch manager");
  default:
    ostringstream oss;
    oss << mpi << " : not yet implemented";
    throw LauncherException(oss.str().c_str());
  }
#else
  throw LauncherException("Method Launcher_cpp::FactoryMpiImpl is not available "
                          "(libBatch was not present at compilation time)");
#endif
}

string Launcher_cpp::getTmpDirForBatchFiles()
{
#ifdef WITH_LIBBATCH
  string ret;
  string thedate;

  // Adding date to the directory name
  Batch::Date date = Batch::Date(time(0));
  thedate = date.str();
  int lend = thedate.size() ;
  int i = 0 ;
  while ( i < lend ) {
    if ( thedate[i] == '/' || thedate[i] == '-' || thedate[i] == ':' ) {
      thedate[i] = '_' ;
    }
    i++ ;
  }

  ret = string("Batch/");
  ret += thedate;
  return ret;
#else
  throw LauncherException("Method Launcher_cpp::getTmpDirForBatchFiles is not available "
                          "(libBatch was not present at compilation time)");
#endif
}

string Launcher_cpp::getRemoteFile( std::string remoteDir, std::string localFile )
{
  string::size_type pos = localFile.find_last_of("/") + 1;
  int ln = localFile.length() - pos;
  string remoteFile = remoteDir + "/" + localFile.substr(pos,ln);
  return remoteFile;
}

bool Launcher_cpp::check(const batchParams& batch_params)
{
  bool rtn = true;
#if defined(_DEBUG_) || defined(_DEBUG)
  cerr << "Job parameters are :" << endl;
  cerr << "Directory : $HOME/Batch/$date" << endl;
#endif

  // check expected_during_time (check the format)
  std::string edt_info = batch_params.expected_during_time;
  std::string edt_value = batch_params.expected_during_time;
  if (edt_value != "") {
    std::string begin_edt_value = edt_value.substr(0, 2);
    std::string mid_edt_value = edt_value.substr(2, 1);
    std::string end_edt_value = edt_value.substr(3);
  
    long value;
    std::istringstream iss(begin_edt_value);
    if (!(iss >> value)) {
      edt_info = "Error on definition ! : " + edt_value;
      rtn = false;
    }
    else if (value < 0) {
      edt_info = "Error on definition time is negative ! : " + value;
      rtn = false;
    }
    std::istringstream iss_2(end_edt_value);
    if (!(iss_2 >> value)) {
      edt_info = "Error on definition ! : " + edt_value;
      rtn = false;
    }
    else if (value < 0) {
      edt_info = "Error on definition time is negative ! : " + value;
      rtn = false;
    }
    if (mid_edt_value != ":") {
      edt_info = "Error on definition ! :" + edt_value;
      rtn = false;
    }
  }
  else {
    edt_info = "No value given";
  }
#if defined(_DEBUG_) || defined(_DEBUG)
  cerr << "Expected during time : " << edt_info << endl;;
#endif

  // check memory (check the format)
  std::string mem_info;
  std::string mem_value = batch_params.mem;
  if (mem_value != "") {
    std::string begin_mem_value = mem_value.substr(0, mem_value.length()-2);
    long re_mem_value;
    std::istringstream iss(begin_mem_value);
    if (!(iss >> re_mem_value)) {
      mem_info = "Error on definition ! : " + mem_value;
      rtn = false;
    }
    else if (re_mem_value <= 0) {
      mem_info = "Error on definition memory is negative ! : " + mem_value;
      rtn = false;
    }
    std::string end_mem_value = mem_value.substr(mem_value.length()-2);
    if (end_mem_value != "gb" && end_mem_value != "mb") {
      mem_info = "Error on definition, type is bad ! " + mem_value;
      rtn = false;
    }
  }
  else {
    mem_info = "No value given";
  }
#if defined(_DEBUG_) || defined(_DEBUG)
  cerr << "Memory : " << mem_info << endl;
#endif

  // check nb_proc
  std::string nb_proc_info;
  ostringstream nb_proc_value;
  nb_proc_value << batch_params.nb_proc;
  if(batch_params.nb_proc <= 0) {
    nb_proc_info = "Bad value ! nb_proc = ";
    nb_proc_info += nb_proc_value.str();
    rtn = false;
  }
  else {
    nb_proc_info = nb_proc_value.str();
  }
#if defined(_DEBUG_) || defined(_DEBUG)
  cerr << "Nb of processors : " << nb_proc_info << endl;
#endif

  return rtn;
}

long Launcher_cpp::getWallTime(std::string edt)
{
  long hh, mm, ret;

  if( edt.size() == 0 )
    return 0;

  string::size_type pos = edt.find(":");
  string h = edt.substr(0,pos);
  string m = edt.substr(pos+1,edt.size()-pos+1);
  istringstream issh(h);
  issh >> hh;
  istringstream issm(m);
  issm >> mm;
  ret = hh*60 + mm;
  return  ret;
}

long Launcher_cpp::getRamSize(std::string mem)
{
  long mv;

  if( mem.size() == 0 )
    return 0;

  string ram = mem.substr(0,mem.size()-2);
  istringstream iss(ram);
  iss >> mv;
  string unity = mem.substr(mem.size()-2,2);
  if( (unity.find("gb") != string::npos) || (unity.find("GB") != string::npos) )
    return mv*1024;
  else if( (unity.find("mb") != string::npos) || (unity.find("MB") != string::npos) )
    return mv;
  else if( (unity.find("kb") != string::npos) || (unity.find("KB") != string::npos) )
    return mv/1024;
  else if( (unity.find("b") != string::npos) || (unity.find("B") != string::npos) )
    return mv/(1024*1024);
  else
    return 0;
}

void Launcher_cpp::ParseXmlFile(string xmlExecuteFile)
{
  SALOME_Launcher_Handler* handler = new SALOME_Launcher_Handler(_launch);

  const char* aFilePath = xmlExecuteFile.c_str();
  FILE* aFile = fopen(aFilePath, "r");
  
  if (aFile != NULL)
    {
      xmlDocPtr aDoc = xmlReadFile(aFilePath, NULL, 0);
      
      if (aDoc != NULL)
        handler->ProcessXmlDocument(aDoc);
      else{
#if defined(_DEBUG_) || defined(_DEBUG)
        cout << "ResourcesManager_cpp: could not parse file "<< aFilePath << endl;
#endif
      }
      
      // Free the document
      xmlFreeDoc(aDoc);

      fclose(aFile);
    }
  else{
#if defined(_DEBUG_) || defined(_DEBUG)
    cout << "Launcher_cpp: file "<<aFilePath<<" is not readable." << endl;
#endif
  }
  
  delete handler;

}

std::string Launcher_cpp::getHomeDir(const ParserResourcesType& p, const std::string& tmpdir)
{
  std::string home;
  std::string command;
  int idx = tmpdir.find("Batch/");
  std::string filelogtemp = tmpdir.substr(idx+6, tmpdir.length());
  filelogtemp = "/tmp/logs" + filelogtemp + "_home";
  
  if( p.Protocol == rsh )
    command = "rsh ";
  else if( p.Protocol == ssh )
    command = "ssh ";
  else
    throw LauncherException("Unknown protocol");
  if (p.UserName != ""){
    command += p.UserName;
    command += "@";
  }
  command += p.Alias;
  command += " 'echo $HOME' > ";
  command += filelogtemp;
#if defined(_DEBUG_) || defined(_DEBUG)
  std::cerr << command.c_str() << std::endl;
#endif
  int status = system(command.c_str());
  if(status)
    throw LauncherException("Error of launching home command on remote host");
  
  std::ifstream file_home(filelogtemp.c_str());
  std::getline(file_home, home);
  file_home.close();
  return home;
}
