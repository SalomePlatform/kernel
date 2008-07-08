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
 * BatchManager_eLSF.cxx : emulation of LSF client
 *
 * Auteur : Bernard SECHER - CEA DEN
 * Mail   : mailto:bernard.secher@cea.fr
 * Date   : Thu Apr 24 10:17:22 2008
 * Projet : PAL Salome 
 *
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include "Batch_BatchManager_eLSF.hxx"

namespace Batch {

  BatchManager_eLSF::BatchManager_eLSF(const FactBatchManager * parent, const char * host, const char * protocol, const char * mpiImpl) throw(InvalidArgumentException,ConnexionFailureException) : BatchManager_eClient(parent,host,protocol,mpiImpl)
  {
    // Nothing to do
  }

  // Destructeur
  BatchManager_eLSF::~BatchManager_eLSF()
  {
    // Nothing to do
  }

  // Methode pour le controle des jobs : soumet un job au gestionnaire
  const JobId BatchManager_eLSF::submitJob(const Job & job)
  {
    int status;
    Parametre params = job.getParametre();
    const std::string dirForTmpFiles = params[TMPDIR];
    const string fileToExecute = params[EXECUTABLE];
    string::size_type p1 = fileToExecute.find_last_of("/");
    string::size_type p2 = fileToExecute.find_last_of(".");
    std::string fileNameToExecute = fileToExecute.substr(p1+1,p2-p1-1);

    // export input files on cluster
    exportInputFiles(job);

    // build batch script for job
    buildBatchScript(job);

    // define name of log file
    string logFile="/tmp/logs/";
    logFile += getenv("USER");
    logFile += "/batchSalome_";
    srand ( time(NULL) );
    int ir = rand();
    ostringstream oss;
    oss << ir;
    logFile += oss.str();
    logFile += ".log";

    string command;

    // define command to submit batch
    command = _protocol;
    command += " ";

    if(_username != ""){
      command += _username;
      command += "@";
    }

    command += _hostname;
    command += " \"cd " ;
    command += dirForTmpFiles ;
    command += "; bsub < " ;
    command += fileNameToExecute ;
    command += "_Batch.sh\" > ";
    command += logFile;
    cerr << command.c_str() << endl;
    status = system(command.c_str());
    if(status)
      throw EmulationException("Error of connection on remote host");

    // read id of submitted job in log file
    char line[128];
    FILE *fp = fopen(logFile.c_str(),"r");
    fgets( line, 128, fp);
    fclose(fp);
    
    string sline(line);
    int p10 = sline.find("<");
    int p20 = sline.find(">");
    string strjob = sline.substr(p10+1,p20-p10-1);

    JobId id(this, strjob);
    return id;
  }

  // Methode pour le controle des jobs : retire un job du gestionnaire
  void BatchManager_eLSF::deleteJob(const JobId & jobid)
  {
    int status;
    int ref;
    istringstream iss(jobid.getReference());
    iss >> ref;
    
    // define command to submit batch
    string command;
    command = _protocol;
    command += " ";

    if (_username != ""){
      command += _username;
      command += "@";
    }

    command += _hostname;
    command += " \"bkill " ;
    command += iss.str();
    command += "\"";
    cerr << command.c_str() << endl;
    status = system(command.c_str());
    if(status)
      throw EmulationException("Error of connection on remote host");

    cerr << "jobId = " << ref << "killed" << endl;
  }
   
  // Methode pour le controle des jobs : suspend un job en file d'attente
  void BatchManager_eLSF::holdJob(const JobId & jobid)
  {
    throw EmulationException("Not yet implemented");
  }

  // Methode pour le controle des jobs : relache un job suspendu
  void BatchManager_eLSF::releaseJob(const JobId & jobid)
  {
    throw EmulationException("Not yet implemented");
  }


  // Methode pour le controle des jobs : modifie un job en file d'attente
  void BatchManager_eLSF::alterJob(const JobId & jobid, const Parametre & param, const Environnement & env)
  {
    throw EmulationException("Not yet implemented");
  }

  // Methode pour le controle des jobs : modifie un job en file d'attente
  void BatchManager_eLSF::alterJob(const JobId & jobid, const Parametre & param)
  {
    alterJob(jobid, param, Environnement());
  }

  // Methode pour le controle des jobs : modifie un job en file d'attente
  void BatchManager_eLSF::alterJob(const JobId & jobid, const Environnement & env)
  {
    alterJob(jobid, Parametre(), env);
  }

