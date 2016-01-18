// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
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

#include "BatchTest.hxx"
#include "Launcher.hxx"

#ifdef WITH_LIBBATCH
#include <libbatch/MpiImpl.hxx>
#endif

#include "utilities.h"

#include <sys/stat.h>
#include <iostream>
#include <fstream>
#ifdef WIN32
# include <io.h>
#endif
BatchTest::BatchTest(const Engines::ResourceDefinition& batch_descr)
{
#ifdef WITH_LIBBATCH
  _batch_descr = batch_descr;

  // Getting date
  const size_t BUFSIZE = 32;
  char date[BUFSIZE];
  time_t curtime = time(NULL);
  strftime(date, BUFSIZE, "%Y_%m_%d__%H_%M_%S", localtime(&curtime));

  // Creating test temporary file
  _test_filename =  "/tmp/";
  _test_filename += std::string(date) + "_test_cluster_file_";
  _test_filename += _batch_descr.hostname.in();
  _base_filename = std::string(date) + "_test_cluster_file_" + _batch_descr.hostname.in();
#endif
}

BatchTest::~BatchTest() {}

bool
BatchTest::test()
{
  bool rtn = false;
  INFOS(std::endl
        << "--- Testing batch Machine :" << std::endl
        << "--- Name       : " << _batch_descr.hostname << std::endl
        << "--- hostname      : " << _batch_descr.hostname << std::endl
        << "--- Protocol   : " << _batch_descr.protocol << std::endl
        << "--- User Name  : " << _batch_descr.username << std::endl
        << "--- Batch Type : " << _batch_descr.batch << std::endl
        << "--- MPI Impl   : " << _batch_descr.mpiImpl << std::endl
        << "--- Appli Path : " << _batch_descr.applipath << std::endl
       );

  std::string result_connection("Not Tested");
  std::string result_filecopy("Not Tested");
  std::string result_getresult("Not Tested");
  std::string result_jobsubmit_simple("Not Tested");
  std::string result_jobsubmit_mpi("Not Tested");
  std::string result_appli("Not Tested");

  result_connection = test_connection();
  result_filecopy = test_filecopy();
  result_getresult = test_getresult();
  result_jobsubmit_simple = test_jobsubmit_simple();
  result_jobsubmit_mpi = test_jobsubmit_mpi();
  result_appli = test_appli();

  INFOS(std::endl
        << "--- Test results" << std::endl
        << "--- Connection          : " << result_connection << std::endl
        << "--- File copy           : " << result_filecopy << std::endl
        << "--- Get results         : " << result_getresult << std::endl
        << "--- Submit simple job   : " << result_jobsubmit_simple << std::endl
        << "--- Submit mpi job      : " << result_jobsubmit_mpi << std::endl
        << "--- Application         : " << result_appli << std::endl
       );

  if (result_connection == "OK"       &&
      result_filecopy == "OK"         &&
      result_getresult == "OK"        &&
      result_jobsubmit_simple == "OK" &&
      result_jobsubmit_mpi == "OK"    &&
      result_appli == "OK")
    rtn = true;

  return rtn;
}

// For this test we use : hostname, protocol, username
std::string
BatchTest::test_connection()
{
  int status;
  std::string command;
  std::string result("Failed : ");
  std::string hostname = _batch_descr.hostname.in();
  std::string username = _batch_descr.username.in();
  std::string protocol = _batch_descr.protocol.in();

  // Basic tests
  if(hostname == "")
  {
    result += "hostname is empty !";
    return result;
  }
  if(username == "")
  {
    result += "username is empty !";
    return result;
  }
  if( protocol != "rsh" && protocol != "ssh")
  {
    result += "protocol unknown ! (" + protocol + ")";
    return result;
  }

  // Build command
  command += protocol
          + " "
          + username + "@" + hostname;

  // Test
  status = system(command.c_str());
  if(status) {
    std::ostringstream oss;
    oss << status;
    result += "Error of connection on remote host ! status = ";
    result += oss.str();
    return result;
  }

  result = "OK";
  return result;
}

