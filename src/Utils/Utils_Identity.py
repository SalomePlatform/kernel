#! /usr/bin/env python

#==============================================================================
#  File      : Utils_Identity.py
#  Created   : thur 02 May 2002
#  Author    : Estelle Deville, CEA
#  Project   : SALOME
#  Copyright : CEA 2002
#  $Header$
#==============================================================================

import sys
import os
import socket
import pwd
import time

class Identity:
    def __init__(self,name):
        self._name = name
        self._pid =  os.getpid()
        self._machine = os.getenv( "HOSTNAME" )
        self._adip	=  socket.gethostbyname(self._machine) # IP adress
        self._uid	= os.getuid() 
        list = pwd.getpwuid(self._uid)
        self._pwname	= list[0] # user name
        self._tc_start	= time.time()
        self._cstart    = time.ctime(self._tc_start)
        self._cdir	= os.getcwd()
