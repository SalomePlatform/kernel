#! /usr/bin/env python
#
#  SALOME Utils : general SALOME's definitions and tools
#
#  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
#  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
#
#
#
#  File   : Utils_Identity.py
#  Author : Estelle Deville, CEA
#  Module : SALOME
#  $Header$

import sys
import os
import socket
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
        self._adip	=  socket.gethostbyname(self._machine) # IP adress
        self._uid	= os.getuid() 
        list = pwd.getpwuid(self._uid)
        self._pwname	= list[0] # user name
        self._tc_start	= time.time()
        self._cstart    = time.ctime(self._tc_start)
        self._cdir	= os.getcwd()
