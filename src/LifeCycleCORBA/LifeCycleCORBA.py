#  SALOME LifeCycleC RBA : implementation of containers and engines life cycle both in Python and C++
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
#  File   : LifeCycleCORBA.py
#  Author : Paul RASCLE, EDF
#  Module : SALOME
#  $Header$

import os
import sys
import time
import string
from omniORB import CORBA
import CosNaming
import Engines
reload(Engines)
import SALOME_ModuleCatalog

from SALOME_utilities import *
from Utils_Identity import getShortHostName
import Utils_Identity 
import Launchers

class LifeCycleCORBA:
    _orb = None
    _rootcontext = None
    _containerRootContext = None
    _catalog = None
    
    #-------------------------------------------------------------------------

    def __init__(self, orb):
        MESSAGE( "LifeCycleCORBA::__init__" )
        self._orb = orb

        obj = self._orb.resolve_initial_references("NameService")
        self._rootContext = obj._narrow(CosNaming.NamingContext)

        if self._rootContext is None:
            MESSAGE( "Name Service Reference is invalid" )

        name = [CosNaming.NameComponent("Containers","dir")]
        try:
            self._containerRootContext = self._rootContext.bind_new_context(name)

        except CosNaming.NamingContext.AlreadyBound, ex:
            MESSAGE( "/Containers.dir Context already exists" )
            obj = self._rootContext.resolve(name)
            self._containerRootContext = obj._narrow(CosNaming.NamingContext)
            if self._containerRootContext is None:
                MESSAGE( "Containers.dir exists but it is not a NamingContext" )

        name = [CosNaming.NameComponent("Kernel","dir"),
                CosNaming.NameComponent("ModulCatalog","object")]
        try:
            obj = self._rootContext.resolve(name)
        except CosNaming.NamingContext.NotFound, ex:
            MESSAGE( "/Kernel.dir/ModulCatalog.object not found in Naming Service" )

        self._catalog = obj._narrow(SALOME_ModuleCatalog.ModuleCatalog)
        if self._catalog is None:
            MESSAGE( "/Kernel.dir/ModulCatalog.object exists but is not a ModulCatalog" )

        name = [CosNaming.NameComponent("ContainerManager","object")]
        try:
            obj = self._rootContext.resolve(name)
        except CosNaming.NamingContext.NotFound, ex:
            MESSAGE( "ContainerManager.object not found in Naming Service" )
        self._contManager = obj._narrow(Engines.ContainerManager)
        if self._contManager is None:
            MESSAGE( "ContainerManager.object exists but is not a ContainerManager")

    #-------------------------------------------------------------------------

    def ContainerName(self, containerName):
        theComputer = ""
        try:
            theComputer , theContainer = containerName.split('/')
        except:
            theComputer = ""
            theContainer = containerName

        if theComputer in ("","localhost") :
            theComputer = getShortHostName()

        MESSAGE( theComputer + theContainer )
        return theComputer,theContainer

    #-------------------------------------------------------------------------

    def ComputerPath(self, ComputerName ):
        try:
            #path = self._catalog.GetPathPrefix( ComputerName )
            path = os.getenv("KERNEL_ROOT_DIR") + "/bin/salome/"
        except SALOME_ModuleCatalog.NotFound, ex:
            path = ""
        return path

    #-------------------------------------------------------------------------

    def FindContainer(self, containerName):
        theComputer,theContainer = self.ContainerName( containerName )
        name = [CosNaming.NameComponent(theComputer,"dir"),
                CosNaming.NameComponent(theContainer,"object")]
        obj = None
        try:
            obj = self._containerRootContext.resolve(name)
            MESSAGE( containerName + ".object found in Naming Service" )

        except CosNaming.NamingContext.NotFound, ex:
            MESSAGE( containerName + ".object not found in Naming Service" )

        if obj is None:
            container = None
        else:
            container = obj._narrow(Engines.Container)
            if container is None:
                MESSAGE( containerName + ".object exists but is not a Container" )
        return container
    
    #-------------------------------------------------------------------------

    def FindComponent(self,containerName,componentName,listOfMachines):
        if containerName!="":
            machinesOK=[]
            for i in range(len(listOfMachines)):
                currentMachine=listOfMachines[i]
                componentNameForNS= [CosNaming.NameComponent(currentMachine,"dir"),
                                     CosNaming.NameComponent(containerName,"dir"),
                                     CosNaming.NameComponent(componentName,"object")]
                obj=None
                try:
                    obj = self._containerRootContext.resolve(componentNameForNS)
                except CosNaming.NamingContext.NotFound, ex:
                    MESSAGE( "component " + componentName + " not found on machine " + currentMachine + " , trying to load" )
                    pass
                if obj is not None:
                    machinesOK.append(currentMachine)
                    pass
                pass
            if len(machinesOK)!=0:
                bestMachine=self._contManager.FindBest(machinesOK)
                componentNameForNS= [CosNaming.NameComponent(bestMachine,"dir"),
                                     CosNaming.NameComponent(containerName,"dir"),
                                     CosNaming.NameComponent(componentName,"object")]
                obj=None
                try:
                    obj = self._containerRootContext.resolve(componentNameForNS)
                except:
                    pass
                if obj is not None:
                    return obj._narrow(Engines.Component)
                else:
                    MESSAGE( "Big problem !!!")
                    return None
            else:
                return None
        else:
            bestMachine=self._contManager.FindBest(listOfMachines)
            MESSAGE("Not implemented yet ...")
            return None
        pass

    #-------------------------------------------------------------------------

    def setLauncher(self,name):
        """Change default launcher to the launcher identified by name

           See module Launchers.py
        """
        Launchers.setLauncher(name)

    #-------------------------------------------------------------------------

    def StartContainer(self, theComputer , theContainer ):
        """Start a container on theComputer machine with theContainer name
	"""
	# Get the Naming Service address
	#
        addr=self._orb.object_to_string(self._rootContext)
	#
	# If container name contains "Py" launch a Python Container
	#
        if theContainer.find('Py') == -1 :
           CMD=['SALOME_Container',theContainer,'-ORBInitRef','NameService='+addr]
        else:
           CMD=['SALOME_ContainerPy.py',theContainer,'-ORBInitRef','NameService='+addr]
        if theComputer in ("","localhost"):
           theComputer=getShortHostName()
	#
	# Get the appropriate launcher and ask to launch
	#
        Launchers.getLauncher(theComputer).launch(theComputer,CMD)
	#
	# Wait until the container is registered in Naming Service
	#
        count =5 
	aContainer=None
        while aContainer is None and count > 0:
            time.sleep(1)
            count = count - 1
            MESSAGE( str(count) + ". Waiting for " + theComputer + "/" + theContainer )
            aContainer = self.FindContainer( theComputer + "/" + theContainer )
	return aContainer

    #-------------------------------------------------------------------------

    def FindOrStartContainer(self, theComputer , theContainer ):
        """Find or Start a container on theComputer machine with theContainer name
	"""
        if theComputer in ("","localhost"):
           theComputer=getShortHostName()
        MESSAGE( "FindOrStartContainer: " + theComputer + theContainer )
        aContainer = self.FindContainer( theComputer + "/" + theContainer )
        if aContainer is None :
            aContainer= self.StartContainer(theComputer , theContainer )
	return aContainer
	    
    #-------------------------------------------------------------------------

    def LoadComponent(self,containerName,componentName,listOfMachine):
        container=self._contManager.FindOrStartContainer(containerName,listOfMachine)
        implementation="lib"+componentName+"Engine.so"
        try:
            component = container.load_impl(componentName, implementation)
            MESSAGE( "component " + component._get_instanceName() + " launched !" )
            return component
        except:
            MESSAGE( "component " + componentName + " NOT launched !" )
            return None

    #-------------------------------------------------------------------------
    

    def FindOrLoadComponent(self, containerName, componentName):
        sp=containerName.split("/")
        if len(sp)==1:
            listOfMachine=[]
            listOfMachine.append(getShortHostName())
            comp=self.FindComponent(containerName,componentName,listOfMachine)
            if comp is None:
                return self.LoadComponent(containerName,componentName,listOfMachine)
            else:
                return comp
            pass
        else:
            params= Engines.MachineParameters(sp[1],sp[0],"LINUX",0,0,0,0)
            listOfMachine=self._contManager.GetFittingResources(params,componentName)
            ret=self.FindComponent(sp[1],componentName,listOfMachine);
            if ret is None:
                return self.LoadComponent(sp[1],componentName,listOfMachine)
            else:
                return ret
            pass
        
