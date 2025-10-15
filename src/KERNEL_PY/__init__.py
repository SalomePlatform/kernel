#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2025  CEA, EDF, OPEN CASCADE
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
from salome.kernel.salome_utils import verbose
from salome.kernel import version_and_config

__version__ = version_and_config.version

__config_datetime__ = version_and_config.config_datetime

del version_and_config

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

from salome.kernel.salome_kernel_utils import *
from salome.kernel.salome_study import *
from salome.kernel.salome_iapp import *

orb, lcc, naming_service, cm, sg, esm, dsm, logm, modulcat, rm, lm = None,None,None,None,None,None,None,None,None,None,None
myStudy, myStudyName = None,None

salome_initial=True

def standalone():
    pass

def withServers():
    from .. import KernelBasis
    KernelBasis.setSSLMode(False)

def salome_init(path=None, embedded=False, iorfakensfile=None, forced=False):
    """
    Initialize SALOME client process (that can also be server).
    3 modes of initialization exists:
    - SSL mode (see salome_init_without_session)
    - SSL mode attached in the context of python execution inside SALOME_Container_No_NS_Serv server (typically YACS)
    - Classical mode (see salome_init_with_session)
    :param iorfakensfile: filename inside which IOR of fake NS will be written
    :param forced: tell if the multi-initialization protection mecanism of salome_init must be skiped of not
                   (typically in the context where a path to a study is given whereas a previous initialisation without it was done)
    """
    if not forced:
        if lcc is not None:# multi-initialization protection mecanism is based on lcc global var
            return
    PATH_TO_STUDY_FILE_TO_INITIATE = "PATH_TO_STUDY_FILE_TO_INITIATE"
    from .. import KernelBasis
    if KernelBasis.getSSLMode():
        if KernelBasis.getIOROfEmbeddedNS() == "":
            import os
            # make runSalome.py -t study.hdf toto.py
            if path is None and PATH_TO_STUDY_FILE_TO_INITIATE in os.environ:
                path = os.environ[PATH_TO_STUDY_FILE_TO_INITIATE]
            salome_init_without_session(path, embedded, iorfakensfile)
        else:
            salome_init_without_session_attached(path, embedded)
    else:
        salome_init_with_session(path, embedded)

def salome_init_without_session_common(path=None, embedded=False):
    from ..ORBConfigFile_impl import writeORBConfigFileSSL
    OMNIORB_USER_PATH = "OMNIORB_USER_PATH"
    def RemoveOmniorbConfigFile():
        import os
        if "OMNIORB_CONFIG" in os.environ:
            fileToRemove = os.environ["OMNIORB_CONFIG"]
            if os.path.exists(fileToRemove):
                os.unlink(fileToRemove)

    if OMNIORB_USER_PATH in os.environ:
        import atexit
        writeORBConfigFileSSL(os.environ[OMNIORB_USER_PATH],kwargs={"with_pid":True})
        atexit.register(RemoveOmniorbConfigFile)

    global lcc,naming_service,myStudy,myStudyName,orb,modulcat,sg
    from .. import KernelBasis
    KernelBasis.setSSLMode(True)
    from .. import KernelDS
    myStudy = KernelDS.myStudy()
    import CORBA
    orb=CORBA.ORB_init([''])
    from .. import KernelModuleCatalog
    from .. import SALOME_ModuleCatalog
    from ..salome_kernel_utils import list_of_catalogs_regarding_environement
    modulcat = KernelModuleCatalog.myModuleCatalog( list_of_catalogs_regarding_environement() )
    #
    poa = orb.resolve_initial_references("RootPOA")
    poaManager = poa._get_the_POAManager()
    poaManager.activate()
    #
    sg = salome_iapp_init(embedded)
    salome_study_init_without_session(path)
    #
    from ..NamingService import NamingService
    naming_service = NamingService()
    myStudyName = myStudy.Name

