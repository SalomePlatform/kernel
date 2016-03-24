#!/usr/bin/env python
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

import os
import sys
import multiprocessing
import unittest
import logging

def port_reservation(obtained_ports, prefered=None, test=None, expected=None):
  from PortManager import getPort
  if prefered:
    port = getPort(prefered)
  else:
    port = getPort()
  print "obtained port = %s"%port

  obtained_ports.put(port)

  if expected:
    test.assertTrue(port == expected, "used = %s, expected = %s"%(port, expected))
#

class TestMinimalExample(unittest.TestCase):
  def testSequential(self):
    from PortManager import releasePort, getBusyPorts
    print "\nBEGIN testSequential"
    print "Busy ports", getBusyPorts()
    obtained_ports = multiprocessing.Queue()

    processes = [
      multiprocessing.Process(target=port_reservation, args=(obtained_ports,))
      for i in range(3)
      ]

    for p in processes:
      p.start()

    for p in processes:
      p.join()

    print "Busy ports", getBusyPorts()
    # Try to get specific port number
    p = multiprocessing.Process(target=port_reservation, args=(obtained_ports, 2872, self, 2872,))
    p.start()
    p.join()

    # Try to get specific port number
    p = multiprocessing.Process(target=port_reservation, args=(obtained_ports, 2812, self,))
    p.start()
    p.join()

    # Try to get specific port number
    p = multiprocessing.Process(target=port_reservation, args=(obtained_ports, 2812, self,))
    p.start()
    p.join()

    # Release port
    print "release port 2812"
    p = multiprocessing.Process(target=releasePort, args=(2812,))
    p.start()
    p.join()

    # Try to get specific port number
    p = multiprocessing.Process(target=port_reservation, args=(obtained_ports, 2812, self, 2812,))
    p.start()
    p.join()

    # Release ports
    print "Busy ports", getBusyPorts()
    while not obtained_ports.empty():
      port = obtained_ports.get()
      print "release port", port
      p = multiprocessing.Process(target=releasePort, args=(port,))
      p.start()
      p.join()

    print "END testSequential"
  #

  def testConcurrent(self):
    from PortManager import releasePort, getBusyPorts
    print "\nBEGIN testConcurrent"
    print "Busy ports", getBusyPorts()
    obtained_ports = multiprocessing.Queue()
    processes = [
      multiprocessing.Process(target=port_reservation, args=(obtained_ports,))

      for i in range(3)
      ]

    # Try to get specific port number
    p = multiprocessing.Process(target=port_reservation, args=(obtained_ports, 2872, self, 2872,))
    processes.append(p)

    # Try to get specific port number
    p = multiprocessing.Process(target=port_reservation, args=(obtained_ports, 2812,))
    processes.append(p)

    # Try to get specific port number
    p = multiprocessing.Process(target=port_reservation, args=(obtained_ports, 2812,))
    processes.append(p)

    for p in processes:
      p.start()

    for p in processes:
      p.join()

    # Release ports
    print "Busy ports", getBusyPorts()
    while not obtained_ports.empty():
      port = obtained_ports.get()
      print "release port", port
      p = multiprocessing.Process(target=releasePort, args=(port,))
      p.start()
      p.join()

    print "END testConcurrent"
  #
#

if __name__ == "__main__":
  omniorb_user_path = os.getenv("OMNIORB_USER_PATH")
  if not omniorb_user_path:
    msg = "\n"
    msg += "Error: please set OMNIORB_USER_PATH variable.\n"
    msg += "       Usually this points to your application USERS directory.\n"
    logging.error(msg)
    sys.exit(1)

  try:
    import PortManager
  except ImportError:
    msg = "\n"
    msg += "Error: can't import PortManager; please check PYTHONPATH variable.\n"
    msg += "       You need to add <KERNEL_INSTALL_PATH>/bin/salome path.\n"
    logging.error(msg)
    sys.exit(1)

  unittest.main()
#
