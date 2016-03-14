#! /usr/bin/env python
# -*- coding: utf-8 -*-

import unittest
import os
import sys
import time

# Test of SalomeLauncher.
# This test should be run in the salome environment, using "salome shell"
# and a salome application should be running.
# The test will try to launch batch jobs on every available ressources which
# have the can_launch_batch_jobs parameter set to True.
# You can use the environment variable USER_CATALOG_RESOURCES_FILE in order to
# define a customised ressource catalog.
# If YACS_ROOT_DIR is not set, the test of submitting a YACS schema will be
# skiped.
class TestCompo(unittest.TestCase):
  @classmethod
  def setUpClass(cls):
    # Prepare the test directory
    import shutil
    cls.test_dir = os.path.join(os.getcwd(), "test_dir")
    cls.suffix = time.strftime("-%Y-%m-%d-%H-%M-%S")
    shutil.rmtree(cls.test_dir, ignore_errors=True)
    os.mkdir(cls.test_dir)
    
    # load catalogs
#    mc = salome.naming_service.Resolve('/Kernel/ModulCatalog')
#    ior = salome.orb.object_to_string(mc)
#    import SALOMERuntime
#    SALOMERuntime.RuntimeSALOME_setRuntime()
#    salome_runtime = SALOMERuntime.getSALOMERuntime()
#    session_catalog = salome_runtime.loadCatalog("session", ior)
#    salome_runtime.addCatalog(session_catalog)

    # Get the list of possible ressources
    ressource_param = salome.ResourceParameters()
    ressource_param.can_launch_batch_jobs = True
    rm = salome.lcc.getResourcesManager()
    cls.ressources = rm.GetFittingResources(ressource_param)

  def verifyFile(self, path, content):
    try:
      f = open(path, 'r')
      text = f.read()
      f.close()
      self.assertEqual(text, content)
    except IOError,ex:
      self.fail("IO exception:" + str(ex));

  ##############################
  # test of python_salome job
  ##############################
  def test_salome_py_job(self):
    case_test_dir = os.path.join(TestCompo.test_dir, "salome_py")
    os.mkdir(case_test_dir)
    
    old_dir = os.getcwd()
    os.chdir(case_test_dir)
    
    # job script
    script_file = "myScript.py"
    job_script_file = os.path.join(case_test_dir, script_file)
    script_text = """#! /usr/bin/env python
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
    
    local_result_dir = os.path.join(case_test_dir, "result_py_job")
    job_params = salome.JobParameters()
    job_params.job_type = "python_salome"
    job_params.job_file = job_script_file
    job_params.in_files = []
    job_params.out_files = ["result.txt", "subdir"]
    job_params.resource_required = salome.ResourceParameters()
    job_params.resource_required.nb_proc = 1
    
    launcher = salome.naming_service.Resolve('/SalomeLauncher')
    
    for resource in self.ressources:
      print "Testing python_salome job on ", resource
      job_params.result_directory = local_result_dir + resource
      job_params.job_name = "PyJob" + resource
      job_params.resource_required.name = resource
      # use default working directory for this test

      job_id = launcher.createJob(job_params)
      launcher.launchJob(job_id)

      jobState = launcher.getJobState(job_id)
      print "Job %d state: %s" % (job_id,jobState)
      while jobState != "FINISHED" and jobState != "FAILED" :
        time.sleep(5)
        jobState = launcher.getJobState(job_id)
        print "Job %d state: %s" % (job_id,jobState)
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
    os.mkdir(case_test_dir)
    
    # job script
    data_file = "in.txt"
    script_file = "myEnvScript.py"
    script_text = """#! /usr/bin/env python
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
    
    #environement script
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
    local_result_dir = os.path.join(case_test_dir, "result_com_job")
    job_params = salome.JobParameters()
    job_params.job_type = "command"
    job_params.job_file = script_file
    job_params.env_file = env_file
    job_params.in_files = [data_file]
    job_params.out_files = ["result.txt", "copie"]
    job_params.local_directory = case_test_dir
    job_params.resource_required = salome.ResourceParameters()
    job_params.resource_required.nb_proc = 1
    
    # create and launch the job
    launcher = salome.naming_service.Resolve('/SalomeLauncher')
    resManager= salome.lcc.getResourcesManager()

    for resource in self.ressources:
      print "Testing command job on ", resource
      job_params.result_directory = local_result_dir + resource
      job_params.job_name = "CommandJob_" + resource
      job_params.resource_required.name = resource

      # use the working directory of the resource
      resParams = resManager.GetResourceDefinition(resource)
      wd = os.path.join(resParams.working_directory,
                        "CommandJob_" + self.suffix)
      job_params.work_directory = wd

      job_id = launcher.createJob(job_params)
      launcher.launchJob(job_id)
      # wait for the end of the job
      jobState = launcher.getJobState(job_id)
      print "Job %d state: %s" % (job_id,jobState)
      while jobState != "FINISHED" and jobState != "FAILED" :
        time.sleep(3)
        jobState = launcher.getJobState(job_id)
        print "Job %d state: %s" % (job_id,jobState)
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
    os.mkdir(case_test_dir)
    
    #environement script
    env_file = "myEnv.sh"
    env_text = """export ENV_TEST_VAR="expected"
"""
    f = open(os.path.join(case_test_dir, env_file), "w")
    f.write(env_text)
    f.close()
    
    # job script
    script_text = """<?xml version='1.0' encoding='iso-8859-1' ?>
<proc name="newSchema_1">
   <property name="DefaultStudyID" value="1"/>
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
    
    local_result_dir = os.path.join(case_test_dir, "result_yacs_job")
    job_params = salome.JobParameters()
    job_params.job_type = "yacs_file"
    job_params.job_file = job_script_file
    job_params.env_file = os.path.join(case_test_dir,env_file)
    job_params.out_files = ["result.txt"]
    
    # define the interval between two YACS schema dumps (3 seconds)
    import Engines
    job_params.specific_parameters = [Engines.Parameter("EnableDumpYACS", "3")]
    job_params.resource_required = salome.ResourceParameters()
    job_params.resource_required.nb_proc = 1

    launcher = salome.naming_service.Resolve('/SalomeLauncher')
    resManager= salome.lcc.getResourcesManager()
    
    for resource in self.ressources:
      print "Testing yacs job on ", resource
      job_params.result_directory = local_result_dir + resource
      job_params.job_name = "YacsJob_" + resource
      job_params.resource_required.name = resource

      # use the working directory of the resource
      resParams = resManager.GetResourceDefinition(resource)
      wd = os.path.join(resParams.working_directory,
                        "YacsJob_" + self.suffix)
      job_params.work_directory = wd

      job_id = launcher.createJob(job_params)
      launcher.launchJob(job_id)
      jobState = launcher.getJobState(job_id)

      yacs_dump_success = False
      print "Job %d state: %s" % (job_id,jobState)
      while jobState != "FINISHED" and jobState != "FAILED" :
        time.sleep(5)
        jobState = launcher.getJobState(job_id)
#        yacs_dump_success = launcher.getJobWorkFile(job_id, "dumpState_mySchema.xml",
        yacs_dump_success = launcher.getJobDumpState(job_id,
                                              job_params.result_directory)
        print "Job %d state: %s - dump: %s" % (job_id,jobState, yacs_dump_success)
        pass

      self.assertEqual(jobState, "FINISHED")

      # Verify dumpState file is in the results
      self.assertTrue(yacs_dump_success)
      dump_file_path = os.path.join(job_params.result_directory,
                                    "dumpState_mySchema.xml")
      self.assertTrue(os.path.isfile(dump_file_path))

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
    os.mkdir(case_test_dir)
    
    # job script
    script_text = """<?xml version='1.0' encoding='iso-8859-1' ?>
