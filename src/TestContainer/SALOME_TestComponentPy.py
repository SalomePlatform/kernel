#! /usr/bin/env python

#=============================================================================
# File      : SALOME_TestComponentPy.py
# Created   : lun sep  3 17:54:13 CEST 2001
# Author    : Paul RASCLE, EDF
# Project   : SALOME
# Copyright : EDF 2001
# $Header$
#=============================================================================

import sys
from omniORB import CORBA, PortableServer
import CosNaming
import Engines, Engines__POA
from SALOME_ComponentPy import *

class SALOME_TestComponentPy( Engines__POA.TestComponent, SALOME_ComponentPy_i):

    def Coucou(self, val):
        print "SALOME_TestComponentPy Coucou ", val
        result = "result from SALOME_TestComponentPy Coucou " + `val`
        return result
    
    def __init__(self, orb, poa, contID, containerName, instanceName, interfaceName):
        notif = 0
        SALOME_ComponentPy_i.__init__(self, orb, poa, contID, containerName,
                                      instanceName, interfaceName, notif)
        print "SALOME_TestComponentPy::__init__"