def salome_init_without_session(path=None, embedded=False, iorfakensfile=None):
    """
    Force creation of all servants needed by SALOME session in the current process.
    A Fake NamingService is created storing reference of all servants in the current process.
    """
    salome_init_without_session_common(path,embedded)
    global lcc,cm,dsm,esm,rm,logm,lm
    from .. import KernelLauncher
    cm = KernelLauncher.myContainerManager()
    type(cm).SetOverrideEnvForContainersSimple = ContainerManagerSetOverrideEnvForContainersSimple
    rm = KernelLauncher.myResourcesManager()
    type(rm).GetResourceDefinition2 = ResourcesManagerGetResourceDefinition
    from ..LifeCycleCORBA import LifeCycleCORBASSL
    lcc = LifeCycleCORBASSL()
    # create a FactoryServer Container servant
    from .. import KernelContainer
    KernelContainer.myContainer()
    lm = KernelLauncher.myLockMaster()
    # activate poaManager to accept co-localized CORBA calls.
    from ..KernelSDS import GetDSMInstance
    import sys
    if hasattr(sys, 'argv'):
      argv = sys.argv
    else:
      argv = ['']
    dsm = GetDSMInstance(argv)
    # esm inherits from SALOME_CPythonHelper singleton already initialized by GetDSMInstance
    # esm inherits also from SALOME_ResourcesManager creation/initialization (concerning SingleThreadPOA POA) when KernelLauncher.GetContainerManager() has been called
    esm = KernelLauncher.GetExternalServer()
    # idem for logm
    logm = KernelLauncher.myLogManager()
    type(logm).NaiveFetch = LogManagerNaiveFetch
    type(logm).Fetch = LogManagerFetch
    type(logm).DumpInFile = LogManagerDumpInFile
    type(logm).LaunchMonitoringDumpFile = LogManagerLaunchMonitoringDumpFile
    type(logm).FinalFetchBeforeDying = LogManagerFinalFetchBeforeDying
    type(logm).GetLatestMonitoringDumpFile = LogManagerGetLatestMonitoringDumpFile
    type(logm).DumpIORInFile = LogManagerDumpIORInFile
    #
    from .. import KernelLogger
    naming_service.Register(KernelLogger.myLogger(),"/Logger")
    #
    from ..NamingService import NamingService
    if iorfakensfile is not None:
        with open(iorfakensfile,"w") as iorfakensf:
            iorfakensf.write(NamingService.IOROfNS())
    
CM_NAME_IN_NS = "/ContainerManager"
RM_NAME_IN_NS = "/ResourcesManager"
DSM_NAME_IN_NS = "/DataServerManager"
ESM_NAME_IN_NS = "/ExternalServers"
LOGM_NAME_IN_NS = "/LogManager"
LAUNCHER_NAME_IN_NS = "/SalomeLauncher"

