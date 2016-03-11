# Copyright (C) 2015-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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
import multiprocessing

def new_instance(running_instances):
  from salome_instance import SalomeInstance
  instance = SalomeInstance.start()
  print "Instance created and now running on port", instance.get_port()
  running_instances.put(instance)
#

class TestLauncher(unittest.TestCase):

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

  def __connectToInstance(self, port):
    import setenv
    setenv.main(True)
    import runConsole
    return runConsole.connect(["-p", port, "-c", "exit()"]) # auto-logout
  #

  def testSequential(self):
    running_instances, processes = self.__createInstances(3)
    for p in processes:
      p.start()
      p.join()
      pass

    self.__terminateInstances(running_instances)
  #

  def testConcurrent(self):
    running_instances, processes = self.__createInstances(5)
    for p in processes:
      p.start()
      pass
    for p in processes:
      p.join()
      pass

    self.__terminateInstances(running_instances)
  #

  def testConnectInstance(self):
    # Create some instances
    running_instances, processes = self.__createInstances(5)
    for p in processes:
      p.start()
      pass
    for p in processes:
      p.join()
      pass

    # move queued instances to a list
    all_instances = []
    while not running_instances.empty():
      all_instances.append(running_instances.get())

    # Connect to one instance
    import runConsole
    port = all_instances[len(all_instances)/2].get_port()
    print "Connect to instance running on port", port
    self.__connectToInstance(port)

    # Connect to another instance
    import runConsole
    port = all_instances[len(all_instances)/4].get_port()
    print "Connect to instance running on port", port
    self.__connectToInstance(port)

    # Terminate instances
    for instance in all_instances:
      print "Terminate instance running on port", instance.get_port()
      instance.stop()
  #

if __name__ == "__main__":
  unittest.main()
#
