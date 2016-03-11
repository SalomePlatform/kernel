#! /usr/bin/env python
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

#  SALOME TestContainer : test of container creation and its life cycle
#  File   : TestComponentPy.py
#  Author : Paul RASCLE, EDF
#  Module : SALOME
#  $Header$
#
import os
import sys
import string
from omniORB import CORBA
import CosNaming
import Engines
from Utils_Identity import getShortHostName


#initialise the ORB
orb = CORBA.ORB_init(sys.argv, CORBA.ORB_ID)

#obtain a reference to the root naming context
obj = orb.resolve_initial_references("NameService")
rootContext = obj._narrow(CosNaming.NamingContext)

if rootContext is None:
    print "Name Service Reference is invalid"
    sys.exit(1)

#resolve the name /Containers.dir/FactoryServerPy.object
machineName=getShortHostName()
containerName = "FactoryServer"
name = [CosNaming.NameComponent("Containers","dir"),
        CosNaming.NameComponent(machineName,"dir"),
        CosNaming.NameComponent(containerName,"object")]

try:
    obj = rootContext.resolve(name)
except CosNaming.NamingContext.NotFound, ex:
    print  containerName , " not found in Naming Service"
    sys.exit(1)

container = obj._narrow(Engines.Container)
print container.getHostName()
comp = container.load_impl("SalomeTestComponent","SalomeTestComponent")
print comp._get_instanceName()
comp.ping()
comptest = comp._narrow(Engines.TestComponent)
if comptest is None:
    print "probleme cast"
print comptest.Coucou(1)
