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

import os, string, sys, re

from killSalomeWithPort import killMyPort, getPiDict

def killAllPorts():
    user = os.getenv('USER')
    filedict = "^%s$"%(getPiDict('(\d*)',full=False))
    fnamere = re.compile(filedict)
    for file in os.listdir(os.getenv("HOME")):
        mo = re.match(fnamere,file)
        if mo and len(mo.groups()):
            killMyPort(mo.groups()[0])
        pass

if __name__ == "__main__":
    killAllPorts()
