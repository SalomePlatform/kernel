#!/usr/bin/env python3
#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2025  CEA, EDF, OPEN CASCADE
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

def readORBConfigFile(filename):
  """ Extract information (host, port) from ORB configuration file. """
  with open(filename) as f:
    contents = f.readlines()

  import re
  host, port = None, None
  for line in contents:
    m = re.match("(ORB)?InitRef = NameService=corbaname::([\D\d]+):(\d*)", line)
    if m:
      host = m.group(2)
      port = m.group(3)
      break
    pass
  return host, port
#

def fillOrbConfigFileNoNS(prefix,orbdata):
  GIOP_MaxMsgSize = 2097152000  # 2 GBytes
  orbdata.append("%sgiopMaxMsgSize = %s # 2 GBytes"%(prefix,GIOP_MaxMsgSize))
  orbdata.append("%straceLevel = 0 # critical errors only"%(prefix))
  orbdata.append("%smaxGIOPConnectionPerServer = 500 # to allow containers parallel launch"%(prefix))
  orbdata.append("%snativeCharCodeSet = UTF-8"%(prefix))
  return GIOP_MaxMsgSize


def getPrefix():
  from omniORB import CORBA
  prefix = "" if CORBA.ORB_ID == "omniORB4" else "ORB"
  return prefix
  
def writeORBConfigFileSSL(path, kwargs={}):
  from salome.kernel.salome_utils import generateFileName
  omniorb_config = generateFileName(path, prefix="omniORB",
                                    extension="cfg",
                                    hidden=True,
                                    with_hostname=True,
                                    **kwargs)
  import os
  os.environ['OMNIORB_CONFIG'] = omniorb_config
  prefix = getPrefix()

  orbdata = []
  GIOP_MaxMsgSize = fillOrbConfigFileNoNS(prefix,orbdata)
  orbdata.append("")

  with open(omniorb_config, "w") as f:
    f.write("\n".join(orbdata))
    
  return [ omniorb_config, GIOP_MaxMsgSize ]

# IMPORTANT NOTE: do not add any print call (cf. note at the bottom of the file)
def writeORBConfigFile(path, host, port, kwargs={}):

  from salome.kernel.salome_utils import generateFileName
  omniorb_config = generateFileName(path, prefix="omniORB",
                                    extension="cfg",
                                    hidden=True,
                                    with_hostname=True,
                                    with_port=port,
                                    **kwargs)
  import os
  os.environ['OMNIORB_CONFIG'] = omniorb_config
  os.environ['NSPORT'] = "%s"%(port)
  os.environ['NSHOST'] = "%s"%(host)

  prefix = getPrefix()

  orbdata = []
  orbdata.append("%sInitRef = NameService=corbaname::%s:%s"%(prefix,host,port))
  GIOP_MaxMsgSize = fillOrbConfigFileNoNS(prefix,orbdata)

  orbdata.append("")

  with open(omniorb_config, "w") as f:
    f.write("\n".join(orbdata))

  return [ omniorb_config, GIOP_MaxMsgSize ]
