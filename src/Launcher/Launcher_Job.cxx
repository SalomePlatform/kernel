// Copyright (C) 2009-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

// Author: André RIBES - EDF R&D
//
//#define _DEBUG_
#include "Launcher_Job.hxx"
#include "Launcher.hxx"

#ifdef WITH_LIBBATCH
#include <libbatch/Constants.hxx>
#endif

using namespace std;

Launcher::Job::Job()
{
  _number = -1;
  _state = "CREATED";
  _launch_date = getLaunchDate();

  _env_file = "";
  _job_name = "";
  _job_file = "";
  _job_file_name = "";
  _job_file_name_complete = "";
  _work_directory = "";
  _local_directory = "";
  _result_directory = "";
  _maximum_duration = "";
  _maximum_duration_in_second = -1;
  _queue = "";
  _job_type = "";
  _exclusive = false;
  _mem_per_cpu = 0;

  // Parameters for COORM
  _launcher_file = "";
  _launcher_args = "";

#ifdef WITH_LIBBATCH
  _batch_job = new Batch::Job();
#endif
}

Launcher::Job::~Job()
{
  LAUNCHER_MESSAGE("Deleting job number: " << _number);
#ifdef WITH_LIBBATCH
  if (_batch_job)
    delete _batch_job;
#endif
}

void
Launcher::Job::stopJob()
{
  LAUNCHER_MESSAGE("Stop resquested for job number: " << _number);
  setState("FAILED");
#ifdef WITH_LIBBATCH
  if (_batch_job_id.getReference() != "undefined")
  {
    try
    {
      _batch_job_id.deleteJob();
    }
    catch (const Batch::GenericException &ex)
    {
      LAUNCHER_INFOS("WARNING: exception when stopping the job: " << ex.message);
    }
  }
#endif
}


void
Launcher::Job::removeJob()
{
  LAUNCHER_MESSAGE("Removing job number: " << _number);
#ifdef WITH_LIBBATCH
  if (_batch_job_id.getReference() != "undefined")
  {
    try
    {
      _batch_job_id.deleteJob();
    }
    catch (const Batch::GenericException &ex)
    {
      LAUNCHER_INFOS("WARNING: exception when removing the job: " << ex.message);
    }
  }
#endif
}

std::string
Launcher::Job::getJobType() const
{
  return _job_type;
}

void
Launcher::Job::setJobName(const std::string & job_name)
{
  _job_name = job_name;
}

std::string
Launcher::Job::getJobName() const
{
  return _job_name;
}

void
Launcher::Job::setState(const std::string & state)
{
  // State of a Job: CREATED, QUEUED, RUNNING, FINISHED, FAILED
  if (state != "CREATED" &&
      state != "IN_PROCESS" &&
      state != "QUEUED" &&
      state != "RUNNING" &&
      state != "PAUSED" &&
      state != "FINISHED" &&
      state != "FAILED" &&
      state != "ERROR")
  {
    throw LauncherException("Bad state, this state does not exist: " + state);
  }
  _state = state;
}

std::string
Launcher::Job::getState() const
{
  return _state;
}

// Get names or ids of hosts assigned to the job
std::string
Launcher::Job::getAssignedHostnames()
{
  return _assigned_hostnames;
}

void
Launcher::Job::setNumber(const int & number)
{
  if (_number != -1)
    std::cerr << "Launcher::Job::setNumber -- Job number was already defined, before: " << _number << " now: " << number << std::endl;
  _number = number;
}

int
Launcher::Job::getNumber()
{
  return _number;
}

void
Launcher::Job::setResourceDefinition(const ParserResourcesType & resource_definition)
{
  // Check machine_definition
  std::string user_name = "";
  if (resource_definition.UserName == "")
  {
    user_name = getenv("USER");
    if (user_name == "")
      user_name = getenv("LOGNAME");
    if (user_name == "")
    {
      std::string mess = "You must define a user name: into your resource description or with one of env variables USER/LOGNAME";
      throw LauncherException(mess);
    }
  }
  else
    user_name = resource_definition.UserName;

  _resource_definition = resource_definition;
  _resource_definition.UserName = user_name;
}

ParserResourcesType
Launcher::Job::getResourceDefinition() const
{
  return _resource_definition;
}

