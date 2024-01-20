# Copyright (C) 2007-2024  CEA, EDF, OPEN CASCADE
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
from optparse import OptionParser
import subprocess
from salome_utils import getUserName
from salomeContextUtils import getShortAndExtraArgs

# Use to display newlines (\n) in epilog
class MyParser(OptionParser):
    def format_epilog(self, formatter):
        return self.epilog
#
#
# --- set the OMNIORB_CONFIG file and environment relative to this run of SALOME
def _writeConfigFile(port, host):
    path = os.environ['OMNIORB_USER_PATH']
    kwargs = {'with_username' : getUserName()}

    from ORBConfigFile import writeORBConfigFile
    [ filename, msgSize ] = writeORBConfigFile(path, host, port, kwargs)

    os.environ['OMNIORB_CONFIG'] = filename
#

def runRemote(args):
    if args is None:
      args = []
    usage = "Usage: salome runremote [options] -- command"
    epilog  = """\n
Execute command in SALOME environment from a remote call, ssh or rsh.
salome runremote is used notably to launch containers from a distant salome session.

For example:
    salome remote -m xxx.cea.fr -p 2810 -- ls /tmp >/dev/null 2>&1  
     -> execute the command ls /tmp >/dev/null 2>&1

    salome remote -m xxx.cea.fr -p 2810 -- SALOME_Container Cont_S 
                                        -ORBInitRef NameService=IOR:01...
     -> starts a Salome container called Cont_S connected to port 2810 
        of xxx.cea.fr
"""
    parser = MyParser(usage=usage, epilog=epilog)
    parser.add_option("-p", "--port", metavar="<port>", default="2810",
                      action="store", type="string", dest="port",
                      help="The port to connect to."
                     )
    parser.add_option("-m", "--machine", metavar="<machine>",
                      action="store", type="string", dest="host", 
                      default="localhost",
                      help="The machine where salome was launched."
                     )
    parser.add_option('-d', '--directory', dest="directory", 
                      metavar="<directory>", default=None,
                      help="The directory where to execute the command."
                     )

    # separate runRemote args from the command to run (given after --)
    short_args, extra_args = getShortAndExtraArgs(args)
    try:
      (options, args) = parser.parse_args(short_args)
    except Exception as e:
      print(e)
      print(usage)
      print(epilog)
      return

    port = options.port
    host = options.host
    directory = options.directory
    command=extra_args[1:]

    _writeConfigFile(port, host)
    os.environ['NSPORT'] = port
    os.environ['NSHOST'] = host
    print("[ Remote Command ] ", " ".join(command))
    cmd = subprocess.Popen(command, cwd=directory)
    cmd.wait()
    return
#

