"""

"""
import import_hook

import glob,os,sys,string,imp

from import_hook import register_name
from import_hook import register_pattern

register_name("qt")
register_name("libSALOME_LifeCycleCORBA")
register_pattern(lambda(x):x.endswith("_idl"))
register_pattern(lambda(x):x.endswith("_Swig"))

register_name("CORBA")
import CORBA

register_name("omniORB")
import omniORB

register_name("CosNaming")
import CosNaming

# Modify omniORB to use right sys.modules dictionnary 
# with multi-interpreter feature
# openModule and newModule are functions of omniORB/__init__.py module
# modified to register modules to share
# Function to return a Python module for the required IDL module name
def openModule(mname, fname=None):
    # Salome modification start
    import sys
    # Salome modification end

    if mname == "CORBA":
        mod = sys.modules["omniORB.CORBA"]
    elif sys.modules.has_key(mname):
        mod = sys.modules[mname]
    else:
        mod = newModule(mname)

    # Salome modification start
    import_hook.set_shared_imported(mname,mod)
    # Salome modification end


    if not hasattr(mod, "__doc__") or mod.__doc__ is None:
        mod.__doc__ = "omniORB IDL module " + mname + "\n\n" + \
                      "Generated from:\n\n"

    if fname is not None:
        mod.__doc__ = mod.__doc__ + "  " + fname + "\n"

    return mod

# Function to create a new module, and any parent modules which do not
# already exist
def newModule(mname):
    # Salome modification start
    import sys
    # Salome modification end

    mlist   = string.split(mname, ".")
    current = ""
    mod     = None

    for name in mlist:
        current = current + name

        if sys.modules.has_key(current):
            mod = sys.modules[current]
        else:
            newmod = imp.new_module(current)
            if mod: setattr(mod, name, newmod)
            sys.modules[current] = mod = newmod

        current = current + "."

    return mod
# Replace openModule and newModule by modified ones
# to take into account the sys.modules that matches
# the right one (multi-interpreter feature)
omniORB.openModule=openModule
omniORB.newModule=newModule

# BE CAREFUL
# Engines, SALOME, SALOMEDS must be imported in that order because :
# Engines imports SALOME_Component_idl
# SALOME imports SALOME_Session_idl and SALOME_Exception_idl which imports SALOME_Component_idl
# and SALOMEDS imports SALOMEDS_idl and SALOMEDS_Attributes_idl which imports SALOME_Exception_idl
# If SALOME is imported before Engines, that module would not be completely imported
import Engines
import SALOME
import SALOMEDS
import SALOME_ModuleCatalog

def init_shared_modules():
   """
      This function initializes shared modules that need to be
   """
   # EDF-CCAR:
   # Problem with omniORB : omniORB creates a C Python module named  _omnipy
   # this module has sub-modules : omni_func, ...
   # _omnipy is quite a package but import with Python sub-interpreters does not seem to work
   # To make it work we need to add those sub-modules in sys.modules
   import sys
   import _omnipy
   sys.modules["_omnipy.omni_func"]=_omnipy.omni_func
   sys.modules["_omnipy.poa_func"]=_omnipy.poa_func
   sys.modules["_omnipy.poamanager_func"]=_omnipy.poamanager_func
   sys.modules["_omnipy.orb_func"]=_omnipy.orb_func

