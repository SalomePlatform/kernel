"""
This module replaces the standard import mechanism with one
that filters some imports that can't be done more than once.

This is related to the multi study feature that is implemented
by using the Python multi interpreter feature.
Some modules register objects or classes by calling modules
implemented in C. These operations can't be done multiple times.
So it's very important to control these imports.

Examples:
  - PyQt : import qt calls a C module to register classes
  - OmniORB : import *_idl calls a C module to register CORBA interfaces

Usage:
  - First import the module : import import_hook. This module will
    replace the original importer mechanism

  - Next register the module names or pattern names to filter out::
     import_hook.register_name("a")
     import_hook.register_pattern(pattern)

    where pattern is a function with one parameter, the module name
    to be imported, that returns true or false depending if this module is
    to be filtered or not.

  - Then it's done

IMPORTANT : Every subinterpretor has its own import_hook module. import_hook is not shared among subinterpretors.
The mechanism only works if shared_imported and pattern are shared between all subinterpretors.
This is done by calling init_shared_modules().
  
"""
import sys, imp, __builtin__

# Keep in shared_imported a copy of dictionnary modules
# that need to be imported only once in multi-study context
shared_imported={}

# patterns contains functions that returns 1 or 0 depending if 
# the module name (argument) must be filtered out or not
# These functions are added by calling register_pattern
patterns=[]

original_import=__builtin__.__import__

def register_name(name):
    if shared_imported.has_key(name):return
    shared_imported[name]=None

def register_pattern(pattern):
    patterns.append(pattern)

def is_shared(name):
    if shared_imported.has_key(name):return 1
    for pattern in patterns:
        if pattern(name) : return 1
    return 0

def get_shared_imported(name):
    return shared_imported.get(name)
def set_shared_imported(name,module):
    shared_imported[name]=module
    #print "Module %s shared registered" % name

def get_shared_imported_with_copy(name):
    module_dict= shared_imported.get(name)
    m=imp.new_module(name)
    m.__dict__.update(module_dict)
    return m
def set_shared_imported_with_copy(name,module):
    shared_imported[name]=module.__dict__.copy()
    #print "Module %s shared registered" % name

def import_hook(name, globals=None, locals=None, fromlist=None):
    module=get_shared_imported(name)
    if module:
       sys.modules[name]=module
       return module

    module= original_import(name, globals, locals, fromlist)

    if is_shared(name):
       set_shared_imported(name,module)
    return module

original_reload=__builtin__.reload

def reload_hook(module):
    if is_shared(module.__name__):
       return module
    return original_reload(module)

__builtin__.__import__=import_hook
# Reload is not replaced 
#__builtin__.reload=reload_hook

def init_shared_modules(shared_module):
    global shared_imported, patterns
    shared_imported=shared_module.shared_imported
    patterns=       shared_module.patterns
    shared_imported["salome_shared_modules"]=shared_module
    import salome_shared_modules
    for m in salome_shared_modules.list_modules:
        m.init_shared_modules()
