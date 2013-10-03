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

from PortManager import getPort, releasePort, stopServer
import sys
import multiprocessing
import unittest

def port_reservation(prefered=None, test=None, expected=None):
  if prefered:
    port = getPort(prefered)
  else:
    port = getPort()
  print "port = %s"%port

  if expected:
    print "expected = %s"%expected
    test.assertTrue(port == expected)
#

class TestMinimalExample(unittest.TestCase):
  @classmethod
  def tearDownClass(cls):
    stopServer()
    stopServer() # no effect
  #
  def testSequential(self):
    print "BEGIN testSequential"
    processes = [
      multiprocessing.Process(target=port_reservation)
      for i in range(3)
      ]

    for p in processes:
      p.start()

    for p in processes:
      p.join()

    # Try to get specific port number
    expected = 2872
    p = multiprocessing.Process(target=port_reservation, args=(2872, self,expected,))
    p.start()
    p.join()

    # Try to get specific port number
    p = multiprocessing.Process(target=port_reservation, args=(2812, self,))
    p.start()
    p.join()

    # Release port
    p = multiprocessing.Process(target=releasePort, args=(2812,))
    p.start()
    p.join()

    # Try to get specific port number
    expected = 2812
    p = multiprocessing.Process(target=port_reservation, args=(2812, self,expected,))
    p.start()
    p.join()

    print "END testSequential"
  #
  def testConcurrent(self):
    print "BEGIN testConcurrent"
    processes = [
      multiprocessing.Process(target=port_reservation)
      for i in range(3)
      ]

    # Try to get specific port number
    p = multiprocessing.Process(target=port_reservation, args=(2852,))
    processes.append(p)

    # Try to get specific port number
    p = multiprocessing.Process(target=port_reservation, args=(2812,))
    processes.append(p)

    # Release port
    p = multiprocessing.Process(target=releasePort, args=(2812,))
    processes.append(p)

    # Try to get specific port number
    p = multiprocessing.Process(target=port_reservation, args=(2812,))
    processes.append(p)

    for p in processes:
      p.start()

    for p in processes:
      p.join()

    print "END testConcurrent"
  #
#

unittest.main()