// For this test we use : hostname, protocol, username
std::string
BatchTest::test_filecopy()
{
  int status;
  std::string home;
  std::string command;
  std::string result("Failed : ");
  std::string hostname = _batch_descr.hostname.in();
  std::string username = _batch_descr.username.in();
  std::string protocol = _batch_descr.protocol.in();

  // Getting home directory
  std::string rst = get_home(&home);
  if(rst != "") {
    result += rst;
    return result;
  }

  // Writing into the tempory file
  command = "echo Hello > " +  _test_filename;
  status = system(command.c_str());
  if(status) {
    std::ostringstream oss;
    oss << status;
    result += "Error in creating tempory file ! status = ";
    result += oss.str();
    return result;
  }

  // Build command
  command = "scp";
  if(protocol == "rsh")
    command = "rcp";
  command += " " + _test_filename + " "
          + username + "@" + hostname + ":" + home;

  // Test
  status = system(command.c_str());
  if(status) {
    std::ostringstream oss;
    oss << status;
    result += "Error in copy file on remote host ! status = ";
    result += oss.str();
    return result;
  }

  result = "OK";
  return result;
}

// For this test we use : hostname, protocol, username
std::string
BatchTest::test_getresult()
{
  int status;
  std::string home;
  std::string command;
  std::string result("Failed : ");
  std::string hostname = _batch_descr.hostname.in();
  std::string username = _batch_descr.username.in();
  std::string protocol = _batch_descr.protocol.in();

  // Getting home directory
  std::string rst = get_home(&home);
  if(rst != "") {
    result += rst;
    return result;
  }

  // Build command
  command = "scp";
  if(protocol == "rsh")
    command = "rcp";
  command += " " + username + "@" + hostname + ":" + home
          + "/" + _base_filename + " " + _test_filename + "_copy";

  // Test
  status = system(command.c_str());
  if(status) {
    std::ostringstream oss;
    oss << status;
    result += "Error in copy file from remote host ! status = ";
    result += oss.str();
    return result;
  }

  // Compare files
  std::ifstream src_file(_test_filename.c_str());
  if (!src_file)
  {
    result += "Error in reading temporary file ! filename = " + _test_filename;
    return result;
  }
  std::string cp_filename = _test_filename + "_copy";
  std::ifstream cp_file(cp_filename.c_str());
  if (!cp_file)
  {
    result += "Error in reading temporary copy file ! filename = " + cp_filename;
    return result;
  }
  std::string src_firstline;
  std::string cp_firstline;
  std::getline(src_file, src_firstline);
  std::getline(cp_file, cp_firstline);
  src_file.close();
  cp_file.close();
  if (src_firstline != cp_firstline)
  {
    result += "Error source file and copy file are not equa ! source = " + src_firstline + " copy = " + cp_firstline;
    return result;
  }

  result = "OK";
  return result;
}

