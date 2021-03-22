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

#  File   : salome_kernel.py
#  Author : Paul RASCLE, EDF
#  Module : SALOME
#  $Header$
#
from omniORB import CORBA
from LifeCycleCORBA import *
from SALOME_NamingServicePy import *
from SALOME_utilities import *
import Engines
import SALOME
import SALOME_ModuleCatalog

orb = None
lcc = None
naming_service = None
cm = None
esm = None
dsm = None
modulcat = None

def list_of_catalogs_regarding_environement():
    """
    Method to detect XML module catalogs regarding environement variables
    """
    import os
    KEY_IN_ENV_VAR = "_ROOT_DIR"
    modules_env_var = [elt for elt in os.environ.keys() if elt[-len(KEY_IN_ENV_VAR):]==KEY_IN_ENV_VAR]
    list_catalogs = []
    for module_env_var in modules_env_var:
        module_upper_case = module_env_var[:-len(KEY_IN_ENV_VAR)]
        file_candidate = os.path.join(os.environ[module_env_var],"share","salome","resources",module_upper_case.lower(),"{}Catalog.xml".format(module_upper_case))
        if os.path.isfile(file_candidate):
            list_catalogs.append(file_candidate)
        pass
    return list_catalogs

def salome_kernel_init():
    global orb, lcc, naming_service, cm, esm, dsm, modulcat
    
    if not orb:
        # initialise the ORB
        orb = CORBA.ORB_init([''], CORBA.ORB_ID)

        # create a LifeCycleCORBA instance
        lcc = LifeCycleCORBA(orb)

        #create a naming service instance
        naming_service = SALOME_NamingServicePy_i(orb)

        # get Container Manager
        obj = naming_service.Resolve('/ContainerManager')
        cm = obj._narrow(Engines.ContainerManager)
        # get External Server Manager
        obj = naming_service.Resolve('/ExternalServers')
        esm = obj._narrow(SALOME.ExternalServerLauncher)
        #
        obj = naming_service.Resolve('/DataServerManager')
        dsm = obj._narrow(SALOME.DataServerManager)
        #
        obj = naming_service.Resolve('Kernel/ModulCatalog')
        modulcat = obj._narrow(SALOME_ModuleCatalog.ModuleCatalog)
        
    return orb, lcc, naming_service, cm, esm, dsm, modulcat
