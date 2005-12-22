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
from omniORB import CORBA

register_name("omniORB")
import omniORB

register_name("CosNaming")
import CosNaming

register_name("omnipatch")
import omnipatch

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

