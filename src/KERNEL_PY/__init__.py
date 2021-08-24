#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2021  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

#  File   : salome.py renamed as __init__.py for python packaging (gboulant)
#  Author : Paul RASCLE, EDF
#  Module : SALOME
#
""" 
Module salome gives access to Salome resources.

variables:

  - salome.orb             : CORBA
  - salome.naming_service  : instance of naming Service class
      - methods:
          - Resolve(name)  : find a CORBA object (ior) by its pathname
          - Register(name) : register a CORBA object under a pathname

  - salome.lcc             : instance of lifeCycleCORBA class
      - methods:
          - FindOrLoadComponent(server,name) :
                           obtain an Engine (CORBA object)
                           or launch the Engine if not found,
                           with a Server name and an Engine name

  - salome.sg              : salome object to communicate with the graphical user interface (if any)
      - methods:
         - updateObjBrowser():

         - SelectedCount():      returns number of selected objects
         - getSelected(i):       returns entry of selected object number i
         - getAllSelected():     returns list of entry of selected objects
         - AddIObject(Entry):    select an existing Interactive object
         - RemoveIObject(Entry): remove object from selection
         - ClearIObjects():      clear selection

         - Display(*Entry):
         - DisplayOnly(Entry):
         - Erase(Entry):
         - DisplayAll():
         - EraseAll():

         - IDToObject(Entry):    returns CORBA reference from entry

  - salome.myStudyName     : active Study Name
  - salome.myStudy         : the active Study itself (CORBA ior)
      - methods : defined in SALOMEDS.idl

"""
## @package salome
# Module salome gives access to Salome resources.
#
#  \param salome.orb             : CORBA orb object
#  \param salome.naming_service  : instance of naming Service class (SALOME_NamingServicePy::SALOME_NamingServicePy_i)
#  \param salome.lcc             : instance of lifeCycleCORBA class (SALOME_LifeCycleCORBA)
#  \param salome.sg              : Salome object to communicate with the graphical user interface, if running (see interface in salome_iapp::SalomeOutsideGUI)
#  \param salome.myStudyName     : active Study Name
#  \param salome.myStudy         : the active Study (interface SALOMEDS::Study)

#
# ==========================================================================
#
# The function extend_path is used here to aggregate in a single
# virtual python package all the python sub-packages embedded in each
# SALOME modules (python "namespace" pattern).
#
ROOT_PYTHONPACKAGE_NAME="salome"
#
# This root package name is expected to be found as a directory in
# some paths of the sys.path variable, especially the paths
# <MODULE_ROOT_DIR>/lib/pythonX.Y/site-packages/salome where are
# installed the python files. These paths are theorically appended by
# the SALOME main runner and should be in the sys.path at this point
# of the application. The extend_path is looking then for directories
# of the type:
#
# <MODULE_ROOT_DIR>/lib/pythonX.Y/site-packages/salome/<ROOT_PYTHONPACKAGE_NAME>
#
# And append them to the sys.path. These directories are supposed to
# be the pieces to be aggregated as a single virtual python package.
#
import os, sys
from salome_utils import verbose

MATCH_ENDING_PATTERN="site-packages" + os.path.sep + "salome"

def extend_path(pname):
    for dir in sys.path:
        if not isinstance(dir, str) or not os.path.isdir(dir) or not dir.endswith(MATCH_ENDING_PATTERN):
            continue
        subdir = os.path.join(dir, pname)
        # XXX This may still add duplicate entries to path on
        # case-insensitive filesystems
        if os.path.isdir(subdir) and subdir not in __path__:
            if verbose(): print("INFO - The directory %s is appended to sys.path" % subdir)
            __path__.append(subdir)

extend_path(ROOT_PYTHONPACKAGE_NAME)
# ==========================================================================
#

from salome_kernel import *
from salome_study import *
from salome_iapp import *
import salome_study

#
# The next block is workaround for the problem of shared symbols loading for the extension modules (e.g. SWIG-generated)
# that causes RTTI unavailable in some cases. To solve this problem, sys.setdlopenflags() function is used.
# Depending on the Python version and platform, the dlopen flags can be defined in the dl, DLFUN or ctypes module.
# 
import sys
flags = None
if not flags:
    try:
        # dl module can be unavailable
        import dl
        flags = dl.RTLD_NOW | dl.RTLD_GLOBAL
    except:
        pass
    pass
