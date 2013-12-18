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

import os
import sys
import threading
import SocketServer

try:
  import cPickle as pickle
except:
  import pickle

import struct
import ctypes

import logging
def createLogger():
  logger = logging.getLogger(__name__)
#  logger.setLevel(logging.DEBUG)
  ch = logging.StreamHandler()
  ch.setLevel(logging.DEBUG)
  formatter = logging.Formatter("%(levelname)s:%(threadName)s:%(message)s")
  ch.setFormatter(formatter)
  logger.addHandler(ch)
  return logger
#
logger = createLogger()


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
    logger.debug("PortManager.releasePort %s"%port)
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
TEST_SERVER_MSG = "TEST_SERVER"

GET_PORT_ACK_MSG = "GET_PORT"
RELEASE_PORT_ACK_MSG = "RELEASE_PORT"
TEST_SERVER_ACK_MSG = "TEST_SERVER"

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
      logger.debug("RELEASE_PORT: %s"%port)
      if not pm.isBusy():
        logger.debug("Close server")
        config_file, lock_file = _getConfigurationFilename()
        try:
          os.remove(config_file)
          pmlock.release()
          os.remove(lock_file)
        except:
          pass
        self.server.shutdown()
      #print pm
    elif data == STOP_SERVER_MSG:
      logger.debug("Close server")
      self.server.shutdown()
    elif data == TEST_SERVER_MSG:
      _send(self.request, TEST_SERVER_ACK_MSG)
#

class _ThreadedTCPServer(SocketServer.ThreadingMixIn, SocketServer.TCPServer):
    pass

#------------------------------------
# A file locker (Linux only)
import fcntl
class PortManagerLock:
  def __init__(self, filename, readonly=False, blocking=True):
    self.filename = filename
    # This will create it if it does not exist already
    logger.debug("Create lock on %s"%filename)
    mode = 'w'
    if readonly:
      mode = 'r'
    self.handle = open(filename, mode)
    self.handle.seek(0) # go back to beginning of file to read it multiple times
    self.__blocking = blocking

  def acquire(self):
    mode = fcntl.LOCK_EX
    if not self.__blocking: # Raise an IOError exception if file has already been locked
      mode = mode | fcntl.LOCK_NB
    fcntl.flock(self.handle, mode)
    logger.debug("lock acquired %s"%self.__blocking)

  def release(self):
    fcntl.flock(self.handle, fcntl.LOCK_UN)
    logger.debug("lock released")

  def __del__(self):
    if logger:
      logger.debug("Close lock file")
    self.handle.close()
#
#------------------------------------

# Server address has to be shared by different processes, without any common
# ancestor.
# The "simplest" solution is to define it here as a global variable. Unfortunately,
# availability of the corresponding socket is not guaranted at all. If PortManager
# tries to use a socket it does not own, server is not created (it is identified as
# already existing), clients then connect on this socket but message passing
# between clients and server will not work and SALOME launch will crash.
# We can introduce a port verification procedure automatically called by importing
# this module (i.e. when creating the server). This procedure consists in creating
# a client which sends a specific message to the server that has to be tested. And
# loop on port numbers until a free socket is found and associated to a new server.
#
# Another approach is to let Python socket API select a free port number, then store
# it to a file on server host machine in order to be shared with clients.
# The logical part can be defined as follows. When server is started (by importing
# this module), write server port number to a specific file (do not use a temporary
# file name). Each client then read server address from this same file ; if file is
# not nound, it is an error (add appropriate processing).
# Server could also check file existence and try to use the same address as previous
# server in order to avoid opening too many unecessary sockets ; but we need to apply
# the above verification procedure. This processing is not necessary because TCP socket
# timeout will automatically close unused sockets.

def _getConfigurationFilename():
  omniorbUserPath = os.getenv("OMNIORB_USER_PATH")

  from salome_utils import generateFileName
  portmanager_config = generateFileName(omniorbUserPath,
                                        prefix="omniORB",
                                        suffix="PortManager",
                                        extension="cfg",
                                        hidden=True)
  lock_file = portmanager_config + "-lock"
  return (portmanager_config, lock_file)
#

def __checkServer():
  while True:
    logger.debug("CHECKING SERVER")
    status = __newClient(TEST_SERVER_MSG)
    if status == TEST_SERVER_ACK_MSG:
      break
  return (status == TEST_SERVER_ACK_MSG)
#

def __getServerAddress(readonly=True):
  address = ("localhost", 0)
  try:
    config_file, lock_file = _getConfigurationFilename()
    lock = PortManagerLock(config_file, readonly, blocking=True)
    lock.acquire()
    address = eval(lock.handle.read())
    lock.release()
  except (IOError, SyntaxError) as e:
    logger.debug("no configuration file")
    pass
  finally:
    return address
#

def __setServerAddress(address):
  config_file, lock_file = _getConfigurationFilename()
  lock = PortManagerLock(config_file, readonly=False, blocking=True)
  lock.acquire()
  logger.debug("setServerAddress: %s"%str(address))
  lock.handle.write(str(address))
  lock.release()
#

def __getServer():
  address = __getServerAddress(readonly=False)
  SocketServer.ThreadingTCPServer.allow_reuse_address = True # can be restarted immediately
  server = _ThreadedTCPServer(address, _ThreadedTCPRequestHandler, False) # Do not automatically bind
  server.allow_reuse_address = True # Prevent 'cannot bind to address' errors on restart
  server.server_bind()     # Manually bind, to support allow_reuse_address
  __setServerAddress(server.server_address)
  server.server_activate()
  return server
#

pmlock = None
def __startServer():
  try:
    config_file, lock_file = _getConfigurationFilename()
    global pmlock
    pmlock = PortManagerLock(lock_file, readonly=False, blocking=False)
    pmlock.acquire()

    server = __getServer()
    # Start a thread with the server -- that thread will then start one
    # more thread for each request
    server_thread = threading.Thread(target=server.serve_forever, name="SALOME_PortManager")
    # Exit the server thread when the main thread terminates
    #server_thread.setDaemon(True)
    server_thread.start()
    #print "Server loop running in thread:", server_thread.getName()
  except:
    logger.debug("Server already started")
    pass
#

def __newClient(message):
  address = __getServerAddress(readonly=True)
  try:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    logger.debug("connect client to %s"%str(address))
    sock.connect(address)
    _send(sock, message)
    response = _receive(sock)
    if response.startswith(GET_PORT_ACK_MSG):
      port = int(response[len(GET_PORT_ACK_MSG)+1:])
      logger.debug("GET_PORT: %s"%port)
      return port
    elif response == RELEASE_PORT_ACK_MSG:
      logger.debug("Received: %s" % response)
      return 0
      pass
    elif response == TEST_SERVER_ACK_MSG:
      logger.debug("Server is ok")
      return TEST_SERVER_ACK_MSG
      pass
    sock.close()
  except socket.error:
    logger.debug("Unable to connect to server")
    return -1
#

def getPort(preferedPort=None):
  if preferedPort:
    return __newClient("%s: %s"%(GET_PREFERED_PORT_MSG,preferedPort))
  else:
    return __newClient(GET_PORT_MSG)
#

def releasePort(port):
  logger.debug("application asks for releasePort %s"%port)
  __newClient("%s: %s"%(RELEASE_PORT_MSG,port))
#

def stopServer():
  __newClient(STOP_SERVER_MSG)
#

# Auto start: unique instance ; no effect if called multiple times
__startServer()
logger.debug("Server started... do check...")
assert(__checkServer())
