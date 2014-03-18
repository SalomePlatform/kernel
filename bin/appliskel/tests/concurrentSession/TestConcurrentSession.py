# Copyright (C) 2013-2014  CEA/DEN, EDF R&D, OPEN CASCADE
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
import tempfile

import os
import sys
import imp
from cStringIO import StringIO
import multiprocessing


class TestConcurrentLaunch(unittest.TestCase):
  def setUp(self):
    # Initialize path to SALOME application
    path_to_launcher = os.getenv("SALOME_LAUNCHER")
    appli_dir = os.path.dirname(path_to_launcher)

    # Configure session startup
    self.SALOME = imp.load_source("SALOME", os.path.join(appli_dir,"salome"))
    self.SALOME_appli_args = ["start", "-t"]
    self.SALOME_shell_args = ["shell"]
  #
  def tearDown(self):
    pass
  #
  def appli(self, args=[]):
    self.SALOME.main(self.SALOME_appli_args + args)
  #
  def session(self, args=[]):
    self.SALOME.main(self.SALOME_shell_args + args)
  #
  def test01_SingleSession(self):
    print "** Testing single session **"
    self.session(["hello.py"])
  #
  def test02_MultiSession(self):
    print "** Testing multi sessions **"
    jobs = []
    for i in range(9):
      p = multiprocessing.Process(target=self.session, args=(["hello.py"],))
      jobs.append(p)
      p.start()

    for j in jobs:
      j.join()
  #
  def test03_SingleAppli(self):
    print "** Testing single appli **"
    current_directory = os.path.dirname(os.path.abspath(__file__))
    session_log = tempfile.NamedTemporaryFile(prefix='session_', suffix='.log')
    self.appli(["--ns-port-log=%s"%session_log.name])
    port_number = None
    with open(session_log.name, "r") as f:
      port_number = f.readline()
    self.session(["hello.py"])
    self.session(["-p", port_number, "killSalomeWithPort.py", "args:%s"%port_number])
    session_log.close()
  #
  def test04_MultiAppli(self):
    print "** Testing multi appli **"
    jobs = []
    for i in range(9):
      p = multiprocessing.Process(target=self.test03_SingleAppli)
      jobs.append(p)
      p.start()

    for j in jobs:
      j.join()
  #
#

if __name__ == "__main__":
  path_to_launcher = os.getenv("SALOME_LAUNCHER")
  if not path_to_launcher:
    msg = "Error: please set SALOME_LAUNCHER variable to the salome command in your application folder."
    raise Exception(msg)

  if not os.path.isfile("hello.py"):
    with open("hello.py", "w") as f:
      f.write("print 'Hello!'")

  unittest.main()
#
