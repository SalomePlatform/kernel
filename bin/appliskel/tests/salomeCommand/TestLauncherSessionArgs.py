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

import os
import sys
import imp
from cStringIO import StringIO
import logging

logger = logging.getLogger("TestLauncherLogger")
logger.level = logging.DEBUG
logger.addHandler(logging.StreamHandler())

class TestSessionArgs(unittest.TestCase):
  #
  logFile = "log.txt"
  # Set some predefined command args and corresponding output messages
  hello0 = ["hello.py", "args:outfile="+logFile]
  hello0Msg = "Hello!"
  hello1 = ["hello.py", "args:you,outfile="+logFile]
  hello1Msg = "Hello to: you"
  helloToAdd = ["hello.py", "args:add.py,1,2,3,outfile="+logFile]
  helloToAddMsg = "Hello to: add.py, 1, 2, 3"
  helloToList = ["hello.py", "args:['file1','file2'],1,2,3,[True,False],'ok',outfile="+logFile]
  helloToListMsg = "Hello to: ['file1','file2'], 1, 2, 3, [True,False], 'ok'"
  add0 = ["add.py", "args:outfile="+logFile]
  add0Msg = "No args!"
  add3 = ["add.py", "args:1,2,3,outfile="+logFile]
  add3Msg = "1+2+3 = 6"
  lines0 = ["lines.py", "args:outfile="+logFile]
  lines0Msg = "No files given"
  lines2 = ["lines.py", "args:hello.py,add.py,outfile="+logFile]
  lines2Msg = "hello.py is 35 lines longadd.py is 37 lines long"
  linesUnreadable = ["lines.py", "args:hello.py,add.py,1,2,outfile="+logFile]
  linesUnreadableMsg = "hello.py is 35 lines longadd.py is 37 lines longFile '1' cannot be readFile '2' cannot be read"
  #
  def setUp(self):
    from salome_instance import SalomeInstance
    self.instance = SalomeInstance.start()
    print "Instance created and now running on port", self.instance.get_port()

    sys.stdout = StringIO()
    self.removeLogFile()
  #
  def tearDown(self):
    self.removeLogFile()
    print "Terminate instance running on port", self.instance.get_port()
    self.instance.stop()
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
        logger.error(e)
      pass
  #
  def removeLogFile(self):
    try:
      os.remove(self.logFile)
    except OSError:
      pass
  #
  def assertLogFileContentsEqual(self, message):
    with open(self.logFile, "r") as f:
      contents = f.read().replace('\n', '')

    self.assertTrue(contents==message, "Contents differ!\n\tGenerated contents: %s\n\tExpected contents: %s"%(contents, message))
  #
  def testHello0(self):
    self.session(self.hello0)
    self.assertLogFileContentsEqual(self.hello0Msg)
  #
  def testPythonHello0(self):
    self.session(["python"]+self.hello0)
    self.assertLogFileContentsEqual(self.hello0Msg)
  #
  def testHello1(self):
    self.session(self.hello1)
    self.assertLogFileContentsEqual(self.hello1Msg)
  #
  def testAdd0(self):
    self.session(self.add0)
    self.assertLogFileContentsEqual(self.add0Msg)
  #
  def testAdd3(self):
    self.session(self.add3)
    self.assertLogFileContentsEqual(self.add3Msg)
  #
  def testHello0Add3(self):
    self.session(self.hello0+self.add3)
    self.assertLogFileContentsEqual(self.hello0Msg+self.add3Msg)
  #
  def testHello1Add3(self):
    self.session(self.hello1+self.add3)
    self.assertLogFileContentsEqual(self.hello1Msg+self.add3Msg)
  #
  def testHelloToAdd(self):
    self.session(self.helloToAdd)
    self.assertLogFileContentsEqual(self.helloToAddMsg)
  #
  def testHelloToList(self):
    self.session(self.helloToList)
    self.assertLogFileContentsEqual(self.helloToListMsg)
  #
  def testLines0(self):
    self.session(self.lines0)
    self.assertLogFileContentsEqual(self.lines0Msg)
  #
  def testLines2(self):
    self.session(self.lines2)
    self.assertLogFileContentsEqual(self.lines2Msg)
  #
  def testLines2Add3(self):
    self.session(self.lines2+self.add3)
    self.assertLogFileContentsEqual(self.lines2Msg+self.add3Msg)
  #
  def testLinesUnreadable(self):
    self.session(self.linesUnreadable)
    self.assertLogFileContentsEqual(self.linesUnreadableMsg)
  #
  def testAddAddHello(self):
    self.session(self.add3+self.add3+self.hello1)
    self.assertLogFileContentsEqual(self.add3Msg+self.add3Msg+self.hello1Msg)
  #
  def testHello0Add3Hello0Add3Hello0(self):
    self.session(self.hello1+self.add3+self.hello0+self.add3+self.hello0)
    self.assertLogFileContentsEqual(self.hello1Msg+self.add3Msg+self.hello0Msg+self.add3Msg+self.hello0Msg)
  #
#

if __name__ == "__main__":
  unittest.main()
#
