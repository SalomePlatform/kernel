#! /usr/bin/env python3
# Copyright (C) 2013-2022  CEA/DEN, EDF R&D, OPEN CASCADE
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
import configparser

from parseConfigFile import parseConfigFile

import tempfile
import pickle
import subprocess
import sys
import platform

from salomeContextUtils import SalomeContextException

def usage():
  msg = '''\
Usage: salome [command] [options] [--config=<file,folder,...>] [--with-env-modules=<env_module1,env_module2,...>]

Commands:
=========
    start           Start a new SALOME instance. Start a single SALOME_Session_Server_No_Server
                    process with environment relevant to the application and hosting all servants in it.
    context         Initialize SALOME context. Current environment is extended.
    shell           Initialize SALOME context, attached to the last created SALOME
                    instance if any, and executes scripts passed as command arguments.
                    User works in a Shell terminal. SALOME environment is set but
                    application is not started.
    test            Run SALOME tests.
    info            Display some information about SALOME.
    doc <module(s)> Show online module documentation (if available).
                    Module names must be separated by blank characters.
    help            Show this message.
    remote          run command in SALOME environment from remote call, ssh or rsh.
    withsession     Start a new SWS SALOME instance with multiple servers hosting all servants.
    connect         In SWS context, Connect a Python console to the active SALOME instance.
    kill <port(s)>  In SWS context, Terminate SALOME instances running on given ports for current user.
                    Port numbers must be separated by blank characters.
    killall         Terminate *all* SALOME running SWS instances for current user.
                    Do not start a new one.

If no command is given, default is start.

Command options:
================
    Use salome <command> --help to show help on command. Available for the
    following commands: start, shell, connect, test, info.

--config=<file,folder,...>
==========================
    Initialize SALOME context from a list of context files and/or a list
    of folders containing context files. The list is comma-separated, without
    any blank characters.

--with-env-modules=<env_module1,env_module2,...>
================================================
    Initialize SALOME context with the provided additional environment modules.
    The list is comma-separated, without any blank characters.
'''

  print(msg)
#

