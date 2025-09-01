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
#  File   : SALOME_Container.py
#  Author : Paul RASCLE, EDF
#  Module : SALOME
#  $Header$

## @package SALOME_Container
# \brief python implementation of container interface for Kernel
#

import abc
import os
import sys
import traceback
import importlib
from omniORB import CORBA, PortableServer
from . import SALOMEDS
from . import Engines, Engines__POA
from .SALOME_NamingServicePy import *
from .SALOME_ComponentPy import *
from . import SALOME_PyNode
import logging

from .SALOME_utilities import *
from .Utils_Identity import getShortHostName
from salome.kernel.salome_utils import verbose
from .KernelBasis import VerbosityActivated,getSSLMode

#=============================================================================

#define an implementation of the container interface for embedding in Container implemented in C++

class SALOME_Container_Abstract_i(abc.ABC):
    _orb = None
    _poa = None
    _containerName = ""
    _naming_service = None

    #-------------------------------------------------------------------------

    def __init__(self, containerName, containerIORStr, dftTimeIntervalInMs):
        # Warning this part of code is called at the very first step of container launching
        # so logging is not instanciate. So use verbose method to discrimine if a message should be printed or not
        try:
          argv = sys.argv
        except AttributeError :
          # for embedded python interpreter
          # shouldn't be needed after python 3.8
          # see https://bugs.python.org/issue32573
          argv = ['']
        if VerbosityActivated():
          logging.debug("Instanciation of {} PID = {}".format(containerName,os.getpid()))
        self._orb = CORBA.ORB_init(argv, CORBA.ORB_ID)
        self._poa = self._orb.resolve_initial_references("RootPOA")
        self._containerName = containerName
        self._logFileName = None
        self._timeIntervalInMs = dftTimeIntervalInMs
        self._logm = None
        self._log = None
        self._container = self._orb.string_to_object(containerIORStr)

    @abc.abstractmethod
    def getPyScriptCls(self):
        raise RuntimeError("Must be overloaded")

    @property
    def logm(self):
        logging.debug("Logm PID = {}".format(os.getpid()))
        from salome.kernel import salome
        if self._logm is None:
           salome.salome_init()
           self._logm = salome.logm
        return self._logm

    #-------------------------------------------------------------------------

    def import_component(self, componentName):
        ret=""
        try:
            logging.debug("try import ",componentName)
            importlib.import_module(componentName)
            logging.debug("import ",componentName," successful")
        except ImportError:
            #can't import python module componentName
            #try to find it in python path
            try:
              _specs = importlib.util.find_spec(componentName)
              _module = importlib.util.module_from_spec(_specs)
              _specs.loader.exec_module(_module)
              #module file found in path
              ret="Component "+componentName+": Python implementation found but it can't be loaded\n"
              ret=ret+traceback.format_exc(10)
            except ImportError as ee:
              ret="ImplementationNotFound"
            except Exception:
              print("error when calling find_module")
              ret="ImplementationNotFound"
        except Exception:
            ret="Component "+componentName+": Python implementation found but it can't be loaded\n"
            ret=ret+traceback.format_exc(10)
            traceback.print_exc()
            print("import ",componentName," not possible")
        return ret

    #-------------------------------------------------------------------------

    def create_component_instance(self, componentName, instanceName):
        comp_iors=""
        ret=""
        try:
            component=importlib.import_module(componentName)
            factory=getattr(component,componentName)
            comp_i=factory(self._orb,
                           self._poa,
                           self._container,
                           self._containerName,
                           instanceName,
                           componentName)

            comp_o = comp_i._this()
            comp_iors = self._orb.object_to_string(comp_o)
        except Exception:
            ret=traceback.format_exc(10)
            traceback.print_exc()
        return comp_iors, ret


    def create_pynode(self,nodeName,code):
        try:
          node=SALOME_PyNode.PyNode_i(nodeName,code,self._poa,self)
          id_o = self._poa.activate_object(node)
          comp_o = self._poa.id_to_reference(id_o)
          comp_iors = self._orb.object_to_string(comp_o)
          return 0,comp_iors
        except Exception:
          exc_typ,exc_val,exc_fr=sys.exc_info()
          l=traceback.format_exception(exc_typ,exc_val,exc_fr)
          return 1,"".join(l)

    def create_pyscriptnode(self,nodeName,code):
        logging.debug("create_pyscriptnode of {} PID = {}".format(nodeName,os.getpid()))
        try:
          logscript = None
          if getSSLMode():
            logscript = self._log.addScript(nodeName,code)
          cls = self.getPyScriptCls()
          node = cls(nodeName,code,self._poa,self, logscript)
          id_o = self._poa.activate_object(node)
          comp_o = self._poa.id_to_reference(id_o)
          comp_iors = self._orb.object_to_string(comp_o)
          return 0,comp_iors
        except Exception:
          exc_typ,exc_val,exc_fr=sys.exc_info()
          l=traceback.format_exception(exc_typ,exc_val,exc_fr)
          print("".join(l)) ; sys.stdout.flush() # print error also in logs of remote container
          return 1,"".join(l)
        
    def positionVerbosityOfLogger(self):
        from . import salome_utils
        salome_utils.positionVerbosityOfLoggerRegardingState()
        logging.debug("positionVerbosityOfLogger PID = {}".format(os.getpid()))

    def setCurrentDirectory(self, newCwd):
        os.chdir( newCwd )
    
    def monitoringtimeresms(self):
        return self._timeIntervalInMs
    
    def shutdownPy(self):
        if getSSLMode():
           if self._log:
              #self._log.destroy()# TODO : choose to destroy perf report or not. For the moment we keep the report
              pass
           
    def killMe(self):
        import signal
        os.kill( os.getpid() , signal.SIGKILL)
           
    def setLogFileName(self, logFileName):
        logging.debug("setLogFileName {} PID = {}".format(logFileName,os.getpid()))
        if getSSLMode():
          self._log = self.logm.declareContainer( self._containerName, logFileName )

    def SetMonitoringtimeresms(self , value):
        self._timeIntervalInMs = value

