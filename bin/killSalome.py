#! /usr/bin/env python
#  -*- coding: iso-8859-1 -*-
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

## \file killSalome.py
#  Stop all %SALOME servers from all sessions by killing them
#

import os, sys, re, signal

from killSalomeWithPort import killMyPort, getPiDict
#from salome_utils import getHostName, getShortHostName
from salome_utils import getUserName

def killAllPorts():
    """
    Kill all SALOME sessions belonging to the user.
    """
    user = getUserName()
    #hostname  = getHostName()
    #shostname = getShortHostName()
    # new-style dot-prefixed pidict file
    #fpidict   = getPiDict('(\d*)',hidden=True)
    #problem with WIN32 path slashes
    fpidict   = getPiDict('#####',hidden=True)
    dirpidict = os.path.dirname(fpidict)
    fpidict   = os.path.basename(fpidict)
    #if hostname in fpidict:
    #    fpidict = fpidict.replace(hostname, shostname+".*")
    fpidict   = fpidict.replace('#####', '(\d*)')
    fnamere   = re.compile("^%s" % fpidict)
    try:
        for f in os.listdir(dirpidict):
            mo = fnamere.match(f)
            try:
                killMyPort(mo.group(1))
            except:
                pass
            pass
        pass
    except:
        pass
    # provide compatibility with old-style pidict file (not dot-prefixed)
    #fpidict   = getPiDict('(\d*)',hidden=False)
    fpidict   = getPiDict('#####',hidden=False)
    dirpidict = os.path.dirname(fpidict)
    fpidict   = os.path.basename(fpidict)
    #if hostname in fpidict:
    #    fpidict = fpidict.replace(hostname, shostname+".*")
    fpidict = fpidict.replace('#####', '(\d*)')
    fnamere   = re.compile("^%s$" % fpidict)
    try:
        for f in os.listdir(dirpidict):
            mo = fnamere.match(f)
            try:
                killMyPort(mo.group(1))
            except:
                pass
            pass
        pass
    except:
        pass
    # kill other processes
    if sys.platform != 'win32':
        import commands
        cmd = "ps -fea | grep '%s' | grep 'ghs3d' | grep 'f /tmp/GHS3D_' | grep -v 'grep' | awk '{print $2}'" % user
        prc = commands.getoutput(cmd)
        for field in prc.split():
            try:
                os.kill(int(field), signal.SIGKILL)
            except:
                pass
            pass
        pass
        # kill ompi-server needed for MPI containers coupling
        cmd = "ps -fea | grep '%s' | grep 'ompi-server' | grep -v 'grep' | awk '{print $2}'" % user
        prc = commands.getoutput(cmd)
        for field in prc.split():
            try:
                os.kill(int(field), signal.SIGKILL)
            except:
                pass
            pass
        pass
        # delete uri files needed by ompi-server
        cmd = "rm -f " + os.environ['HOME'] + "/.urifile_*"
        os.system(cmd)
    pass

if __name__ == "__main__":
    try:
        from salomeContextUtils import setOmniOrbUserPath
        setOmniOrbUserPath()
    except Exception, e:
        print e
        sys.exit(1)
    killAllPorts()
    pass
