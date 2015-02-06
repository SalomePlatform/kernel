# Copyright (C) 2013-2015  CEA/DEN, EDF R&D, OPEN CASCADE
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

import os
import sys
import imp
import logging

class TestLauncher(unittest.TestCase):

  def setUp(self):
    path_to_launcher = os.getenv("SALOME_LAUNCHER")
    appli_dir = os.path.dirname(path_to_launcher)
    sys.path[:0] = [os.path.join(appli_dir, "bin", "salome", "appliskel")]

    self.SALOME = imp.load_source("SALOME", os.path.join(appli_dir,"salome"))
   #

  def testHello(self):
    try:
      self.SALOME.main(["shell", "hello.py"])
    except SystemExit, e:
      if str(e) != '0':
        logging.error(e)
  #
#

if __name__ == "__main__":
  path_to_launcher = os.getenv("SALOME_LAUNCHER")
  if not path_to_launcher:
    msg = "Error: please set SALOME_LAUNCHER variable to the salome command of your application folder."
    raise Exception(msg)

  unittest.main()
#
