#!/usr/bin/env python

# Copyright (C) 2005  OPEN CASCADE, CEA, EDF R&D, LEG
#           PRINCIPIA R&D, EADS CCR, Lip6, BV, CEDRAT
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either 
# version 2.1 of the License.
# 
# This library is distributed in the hope that it will be useful 
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

import os, sys, re, signal

from killSalomeWithPort import killMyPort, getPiDict

def killAllPorts():
    """
    Kill all SALOME sessions belonging to the user.
    """
    user = os.getenv('USER')
    # new-style dot-prefixed pidict file
    fnamere  = re.compile("^%s$"%(getPiDict('(\d*)',full=False,hidden=True)))
    # provide compatibility with old-style pidict file (not dot-prefixed)
    fnamere1 = re.compile("^%s$"%(getPiDict('(\d*)',full=False,hidden=False)))
    for file in os.listdir(os.getenv("HOME")):
        mo = fnamere.match(file)
        if not mo: mo = fnamere1.match(file)
        if mo and len(mo.groups()):
            killMyPort(mo.group(1))
        pass

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
    pass

if __name__ == "__main__":
    killAllPorts()
    pass
    
