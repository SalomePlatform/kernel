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

#  SALOME TestContainer : test of container creation and its life cycle
#  File   : SALOME_TestComponentPy.py
#  Author : Paul RASCLE, EDF
#  Module : SALOME
#  $Header$
#
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
        notif = False
        SALOME_ComponentPy_i.__init__(self, orb, poa, contID, containerName,
                                      instanceName, interfaceName, notif)
        print "SALOME_TestComponentPy::__init__"

