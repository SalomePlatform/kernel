#! /usr/bin/env python

import os
import sys

# Preliminary work to initialize path to SALOME Python modules
def __initialize():
  currentPath = os.path.dirname(__file__)
  homePath = os.path.realpath(os.environ['HOME'])
  appliPath = os.path.relpath(currentPath, homePath)
  absoluteAppliPath = homePath+os.sep+appliPath
  os.environ['APPLI'] = appliPath # needed to convert .sh environment files
  os.environ['ABSOLUTE_APPLI_PATH'] = absoluteAppliPath

  sys.path[:0] = [absoluteAppliPath+'/bin/salome']
# End of preliminary work

def __getConfigFileNamesDefault():
  import glob

  absoluteAppliPath = os.environ['ABSOLUTE_APPLI_PATH']
  envdDir = absoluteAppliPath + '/env.d'
  if os.path.isdir(envdDir):
    configFileNames = glob.glob(envdDir+os.sep+'*.cfg') + glob.glob(envdDir+os.sep+'*.sh')
    configFileNames = [os.path.basename(x) for x in configFileNames]
  else:
    configFileNames = []

  configFileNames = [envdDir+'/'+x for x in configFileNames]
  return configFileNames
#

def __getConfigFileNames(args):
  # special case: configuration files are provided by user
  # Search for command-line argument(s) --config=file1,file2,..., filen
  configOptionPrefix = "--config="
  configArgs = [ str(x) for x in args if str(x).startswith(configOptionPrefix) ]

  if len(configArgs) == 0:
    return __getConfigFileNamesDefault(), args

  args = [ x for x in args if not x.startswith(configOptionPrefix) ]
  files = [ x.replace(configOptionPrefix, '') for x in configArgs ]
  configFileNames = []
  for x in files:
    configFileNames += x.split(',')

  return configFileNames, args
#

if __name__ == "__main__":
  args = sys.argv[1:]

  # Identify application path then locate configuration files
  __initialize()
  configFileNames, args = __getConfigFileNames(args)

  # Create a SalomeRunner which parses configFileNames to initialize environment
  from salomeRunner import SalomeRunner
  runner = SalomeRunner(configFileNames)


  # Here set specific variables, if needed
  # runner.addToPath('mypath')
  # runner.addToLdLibraryPath('myldlibrarypath')
  # runner.addToPythonPath('mypythonpath')
  # runner.setEnviron('myvarname', 'value')


  # Start SALOME, parsing command line arguments
  runner.go(args)

  print 'Thank you for using SALOME!'
#
