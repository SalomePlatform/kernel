#! /usr/bin/env python3
# -*- coding: utf-8 -*-
# Copyright (C) 2014-2021  CEA/DEN, EDF R&D
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

import unittest
import os
import sys
import time
import tempfile
import errno

def mkdir_p(path):
  try:
    os.makedirs(path)
  except OSError as exc:  # Python >2.5
    if exc.errno == errno.EEXIST and os.path.isdir(path):
      pass
    else:
      raise

import pylauncher
def createLauncher():
  launcher = pylauncher.Launcher_cpp()
  launcher.SetResourcesManager(createResourcesManager())
  return launcher

def createResourcesManager():
  # localhost is defined anyway, even if the catalog file does not exist.
  catalog_path = os.environ.get("USER_CATALOG_RESOURCES_FILE", "")
  return pylauncher.ResourcesManager_cpp(catalog_path)

def createJobParameters():
  jp = pylauncher.JobParameters_cpp()
  jp.resource_required = createResourceParameters()
  return jp

def createResourceParameters():
  return pylauncher.resourceParams()

# Test of SalomeLauncher.
# This test should be run in the salome environment, using "salome shell".
# It does not need a salome application running.
# The test will try to launch batch jobs on every available resources which
# have the can_launch_batch_jobs parameter set to True.
# You can use the environment variable USER_CATALOG_RESOURCES_FILE in order to
# define a customised resource catalog.
# If YACS_ROOT_DIR is not set, the test of submitting a YACS schema will be
# skipped.
class TestCompo(unittest.TestCase):
  @classmethod
  def setUpClass(cls):
    # Prepare the test directory
    temp = tempfile.NamedTemporaryFile()
    cls.test_dir = os.path.join(temp.name, "test_dir")
    name = os.path.basename(temp.name)
    temp.close()
    cls.suffix = time.strftime("-%Y-%m-%d-%H-%M-%S")+"-%s"%(os.getpid())
    mkdir_p(cls.test_dir)

    # Get the list of possible ressources
    ressource_param = createResourceParameters()
    ressource_param.can_launch_batch_jobs = True
    rm = createResourcesManager()
    cls.ressources = rm.GetFittingResources(ressource_param)

  def verifyFile(self, path, content):
    try:
      f = open(path, 'r')
      text = f.read()
      f.close()
      self.assertEqual(text, content)
    except IOError as ex:
      self.fail("IO exception:" + str(ex));

  def create_JobParameters(self):
    job_params = createJobParameters()
    job_params.wckey="P11U5:CARBONES" #needed by edf clusters
    job_params.resource_required.nb_proc = 1
    return job_params

  ##############################
  # test of python_salome job
  ##############################
  def test_salome_py_job(self):
    case_test_dir = os.path.join(TestCompo.test_dir, "salome_py")
    mkdir_p(case_test_dir)

    old_dir = os.getcwd()
    os.chdir(case_test_dir)

    # job script
    script_file = "myScript.py"
    job_script_file = os.path.join(case_test_dir, script_file)
    script_text = """#! /usr/bin/env python3
# -*- coding: utf-8 -*-

# verify import salome
import salome
salome.salome_init()

f = open('result.txt', 'w')
f.write("Salut!")
f.close()

import os
os.mkdir("subdir")
f = open(os.path.join("subdir",'autre.txt'), 'w')
f.write("Hello!")
f.close()
"""
    f = open(job_script_file, "w")
    f.write(script_text)
    f.close()

    local_result_dir = os.path.join(case_test_dir, "result_py_job-")
    job_params = self.create_JobParameters()
    job_params.job_type = "python_salome"
    job_params.job_file = job_script_file
    job_params.in_files = []
    job_params.out_files = ["result.txt", "subdir"]

    launcher = createLauncher()

    for resource in self.ressources:
      print("Testing python_salome job on ", resource)
      job_params.result_directory = local_result_dir + resource
      job_params.job_name = "PyJob" + resource
      job_params.resource_required.name = resource
      # use default working directory for this test

      job_id = launcher.createJob(job_params)
      launcher.launchJob(job_id)

      jobState = launcher.getJobState(job_id)
      print("Job %d state: %s" % (job_id,jobState))
      while jobState != "FINISHED" and jobState != "FAILED" :
        time.sleep(5)
        jobState = launcher.getJobState(job_id)
        print("Job %d state: %s" % (job_id,jobState))
        pass

      self.assertEqual(jobState, "FINISHED")

      # getJobResults to default directory (result_directory)
      launcher.getJobResults(job_id, "")
      self.verifyFile(os.path.join(job_params.result_directory, "result.txt"),
                      "Salut!")
      self.verifyFile(os.path.join(job_params.result_directory,
                                   "subdir", "autre.txt"),
                      "Hello!")

      # getJobResults to a specific directory
      mydir = os.path.join(case_test_dir, "custom_result_dir" + resource)
      launcher.getJobResults(job_id, mydir)
      self.verifyFile(os.path.join(mydir, "result.txt"), "Salut!")
      self.verifyFile(os.path.join(mydir, "subdir", "autre.txt"), "Hello!")
      pass #for

    os.chdir(old_dir)

  ##############################
  # test of command job type
  ##############################
  def test_command(self):
    case_test_dir = os.path.join(TestCompo.test_dir, "command")
    mkdir_p(case_test_dir)

    # job script
    data_file = "in.txt"
    script_file = "myEnvScript.py"
    script_text = """#! /usr/bin/env python3
# -*- coding: utf-8 -*-

import os,sys

text_result = os.getenv("ENV_TEST_VAR","")

f = open('result.txt', 'w')
f.write(text_result)
f.close()

in_f = open("in.txt", "r")
in_text = in_f.read()
in_f.close()

os.mkdir("copie")
f = open(os.path.join("copie",'copie.txt'), 'w')
f.write(in_text)
f.close()
"""
    abs_script_file = os.path.join(case_test_dir, script_file)
    f = open(abs_script_file, "w")
    f.write(script_text)
    f.close()
    os.chmod(abs_script_file, 0o755)

    #environment script
    env_file = "myEnv.sh"
    env_text = """export ENV_TEST_VAR="expected"
"""
    f = open(os.path.join(case_test_dir, env_file), "w")
    f.write(env_text)
    f.close()

    # write data file
    f = open(os.path.join(case_test_dir, data_file), "w")
    f.write("to be copied")
    f.close()

    # job params
    local_result_dir = os.path.join(case_test_dir, "result_com_job-")
    job_params = self.create_JobParameters()
    job_params.job_type = "command"
    job_params.job_file = script_file
    job_params.env_file = env_file
    job_params.in_files = [data_file]
    job_params.out_files = ["result.txt", "copie"]
    job_params.local_directory = case_test_dir

    # create and launch the job
    launcher = createLauncher()
    resManager= createResourcesManager()

    for resource in self.ressources:
      print("Testing command job on ", resource)
      job_params.result_directory = local_result_dir + resource
      job_params.job_name = "CommandJob_" + resource
      job_params.resource_required.name = resource

      # use the working directory of the resource
      resParams = resManager.GetResourceDefinition(resource)
      wd = os.path.join(resParams.working_directory,
                        "CommandJob" + self.suffix)
      job_params.work_directory = wd

      job_id = launcher.createJob(job_params)
      launcher.launchJob(job_id)
      # wait for the end of the job
      jobState = launcher.getJobState(job_id)
      print("Job %d state: %s" % (job_id,jobState))
      while jobState != "FINISHED" and jobState != "FAILED" :
        time.sleep(3)
        jobState = launcher.getJobState(job_id)
        print("Job %d state: %s" % (job_id,jobState))
        pass

      # verify the results
      self.assertEqual(jobState, "FINISHED")
      launcher.getJobResults(job_id, "")
      self.verifyFile(os.path.join(job_params.result_directory, "result.txt"),
                      "expected")
      self.verifyFile(os.path.join(job_params.result_directory,
                                   "copie",'copie.txt'),
                      "to be copied")

      # verify getJobWorkFile
      mydir = os.path.join(case_test_dir, "work_dir" + resource)
      success = launcher.getJobWorkFile(job_id, "result.txt", mydir)
      self.assertEqual(success, True)
      self.verifyFile(os.path.join(mydir, "result.txt"), "expected")

      success = launcher.getJobWorkFile(job_id, "copie", mydir)
      self.assertEqual(success, True)
      self.verifyFile(os.path.join(mydir, "copie", "copie.txt"),
                      "to be copied")


  ##############################
  # test of yacs job type
  ##############################
  def test_yacs(self):
    yacs_path = os.getenv("YACS_ROOT_DIR", "")
    if not os.path.isdir(yacs_path):
      self.skipTest("Needs YACS module to run. Please define YACS_ROOT_DIR.")

    case_test_dir = os.path.join(TestCompo.test_dir, "yacs")
    mkdir_p(case_test_dir)

    #environment script
    env_file = "myEnv.sh"
    env_text = """export ENV_TEST_VAR="expected"
"""
    f = open(os.path.join(case_test_dir, env_file), "w")
    f.write(env_text)
    f.close()

    # job script
    script_text = """<?xml version='1.0' encoding='iso-8859-1' ?>
<proc name="newSchema_1">
   <container name="DefaultContainer">
      <property name="container_kind" value="Salome"/>
      <property name="attached_on_cloning" value="0"/>
      <property name="container_name" value="FactoryServer"/>
      <property name="name" value="localhost"/>
   </container>
   <inline name="PyScript0">
      <script><code><![CDATA[import os
text_result = os.getenv("ENV_TEST_VAR","")
f = open('result.txt', 'w')
f.write(text_result)
f.close()
]]></code></script>
      <load container="DefaultContainer"/>
   </inline>
</proc>
"""
    yacs_file = "mySchema.xml"
    job_script_file = os.path.join(case_test_dir, yacs_file)
    f = open(job_script_file, "w")
    f.write(script_text)
    f.close()

    local_result_dir = os.path.join(case_test_dir, "result_yacs_job-")
    job_params = self.create_JobParameters()
    job_params.job_type = "yacs_file"
    job_params.job_file = job_script_file
    job_params.env_file = os.path.join(case_test_dir,env_file)
    job_params.out_files = ["result.txt"]

    # define the interval between two YACS schema dumps (3 seconds)
    #import Engines
    #job_params.specific_parameters = [Engines.Parameter("EnableDumpYACS", "3")]
    job_params.specific_parameters = {"EnableDumpYACS": "3"}

    launcher = createLauncher()
    resManager= createResourcesManager()

    for resource in self.ressources:
      print("Testing yacs job on ", resource)
      job_params.result_directory = local_result_dir + resource
      job_params.job_name = "YacsJob_" + resource
      job_params.resource_required.name = resource

      # use the working directory of the resource
      resParams = resManager.GetResourceDefinition(resource)
      wd = os.path.join(resParams.working_directory,
                        "YacsJob" + self.suffix)
      job_params.work_directory = wd

      job_id = launcher.createJob(job_params)
      launcher.launchJob(job_id)
      jobState = launcher.getJobState(job_id)

      yacs_dump_success = False
      print("Job %d state: %s" % (job_id,jobState))
      while jobState != "FINISHED" and jobState != "FAILED" :
        time.sleep(5)
        jobState = launcher.getJobState(job_id)
