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

from PortManager import start_server, start_clients
import time
import datetime
import random
import sys
import unittest

def hello(manager, name, processId):
  seconds = random.randrange(3, 10)
  startTime = datetime.datetime.now().time()

  portManager = manager.PortManager()
  message_queue = manager.get_message_queue()
  port = portManager.getPort()
  message_queue.put("+ Process %s (%d) says: Hello! at %s on port %d and waits for %d seconds"%(name, processId, startTime, port, seconds))
  time.sleep(seconds)
  endTime = datetime.datetime.now().time()
  portManager.releasePort(port)
  message_queue.put("- Process %s (%d) finished at %s --> port %d is free"%(name, processId, endTime, port))
#

class TestMinimalExample(unittest.TestCase):
  def testConcurrent(self):
    nbProcesses = 10
    nbSimultaneous = 4
    ip = '127.0.0.1'

    # start server
    start_server(nbSimultaneous=nbSimultaneous, timeout=5)

    # start several clients
    start_clients(nbProcesses, ip, hello)
  #
#

unittest.main()
