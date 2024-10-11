# -*- coding: utf-8 -*-
# Copyright (C) 2024  CEA, EDF
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

import Engines__POA

class SALOME_GlobalsImpl(Engines__POA.GlobalsHolder):
  def __init__(self):
    self._map = {}
  
  def setAttr(self, ctx, glbsAttachedToCtx):
    self._map[ctx] = glbsAttachedToCtx

  def getAttr(self, ctx):
    return self._map[ctx]
  
  def execute(self, args):
    import subprocess as sp
    proc = sp.Popen(args,stdout = sp.PIPE, stderr = sp.PIPE)
    stdout, stderr = proc.communicate()
    returncode = proc.returncode
    return returncode, stdout, stderr
