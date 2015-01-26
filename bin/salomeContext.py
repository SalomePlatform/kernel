# Copyright (C) 2013-2015  CEA/DEN, EDF R&D, OPEN CASCADE
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
import logging
import ConfigParser

from parseConfigFile import parseConfigFile
from parseConfigFile import convertEnvFileToConfigFile

import tempfile
import pickle
import subprocess
import platform

from salomeContextUtils import SalomeContextException

def usage():
  #exeName = os.path.splitext(os.path.basename(__file__))[0]

  msg = '''\
Usage: salome [command] [options] [--config=<file,folder,...>]

Commands:
=========
    start         Starts a SALOME session (through virtual application)
    shell         Initializes SALOME environment, and executes scripts passed
                  as command arguments
    connect       Connects a Python console to the active SALOME session
    killall       Kill all SALOME running sessions for current user
    info          Display some information about SALOME
    help          Show this message
    coffee        Yes! SALOME can also make coffee!!

If no command is given, default to start.

Command options:
================
    Use salome <command> --help to show help on command ; available for start
    and shell commands.

--config=<file,folder,...>
==========================
    Initialize SALOME environment from a list of context files and/or a list
    of folders containing context files. The list is comma-separated, whithout
    any blank characters.
'''

  print msg
#

"""
The SalomeContext class in an API to configure SALOME environment then
start SALOME using a single python command.

"""
class SalomeContext:
  """
  Initialize environment from a list of configuration files
  identified by their names.
  These files should be in appropriate (new .cfg) format.
  However you can give old .sh environment files; in this case,
  the SalomeContext class will try to automatically convert them
  to .cfg format before setting the environment.
  """
  def __init__(self, configFileNames=0):
    #it could be None explicitely (if user use multiples setVariable...for standalone)
    if configFileNames is None:
       return
    configFileNames = configFileNames or []
    if len(configFileNames) == 0:
      raise SalomeContextException("No configuration files given")

    reserved=['PATH', 'DYLD_LIBRARY_PATH', 'LD_LIBRARY_PATH', 'PYTHONPATH', 'MANPATH', 'PV_PLUGIN_PATH']
    for filename in configFileNames:
      basename, extension = os.path.splitext(filename)
      if extension == ".cfg":
        self.__setEnvironmentFromConfigFile(filename, reserved)
      elif extension == ".sh":
        #new convert procedures, temporary could be use not to be automatically deleted
        #temp = tempfile.NamedTemporaryFile(suffix='.cfg', delete=False)
        temp = tempfile.NamedTemporaryFile(suffix='.cfg')
        try:
          convertEnvFileToConfigFile(filename, temp.name, reserved)
          self.__setEnvironmentFromConfigFile(temp.name, reserved)
          temp.close()
        except (ConfigParser.ParsingError, ValueError) as e:
          self.getLogger().error("Invalid token found when parsing file: %s\n"%(filename))
          temp.close()
          sys.exit(1)
      else:
        self.getLogger().warning("Unrecognized extension for configuration file: %s", filename)
  #

  def runSalome(self, args):
    # Run this module as a script, in order to use appropriate Python interpreter
    # according to current path (initialized from environment files).
