#! /usr/bin/env python

#=============================================================================
# File      : SALOME_ComponentPy.py
# Created   : lun sep  3 17:54:13 CEST 2001
# Author    : Paul RASCLE, EDF
# Project   : SALOME
# Copyright : EDF 2001
# $Header$
#=============================================================================

import os
import sys
from omniORB import CORBA, PortableServer
import Engines, Engines__POA
import Registry
from Utils_Identity import *
from SALOME_NamingServicePy import *
from libNOTIFICATION import *

from SALOME_utilities import *

#=============================================================================

#define an implementation of the component interface

class SALOME_ComponentPy_i (Engines__POA.Component):
    _orb = None
    _poa = None
    
    #-------------------------------------------------------------------------

    def __init__ (self, orb, poa, contID, containerName,
                  instanceName, interfaceName, notif):
        # Notif for notification services
        # NOT YET IMPLEMENTED
        MESSAGE(  "SALOME_ComponentPy_i::__init__" + " " + str (containerName) + " " + str(instanceName) + " " + str(interfaceName) )
        self._orb = orb
        self._poa = poa
        self._instanceName = instanceName
        self._interfaceName = interfaceName
        self._containerName = containerName
        self._notif = notif
        self._myConnexionToRegistry = 0
        self._graphName = ''
        self._nodeName = ''
        self._ThreadId = 0

        naming_service = SALOME_NamingServicePy_i(self._orb)
        Component_path = "/Containers/" +  os.getenv( "HOSTNAME" ) + "/" + self._containerName + "/" + self._interfaceName
        MESSAGE(  'SALOME_ComponentPy_i Register' + str( Component_path ) )
        naming_service.Register(self._this(), Component_path)

        # Add componentinstance to registry
        obj = naming_service.Resolve('/Registry')
        if obj is None:
            MESSAGE(  "Registry Reference is invalid" )
        else:
            regist = obj._narrow(Registry.Components)
            if regist is None:
                MESSAGE(  "Registry Reference is invalid" )
            else:
                ior = orb.object_to_string(contID)
                MESSAGE(  ior )

                lesInfos = Identity(self._instanceName)
                infos = Registry.Infos(lesInfos._name,
                                       lesInfos._pid,
                                       lesInfos._machine,
                                       lesInfos._adip,
                                       lesInfos._uid,
                                       lesInfos._pwname,
                                       int(lesInfos._tc_start),
                                       0,0,0,
                                       lesInfos._cdir,
                                       -1,
                                       ior)

                res = regist.add(infos)

        self._notifSupplier = NOTIFICATION_Supplier_Swig(instanceName, notif)

    #-------------------------------------------------------------------------

    def _get_instanceName(self):
        MESSAGE(  "SALOME_ComponentPy_i::_get_instanceName" )
        return self._instanceName
    
    #-------------------------------------------------------------------------

    def ping(self):
        MESSAGE(  "SALOME_ComponentPy_i::ping" )
        
    #-------------------------------------------------------------------------

    def destroy(self):
        MESSAGE(  "SALOME_ComponentPy_i::destroy" )
        poa.deactivate_object(self)
        CORBA.release(_poa)
        
    #-------------------------------------------------------------------------

    def GetContainerRef(self):
        MESSAGE(  "SALOME_ComponentPy_i::GetContainerRef" )
        
    #-------------------------------------------------------------------------

    def beginService(self , serviceName ):
        MESSAGE(  "Send BeginService notification for " + str(serviceName) + "for graph/node" + str(self._graphName) + str(self._nodeName) )
        MESSAGE(  "Component instance : " + str ( self._instanceName ) )

    #-------------------------------------------------------------------------

    def endService(self , serviceName ):
        MESSAGE(  "Send EndService notification for " + str(serviceName) + "for graph/node" + str(self._graphName) + str(self._nodeName) )
        MESSAGE(  "Component instance : " + str(self._instanceName) )

    #-------------------------------------------------------------------------

    def sendMessage(self , event_type , message ):
        self._notifSupplier.Send(self.graphName(), self.nodeName(), event_type, message)

    #-------------------------------------------------------------------------

    def Names(self , GraphName , NodeName ):
        MESSAGE(  "SALOME_ComponentPy_i::Names" + str(GraphName) + str(NodeName) )
        self._graphName = GraphName
        self._nodeName = NodeName
        
    #-------------------------------------------------------------------------

    def graphName(self):
        return self._graphName

    #-------------------------------------------------------------------------

    def nodeName(self):
        return self._nodeName

    #-------------------------------------------------------------------------

    def Kill(self):
        MESSAGE(  "SALOME_ComponentPy_i::Kill not yet implemented" )

    #-------------------------------------------------------------------------

    def Stop(self):
        MESSAGE(  "SALOME_ComponentPy_i::Stop not yet implemented" )

    #-------------------------------------------------------------------------

    def Suspend(self):
        MESSAGE(  "SALOME_ComponentPy_i::Suspend not yet implemented" )

    #-------------------------------------------------------------------------

    def Resume(self):
        MESSAGE(  "SALOME_ComponentPy_i::Resume not yet implemented" )

