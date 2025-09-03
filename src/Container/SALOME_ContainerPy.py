#! /usr/bin/env python3
#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2025  CEA, EDF, OPEN CASCADE
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

#  SALOME Container : implementation of container and engine for Kernel
#  File   : SALOME_ContainerPy.py
#  Author : Paul RASCLE, EDF
#  Module : SALOME
#  $Header$
#
import os
import sys
import importlib

from omniORB import CORBA, PortableServer
from salome.kernel import SALOMEDS
from salome.kernel import Engines, Engines__POA
import salome_psutil
from SALOME_NamingServicePy import *
from SALOME_Embedded_NamingService import SALOME_Embedded_NamingService
from SALOME_ComponentPy import *

from SALOME_utilities import *
from Utils_Identity import getShortHostName
from launchConfigureParser import verbose

#=============================================================================

#define an implementation of the container interface for the container implemented in Python

class SALOME_ContainerPy_Gen_i(Engines__POA.Container):
    """
    Implementation without naming_service server
    """
    _orb = None
    _poa = None
    _numInstance = 0
    _listInstances_map = {}
    _script = ""

    #-------------------------------------------------------------------------

    def __init__(self, orb, poa, containerName):
        MESSAGE( "SALOME_ContainerPy_i::__init__" )
        self._orb = orb
        self._poa = poa
        self._load_script = None
        myMachine=getShortHostName()
        Container_path = "/Containers/" + myMachine + "/" + containerName
        self._containerName = Container_path
        if verbose(): print("container name ",self._containerName)

    #-------------------------------------------------------------------------

    def instance(self, nameToRegister, componentName):
        MESSAGE(  "SALOME_ContainerPy_i::instance " + str(nameToRegister) + ' ' + str(componentName) )
        self._numInstance = self._numInstance +1
        instanceName = nameToRegister + "_inst_" + repr(self._numInstance)

        component=importlib.import_module(componentName)
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
        instanceName = nameToRegister + "_inst_" + repr(self._numInstance)
        interfaceName = nameToRegister
        the_command = "import " + nameToRegister + "\n"
        the_command = the_command + "comp_i = " + nameToRegister + "." + nameToRegister
        the_command = the_command + "(self._orb, self._poa, self._this(), self._containerName, instanceName, interfaceName)\n"
        MESSAGE( "SALOME_ContainerPy_i::load_impl :" + str (the_command) )
        exec(the_command)
        comp_o = comp_i._this()
        return comp_o

    #-------------------------------------------------------------------------

    def import_component(self, componentName):
        MESSAGE( "SALOME_Container_i::import_component" )
        reason = ""
        try:
            if verbose(): print("try import %s" % componentName)
            # try import component
            module=importlib.import_module(componentName)
            if verbose(): print("import %s is done successfully" % componentName)
            # if import successfully, check that component is loadable
            if not hasattr(module, componentName):
                reason = "module %s is not loadable" % componentName
                print(reason)
                pass
            pass
        except Exception:
            import traceback
            print("cannot import %s" % componentName)
            traceback.print_exc()
            reason = "cannot import %s" % componentName
        return reason

    #-------------------------------------------------------------------------

    def load_component_Library(self, componentName):
        MESSAGE(  "SALOME_ContainerPy_i::load_component_Library " + str(componentName) )
        ret = 0
        instanceName = componentName + "_inst_" + repr(self._numInstance)
        interfaceName = componentName
        reason = self.import_component(componentName)
        return reason == "", reason

    #-------------------------------------------------------------------------

    def create_component_instance_env(self, componentName, env):
      return self.create_component_instance(componentName), ""

    def create_component_instance(self, componentName):
        MESSAGE( "SALOME_ContainerPy_i::create_component_instance ==> " + str(componentName) )
        self._numInstance = self._numInstance +1
        instanceName = componentName + "_inst_" + repr(self._numInstance)
        comp_iors=""
        try:
            component=importlib.import_module(componentName)
            factory=getattr(component,componentName)
            comp_i=factory(self._orb,
                           self._poa,
                           self._this(),
                           self._containerName,
                           instanceName,
                           componentName)

            MESSAGE( "SALOME_Container_i::create_component_instance : OK")
            comp_o = comp_i._this()
            self._listInstances_map[instanceName] = comp_i
        except Exception:
            import traceback
            traceback.print_exc()
            MESSAGE( "SALOME_Container_i::create_component_instance : NOT OK")
        return comp_o

    #-------------------------------------------------------------------------

    def find_component_instance(self, registeredName):
        anEngine = None
        for instance in self._listInstances_map:
            if find(instance,registeredName) == 0:
                anEngine = self._listInstances_map[instance]
                return anEngine._this()
        return anEngine


    #-------------------------------------------------------------------------

    def create_python_service_instance(self, CompName):
        return self.create_component_instance(CompName)

    #-------------------------------------------------------------------------

    def remove_impl(self, component):
        MESSAGE( "SALOME_ContainerPy_i::remove_impl" )
        instanceName = component._get_instanceName()
        MESSAGE( "unload component " + str(instanceName) )
        self._listInstances_map.remove(instanceName)
        component.destroy()
        self._naming_service.Destroy_Name(str(instanceName))

    #-------------------------------------------------------------------------

    def finalize_removal(self):
        MESSAGE( "SALOME_ContainerPy_i::finalize_removal" )
        return None

    #-------------------------------------------------------------------------

    def ping(self):
        MESSAGE( "SALOME_ContainerPy_i::ping() pid " + str(os.getpid()) )
        return None

    #-------------------------------------------------------------------------

    def getPID(self):
        return os.getpid()

    #-------------------------------------------------------------------------

    def getNumberOfCPUCores(self):
        return salome_psutil.getNumberOfCPUCores()

    #-------------------------------------------------------------------------

    def loadOfCPUCores(self):
        return salome_psutil.loadOfCPUCores(self._load_script)

    #-------------------------------------------------------------------------

    def setPyScriptForCPULoad(self, script):
        self._load_script = script

    #-------------------------------------------------------------------------

    def resetScriptForCPULoad(self):
        self._load_script = None

    #-------------------------------------------------------------------------

    def getTotalPhysicalMemory(self):
        return salome_psutil.getTotalPhysicalMemory()

    #-------------------------------------------------------------------------

    def getTotalPhysicalMemoryInUse(self):
        return salome_psutil.getTotalPhysicalMemoryInUse()

    #-------------------------------------------------------------------------

    def getTotalPhysicalMemoryInUseByMe(self):
        return salome_psutil.getTotalPhysicalMemoryInUseByMe()

    #-------------------------------------------------------------------------

    def _get_name(self):
        MESSAGE( "SALOME_ContainerPy_i::_get_name" )
        return self._containerName

    #-------------------------------------------------------------------------

    def getHostName(self):
        MESSAGE( "SALOME_ContainerPy_i::_get_MachineName" )
        self._machineName = "localhost"
        return self._machineName

    #-------------------------------------------------------------------------

    def _get_machineName(self):
        MESSAGE( "SALOME_ContainerPy_i::_get_MachineName" )
        self._machineName = "localhost"
        return self._machineName

    #-------------------------------------------------------------------------

    def Shutdown(self):
        self._naming_service.Destroy_Name(self._containerName);
        self._naming_service.Destroy_FullDirectory(self._containerName);
        self._orb.shutdown(0)
        pass

    def _get_logfilename(self):
      return self._logfilename
    def _set_logfilename(self,logfilename):
      self._logfilename=logfilename
    def _get_workingdir(self):
      return os.getcwd()