void
Launcher::Job::setJobFile(const std::string & job_file)
{
  // Check job file
  if (job_file == "")
  {
    std::string mess = "Empty Job File is forbidden !";
    throw LauncherException(mess);
  }

  _job_file = job_file;
  std::string::size_type p1 = _job_file.find_last_of("/");
  std::string::size_type p2 = _job_file.find_last_of(".");
  _job_file_name_complete = _job_file.substr(p1+1);
  _job_file_name = _job_file.substr(p1+1,p2-p1-1);
}

std::string
Launcher::Job::getJobFile() const
{
  return _job_file;
}
void
Launcher::Job::setEnvFile(const std::string & env_file)
{
  _env_file = env_file;
}

std::string
Launcher::Job::getEnvFile() const
{
  return _env_file;
}

void
Launcher::Job::setWorkDirectory(const std::string & work_directory)
{
  _work_directory = work_directory;
}

void
Launcher::Job::setLocalDirectory(const std::string & local_directory)
{
  _local_directory = local_directory;
}

void
Launcher::Job::setResultDirectory(const std::string & result_directory)
{
  _result_directory = result_directory;
}

void
Launcher::Job::add_in_file(const std::string & file)
{
  std::list<std::string>::iterator it = std::find(_in_files.begin(), _in_files.end(), file);
  if (it == _in_files.end())
    _in_files.push_back(file);
  else
    std::cerr << "Launcher::Job::add_in_file -- Warning file was already entered in in_files: " << file << std::endl;
}

void
Launcher::Job::add_out_file(const std::string & file)
{
  std::list<std::string>::iterator it = std::find(_out_files.begin(), _out_files.end(), file);
  if (it == _out_files.end())
    _out_files.push_back(file);
  else
    std::cerr << "Launcher::Job::add_out_file -- Warning file was already entered in out_files: " << file << std::endl;
}

void
Launcher::Job::setMaximumDuration(const std::string & maximum_duration)
{
  checkMaximumDuration(maximum_duration);
  _maximum_duration_in_second = convertMaximumDuration(maximum_duration);
  _maximum_duration = maximum_duration;
}

// For COORM
void
Launcher::Job::setLauncherFile(const std::string & launcher_file)
{
        _launcher_file = launcher_file;
}
void
Launcher::Job::setLauncherArgs(const std::string & launcher_args)
{
        _launcher_args = launcher_args;
}

void
Launcher::Job::setResourceRequiredParams(const resourceParams & resource_required_params)
{
  checkResourceRequiredParams(resource_required_params);
  _resource_required_params = resource_required_params;
}

void
Launcher::Job::setQueue(const std::string & queue)
{
  _queue = queue;
}

void
Launcher::Job::setExclusive(bool exclusive)
{
  _exclusive = exclusive;
}

void
Launcher::Job::setExclusiveStr(const std::string & exclusiveStr)
{
  if (exclusiveStr == "true")
    _exclusive = true;
  else if (exclusiveStr == "false")
    _exclusive = false;
  else
    throw LauncherException(std::string("Invalid boolean value for exclusive: ") + exclusiveStr);
}

void
Launcher::Job::setMemPerCpu(unsigned long mem_per_cpu)
{
  _mem_per_cpu = mem_per_cpu;
}

void
Launcher::Job::setWCKey(const std::string & wckey)
{
  _wckey = wckey;
}

void
Launcher::Job::setExtraParams(const std::string & extra_params)
{
  _extra_params = extra_params;
}

void
Launcher::Job::setReference(const std::string & reference)
{
  _reference = reference;
}

std::string
Launcher::Job::getWorkDirectory() const
{
  return _work_directory;
}

std::string
Launcher::Job::getLocalDirectory() const
{
  return _local_directory;
}

std::string
Launcher::Job::getResultDirectory() const
{
  return _result_directory;
}

const std::list<std::string> &
Launcher::Job::get_in_files() const
{
  return _in_files;
}

const std::list<std::string> &
Launcher::Job::get_out_files() const
{
  return _out_files;
}

std::string
Launcher::Job::getMaximumDuration() const
{
  return _maximum_duration;
}

// For COORM
std::string
Launcher::Job::getLauncherFile() const
{
        return _launcher_file;
}
std::string
Launcher::Job::getLauncherArgs() const
{
        return _launcher_args;
}

resourceParams
Launcher::Job::getResourceRequiredParams() const
{
  return _resource_required_params;
}

std::string
Launcher::Job::getQueue() const
{
  return _queue;
}

