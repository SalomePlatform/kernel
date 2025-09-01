#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2024-2025  CEA, EDF
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

import os
import sys
import pickle
from .SALOME_ContainerHelper import ScriptExecInfo

class SALOME_ContainerScriptExecPerfLog:
  def __init__(self, father):
    self._father = father
    self._start_pos = None
    self._stop_pos = None
    
  @property
  def father(self):
    return self._father

  def end(self,s):
    obj = pickle.loads(s)
    self._stop_pos = os.path.getsize( self.father.father.logfilename )
    setattr(obj,"tracePosStop",self._stop_pos)
    setattr(obj,"tracePosStart",self._start_pos)
    return pickle.dumps(obj)
  
  def flushAndAppendFreestyle(self,alreadyOnSiteBytes,instanceRemoteBytes):
    """
    :param alreadyOnSiteBytes: pickle of instance of ScriptExecInfo of previous value (if any) [bytes]
    :param instanceRemoteBytes: pickle of current instance of ScriptExecInfo (if any) [bytes]
    """
    instanceRemote = pickle.loads(instanceRemoteBytes)
    alreadyOnSite = None
    if len( alreadyOnSiteBytes ) > 0:
      alreadyOnSite = pickle.loads(alreadyOnSiteBytes)
    self._stop_pos = os.path.getsize( self.father.father.logfilename )
    if alreadyOnSite:
      setattr(alreadyOnSite,"tracePosStop",self._stop_pos)
      setattr(alreadyOnSite,"tracePosStart",self._start_pos)
      alreadyOnSite.appendFreestyle( instanceRemote._freestyle_log )
      return pickle.dumps(alreadyOnSite)
    else:
      setattr(instanceRemote,"tracePosStop",self._stop_pos)
      setattr(instanceRemote,"tracePosStart",self._start_pos)
      return pickle.dumps(instanceRemote)

  def start(self):
    self._start_pos = os.path.getsize( self.father.father.logfilename )

class SALOME_ContainerScriptPerfLog:
  def __init__(self, father):
    self._father = father

  @property
  def father(self):
    return self._father

  def addExecution(self):
    return SALOME_ContainerScriptExecPerfLog(self)

class SALOME_ContainerPerfLog:
  def __init__(self,contInNS,logfile):
    self._container_in_ns = contInNS
    self._log_filename = logfile
    
  @property
  def logfilename(self):
    return self._log_filename
    
  @property
  def father(self):
    return self._father

  def addScript(self):
    return SALOME_ContainerScriptPerfLog(self)

class SALOME_LogManagerHelper:
  def __init__(self):
    pass
  
  def declareContainer(self, contInNS,logfile):
    inst = SALOME_ContainerPerfLog(contInNS,logfile)
    return inst
  
  def putStructInFileAtomic(self,s,fileName):
    with open(fileName,"wb") as f:
      f.write( s )
