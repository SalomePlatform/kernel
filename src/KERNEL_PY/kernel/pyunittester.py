# -*- coding: iso-8859-1 -*-
# Copyright (C) 2011-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

## \defgroup pyunittester pyunittester
#  \{ 
#  \details Simple wrapper of the pyunit framework
#  \}

import sys
from salome.kernel import termcolor
def printfile(filename):
    stream = open(filename,'r')
    lines = stream.readlines()
    stream.close()
    for line in lines:
	if not termcolor.canDisplayColor(sys.stdout):
	    msg = line.split('\n')[0]
	else:
            msg = termcolor.makeColoredMessage(line.split('\n')[0], termcolor.BLUE)
        print msg

import os
import unittest
from unittest import TestCase
from uiexception import DevelException

## This class is a simple wrapper of the pyunit framework.
#  \ingroup pyunittester
class PyUnitTester():
    """
    This class is a simple wrapper of the pyunit framework.
    """
    def __init__(self):
        self.__listTestSuite=[]
        self.setLogFilename("testlog.txt")

    def addTestCase(self,testCaseClass):
        # We test some attributes to validate that the argument
        # corresponds to a unittest.TestCase class (WARN: it's not an
        # instance but a class keyword)
        if "assertTrue" not in dir(testCaseClass):
            raise DevelException("Not a pyunit test case")

        suite = unittest.TestLoader().loadTestsFromTestCase(testCaseClass)
        self.__listTestSuite.append(suite)

    def setLogFilename(self, filename):
        self.__logfilename = filename

    def run(self):
        # We first open the output stream
        if os.path.exists(self.__logfilename):
            os.remove(self.__logfilename)
        logfile = open(self.__logfilename,'w')
        
        # Then, define and execute the runner to play the test suites
        runner = unittest.TextTestRunner(stream=logfile,verbosity=2)
        for suite in self.__listTestSuite:
            runner.run(suite)

        # Finally close the ouput stream and print the report
        logfile.close()
        self.printreport()

    def printreport(self):
        printfile(self.__logfilename)

## This can be used to just execute a test function that is
#  considered as OK simply if no exception is raised (can be test by
#  an assertTrue).
#  \ingroup pyunittester
def execAndConvertExceptionToBoolean(function):
    """
    This can be used to just execute a test function that is
    considered as OK simply if no exception is raised (can be test by
    an assertTrue).
    """
    try:
        result = function()
        if result is None:
            return True
        return result
    except Exception, e:
        print e
        return False

# Simple helper function for most cases where there is only one
# TestCase classe.
## Run the test suite provided by the specified TestCase class.
#  \ingroup pyunittester
def run(testCaseClass):
    """
    Run the test suite provided by the specified TestCase class.
    """
    tester = PyUnitTester()
    tester.addTestCase(testCaseClass)
    tester.run()

#
# ==============================================================================
# Simple use cases
# ==============================================================================
#
class MyTestCase(unittest.TestCase):
    def test_myTestOk_01(self):
        r=True
        print "myTestOk_01: should be OK"
        self.assertTrue(r)

    def test_myTestOk_02(self):
        r=True
        print "myTestOk_02: should be OK"
        self.assertTrue(r)

    def test_myTestNotOk(self):
        r=False
        print "myTestNotOk: should be NOT OK"
        self.assertTrue(r)

def functionRaisingAnException():
    raise Exception("An error occurs")

def functionReturningFalse():
    return False

def functionReturningTrue():
    return True

def functionReturningNothing():
    print "functionReturningNothing"

class MyTestCase2(unittest.TestCase):
    def test_myTest_01(self):
        r=execAndConvertExceptionToBoolean(functionRaisingAnException)
        print "test 01: this test should be NOT OK"
        self.assertTrue(r)

    def test_myTest_02(self):
        r=execAndConvertExceptionToBoolean(functionReturningFalse)
        print "test 02: this test should be NOT OK"
        self.assertTrue(r)

    def test_myTest_03(self):
        r=execAndConvertExceptionToBoolean(functionReturningTrue)
        print "test 03: this test should be OK"
        self.assertTrue(r)

    def test_myTest_04(self):
        r=execAndConvertExceptionToBoolean(functionReturningNothing)
        print "test 04: this test should be OK"
        self.assertTrue(r)

    def test_myTest_05(self):
        r=True
        print "test 05: this test should be OK"
        self.assertTrue(r)


def TEST_basic_usecase():
    tester = PyUnitTester()
    # Note that the class keywords must be passed
    tester.addTestCase(MyTestCase)
    tester.addTestCase(MyTestCase2)
    tester.run()
    
if __name__ == "__main__":
    TEST_basic_usecase()
