#! /usr/bin/env python3
# -*- coding: utf-8 -*-
# classic use case of a job
import os
import salome
import tempfile
import time
import sys

if __name__ == '__main__':
  salome.salome_init()
  launcher = salome.naming_service.Resolve('/SalomeLauncher')
  job_params = salome.JobParameters()
  job_params.resource_required = salome.ResourceParameters()
  job_params.resource_required.name = "localhost"
  job_params.resource_required.nb_proc = 1 # slurm: --ntasks

  job_params.job_type = "command"
  #cwd = os.getcwd()
  cwd = sys.argv[1]
  case_dir = tempfile.mkdtemp(prefix="test")
  job_params.local_directory = cwd
  job_params.job_file = "command.sh"
  job_params.work_directory = os.path.join(case_dir, "run")
  job_params.result_directory = os.path.join(case_dir, "result")
  job_params.out_files = ["result.txt"]
  job_params.wckey="P11U5:CARBONES"
  job_params.job_name = "MyJob"
  job_id = launcher.createJob(job_params)
  launcher.launchJob(job_id)
  job_string = launcher.dumpJob(job_id)
  jobState = launcher.getJobState(job_id)
  while jobState != 'FINISHED' and jobState != 'FAILED':
      time.sleep(1)
      jobState = launcher.getJobState(job_id)
  launcher.getJobResults(job_id, '')
  launcher.getJobWorkFile(job_id, "result.txt", '')
  launcher.clearJobWorkingDir(job_id)
  launcher.removeJob(job_id)
  new_id = launcher.restoreJob(job_string)
  job_params_bis = launcher.getJobParameters(new_id)
  jobState = launcher.getJobState(new_id)
  launcher.removeJob(new_id)
  launcher.saveJobs(os.path.join(case_dir,"savejobs.xml"))
