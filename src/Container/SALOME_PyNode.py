#! /usr/bin/env python
#  -*- coding: iso-8859-1 -*-
#  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
#
#  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
#  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
#  File   : SALOME_PyNode.py
#  Author : Christian CAREMOLI, EDF
#  Module : SALOME
#  $Header$
#

import sys,traceback,string
import linecache
import cPickle
import Engines__POA
import SALOME__POA
import SALOME

class Generic(SALOME__POA.GenericObj):
  """A Python implementation of the GenericObj CORBA IDL"""
  def __init__(self,poa):
    self.poa=poa
    self.cnt=1

  def Register(self):
    self.cnt+=1

  def Destroy(self):
    self.cnt-=1
    if self.cnt <= 0:
      oid=self.poa.servant_to_id(self)
      self.poa.deactivate_object(oid)

class PyNode_i (Engines__POA.PyNode,Generic):
  """The implementation of the PyNode CORBA IDL"""
  def __init__(self, nodeName,code,poa):
    """Initialize the node : compilation in the local context"""
    Generic.__init__(self,poa)
    self.nodeName=nodeName
    self.code=code
    linecache.cache[nodeName]=0,None,string.split(code,'\n'),nodeName
    ccode=compile(code,nodeName,'exec')
    self.context={}
    exec ccode in self.context

  def execute(self,funcName,argsin): 
    """Execute the function funcName found in local context with pickled args (argsin)"""
    try:
      argsin,kws=cPickle.loads(argsin)
      func=self.context[funcName]
      argsout=func(*argsin,**kws)
      argsout=cPickle.dumps(argsout,-1)
      return argsout
    except:
      exc_typ,exc_val,exc_fr=sys.exc_info()
      l=traceback.format_exception(exc_typ,exc_val,exc_fr)
      raise SALOME.SALOME_Exception(SALOME.ExceptionStruct(SALOME.BAD_PARAM,"".join(l),"PyNode: %s, function: %s" % (self.nodeName,funcName),0))

