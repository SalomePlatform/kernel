# Copyright (C) 2013-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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
import logging

def new_instance(running_instances):
  from salome_instance import SalomeInstance
  instance = SalomeInstance.start()
  print "Instance created and now running on port", instance.get_port()
  running_instances.put(instance)
#

class TestConcurrentLaunch(unittest.TestCase):
  def __createInstances(self, nb):
    running_instances = multiprocessing.Queue()
    processes = [
      multiprocessing.Process(target=new_instance, args=(running_instances,))
      for i in range(nb)
      ]
    return running_instances, processes
  #
  def __terminateInstances(self, running_instances):
    while not running_instances.empty():
      instance = running_instances.get()
      print "Terminate instance running on port", instance.get_port()
      instance.stop()
  #

  def appli(self, args=None):
    if args is None:
      args = []
    try:
      sys.argv = ['runSalome', '-t']
      import setenv
      setenv.main(True, exeName="salome start")
      import runSalome
      runSalome.runSalome()
    except SystemExit, e:
      if str(e) != '0':
        logging.error(e)
      pass
  #
  def session(self, args=None):
    if args is None:
      args = []
    try:
      import setenv
      setenv.main(True)
      import runSession
      params, args = runSession.configureSession(args, exe="salome shell")
      return runSession.runSession(params, args)
    except SystemExit, e:
      if str(e) != '0':
        logging.error(e)
      pass
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
    running_instances, processes = self.__createInstances(1)
    for p in processes:
      p.start()
      pass
    for p in processes:
      p.join()
      pass

    self.session(["hello.py"])
    self.__terminateInstances(running_instances)
  #
  def test04_MultiAppli(self):
    print "** Testing multi appli **"
    running_instances, processes = self.__createInstances(9)
    for p in processes:
      p.start()
      pass
    for p in processes:
      p.join()
      pass

    self.session(["hello.py"])
    self.__terminateInstances(running_instances)
  #
#

if __name__ == "__main__":
  if not os.path.isfile("hello.py"):
    with open("hello.py", "w") as f:
      f.write("print 'Hello!'")

  unittest.main()
#
