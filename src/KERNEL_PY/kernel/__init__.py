# -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

## \defgroup salome_kernel salome.kernel
#  \{ 
#  \details Package containing the KERNEL python utilities
#  \defgroup deprecation
#  \defgroup termcolor
#  \defgroup logger
#  \defgroup enumerate
#  \defgroup uiexception
#  \defgroup datamodeler
#  \defgroup diclookup
#  \defgroup service
#  \defgroup studyedit
#  \defgroup unittester
#  \defgroup pyunittester
#  \}

__all__ = [ "deprecation", "logger", "termcolor", "logconfig" ]

# WARN: This file SHOULD NOT import salome stuff so that modules of the
# package could be used outside of a SALOME session context when
# possible. For example logger.py, datamodeler.py, enumerate.py,
# diclookup.py, ... does not need a SALOME running application.

#
# ==============================================================================
# Generic serialization functions for the module datamodeler
# ==============================================================================
#
import pickle

def serialize(dataToSerialize):
    """
    Creates a string description of the specified data.
    .. attribute:: dataToSerialize
    a data object to serialize
    """
    serialstring = pickle.dumps(dataToSerialize)
    return serialstring

def unserialize(stringToUnserialize):
    """
    Creates a data object from its string description. The string description
    is supposed to be obtained by a call to serialize.
    .. attribute:: stringToUnserialize
    a string description of the object
    """
    data = pickle.loads(stringToUnserialize)
    return data

class Callable:
    """
    This class is used to create class-method (see MetaData)
    """
    def __init__(self, anycallable):
        self.__call__ = anycallable
#
# ==============================================================================
# Basic use cases and unit tests
# ==============================================================================
#

def TEST_serialization():
    from testdata import TestData
    ref_name = "my study case"
    studyData = TestData()
    studyData.setName(ref_name)

    print "serialize data ..."
    serialString = serialize(studyData)
    print "unserialize data ..."
    unserialData = unserialize(serialString)

    res_name = unserialData.getName()
    print res_name
    if ( ref_name != res_name ):
        return False

    unserialData.log()
    unserialData.setName("an other name")

    return True

if __name__ == "__main__":
    import unittester
    unittester.run("salome/kernel/__init__","TEST_serialization")