bool
Launcher::Job::getExclusive() const
{
  return _exclusive;
}

std::string
Launcher::Job::getExclusiveStr() const
{
  return _exclusive ? "true" : "false";
}

unsigned long
Launcher::Job::getMemPerCpu() const
{
  return _mem_per_cpu;
}

std::string
Launcher::Job::getWCKey() const
{
  return _wckey;
}

std::string
Launcher::Job::getExtraParams() const
{
  return _extra_params;
}

std::string
Launcher::Job::getReference() const
{
  return _reference;
}

void
Launcher::Job::checkMaximumDuration(const std::string & maximum_duration)
{
  std::string result("");
  std::string edt_value = maximum_duration;
  std::size_t pos = edt_value.find(":");

  if (edt_value != "") {
    if (pos == edt_value.npos) {
      throw LauncherException("[Launcher::Job::checkMaximumDuration] Error on definition: " + edt_value);
    }
    std::string begin_edt_value = edt_value.substr(0, pos);
    std::string mid_edt_value = edt_value.substr(pos, 1);
    std::string end_edt_value = edt_value.substr(pos + 1, edt_value.npos);

    long value;
    std::istringstream iss(begin_edt_value);
    if (!(iss >> value)) {
      result = "[Launcher::Job::checkExpectedDuration] Error on definition ! : " + edt_value;
    }
    else if (value < 0) {
      result = "[Launcher::Job::checkExpectedDuration] Error on definition time is negative ! : " + value;
    }
    std::istringstream iss_2(end_edt_value);
    if (!(iss_2 >> value)) {
      result = "[Launcher::Job::checkExpectedDuration] Error on definition ! : " + edt_value;
    }
    else if (value < 0) {
      result = "[Launcher::Job::checkExpectedDuration] Error on definition time is negative ! : " + value;
    }
    if (mid_edt_value != ":") {
      result = "[Launcher::Job::checkExpectedDuration] Error on definition ! :" + edt_value;
    }
  }
  if (result != "")
    throw LauncherException(result);
}

void
Launcher::Job::checkResourceRequiredParams(const resourceParams & resource_required_params)
{
  // nb_proc has be to > 0
  if (resource_required_params.nb_proc <= 0)
  {
    std::string message("[Launcher::Job::checkResourceRequiredParams] proc number is not > 0 ! ");
    throw LauncherException(message);
  }
}

long
Launcher::Job::convertMaximumDuration(const std::string & edt)
{
  long hh, mm, ret;

  if( edt.size() == 0 )
    return -1;

  std::string::size_type pos = edt.find(":");
  std::string h = edt.substr(0,pos);
  std::string m = edt.substr(pos+1,edt.size()-pos+1);
  std::istringstream issh(h);
  issh >> hh;
  std::istringstream issm(m);
  issm >> mm;
  ret = hh*60 + mm;
  ret = ret * 60;

  return ret;
}

std::string
Launcher::Job::getLaunchDate() const
{
  time_t rawtime;
  time(&rawtime);
  std::string launch_date = ctime(&rawtime);
  int i = 0 ;
  for (;i < launch_date.size(); i++)
    if (launch_date[i] == '/' ||
        launch_date[i] == '-' ||
        launch_date[i] == ':' ||
        launch_date[i] == ' ')
      launch_date[i] = '_';
  launch_date.erase(--launch_date.end()); // Last caracter is a \n

  return launch_date;
}

std::string
Launcher::Job::updateJobState()
{

  if (_state != "FINISHED" &&
      _state != "ERROR"    &&
      _state != "FAILED")
  {
#ifdef WITH_LIBBATCH
    if (_batch_job_id.getReference() != "undefined")
    {
      // A batch manager has been affected to the job
      Batch::JobInfo job_info = _batch_job_id.queryJob();
      Batch::Parametre par = job_info.getParametre();
      _state = par[Batch::STATE].str();
      _assigned_hostnames = (par.find(Batch::ASSIGNEDHOSTNAMES) == par.end())?
                            "" : par[Batch::ASSIGNEDHOSTNAMES].str();
      LAUNCHER_MESSAGE("State received is: " << par[Batch::STATE].str());
    }
#endif
  }
  return _state;
}

#ifdef WITH_LIBBATCH
Batch::Job *
Launcher::Job::getBatchJob()
{
  update_job();
  return _batch_job;
}

