#!/usr/bin/env python
# Copyright (C) 2013  CEA/DEN, EDF R&D, OPEN CASCADE
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
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

import os
import sys
import unittest
import multiprocessing
import imp

def unwrap_self_session(arg, **kwarg):
  return TestConcurrentLaunch.session(*arg, **kwarg)
#

class TestConcurrentLaunch(unittest.TestCase):
  @classmethod
  def setUpClass(cls):
    # Initialize path to SALOME application
    path_to_launcher = os.getenv("SALOME_LAUNCHER")
    appli_dir = os.path.dirname(path_to_launcher)
    cls.envd_dir = os.path.join(appli_dir, "env.d")

    # Configure session startup
    cls.SALOME = imp.load_source("SALOME", os.path.join(appli_dir,"salome"))
    #cls.SALOME_args = ["shell", "--config="+cls.envd_dir]
    cls.SALOME_args = ["--config="+cls.envd_dir]
  #
  @classmethod
  def tearDownClass(cls):
    args = ["killall", "--config="+cls.envd_dir]
    cls.SALOME.main(args)
    pass
  #
  def session(self, args=[]):
    self.SALOME.main(self.SALOME_args + args)
  #
  def test01_SingleSession(self):
    print "** Testing single session **"
    self.session()
  #
  def test02_MultiSession(self):
    print "** Testing multi sessions **"

    jobs = []
    for i in range(3):
      p = multiprocessing.Process(target=unwrap_self_session, args=([self],))
      jobs.append(p)
      p.start()

    for j in jobs:
      j.join()
  #
#


if __name__ == "__main__":
  path_to_launcher = os.getenv("SALOME_LAUNCHER")
  if not path_to_launcher:
    msg = "Error: please set SALOME_LAUNCHER variable to the salome command of your application folder."
    raise Exception(msg)

  unittest.main()
#
