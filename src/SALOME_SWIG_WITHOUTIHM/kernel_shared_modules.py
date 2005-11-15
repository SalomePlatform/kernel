"""

"""
import import_hook

import glob,os,sys,string,imp

from import_hook import register_name
from import_hook import register_pattern

register_name("qt")
register_pattern(lambda(x):x.endswith("_idl"))
register_pattern(lambda(x):x.endswith("_Swig"))

register_name("omniORB")
import omniORB

register_name("omnipatch")
import omnipatch

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

