# -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

__author__="gboulant"
__date__ ="$17 avr. 2010 19:44:36$"

from enumerate import Enumerate
from datamodeler import DataModeler, TypeString, TypeInteger
from salome.kernel import Callable

class TestData(DataModeler):
    """
    This class is the placeholder for meta data associated to a study case.
    The meta-data are used for organisation and indexing purposes
    """
    TYPES_LIST=Enumerate([
        'SEP',
        'OTHER'
    ])

    def __init__(self):
        DataModeler.__init__(self)
        self.addAttribute(
            name  = "NAME",
            type  = TypeString,
            range = None
            )
        self.addAttribute(
            name  = "TYPE",
            type  = TypeInteger,
            range = self.TYPES_LIST.listvalues()
            )

    def setName(self, value):
        self.NAME = value

    def getName(self):
        return self.NAME

    def setType(self, value):
        self.TYPE = value

    def getType(self):
        return self.TYPE

    def getDefault():
        """
        Create a default instance of TestData
        @class-method
        """
        testdata = TestData()
        testdata.NAME = "my case"
        testdata.TYPE = TestData.TYPES_LIST.SEP
        return testdata

    getDefault = Callable(getDefault)
#
# ==============================================================================
# Basic use cases and unit tests
# ==============================================================================
#
from uiexception import UiException

def TEST_getName():
    testdata = TestData()
    testdata.setName("Sous-epaisseur")
    testdata.setType(TestData.TYPES_LIST.SEP)
    if ( testdata.NAME != "Sous-epaisseur" ):
        return False
    return True


def TEST_useBadKey():
    testdata = TestData()
    try:
        testdata.unknown = "unknown"
        # This should not arrive here
        return False
    except UiException, err:
        print err
        return True

def TEST_useBadType():
    testdata = TestData()
    try:
        testdata.TYPE = "unknown"
        # This should not arrive here
        return False
    except UiException, err:
        print err
        return True

def TEST_useBadRange():
    testdata = TestData()

    try:
        testdata.TYPE = TestData.TYPES_LIST.SEP
        testdata.setType(TestData.TYPES_LIST.SEP)
        # This should arrive here
    except UiException, err:
        # And not here
        print err
        return False

    try:
        testdata.TYPE = 9999 # a type that does not exist in the range
        # This should not arrive here
        return False
    except UiException, err:
        print err
        return True

def TEST_serialize():
    import salome.kernel
    ref_testdata = TestData()
    ref_testdata.setName("The firts name")
    res_testdata = salome.kernel.unserialize(salome.kernel.serialize(ref_testdata))

    print res_testdata.getName()

    if res_testdata.getName() != ref_testdata.getName():
        return False

    # Is the unserialized data still functional?
    try:
        res_testdata.setName("An other name")
        print res_testdata.getName()
    except:
        print e
        return False
    return True

if __name__ == "__main__":
    from unittester import run
    run("salome/kernel/testdata","TEST_getName")
    run("salome/kernel/testdata","TEST_useBadKey")
    run("salome/kernel/testdata","TEST_useBadType")
    run("salome/kernel/testdata","TEST_useBadRange")
    run("salome/kernel/testdata","TEST_serialize")