#=============================================================================

class SALOME_ContainerPy_i(SALOME_ContainerPy_Gen_i):
    """
    Implementation with naming_service server
    """
    def __init__(self, orb, poa, containerName):
        SALOME_ContainerPy_Gen_i.__init__(self, orb, poa, containerName)
        naming_service = SALOME_NamingServicePy_i(self._orb)
        self._naming_service = naming_service
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

        # mpv: fix for SAL4731 - always create new file to write log of server
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

    pass

class SALOME_ContainerPy_SSL_i(SALOME_ContainerPy_Gen_i):
    """
    Implementation with naming_service server
    """
    def __init__(self, orb, poa, containerName):
        SALOME_ContainerPy_Gen_i.__init__(self, orb, poa, containerName)
        naming_service = SALOME_Embedded_NamingService()
        self._naming_service = naming_service._this()

    def get_embedded_NS_if_ssl(self):
        return self._naming_service

if __name__ == "__main__":
  # change the stdout buffering to line buffering (same as C++ cout buffering)
  sys.stdout=os.fdopen(1,"w",1)
  #initialise the ORB and find the root POA
  if verbose():print("Starting ",sys.argv[1])
  orb = CORBA.ORB_init(sys.argv, CORBA.ORB_ID)
  poa = orb.resolve_initial_references("RootPOA")
  if verbose():print("ORB and POA initialized")

  #create an instance of SALOME_ContainerPy_i and a Container reference
  #containerName = "FactoryServerPy"
  MESSAGE( str(sys.argv) )
  containerName = sys.argv[1]
  cpy_i = SALOME_ContainerPy_i(orb, poa, containerName)
  if verbose():print("SALOME_ContainerPy_i instance created ",cpy_i)
  cpy_o = cpy_i._this()
  if verbose():print("SALOME_ContainerPy_i instance activated ",cpy_o)
  sys.stdout.flush()
  sys.stderr.flush()

  #activate the POA
  poaManager = poa._get_the_POAManager()
  poaManager.activate()

  #Block for ever
  orb.run()
  if verbose():print("SALOME_ContainerPy_i shutdown")