"""
The SalomeContext class in an API to configure SALOME context then
start SALOME using a single python command.

"""
class SalomeContext:
  """
  Initialize context from a list of configuration files
  identified by their names.
  These files should be in appropriate .cfg format.
  """
  def __init__(self, configFileNames=0):
    self.getLogger().setLevel(logging.INFO)
    #it could be None explicitly (if user use multiples setVariable...for standalone)
    if configFileNames is None:
       return
    configFileNames = configFileNames or []
    if len(configFileNames) == 0:
      raise SalomeContextException("No configuration files given")

    reserved=['PATH', 'DYLD_FALLBACK_LIBRARY_PATH', 'DYLD_LIBRARY_PATH', 'LD_LIBRARY_PATH', 'PYTHONPATH', 'MANPATH', 'PV_PLUGIN_PATH', 'INCLUDE', 'LIBPATH', 'SALOME_PLUGINS_PATH', 'LIBRARY_PATH', 'QT_PLUGIN_PATH']
    for filename in configFileNames:
      basename, extension = os.path.splitext(filename)
      if extension == ".cfg":
        self.__setContextFromConfigFile(filename, reserved)
      else:
        self.getLogger().error("Unrecognized extension for configuration file: %s", filename)
  #

  def __loadEnvModules(self, env_modules):
    modulecmd = os.getenv('LMOD_CMD')
    if not modulecmd:
      raise SalomeContextException("Module environment not present")
      return
    try:
      out, err = subprocess.Popen([modulecmd, "python", "try-load"] + env_modules, stdout=subprocess.PIPE).communicate()
      exec(out)  # define specific environment variables
    except Exception:
      raise SalomeContextException("Failed to load env modules: %s ..." % ' '.join(env_modules))
      pass
  #

  def runSalome(self, args):
    import os
    # Run this module as a script, in order to use appropriate Python interpreter
    # according to current path (initialized from context files).
    env_modules_option = "--with-env-modules="
    env_modules_l = [x for x in args if x.startswith(env_modules_option)]
    if env_modules_l:
      env_modules = env_modules_l[-1][len(env_modules_option):].split(',')
      self.__loadEnvModules(env_modules)
      args = [x for x in args if not x.startswith(env_modules_option)]
    else:
      env_modules = os.getenv("SALOME_ENV_MODULES", None)
      if env_modules:
        self.__loadEnvModules(env_modules.split(','))

    absoluteAppliPath = os.getenv('ABSOLUTE_APPLI_PATH','')
    env_copy = os.environ.copy()
    selfBytes= pickle.dumps(self, protocol=0)
    argsBytes= pickle.dumps(args, protocol=0)
    proc = subprocess.Popen(['python3', os.path.join(absoluteAppliPath,"bin","salome","salomeContext.py"), selfBytes.decode('latin1'), argsBytes.decode('latin1')], shell=False, close_fds=True, env=env_copy)
    out, err = proc.communicate()
    return out, err, proc.returncode
  #

  """Append value to PATH environment variable"""
  def addToPath(self, value):
    self.addToVariable('PATH', value)
  #

  """Append value to LD_LIBRARY_PATH environment variable"""
  def addToLdLibraryPath(self, value):
    if sys.platform == 'win32':
      self.addToVariable('PATH', value)
    elif  sys.platform == 'darwin':
      if "LAPACK" in value:
        self.addToVariable('DYLD_FALLBACK_LIBRARY_PATH', value)
      else:
        self.addToVariable('DYLD_LIBRARY_PATH', value)
    else:
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
      self.getLogger().error("Environment variable already existing (and not overwritten): %s=%s", name, value)
      return

    if env:
      self.getLogger().debug("Overwriting environment variable: %s=%s", name, value)

    value = os.path.expandvars(value) # expand environment variables
    self.getLogger().debug("Set environment variable: %s=%s", name, value)
    os.environ[name] = value
  #

  def setDefaultValue(self, name, value):
    """ Set environment variable only if it is undefined."""
    env = os.getenv(name, '')
    if not env:
      value = os.path.expandvars(value) # expand environment variables
      self.getLogger().debug("Set environment variable: %s=%s", name, value)
      os.environ[name] = value

  """Unset environment variable"""
  def unsetVariable(self, name):
    if name in os.environ:
      del os.environ[name]
  #

  """Prepend value to environment variable"""
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

  """Append a variable"""
  def appendVariable(self, name, value, separator=os.pathsep):
    if value == '':
      return

    value = os.path.expandvars(value) # expand environment variables
    env = os.getenv(name, None)
    if env is None:
      os.environ[name] = value
    else:
      os.environ[name] = env + separator + value
    return

  ###################################
  # This begins the private section #
  ###################################

  def __parseArguments(self, args):
    if len(args) == 0 or args[0].startswith("-"):
      return None, args

    command = args[0]
    options = args[1:]

    availableCommands = {
      'start'   : '_sessionless',
      'withsession' : '_runAppli',
      'context' : '_setContext',
      'shell'   : '_runSession',
      'remote'  : '_runRemote',
      'connect' : '_runConsole',
      'kill'    : '_kill',
      'killall' : '_killAll',
      'test'    : '_runTests',
      'info'    : '_showInfo',
      'doc'     : '_showDoc',
      'help'    : '_usage',
      'coffee'  : '_makeCoffee',
      'car'     : '_getCar',
      }

    if command not in availableCommands:
      command = "start"
      options = args

    return availableCommands[command], options
  #

  """
  Run SALOME!
  Args consist in a mandatory command followed by optional parameters.
  See usage for details on commands.
  """
  def _startSalome(self, args):
    import os
    import sys
    try:
      from setenv import add_path
      absoluteAppliPath = os.getenv('ABSOLUTE_APPLI_PATH')
      path = os.path.realpath(os.path.join(absoluteAppliPath, "bin", "salome"))
      add_path(path, "PYTHONPATH")
      path = os.path.realpath(os.path.join(absoluteAppliPath, "bin", "salome", "appliskel"))
      add_path(path, "PYTHONPATH")

    except Exception:
      pass

    command, options = self.__parseArguments(args)
    sys.argv = options

    if command is None:
      if args and args[0] in ["-h","--help","help"]:
        usage()
        return 0
      # try to default to "start" command
      command = "_sessionless"

    try:
      res = getattr(self, command)(options) # run appropriate method
      return res or 0
    except SystemExit as ex:
      if ex.code != 0:
        self.getLogger().error("SystemExit %s in method %s.", ex.code, command)
      return ex.code
    except SalomeContextException as e:
      self.getLogger().error(e)
      return 1
    except Exception:
      self.getLogger().error("Unexpected error:")
      import traceback
      traceback.print_exc()
      return 1
  #

  def __setContextFromConfigFile(self, filename, reserved=None):
    mesa_root_dir = "MESA_ROOT_DIR"
    if reserved is None:
      reserved = []
    try:
      configInfo = parseConfigFile(filename, reserved)
      unsetVars = configInfo.unsetVariables
      configVars = configInfo.outputVariables
      reservedDict = configInfo.reservedValues
      defaultValues = configInfo.defaultValues
    except SalomeContextException as e:
      msg = "%s"%e
      self.getLogger().error(msg)
      return 1

    # unset variables
    for var in unsetVars:
      self.unsetVariable(var)

    # mesa stuff
    if "MESA_GL_VERSION_OVERRIDE" in os.environ:
      configVarsDict = {k:v for (k,v) in configVars}
      if mesa_root_dir in configVarsDict:
        path_to_mesa_lib = os.path.join(configVarsDict[mesa_root_dir],"lib")
        if os.name == "posix":
          self.addToVariable("LD_LIBRARY_PATH",path_to_mesa_lib)
        else:
          self.addToVariable("PATH",path_to_mesa_lib)

    # set context
    for reserved in reservedDict:
      a = [_f for _f in reservedDict[reserved] if _f] # remove empty elements
      a = [ os.path.realpath(x) for x in a ]
      reformattedVals = os.pathsep.join(a)
      if reserved in ["INCLUDE", "LIBPATH"]:
        self.addToVariable(reserved, reformattedVals, separator=' ')
      else:
        self.addToVariable(reserved, reformattedVals)
      pass


    for key,val in configVars:
      self.setVariable(key, val, overwrite=True)
      pass

    for key,val in defaultValues:
      self.setDefaultValue(key, val)
      pass

    pythonpath = os.getenv('PYTHONPATH','').split(os.pathsep)
    pythonpath = [ os.path.realpath(x) for x in pythonpath ]
    sys.path[:0] = pythonpath
  #

  def _runAppli(self, args=None):
    if args is None:
      args = []
    # Initialize SALOME environment
    sys.argv = ['runSalomeOld'] + args
    import setenv
    setenv.main(True, exeName="salome withsession")

    import runSalomeOld
    runSalomeOld.runSalome()
    return 0
  #

  def _sessionless(self, args=None):
    if args is None:
      args = []
    sys.argv = ['runSalome'] + args
    import setenv
    setenv.main(True, exeName="salome withsession")

    import runSalome
    runSalome.runSalome()
    return 0
  #

  def _setContext(self, args=None):
    salome_context_set = os.getenv("SALOME_CONTEXT_SET")
    if salome_context_set:
      print("***")
      print("*** SALOME context has already been set.")
      print("*** Enter 'exit' (only once!) to leave SALOME context.")
      print("***")
      return 0

    os.environ["SALOME_CONTEXT_SET"] = "yes"
    print("***")
    print("*** SALOME context is now set.")
    print("*** Enter 'exit' (only once!) to leave SALOME context.")
    print("***")

    if sys.platform == 'win32':
      cmd = ['cmd.exe']
    else:
      cmd = ["/bin/bash"]
    proc = subprocess.Popen(cmd, shell=False, close_fds=True)
    proc.communicate()
    return proc.returncode
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

  def _runRemote(self, args=None):
    if args is None:
      args = []
