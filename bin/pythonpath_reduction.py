#!/usr/bin/env python3
# -*- coding: utf-8 *-

############################################################
# This script reduce the pythonpath of salome environement.#
# The single input of this script is install path of all   #
# salome prerequisites                                     #
############################################################

import os,sys,fnmatch,shutil,glob
from parseConfigFile import parseConfigFile

def remove(path):
    if os.path.isdir(path):
        shutil.rmtree(path)
    else:
        os.remove(path)

def copy(src,dst):
    if os.path.exists(dst):
        remove(dst)
    if os.path.isdir(src):
        shutil.copytree(src,dst)
    else:
        shutil.copyfile(src,dst)

if sys.version_info[0] < 3:
    raise Exception("Must be using Python 3")

# The single input of this script is install path of all salome prerequisites
# Create the new python module folder on the same directory of prerequisites folder (salome install path ) 
prerequis_install_dir = sys.argv[1]
while(prerequis_install_dir[-1] == '/'):
    prerequis_install_dir = prerequis_install_dir[:-1]
salome_install_dir = os.path.dirname(prerequis_install_dir)

#new pythonpath initiation; creation a directory containing all python module for salome
pythonpath_common = os.path.join(salome_install_dir,'python_modules')
if os.path.exists(pythonpath_common):
    remove(pythonpath_common)
os.mkdir(pythonpath_common)
refDir = os.path.abspath(os.path.dirname(__file__))
shutil.copy(os.path.join(sys.path[-1],"setuptools/site-patch.py"),os.path.join(pythonpath_common,"site.py"))
new_pythonpath_list = []
new_pythonpath_list.append(pythonpath_common)

#parsing configuration file (cfg) to get old pythonpath
reserved_key=['PYTHONPATH']
context_file = os.path.join(salome_install_dir,'salome_context.cfg')
context_file_sh = os.path.join(salome_install_dir,'salome_prerequisites.sh')
if not os.path.exists(context_file) or not os.path.exists(context_file_sh):
    raise Exception("salome_context files are not found in the salome install. Verify if salome_context.cfg and salome_prerequisistes.sh are contained in the salome install path: %s" %salome_install_dir)
configInfo = parseConfigFile(context_file, reserved_key)
configVars = configInfo.outputVariables
reservedDict = configInfo.reservedValues
configVarsDict = {k:v for (k,v) in configVars}

# Synchronize new pythonpath with the old
# Copy all the .egg files to the new directory of python module and list them in new single easy-install.pth
# If the module hasn't .egg file, we must copy all files from its pythonpath and paste them in the python module directory
# It's complicated to reduce pythonpath of paraview so for now we keep the old paraview pythonpath
pattern = "*.egg"
with open(os.path.join(pythonpath_common,'easy-install.pth'), 'w') as easy_install:
    for dirs in reservedDict[reserved_key[0]]:
        for d in dirs.split(':'):
            if "paraview" not in d.lower():
               egg_dir_list = glob.glob(os.path.join(d,"*.egg"))
               if egg_dir_list:
                   for egg_dir in egg_dir_list:
                       egg_file = egg_dir.split('/')[-1]
                       new_dir = os.path.join(pythonpath_common,egg_file )
                       copy(egg_dir,new_dir)
                       easy_install.write("./%s\n" %egg_file)
               else:
                   for f in os.listdir(d):
                       full_file_srcpath = os.path.join(d,f)
                       full_file_dstpath = os.path.join(pythonpath_common,f)
                       copy(full_file_srcpath,full_file_dstpath)
            else:
                new_pythonpath_list.append(d)
# In the case of matplotlib, some prerequis are found matplotlib_root. They also need to be put in new directory of module python
try:
    with open(os.path.join(pythonpath_common,'easy-install.pth'), 'a') as easy_install:
        MATPLOTLIB_ROOT = configVarsDict['MATPLOTLIB_HOME']
        for d in os.listdir(MATPLOTLIB_ROOT):
            if ".egg" in d:
               copy(os.path.join(MATPLOTLIB_ROOT,d),os.path.join(pythonpath_common,d))
               easy_install.write("./%s\n" %d)
except:
    print("Not found MATPLOTLIB_HOME variable in the .cfg")
    print("Matplotlib isn't compiled with salome or maybe it is in the system package ")
    print("If matplotlib is compiled with salome, we need define its install path with variable MATPLOTLIB_HOME")

# Overwrite salome_context.cfg
with open(context_file, 'r') as f:
    lines = f.readlines()
with open(context_file ,'w') as f:
    for l in lines:
        if "ADD_TO_PYTHONPATH" not in l:
            f.write(l)
with open(context_file,'a') as f:
    f.write("#------ NEWPYTHONPATH ------\n")
    for p in new_pythonpath_list:
        f.write("ADD_TO_PYTHONPATH: %s\n"%p)

# Overwrite salome_prerequisites.sh
with open(context_file_sh, 'r') as f:
    lines = f.readlines()
with open(context_file_sh ,'w') as f:
    for l in lines:
        if "PYTHONPATH" not in l:
            f.write(l)
with open(context_file_sh,'a') as f:
    f.write("#------ NEWPYTHONPATH ------\n")
    for p in new_pythonpath_list:
        f.write("export PYTHONPATH=%s:${PYTHONPATH}\n"%p)