#!/usr/bin/env python3
#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2017  CEA/DEN, EDF R&D, OPEN CASCADE
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
import os
import sys

try:
  import cPickle as pickle #@UnusedImport
except:
  import pickle #@Reimport

__PORT_MIN_NUMBER = 2810
__PORT_MAX_NUMBER = 2910

import logging
def createLogger():
  logger = logging.getLogger(__name__)
  logger.setLevel(logging.DEBUG)
  #logger.setLevel(logging.INFO)
  ch = logging.StreamHandler()
  ch.setLevel(logging.DEBUG)
  formatter = logging.Formatter("%(levelname)s:%(threadName)s:%(pathname)s[%(lineno)s]%(message)s")
  ch.setFormatter(formatter)
  logger.addHandler(ch)
  return logger
#
logger = createLogger()

#------------------------------------
# A file locker
def __acquire_lock(lock):
  logger.debug("ACQUIRE LOCK")
  if sys.platform == "win32":
    import msvcrt
    # lock 1 byte: file is supposed to be zero-byte long
    msvcrt.locking(lock.fileno(), msvcrt.LK_LOCK, 1)
  else:
    import fcntl
    fcntl.flock(lock, fcntl.LOCK_EX)
  logger.debug("LOCK ACQUIRED")
#
def __release_lock(lock):
  logger.debug("RELEASE LOCK")
  if sys.platform == "win32":
    import msvcrt
    msvcrt.locking(lock.fileno(), msvcrt.LK_UNLCK, 1)
  else:
    import fcntl
    fcntl.flock(lock, fcntl.LOCK_UN)
  logger.debug("LOCK RELEASED")
#
#------------------------------------

def _getConfigurationFilename():
  omniorbUserPath = os.getenv("OMNIORB_USER_PATH")

  from salome_utils import generateFileName
  portmanager_config = generateFileName(omniorbUserPath,
                                        prefix="salome",
                                        suffix="PortManager",
                                        extension="cfg",
                                        hidden=True)
  import tempfile
  temp = tempfile.NamedTemporaryFile()
  lock_file = os.path.join(os.path.dirname(temp.name), ".salome_PortManager.lock")
  temp.close()

  return (portmanager_config, lock_file)
#

def __isPortUsed(port, config):
  busy_ports = []
  for ports in config.values():
    busy_ports += ports
  return (port in busy_ports) or __isNetworkConnectionActiveOnPort(port)
#

def __isNetworkConnectionActiveOnPort(port):
  # :NOTE: Under windows:
  #        netstat options -l and -t are unavailable
  #        grep command is unavailable
  if sys.platform == "win32":
    cmd = ['netstat','-a','-n','-p tcp']
  else:
    cmd = ['netstat','-ant']
    pass

  err = None
  try:
    from subprocess import Popen, PIPE, STDOUT
    p = Popen(cmd, stdout=PIPE, stderr=STDOUT)
    out, err = p.communicate()
  except:
    print("Error when trying to access active network connections.")
    if err: print(err)
    import traceback
    traceback.print_exc()
    return False

  from io import StringIO
  buf = StringIO(out.decode())
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
  return False
#

def getPort(preferedPort=None):
  logger.debug("GET PORT")

  config_file, lock_file = _getConfigurationFilename()
  oldmask = os.umask(0)
  with open(lock_file, 'wb') as lock:
    # acquire lock
    __acquire_lock(lock)

    # read config
    config = {}
    logger.debug("read configuration file")
    try:
      with open(config_file, 'rb') as f:
        config = pickle.load(f)
    except:
      logger.info("Problem loading PortManager file: %s"%config_file)
      # In this case config dictionary is reset

    logger.debug("load config: %s"%str(config))
    appli_path = os.getenv("ABSOLUTE_APPLI_PATH", "unknown")
    try:
        config[appli_path]
    except KeyError:
        config[appli_path] = []

    # append port
    port = preferedPort
    if not port or __isPortUsed(port, config):
      port = __PORT_MIN_NUMBER
      while __isPortUsed(port, config):
        if port == __PORT_MAX_NUMBER:
          msg  = "\n"
          msg += "Can't find a free port to launch omniNames\n"
          msg += "Try to kill the running servers and then launch SALOME again.\n"
          raise RuntimeError(msg)
        logger.debug("Port %s seems to be busy"%str(port))
        port = port + 1
    logger.debug("found free port: %s"%str(port))
    config[appli_path].append(port)

    # write config
    logger.debug("write config: %s"%str(config))
    try:
      with open(config_file, 'wb') as f:
        pickle.dump(config, f, protocol=0)
    except IOError:
      pass

    # release lock
    __release_lock(lock)
  #

  os.umask(oldmask)
  logger.debug("get port: %s"%str(port))
  return port
#

def releasePort(port):
  port = int(port)
  logger.debug("RELEASE PORT (%s)"%port)

  config_file, lock_file = _getConfigurationFilename()
  oldmask = os.umask(0)
  with open(lock_file, 'wb') as lock:
    # acquire lock
    __acquire_lock(lock)

    # read config
    config = {}
    logger.debug("read configuration file")
    try:
      with open(config_file, 'rb') as f:
        config = pickle.load(f)
    except IOError: # empty file
      pass

    logger.debug("load config: %s"%str(config))
    appli_path = os.getenv("ABSOLUTE_APPLI_PATH", "unknown")
    try:
        config[appli_path]
    except KeyError:
        config[appli_path] = []

    # remove port from list
    ports_info = config[appli_path]
    config[appli_path] = [x for x in ports_info if x != port]

    # write config
    logger.debug("write config: %s"%str(config))
    try:
      with open(config_file, 'wb') as f:
        pickle.dump(config, f, protocol=0)
    except IOError:
      pass

    # release lock
    __release_lock(lock)

    logger.debug("released port port: %s"%str(port))

  os.umask(oldmask)
#

def getBusyPorts():
  config_file, lock_file = _getConfigurationFilename()
  oldmask = os.umask(0)
  with open(lock_file, 'wb') as lock:
    # acquire lock
    __acquire_lock(lock)

    # read config
    config = {}
    logger.debug("read configuration file")
    try:
      with open(config_file, 'rb') as f:
        config = pickle.load(f)
    except IOError: # empty file
      pass

    logger.debug("load config: %s"%str(config))
    appli_path = os.getenv("ABSOLUTE_APPLI_PATH", "unknown")
    try:
        config[appli_path]
    except KeyError:
        config[appli_path] = []

    # Scan all possible ports to determine which ones are owned by other applications
    ports_info = { 'this': [], 'other': [] }
    my_busy_ports = config[appli_path]
    for port in range(__PORT_MIN_NUMBER, __PORT_MAX_NUMBER):
      if __isPortUsed(port, config):
        logger.debug("Port %s seems to be busy"%str(port))
        if port in my_busy_ports:
          ports_info["this"].append(port)
        else:
          ports_info["other"].append(port)

    logger.debug("all busy_ports: %s"%str(ports_info))

    sorted_ports = { 'this': sorted(ports_info['this']),
                     'other': sorted(ports_info['other']) }

    # release lock
    __release_lock(lock)

  os.umask(oldmask)
  return sorted_ports
#