def salome_init_without_session_attached(path=None, embedded=False):
    """
    Configuration SSL inside a python interpretor launched in the SALOME_Container_No_NS_Serv.
    In this configuration, a local FakeNamingService is created and remote objects are stored in it.
    lcc is pointing to the FakeNamingService above.
    """
    salome_init_without_session_common(path,embedded)
    global lcc,cm,dsm,esm,rm,logm,lm
    import CORBA
    orb=CORBA.ORB_init([''])
    from .. import Engines
    from .. import KernelBasis
    from .. import KernelLauncher
    nsAbroad = orb.string_to_object( KernelBasis.getIOROfEmbeddedNS() )
    from .. import SALOME_CMOD
    cm = orb.string_to_object( nsAbroad.Resolve(CM_NAME_IN_NS).decode() )
    type(cm).SetOverrideEnvForContainersSimple = ContainerManagerSetOverrideEnvForContainersSimple
    naming_service.Register(cm,CM_NAME_IN_NS)
    rm = orb.string_to_object( nsAbroad.Resolve(RM_NAME_IN_NS).decode() )
    type(rm).GetResourceDefinition2 = ResourcesManagerGetResourceDefinition
    naming_service.Register(rm,RM_NAME_IN_NS)
    #
    from ..LifeCycleCORBA import LifeCycleCORBASSL
    lcc = LifeCycleCORBASSL()
    dsm = orb.string_to_object( nsAbroad.Resolve(DSM_NAME_IN_NS).decode() )
    naming_service.Register(dsm,DSM_NAME_IN_NS)
    #
    esm = orb.string_to_object( nsAbroad.Resolve(ESM_NAME_IN_NS).decode() )
    naming_service.Register(esm,ESM_NAME_IN_NS)
    #
    logm = orb.string_to_object( nsAbroad.Resolve(LOGM_NAME_IN_NS).decode() )
    naming_service.Register(logm,LOGM_NAME_IN_NS)
    #
    lm = orb.string_to_object( nsAbroad.Resolve(KernelLauncher.GetLockMasterEntryInNS()).decode() )
    naming_service.Register(lm,KernelLauncher.GetLockMasterEntryInNS())
    #
    sl = orb.string_to_object( nsAbroad.Resolve(LAUNCHER_NAME_IN_NS).decode() )
    naming_service.Register(sl, LAUNCHER_NAME_IN_NS)

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
    from salome.kernel import KernelBasis
    KernelBasis.setSSLMode(False)
    try:
        if salome_initial:
            salome_initial=False
            sg = salome_iapp_init(embedded)
            orb, lcc, naming_service, cm, esm, dsm, modulcat = salome_kernel_init()
            type(cm).SetOverrideEnvForContainersSimple = ContainerManagerSetOverrideEnvForContainersSimple
            myStudy, myStudyName = salome_study_init(path)
            pass
        pass
    except RuntimeError as inst:
        # wait a little to avoid trace mix
        import time
        time.sleep(0.2)
        x = inst
        print("salome.salome_init_with_session():", x)
        print("""
        ============================================
        May be there is no running SALOME session
        salome.salome_init() is intended to be used
        within an already running session
        ============================================
        """)
        raise
    
def salome_close():
    global salome_initial, myStudy, myStudyName, lcc
    try:
        # study can be clear either from GUI or directly with salome.myStudy.Clear()
        myStudy.Clear()
    except Exception:
        pass
    salome_initial=True
    salome_iapp_close()
    salome_study_close()
    myStudy, myStudyName = None, None
    lcc = None # to salome_init to rebuild all in case of salome_init after salome_close
    from .. import KernelBasis
    if KernelBasis.getSSLMode() and not KernelBasis.getGUIMode():
        from .. import KernelDS
        KernelDS.KillGlobalSessionInstance()
        from .. import KernelSDS
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
                from .. import Engines
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

def salome_shutdown_containers_with_session():
    salome_init()
    ns=salome_NS()
    li = [elt for elt in salome_walk_on_containers(ns,[""])]
    print("Number of containers in NS : {}".format(len(li)))
    for cont,(root,cont_name) in li:
        try:
            cont.Shutdown()
        except Exception:
            pass
        ref_in_ns = "/".join(root+[cont_name])
        naming_service.Destroy_Name(ref_in_ns)
    print("Number of containers in NS after clean : {}".format( len( list(salome_walk_on_containers(ns,[""])) )))

def retrieve_containers_in_ns():
    return [elt for elt in naming_service.repr() if "/Containers/" == elt[:12]]

def get_all_containers():
    containersEntries = retrieve_containers_in_ns()
    return [naming_service.Resolve(containerEntry) for containerEntry in containersEntries]
    
def salome_shutdown_containers_without_session():
    for cont in get_all_containers():
        try:
            cont.Shutdown()
        except:
            pass

def salome_shutdown_containers():
    from .. import KernelBasis
    if KernelBasis.getSSLMode():
        salome_shutdown_containers_without_session()
    else:
        salome_shutdown_containers_with_session()