<proc name="myschema">
   <property name="DefaultStudyID" value="1"/>
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
    
    local_result_dir = os.path.join(case_test_dir, "result_yacsopt_job")
    job_params = salome.JobParameters()
    job_params.job_type = "yacs_file"
    job_params.job_file = job_script_file
    #job_params.env_file = os.path.join(case_test_dir,env_file)
    job_params.out_files = ["result.txt"]
    
    # define the interval between two YACS schema dumps (3 seconds)
    import Engines
    job_params.specific_parameters = [Engines.Parameter("YACSDriverOptions",
               "-imynode.i=5 -imynode.d=3.7 -imynode.b=False -imynode.s=lili")]
    expected_result="i=5,d=3.7,b=False,s=lili"
    job_params.resource_required = salome.ResourceParameters()
    job_params.resource_required.nb_proc = 1

    launcher = salome.naming_service.Resolve('/SalomeLauncher')
    resManager= salome.lcc.getResourcesManager()
    
    for resource in self.ressources:
      print "Testing yacs job with options on ", resource
      job_params.result_directory = local_result_dir + resource
      job_params.job_name = "YacsJobOpt_" + resource
      job_params.resource_required.name = resource

      # use the working directory of the resource
      resParams = resManager.GetResourceDefinition(resource)
      wd = os.path.join(resParams.working_directory,
                        "YacsJobOpt_" + self.suffix)
      job_params.work_directory = wd

      job_id = launcher.createJob(job_params)
      launcher.launchJob(job_id)
      jobState = launcher.getJobState(job_id)

      yacs_dump_success = False
      print "Job %d state: %s" % (job_id,jobState)
      while jobState != "FINISHED" and jobState != "FAILED" :
        time.sleep(5)
        jobState = launcher.getJobState(job_id)
        print "Job %d state: %s " % (job_id,jobState)
        pass

      self.assertEqual(jobState, "FINISHED")

      # getJobResults to default directory (result_directory)
      launcher.getJobResults(job_id, "")
      self.verifyFile(os.path.join(job_params.result_directory, "result.txt"),
                      expected_result)

if __name__ == '__main__':
    # creat study
    import salome
    salome.salome_init()
    unittest.main()