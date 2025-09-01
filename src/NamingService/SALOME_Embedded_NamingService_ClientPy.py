#! /usr/bin/env python3
#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2021-2025  CEA, EDF
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

from omniORB import CORBA

class SALOME_Embedded_NamingService_ClientPy:
  """
    A python client class to handle remote servant of SALOME_Embedded_NamingService
  """
  #-------------------------------------------------------------------------

  def __init__(self, serv):
    self._orb = CORBA.ORB_init([''], CORBA.ORB_ID)
    self._obj = serv
  #-------------------------------------------------------------------------

  def Register(self,ObjRef, Path):
    ior = bytes(self._orb.object_to_string(ObjRef),encoding="utf-8")
    self._obj.Register(ior,Path)

  #-------------------------------------------------------------------------

  def Resolve(self, Path):
      ret = self._obj.Resolve(Path)
      return self._orb.string_to_object(ret.decode())
  
  def keys(self):
      return self._obj.keys()
  
  def repr(self):
      return self.keys()
  
def SALOME_Embedded_NamingService_ClientPy_BuildFromIORFile(cls, iorNSFile):
   from . import Engines
   orb = CORBA.ORB_init([''], CORBA.ORB_ID)
   with open(iorNSFile,"r") as f:
    ior = f.read()
   serv = orb.string_to_object( ior )
   return SALOME_Embedded_NamingService_ClientPy(serv)

SALOME_Embedded_NamingService_ClientPy.BuildFromIORFile = classmethod(SALOME_Embedded_NamingService_ClientPy_BuildFromIORFile)
