#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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
Module salome gives access to Salome ressources.

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
         - updateObjBrowser(bool):
         - getActiveStudyId():
         - getActiveStudyName():

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
  - salome.myStudyId       : active Study Id
  - salome.myStudy         : the active Study itself (CORBA ior)
      - methods : defined in SALOMEDS.idl

"""
## @package salome
# Module salome gives access to Salome ressources.
#
#  \param salome.orb             : CORBA orb object
#  \param salome.naming_service  : instance of naming Service class (SALOME_NamingServicePy::SALOME_NamingServicePy_i)
#  \param salome.lcc             : instance of lifeCycleCORBA class (SALOME_LifeCycleCORBA)
#  \param salome.sg              : Salome object to communicate with the graphical user interface, if running (see interface in salome_iapp::SalomeOutsideGUI)
#  \param salome.myStudyName     : active Study Name
#  \param salome.myStudyId       : active Study Id
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
        if not isinstance(dir, basestring) or not os.path.isdir(dir) or not dir.endswith(MATCH_ENDING_PATTERN):
            continue
        subdir = os.path.join(dir, pname)
        # XXX This may still add duplicate entries to path on
        # case-insensitive filesystems
        if os.path.isdir(subdir) and subdir not in __path__:
            if verbose(): print "INFO - The directory %s is appended to sys.path" % subdir
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

orb, lcc, naming_service, cm,sg=None,None,None,None,None
myStudyManager, myStudyId, myStudy, myStudyName=None,None,None,None

def setCurrentStudy(theStudy):
    """
    Change current study : an existing one given by a study object.

    :param theStudy: the study CORBA object to set as current study
    """
    global myStudyId, myStudy, myStudyName
    myStudyId, myStudy, myStudyName =salome_study.setCurrentStudy(theStudy)

def setCurrentStudyId(theStudyId=0):
    """
    Change current study : an existing or new one given by Id.

    :param theStudyId: the study Id (optional argument)
           0      : create a new study (default).
           n (>0) : try connection to study with Id = n, or create a new one
                      if study not found.
    """
    global myStudyId, myStudy, myStudyName
    myStudyId, myStudy, myStudyName =salome_study.setCurrentStudyId(theStudyId)

salome_initial=1
def salome_init(theStudyId=0,embedded=0):
    """
    Performs only once SALOME general purpose intialisation for scripts.
    optional argument : theStudyId
      When in embedded interpreter inside IAPP, theStudyId is not used
      When used without GUI (external interpreter)
        0      : create a new study (default).
        n (>0) : try connection to study with Id = n, or create a new one
                 if study not found.
                 If study creation, its Id may be different from theStudyId !
    Provides:
    orb             reference to CORBA
    lcc             a LifeCycleCorba instance
    naming_service  a naming service instance
    cm              reference to the container manager
    sg              access to SALOME GUI (when linked with IAPP GUI)
    myStudyManager  the study manager
    myStudyId       active study identifier
    myStudy         active study itself (CORBA reference)
    myStudyName     active study name
    """
    global salome_initial
    global orb, lcc, naming_service, cm
    global sg
    global myStudyManager, myStudyId, myStudy, myStudyName

    try:
        if salome_initial:
            salome_initial=0
            sg = salome_iapp_init(embedded)
            orb, lcc, naming_service, cm = salome_kernel_init()
            myStudyManager, myStudyId, myStudy, myStudyName = salome_study_init(theStudyId)
            pass
        pass
    except RuntimeError, inst:
        # wait a little to avoid trace mix
        import time
        time.sleep(0.2)
        x = inst
        print "salome.salome_init():", x
        print """
        ============================================
        May be there is no running SALOME session
        salome.salome_init() is intented to be used
        within an already running session
        ============================================
        """
        raise
    
def salome_close():
    global salome_initial, myStudy, myStudyId, myStudyName
    try:
        # study can be closed either from GUI or directly with salome.myStudy.Close()
        myStudy.Close()
    except:
        pass
    salome_initial=1
    salome_iapp_close()
    salome_study_close()
    myStudyId, myStudy, myStudyName=None,None,None
    pass


#to expose all objects to pydoc
__all__=dir()
