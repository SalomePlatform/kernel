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

#include "BatchLight_BatchManager_SLURM.hxx"
#include "utilities.h"
#include "BatchLight_Job.hxx"
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/stat.h>

using namespace std;

namespace BatchLight {

  // Constructeur
  BatchManager_SLURM::BatchManager_SLURM(const batchParams& p) throw(SALOME_Exception) : BatchManager(p)
  {
  }

  // Destructeur
  BatchManager_SLURM::~BatchManager_SLURM()
  {
    MESSAGE("BatchManager_SLURM destructor "<<_params.hostname);
  }

  // Methode pour le controle des jobs : retire un job du gestionnaire
  void BatchManager_SLURM::deleteJob(const int & jobid)
  {
    BEGIN_OF("BatchManager_SLURM::deleteJob");
    string command;
    int status;
    ostringstream oss;
    oss << jobid;

    // define command to submit batch
    if( _params.protocol == "rsh" )
      command = "rsh ";
    else if( _params.protocol == "ssh" )
      command = "ssh ";
    else
      throw SALOME_Exception("Unknown protocol");

    if (_params.username != ""){
      command += _params.username;
      command += "@";
    }

    command += _params.hostname;
    command += " \"bkill " ;
    command += oss.str();
    command += "\"";
    SCRUTE(command.c_str());
    status = system(command.c_str());
    if(status)
      throw SALOME_Exception("Error of connection on remote host");

    MESSAGE("jobId = " << jobid << "killed");
    END_OF("BatchManager_SLURM::deleteJob");
  }
   
  // Methode pour le controle des jobs : renvoie l'etat du job
  string BatchManager_SLURM::queryJob(const int & jobid)
  {
    BEGIN_OF("BatchManager_SLURM::queryJob");
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
    if( _params.protocol == "rsh" )
      command = "rsh ";
    else if( _params.protocol == "ssh" )
      command = "ssh ";
    else
      throw SALOME_Exception("Unknown protocol");

    if (_params.username != ""){
      command += _params.username;
      command += "@";
    }

    command += _params.hostname;
    command += " \"bjobs " ;
    ostringstream oss2;
    oss2 << jobid;
    command += oss2.str();
    command += "\" > ";
    command += logFile;
    SCRUTE(command.c_str());
    status = system(command.c_str());
    if(status)
      throw SALOME_Exception("Error of connection on remote host");

    // read staus of job in log file
    char line[128];
    ifstream fp(logFile.c_str(),ios::in);
    fp.getline(line,80,'\n');
    
    string sjobid, username, jstatus;
    fp >> sjobid;
    fp >> username;
    fp >> jstatus;

    MESSAGE("jobId = " << jobid << " " << jstatus);
    END_OF("BatchManager_SLURM::queryJob");
    return jstatus;
  }

  void BatchManager_SLURM::buildSalomeCouplingScript( const char *fileToExecute ) throw(SALOME_Exception)
  {
    BEGIN_OF("BatchManager_SLURM::buildSalomeCouplingScript");
    int status;

    string::size_type p1 = string(fileToExecute).find_last_of("/");
    string::size_type p2 = string(fileToExecute).find_last_of(".");
    _fileNameToExecute = string(fileToExecute).substr(p1+1,p2-p1-1);

    _TmpFileName = BuildTemporaryFileName();
    ofstream tempOutputFile;
    tempOutputFile.open(_TmpFileName.c_str(), ofstream::out );
    tempOutputFile << "#! /bin/sh -f" << endl ;
    tempOutputFile << "cd " ;
    tempOutputFile << _params.applipath << endl ;
    tempOutputFile << "export PYTHONPATH=~/" ;
    tempOutputFile << _dirForTmpFiles ;
    tempOutputFile << ":$PYTHONPATH" << endl ;
    tempOutputFile << "if test $SLURM_PROCID = 0; then" << endl ;
    tempOutputFile << "  ./runAppli --terminal --modules=" ;
    for ( int i = 0 ; i < _params.modulesList.size() ; i++ ) {
      tempOutputFile << _params.modulesList[i] ;
      if ( i != _params.modulesList.size()-1 )
	tempOutputFile << "," ;
    }
    tempOutputFile << " --standalone=registry,study,moduleCatalog --killall &" << endl ;
    tempOutputFile << "  for ((ip=1; ip < ${SLURM_NPROCS} ; ip++))" << endl;
    tempOutputFile << "  do" << endl ;
    tempOutputFile << "    arglist=\"$arglist YACS_Server_\"$ip" << endl ;
    tempOutputFile << "  done" << endl ;
    tempOutputFile << "  ./runSession waitNS.sh" << endl ;
    tempOutputFile << "  ./runSession waitContainers.py $arglist" << endl ;
    tempOutputFile << "  ./runSession python ~/" << _dirForTmpFiles << "/" << _fileNameToExecute << ".py" << endl;
    tempOutputFile << "  ./runSession killCurrentPort" << endl;
    tempOutputFile << "else" << endl ;
    tempOutputFile << "  ./runSession waitNS.sh" << endl ;
    tempOutputFile << "  ./runSession SALOME_Container 'YACS_Server_'${SLURM_PROCID}" << endl ;
    tempOutputFile << "fi" << endl ;
    tempOutputFile.flush();
    tempOutputFile.close();
    chmod(_TmpFileName.c_str(), 0x1ED);
    SCRUTE(_TmpFileName.c_str()) ;

    string command;
    if( _params.protocol == "rsh" )
      command = "rcp ";
    else if( _params.protocol == "ssh" )
      command = "scp ";
    else
      throw SALOME_Exception("Unknown protocol");
    
    command += _TmpFileName;
    command += " ";
    if (_params.username != ""){
      command += _params.username;
      command += "@";
    }
    command += _params.hostname;
    command += ":";
    command += _dirForTmpFiles ;
    command += "/runSalome_" ;
    command += _fileNameToExecute ;
    command += "_Batch.sh" ;
    SCRUTE(command.c_str());
    status = system(command.c_str());
    if(status)
      throw SALOME_Exception("Error of connection on remote host");    
    RmTmpFile();
    
    END_OF("BatchManager_SLURM::buildSalomeCouplingScript");
  }

