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
/*
 * BatchManager_eSGE.cxx : emulation of SGE client
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
#include "Batch_BatchManager_eSGE.hxx"
#include <stdlib.h>
#ifdef WIN32
# include <time.h>
# include <io.h>
#else
#include <libgen.h>
#endif

using namespace std;

namespace Batch {

  BatchManager_eSGE::BatchManager_eSGE(const FactBatchManager * parent, const char * host, const char * protocol, const char * mpiImpl) throw(InvalidArgumentException,ConnexionFailureException) : BatchManager_eClient(parent,host,protocol,mpiImpl)
  {
    // Nothing to do
  }

  // Destructeur
  BatchManager_eSGE::~BatchManager_eSGE()
  {
    // Nothing to do
  }

  // Methode pour le controle des jobs : soumet un job au gestionnaire
  const JobId BatchManager_eSGE::submitJob(const Job & job)
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
    command += "; qsub " ;
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

    string strjob;
    istringstream iss(line);
    iss >> strjob >> strjob >> strjob;

    JobId id(this, strjob);
    return id;
  }

  // Methode pour le controle des jobs : retire un job du gestionnaire
  void BatchManager_eSGE::deleteJob(const JobId & jobid)
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
    command += " \"qdel " ;
    command += iss.str();
    command += "\"";
    cerr << command.c_str() << endl;
    status = system(command.c_str());
    if(status)
      throw EmulationException("Error of connection on remote host");

    cerr << "jobId = " << ref << "killed" << endl;
  }

  // Methode pour le controle des jobs : suspend un job en file d'attente
  void BatchManager_eSGE::holdJob(const JobId & jobid)
  {
    throw EmulationException("Not yet implemented");
  }

  // Methode pour le controle des jobs : relache un job suspendu
  void BatchManager_eSGE::releaseJob(const JobId & jobid)
  {
    throw EmulationException("Not yet implemented");
  }


  // Methode pour le controle des jobs : modifie un job en file d'attente
  void BatchManager_eSGE::alterJob(const JobId & jobid, const Parametre & param, const Environnement & env)
  {
    throw EmulationException("Not yet implemented");
  }

  // Methode pour le controle des jobs : modifie un job en file d'attente
  void BatchManager_eSGE::alterJob(const JobId & jobid, const Parametre & param)
  {
    alterJob(jobid, param, Environnement());
  }

  // Methode pour le controle des jobs : modifie un job en file d'attente
  void BatchManager_eSGE::alterJob(const JobId & jobid, const Environnement & env)
  {
    alterJob(jobid, Parametre(), env);
  }

  // Methode pour le controle des jobs : renvoie l'etat du job
  JobInfo BatchManager_eSGE::queryJob(const JobId & jobid)
  {
    int id;
    istringstream iss(jobid.getReference());
    iss >> id;

    // define name of log file
    string logFile="/tmp/logs/";
    logFile += getenv("USER");
    logFile += "/batchSalome_";

    ostringstream oss;
    oss << this << "_" << id;
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
    command += " \"qstat | grep " ;
    command += iss.str();
    command += "\" > ";
    command += logFile;
    cerr << command.c_str() << endl;
    status = system(command.c_str());
    if(status && status != 256)
      throw EmulationException("Error of connection on remote host");

    JobInfo_eSGE ji = JobInfo_eSGE(id,logFile);
    return ji;
  }

  // Methode pour le controle des jobs : teste si un job est present en machine
  bool BatchManager_eSGE::isRunning(const JobId & jobid)
  {
    throw EmulationException("Not yet implemented");
  }

  void BatchManager_eSGE::buildBatchScript(const Job & job)
  {
#ifndef WIN32
    //TODO porting on Win32 platform
    int status;
    Parametre params = job.getParametre();
    Environnement env = job.getEnvironnement();
    const long nbproc = params[NBPROC];
    const long edt = params[MAXWALLTIME];
    const long mem = params[MAXRAMSIZE];
    const string workDir = params[WORKDIR];
    const std::string dirForTmpFiles = params[TMPDIR];
    const string fileToExecute = params[EXECUTABLE];
    const string home = params[HOMEDIR];
    const std::string queue = params[QUEUE];
    std::string rootNameToExecute;
    std::string fileNameToExecute;
    std::string filelogtemp;
    if( fileToExecute.size() > 0 ){
      string::size_type p1 = fileToExecute.find_last_of("/");
      string::size_type p2 = fileToExecute.find_last_of(".");
      rootNameToExecute = fileToExecute.substr(p1+1,p2-p1-1);
      fileNameToExecute = "~/" + dirForTmpFiles + "/" + string(basename((char *) fileToExecute.c_str()));

      int idx = dirForTmpFiles.find("Batch/");
      filelogtemp = dirForTmpFiles.substr(idx+6, dirForTmpFiles.length());
    }
    else{
      rootNameToExecute = "command";
    }

    ofstream tempOutputFile;
    std::string TmpFileName = createAndOpenTemporaryFile(tempOutputFile);

    tempOutputFile << "#! /bin/sh -f" << endl;
    if (queue != "")
      tempOutputFile << "#$ -q " << queue << endl;
    tempOutputFile << "#$ -pe mpich " << nbproc << endl;
    if( edt > 0 )
      tempOutputFile << "#$ -l h_rt=" << getWallTime(edt) << endl ;
    if( mem > 0 )
      tempOutputFile << "#$ -l h_vmem=" << mem << "M" << endl ;
    if( fileToExecute.size() > 0 ){
      tempOutputFile << "#$ -o " << home << "/" << dirForTmpFiles << "/output.log." << filelogtemp << endl ;
      tempOutputFile << "#$ -e " << home << "/" << dirForTmpFiles << "/error.log." << filelogtemp << endl ;
    }
    else{
      tempOutputFile << "#$ -o " << dirForTmpFiles << "/" << env["LOGFILE"] << ".output.log" << endl ;
      tempOutputFile << "#$ -e " << dirForTmpFiles << "/" << env["LOGFILE"] << ".error.log" << endl ;
    }
    if( workDir.size() > 0 )
      tempOutputFile << "cd " << workDir << endl ;
    if( fileToExecute.size() > 0 ){
      tempOutputFile << _mpiImpl->boot("",nbproc);
      tempOutputFile << _mpiImpl->run("${TMPDIR}/machines",nbproc,fileNameToExecute);
      tempOutputFile << _mpiImpl->halt();
    }
    else{
      tempOutputFile << "source " << env["SOURCEFILE"] << endl ;
      tempOutputFile << env["COMMAND"];
    }

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

    remove(TmpFileName.c_str());
#endif //WIN32
  }

  std::string BatchManager_eSGE::getWallTime(const long edt)
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
