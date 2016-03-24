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

#  SALOME Container : implementation of container and engine for Kernel
#  File   : SALOME_Container.py
#  Author : Paul RASCLE, EDF
#  Module : SALOME
#  $Header$

## @package SALOME_Container
# \brief python implementation of container interface for Kernel
#

import os
import sys
import string
import traceback
import imp
from omniORB import CORBA, PortableServer
import SALOMEDS
import Engines, Engines__POA
from SALOME_NamingServicePy import *
from SALOME_ComponentPy import *
import SALOME_PyNode

from SALOME_utilities import *
from Utils_Identity import getShortHostName
from launchConfigureParser import verbose

#=============================================================================

#define an implementation of the container interface for embedding in Container implemented in C++

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
        ret=""
        try:
            if verbose(): print "try import ",componentName
            __import__(componentName)
            if verbose(): print "import ",componentName," successful"
        except ImportError,e:
            #can't import python module componentName
            #try to find it in python path
            try:
              fp, pathname, description = imp.find_module(componentName)
              if fp:fp.close()
              #module file found in path
              ret="Component "+componentName+": Python implementation found but it can't be loaded\n"
              ret=ret+traceback.format_exc(10)
            except ImportError,ee:
              ret="ImplementationNotFound"
            except:
              if verbose():print "error when calling find_module"
              ret="ImplementationNotFound"
        except:
            ret="Component "+componentName+": Python implementation found but it can't be loaded\n"
            ret=ret+traceback.format_exc(10)
            if verbose():
              traceback.print_exc()
              print "import ",componentName," not possible"
        return ret
        
    #-------------------------------------------------------------------------

    def create_component_instance(self, componentName, instanceName, studyId):
        MESSAGE( "SALOME_Container_i::create_component_instance" )
        comp_iors=""
        ret=""
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
            ret=traceback.format_exc(10)
            traceback.print_exc()
            MESSAGE( "SALOME_Container_i::create_component_instance : NOT OK")
        return comp_iors, ret
        

    def create_pynode(self,nodeName,code):
        try:
          node=SALOME_PyNode.PyNode_i(nodeName,code,self._poa,self)
          id_o = self._poa.activate_object(node)
          comp_o = self._poa.id_to_reference(id_o)
          comp_iors = self._orb.object_to_string(comp_o)
          return 0,comp_iors
        except:
          exc_typ,exc_val,exc_fr=sys.exc_info()
          l=traceback.format_exception(exc_typ,exc_val,exc_fr)
          return 1,"".join(l)

    def create_pyscriptnode(self,nodeName,code):
        try:
          node=SALOME_PyNode.PyScriptNode_i(nodeName,code,self._poa,self)
          id_o = self._poa.activate_object(node)
          comp_o = self._poa.id_to_reference(id_o)
          comp_iors = self._orb.object_to_string(comp_o)
          return 0,comp_iors
        except:
          exc_typ,exc_val,exc_fr=sys.exc_info()
          l=traceback.format_exception(exc_typ,exc_val,exc_fr)
          return 1,"".join(l)
