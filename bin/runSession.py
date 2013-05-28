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

import os
import sys
from optparse import OptionParser
from NSparam import getNSparams

# Use to display newlines (\n) in epilog
class MyParser(OptionParser):
    def format_epilog(self, formatter):
        return self.epilog
#

def configureSession(args=[]):
  usage = "Usage: %prog [options]"
  epilog  = """\nIf the command is not given a shell is opened.
If PORT and MACHINE are not given, try to connect to the last active session on the local machine.
If PORT and MACHINE are given, try to connect to the remote session associated with PORT on MACHINE.
If MACHINE is not given, try to connect to the session associated to PORT on the local machine.
If PORT is not given, try to connect to the remote session associated to port 2810 on MACHINE.\n\n"""
  parser = MyParser(usage=usage, epilog=epilog)
  parser.add_option("-p", "--port", metavar="<port>", default=0,
                    action="store", type="int", dest="port",
                    help="The port to connect to."
                    )
  parser.add_option("-m", "--machine", metavar="<machine>", default=0,
                    action="store", type="int", dest="machine",
                    help="The machine to connect to."
                    )
  try:
    (options, args) = parser.parse_args(args)
  except Exception, e:
    print e
    return

  port = options.port
  machine = options.machine

  # :GLITCH: this code defines specific environment variables (OMNIORB_CONFIG, NSPORT,
  # NSHOST) which are later used by other modules. Working, but not really "safe"...
  if not port:
    if not machine:
      # neither MACHINE nor PORT are given
      # --- set omniORB configuration to current session if any
      absoluteAppliPath = os.environ['ABSOLUTE_APPLI_PATH']
      fileOmniConfig = absoluteAppliPath + '/USERS/.omniORB_' + os.environ['USER'] + '_last.cfg'
      if os.path.isfile(fileOmniConfig):
        os.environ['OMNIORB_CONFIG'] = fileOmniConfig
        # --- set environment variables for port and hostname of NamingService
        host, port = getNSparams()
        os.environ['NSHOST'] = host
        os.environ['NSPORT'] = port
      else:
        # No running session
        os.environ['NSHOST'] = "no_host"
        os.environ['NSPORT'] = "no_port"
        pass
    else:
      # only MACHINE is given
      _writeConfigFile(2810, os.environ['NSHOST'])
    #
  else:
    if not machine:
      # only PORT is given
      os.environ['NSHOST'] = `hostname`
      pass
    # both MACHINE and PORT are given
    _writeConfigFile(os.environ['NSPORT'], os.environ['NSHOST'])
  #
#

# --- set the OMNIORB_CONFIG file and environment relative to this run of SALOME
def _writeConfigFile(port, host):
  os.environ['NSPORT'] = port
  os.environ['NSHOST'] = host

  absoluteAppliPath = os.environ['ABSOLUTE_APPLI_PATH']
  path = absoluteAppliPath + '/USERS'
  kwargs = {'with_username' : os.environ['USER']}

  from ORBConfigFile import writeORBConfigFile
  [ filename, msgSize ] = writeORBConfigFile(path, host, port, kwargs)

  os.environ['OMNIORB_CONFIG'] = filename
#
