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
/*
 * BatchManager.cxx : 
 *
 * Auteur : Bernard SECHER - CEA/DEN
 * Date   : Juillet 2007
 * Projet : SALOME
 *
 */

#include <iostream>
#include <sstream>
#include <string>
#include <netdb.h>
#include "BatchLight_Job.hxx"
#include "BatchLight_BatchManager.hxx"
#include "Batch_Date.hxx"
using namespace std;

namespace BatchLight {

  // Constructeur
  BatchManager::BatchManager(const batchParams& p) throw(SALOME_Exception) : _params(p)
  {
    SCRUTE(_params.hostname);
    SCRUTE(_params.protocol);
    SCRUTE(_params.username);
    // On verifie que le hostname est correct
    if (!gethostbyname(_params.hostname.c_str())) { // hostname unknown from network
      string msg = "hostname \"";
      msg += _params.hostname;
      msg += "\" unknown from the network";
      throw SALOME_Exception(msg.c_str());
    }
    _mpiImpl = NULL;
  }

  // Destructeur
  BatchManager::~BatchManager()
  {
    MESSAGE("BatchManager destructor "<<_params.hostname);
    std::map < int, const BatchLight::Job * >::const_iterator it;
    for(it=_jobmap.begin();it!=_jobmap.end();it++)
      delete it->second; 
    if(_mpiImpl) delete _mpiImpl;
  }

  // Methode pour le controle des jobs : soumet un job au gestionnaire
  const int BatchManager::submitJob(Job* job)
  {
    BEGIN_OF("BatchManager::submitJob");
    int id;

    // temporary directory on cluster to put input files for job
    setDirForTmpFiles(job);

    // export input files on cluster
    exportInputFiles(job);

    // build salome coupling script for job
    buildSalomeCouplingScript(job);

    // build batch script for job
    buildSalomeBatchScript(job);

    // submit job on cluster
    id = submit(job);

    // register job on map
    _jobmap[id] = job;
    END_OF("BatchManager::submitJob");
    return id;
  }

  void BatchManager::setDirForTmpFiles(BatchLight::Job* job)
  {
    std::string dirForTmpFiles;
    std::string thedate;

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

    dirForTmpFiles += string("Batch/");
    dirForTmpFiles += thedate ;
    job->setDirForTmpFiles(dirForTmpFiles);
  }

  void BatchManager::exportInputFiles(BatchLight::Job* job) throw(SALOME_Exception)
  {
    BEGIN_OF("BatchManager::exportInFiles");
    int status;
    const char * fileToExecute = job->getFileToExecute();
    const Engines::FilesList filesToExportList = job->getFilesToExportList();
    const std::string dirForTmpFiles = job->getDirForTmpFiles();
    std::string command;
    std::string copy_command;

    // Test protocol
    if( _params.protocol == "rsh" )
      copy_command = "rcp ";
    else if( _params.protocol == "ssh" )
      copy_command = "scp ";
    else
      throw SALOME_Exception("Unknown protocol : only rsh and ssh are known !");
    
    // First step : creating batch tmp files directory
    command = _params.protocol;
    command += " ";
    if (_params.username != ""){
      command += _params.username;
      command += "@";
    }
    command += _params.hostname;
    command += " \"mkdir -p ";
    command += dirForTmpFiles;
    command += "\"" ;
    SCRUTE(command.c_str());
    status = system(command.c_str());
    if(status) {
      std::ostringstream oss;
      oss << status;
      std::string ex_mess("Error of connection on remote host ! status = ");
      ex_mess += oss.str();
      throw SALOME_Exception(ex_mess.c_str());
    }

    // Second step : copy fileToExecute into
    // batch tmp files directory
    command = copy_command;
    command += fileToExecute;
    command += " ";
    if (_params.username != ""){
      command += _params.username;
      command += "@";
    }
    command += _params.hostname;
    command += ":";
    command += dirForTmpFiles;
    SCRUTE(command.c_str());
    status = system(command.c_str());
    if(status) {
      std::ostringstream oss;
      oss << status;
      std::string ex_mess("Error of connection on remote host ! status = ");
      ex_mess += oss.str();
      throw SALOME_Exception(ex_mess.c_str());
    }
    
    // Third step : copy filesToExportList into
    // batch tmp files directory
    for (int i = 0 ; i < filesToExportList.length() ; i++ ) {
      command = copy_command;
      command += filesToExportList[i] ;
      command += " ";
      if (_params.username != ""){
	command += _params.username;
	command += "@";
      }
      command += _params.hostname;
      command += ":";
      command += dirForTmpFiles ;
      SCRUTE(command.c_str());
      status = system(command.c_str());
      if(status) {
	std::ostringstream oss;
	oss << status;
	std::string ex_mess("Error of connection on remote host ! status = ");
	ex_mess += oss.str();
	throw SALOME_Exception(ex_mess.c_str());
      }
    }

    END_OF("BatchManager::exportInFiles");
  }