if not flags:
    try:
        # DLFCN module can be unavailable
        import DLFCN
        flags = DLFCN.RTLD_NOW | DLFCN.RTLD_GLOBAL
    except:
        pass
    pass
if not flags:
    try:
        # ctypes module can be unavailable
        import ctypes
        flags = ctypes.RTLD_GLOBAL
    except:
        pass
    pass

# Disable -> bug with scipy, seems very dangerous to do that
#if flags:
#    sys.setdlopenflags(flags)
#    pass

orb, lcc, naming_service, cm, sg, esm, dsm, modulcat = None,None,None,None,None,None,None,None
myStudy, myStudyName = None,None

salome_initial=True

__EMB_SERVANT_ENV_VAR_NAME = "SALOME_EMB_SERVANT"

def standalone():
    import os
    os.environ[__EMB_SERVANT_ENV_VAR_NAME] = "1"
    import KernelBasis
    KernelBasis.setSSLMode(True)

def salome_init(path=None, embedded=False):
    import os
    import KernelBasis
    if __EMB_SERVANT_ENV_VAR_NAME in os.environ:
        KernelBasis.setSSLMode(True)
    #
    if KernelBasis.getSSLMode():
        if KernelBasis.getIOROfEmbeddedNS() == "":
            salome_init_without_session(path, embedded)
        else:
            salome_init_without_session_attached(path, embedded)
    else:
        salome_init_with_session(path, embedded)

class StandAloneLifecyle:
    def __init__(self, containerManager, resourcesManager):
        self._cm = containerManager
        self._rm = resourcesManager

    def FindOrLoadComponent(self,contName,moduleName):
        global orb
        import importlib
        builder_name = moduleName + "_SalomeSessionless"
        moduleObj = importlib.import_module(builder_name)
        result, orb = moduleObj.buildInstance(orb)
        return result
        #raise RuntimeError("Undealed situation cont = {} module = {}".format(contName,moduleName))

    def getContainerManager(self):
      return self._cm

    def getResourcesManager(self):
      return self._rm

def salome_init_without_session_common(path=None, embedded=False):
    global lcc,naming_service,myStudy,orb,modulcat,sg
    import KernelBasis
    KernelBasis.setSSLMode(True)
    import KernelDS
    myStudy = KernelDS.myStudy()
    import CORBA
    orb=CORBA.ORB_init([''])
    import KernelModuleCatalog
    import SALOME_ModuleCatalog
    from salome_kernel import list_of_catalogs_regarding_environement
    modulcat = KernelModuleCatalog.myModuleCatalog( list_of_catalogs_regarding_environement() )
    #
    poa = orb.resolve_initial_references("RootPOA")
    poaManager = poa._get_the_POAManager()
    poaManager.activate()
    #
    sg = salome_iapp_init(embedded)
    salome_study_init_without_session(path)
    #
    from NamingService import NamingService
    naming_service = NamingService()

def salome_init_without_session(path=None, embedded=False):
    salome_init_without_session_common(path,embedded)
    global lcc,cm,dsm,esm
    import KernelLauncher
    cm = KernelLauncher.myContainerManager()
    lcc = StandAloneLifecyle(cm, KernelLauncher.myResourcesManager())
    # activate poaManager to accept co-localized CORBA calls.
    from KernelSDS import GetDSMInstance
    import sys
    dsm = GetDSMInstance(sys.argv)
    # esm inherits from SALOME_CPythonHelper singleton already initialized by GetDSMInstance
    # esm inherits also from SALOME_ResourcesManager creation/initialization (concerning SingleThreadPOA POA) when KernelLauncher.GetContainerManager() has been called
    esm = KernelLauncher.GetExternalServer()
    
