#!/usr/bin/env python
#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
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
from Singleton import Singleton

import multiprocessing
import time
import socket

import sys
import threading
import SocketServer

try:
  import cPickle as pickle
except:
  import pickle

import struct
import ctypes

if sys.platform == 'win32':
  import multiprocessing.reduction    # make sockets pickable/inheritable

multiprocessing.freeze_support() # Add support for when a program which uses multiprocessing has been frozen to produce a Windows executable.

"""
This class handles port distribution for SALOME sessions.
In case of concurent sessions, each will have its own port number.
"""
class _PortManager(object): # :TODO: must manage lock owner
  __metaclass__ = Singleton
  #
  def __init__(self, startNumber = 2810, limit = 100, timeout=60):
    super(_PortManager, self).__init__()
    self.__startNumber = startNumber
    self.__limit = startNumber + limit
    self.__lockedPorts = []
    self.__lock = multiprocessing.Lock()
    self.__timeout = timeout
    self.__lastChangeTime = time.time()
  #
  # Test for prefered port number, if asked.
  def getPort(self, port=None):
    with self.__lock:
      if not port or self.isPortUsed(port):
        port = self.__startNumber
        while self.isPortUsed(port):
          if port == self.__limit:
            msg  = "\n"
            msg += "Can't find a free port to launch omniNames\n"
            msg += "Try to kill the running servers and then launch SALOME again.\n"
            raise RuntimeError, msg
          port = port + 1
      #
      self.__lockedPorts.append(port)
      self.__lastChangeTime = time.time()
      return port
  #
  def releasePort(self, port):
    with self.__lock:
      if port in self.__lockedPorts:
        self.__lockedPorts.remove(port)
        self.__lastChangeTime = time.time()
  #
  def isBusy(self):
    return len(self.__lockedPorts)
  #
  def isPortUsed(self, port):
    return (port in self.__lockedPorts) or self.__isNetworkConnectionActiveOnPort(port)
  #
  def __isNetworkConnectionActiveOnPort(self, port):
    # :NOTE: Under windows:
    #        netstat options -l and -t are unavailable
    #        grep command is unavailable
    from subprocess import Popen, PIPE
    (stdout, stderr) = Popen(['netstat','-an'], stdout=PIPE).communicate()
    import StringIO
    buf = StringIO.StringIO(stdout)
    ports = buf.readlines()
    # search for TCP - LISTEN connections
    import re
    regObj = re.compile( ".*tcp.*:([0-9]+).*:.*listen", re.IGNORECASE );
    for item in ports:
      try:
        p = int(regObj.match(item).group(1))
        if p == port: return True
      except:
        pass
  #
  def timeout(self):
    return (time.time() - self.__lastChangeTime > self.__timeout)
  #
  def __str__(self):
    with self.__lock:
      return "_PortManager: list of locked ports:" + str(sorted(self.__lockedPorts))
  #
#

#------------------------------------
# Communication methods

_marshall = pickle.dumps
_unmarshall = pickle.loads

def _send(channel, *args):
  buf = _marshall(args)
  value = socket.htonl(len(buf))
  size = struct.pack("L",value)
  channel.send(size)
  channel.send(buf)
#

def _receive(channel):
  size = struct.calcsize("L")
  size = channel.recv(size)
  try:
    size = socket.ntohl(struct.unpack("L", size)[0])
  except struct.error, e:
    return ''

  buf = ""
  while len(buf) < size:
    buf = channel.recv(size - len(buf))

  return _unmarshall(buf)[0]
#
#------------------------------------

GET_PORT_MSG = "GET_PORT"
GET_PREFERED_PORT_MSG = "GET_PREFERED_PORT"
RELEASE_PORT_MSG = "RELEASE_PORT"
STOP_SERVER_MSG = "STOP_SERVER"

GET_PORT_ACK_MSG = "GET_PORT"
RELEASE_PORT_ACK_MSG = "RELEASE_PORT"

class _ThreadedTCPRequestHandler(SocketServer.BaseRequestHandler):
  def handle(self):
    data = _receive(self.request)
    if data == GET_PORT_MSG:
      pm = _PortManager()
      port = pm.getPort()
      response = "%s: %s" % (GET_PORT_ACK_MSG, port)
      _send(self.request, response)
    elif data.startswith(GET_PREFERED_PORT_MSG):
      port = int(data[len(GET_PREFERED_PORT_MSG)+1:])
      pm = _PortManager()
      port = pm.getPort(port)
      response = "%s: %s" % (GET_PORT_ACK_MSG, port)
      _send(self.request, response)
    elif data.startswith(RELEASE_PORT_MSG):
      port = int(data[len(RELEASE_PORT_MSG)+1:])
      pm = _PortManager()
      pm.releasePort(port)
      response = "%s" % (RELEASE_PORT_ACK_MSG)
      _send(self.request, response)
      #print "RELEASE_PORT:", port
      if not pm.isBusy():
        #print "Close server"
        self.server.shutdown()
      #print pm
    elif data == STOP_SERVER_MSG:
      #print "Close server"
      self.server.shutdown()
#

class _ThreadedTCPServer(SocketServer.ThreadingMixIn, SocketServer.TCPServer):
    pass

pm_address = ('127.0.0.1', 51843)
def __getServer(address):
  SocketServer.ThreadingTCPServer.allow_reuse_address = True # can be restarted immediately
  server = _ThreadedTCPServer(address, _ThreadedTCPRequestHandler, False) # Do not automatically bind
  server.allow_reuse_address = True # Prevent 'cannot bind to address' errors on restart
  return server
#

def __startServer():
  global pm_address
  try:
    server = __getServer(pm_address)
    server.server_bind()     # Manually bind, to support allow_reuse_address
    server.server_activate()
    pm_address = server.server_address

    # Start a thread with the server -- that thread will then start one
    # more thread for each request
    server_thread = threading.Thread(target=server.serve_forever, name="SALOME_PortManager")
    # Exit the server thread when the main thread terminates
    #server_thread.setDaemon(True)
    server_thread.start()
    #print "Server loop running in thread:", server_thread.getName()
    #print "Server address:", pm_address
    #return address
  except:
    #print "Server already started"
    #print "Server address:", pm_address
    #return pm_address
    pass
#

def __newClient(address, message):
  try:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    #print "connect client to", address
    sock.connect(address)
    _send(sock, message)
    response = _receive(sock)
    if response.startswith(GET_PORT_ACK_MSG):
      port = int(response[len(GET_PORT_ACK_MSG)+1:])
      #print "GET_PORT:", port
      return port
    elif response == RELEASE_PORT_ACK_MSG:
      #print "Received: %s" % response
      return 0
      pass
    sock.close()
  except socket.error:
    #print "Unable to connect to server"
    return -1
#

def getPort(preferedPort=None):
  if preferedPort:
    return __newClient(pm_address, "%s: %s"%(GET_PREFERED_PORT_MSG,preferedPort))
  else:
    return __newClient(pm_address, GET_PORT_MSG)
#

def releasePort(port):
  __newClient(pm_address, "%s: %s"%(RELEASE_PORT_MSG,port))
#

def stopServer():
  __newClient(pm_address, STOP_SERVER_MSG)
#

# Auto start: unique instance ; no effect if called multiple times
__startServer()
