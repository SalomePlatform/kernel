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
#  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
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
import SALOME_ModuleCatalog

from SALOME_utilities import *

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

    #-------------------------------------------------------------------------

    def ContainerName(self, containerName):
        theComputer = ""
        try:
            theComputer , theContainer = containerName.split('/')
        except:
            theComputer = ""
            theContainer = containerName
        if theComputer == "" :
            theComputer = os.getenv("HOSTNAME")
        if theComputer == "localhost" :
            theComputer = os.getenv("HOSTNAME")
        computerSplitName = theComputer.split('.')
        theComputer = computerSplitName[0]
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

    def FindOrStartContainer(self, theComputer , theContainer ):
        MESSAGE( "FindOrStartContainer" + theComputer + theContainer )
        aContainer = self.FindContainer( theComputer + "/" + theContainer )
        if aContainer is None :
            if (theContainer == "FactoryServer") | (theContainer == "FactoryServerPy") :
                myMachine=string.split(os.getenv( "HOSTNAME" ),'.')
                if theComputer == myMachine[0] :
                    rshstr = ""
                else :
                    rshstr = "rsh -n " + theComputer + " "
                path = self.ComputerPath( theComputer )
##                if path != "" :
##                    rshstr = rshstr + path + "/../bin/"
##                else :
##                    rshstr = rshstr + os.getenv( "KERNEL_ROOT_DIR" ) + "/bin/"
                if theContainer == "FactoryServer" :
                    rshstr = rshstr + path + "SALOME_Container "
                else :
                    rshstr = rshstr + path + "SALOME_ContainerPy.py '"
                rshstr = rshstr + theContainer + " -"
		omniORBcfg = os.getenv( "OMNIORB_CONFIG" )
                file = os.open( omniORBcfg , os.O_RDONLY )
                ORBInitRef = os.read(file,132)
                if ORBInitRef[len(ORBInitRef)-1] == '\n' :
                    ORBInitRef,bsn = ORBInitRef.split('\n')
                os.close( file )
                rshstr = rshstr + ORBInitRef
                if theContainer == "FactoryServerPy" :
                    rshstr = rshstr + "'"
                rshstr = rshstr + " > /tmp/" + theContainer + "_"
                rshstr = rshstr + theComputer
                rshstr = rshstr + ".log 2>&1 &"
                os.system( rshstr )
                MESSAGE( "FindOrStartContainer" + rshstr + " done" )
            else :
                if theContainer.find('Py') == -1 :
                    aContainer = self.FindContainer( theComputer + "/" + "FactoryServer" )
                else :
                    aContainer = self.FindContainer( theComputer + "/" + "FactoryServerPy" )
                aContainer = aContainer.start_impl( theContainer )

            count = 21
            while aContainer is None :
                time.sleep(1)
                count = count - 1
                MESSAGE( str(count) + ". Waiting for " + theComputer + "/" + theContainer )
                aContainer = self.FindContainer( theComputer + "/" + theContainer )
                if count == 0 :
                    return aContainer
            
        return  aContainer       
        #os.system("rsh -n dm2s0017 /export/home/KERNEL_ROOT/bin/runSession SALOME_Container -ORBInitRef NameService=corbaname::dm2s0017:1515")

    #-------------------------------------------------------------------------

    def FindOrLoadComponent(self, containerName, componentName):

        theComputer,theContainer = self.ContainerName( containerName )
        name = [CosNaming.NameComponent(theComputer,"dir"),
                CosNaming.NameComponent(theContainer,"dir"),
                CosNaming.NameComponent(componentName,"object")]
        try:
            obj = self._containerRootContext.resolve(name)
        except CosNaming.NamingContext.NotFound, ex:
            MESSAGE( "component " + componentName + " not found, trying to load" )
            container = self.FindContainer(theComputer + "/" + theContainer)
            if container is None:
                MESSAGE( "container " + theComputer + "/" + theContainer + " not found in Naming Service, trying to start" )
                if (theContainer != "FactoryServer") & (theContainer != "FactoryServerPy") :
                    if theContainer.find('Py') == -1 :
                        theFactorycontainer = "FactoryServer"
                    else :
                        theFactorycontainer = "FactoryServerPy"
                    Factorycontainer = self.FindContainer(theComputer + "/" + theFactorycontainer)
                    if Factorycontainer is None:
                        MESSAGE( "container " + theComputer + "/" + theFactorycontainer + " not found in Naming Service, trying to start" )
                        Factorycontainer = self.FindOrStartContainer(theComputer,theFactorycontainer)
                else:
                    Factorycontainer = self.FindOrStartContainer(theComputer,theContainer)
                if Factorycontainer != None :
                    container = self.FindOrStartContainer(theComputer,theContainer)

            if container != None:
                compoinfo = self._catalog.GetComponent(componentName)
                if compoinfo is None:
                    MESSAGE( "component " + componentName + " not found in Module Catalog" )
                else:
                    try:
                        machineName = theComputer
                        path = compoinfo.GetPathPrefix(machineName) + "/"
                    except SALOME_ModuleCatalog.NotFound, ex:
                        MESSAGE( "machine " + machineName + " not found in Module Catalog" )
                        MESSAGE( "trying localhost" )
                        try:
                            path = compoinfo.GetPathPrefix("localhost") + "/"
                        except SALOME_ModuleCatalog.NotFound, ex:
                            path = ""
                    implementation = path + "lib" + componentName + "Engine.so"
                    MESSAGE( "Trying to load " + implementation )
                    try:
                        component = container.load_impl(componentName, implementation)
                        MESSAGE( "component " + component._get_instanceName() + " launched !" )
                        return component
                    except:
                        MESSAGE( "component " + componentName + " NOT launched !" )

        else:
            try:
                component = obj._narrow(Engines.Component)
                if component is None:
                    MESSAGE( componentName + " is not a component !" )
                else:
                    MESSAGE( "component " + component._get_instanceName() + " found !" )
                return component
            except:
                MESSAGE( componentName + " failure" )
                return None
