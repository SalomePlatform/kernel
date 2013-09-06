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

class TestConcurrentLaunch(unittest.TestCase):
  @classmethod
  def setUpClass(cls):
    # Initialize path to SALOME application
    path_to_launcher = os.getenv("SALOME_LAUNCHER")
    appli_dir = os.path.dirname(path_to_launcher)
    envd_dir = os.path.join(appli_dir, "env.d")

    # Configure session startup
    cls.SALOME = imp.load_source("SALOME", os.path.join(appli_dir,"salome"))
    cls.SALOME_args = ["shell", "--config="+envd_dir]
  #
  def session(self, args=[]):
    self.SALOME.main(self.SALOME_args + args)
  #
  def testSingleSession(self):
    print "** Testing single session **"
    self.session()
  #
  def testMultiSession(self):
    print "** Testing multi sessions **"

    jobs = []
    for i in range(3):
      p = multiprocessing.Process(target=session, args=(self,))
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
