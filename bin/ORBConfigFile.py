#!/usr/bin/env python
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

# IMPORTANT NOTE: do not add any print call (cf. note at the bottom of the file)
def writeORBConfigFile(path, host, port, kwargs={}):

  from salome_utils import generateFileName
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

  from omniORB import CORBA
  prefix = "" if CORBA.ORB_ID == "omniORB4" else "ORB"

  GIOP_MaxMsgSize = 2097152000  # 2 GBytes

  orbdata = []
  orbdata.append("%sInitRef = NameService=corbaname::%s:%s"%(prefix,host,port))
  orbdata.append("%sgiopMaxMsgSize = %s # 2 GBytes"%(prefix,GIOP_MaxMsgSize))
  orbdata.append("%straceLevel = 0 # critical errors only"%(prefix))
  orbdata.append("%smaxGIOPConnectionPerServer = 500 # to allow containers parallel launch"%(prefix))
  orbdata.append("")

  with open(omniorb_config, "w") as f:
    f.write("\n".join(orbdata))

  return [ omniorb_config, GIOP_MaxMsgSize ]

# -----------------------------------------------------------------------------

if __name__ == "__main__":
  import sys

  path = sys.argv[1]
  host = sys.argv[2]
  port = sys.argv[3]
  argv = sys.argv[4:]

  kwargs = {}
  for a in argv:
    alist = str(a).split("=", 1)
    opt = str(alist[0])
    arg = alist[1]
    kwargs[opt] = arg
    pass

  [ filename, msgSize ] = writeORBConfigFile(path, host, port, kwargs)

  # :TRICKY: print values so they can be read from caller bash script
  # Example of bash script:
  # RETURN_VALUES=$(python ORBConfigFile.py path host port)
  # RETURN_VALUE_1=$(echo ${RETURN_VALUES} | cut -d' ' -f1)
  # RETURN_VALUE_2=$(echo ${RETURN_VALUES} | cut -d' ' -f2)
  # ...
  # IMPORTANT NOTE: this print call MUST BE the first one!!
  print filename, msgSize
