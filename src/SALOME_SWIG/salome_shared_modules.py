"""
This module with help of import_hook and *_shared_modules
filters imports when using the embedded Python interpretor.

Some imports can't be done more than once.
This is related to the multi study feature that is implemented
by using the Python multi interpreter feature.
Some modules register objects or classes by calling modules
implemented in C. These operations can't be done multiple times.
So it's very important to control these imports.

Examples:
  - PyQt : import qt calls a C module to register classes
  - OmniORB : import *_idl calls a C module to register CORBA interfaces

Usage:
  - First : the module salome_shared_modules is imported by main Python interpretor.
    It will keep a dictionnary and a list that are shared among all
    the subinterpretors and imports import_hook module that replaces
    the standard import mechanism par one that prevents more than one import
    for some modules identified by name (call register_name) or by a 
    validator (call register_pattern).

  Calls to register_name and register_pattern are done in modules named *_shared_modules
  that could be found in the path SALOMEPATH

"""
import glob,os,sys

import import_hook
# shared_imported, patterns, register_name, register_pattern
# will be shared by all Python sub interpretors
from import_hook import shared_imported
from import_hook import patterns
from import_hook import register_name
from import_hook import register_pattern

register_name("salome_shared_modules")

# Get the SALOMEPATH if set or else use KERNEL_ROOT_DIR that should be set.
salome_path=os.environ.get("SALOMEPATH",os.getenv("KERNEL_ROOT_DIR"))

list_modules=[]

# Import all *_shared_modules in the path and store them in list_modules
path=salome_path.split(":")
for rep in path:
    # Import all *_shared_modules in rep
    for f in glob.glob(os.path.join(rep,"lib","python"+sys.version[:3],"site-packages","salome","shared_modules","*_shared_modules.py")):
        try:
           m=__import__(os.path.splitext(os.path.basename(f))[0])
           list_modules.append(m)
        except:
           pass

# 
# If shared modules have been imported before installing import mechanism
# we add them to shared_imported
#
for name,module in sys.modules.items():
    if import_hook.is_shared(name) and shared_imported.get(name) is None:
       #print "Module shared added to shared_imported: ",name
       shared_imported[name]=module

