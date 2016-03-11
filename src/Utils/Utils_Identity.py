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

#  SALOME Utils : general SALOME's definitions and tools
#  File   : Utils_Identity.py
#  Author : Estelle Deville, CEA
#  Module : SALOME
#  $Header$
## @package Utils_Identity
# \brief Module to get information about user and version
#
import sys
import os
import socket

if not sys.platform == "win32":
    import pwd

import time
import string

def getShortHostName():
    """
    gives Hostname without domain extension.

    SALOME naming service needs short Hostnames (without domain extension).
    HOSTNAME is not allways defined in environment,
    socket.gethostname() gives short or complete Hostname, depending on
    defined aliases.
    """
    return string.split(socket.gethostname(),'.')[0]

class Identity:
    def __init__(self,name):
        self._name = name
        self._pid =  os.getpid()
        self._machine = socket.gethostname()
        self._adip =  socket.gethostbyname(self._machine) # IP adress        
        if sys.platform == "win32":
          self._uid  = os.getpid() 
          self._pwname = os.environ["USERNAME"]
        else:
          self._uid = os.getuid()
          list = pwd.getpwuid(self._uid)
          self._pwname  = list[0] # user name

        self._tc_start = time.time()
        self._cstart    = time.ctime(self._tc_start)
        self._cdir = os.getcwd()

def getapplipath():
    """
      Gives short application path (the complete path is $HOME/$APPLI)
    """
    return os.environ.get("APPLI",".salome_"+versnb)

try:
  file = open(os.path.join(os.environ["KERNEL_ROOT_DIR"],"bin","salome","VERSION"), "r")
  s = file.readline()
  versnb = string.strip(string.split(s, ":")[1])
  dirname=".salome_"+versnb
  file.close()
except:
  versnb = ""
  dirname=".salome"

def version():
    """
      Gives salome version number
    """
    return versnb
