# -*- coding: iso-8859-1 -*-
# Copyright (C) 2010-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

## \defgroup uiexception uiexception
#  \{ 
#  \details Exception for user error management
#  \}

__author__="gboulant"
__date__ ="$31 mars 2010 11:59:33$"

from enumerate import Enumerate

## This exception should be used for functionnal error management, at least in the GUI
#  part of the application, for example to set user oriented messages at point
#  of exception raise.
#  \warning The exception should NOT be used to hide defaults in the algorithm, but
#  only predicted error in the specified use case.
#  \ingroup uiexception
class UiException(Exception):

    TYPES = Enumerate([
        'USER',  # This type should be displayed to end user using a dialog box
        'ADMIN', # This type should be displayed to admin user in console log
        'DEVEL'  # This type should be displayed to developer only
    ])

    _UImessage = "An error occurs"
    _type = TYPES.USER

    """
    This exception should be used for functionnal error management, at least in the GUI
    part of the application, for example to set user oriented messages at point
    of exception raise.
    WARN: The exception should NOT be used to hide defaults in the algorithm, but
    only predicted error in the specified use case.
    """
    
    ## Canonical constructor
    def __init__(self, message, type=TYPES.USER):
        """Canonical constructor"""
        Exception.__init__(self,message)
        self.setUIMessage(message)
        self.setType(type)

    def setUIMessage(self, UImessage):
        self._UImessage = UImessage

    def getUIMessage(self):
        return self._UImessage

    ## Specify the type of this exception. To be choosen in the TYPES list.
    def setType(self, type):
        """Specify the type of this exception. To be choosen in the TYPES list."""
        if not self.TYPES.isValid(type):
            raise UiException("The exception type "+str(type)+" can't be used",self.TYPES.DEVEL)
        self._type = type

    def getType(self):
        return self._type

    def __str__(self):
        return self.getUIMessage()


def UserException(msg):
    return UiException(msg,UiException.TYPES.USER)

def AdminException(msg):
    return UiException(msg,UiException.TYPES.ADMIN)

def DevelException(msg):
    return UiException(msg,UiException.TYPES.DEVEL)

#
# ==============================================================================
# Basic use cases and unit test functions
# ==============================================================================
#
def somethingGoesWrong():
    raise UiException("Something goes wrong")

def TEST_uimessage():
    try:
        somethingGoesWrong()
        return False
    except UiException, err:
        print 'ERROR: %s' % str(err)
        if ( str(err) == "Something goes wrong" ):
            return True
        else:
            return False

def TEST_specificException():
    print DevelException("err")
    print AdminException("err")
    print UserException("err")
    return True

if __name__ == "__main__":
    import unittester
    unittester.run("uiexception","TEST_uimessage")
    unittester.run("uiexception","TEST_specificException")
    