class SALOME_Container_i(SALOME_Container_Abstract_i):
    def __init__(self, containerName, containerIORStr, dftTimeIntervalInMs):
      super().__init__(containerName, containerIORStr, dftTimeIntervalInMs)

    def getPyScriptCls(self):
      return SALOME_PyNode.PyScriptNode_i

class SALOME_Container_OutOfProcess_i(SALOME_Container_i):
    def __init__(self, containerName, containerIORStr, dftTimeIntervalInMs):
      super().__init__(containerName, containerIORStr, dftTimeIntervalInMs)
      
    def getPyScriptCls(self):
      return SALOME_PyNode.PyScriptNode_OutOfProcess_i

class SALOME_Container_OutOfProcess_Replay_i(SALOME_Container_i):
    def __init__(self, containerName, containerIORStr, dftTimeIntervalInMs):
      super().__init__(containerName, containerIORStr, dftTimeIntervalInMs)

    def getPyScriptCls(self):
      return SALOME_PyNode.PyScriptNode_OutOfProcess_Replay_i

class SALOME_Container_OutOfProcess_FT_i(SALOME_Container_i):
    def __init__(self, containerName, containerIORStr, dftTimeIntervalInMs):
      super().__init__(containerName, containerIORStr, dftTimeIntervalInMs)
      
    def getPyScriptCls(self):
      return SALOME_PyNode.PyScriptNode_OutOfProcess_FT_i

class SALOME_Container_OutOfProcess_Replay_FT_i(SALOME_Container_i):
    def __init__(self, containerName, containerIORStr, dftTimeIntervalInMs):
      super().__init__(containerName, containerIORStr, dftTimeIntervalInMs)

    def getPyScriptCls(self):
      return SALOME_PyNode.PyScriptNode_OutOfProcess_Replay_FT_i
