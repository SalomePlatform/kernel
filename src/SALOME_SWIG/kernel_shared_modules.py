"""

"""
import glob,os,sys

import import_hook
from import_hook import register_name
from import_hook import register_pattern

register_name("qt")
register_pattern(lambda(x):x.endswith("_idl"))

register_name("omniORB")
register_name("CosNaming")

register_name("Engines")
register_name("SALOME")
register_name("SALOMEDS")
register_name("SALOME_ModuleCatalog")

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
from SALOME_utilities import MESSAGE
#
# We search all Python CORBA (omniorb) modules.
# A Python CORBA module has 2 associated Python packages 
# These packages are named : <module_name> and <module_name>__POA
#
# That module is normally installed in shared_modules
# So we should find CORBA shared modules in ..
repertoire=os.path.join(os.path.dirname(__file__),'..')
path=[repertoire,]
#
for rep in path:
   # Add rep directory in the Python path to be able to import modules 
   listdir=glob.glob(os.path.join(rep,"*__POA"))
   for elem in listdir:
      if os.path.isdir(elem):
         # Found a directory (Python package) named *__POA 
         module__POA=os.path.basename(elem)
         module=module__POA[:-5]
         MESSAGE( "Import CORBA module: " + module + ".\n Directory: " + os.path.abspath(elem)[:-5] )
         register_name(module)

   # Now we import modules found in shared_modules directory
   r=os.path.join(rep,"shared_modules")
   if os.path.isdir(r):
      listfich=glob.glob(os.path.join(r,"*.py"))
      MESSAGE( str(listfich) )
      for m in listfich:
         module=os.path.basename(m)[:-3]
         MESSAGE( "Import module: " + module + ".\n Location: " + os.path.abspath(m) )
         register_name(module)


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