  void BatchManager_SLURM::buildSalomeBatchScript( const int nbproc ) throw(SALOME_Exception)
  {
    BEGIN_OF("BatchManager_SLURM::buildSalomeBatchScript");
    int status;
    _TmpFileName = BuildTemporaryFileName();
    ofstream tempOutputFile;
    tempOutputFile.open(_TmpFileName.c_str(), ofstream::out );

    tempOutputFile << "#! /bin/sh -f" << endl ;
    tempOutputFile << "#BSUB -n " << nbproc << endl ;
    tempOutputFile << "#BSUB -o " << _dirForTmpFiles << "/runSalome.log%J" << endl ;
    tempOutputFile << "mpirun -srun ~/" << _dirForTmpFiles << "/runSalome_" << _fileNameToExecute << "_Batch.sh" << endl ;
    tempOutputFile.flush();
    tempOutputFile.close();
    chmod(_TmpFileName.c_str(), 0x1ED);
    SCRUTE(_TmpFileName.c_str()) ;

    string command;
    if( _params.protocol == "rsh" )
      command = "rcp ";
    else if( _params.protocol == "ssh" )
      command = "scp ";
    else
      throw SALOME_Exception("Unknown protocol");
    command += _TmpFileName;
    command += " ";
    if (_params.username != ""){
      command += _params.username;
      command += "@";
    }
    command += _params.hostname;
    command += ":";
    command += _dirForTmpFiles ;
    command += "/" ;
    command += _fileNameToExecute ;
    command += "_Batch.sh" ;
    SCRUTE(command.c_str());
    status = system(command.c_str());
    if(status)
      throw SALOME_Exception("Error of connection on remote host");    

    RmTmpFile();
    END_OF("BatchManager_SLURM::buildSalomeBatchScript");
    
  }

  int BatchManager_SLURM::submit() throw(SALOME_Exception)
  {
    BEGIN_OF("BatchManager_SLURM::submit");

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
    if( _params.protocol == "rsh" )
      command = "rsh ";
    else if( _params.protocol == "ssh" )
      command = "ssh ";
    else
      throw SALOME_Exception("Unknown protocol");

    if (_params.username != ""){
      command += _params.username;
      command += "@";
    }

    command += _params.hostname;
    command += " \"bsub < " ;
    command += _dirForTmpFiles ;
    command += "/" ;
    command += _fileNameToExecute ;
    command += "_Batch.sh\" > ";
    command += logFile;
    SCRUTE(command.c_str());
    status = system(command.c_str());
    if(status)
      throw SALOME_Exception("Error of connection on remote host");

    // read id of submitted job in log file
    char line[128];
    FILE *fp = fopen(logFile.c_str(),"r");
    fgets( line, 128, fp);
    fclose(fp);
    
    string sline(line);
    int p1 = sline.find("<");
    int p2 = sline.find(">");
    string strjob = sline.substr(p1+1,p2-p1-1);

    int id;
    istringstream iss(strjob);
    iss >> id;

    END_OF("BatchManager_SLURM::submit");
    return id;
  }

}
