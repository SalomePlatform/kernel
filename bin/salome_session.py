#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

## @package salome_session
# \brief Module that provides the SalomeSession object that helps to launch
#        a SALOME script session
#

import os
import sys
import string
import socket

_session = None

def startSession(modules=[]):
    global _session
    if _session: return
    from searchFreePort import searchFreePort
    searchFreePort()
    _session = SalomeSession(modules)
    return

def getShortHostName():
    """
    gives Hostname without domain extension.
    SALOME naming service needs short Hostnames (without domain extension).
    HOSTNAME is not allways defined in environment,
    socket.gethostname() gives short or complete Hostname, depending on
    defined aliases.
    """
    from salome_utils import getShortHostName
    return getShortHostName()


class SalomeSession(object):
    """Salome session launcher"""
    import runSalome
    import killSalomeWithPort
    import killSalome
    def __init__(self, modules):
        import runSalome
        sys.argv  = ["dummy.py"]
        sys.argv += ["--terminal"]
        if modules:
            sys.argv += ['--modules=%s'%(",".join(modules))]
            pass
        runSalome.clt, runSalome.args = runSalome.main()
        import salome
        salome.salome_init()
        return
    def __del__(self):
        import runSalome
        runSalome.killLocalPort()
        return
    pass
