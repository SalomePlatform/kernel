#! /usr/bin/env python3
# -*- coding: utf-8 -*-
# Copyright (C) 2017-2024  CEA, EDF
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
""" Easier access to SalomeLauncher"""

import os
import salome
import time

JOB_FILE_NAME = "jobDump.xml"

class Job:
  """
  This class makes an easier access to SalomeLauncher.
  It adds an automatic save of the job's parameters after the launch. The save
  is done into the result directory of the job. It is then possible to reload
  the job from that file. It also provides a waiting loop for the job to finish.
  This proxy also hides the calls to the naming service in order to get the
  instance of SalomeLauncher.
  """

  @staticmethod
  def launch(job_params):
    """ Create, submit and dump a new job to result_dir."""
    myjob = Job()
    launcher = salome.naming_service.Resolve('/SalomeLauncher')
    myjob.launcher = launcher
    result_dir = job_params.result_directory # local path where to copy out_files

    myjob.job_id = launcher.createJob(job_params) #SALOME id of the job
    launcher.launchJob(myjob.job_id) # copy files, run pre_command, submit job
    myjob.saveJob(result_dir)
    return myjob

  @staticmethod
  def reloadJob(result_dir):
    """ Create a new job from a job dumped in result_dir."""
    myjob = Job()
    launcher = salome.naming_service.Resolve('/SalomeLauncher')
    myjob.launcher = launcher
    myjob.job_id = -1
    try:
      job_file_path = os.path.join(result_dir, JOB_FILE_NAME)
      job_string = ""
      with open(job_file_path, "r") as f:
        job_string = f.read()
      myjob.job_id = launcher.restoreJob(job_string)
    except Exception:
      myjob = None
    return myjob

  def saveJob(self, result_dir):
    job_string = self.launcher.dumpJob(self.job_id)
    job_file_path = os.path.join(result_dir, JOB_FILE_NAME)
    if not os.path.exists(result_dir):
      os.makedirs(result_dir)
    with open(job_file_path, "w") as f:
      f.write(job_string)

  def wait(self, sleep_delay=10):
    """ Wait for the end of the job """
    launcher = self.launcher
    job_id = self.job_id
    jobState = launcher.getJobState(job_id)
    while jobState != "FINISHED" and jobState != "FAILED" :
      time.sleep(sleep_delay)
      jobState = launcher.getJobState(job_id)
      print ("Job %d state: %s" % (job_id,jobState))

  def verify(self):
    """ Get the return code of the solver if the job is finished.
        If the job is not finished (submission error or another state),
        return an empty string.
    """
    launcher = self.launcher
    job_id = self.job_id
    jobState = launcher.getJobState(job_id)
    exit_code = ""
    if jobState != "FINISHED" :
      print ("Job has not finished correctly.")
      print ("Job %d state: %s" % (job_id,jobState))
    else :
      job_params = launcher.getJobParameters(job_id)
      temp_dir = job_params.result_directory
      temp_log_dir = os.path.join(temp_dir, "logs")
      if(launcher.getJobWorkFile(job_id, "logs/exit_code.log", temp_log_dir)):
        exit_code_file = os.path.join(temp_log_dir, "exit_code.log")
        if os.path.isfile(exit_code_file):
          with open(exit_code_file) as myfile:
            exit_code = myfile.read()
    return exit_code.strip()

  def getResults(self):
    """ Copy the result files from remote working_directory
        to the local result_directory."""
    self.launcher.getJobResults(self.job_id, "")

  def relaunch(self, script=""):
    job_params = self.launcher.getJobParameters(self.job_id)
    if script:
      job_params.job_file = script
    job_params.pre_command = ""
    self.job_id = self.launcher.createJob(job_params)
    self.launcher.launchJob(self.job_id)
    self.saveJob(job_params.result_directory)
