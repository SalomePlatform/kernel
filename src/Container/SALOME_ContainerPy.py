#! /usr/bin/env python

#=============================================================================
# File      : SALOME_ContainerPy.py
# Created   : lun sep  3 17:54:13 CEST 2001
# Author    : Paul RASCLE, EDF
# Project   : SALOME
# Copyright : EDF 2001
# $Header$
#=============================================================================

import os
import sys
import string
from omniORB import CORBA, PortableServer
# import SALOMEDS before other SALOME modules
# (if not, incomplete import done by SALOME module: no load of SALOMEDS_attributes)
import SALOMEDS 
import Engines, Engines__POA
from SALOME_NamingServicePy import *
from SALOME_ComponentPy import *

from SALOME_utilities import *

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

        myMachine=string.split(os.getenv( "HOSTNAME" ),'.')
        naming_service = SALOME_NamingServicePy_i(self._orb)
        self._naming_service = naming_service
        Container_path = "/Containers/" + myMachine[0] + "/" + self._containerName
        MESSAGE( str(Container_path) )
        naming_service.Register(self._this(), Container_path)
            
    #-------------------------------------------------------------------------

    def start_impl(self, ContainerName):
        MESSAGE(  "SALOME_ContainerPy_i::start_impl " + str(ContainerName) )
        myMachine=string.split(os.getenv( "HOSTNAME" ),'.')
        theContainer = "/Containers/" + myMachine[0] + "/" + ContainerName
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
                MESSAGE( "SALOME_ContainerPy_i::start_impl " + str(ContainerName) + ".object found without runSession" )
            return container
        shstr = os.getenv( "PWD" ) + "/"
        shstr += "runSession ./SALOME_ContainerPy.py "
        shstr += ContainerName
        shstr += " > /tmp/"
        shstr += ContainerName
        shstr += ".log 2>&1 &"
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

    #-------------------------------------------------------------------------

    def finalize_removal(self):
        MESSAGE( "SALOME_ContainerPy_i::finalize_removal" )

    #-------------------------------------------------------------------------

    def ping(self):
        MESSAGE( "SALOME_ContainerPy_i::ping" )

    #-------------------------------------------------------------------------

    def _get_name(self):
        MESSAGE( "SALOME_ContainerPy_i::_get_name" )

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


        
            


