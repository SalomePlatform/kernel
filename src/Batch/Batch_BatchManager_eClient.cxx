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

#include "Batch_BatchManager_eClient.hxx"
#include "Basics_DirUtils.hxx"

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

using namespace std;


namespace Batch {

  BatchManager_eClient::BatchManager_eClient(const Batch::FactBatchManager * parent, const char* host, const char* protocol, const char* mpiImpl) : BatchManager(parent, host), _protocol(protocol), _username("")
  {
    // instanciation of mpi implementation needed to launch executable in batch script
    _mpiImpl = FactoryMpiImpl(mpiImpl);
  }

  // Destructeur
  BatchManager_eClient::~BatchManager_eClient()
  {
    // Nothing to do
    delete _mpiImpl;
  }

  void BatchManager_eClient::exportInputFiles(const Job& job) throw(EmulationException)
  {
    int status;
    Parametre params = job.getParametre();
    Versatile V = params[INFILE];
    Versatile::iterator Vit;
    string command;
    string copy_command;
    _username = string(params[USER]);

    // Test protocol
    if( _protocol == "rsh" )
      copy_command = "rcp ";
    else if( _protocol == "ssh" )
      copy_command = "scp ";
    else
      throw EmulationException("Unknown protocol : only rsh and ssh are known !");

    // First step : creating batch tmp files directory
    command = _protocol;
    command += " ";
    if(_username != ""){
      command += _username;
      command += "@";
    }
    command += _hostname;
    command += " \"mkdir -p ";
    command += string(params[TMPDIR]);
    command += "\"" ;
    cerr << command.c_str() << endl;
    status = system(command.c_str());
    if(status) {
      std::ostringstream oss;
      oss << status;
      std::string ex_mess("Error of connection on remote host ! status = ");
      ex_mess += oss.str();
      throw EmulationException(ex_mess.c_str());
    }

    // Second step : copy fileToExecute into
    // batch tmp files directory
    string executeFile = params[EXECUTABLE];
    if( executeFile.size() > 0 ){
      command = copy_command;
      command += string(params[EXECUTABLE]);
      command += " ";
      if(_username != ""){
	command += _username;
	command += "@";
      }
      command += _hostname;
      command += ":";
      command += string(params[TMPDIR]);
      cerr << command.c_str() << endl;
      status = system(command.c_str());
      if(status) {
	std::ostringstream oss;
	oss << status;
	std::string ex_mess("Error of connection on remote host ! status = ");
	ex_mess += oss.str();
	throw EmulationException(ex_mess.c_str());
      }
    }

    // Third step : copy filesToExportList into
    // batch tmp files directory
    for(Vit=V.begin(); Vit!=V.end(); Vit++) {
      CoupleType cpt  = *static_cast< CoupleType * >(*Vit);
      Couple inputFile = cpt;
      command = copy_command;
      command += inputFile.getLocal();
      command += " ";
      if(_username != ""){
        command += _username;
        command += "@";
      }
      command += _hostname;
      command += ":";
      command += inputFile.getRemote();
      cerr << command.c_str() << endl;
      status = system(command.c_str());
      if(status) {
        std::ostringstream oss;
        oss << status;
        std::string ex_mess("Error of connection on remote host ! status = ");
        ex_mess += oss.str();
        throw EmulationException(ex_mess.c_str());
      }
    }

  }

  void BatchManager_eClient::importOutputFiles( const Job & job, const string directory ) throw(EmulationException)
  {
    string command;
    int status;

    Parametre params = job.getParametre();
    Versatile V = params[OUTFILE];
    Versatile::iterator Vit;

    for(Vit=V.begin(); Vit!=V.end(); Vit++) {
      CoupleType cpt  = *static_cast< CoupleType * >(*Vit);
      Couple outputFile = cpt;
      if( _protocol == "rsh" )
        command = "rcp ";
      else if( _protocol == "ssh" )
        command = "scp ";
      else
        throw EmulationException("Unknown protocol");

      if (_username != ""){
        command += _username;
        command += "@";
      }
      command += _hostname;
      command += ":";
      command += outputFile.getRemote();
      command += " ";
      command += directory;
      cerr << command.c_str() << endl;
      status = system(command.c_str());
      if(status) 
      {
        // Try to get what we can (logs files)
        // throw BatchException("Error of connection on remote host");    
        std::string mess("Copy command failed ! status is :");
        ostringstream status_str;
        status_str << status;
        mess += status_str.str();
        cerr << mess << endl;
      }
    }

  }

  MpiImpl *BatchManager_eClient::FactoryMpiImpl(string mpiImpl) throw(EmulationException)
  {
    if(mpiImpl == "lam")
      return new MpiImpl_LAM();
    else if(mpiImpl == "mpich1")
      return new MpiImpl_MPICH1();
    else if(mpiImpl == "mpich2")
      return new MpiImpl_MPICH2();
    else if(mpiImpl == "openmpi")
      return new MpiImpl_OPENMPI();
    else if(mpiImpl == "slurm")
      return new MpiImpl_SLURM();
    else if(mpiImpl == "prun")
      return new MpiImpl_PRUN();
    else if(mpiImpl == "nompi")
      throw EmulationException("you must specified an mpi implementation for batch manager");
    else{
      ostringstream oss;
      oss << mpiImpl << " : not yet implemented";
      throw EmulationException(oss.str().c_str());
    }
  }

  string BatchManager_eClient::BuildTemporaryFileName() const
  {
    //build more complex file name to support multiple salome session
    string aFileName = Kernel_Utils::GetTmpFileName();
#ifndef WIN32
    aFileName += ".sh";
#else
    aFileName += ".bat";
#endif
    return aFileName;
  }

  void BatchManager_eClient::RmTmpFile(std::string & TemporaryFileName)
  {
#ifdef WIN32
    string command = "del /F ";
#else
    string command = "rm ";
#endif
    command += TemporaryFileName;
    char *temp = strdup(command.c_str());
    int lgthTemp = strlen(temp);
    temp[lgthTemp - 3] = '*';
    temp[lgthTemp - 2] = '\0';
    system(temp);
    free(temp);
  }
}