std::string
BatchTest::test_jobsubmit_simple()
{
  int status;
  std::string home;
  std::string command;
  std::string result("Failed : ");
  std::string hostname = _batch_descr.hostname.in();
  std::string username = _batch_descr.username.in();
  std::string protocol = _batch_descr.protocol.in();
  std::string batch_type = _batch_descr.batch.in();

  // Basic test
  if (batch_type == "lsf")
  {
    INFOS("test_jobsubmit_simple not yet implemented for lsf... return OK");
    result = "OK";
    return result;
  }
  if (batch_type == "ccc")
  {
    INFOS("test_jobsubmit_simple not yet implemented for ccc... return OK");
    result = "OK";
    return result;
  }
  if (batch_type == "slurm")
  {
    INFOS("test_jobsubmit_simple not yet implemented for slurm... return OK");
    result = "OK";
    return result;
  }
  if (batch_type == "sge")
  {
    INFOS("test_jobsubmit_simple not yet implemented for sge... return OK");
    result = "OK";
    return result;
  }
  if (batch_type == "oar")
  {
    INFOS("test_jobsubmit_simple not yet implemented for oar... return OK");
    result = "OK";
    return result;
  }
  if (batch_type == "coorm")
  {
    INFOS("test_jobsubmit_simple not yet implemented for coorm... return OK");
    result = "OK";
    return result;
  }
  if (batch_type != "pbs")
  {
    result += "Batch type unknown ! : " + batch_type;
    return result;
  }

  // Getting home directory
  std::string rst = get_home(&home);
  if(rst != "") {
    result += rst;
    return result;
  }

  // PBS test
  std::string _test_file_simple = _test_filename + "_simple";
  std::ofstream file;
  file.open(_test_file_simple.c_str(), std::ofstream::out);
  file << "#!/bin/bash\n"
       << "#PBS -l nodes=1\n"
       << "#PBS -l walltime=00:01:00\n"
       << "#PBS -o " + home + "/" + _date + "_simple_output.log\n"
       << "#PBS -e " + home + "/" + _date + "_simple_error.log\n"
       << "echo Bonjour\n"
       << "echo Error >&2\n";
  file.flush();
  file.close();


  // Build command for copy
  command = "scp";
  if(protocol == "rsh")
    command = "rcp";
  command += " " + _test_file_simple + " "
          + username + "@" + hostname + ":" + home;
  status = system(command.c_str());
  if(status) {
    std::ostringstream oss;
    oss << status;
    result += "Error in copy job file to remote host ! status = ";
    result += oss.str();
    return result;
  }

  // Build command for submit job
  std::string file_job_name = _test_filename + "_jobid";
  command = protocol + " " + username + "@" + hostname + " qsub " + _base_filename + "_simple > " + file_job_name;
  status = system(command.c_str());
  if(status) {
    std::ostringstream oss;
    oss << status;
    result += "Error in sending qsub to remote host ! status = ";
    result += oss.str();
    return result;
  }
  std::string jobid;
  std::ifstream file_job(file_job_name.c_str());
  if (!file_job)
  {
    result += "Error in reading temporary file ! filename = " + file_job_name;
    return result;
  }
  std::getline(file_job, jobid);
  file_job.close();

  // Wait the end of the job
  command = protocol + " " + username + "@" + hostname + " qstat -f " + jobid + " > " + file_job_name;
  bool stop = false;
  while (!stop)
  {
    status = system(command.c_str());
    if(status && status != 153 && status != 256*153)
    {
      std::ostringstream oss;
      oss << status;
      result += "Error in sending qstat to remote host ! status = ";
      result += oss.str();
      return result;
    }

    if(status == 153 || status == 256*153 )
      stop = true;
#ifdef WIN32
    Sleep(1);
#else
    sleep(1);
#endif
  }

  // Build command for getting results
  command = "scp";
  if(protocol == "rsh")
    command = "rcp";
  command += " "
          + username + "@" + hostname + ":" + home + "/" + _date + "_simple* /tmp";
  status = system(command.c_str());
  if(status) {
    std::ostringstream oss;
    oss << status;
    result += "error in getting file result of qsub simple to remote host ! status = ";
    result += oss.str();
    return result;
  }

  // Test results
  std::string normal_input;
  std::string file_normal_name = "/tmp/" + _date + "_simple_output.log";
  std::ifstream file_normal(file_normal_name.c_str());
  if (!file_normal)
  {
    result += "Error in reading temporary file ! filename = " + file_normal_name;
    return result;
  }
  std::getline(file_normal, normal_input);
  file_normal.close();
  if (normal_input != "Bonjour")
  {
    result += "error from simple ouput file ! waiting for Bonjour and get : " + normal_input;
    return result;
  }
  std::string error_input;
  std::string file_error_name = "/tmp/" + _date + "_simple_error.log";
  std::ifstream file_error(file_error_name.c_str());
  if (!file_error)
  {
    result += "Error in reading temporary file ! filename = " + file_error_name;
    return result;
  }
  std::getline(file_error, error_input);
  file_error.close();
  if (error_input != "Error")
  {
    result += "error from simple error file ! waiting for Error and get : " + error_input;
    return result;
  }
  result = "OK";
  return result;
}

