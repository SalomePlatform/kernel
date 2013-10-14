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

from salomeLauncherUtils import SalomeRunnerException
from salomeLauncherUtils import getScriptsAndArgs, formatScriptsAndArgs

def usage():
  #exeName = os.path.splitext(os.path.basename(__file__))[0]

  msg = '''\
Usage: salome [command] [options] [--config=file1,...,filen]

Commands:
    start         Launches SALOME virtual application [DEFAULT]
    shell         Executes a script under SALOME application environment
    connect       Connects a Python console to the active SALOME session
    killall       Kill all SALOME running sessions
    info          Display some information about SALOME
    help          Show this message
    coffee        Yes! SALOME can also make coffee!!"

Use salome start --help or salome shell --help
to show help on start and shell commands.
'''

  print msg
#

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
    #it could be None explicitely (if user use multiples setEnviron...for standalone)
    if configFileNames==None:
       return

    if len(configFileNames) == 0:
      raise SalomeRunnerException("No configuration files given")

    reserved=['PATH', 'LD_LIBRARY_PATH', 'PYTHONPATH', 'MANPATH', 'PV_PLUGIN_PATH']
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
        except ConfigParser.ParsingError, e:
          self.getLogger().warning("Invalid token found when parsing file: %s\n"%(filename))
          print e
          print '\n'
        finally:
          # Automatically cleans up the file
          temp.close()
      else:
        self.getLogger().warning("Unrecognized extension for configuration file: %s", filename)
  #

  def go(self, args):
    # Run this module as a script, in order to use appropriate Python interpreter
    # according to current path (initialized from environment files).
    absoluteAppliPath = os.getenv('ABSOLUTE_APPLI_PATH','')
    proc = subprocess.Popen(['python', absoluteAppliPath+'/bin/salome/salomeRunner.py', pickle.dumps(self),  pickle.dumps(args)], shell=False, close_fds=True)
    proc.wait()
  #

  """Append value to PATH environment variable"""
  def addToPath(self, value):
    self.addToEnviron('PATH', value)
  #

  """Append value to LD_LIBRARY_PATH environment variable"""
  def addToLdLibraryPath(self, value):
    self.addToEnviron('LD_LIBRARY_PATH', value)
  #

  """Append value to PYTHONPATH environment variable"""
  def addToPythonPath(self, value):
    self.addToEnviron('PYTHONPATH', value)
  #

  """Set environment variable to value"""
  def setEnviron(self, name, value, overwrite=False):
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
  def unsetEnviron(self, name):
    if os.environ.has_key(name):
      del os.environ[name]
  #

  """Append value to environment variable"""
  def addToEnviron(self, name, value, separator=os.pathsep):
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
  def _getStarted(self, args):
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
    except SystemExit, exc:
      if exc==0:
        sys.exit(0) #catch sys.exit(0) happy end no warning
      if exc==1:
        self.getLogger().warning("SystemExit 1 in method %s.", command)
      sys.exit(1)
    except StandardError:
      self.getLogger().error("Unexpected error:")
      import traceback
      traceback.print_exc()
      sys.exit(1)
    except SalomeRunnerException, e:
      self.getLogger().error(e)
      sys.exit(1)
  #

  def __setEnvironmentFromConfigFile(self, filename, reserved=[]):
    unsetVars, configVars, reservedDict = parseConfigFile(filename, reserved)

    # unset variables
    for var in unsetVars:
      self.unsetEnviron(var)

    # set environment
    for reserved in reservedDict:
      a = filter(None, reservedDict[reserved]) # remove empty elements
      reformattedVals = ':'.join(a)
      self.addToEnviron(reserved, reformattedVals)
      pass

    for key,val in configVars:
      self.setEnviron(key, val, overwrite=True)
      pass

    sys.path[:0] = os.getenv('PYTHONPATH','').split(':')
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

    scriptArgs = getScriptsAndArgs(args)
    command = formatScriptsAndArgs(scriptArgs)
    if command:
      proc = subprocess.Popen(command, shell=True, close_fds=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
      return proc.communicate()
    else:
      absoluteAppliPath = os.getenv('ABSOLUTE_APPLI_PATH','')
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
    from killSalome import killAllPorts
    killAllPorts()
  #

  def _showInfo(self, args=[]):
    print "Running with python", platform.python_version()
    self._runAppli(["--version"])
  #

  def _usage(self, unused=[]):
    usage()
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
      self._logger.setLevel(logging.ERROR)
    return self._logger;
  #

###
import pickle
if __name__ == "__main__":
  if len(sys.argv) == 3:
    runner = pickle.loads(sys.argv[1])
    args = pickle.loads(sys.argv[2])
    (out, err) = runner._getStarted(args)
    if out:
      sys.stdout.write(out)
    if err:
      sys.stderr.write(err)
  else:
    usage()
#