#   complete salome environment 
    sys.argv = ['runRemote']
    import setenv
    setenv.main(True)

    import runRemote
    return runRemote.runRemote(args)
  #

  def _runConsole(self, args=None):
    if args is None:
      args = []
    # Initialize SALOME environment
    sys.argv = ['runConsole']
    import setenv
    setenv.main(True)

    import runConsole
    return runConsole.connect(args)
  #

  def _kill(self, args=None):
    if args is None:
      args = []
    ports = args
    if not ports:
      print("Port number(s) not provided to command: salome kill <port(s)>")
      return 1

    import subprocess
    sys.argv = ['kill']
    import setenv
    setenv.main(True)
    if os.getenv("NSHOST") == "no_host":
      os.unsetenv("NSHOST")
    for port in ports:
      if sys.platform == "win32":
        proc = subprocess.Popen([os.getenv("PYTHONBIN"), "-m", "killSalomeWithPort", str(port)])
      else:
        proc = subprocess.Popen(["killSalomeWithPort.py", str(port)])
      proc.communicate()
    return 0
  #

  def _killAll(self, unused=None):
    sys.argv = ['killAll']
    import setenv
    setenv.main(True)
    if os.getenv("NSHOST") == "no_host":
      os.unsetenv("NSHOST")
    try:
      import PortManager # mandatory
      import subprocess
      ports = PortManager.getBusyPorts()['this']

      if ports:
        for port in ports:
          if sys.platform == "win32":
            proc = subprocess.Popen([os.getenv("PYTHONBIN"), "-m", "killSalomeWithPort", str(port)])
          else:
            proc = subprocess.Popen(["killSalomeWithPort.py", str(port)])
          proc.communicate()
    except ImportError:
      # :TODO: should be declared obsolete
      from killSalome import killAllPorts
      killAllPorts()
      pass
    from addToKillList import killList
    killList()
    return 0
  #

  def _runTests(self, args=None):
    if args is None:
      args = []
    sys.argv = ['runTests']
    import setenv
    setenv.main(True)

    import runTests
    return runTests.runTests(args, exe="salome test")
  #

  def _showSoftwareVersions(self, softwares=None):
    config = configparser.SafeConfigParser()
    absoluteAppliPath = os.getenv('ABSOLUTE_APPLI_PATH')
    filename = os.path.join(absoluteAppliPath, "sha1_collections.txt")
    versions = {}
    max_len = 0
    with open(filename) as f:
      for line in f:
        try:
          software, version, sha1 = line.split()
          versions[software.upper()] = version
          if len(software) > max_len:
            max_len = len(software)
        except Exception:
          pass
        pass
      pass
    if softwares:
      for soft in softwares:
        if soft.upper() in versions:
          print(soft.upper().rjust(max_len), versions[soft.upper()])
    else:
      import collections
      od = collections.OrderedDict(sorted(versions.items()))
      for name, version in od.items():
        print(name.rjust(max_len), versions[name])
    pass

  def _showInfo(self, args=None):
    if args is None:
      args = []

    usage = "Usage: salome info [options]"
    epilog  = """\n
Display some information about SALOME.\n
Available options are:
    -p,--ports                     Show the list of busy ports (running SALOME instances).
    -s,--softwares [software(s)]   Show the list and versions of SALOME softwares.
                                   Software names must be separated by blank characters.
                                   If no software is given, show version of all softwares.
    -v,--version                   Show running SALOME version.
    -h,--help                      Show this message.
"""
    if not args:
      args = ["--version"]

    if "-h" in args or "--help" in args:
      print(usage + epilog)
      return 0

    if "-p" in args or "--ports" in args:
      import PortManager
      ports = PortManager.getBusyPorts()
      this_ports = ports['this']
      other_ports = ports['other']
      if this_ports or other_ports:
          print("SALOME instances are running on the following ports:")
          if this_ports:
              print("   This application:", this_ports)
          else:
              print("   No SALOME instances of this application")
          if other_ports:
              print("   Other applications:", other_ports)
          else:
              print("   No SALOME instances of other applications")
      else:
          print("No SALOME instances are running")

    if "-s" in args or "--softwares" in args:
      if "-s" in args:
        index = args.index("-s")
      else:
        index = args.index("--softwares")
      indexEnd=index+1
      while indexEnd < len(args) and args[indexEnd][0] != "-":
        indexEnd = indexEnd + 1
      self._showSoftwareVersions(softwares=args[index+1:indexEnd])

    if "-v" in args or "--version" in args:
      print("Running with python", platform.python_version())
      return self._sessionless(["--version"])

    return 0
  #

  def _showDoc(self, args=None):
    if args is None:
      args = []

    modules = args
    if not modules:
      print("Module(s) not provided to command: salome doc <module(s)>")
      return 1

    appliPath = os.getenv("ABSOLUTE_APPLI_PATH")
    if not appliPath:
      raise SalomeContextException("Unable to find application path. Please check that the variable ABSOLUTE_APPLI_PATH is set.")
    baseDir = os.path.join(appliPath, "share", "doc", "salome")
    for module in modules:
      docfile = os.path.join(baseDir, "gui", module.upper(), "index.html")
      if not os.path.isfile(docfile):
        docfile = os.path.join(baseDir, "tui", module.upper(), "index.html")
      if not os.path.isfile(docfile):
        docfile = os.path.join(baseDir, "dev", module.upper(), "index.html")
      if os.path.isfile(docfile):
        out, err = subprocess.Popen(["xdg-open", docfile]).communicate()
      else:
        print("Online documentation is not accessible for module:", module)

  def _usage(self, unused=None):
    usage()
  #

  def _makeCoffee(self, unused=None):
    print("                        (")
    print("                          )     (")
    print("                   ___...(-------)-....___")
    print("               .-\"\"       )    (          \"\"-.")
    print("         .-\'``\'|-._             )         _.-|")
    print("        /  .--.|   `\"\"---...........---\"\"`   |")
    print("       /  /    |                             |")
    print("       |  |    |                             |")
    print("        \\  \\   |                             |")
    print("         `\\ `\\ |                             |")
    print("           `\\ `|            SALOME           |")
    print("           _/ /\\            4 EVER           /")
    print("          (__/  \\             <3            /")
    print("       _..---\"\"` \\                         /`\"\"---.._")
    print("    .-\'           \\                       /          \'-.")
    print("   :               `-.__             __.-\'              :")
    print("   :                  ) \"\"---...---\"\" (                 :")
    print("    \'._               `\"--...___...--\"`              _.\'")
    print("      \\\"\"--..__                              __..--\"\"/")
    print("       \'._     \"\"\"----.....______.....----\"\"\"     _.\'")
    print("          `\"\"--..,,_____            _____,,..--\"\"`")
    print("                        `\"\"\"----\"\"\"`")
    print("")
    print("                    SALOME is working for you; what else?")
    print("")
  #

  def _getCar(self, unused=None):
    print("                                              _____________")
    print("                                  ..---:::::::-----------. ::::;;.")
    print("                               .\'\"\"\"\"\"\"                  ;;   \\  \":.")
    print("                            .\'\'                          ;     \\   \"\\__.")
    print("                          .\'                            ;;      ;   \\\\\";")
    print("                        .\'                              ;   _____;   \\\\/")
    print("                      .\'                               :; ;\"     \\ ___:\'.")
    print("                    .\'--...........................    : =   ____:\"    \\ \\")
    print("               ..-\"\"                               \"\"\"\'  o\"\"\"     ;     ; :")
    print("          .--\"\"  .----- ..----...    _.-    --.  ..-\"     ;       ;     ; ;")
    print("       .\"\"_-     \"--\"\"-----\'\"\"    _-\"        .-\"\"         ;        ;    .-.")
    print("    .\'  .\'   SALOME             .\"         .\"              ;       ;   /. |")
    print("   /-./\'         4 EVER <3    .\"          /           _..  ;       ;   ;;;|")
    print("  :  ;-.______               /       _________==.    /_  \\ ;       ;   ;;;;")
    print("  ;  / |      \"\"\"\"\"\"\"\"\"\"\".---.\"\"\"\"\"\"\"          :    /\" \". |;       ; _; ;;;")
    print(" /\"-/  |                /   /                  /   /     ;|;      ;-\" | ;\';")
    print(":-  :   \"\"\"----______  /   /              ____.   .  .\"\'. ;;   .-\"..T\"   .")
    print("\'. \"  ___            \"\":   \'\"\"\"\"\"\"\"\"\"\"\"\"\"\"    .   ; ;    ;; ;.\" .\"   \'--\"")
    print(" \",   __ \"\"\"  \"\"---... :- - - - - - - - - \' \'  ; ;  ;    ;;\"  .\"")
    print("  /. ;  \"\"\"---___                             ;  ; ;     ;|.\"\"")
    print(" :  \":           \"\"\"----.    .-------.       ;   ; ;     ;:")
    print("  \\  \'--__               \\   \\        \\     /    | ;     ;;")
    print("   \'-..   \"\"\"\"---___      :   .______..\\ __/..-\"\"|  ;   ; ;")
    print("       \"\"--..       \"\"\"--\"        m l s         .   \". . ;")
    print("             \"\"------...                  ..--\"\"      \" :")
    print("                        \"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"    \\        /")
    print("                                               \"------\"")
    print("")
    print("                                Drive your simulation properly with SALOME!")
    print("")
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
    context = pickle.loads(sys.argv[1].encode('latin1'))
    args = pickle.loads(sys.argv[2].encode('latin1'))

    status = context._startSalome(args)
    sys.exit(status)
  else:
    usage()
#