#        yacs_dump_success = launcher.getJobWorkFile(job_id, "dumpState_mySchema.xml",
        yacs_dump_success = launcher.getJobDumpState(job_id,
                                              job_params.result_directory)
        print("Job %d state: %s - dump: %s" % (job_id,jobState, yacs_dump_success))
        pass

      self.assertEqual(jobState, "FINISHED")

      # Verify dumpState file is in the results
      self.assertTrue(yacs_dump_success)
      dump_file_path = os.path.join(job_params.result_directory,
                                    "dumpState_mySchema.xml")
      self.assertTrue(os.path.isfile(dump_file_path))

      """
      # Load the schema state from the dump file and verify the state of a node
      import SALOMERuntime
      SALOMERuntime.RuntimeSALOME_setRuntime(1)
      import loader
      schema = loader.YACSLoader().load(job_script_file)
      stateParser = loader.stateParser()
      sl = loader.stateLoader(stateParser, schema)
      sl.parse(dump_file_path)
      # 106 : "DONE" state code
      self.assertEqual(106, schema.getChildByName("PyScript0").getEffectiveState())
      """

      # getJobResults to default directory (result_directory)
      launcher.getJobResults(job_id, "")
      self.verifyFile(os.path.join(job_params.result_directory, "result.txt"),
                      "expected")

  ##############################
  # test of yacs job type using "--init_port" driver option
  ##############################
  def test_yacsopt(self):
    yacs_path = os.getenv("YACS_ROOT_DIR", "")
    if not os.path.isdir(yacs_path):
      self.skipTest("Needs YACS module to run. Please define YACS_ROOT_DIR.")

    case_test_dir = os.path.join(TestCompo.test_dir, "yacs_opt")
    mkdir_p(case_test_dir)

    # job script
    script_text = """<?xml version='1.0' encoding='iso-8859-1' ?>
<proc name="myschema">
   <type name="string" kind="string"/>
   <type name="bool" kind="bool"/>
   <type name="double" kind="double"/>
   <type name="int" kind="int"/>
   <container name="DefaultContainer">
      <property name="container_kind" value="Salome"/>
      <property name="attached_on_cloning" value="0"/>
      <property name="container_name" value="FactoryServer"/>
      <property name="name" value="localhost"/>
   </container>
   <inline name="mynode">
      <script><code><![CDATA[
text_result = "i=%s,d=%s,b=%s,s=%s" % (i,d,b,s)
f = open('result.txt', 'w')
f.write(text_result)
f.close()
]]></code></script>
      <load container="DefaultContainer"/>
      <inport name="i" type="int"/>
      <inport name="d" type="double"/>
      <inport name="b" type="bool"/>
      <inport name="s" type="string"/>
   </inline>
</proc>
"""
    yacs_file = "simpleSchema.xml"
    job_script_file = os.path.join(case_test_dir, yacs_file)
    f = open(job_script_file, "w")
    f.write(script_text)
    f.close()

    local_result_dir = os.path.join(case_test_dir, "result_yacsopt_job-")
    job_params = self.create_JobParameters()
    job_params.job_type = "yacs_file"
    job_params.job_file = job_script_file
    job_params.out_files = ["result.txt"]

    # define the interval between two YACS schema dumps (3 seconds)
    #import Engines
    #job_params.specific_parameters = [Engines.Parameter("YACSDriverOptions",
    #           "-imynode.i=5 -imynode.d=3.7 -imynode.b=False -imynode.s=lili")]
    job_params.specific_parameters = {"YACSDriverOptions":
                 "-imynode.i=5 -imynode.d=3.7 -imynode.b=False -imynode.s=lili"}
    expected_result="i=5,d=3.7,b=False,s=lili"

    launcher = createLauncher()
    resManager= createResourcesManager()

    for resource in self.ressources:
      print("Testing yacs job with options on ", resource)
      job_params.result_directory = local_result_dir + resource
      job_params.job_name = "YacsJobOpt_" + resource
      job_params.resource_required.name = resource

      # use the working directory of the resource
      resParams = resManager.GetResourceDefinition(resource)
      wd = os.path.join(resParams.working_directory,
                        "YacsJobOpt" + self.suffix)
      job_params.work_directory = wd

      job_id = launcher.createJob(job_params)
      launcher.launchJob(job_id)
      jobState = launcher.getJobState(job_id)

      yacs_dump_success = False
      print("Job %d state: %s" % (job_id,jobState))
      while jobState != "FINISHED" and jobState != "FAILED" :
        time.sleep(5)
        jobState = launcher.getJobState(job_id)
        print("Job %d state: %s " % (job_id,jobState))
        pass

      self.assertEqual(jobState, "FINISHED")

      # getJobResults to default directory (result_directory)
      launcher.getJobResults(job_id, "")
      self.verifyFile(os.path.join(job_params.result_directory, "result.txt"),
                      expected_result)

  ############################################
  # test of command job type with pre_command
  ############################################
  def test_command_pre(self):
    case_test_dir = os.path.join(TestCompo.test_dir, "command_pre")
    mkdir_p(case_test_dir)

    # command to be run before the job
    pre_command = "pre_command.sh"
    pre_command_text = "echo 'it works!' > in.txt"
    abs_pre_command_file = os.path.join(case_test_dir, pre_command)
    f = open(abs_pre_command_file, "w")
    f.write(pre_command_text)
    f.close()
    os.chmod(abs_pre_command_file, 0o755)
    
    # job script
    script_file = "myTestScript.py"
    script_text = """#! /usr/bin/env python3
# -*- coding: utf-8 -*-

in_f = open("in.txt", "r")
in_text = in_f.read()
in_f.close()

f = open('result.txt', 'w')
f.write(in_text)
f.close()
"""
    abs_script_file = os.path.join(case_test_dir, script_file)
    f = open(abs_script_file, "w")
    f.write(script_text)
    f.close()
    os.chmod(abs_script_file, 0o755)

    # job params
    local_result_dir = os.path.join(case_test_dir, "result_com_pre_job-")
    job_params = self.create_JobParameters()
    job_params.job_type = "command"
    job_params.job_file = script_file
    job_params.pre_command = pre_command
    job_params.in_files = []
    job_params.out_files = ["result.txt"]
    job_params.local_directory = case_test_dir

    # create and launch the job
    launcher = createLauncher()
    resManager= createResourcesManager()

    for resource in self.ressources:
      print("Testing command job on ", resource)
      job_params.result_directory = local_result_dir + resource
      job_params.job_name = "CommandPreJob_" + resource
      job_params.resource_required.name = resource

      # use the working directory of the resource
      resParams = resManager.GetResourceDefinition(resource)
      wd = os.path.join(resParams.working_directory,
                        "CommandPreJob" + self.suffix)
      job_params.work_directory = wd

      job_id = launcher.createJob(job_params)
      launcher.launchJob(job_id)
      # wait for the end of the job
      jobState = launcher.getJobState(job_id)
      print("Job %d state: %s" % (job_id,jobState))
      while jobState != "FINISHED" and jobState != "FAILED" :
        time.sleep(3)
        jobState = launcher.getJobState(job_id)
        print("Job %d state: %s" % (job_id,jobState))
        pass

      # verify the results
      self.assertEqual(jobState, "FINISHED")
      launcher.getJobResults(job_id, "")
      self.verifyFile(os.path.join(job_params.result_directory, "result.txt"),
                      "it works!\n")

  #################################
  # test of command salome job type
  #################################
  def test_command_salome(self):
    case_test_dir = os.path.join(TestCompo.test_dir, "command_salome")
    mkdir_p(case_test_dir)

    # job script
    data_file = "in.txt"
    script_file = "myEnvScript.py"
    script_text = """#! /usr/bin/env python3
# -*- coding: utf-8 -*-

import os,sys
# verify import salome
import salome

text_result = os.getenv("ENV_TEST_VAR","")

f = open('result.txt', 'w')
f.write(text_result)
f.close()

in_f = open("in.txt", "r")
in_text = in_f.read()
in_f.close()

os.mkdir("copie")
f = open(os.path.join("copie",'copie.txt'), 'w')
f.write(in_text)
f.close()
"""
    abs_script_file = os.path.join(case_test_dir, script_file)
    f = open(abs_script_file, "w")
    f.write(script_text)
    f.close()
    os.chmod(abs_script_file, 0o755)

    #environment script
    env_file = "myEnv.sh"
    env_text = """export ENV_TEST_VAR="expected"
"""
    f = open(os.path.join(case_test_dir, env_file), "w")
    f.write(env_text)
    f.close()

    # write data file
    f = open(os.path.join(case_test_dir, data_file), "w")
    f.write("to be copied")
    f.close()

    # job params
    local_result_dir = os.path.join(case_test_dir, "result_comsalome_job-")
    job_params = self.create_JobParameters()
    job_params.job_type = "command_salome"
    job_params.job_file = script_file
    job_params.env_file = env_file
    job_params.in_files = [data_file]
    job_params.out_files = ["result.txt", "copie"]
    job_params.local_directory = case_test_dir

    # create and launch the job
    launcher = createLauncher()
    resManager= createResourcesManager()

    for resource in self.ressources:
      print("Testing command salome job on ", resource)
      job_params.result_directory = local_result_dir + resource
      job_params.job_name = "CommandSalomeJob_" + resource
      job_params.resource_required.name = resource

      # use the working directory of the resource
      resParams = resManager.GetResourceDefinition(resource)
      wd = os.path.join(resParams.working_directory,
                        "CommandSalomeJob" + self.suffix)
      job_params.work_directory = wd

      job_id = launcher.createJob(job_params)
      launcher.launchJob(job_id)
      # wait for the end of the job
      jobState = launcher.getJobState(job_id)
      print("Job %d state: %s" % (job_id,jobState))
      while jobState != "FINISHED" and jobState != "FAILED" :
        time.sleep(3)
        jobState = launcher.getJobState(job_id)
        print("Job %d state: %s" % (job_id,jobState))
        pass

      # verify the results
      self.assertEqual(jobState, "FINISHED")
      launcher.getJobResults(job_id, "")
      self.verifyFile(os.path.join(job_params.result_directory, "result.txt"),
                      "expected")
      self.verifyFile(os.path.join(job_params.result_directory,
                                   "copie",'copie.txt'),
                      "to be copied")

      # verify getJobWorkFile
      mydir = os.path.join(case_test_dir, "work_dir" + resource)
      success = launcher.getJobWorkFile(job_id, "result.txt", mydir)
      self.assertEqual(success, True)
      self.verifyFile(os.path.join(mydir, "result.txt"), "expected")

      success = launcher.getJobWorkFile(job_id, "copie", mydir)
      self.assertEqual(success, True)
      self.verifyFile(os.path.join(mydir, "copie", "copie.txt"),
                      "to be copied")
      pass
    pass
  pass

if __name__ == '__main__':
    # create study
    unittest.main()