std::string
BatchTest::test_jobsubmit_mpi()
{
#ifdef WITH_LIBBATCH
  int status;
  std::string home;
  std::string command;
  MpiImpl * mpiImpl;
  std::string result("Failed : ");
  std::string hostname = _batch_descr.hostname.in();
  std::string username = _batch_descr.username.in();
  std::string protocol = _batch_descr.protocol.in();
  std::string batch_type = _batch_descr.batch.in();
  std::string mpi_type = _batch_descr.mpiImpl.in();

  // Basic test
  if(mpi_type == "lam")
    mpiImpl = new MpiImpl_LAM();
  else if(mpi_type == "mpich1")
    mpiImpl = new MpiImpl_MPICH1();
  else if(mpi_type == "mpich2")
    mpiImpl = new MpiImpl_MPICH2();
  else if(mpi_type == "openmpi")
    mpiImpl = new MpiImpl_OPENMPI();
  else if(mpi_type == "ompi")
    mpiImpl = new MpiImpl_OMPI();
  else if(mpi_type == "slurmmpi")
    mpiImpl = new MpiImpl_SLURM();
  else
  {
    result += "Error MPI impl not supported : " + mpi_type;
    return result;
  }

  // LSF et SGE not yet implemented...
  if (batch_type == "lsf")
  {
    INFOS("test_jobsubmit_simple not yet implemented for lsf... return OK");
    result = "OK";
    return result;
  }

  if (batch_type == "ccc")
  {
    INFOS("test_jobsubmit_simple not yet implemented for ccc... return OK");
    result = "OK";
    return result;
  }

  if (batch_type == "slurm")
  {
    INFOS("test_jobsubmit_simple not yet implemented for slurm... return OK");
    result = "OK";
    return result;
  }

  if (batch_type == "sge")
  {
    INFOS("test_jobsubmit_simple not yet implemented for sge... return OK");
    result = "OK";
    return result;
  }

  // Getting home directory
  std::string rst = get_home(&home);
  if(rst != "") {
    result += rst;
    return result;
  }

  // MPI test
  std::string _test_file_script = _test_filename + "_script";
  std::ofstream file_script;
  file_script.open(_test_file_script.c_str(), std::ofstream::out);
  file_script << "#!/bin/bash\n"
              << "echo HELLO MPI\n";
  file_script.flush();
  file_script.close();
#ifdef WIN32
  _chmod
#else
  chmod
#endif
    (_test_file_script.c_str(), 0x1ED);

  std::string _test_file_mpi = _test_filename + "_mpi";
  std::ofstream file_mpi;
  file_mpi.open(_test_file_mpi.c_str(), std::ofstream::out);
  file_mpi << "#!/bin/bash\n"
           << "#PBS -l nodes=1\n"
           << "#PBS -l walltime=00:01:00\n"
           << "#PBS -o " << home << "/" << _date << "_mpi_output.log\n"
           << "#PBS -e " << home << "/" << _date << "_mpi_error.log\n"
           << mpiImpl->boot("${PBS_NODEFILE}", 1)
           << mpiImpl->run("${PBS_NODEFILE}", 1, _base_filename + "_script")
           << mpiImpl->halt();
  file_mpi.flush();
  file_mpi.close();


  // Build command for copy
  command = "scp";
  if(protocol == "rsh")
    command = "rcp";
  command += " " + _test_file_script + " "
          + username + "@" + hostname + ":" + home;
  status = system(command.c_str());
  if(status) {
    std::ostringstream oss;
    oss << status;
    result += "Error in copy job file to remote host ! status = ";
    result += oss.str();
    return result;
  }
  command = "scp";
  if(protocol == "rsh")
    command = "rcp";
  command += " " + _test_file_mpi + " "
          + username + "@" + hostname + ":" + home;
  status = system(command.c_str());
  if(status) {
    std::ostringstream oss;
    oss << status;
    result += "Error in copy job file to remote host ! status = ";
    result += oss.str();
    return result;
  }

  // Build command for submit job
  std::string file_job_name = _test_filename + "_jobid";
  command = protocol + " " + username + "@" + hostname + " qsub " + _base_filename + "_mpi > " + file_job_name;
  status = system(command.c_str());
  if(status) {
    std::ostringstream oss;
    oss << status;
    result += "Error in sending qsub to remote host ! status = ";
    result += oss.str();
    return result;
  }
  std::string jobid;
  std::ifstream file_job(file_job_name.c_str());
  if (!file_job)
  {
    result += "Error in reading temporary file ! filename = " + file_job_name;
    return result;
  }
  std::getline(file_job, jobid);
  file_job.close();

  // Wait the end of the job
  command = protocol + " " + username + "@" + hostname + " qstat -f " + jobid + " > " + file_job_name;
  bool stop = false;
  while (!stop)
  {
    status = system(command.c_str());
    if(status && status != 153 && status != 256*153)
    {
      std::ostringstream oss;
      oss << status;
      result += "Error in sending qstat to remote host ! status = ";
      result += oss.str();
      return result;
    }

    if(status == 153 || status == 256*153 )
      stop = true;
#ifdef WIN32
    Sleep(1);
#else
    sleep(1);
#endif
  }

  // Build command for getting results
  command = "scp";
  if(protocol == "rsh")
    command = "rcp";
  command += " "
          + username + "@" + hostname + ":" + home + "/" + _date + "_mpi* /tmp";
  status = system(command.c_str());
  if(status) {
    std::ostringstream oss;
    oss << status;
    result += "error in getting file result of qsub mpi from remote host ! status = ";
    result += oss.str();
    return result;
  }

  // Test results
  std::string normal_input;
  std::string file_normal_name = "/tmp/" + _date + "_mpi_output.log";
  std::ifstream file_normal(file_normal_name.c_str());
  if (!file_normal)
  {
    result += "Error in reading temporary file ! filename = " + file_normal_name;
    return result;
  }
  bool test_ok = false;
  while (std::getline(file_normal, normal_input))
  {
    if (normal_input == "HELLO MPI")
      test_ok = true;
  }
  file_normal.close();
  if (!test_ok)
  {
    result += "error from mpi ouput file ! waiting for HELLO MPI please watch /tmp/" + _date + "_mpi_output.log file";
    return result;
  }
  result = "OK";
  return result;
#else
  throw LauncherException("Method BatchTest::test_jobsubmit_mpi is not available "
                          "(libBatch was not present at compilation time)");
#endif
}

