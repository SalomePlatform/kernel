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

from multiprocessing.managers import SyncManager
import multiprocessing
import time
import socket
import Queue
import sys
import os

"""
This class handles port distribution for SALOME sessions.
In case of concurent sessions, each will have its own port number.
"""
class PortManager(object): # :TODO: must manage lock owner
  __metaclass__ = Singleton
  #
  def __init__(self, startNumber = 2810, limit = 100, timeout=60):
    super(PortManager, self).__init__()
    self.__startNumber = startNumber
    self.__limit = startNumber + limit
    self.__lockedPorts = []
    self.__lock = multiprocessing.Lock()
    self.__timeout = timeout
    self.__lastChangeTime = time.time()
  #
  def getPort(self):
    with self.__lock:
      port = self.__startNumber
      while self.isPortUsed(port):
        if port == self.__limit:
          msg  = "\n"
          msg += "Can't find a free port to launch omniNames\n"
          msg += "Try to kill the running servers and then launch SALOME again.\n"
          raise RuntimeError, msg
        port = port + 1

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
      return "PortManager: list of locked ports:" + str(self.__lockedPorts)
  #
#

def __build_server(ip, port, authkey):
  message_queue = multiprocessing.Queue()

  class MyManager(SyncManager):
    pass

  MyManager.register("PortManager", PortManager, exposed=['getPort', 'releasePort', 'isBusy', 'isPortUsed', 'timeout', '__str__'])
  MyManager.register("get_message_queue", callable=lambda: message_queue)

  manager = MyManager(address=(ip, port), authkey=authkey)

  try:
    manager.get_server()
    manager.start()
  except (EOFError, socket.error):
    print 'Server already started on %s:%s'%(ip,port)
    sys.exit(1)

  return manager
#

def __build_client(ip, port, authkey):
  class MyManager(SyncManager):
    pass

  MyManager.register("PortManager")
  MyManager.register("get_message_queue")

  manager = MyManager(address=(ip, port), authkey=authkey)
  try:
    manager.connect()
  except socket.error:
    raise Exception("Unable to connect to server on %s:%s"%(ip, port))
  return manager
#

def __run_server(ip, port, authkey, timeout):
  theserver = __build_server(ip, port, authkey)
  shared_mesq = theserver.get_message_queue()
  print 'PortManager server started on %s:%s'%(ip,port)
  portManager = theserver.PortManager(timeout=timeout)

  while portManager.isBusy() or not portManager.timeout():
    try:
      message = shared_mesq.get(block=False)
      print message
    except Queue.Empty:
      pass

  print "PortManager server is shuting down..."
  time.sleep(2)
  theserver.shutdown()
#

#
semaphore = None
#
def __run_client(manager, execute_func):
  with semaphore:
    name = multiprocessing.current_process().name
    processId = os.getpid()

    # do the job
    if execute_func:
      execute_func(manager, name, processId)
#

#
local_ip = '127.0.0.1'
port = 5000
authkey = 'salome_port_manager_access'
#
lock = multiprocessing.Lock()
def start_server(nbSimultaneous=10, timeout=10):
  with lock:
    procServer = multiprocessing.Process(target=__run_server, args=(local_ip,port,authkey,timeout,))
    procServer.start()
    global semaphore
    semaphore = multiprocessing.Semaphore(nbSimultaneous)
    time.sleep(2)
#

def start_client(ip=local_ip, execute_func=None, name="anonymous"):
  manager = __build_client(ip, port, authkey)
  p = multiprocessing.Process(target=__run_client, name=name, args=(manager,execute_func,))
  p.start()
  return manager
#

client_id = 0
def start_clients(nbClients, ip, execute_func, name_prefix="Client"):
  global client_id
  for i in range(nbClients):
    start_client(ip, execute_func, name=name_prefix+"_"+str(client_id))
    client_id = client_id + 1
#
