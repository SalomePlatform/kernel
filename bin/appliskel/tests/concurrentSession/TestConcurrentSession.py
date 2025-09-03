#!/usr/bin/env python3
# Copyright (C) 2013-2025  CEA, EDF, OPEN CASCADE
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

import logging
import multiprocessing
import sys
import unittest

def new_instance(running_instances):
  from salome_instance import SalomeInstance
  instance = SalomeInstance.start()
  print("Instance created and now running on port", instance.get_port())
  running_instances.put(instance)
#

def createInstances(nb):
  running_instances = multiprocessing.Queue()
  processes = [
    multiprocessing.Process(target=new_instance, args=(running_instances,))
    for i in range(nb)
  ]
  return running_instances, processes
#

def terminateInstances(running_instances):
  import time
  timeout = time.time() + 60 * 10  # the test duration is about 50 s, we reasonably assume a max duration of 10mn

  while not running_instances.empty() and time.time() < timeout:
    instance = running_instances.get()
    print("Terminate instance running on port", instance.get_port())
    instance.stop()
#

def session(args=None):
  if args is None:
    args = []
  try:
    from salome.kernel import setenv_impl
    setenv_impl.main(True)
    import runSession
    params, args = runSession.configureSession(args, exe="salome shell")
    return runSession.runSession(params, args)
  except SystemExit as e:
    if str(e) != '0':
      logging.error(e)
    pass
#

def appli(args=None):
  if args is None:
    args = []
  try:
    sys.argv = ['runSalome', '-t']
    from salome.kernel import setenv_impl
    setenv_impl.main(True, exeName="salome start")
    import runSalome
    runSalome.runSalome()
  except SystemExit as e:
    if str(e) != '0':
      logging.error(e)
    pass
#

class TestConcurrentLaunch(unittest.TestCase):

  def test01_SingleSession(self):
    print("** Testing single session **")
    session(["hello.py"])

  def test02_MultiSession(self):
    print("** Testing multi sessions **")
    jobs = []
    for i in range(9):
      p = multiprocessing.Process(target=session, args=(["hello.py"],))
      jobs.append(p)
      p.start()

    for j in jobs:
      j.join()
  #

  def test03_SingleAppli(self):
    print("** Testing single appli **")
    running_instances, processes = createInstances(1)
    for p in processes:
      p.start()
      pass
    for p in processes:
      p.join()
      pass

    session(["hello.py"])
    terminateInstances(running_instances)
  #

  def test04_MultiAppli(self):
    print("** Testing multi appli **")
    running_instances, processes = createInstances(9)
    for p in processes:
      p.start()
      pass
    for p in processes:
      p.join()
      pass

    session(["hello.py"])
    terminateInstances(running_instances)
  #
#

if __name__ == "__main__":
  unittest.main()
#