  // Methode pour le controle des jobs : renvoie l'etat du job
  JobInfo BatchManager_eLSF::queryJob(const JobId & jobid)
  {
    int id;
    istringstream iss(jobid.getReference());
    iss >> id;

    // define name of log file
    string logFile="/tmp/logs/";
    logFile += getenv("USER");
    logFile += "/batchSalome_";

    srand ( time(NULL) );
    int ir = rand();
    ostringstream oss;
    oss << ir;
    logFile += oss.str();
    logFile += ".log";

    string command;
    int status;

    // define command to submit batch
    command = _protocol;
    command += " ";

    if (_username != ""){
      command += _username;
      command += "@";
    }

    command += _hostname;
    command += " \"bjobs " ;
    command += iss.str();
    command += "\" > ";
    command += logFile;
    cerr << command.c_str() << endl;
    status = system(command.c_str());
    if(status)
      throw EmulationException("Error of connection on remote host");

    JobInfo_eLSF ji = JobInfo_eLSF(id,logFile);
    return ji;
  }



  // Methode pour le controle des jobs : teste si un job est present en machine
  bool BatchManager_eLSF::isRunning(const JobId & jobid)
  {
    throw EmulationException("Not yet implemented");
  }

  void BatchManager_eLSF::buildBatchScript(const Job & job) throw(EmulationException)
  {
    int status;
    Parametre params = job.getParametre();
    const int nbproc = params[NBPROC];
    const long edt = params[MAXWALLTIME];
    const long mem = params[MAXRAMSIZE];
    const string workDir = params[WORKDIR];
    const std::string dirForTmpFiles = params[TMPDIR];
    const string fileToExecute = params[EXECUTABLE];
    string::size_type p1 = fileToExecute.find_last_of("/");
    string::size_type p2 = fileToExecute.find_last_of(".");
    std::string rootNameToExecute = fileToExecute.substr(p1+1,p2-p1-1);
    std::string fileNameToExecute = "~/" + dirForTmpFiles + "/" + string(basename(fileToExecute.c_str()));

    int idx = dirForTmpFiles.find("Batch/");
    std::string filelogtemp = dirForTmpFiles.substr(idx+6, dirForTmpFiles.length());

    std::string TmpFileName = BuildTemporaryFileName();
    ofstream tempOutputFile;
    tempOutputFile.open(TmpFileName.c_str(), ofstream::out );

    tempOutputFile << "#! /bin/sh -f" << endl ;
    if( edt > 0 )
      tempOutputFile << "#BSUB -W " << getWallTime(edt) << endl ;
    if( mem > 0 )
      tempOutputFile << "#BSUB -M " << mem*1024 << endl ;
    tempOutputFile << "#BSUB -n " << nbproc << endl ;
    tempOutputFile << "#BSUB -o runSalome.output.log." << filelogtemp << endl ;
    tempOutputFile << "#BSUB -e runSalome.error.log." << filelogtemp << endl ;
    if( workDir.size() > 0 )
      tempOutputFile << "cd " << workDir << endl ;
    tempOutputFile << _mpiImpl->boot("",nbproc);
    tempOutputFile << _mpiImpl->run("",nbproc,fileNameToExecute);
    tempOutputFile << _mpiImpl->halt();
    tempOutputFile.flush();
    tempOutputFile.close();
    chmod(TmpFileName.c_str(), 0x1ED);
    cerr << TmpFileName.c_str() << endl;

    string command;
    if( _protocol == "rsh" )
      command = "rcp ";
    else if( _protocol == "ssh" )
      command = "scp ";
    else
      throw EmulationException("Unknown protocol");
    command += TmpFileName;
    command += " ";
    if(_username != ""){
      command +=  _username;
      command += "@";
    }
    command += _hostname;
    command += ":";
    command += dirForTmpFiles ;
    command += "/" ;
    command += rootNameToExecute ;
    command += "_Batch.sh" ;
    cerr << command.c_str() << endl;
    status = system(command.c_str());
    if(status)
      throw EmulationException("Error of connection on remote host");    

    RmTmpFile(TmpFileName);
    
  }

  std::string BatchManager_eLSF::getWallTime(const long edt)
  {
    long h, m;
    h = edt / 60;
    m = edt - h*60;
    ostringstream oss;
    if( m >= 10 )
      oss << h << ":" << m;
    else
      oss << h << ":0" << m;
    return oss.str();
  }

}