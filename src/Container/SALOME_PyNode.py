#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2019  CEA/DEN, EDF R&D, OPEN CASCADE
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

#  File   : SALOME_PyNode.py
#  Author : Christian CAREMOLI, EDF
#  Module : SALOME
#  $Header$
#
import sys,traceback
import linecache
import pickle
import Engines__POA
import SALOME__POA
import SALOME

class Generic(SALOME__POA.GenericObj):
  """A Python implementation of the GenericObj CORBA IDL"""
  def __init__(self,poa):
    self.poa=poa
    self.cnt=1

  def Register(self):
    print("Register called : %d"%self.cnt)
    self.cnt+=1

  def UnRegister(self):
    print("UnRegister called : %d"%self.cnt)
    self.cnt-=1
    if self.cnt <= 0:
      oid=self.poa.servant_to_id(self)
      self.poa.deactivate_object(oid)

  def Destroy(self):
    print("WARNING SALOME::GenericObj::Destroy() function is obsolete! Use UnRegister() instead.")
    self.UnRegister()

  def __del__(self):
    print("Destuctor called")

class PyNode_i (Engines__POA.PyNode,Generic):
  """The implementation of the PyNode CORBA IDL"""
  def __init__(self, nodeName,code,poa,my_container):
    """Initialize the node : compilation in the local context"""
    Generic.__init__(self,poa)
    self.nodeName=nodeName
    self.code=code
    self.my_container=my_container._container
    linecache.cache[nodeName]=0,None,code.split('\n'),nodeName
    ccode=compile(code,nodeName,'exec')
    self.context={}
    self.context["my_container"] = self.my_container
    exec(ccode, self.context)

  def defineNewCustomVar(self,varName,valueOfVar):
    self.context[varName] = pickle.loads(valueOfVar)
    pass

  def executeAnotherPieceOfCode(self,code):
    """Called for initialization of container lodging self."""
    try:
      ccode=compile(code,self.nodeName,'exec')
      exec(ccode, self.context)
    except:
      raise SALOME.SALOME_Exception(SALOME.ExceptionStruct(SALOME.BAD_PARAM,"","PyScriptNode (%s) : code to be executed \"%s\"" %(self.nodeName,code),0))

  def execute(self,funcName,argsin):
    """Execute the function funcName found in local context with pickled args (argsin)"""
    try:
      argsin,kws=pickle.loads(argsin)
      func=self.context[funcName]
      argsout=func(*argsin,**kws)
      argsout=pickle.dumps(argsout,-1)
      return argsout
    except:
      exc_typ,exc_val,exc_fr=sys.exc_info()
      l=traceback.format_exception(exc_typ,exc_val,exc_fr)
      raise SALOME.SALOME_Exception(SALOME.ExceptionStruct(SALOME.BAD_PARAM,"".join(l),"PyNode: %s, function: %s" % (self.nodeName,funcName),0))

class PyScriptNode_i (Engines__POA.PyScriptNode,Generic):
  """The implementation of the PyScriptNode CORBA IDL that executes a script"""
  def __init__(self, nodeName,code,poa,my_container):
    """Initialize the node : compilation in the local context"""
    Generic.__init__(self,poa)
    self.nodeName=nodeName
    self.code=code
    self.my_container=my_container._container
    linecache.cache[nodeName]=0,None,code.split('\n'),nodeName
    self.ccode=compile(code,nodeName,'exec')
    self.context={}
    self.context["my_container"] = self.my_container

  def defineNewCustomVar(self,varName,valueOfVar):
    self.context[varName] = pickle.loads(valueOfVar)
    pass

  def executeAnotherPieceOfCode(self,code):
    """Called for initialization of container lodging self."""
    try:
      ccode=compile(code,self.nodeName,'exec')
      exec(ccode, self.context)
    except:
      raise SALOME.SALOME_Exception(SALOME.ExceptionStruct(SALOME.BAD_PARAM,"","PyScriptNode (%s) : code to be executed \"%s\"" %(self.nodeName,code),0))

  def assignNewCompiledCode(self,codeStr):
    try:
      self.code=codeStr
      self.ccode=compile(codeStr,self.nodeName,'exec')
    except:
      raise SALOME.SALOME_Exception(SALOME.ExceptionStruct(SALOME.BAD_PARAM,"","PyScriptNode.assignNewCompiledCode (%s) : code to be executed \"%s\"" %(self.nodeName,codeStr),0))

  def execute(self,outargsname,argsin):
    """Execute the script stored in attribute ccode with pickled args (argsin)"""
    try:
      argsname,kws=pickle.loads(argsin)
      self.context.update(kws)
      exec(self.ccode, self.context)
      argsout=[]
      for arg in outargsname:
        if arg not in self.context:
          raise KeyError("There is no variable %s in context" % arg)
        argsout.append(self.context[arg])
      argsout=pickle.dumps(tuple(argsout),-1)
      return argsout
    except:
      exc_typ,exc_val,exc_fr=sys.exc_info()
      l=traceback.format_exception(exc_typ,exc_val,exc_fr)
      raise SALOME.SALOME_Exception(SALOME.ExceptionStruct(SALOME.BAD_PARAM,"".join(l),"PyScriptNode: %s, outargsname: %s" % (self.nodeName,outargsname),0))

  def getValueOfVarInContext(self,varName):
    try:
      return pickle.dumps(self.context[varName],-1)
    except:
      exc_typ,exc_val,exc_fr=sys.exc_info()
      l=traceback.format_exception(exc_typ,exc_val,exc_fr)
      raise SALOME.SALOME_Exception(SALOME.ExceptionStruct(SALOME.BAD_PARAM,"".join(l),"PyScriptNode: %s" %self.nodeName,0))
