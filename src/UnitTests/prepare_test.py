#! /usr/bin/env python3
# -*- coding: utf-8 -*-
# Copyright (C) 2014-2019  CEA/DEN, EDF R&D
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

usage="""
This script prepares the test environment and runs a test script:
 - clean and create test directory
 - create a SALOME application
 - launch salome
 - launch the test script within SALOME environment
 - kill salome

 This script uses the following environment variables:
   - ROOT_SALOME : directory which contains salome_context.cfg.
                  This variable is usually defined in salome_prerequisites.sh
   - KERNEL_ROOT_DIR and YACS_ROOT_DIR : directories of modules installation
                  Those variables are usually defined in salome_modules.sh
 Environment variables can be passed to the script using the -d option.
"""

import os
import sys

class TestEnvironment:
  def setUp(self):
    import shutil
    shutil.rmtree("appli", ignore_errors=True)
    
    # create config_appli.xml in current directory
    salome_path = os.getenv("ROOT_SALOME", "")
    salome_context_file = os.path.join(salome_path, "salome_context.cfg")
    if not os.path.isfile(salome_context_file):
      print("File salome_context.cfg not found.")
      print("Search path:" + salome_path)
      print("This test needs ROOT_SALOME environment variable in order to run")
      exit(0)
    
    config_appli_text = '''<application>
<context path="''' + salome_context_file + '''"/>
<modules>
   <module name="KERNEL" path="'''
    kernel_path = os.getenv("KERNEL_ROOT_DIR", "")
    if not os.path.isdir(kernel_path) :
      print("KERNEL_ROOT_DIR not defined")
      exit(0)
      pass
    
    config_appli_text += kernel_path + '"/>'
        
    # some tests need YACS module.
    yacs_path = os.getenv("YACS_ROOT_DIR", "")
    if os.path.isdir(yacs_path):
      config_appli_text += '''
   <module name="YACS" path="'''
      config_appli_text += yacs_path + '"/>'
      pass
    config_appli_text += '''
</modules>
</application>'''

    f = open("config_appli.xml", 'w')
    f.write(config_appli_text)
    f.close()
    
    # create a SALOME application
    appli_gen_file = os.path.join(kernel_path,
                                  "bin","salome","appli_gen.py")
    appli_dir = "appli"
    os.system(appli_gen_file + " --prefix="+appli_dir+
                               " --config=config_appli.xml")
    
    # start salome
    import imp
    sys.path[:0] = [os.path.join(appli_dir, "bin", "salome", "appliskel")]
    self.salome_module = imp.load_source("SALOME", os.path.join(appli_dir, "salome"))
    try:
      self.salome_module.main(["start", "-t"])
    except SystemExit as e:
      # There is an exit() call in salome.main. Just ignore it.
      pass
    
  def run(self, script):
    ret = 0
    try:
      ret = self.salome_module.main(["shell", script])
    except SystemExit as e:
      # return exit value
      ret = e.code
    return ret
  
  def tearDown(self):
    try:
      self.salome_module.main(["killall"])
    except SystemExit as e:
      pass
    pass
  pass #class TestEnvironment

if __name__ == '__main__':
  import argparse
  parser = argparse.ArgumentParser(description=usage,
                    formatter_class=argparse.RawDescriptionHelpFormatter)
  parser.add_argument('command', help="Test command to be run.")
  parser.add_argument('-d', '--define', action='append', help="VARIABLE=VALUE")

  args = parser.parse_args()
  for opt in args.define:
    opts = opt.split('=', 1)
    os.environ[opts[0]] = opts[1]
  
  envTest = TestEnvironment()
  envTest.setUp()
  ret = envTest.run(args.command)
  envTest.tearDown()
  exit(ret)
  pass

