#!/usr/bin/env python
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

## \file waitContainers.py
# \brief command to wait until containers are launched
#
# the container names (in naming service) are passed on the command line as in :
# waitContainers.py FactoryServer

import sys
import time
import orbmodule
import CosNaming
clt = orbmodule.client()
clt.waitNS("/ContainerManager")
obj = clt.orb.resolve_initial_references("NameService")
rootContext = obj._narrow(CosNaming.NamingContext)
cname = []
cname.append(CosNaming.NameComponent('Containers', 'dir'))

while(1):
  try:
    ccontext = rootContext.resolve(cname)
    break
  except CosNaming.NamingContext.NotFound, ex:
    time.sleep(1)
  except CosNaming.NamingContext.InvalidName, ex:
    time.sleep(1)
  except CosNaming.NamingContext.CannotProceed, ex:
    time.sleep(1)
  except (CORBA.TRANSIENT,CORBA.OBJECT_NOT_EXIST,CORBA.COMM_FAILURE):
    time.sleep(1)

def waitContainer(mycont):
  while(1):
    bl,bi=ccontext.list(0)
    if bi is not None:
      ok,b=bi.next_one()
      while(ok):
        for s in b.binding_name :
          if s.kind == "dir":
            obj=ccontext.resolve([s])
            scontext = obj._narrow(CosNaming.NamingContext)
            bll,bii=scontext.list(0)
            if bii is not None:
              ok,bb=bii.next_one()
              while(ok):
                for s in bb.binding_name :
                   if s.id == mycont:
                     print s.id
                     return
                ok,bb=bii.next_one()
        ok,b=bi.next_one()
    sys.stdout.write('+')
    sys.stdout.flush()
    time.sleep(1)

for cont in sys.argv:
  if cont != sys.argv[0]:
    waitContainer(cont)

