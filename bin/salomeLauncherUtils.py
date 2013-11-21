#! /usr/bin/env python

import os
import sys
import glob
import subprocess
import re

"""
Define a specific exception class to manage exceptions related to SalomeRunner
"""
class SalomeRunnerException(Exception):
  """Report error messages to the user interface of SalomeRunner."""
#

def __listDirectory(path):
  allFiles = []
  for root, dirs, files in os.walk(path):
    configFileNames = glob.glob(os.path.join(root,'*.cfg')) + glob.glob(os.path.join(root,'*.sh'))
    allFiles += configFileNames
  return allFiles
#

def __getConfigFileNamesDefault():
  absoluteAppliPath = os.getenv('ABSOLUTE_APPLI_PATH','')
  if not absoluteAppliPath:
    return []

  envdDir = absoluteAppliPath + '/env.d'
  if not os.path.isdir(envdDir):
    return []

  return __listDirectory(envdDir)
#

def getConfigFileNames(args, checkExistence=False):
  # special case: configuration files are provided by user
  # Search for command-line argument(s) --config=file1,file2,..., filen
  # Search for command-line argument(s) --config=dir1,dir2,..., dirn
  configOptionPrefix = "--config="
  configArgs = [ str(x) for x in args if str(x).startswith(configOptionPrefix) ]

  if len(configArgs) == 0:
    return __getConfigFileNamesDefault(), args, []

  args = [ x for x in args if not x.startswith(configOptionPrefix) ]
  allLists = [ x.replace(configOptionPrefix, '') for x in configArgs ]

  configFileNames = []
  unexisting = []
  for currentList in allLists:
    elements = currentList.split(',')
    for elt in elements:
      elt = os.path.realpath(os.path.expanduser(elt))
      if os.path.isdir(elt):
        configFileNames += __listDirectory(elt)
      else:
        if checkExistence and not os.path.isfile(elt):
          unexisting += [elt]
        else:
          configFileNames += [elt]

  return configFileNames, args, unexisting
#

# Return an array of dictionaries {script_name: [list_of_its_args]}
def getScriptsAndArgs(args=[]):
  # Syntax of args: script.py [args:a1,a2=val,an] ... script.py [args:a1,a2=val,an]
  scriptArgs = []
  currentKey = None
  argsPrefix = "args:"
  callPython = False
  currentScript = None

  for i in range(len(args)):
    elt = args[i]

    if elt.startswith(argsPrefix):
      if not currentKey or callPython:
        raise SalomeRunnerException("args list must follow corresponding script file in command line.")
      elt = elt.replace(argsPrefix, '')
      scriptArgs[len(scriptArgs)-1][currentKey] = elt.split(",")
      currentKey = None
      callPython = False
    elif elt.startswith("python"):
      callPython = True
    elif os.path.isfile(elt) or os.path.isfile(elt+".py"):
      if elt[-4:] != ".hdf":
        if elt[-3:] == ".py":
          currentScript = os.path.abspath(elt)
        elif os.path.isfile(elt+".py"):
          currentScript = os.path.abspath(elt+".py")
        else:
          currentScript = os.path.abspath(elt) # python script not necessary has .py extension
        pass
      if currentScript and callPython:
        currentKey = "python "+currentScript
        scriptArgs.append({currentKey:[]})
        callPython = False
      elif currentScript:
        if not os.access(currentScript, os.X_OK):
          currentKey = "python "+currentScript
          scriptArgs.append({currentKey:[]})
        else:
          ispython = False
          try:
            fn = open(currentScript)
            for i in xrange(10): # read only 10 first lines 
              ln = fn.readline()
              if re.search("#!.*python"): 
                ispython = True
                break
              pass
            fn.close()
          except:
            pass
          if not ispython and currentScript[-3:] == ".py":
            currentKey = "python "+currentScript
          else:
            currentKey = currentScript
            pass
          scriptArgs.append({currentKey:[]})
  # end for loop
  return scriptArgs
#

# Formatting scripts and args as a Bash-like command-line:
# script1.py [args] ; script2.py [args] ; ...
def formatScriptsAndArgs(scriptArgs=[]):
    commands = []
    for sc_dict in scriptArgs:
      for script, sc_args in sc_dict.items(): # single entry
        cmd = script
        if sc_args:
          cmd = cmd + " " + " ".join(sc_args)
        commands.append(cmd)

    command = "; ".join(["%s"%x for x in commands])
    return command
#

# Ensure OMNIORB_USER_PATH is defined. This variable refers to a the folder in which
# SALOME will write omniOrb configuration files.
# If OMNIORB_USER_PATH is already set, only checks write access to associated directory ;
# an exception is raised if check fails. It allows users for choosing a specific folder.
# Else the function sets OMNIORB_USER_PATH this way:
# - If APPLI environment variable is set, OMNIORB_USER_PATH is set to ${APPLI}/USERS.
#   The function does not check USERS folder existence or wrute access. This folder
#   must exist ; this is the case if SALOME virtual application has been create using
#   appli_gen.py script.
# - Else OMNIORB_USER_PATH is set to user home directory.
def setOmniOrbUserPath():
  omniorbUserPath = os.getenv("OMNIORB_USER_PATH")
  if omniorbUserPath:
    if not os.access(omniorbUserPath, os.W_OK):
      raise Exception("Unable to get write access to directory: %s"%omniorbUserPath)
    pass
  else:
    homePath = os.path.realpath(os.path.expanduser('~'))
    #defaultOmniorbUserPath = os.path.join(homePath, ".salomeConfig/USERS")
    defaultOmniorbUserPath = homePath
    if os.getenv("APPLI"):
      defaultOmniorbUserPath = os.path.join(homePath, os.getenv("APPLI"), "USERS")
      pass
    os.environ["OMNIORB_USER_PATH"] = defaultOmniorbUserPath
#
