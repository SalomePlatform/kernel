#! /usr/bin/env python

#=============================================================================
# File      : TestComponentPy.py
# Created   : lun sep  3 17:54:13 CEST 2001
# Author    : Paul RASCLE, EDF
# Project   : SALOME
# Copyright : EDF 2001
# $Header$
#=============================================================================

import os
import sys
import string
from omniORB import CORBA
import CosNaming
import Engines


#initialise the ORB
orb = CORBA.ORB_init(sys.argv, CORBA.ORB_ID)

#obtain a reference to the root naming context
obj = orb.resolve_initial_references("NameService")
rootContext = obj._narrow(CosNaming.NamingContext)

if rootContext is None:
    print "Name Service Reference is invalid"
    sys.exit(1)

#resolve the name /Containers.dir/FactoryServerPy.object
myMachine=string.split(os.getenv( "HOSTNAME" ),'.')
machineName= myMachine[0]
containerName = "FactoryServerPy"
name = [CosNaming.NameComponent("Containers","dir"),
        CosNaming.NameComponent(machineName,"dir"),
        CosNaming.NameComponent(containerName,"object")]

try:
    obj = rootContext.resolve(name)
except CosNaming.NamingContext.NotFound, ex:
    print  containerName , " not found in Naming Service"
    sys.exit(1)

container = obj._narrow(Engines.Container)
print container._get_machineName()
comp = container.load_impl("SALOME_TestComponentPy","SALOME_TestComponentPy")
print comp._get_instanceName()
comp.ping()
comptest = comp._narrow(Engines.TestComponent)
if comptest is None:
    print "probleme cast"
print comptest.Coucou(1)