class SessionContextManager:
    def __enter__(self):
        standalone()
        salome_init()
    def __exit__(self, type, value, traceback):
        salome_close()

def ResourcesManagerGetResourceDefinition(self, machine):
    """
    Implementation pickle ready
    """
    ret = self.GetResourceDefinition( machine )
    ret.__class__.__module__ = "salome.kernel.Engines"
    return ret

def ContainerManagerSetOverrideEnvForContainersSimple(self,env):
    from .. import Engines
    envEff = [ Engines.KeyValPairString(key=k,val=v) for k,v in env ]
    return self.SetOverrideEnvForContainers( envEff )

def LogManagerNaiveFetch(self):
    """
    Fetch data from server with multiple CORBA invokations.
    """
    from .. import SALOME_ContainerHelper
    return [SALOME_ContainerHelper.ContainerLogInfoClt(elt) for elt in self.listOfContainerLogs()]

def LogManagerFetch(self,clearMemory = False):
    """
    Fetch data from server in one shot mode.
    """
    from ..SALOME_ContainerHelper import unserializeLogManager
    return unserializeLogManager( self.getAllStruct(clearMemory) )

def LogManagerDumpInFile(self,fileName,clearMemory = False):
    with open(fileName,"wb") as f:
        f.write( self.getAllStruct( clearMemory ) )


class LogManagerLaunchMonitoringFileCtxMgr:
    def __init__(self, intervalInMs, outFileName):
        self._interval_in_ms = intervalInMs
        self._out_filename = outFileName
        self._monitoring_params = None
    def __enter__(self):
        from .. import salome
        self._monitoring_params = salome.logm.LaunchMonitoringDumpFile(self._interval_in_ms, self._out_filename)
        return self._monitoring_params
    def __exit__(self,exctype, exc, tb):
        from .. import SALOME_PyNode
        from .. import salome
        SALOME_PyNode.StopMonitoring( self._monitoring_params )
        salome.logm.GetLatestMonitoringDumpFile()
        pass

def LogManagerLaunchMonitoringDumpFile(self, intervalInMs, outFileName):
    """
    This method loops indefinitely every intervalInMs milliseconds to dump the singleton 
    content of perf log stored in salome.logm.
    This method runs in a dedicated subprocess that can be killed at any time.
    So subprocess code must deal with.

    See also LogManagerGetLatestMonitoringDumpFile
    """
    global orb,logm
    ior = orb.object_to_string( logm )
    import os
    outFileName2 = os.path.abspath( os.path.expanduser(outFileName) )
    import tempfile
    import logging
    from .. import SALOME_PyNode
    from .. import KernelBasis
    # outFileNameSave stores the content of outFileName during phase of dumping
    with tempfile.NamedTemporaryFile(prefix=os.path.basename(outFileName2),dir=os.path.dirname(outFileName2)) as f:
      outFileNameSave = f.name
    with tempfile.NamedTemporaryFile(prefix="htopmain_",suffix=".py") as f:
      tempPyFile = f.name
    with open(tempPyFile,"w") as f:
        f.write("""from salome.kernel import Engines
import os
import shutil
import CORBA
import time
orb=CORBA.ORB_init([''], CORBA.ORB_ID)
logm = orb.string_to_object("{ior}")
outFileName = "{outFileName}"
outFileNameSave = "{outFileNameSave}"
logm.setFileNamePairOfLogger(outFileName, outFileNameSave )
from salome.kernel import salome
while(True):
  if os.path.exists( outFileName ):
    shutil.copy(outFileName,outFileNameSave)
    logm.versionB_IsTheLatestValidVersion()
  salome.LogManagerDumpInFile(logm,outFileName)
  logm.versionA_IsTheLatestValidVersion()
  time.sleep( {intervalInMs} / 1000.0 )
""".format( **locals()))
    logging.debug( "File for monitoring dump file : {}".format(tempPyFile) )
    pyFileName = SALOME_PyNode.FileDeleter( tempPyFile )
    pid = KernelBasis.LaunchMonitoring( tempPyFile )
    return SALOME_PyNode.MonitoringInfo(pyFileName,intervalInMs,None,pid)