std::string
BatchTest::test_appli()
{
  int status;
  std::string home;
  std::string command;
  std::string result("Failed : ");
  std::string hostname = _batch_descr.hostname.in();
  std::string username = _batch_descr.username.in();
  std::string protocol = _batch_descr.protocol.in();
  std::string applipath = _batch_descr.applipath.in();

  // Getting home directory
  std::string rst = get_home(&home);
  if(rst != "") {
    result += rst;
    return result;
  }

  std::string _test_file_appli = _test_filename + "_appli_test";
  std::ofstream file_appli;
  file_appli.open(_test_file_appli.c_str(), std::ofstream::out);
  file_appli << "#!/bin/bash\n"
             << "if [ -f " << applipath << "/salome ]\n"
             << "then\n"
             << "  echo OK\n"
             << "else\n"
             << "  echo NOK\n"
             << "fi\n";
  file_appli.flush();
  file_appli.close();

  // Build command for copy
  command = "scp";
  if(protocol == "rsh")
    command = "rcp";
  command += " " + _test_file_appli + " "
          + username + "@" + hostname + ":" + home;
  status = system(command.c_str());
  if(status) {
    std::ostringstream oss;
    oss << status;
    result += "Error in copy appli test file to remote host ! status = ";
    result += oss.str();
    return result;
  }

  // Launch test
  command = protocol + " " + username + "@" + hostname
          + " sh " + home + "/" + _base_filename + "_appli_test > "
          + _test_filename + "_appli_test_result";

  status = system(command.c_str());
  if(status) {
    std::ostringstream oss;
    oss << status;
    result += "Error in launching appli test on remote host ! status = ";
    result += oss.str();
    return result;
  }

  // Read test result
  std::string rst_appli;
  std::string file_appli_result_name = _test_filename + "_appli_test_result";
  std::ifstream file_appli_result(file_appli_result_name.c_str());
  if (!file_appli_result)
  {
    result += "Error in reading temporary file ! filename = " + file_appli_result_name;
    return result;
  }
  std::getline(file_appli_result, rst_appli);
  file_appli_result.close();

  if (rst_appli != "OK")
  {
    result += "Error checking application on remote host ! result = " + rst;
    return result;
  }

  result = "OK";
  return result;
}

// Useful methods
std::string
BatchTest::get_home(std::string * home)
{
  int status;
  std::string result = "";
  std::string command;
  std::string hostname = _batch_descr.hostname.in();
  std::string username = _batch_descr.username.in();
  std::string protocol = _batch_descr.protocol.in();
  std::string file_home_name = _test_filename + "_home";

  command = protocol + " " + username + "@" + hostname + " 'echo $HOME' > " + file_home_name;
  status = system(command.c_str());
  if(status) {
    std::ostringstream oss;
    oss << status;
    result += "Error in getting home directory ! status = ";
    result += oss.str();
    return result;
  }

  std::ifstream file_home(file_home_name.c_str());
  if (!file_home)
  {
    result += "Error in reading temporary file ! filename = " + file_home_name;
    return result;
  }
  std::getline(file_home, *home);
  file_home.close();
  return result;
}
