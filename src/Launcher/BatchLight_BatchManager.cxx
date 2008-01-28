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
    setDirForTmpFiles();
    SCRUTE(_dirForTmpFiles);

    // export input files on cluster
    exportInputFiles(job->getFileToExecute(),job->getFilesToExportList());

    // build salome coupling script for job
    buildSalomeCouplingScript(job->getFileToExecute());

    // build batch script for job
    buildSalomeBatchScript(job->getNbProc());

    // submit job on cluster
    id = submit();

    // register job on map
    _jobmap[id] = job;
    END_OF("BatchManager::submitJob");
    return id;
  }

  void BatchManager::setDirForTmpFiles()
  {
    int i;

    _dirForTmpFiles = string("Batch/");
    Batch::Date date = Batch::Date(time(0)) ;
    std::string thedate = date.str() ;
    int lend = thedate.size() ;
    i = 0 ;
    while ( i < lend ) {
      if ( thedate[i] == '/' || thedate[i] == '-' || thedate[i] == ':' ) {
        thedate[i] = '_' ;
      }
      i++ ;
    }
    _dirForTmpFiles += thedate ;
  }

  void BatchManager::exportInputFiles(const char *fileToExecute, const Engines::FilesList filesToExportList) throw(SALOME_Exception)
  {
    BEGIN_OF("BatchManager::exportInFiles");
    string command = _params.protocol;
    int status;

    command += " ";

    if (_params.username != ""){
      command += _params.username;
      command += "@";
    }

    command += _params.hostname;
    command += " \"mkdir -p ";
    command += _dirForTmpFiles ;
    command += "\"" ;
    SCRUTE(command.c_str());
    status = system(command.c_str());
    if(status)
      throw SALOME_Exception("Error of connection on remote host");    

    if( _params.protocol == "rsh" )
      command = "rcp ";
    else if( _params.protocol == "ssh" )
      command = "scp ";
    else
      throw SALOME_Exception("Unknown protocol");

    command += fileToExecute;
    command += " ";

    if (_params.username != ""){
      command += _params.username;
      command += "@";
    }

    command += _params.hostname;
    command += ":";
    command += _dirForTmpFiles ;
    SCRUTE(command.c_str());
    status = system(command.c_str());
    if(status)
      throw SALOME_Exception("Error of connection on remote host");    
    
    int i ;
    for ( i = 0 ; i < filesToExportList.length() ; i++ ) {
      if( _params.protocol == "rsh" )
	command = "rcp ";
      else if( _params.protocol == "ssh" )
	command = "scp ";
      else
	throw SALOME_Exception("Unknown protocol");
      command += filesToExportList[i] ;
      command += " ";
      if (_params.username != ""){
	command += _params.username;
	command += "@";
      }
      command += _params.hostname;
      command += ":";
      command += _dirForTmpFiles ;
      SCRUTE(command.c_str());
      status = system(command.c_str());
      if(status)
	throw SALOME_Exception("Error of connection on remote host");    
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
	throw SALOME_Exception("Error of connection on remote host");    
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

  void BatchManager::RmTmpFile()
  {
    if (_TmpFileName != ""){
      string command = "rm ";
      command += _TmpFileName;
      char *temp = strdup(command.c_str());
      int lgthTemp = strlen(temp);
      temp[lgthTemp - 3] = '*';
      temp[lgthTemp - 2] = '\0';
      system(temp);
      free(temp);
    }
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
