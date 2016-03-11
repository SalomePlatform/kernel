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

from optparse import OptionParser
import os
import sys
import user
import pickle

# Use to display newlines (\n) in epilog
class MyParser(OptionParser):
  def format_epilog(self, formatter):
    return self.epilog
#

def __parse_args(args):
  if args is None:
    args = []

  usage = "Usage: salome connect [-p port] [ -c command | script | - ]"
  epilog  = """
Connects a Python console to a local SALOME instance.\n
If port is given, try to connect to corresponding instance.
If port is not given, or does not correspond to a running instance,
ask user to select a port from list of available SALOME instances.\n

The -c option can be used to specify the command to execute in the interpreter.
A script can also be used.
For example:
       salome connect -p 2810 -c 'print "Hello"'
       salome connect -p 2810 hello.py
"""
  parser = MyParser(usage=usage, epilog=epilog)
  parser.add_option("-p", metavar="<port>", default=0,
                    action="store", type="string", dest="port",
                    help="The port to connect to."
                    )
  parser.add_option('-c', dest="command", default=None,
                    help="The command to execute in the interpreter."
                    )
  try:
    (options, args) = parser.parse_args(args)
  except Exception, e:
    print e
    return

  return options, args
#

def __show_running_instances(list_of_instances):
  print '-'*10
  print "Running instances:"
  for i in range(len(list_of_instances)):
    host, port, _ = list_of_instances[i]
    print "   [%d] %s:%s"%(i+1, host, port)
  print '-'*10
#

def __choose_in(choices):
  __show_running_instances(choices)
  rep = raw_input("Please enter the number of instance to use (0 to cancel): ")
  if rep == '0':
    return None, None, None
  elif rep in [str(i) for i in range(1, len(choices)+1)]:
    return choices[int(rep)-1]
  else:
    print "*** Invalid number! ***"
    return __choose_in(choices)
#

def __get_running_session(requested_port=None, lastInstanceByDefault=False):
  import glob
  import salome_utils
  from ORBConfigFile import readORBConfigFile

  omniorbUserPath = os.getenv("OMNIORB_USER_PATH")
  files = glob.glob(os.path.join(omniorbUserPath,".omniORB_"+salome_utils.getUserName()+"_*[!last].cfg"))
  available_connexions = []
  for filename in files:
    host, port = readORBConfigFile(filename)
    available_connexions.append((host, port, filename))

  host, port, filename = None, None, None
  if requested_port:
    print "Search for running instance on port %s..."%requested_port
    found = [(h,p,f) for h,p,f in available_connexions if int(p) == int(requested_port)]
    if not found:
      print "   ...no running instance found"
    elif len(found) == 1:
      host, port, filename = found[0]
      print "   ...found unique instance: %s:%s"%(host,port)
    else:
      print "   ...multiple instances found ; please choose one in the following:"
      host, port, filename = __choose_in(found)
  else: # no requested port
    if not available_connexions:
      print "No running instance found"
    elif len(available_connexions) == 1:
      host, port, filename = available_connexions[0]
      print "Found unique instance: %s:%s"%(host,port)
    else:
      print "Multiple instances found ; please choose one in the following:"
      host, port, filename = __choose_in(available_connexions)
      pass

  if port:
    print "Selected instance: %s:%s"%(host, port)
  else:
    print "Cancel."

  return host, port, filename
#

import CORBA
import CosNaming
import orbmodule

class client(orbmodule.client):
  def initNS(self,args):
    # Obtain a reference to the root naming context
    obj = self.orb.resolve_initial_references("NameService")
    try:
      self.rootContext = obj._narrow(CosNaming.NamingContext)
      return
    except (CORBA.TRANSIENT,CORBA.OBJECT_NOT_EXIST,CORBA.COMM_FAILURE):
      print "It's not a valid naming service"
      self.rootContext = None
      sys.stdout.flush()
      raise
#

def start_client():
  try:
    clt = client()
  except Exception:
    import traceback
    traceback.print_exc()
    sys.exit(1)
  #

  session_server = clt.Resolve('/Kernel/Session')
  if session_server:
    session = clt.waitNS("/Kernel/Session")
  catalog = clt.waitNS("/Kernel/ModulCatalog")
  studyMgr = clt.waitNS("/myStudyManager")

  import salome
  salome.salome_init()
  from salome import lcc
  print "--> now connected to SALOME"
#

def _prompt(environment=None, commands=None, message="Connecting to SALOME"):
  if environment is None:
    environment = globals().copy()
    environment.update(locals())
  if commands is None:
    commands = []

  import code
  import rlcompleter
  import readline
  readline.set_completer(rlcompleter.Completer(environment).complete)
  readline.parse_and_bind("tab: complete")
  # calling this with globals ensures we can see the environment
  print message
  shell = code.InteractiveConsole(environment)
  for cmd in commands:
    print "Execute command:", cmd
    shell.push(cmd)
    pass
  shell.interact()
#

def connect(args=None, env=None):
  if env is not None:
    os.environ = env
  options, args = __parse_args(args)
  host, port, filename = __get_running_session(options.port)
  if not port:
    sys.exit(0)

  cmd = [
    "os.environ['OMNIORB_CONFIG'] = '%s'"%filename,
    "start_client()"
    ]
  if options.command:
    cmd.append(options.command)
  if args: # unprocessed: may be scripts
    for arg in args:
      cmd.append("execfile('%s')"%os.path.abspath(os.path.expanduser(arg)))

  if port:
    import subprocess
    absoluteAppliPath = os.getenv('ABSOLUTE_APPLI_PATH','')
    env_copy = os.environ.copy()
    proc = subprocess.Popen(['python', os.path.join(absoluteAppliPath,"bin","salome","runConsole.py"), pickle.dumps(cmd)], shell=False, close_fds=True, env=env_copy)
    return proc.communicate()
#

if __name__ == "__main__":
  if len(sys.argv) == 2:
    cmd = pickle.loads(sys.argv[1])
    sys.argv = []
    _prompt(commands=cmd)
  else:
    print "runConsole.py: incorrect usage!"
#