def LogManagerDumpIORInFile(self, iorFileName):
    global logm
    with open(iorFileName,"w") as f:
        f.write( orb.object_to_string( logm ) )

def LogManagerLoadFromFile(fileName):
    from salome.kernel.SALOME_ContainerHelper import unserializeLogManager
    with open(fileName,"rb") as f:
        data = f.read()
    return unserializeLogManager( data )

def LogManagerLoadFromIORFile( iorFile ):
    global orb
    def LoadAndWrite(logm,tempFileName):
        from salome.kernel import SALOME_PyNode
        logm.putStructInFileAtomic( False, tempFileName )
        tempFileAuto = SALOME_PyNode.FileDeleter( tempFileName )
        ret = LogManagerLoadFromFile( tempFileAuto.filename )
        return ret
    with open(iorFile,"r") as f:
        ior = f.read()
    from salome.kernel import Engines
    import tempfile
    salome_init_without_session()
    logm = orb.string_to_object( ior )
    with tempfile.NamedTemporaryFile(dir=os.path.expanduser("~")) as f:
        tempFileName = f.name
    return LoadAndWrite( logm, tempFileName )

def LogManagerFinalFetchBeforeDying(self):
    import shutil
    a,b = self.getFileNamePairOfLogger()
    self.DumpInFile( b )
    shutil.move( b, a)

def LogManagerGetLatestMonitoringDumpFile(self):
    import shutil
    import logging
    a,b = self.getFileNamePairOfLogger()
    if a=="" or b=="":
        return ""
    if a == b:
        return a
    lastVersion = self.getLastVersionOfFileNameLogger()
    if lastVersion == a:
        logging.debug("LogManagerGetLatestMonitoringDumpFile SITUATION A")
        if os.path.exists( b ):
            os.remove( b )
        self.FinalFetchBeforeDying()
        return a
    if lastVersion == b:
        logging.debug("LogManagerGetLatestMonitoringDumpFile SITUATION B")
        if os.path.exists( b ):
            shutil.move( b, a)
        self.FinalFetchBeforeDying()
        return a
    logging.warning("in LogManagerGetLatestMonitoringDumpFile an unexpected situation araises.")
    return ""

class Barrier:
  def __init__(self, nbClients):
    import CORBA
    orb=CORBA.ORB_init([''])
    self._ior = orb.object_to_string( lm.buildRendezVous( nbClients ) )
  def barrier(self):
    import CORBA
    from .. import Engines
    orb=CORBA.ORB_init([''])
    rvPtr = orb.string_to_object( self._ior )
    rvPtr.acquire()

class LockGuardCM:
  def __init__(self, key):
    if lm is None:
        raise RuntimeError("SALOME has not been initialized !")
    self._lock = lm.getLockerFor( key )
  def __enter__(self):
    self._lock.acquire()
  def __exit__(self,exctype, exc, tb):
    self._lock.release()

class ContainerLauncherCM:
  def __init__(self, cp, aggressiveShutdown = False):
    """
    :param cp: Engines.ContainerParameters instance specifying the request where the container will be launched
    :param aggressiveShutdown:
    """
    self._cp = cp
    self.aggressiveShutdown = aggressiveShutdown
  def __enter__(self):
    self._cont = cm.GiveContainer(self._cp)
    return self._cont
  def __exit__(self,exctype, exc, tb):
    if not self.aggressiveShutdown:
      self._cont.Shutdown()
    else:
      try:# we are in aggressive situation the following call is not expected to return normally
        self._cont.ShutdownNow()
      except:
        pass

#to expose all objects to pydoc
__all__ = dir()
