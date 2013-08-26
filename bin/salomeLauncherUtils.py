#! /usr/bin/env python

import os
import sys
import glob
import subprocess

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

def getConfigFileNames(args):
  # special case: configuration files are provided by user
  # Search for command-line argument(s) --config=file1,file2,..., filen
  # Search for command-line argument(s) --config=dir1,dir2,..., dirn
  configOptionPrefix = "--config="
  configArgs = [ str(x) for x in args if str(x).startswith(configOptionPrefix) ]

  if len(configArgs) == 0:
    return __getConfigFileNamesDefault(), args

  args = [ x for x in args if not x.startswith(configOptionPrefix) ]
  allLists = [ x.replace(configOptionPrefix, '') for x in configArgs ]

  configFileNames = []
  for currentList in allLists:
    elements = currentList.split(',')
    for elt in elements:
      elt = os.path.realpath(os.path.expanduser(elt))
      if os.path.isdir(elt):
        configFileNames += __listDirectory(elt)
      else:
        configFileNames += [elt]

  return configFileNames, args
#

# Return an array of dictionaries {script_name: [list_of_its_args]}
def getScriptsAndArgs(args=[]):
  # Syntax of args: script.py [args:a1,a2=val,an] ... script.py [args:a1,a2=val,an]
  scriptArgs = []
  currentScript = None
  argsPrefix = "args:"
  callPython = False

  for i in range(len(args)):
    elt = args[i]

    if elt.startswith(argsPrefix):
      if not currentScript or callPython:
        raise SalomeRunnerException("args list must follow corresponding script file in command line.")
      elt = elt.replace(argsPrefix, '')
      scriptArgs[len(scriptArgs)-1][currentScript] = elt.split(",")
      currentScript = None
      callPython = False
    elif elt.startswith("python"):
      callPython = True
    elif os.path.isfile(elt) or os.path.isfile(elt+".py"):
      if elt[-3:] == ".py":
        currentScript = os.path.abspath(elt)
      else:
        currentScript = os.path.abspath(elt+".py")
      if callPython:
        scriptArgs.append({"python "+currentScript:[]})
        callPython = False
      else:
        if not os.access(currentScript, os.X_OK):
          raise SalomeRunnerException("Argument %s cannot be executed (please check file permissions)"%currentScript)
        scriptArgs.append({currentScript:[]})
    else:
      raise SalomeRunnerException("Incorrect syntax in command line: %s:\n\t%s"%(elt," ".join(args)))
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