Batch::Parametre
Launcher::Job::common_job_params()
{
  Batch::Parametre params;

  params[Batch::NAME] = getJobName();
  params[Batch::NBPROC] = _resource_required_params.nb_proc;
  params[Batch::NBPROCPERNODE] = _resource_required_params.nb_proc_per_node;

  // Memory in megabytes
  if (_resource_required_params.mem_mb > 0)
  {
    params[Batch::MAXRAMSIZE] = _resource_required_params.mem_mb;
  }
  else if (_mem_per_cpu > 0)
  {
    params[Batch::MEMPERCPU] = (long)_mem_per_cpu;
  }

  // We define a default directory
  if (_work_directory == "")
  {
    const size_t BUFSIZE = 32;
    char date[BUFSIZE];
    time_t curtime = time(NULL);
    strftime(date, BUFSIZE, "%Y_%m_%d__%H_%M_%S", localtime(&curtime));
    if(!_resource_definition.working_directory.empty())
    {
      std::string job_dir = std::string("/job_") + date;
      _work_directory = _resource_definition.working_directory + job_dir;
    }
    else
    {
      _work_directory = std::string("/$HOME/Batch/workdir_");
      _work_directory += date;
    }
  }
  params[Batch::WORKDIR] = _work_directory;

  // Parameters for COORM
  params[Batch::LAUNCHER_FILE] = _launcher_file;
  params[Batch::LAUNCHER_ARGS] = _launcher_args;

  // If result_directory is not defined, we use HOME environnement
  if (_result_directory == "")
    _result_directory = getenv("HOME");

  // _in_files
  std::list<std::string> in_files(_in_files);
  in_files.push_back(_job_file);
  if (_env_file != "")
          in_files.push_back(_env_file);
  for(std::list<std::string>::iterator it = in_files.begin(); it != in_files.end(); it++)
  {
    std::string file = *it;

    // local file -> If file is not an absolute path, we apply _local_directory
    std::string local_file;
    if (file.substr(0, 1) == std::string("/"))
      local_file = file;
    else
#ifndef WIN32
      local_file = _local_directory + "/" + file;
#else
      local_file = file;
#endif

    // remote file -> get only file name from in_files
    size_t found = file.find_last_of("/");
    std::string remote_file = _work_directory + "/" + file.substr(found+1);

    params[Batch::INFILE] += Batch::Couple(local_file, remote_file);
  }

  // _out_files
  for(std::list<std::string>::iterator it = _out_files.begin(); it != _out_files.end(); it++)
  {
    std::string file = *it;
    // remote file -> If file is not an absolute path, we apply _work_directory
    std::string remote_file;
    std::string local_file;
    if (file.substr(0, 1) == std::string("/"))
    {
      remote_file = file;
      size_t found = file.find_last_of("/");
      local_file = file.substr(found+1);
    }
    else
    {
      remote_file = _work_directory + "/" + file;
      local_file = file;
    }

    params[Batch::OUTFILE] += Batch::Couple(local_file, remote_file);
  }

  // Time
  if (_maximum_duration_in_second != -1)
    params[Batch::MAXWALLTIME] = _maximum_duration_in_second / 60;

  // Queue
  if (_queue != "")
    params[Batch::QUEUE] = _queue;

  // Exclusive
  if (getExclusive())
    params[Batch::EXCLUSIVE] = true;

  // WC Key
  if (_wckey != "")
    params[Batch::WCKEY] = _wckey;

  // Extra params
  if (_extra_params != "")
    params[Batch::EXTRAPARAMS] = _extra_params;

  // Specific parameters
  std::map<std::string, std::string>::iterator it = _specific_parameters.find("LoalLevelerJobType");
  if (it != _specific_parameters.end())
    params["LL_JOBTYPE"] = it->second;
  return params;
}

void
Launcher::Job::setBatchManagerJobId(Batch::JobId batch_manager_job_id)
{
  _batch_job_id = batch_manager_job_id;
}

Batch::JobId
Launcher::Job::getBatchManagerJobId() const
{
  return _batch_job_id;
}
#endif

void
Launcher::Job::addSpecificParameter(const std::string & name,
                                      const std::string & value)
{
  _specific_parameters[name] = value;
}

const std::map<std::string, std::string> &
Launcher::Job::getSpecificParameters() const
{
  return _specific_parameters;
}

void
Launcher::Job::checkSpecificParameters()
{
}
