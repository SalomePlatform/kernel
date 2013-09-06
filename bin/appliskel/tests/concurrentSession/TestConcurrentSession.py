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

class SalomeSession(object):
  def __init__(self, script, killAtEnd=True):
    self.__killAtEnd = killAtEnd
    import runSalome
    sys.argv  = ["runSalome.py"]
    sys.argv += ["--terminal"]
    sys.argv += ["%s" % script]
    clt, d = runSalome.main()
  #
  def __del__(self):
    if self.__killAtEnd:
      port = os.getenv('NSPORT')
      import killSalomeWithPort
      killSalomeWithPort.killMyPort(port)
    return
  #
#

def run_session():
  SalomeSession("")
#
class TestConcurrentLaunch(unittest.TestCase):
  def testSingleSession(self):
    print "** Testing single session **"
    SalomeSession("")
  #
  def testMultiSession(self):
    print "** Testing multi sessions **"

    jobs = []
    for i in range(3):
      p = multiprocessing.Process(target=run_session)
      jobs.append(p)
      p.start()

    for j in jobs:
      j.join()
  #
  @classmethod
  def tearDownClass(cls):
    import killSalome
    killSalome.killAllPorts()
  #
#

unittest.main()
