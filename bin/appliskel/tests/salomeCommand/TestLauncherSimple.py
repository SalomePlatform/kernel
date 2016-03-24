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
import logging

class TestLauncher(unittest.TestCase):

  def setUp(self):
    from salome_instance import SalomeInstance
    self.instance = SalomeInstance.start()
    print "Instance created and now running on port", self.instance.get_port()
   #

  def tearDown(self):
    print "Terminate instance running on port", self.instance.get_port()
    self.instance.stop()
  #

  def testHello(self):
    try:
      import setenv
      setenv.main(True)
      import runSession
      args = ["hello.py"]
      params, args = runSession.configureSession(args, exe="salome shell")
      return runSession.runSession(params, args)
    except SystemExit, e:
      if str(e) != '0':
        logging.error(e)
  #
#

if __name__ == "__main__":
  unittest.main()
#
