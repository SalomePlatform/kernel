#! /usr/bin/env python
#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
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

#  SALOME DataContainer : implementation of data container
#  File   : SALOME_DataContainerPy.py
#  Author : Mikhail PONIKARIOV
#  Module : SALOME
#  $Header$
#
import os
import sys
import string

from omniORB import CORBA, PortableServer
import Engines, Engines__POA
from SALOME_ComponentPy import *

#=============================================================================

#define an implementation of the data container interface for the data transfer implemented in Python

class SALOME_DataContainerPy_i (Engines__POA.DataContainer):
    _url = ""
    _name = ""
    _identifier = ""
    _ext = -1
    _removeAfterGet = True;

    #-------------------------------------------------------------------------

    def __init__(self, urlorstream, name, identifier, removeAfterGet, isStream = False):
        self._urlorstream = urlorstream
        self._name = name
        self._identifier = identifier
        self._removeAfterGet = removeAfterGet
        self._isStream = isStream
        if isStream:
          self._ext = ""
        else:
          self._ext = urlorstream[urlorstream.rfind(".") + 1 : ]

    #-------------------------------------------------------------------------

    def get(self):
      if self._isStream:
        return self._urlorstream

      f = open(self._urlorstream, 'r')
      stream = f.read()
      f.close()
      if self._removeAfterGet:
        os.remove(self._urlorstream)
        try: # try to remove directory if it is empty
          index = max(self._urlorstream.rfind("\\"), self._url.rfind("/"))
          if index > 0:
            os.rmdir(self._urlorstream[:index])
        except:
          pass
      return stream

    #-------------------------------------------------------------------------
    
    def name(self):
	return self._name

    #-------------------------------------------------------------------------

    def identifier(self):
	return self._identifier

    def extension(self):
	return self._ext

    def setExtension(self, ext):
	self._ext = ext
