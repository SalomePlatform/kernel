#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2014  CEA/DEN, EDF R&D, OPEN CASCADE
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
from optparse import OptionParser
from NSparam import getNSparams
import socket
import subprocess

# Use to display newlines (\n) in epilog
class MyParser(OptionParser):
    def format_epilog(self, formatter):
        return self.epilog
#

def configureSession(args=[]):
  usage = "Usage: %prog [options] [command]"
  epilog  = """\n
If the command is not given a shell is opened; else execute the given command.
Command may be a series of Python scripts with arguments: [PYTHON_FILE [args] [PYTHON_FILE [args]...]]
Python file arguments, if any, must be comma-separated (without blank characters) and prefixed by "args:" (without quotes), e.g. myscript.py args:arg1,arg2=val,...
\n
If PORT and MACHINE are not given, try to connect to the last active session on the local machine.
If PORT and MACHINE are given, try to connect to the remote session associated with PORT on MACHINE.
If MACHINE is not given, try to connect to the session associated to PORT on the local machine.
If PORT is not given, try to connect to the remote session associated to port 2810 on MACHINE.\n\n"""
  parser = MyParser(usage=usage, epilog=epilog)
  parser.add_option("-p", "--port", metavar="<port>", default=0,
                    action="store", type="string", dest="port",
                    help="The port to connect to."
                    )
  parser.add_option("-m", "--machine", metavar="<machine>", default=0,
                    action="store", type="string", dest="host",
                    help="The machine to connect to."
                    )
  try:
    (options, args) = parser.parse_args(args)
  except Exception, e:
    print e
    return

  port = options.port
  host = options.host

  # :GLITCH: this code defines specific environment variables (OMNIORB_CONFIG, NSPORT,
  # NSHOST) which are later used by other modules. Working, but not really "safe"...
  if not port:
    if not host:
      # neither MACHINE nor PORT are given
      # --- set omniORB configuration to current session if any
      omniorbUserPath = os.environ['OMNIORB_USER_PATH']
      fileOmniConfig = omniorbUserPath + '/.omniORB_' + os.environ['USER'] + '_last.cfg'
      if os.path.isfile(fileOmniConfig):
        os.environ['OMNIORB_CONFIG'] = fileOmniConfig
        # --- set environment variables for port and hostname of NamingService
        host, port = getNSparams()
      else:
        # No running session
        host = "no_host"
        port = "no_port"
    else:
      # only MACHINE is given
      port = '2810'
      _writeConfigFile(port, host)
    #
  else:
    if not host:
      # only PORT is given
      host = socket.gethostname()
    # both MACHINE and PORT are given
    _writeConfigFile(port, host)
  #
  os.environ['NSPORT'] = port
  os.environ['NSHOST'] = host
#

# --- set the OMNIORB_CONFIG file and environment relative to this run of SALOME
def _writeConfigFile(port, host):
  path = os.environ['OMNIORB_USER_PATH']
  kwargs = {'with_username' : os.environ['USER']}

  from ORBConfigFile import writeORBConfigFile
  [ filename, msgSize ] = writeORBConfigFile(path, host, port, kwargs)

  os.environ['OMNIORB_CONFIG'] = filename
#

# command looks like a Bash command-line:
# script1.py [args] ; script2.py [args] ; ...
def runSession(command):
  if command:
    sep = ";"
    if sys.platform == "win32":
      sep= "&"
    command = command.split(sep)
    outmsg = []
    errmsg = []
    for cmd in command:
      save_cmd = cmd
      cmd = cmd.strip().split(' ')
      #proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
      proc = subprocess.Popen(cmd)
      (stdoutdata, stderrdata) = proc.communicate()
      if stdoutdata:
        outmsg.append(stdoutdata)
      if stderrdata:
        errmsg.append(stderrdata)

      if proc.returncode != 0:
        errmsg.append("Error raised when executing command: %s\n"%save_cmd)
        if outmsg:
          sys.stdout.write("".join(outmsg))
        if errmsg:
          sys.stderr.write("".join(errmsg))
        sys.exit(proc.returncode)

    return ("".join(outmsg), "".join(errmsg))
  else:
    absoluteAppliPath = os.getenv('ABSOLUTE_APPLI_PATH','')
    cmd = ["/bin/bash",  "--rcfile", absoluteAppliPath + "/.bashrc" ]
    proc = subprocess.Popen(cmd, shell=False, close_fds=True)
    return proc.communicate()
#
