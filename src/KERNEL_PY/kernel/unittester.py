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

## \defgroup unittester unittester
#  \{ 
#  \details Run very basic unit tests
#  \}

__author__="gboulant"
__date__ ="$1 avr. 2010 09:45:21$"

import sys

## This function should be used for very basic unit tests and only for a
#  rapid development. %A better way should be the pyunit framework.
#  The function functionName is supposed here to return a boolean value
#  indicating if the test is OK (True) or NOT OK (False)
#  \ingroup unittester
def run(modulePath, functionName):
    """
    This function should be used for very basic unit tests and only for a
    rapid development. A better way should be the pyunit framework.
    The function functionName is supposed here to return a boolean value
    indicating if the test is OK (True) or NOT OK (False)
    """
    moduleName = modulePath.replace('/','.') 
    __import__ (moduleName)
    module=sys.modules[moduleName]
    func = getattr(module,functionName)
    tabsize = 70-len(moduleName)-len(functionName)
    print "[TEST] %s.%s %s test in progress" % (moduleName, functionName,"."*tabsize) 
    ok = func()
    if ( ok ):
        print "[TEST] %s.%s %s OK" % (moduleName, functionName,"."*tabsize)
    else:
        print "[TEST] %s.%s %s NOT OK" % (moduleName, functionName,"."*tabsize)

## This function is for debug only. It executes the specified function with the
#  specified arguments in a try/except bloc so that to display the exception in
#  the case where an exception is raised (usefull to debug server side of a CORBA
#  process).
#  \ingroup unittester
def tryfunction(function,*argv):
    """
    This function is for debug only. It executes the specified function with the
    specified arguments in a try/except bloc so that to display the exception in
    the case where an exception is raised (usefull to debug server side of a CORBA
    process).
    """
    print "[TEST] trying the function %s" % function
    try:
        return function(*argv)
    except Exception, e:
        print e
        raise e


#
# ==============================================================================
# Simple use cases
# ==============================================================================
#
def TEST_myTestIsOk():
    return True

def TEST_myTestIsNotOk():
    return False

if __name__ == "__main__":
    run("salome/kernel/unittester","TEST_myTestIsOk")
    run("salome/kernel/unittester","TEST_myTestIsNotOk")
