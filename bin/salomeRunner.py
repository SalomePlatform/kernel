import os
import sys
import logging

from parseConfigFile import parseConfigFile
from parseConfigFile import convertEnvFileToConfigFile

import tempfile
import pickle
import subprocess
import platform

"""
The SalomeRunner class in an API to configure SALOME environment then
start SALOME using a single python command.

"""
class SalomeRunner:
  """
  Initialize environment from a list of configuration files
  identified by their names.
  These files should be in appropriate (new .cfg) format.
  However you can give old .sh environment files; in this case,
  the SalomeRunner class will try to automatically convert them
  to .cfg format before setting the environment.
  """
  def __init__(self, configFileNames=[]):
    for filename in configFileNames:
      basename, extension = os.path.splitext(filename)
      if extension == ".cfg":
        self.__setEnvironmentFromConfigFile(filename)
      elif extension == ".sh":
        temp = tempfile.NamedTemporaryFile(suffix='.cfg')
        try:
          convertEnvFileToConfigFile(filename, temp.name)
          self.__setEnvironmentFromConfigFile(temp.name)
        finally:
          # Automatically cleans up the file
          temp.close()
      else:
        self._getLogger().warning("Unrecognized extension for configuration file: %s", filename)
  #

  def go(self, args):
    # Run this module as a script, in order to use appropriate Python interpreter
    # according to current path (initialized from environment files).
    absoluteAppliPath = os.environ['ABSOLUTE_APPLI_PATH']
    proc = subprocess.Popen(['python', absoluteAppliPath+'/bin/salome/salomeRunner.py', pickle.dumps(self),  pickle.dumps(args)], shell=False, close_fds=True)
    proc.wait()
  #

  """Append value to PATH environment variable"""
  def addToPath(self, value):
    self.__addToReserved( 'PATH', value)
  #

  """Append value to LD_LIBRARY_PATH environment variable"""
  def addToLdLibraryPath(self, value):
    self.__addToReserved('LD_LIBRARY_PATH', value)
  #

  """Append value to PYTHONPATH environment variable"""
  def addToPythonPath(self, value):
    self.__addToReserved('PYTHONPATH', value)
  #

  """Set environment variable to value"""
  def setEnviron(self, name, value, overwrite=False):
    env = os.getenv(name, '')
    if env and not overwrite:
      self._getLogger().warning("Environment variable already existing and not overwritten: %s", name)
      return

    if env:
      self._getLogger().info("Overwriting environment variable: %s", name)

    value = os.path.expandvars(value) # expand environment variables
    self._getLogger().debug("Set environment variable: %s=%s", name, value)
    os.environ[name] = value
  #

  ###################################
  # This begins the private section #
  ###################################

  def _usage(self, unused=[]):
    exeName = os.path.splitext(os.path.basename(__file__))[0]

    msg = '''\
Usage: %s [command] [options] [--config=file1,...,filen]

Commands:
    start         Launches SALOME virtual application [DEFAULT]
    shell         Executes a script under SALOME application environment
    connect       Connects a Python console to the active SALOME session
    killall       Kill all SALOME running sessions
    info          Display some information about SALOME
    help          Show this message
    coffee        Yes! SALOME can also make coffee!!"\

'''%exeName

    print msg
  #

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
      self._getLogger().error("Unrecognized command: %s.", command)
      self._usage()
      sys.exit(1)

    return availableCommands[command], options
  #

  """
  Run SALOME!
  Args consist in a mandatory command followed by optionnal parameters.
  See usage for details on commands.
  """
  def _getStarted(self, args):
    command, options = self.__parseArguments(args)
    sys.argv = options

    if command is None:
      if args and args[0] in ["-h","--help","help"]:
        self._usage()
        sys.exit(0)
      # try to default to "start" command
      command = "_runAppli"

    try:
      getattr(self, command)(options) # run appropriate method
    except AttributeError:
      self._getLogger().error("Method %s is not implemented.", command)
      sys.exit(1)
  #

  def __setEnvironmentFromConfigFile(self, filename):
    configVars, reservedDict = parseConfigFile(filename, reserved=['PATH', 'LD_LIBRARY_PATH', 'PYTHONPATH'])

    # set environment
    for reserved in reservedDict:
      a = filter(None, reservedDict[reserved]) # remove empty elements
      reformattedVals = ':'.join(a)
      self.__addToReserved(reserved, reformattedVals)
      pass

    for key,val in configVars:
      self.setEnviron(key, val, overwrite=True)
      pass

    sys.path[:0] = os.environ['PYTHONPATH'].split(':')
  #

  def __addToReserved(self, name, value):
    if value == '':
      return

    value = os.path.expandvars(value) # expand environment variables
    self._getLogger().debug("Add to %s: %s", name, value)
    env = os.getenv(name, None)
    if env is None:
      os.environ[name] = value
    else:
      os.environ[name] = value + os.pathsep + env
  #

  def _runAppli(self, args=[]):
    # Initialize SALOME environment
    sys.argv = ['runSalome'] + args
    import setenv
    setenv.main(True)

    import runSalome
    runSalome.runSalome()
  #

  def _runSession(self, args=[]):
    sys.argv = ['runSession'] + args
    import runSession
    runSession.configureSession(args)

    import setenv
    setenv.main(True)

    if args:
      exe = args[0]
      # if exe does not contain any slashes (/), search in PATH
      # if exe contains slashes:
      #    if exe begins with a slash, use this absolute path
      #    else build absolute path relative to current working directory
      if (os.sep in exe) and (exe[0] is not os.sep):
        args[0] = os.getcwd() + os.sep + exe

      proc = subprocess.Popen(args, shell=False, close_fds=True)
      proc.wait()
    else:
      absoluteAppliPath = os.environ['ABSOLUTE_APPLI_PATH']
      cmd = ["/bin/bash",  "--rcfile", absoluteAppliPath + "/.bashrc" ]
      proc = subprocess.Popen(cmd, shell=False, close_fds=True)
      proc.wait()
  #

  def _runConsole(self, args=[]):
    # Initialize SALOME environment
    sys.argv = ['runConsole'] + args
    import setenv
    setenv.main(True)

    import runConsole
    runConsole.connect()
  #

  def _killAll(self, args=[]):
    #self._runAppli(['-k'] + args)
    from killSalome import killAllPorts
    killAllPorts()
  #

  def _showInfo(self, args=[]):
    print "Running with python", platform.python_version()
    self._runAppli(["--version"])
  #

  def _makeCoffee(self, args=[]):
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
    del d['_logger']
    return d
  #
  def __setstate__(self, d):
    self.__dict__.update(d) # I *think* this is a safe way to do it
  #
  # Excluding self._logger from pickle operation imply using the following method to access logger
  def _getLogger(self):
    if not hasattr(self, '_logger'):
      self._logger = logging.getLogger(__name__)
      #self._logger.setLevel(logging.DEBUG)
    return self._logger;
  #

###
import pickle
if __name__ == "__main__":
  if len(sys.argv) == 3:
    runner = pickle.loads(sys.argv[1])
    args = pickle.loads(sys.argv[2])
    runner._getStarted(args)
  else:
    SalomeRunner()._usage()
#