#    kill = False
#    for e in args:
#      if "--shutdown-server" in e:
#        kill = True
#        args.remove(e)

    import os
    absoluteAppliPath = os.getenv('ABSOLUTE_APPLI_PATH','')
    env_copy = os.environ.copy()
    proc = subprocess.Popen(['python', os.path.join(absoluteAppliPath,"bin","salome","salomeContext.py"), pickle.dumps(self), pickle.dumps(args)], shell=False, close_fds=True, env=env_copy)
    msg = proc.communicate()
 #   if kill:
 #     self._killAll(args)
    return msg, proc.returncode
  #

  """Append value to PATH environment variable"""
  def addToPath(self, value):
    self.addToVariable('PATH', value)
  #

  """Append value to LD_LIBRARY_PATH environment variable"""
  def addToLdLibraryPath(self, value):
    self.addToVariable('LD_LIBRARY_PATH', value)
  #

  """Append value to DYLD_LIBRARY_PATH environment variable"""
  def addToDyldLibraryPath(self, value):
    self.addToVariable('DYLD_LIBRARY_PATH', value)
  #

  """Append value to PYTHONPATH environment variable"""
  def addToPythonPath(self, value):
    self.addToVariable('PYTHONPATH', value)
  #

  """Set environment variable to value"""
  def setVariable(self, name, value, overwrite=False):
    env = os.getenv(name, '')
    if env and not overwrite:
      self.getLogger().warning("Environment variable already existing (and not overwritten): %s=%s", name, value)
      return

    if env:
      self.getLogger().warning("Overwriting environment variable: %s=%s", name, value)

    value = os.path.expandvars(value) # expand environment variables
    self.getLogger().debug("Set environment variable: %s=%s", name, value)
    os.environ[name] = value
  #

  """Unset environment variable"""
  def unsetVariable(self, name):
    if os.environ.has_key(name):
      del os.environ[name]
  #

  """Append value to environment variable"""
  def addToVariable(self, name, value, separator=os.pathsep):
    if value == '':
      return

    value = os.path.expandvars(value) # expand environment variables
    self.getLogger().debug("Add to %s: %s", name, value)
    env = os.getenv(name, None)
    if env is None:
      os.environ[name] = value
    else:
      os.environ[name] = value + separator + env
  #

  ###################################
  # This begins the private section #
  ###################################

  def __parseArguments(self, args):
    if len(args) == 0 or args[0].startswith("-"):
      return None, args

    command = args[0]
    options = args[1:]

    availableCommands = {
      'start' :   '_runAppli',
      'shell' :   '_runSession',
      'connect' : '_runConsole',
      'killall':  '_killAll',
      'info':     '_showInfo',
      'help':     '_usage',
      'coffee' :  '_makeCoffee'
      }

    if not command in availableCommands.keys():
      command = "start"
      options = args

    return availableCommands[command], options
  #

  """
  Run SALOME!
  Args consist in a mandatory command followed by optionnal parameters.
  See usage for details on commands.
  """
  def _startSalome(self, args):
    try:
      import os
      absoluteAppliPath = os.getenv('ABSOLUTE_APPLI_PATH')
      import sys
      path = os.path.realpath(os.path.join(absoluteAppliPath, "bin", "salome"))
      if not path in sys.path:
        sys.path[:0] = [path]
    except:
      pass

    command, options = self.__parseArguments(args)
    sys.argv = options

    if command is None:
      if args and args[0] in ["-h","--help","help"]:
        usage()
        sys.exit(0)
      # try to default to "start" command
      command = "_runAppli"

    try:
      res = getattr(self, command)(options) # run appropriate method
      return res or (None, None)
    except SystemExit, returncode:
      if returncode != 0:
        self.getLogger().warning("SystemExit %s in method %s.", returncode, command)
      sys.exit(returncode)
    except StandardError:
      self.getLogger().error("Unexpected error:")
      import traceback
      traceback.print_exc()
      sys.exit(1)
    except SalomeContextException, e:
      self.getLogger().error(e)
      sys.exit(1)
  #

  def __setEnvironmentFromConfigFile(self, filename, reserved=None):
    if reserved is None:
      reserved = []
    try:
      unsetVars, configVars, reservedDict = parseConfigFile(filename, reserved)
    except SalomeContextException, e:
      msg = "%s"%e
      file_dir = os.path.dirname(filename)
      file_base = os.path.basename(filename)
      base_no_ext, ext = os.path.splitext(file_base)
      sh_file = os.path.join(file_dir, base_no_ext+'.sh')
      if ext == ".cfg" and os.path.isfile(sh_file):
        msg += "Found similar %s file; trying to parse this one instead..."%(base_no_ext+'.sh')
        temp = tempfile.NamedTemporaryFile(suffix='.cfg')
        try:
          convertEnvFileToConfigFile(sh_file, temp.name, reserved)
          self.__setEnvironmentFromConfigFile(temp.name, reserved)
          msg += "OK\n"
          self.getLogger().warning(msg)
          temp.close()
          return
        except (ConfigParser.ParsingError, ValueError) as e:
          msg += "Invalid token found when parsing file: %s\n"%(sh_file)
          self.getLogger().error(msg)
          temp.close()
          sys.exit(1)
      else:
        self.getLogger().error(msg)
        sys.exit(1)

    # unset variables
    for var in unsetVars:
      self.unsetVariable(var)

    # set environment
    for reserved in reservedDict:
      a = filter(None, reservedDict[reserved]) # remove empty elements
      a = [ os.path.realpath(x) for x in a ]
      reformattedVals = os.pathsep.join(a)
      self.addToVariable(reserved, reformattedVals)
      pass

    for key,val in configVars:
      self.setVariable(key, val, overwrite=True)
      pass

    pythonpath = os.getenv('PYTHONPATH','').split(os.pathsep)
    pythonpath = [ os.path.realpath(x) for x in pythonpath ]
    sys.path[:0] = pythonpath
  #

  def _runAppli(self, args=None):
    if args is None:
      args = []
    # Initialize SALOME environment
    sys.argv = ['runSalome'] + args
    import setenv
    setenv.main(True)

    import runSalome
    runSalome.runSalome()
  #

  def _runSession(self, args=None):
    if args is None:
      args = []
    sys.argv = ['runSession'] + args
    import runSession
    params, args = runSession.configureSession(args, exe="salome shell")

    sys.argv = ['runSession'] + args
    import setenv
    setenv.main(True)

    return runSession.runSession(params, args)
  #

  def _runConsole(self, args=None):
    if args is None:
      args = []
    # Initialize SALOME environment
    sys.argv = ['runConsole'] + args
    import setenv
    setenv.main(True)

    cmd = ["python", "-c", "import runConsole\nrunConsole.connect()" ]
    proc = subprocess.Popen(cmd, shell=False, close_fds=True)
    return proc.communicate()
  #

  def _killAll(self, args=None):
    if args is None:
      args = []
    try:
      import PortManager # mandatory
      from multiprocessing import Process
      from killSalomeWithPort import killMyPort
      ports = PortManager.getBusyPorts()

      if ports:
        import tempfile
        for port in ports:
          with tempfile.NamedTemporaryFile():
            p = Process(target = killMyPort, args=(port,))
            p.start()
            p.join()
    except ImportError:
      from killSalome import killAllPorts
      killAllPorts()
      pass

  #

  def _showInfo(self, args=None):
    print "Running with python", platform.python_version()
    self._runAppli(["--version"])
  #

  def _usage(self, unused=None):
    usage()
  #

  def _makeCoffee(self, args=None):
    print "                        ("
    print "                          )     ("
    print "                   ___...(-------)-....___"
    print "               .-\"\"       )    (          \"\"-."
    print "         .-\'``\'|-._             )         _.-|"
    print "        /  .--.|   `\"\"---...........---\"\"`   |"
    print "       /  /    |                             |"
    print "       |  |    |                             |"
    print "        \\  \\   |                             |"
    print "         `\\ `\\ |                             |"
    print "           `\\ `|                             |"
    print "           _/ /\\                             /"
    print "          (__/  \\                           /"
    print "       _..---\"\"` \\                         /`\"\"---.._"
    print "    .-\'           \\                       /          \'-."
    print "   :               `-.__             __.-\'              :"
    print "   :                  ) \"\"---...---\"\" (                 :"
    print "    \'._               `\"--...___...--\"`              _.\'"
    print "      \\\"\"--..__                              __..--\"\"/"
    print "       \'._     \"\"\"----.....______.....----\"\"\"     _.\'"
    print "          `\"\"--..,,_____            _____,,..--\"\"`"
    print "                        `\"\"\"----\"\"\"`"
    sys.exit(0)
  #

  # Add the following two methods since logger is not pickable
  # Ref: http://stackoverflow.com/questions/2999638/how-to-stop-attributes-from-being-pickled-in-python
  def __getstate__(self):
    d = dict(self.__dict__)
    if hasattr(self, '_logger'):
      del d['_logger']
    return d
  #
  def __setstate__(self, d):
    self.__dict__.update(d) # I *think* this is a safe way to do it
  #
  # Excluding self._logger from pickle operation imply using the following method to access logger
  def getLogger(self):
    if not hasattr(self, '_logger'):
      self._logger = logging.getLogger(__name__)
      #self._logger.setLevel(logging.DEBUG)
      #self._logger.setLevel(logging.WARNING)
      self._logger.setLevel(logging.ERROR)
    return self._logger
  #

if __name__ == "__main__":
  if len(sys.argv) == 3:
    context = pickle.loads(sys.argv[1])
    args = pickle.loads(sys.argv[2])

    (out, err) = context._startSalome(args)
    if out:
      sys.stdout.write(out)
    if err:
      sys.stderr.write(err)
  else:
    usage()
#
