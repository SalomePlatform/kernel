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
#  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
#
#
#
#  File   : SALOME_ContainerPy.py
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

#=============================================================================

#define an implementation of the container interface

class SALOME_ContainerPy_i (Engines__POA.Container):
    _orb = None
    _poa = None
    _numInstance = 0

    #-------------------------------------------------------------------------

    def __init__(self, orb, poa, containerName):
        MESSAGE( "SALOME_ContainerPy_i::__init__" )
        self._orb = orb
        self._poa = poa
        self._containerName = containerName

        myMachine=getShortHostName()
        naming_service = SALOME_NamingServicePy_i(self._orb)
        self._naming_service = naming_service
        Container_path = "/Containers/" + myMachine + "/" + self._containerName
        MESSAGE( str(Container_path) )
        naming_service.Register(self._this(), Container_path)
            
    #-------------------------------------------------------------------------

    def start_impl(self, ContainerName):
        MESSAGE(  "SALOME_ContainerPy_i::start_impl " + str(ContainerName) )
        myMachine=getShortHostName()
        theContainer = "/Containers/" + myMachine + "/" + ContainerName
        try:
            obj = self._naming_service.Resolve(theContainer)
        except :
            obj = None
            MESSAGE(  "SALOME_ContainerPy_i::start_impl " + str(ContainerName) + ".object not found in Naming Service" )
        if obj is None:
            container = None
        else:
            container = obj._narrow(Engines.Container)
            if container is None:
                MESSAGE( "SALOME_ContainerPy_i::start_impl " + str(containerName) + ".object exists but is not a Container" )
            else :
                MESSAGE( "SALOME_ContainerPy_i::start_impl " + str(ContainerName) + ".object found without new launch" )
            return container
        #shstr = os.getenv( "PWD" ) + "/"
        #shstr += "runSession ./SALOME_ContainerPy.py "
        shstr = os.getenv("KERNEL_ROOT_DIR") + "/bin/salome/SALOME_ContainerPy.py ";
        #shstr = "runSession SALOME_ContainerPy.py "
        shstr += ContainerName

        # mpv: fix for SAL4731 - allways create new file to write log of server
        num = 1
        fileName = ""
        while 1:
            fileName = "/tmp/"+ContainerName+"_%i.log"%num
            if not os.path.exists(fileName):
                break
            num += 1
            pass
        
        shstr += " > "
        shstr += fileName
        shstr += " 2>&1 &"
        
        #shstr += " > /tmp/"
        #shstr += ContainerName
        #shstr += ".log 2>&1 &"
        
        MESSAGE(  "SALOME_ContainerPy_i::start_impl " + "os.system(" + str(shstr) + ")" )
        os.system( shstr )
        count = 21
        while container is None :
            time.sleep(1)
            count = count - 1
            MESSAGE(  str(count) + ". Waiting for " + str(theContainer) )
            try :
                obj = self._naming_service.Resolve(theContainer)
            except :
                obj = None
            if obj is None:
                container = None
            else:
                container = obj._narrow(Engines.Container)
                if container is None:
                    MESSAGE(  str(containerName) + ".object exists but is not a Container" )
                return container
            if count == 0 :
                return container

    #-------------------------------------------------------------------------

    def instance(self, nameToRegister, componentName):
        MESSAGE(  "SALOME_ContainerPy_i::instance " + str(nameToRegister) + ' ' + str(componentName) )
        self._numInstance = self._numInstance +1
        instanceName = nameToRegister + "_inst_" + `self._numInstance`

	component=__import__(componentName)
	factory=getattr(component,componentName)
	comp_i=factory(self._orb, self._poa, self._this(), self._containerName,
	               instanceName, nameToRegister)

        MESSAGE( "SALOME_ContainerPy_i::instance : component created")
        comp_o = comp_i._this()
        return comp_o

    #-------------------------------------------------------------------------

    def load_impl(self, nameToRegister, componentName):
        MESSAGE(  "SALOME_ContainerPy_i::load_impl " + str(nameToRegister) + ' ' + str(componentName) )
        self._numInstance = self._numInstance +1
        instanceName = nameToRegister + "_inst_" + `self._numInstance`
        interfaceName = nameToRegister
        the_command = "import " + nameToRegister + "\n"
        the_command = the_command + "comp_i = " + nameToRegister + "." + nameToRegister
        the_command = the_command + "(self._orb, self._poa, self._this(), self._containerName, instanceName, interfaceName)\n"
        MESSAGE( "SALOME_ContainerPy_i::load_impl :" + str (the_command) )
        exec the_command
        comp_o = comp_i._this()
        return comp_o
    
    #-------------------------------------------------------------------------

    def remove_impl(self, component):
        MESSAGE( "SALOME_ContainerPy_i::remove_impl" )
        return None

    #-------------------------------------------------------------------------

    def finalize_removal(self):
        MESSAGE( "SALOME_ContainerPy_i::finalize_removal" )
        return None

    #-------------------------------------------------------------------------

    def ping(self):
        MESSAGE( "SALOME_ContainerPy_i::ping" )
        return None

    #-------------------------------------------------------------------------

    def _get_name(self):
        MESSAGE( "SALOME_ContainerPy_i::_get_name" )
        return self._containerName

    #-------------------------------------------------------------------------

    def _get_machineName(self):
        MESSAGE( "SALOME_ContainerPy_i::_get_MachineName" )
        self._machineName = "localhost"
        return self._machineName

#=============================================================================

#initialise the ORB and find the root POA
orb = CORBA.ORB_init(sys.argv, CORBA.ORB_ID)
poa = orb.resolve_initial_references("RootPOA")

#create an instance of SALOME_ContainerPy_i and a Container reference
#containerName = "FactoryServerPy"
MESSAGE( str(sys.argv) )
containerName = sys.argv[1]
cpy_i = SALOME_ContainerPy_i(orb, poa, containerName)
cpy_o = cpy_i._this()

#activate the POA
poaManager = poa._get_the_POAManager()
poaManager.activate()

#Block for ever
orb.run()


        
            


