#! /usr/bin/env python
#
#  SALOME Container : implementation of container and engine for Kernel
#
#  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
#  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
# 
#  This library is free software; you can redistribute it and/or 
#  modify it under the terms of the GNU Lesser General Public 
#  License as published by the Free Software Foundation; either 
#  version 2.1 of the License. 
# 
#  This library is distributed in the hope that it will be useful, 
#  but WITHOUT ANY WARRANTY; without even the implied warranty of 
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
#  Lesser General Public License for more details. 
# 
#  You should have received a copy of the GNU Lesser General Public 
#  License along with this library; if not, write to the Free Software 
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
# 
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
#
#
#  File   : SALOME_Container.py
#  Author : Paul RASCLE, EDF
#  Module : SALOME
#  $Header$

import os
import sys
import string
from omniORB import CORBA, PortableServer
# import SALOMEDS before other SALOME modules
# (if not, incomplete import done by SALOME module: no load of SALOMEDS_attributes)
import SALOMEDS 
import Engines, Engines__POA
reload(Engines)
reload(Engines__POA)
from SALOME_NamingServicePy import *
from SALOME_ComponentPy import *

from SALOME_utilities import *
from Utils_Identity import getShortHostName
from launchConfigureParser import verbose

#=============================================================================

#define an implementation of the container interface

class SALOME_Container_i:
    _orb = None
    _poa = None
    _containerName = ""
    _naming_service = None

    #-------------------------------------------------------------------------

    def __init__(self ,containerName, containerIORStr):
        MESSAGE( "SALOME_Container_i::__init__" )
        self._orb = CORBA.ORB_init(sys.argv, CORBA.ORB_ID)
        self._poa = self._orb.resolve_initial_references("RootPOA")
        self._containerName = containerName
        if verbose(): print "SALOME_Container.SALOME_Container_i : _containerName ",self._containerName
        #self._naming_service = SALOME_NamingServicePy_i(self._orb)
        self._container = self._orb.string_to_object(containerIORStr)

    #-------------------------------------------------------------------------

    def import_component(self, componentName):
        MESSAGE( "SALOME_Container_i::import_component" )
        ret=0
        try:
            print "try import ",componentName
            __import__(componentName)
            print "import ",componentName," successful"
            ret=1
        except:
            import traceback
            traceback.print_exc()
            print "import ",componentName," not possible"
        return ret
        
    #-------------------------------------------------------------------------

    def create_component_instance(self, componentName, instanceName, studyId):
        MESSAGE( "SALOME_Container_i::create_component_instance" )
        comp_iors=""
        try:
            component=__import__(componentName)
            factory=getattr(component,componentName)
            comp_i=factory(self._orb,
                           self._poa,
                           self._container,
                           self._containerName,
                           instanceName,
                           componentName)

            MESSAGE( "SALOME_Container_i::create_component_instance : OK")
            comp_o = comp_i._this()
            comp_iors = self._orb.object_to_string(comp_o)
        except:
            import traceback
            traceback.print_exc()
            MESSAGE( "SALOME_Container_i::create_component_instance : NOT OK")
        return comp_iors 
        