  void BatchManager::importOutputFiles( const char *directory, const CORBA::Long jobId ) throw(SALOME_Exception)
  {
    BEGIN_OF("BatchManager::importOutputFiles");
    string command;
    int status;

    const BatchLight::Job* myJob = _jobmap[jobId];
    Engines::FilesList filesToImportList = myJob->getFilesToImportList();

    for ( int i = 0 ; i < filesToImportList.length() ; i++ ) {
      if( _params.protocol == "rsh" )
	command = "rcp ";
      else if( _params.protocol == "ssh" )
	command = "scp ";
      else
	throw SALOME_Exception("Unknown protocol");
      if (_params.username != ""){
	command += _params.username;
	command += "@";
      }
      command += _params.hostname;
      command += ":";
      command += filesToImportList[i] ;
      command += " ";
      command += directory;
      SCRUTE(command.c_str());
      status = system(command.c_str());
      if(status) 
      {
	// Try to get what we can (logs files)
	// throw SALOME_Exception("Error of connection on remote host");    
	std::string mess("Copy command failed ! status is :");
	ostringstream status_str;
	status_str << status;
	mess += status_str.str();
	INFOS(mess);
      }
    }

    END_OF("BatchManager::importOutputFiles");
  }

  string BatchManager::BuildTemporaryFileName() const
  {
    //build more complex file name to support multiple salome session
    char *temp = new char[19];
    strcpy(temp, "/tmp/command");
    strcat(temp, "XXXXXX");
#ifndef WNT
    mkstemp(temp);
#else
    char aPID[80];
    itoa(getpid(), aPID, 10);
    strcat(temp, aPID);
#endif

    string command(temp);
    delete [] temp;
    command += ".sh";
    return command;
  }

  void BatchManager::RmTmpFile(std::string & TemporaryFileName)
  {
    string command = "rm ";
    command += TemporaryFileName;
    char *temp = strdup(command.c_str());
    int lgthTemp = strlen(temp);
    temp[lgthTemp - 3] = '*';
    temp[lgthTemp - 2] = '\0';
    system(temp);
    free(temp);
  }

  MpiImpl *BatchManager::FactoryMpiImpl(string mpiImpl) throw(SALOME_Exception)
  {
    if(mpiImpl == "lam")
      return new MpiImpl_LAM();
    else if(mpiImpl == "mpich1")
      return new MpiImpl_MPICH1();
    else if(mpiImpl == "mpich2")
      return new MpiImpl_MPICH2();
    else if(mpiImpl == "openmpi")
      return new MpiImpl_OPENMPI();
    else if(mpiImpl == "indif")
      throw SALOME_Exception("you must specify a mpi implementation in CatalogResources.xml file");
    else{
      ostringstream oss;
      oss << mpiImpl << " : not yet implemented";
      throw SALOME_Exception(oss.str().c_str());
    }
  }

}