def salome_init_without_session_attached(path=None, embedded=False):
    """
    Configuration SSL inside a python interpretor launched in the SALOME_Container_No_NS_Serv.
    In this configuration, 
    """
    salome_init_without_session_common(path,embedded)
    global lcc,cm,dsm,esm
    import CORBA
    orb=CORBA.ORB_init([''])
    import Engines
    import KernelBasis
    nsAbroad = orb.string_to_object( KernelBasis.getIOROfEmbeddedNS() )
    import SALOME
    cm = orb.string_to_object( nsAbroad.Resolve("/ContainerManager").decode() )
    rm = orb.string_to_object( nsAbroad.Resolve("/ResourcesManager").decode() )
    lcc = StandAloneLifecyle(cm,rm)
    dsm = orb.string_to_object( nsAbroad.Resolve("/DataServerManager").decode() )
    esm = orb.string_to_object( nsAbroad.Resolve("/ExternalServers").decode() )

def salome_init_with_session(path=None, embedded=False):
    """
    Performs only once SALOME general purpose initialisation for scripts.
    Provides:
    orb             reference to CORBA
    lcc             a LifeCycleCorba instance
    naming_service  a naming service instance
    cm              reference to the container manager
    esm             reference to external server manager
    dsm             reference to shared dataserver manager
    modulcat        reference to modulecatalog instance
    sg              access to SALOME GUI (when linked with IAPP GUI)
    myStudy         active study itself (CORBA reference)
    myStudyName     active study name
    """
    global salome_initial
    global orb, lcc, naming_service, cm, esm, dsm, modulcat
    global sg
    global myStudy, myStudyName
    import KernelBasis
    KernelBasis.setSSLMode(False)
    try:
        if salome_initial:
            salome_initial=False
            sg = salome_iapp_init(embedded)
            orb, lcc, naming_service, cm, esm, dsm, modulcat = salome_kernel_init()
            myStudy, myStudyName = salome_study_init(path)
            pass
        pass
    except RuntimeError as inst:
        # wait a little to avoid trace mix
        import time
        time.sleep(0.2)
        x = inst
        print("salome.salome_init():", x)
        print("""
        ============================================
        May be there is no running SALOME session
        salome.salome_init() is intended to be used
        within an already running session
        ============================================
        """)
        raise
    
def salome_close():
    global salome_initial, myStudy, myStudyName
    try:
        # study can be clear either from GUI or directly with salome.myStudy.Clear()
        myStudy.Clear()
    except:
        pass
    salome_initial=True
    salome_iapp_close()
    salome_study_close()
    myStudy, myStudyName = None, None
    import KernelBasis
    if KernelBasis.getSSLMode():
        import KernelDS
        KernelDS.KillGlobalSessionInstance()
        import KernelSDS
        KernelSDS.KillCPythonHelper()
    pass

def salome_NS():
    import CORBA
    import CosNaming
    orb = CORBA.ORB_init()
    ns0 = orb.resolve_initial_references("NameService")
    return ns0._narrow(CosNaming.NamingContext)

def salome_walk_on_containers(ns,root):
    import CosNaming
    it = ns.list(0)[1]
    if not it:
        return
    cont = True
    while cont:
        cont,obj = it.next_one()
        if cont:
            if obj.binding_name[0].kind == "object":
                import Engines
                corbaObj = ns.resolve(obj.binding_name)
                if isinstance(corbaObj,Engines._objref_Container):
                    yield corbaObj,(root,obj.binding_name[0].id)
            else:
                father = ns.resolve([obj.binding_name[0]])
                for elt,elt2 in salome_walk_on_containers(father,root+[obj.binding_name[0].id]):
                    yield elt,elt2
            pass
        pass
    pass

def salome_shutdown_containers():
    salome_init()
    ns=salome_NS()
    li = [elt for elt in salome_walk_on_containers(ns,[""])]
    print("Number of containers in NS : {}".format(len(li)))
    for cont,(root,cont_name) in li:
        try:
            cont.Shutdown()
        except:
            pass
        ref_in_ns = "/".join(root+[cont_name])
        naming_service.Destroy_Name(ref_in_ns)
    print("Number of containers in NS after clean : {}".format( len( list(salome_walk_on_containers(ns,[""])) )))

class SessionContextManager:
    def __enter__(self):
        standalone()
        salome_init()
    def __exit__(self, type, value, traceback):
        salome_close()

#to expose all objects to pydoc
__all__=dir()
