#  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
#
#  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
#  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
#  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
#  SALOME LifeCycleCORBA : implementation of containers and engines life cycle both in Python and C++
#  File   : LifeCycleCORBA.py
#  Author : Paul RASCLE, EDF
#  Module : SALOME
#  $Header$
#
import Engines
from libSALOME_LifeCycleCORBA import *

class LifeCycleCORBA (SALOME_LifeCycleCORBA):

    def __init__(self, orb = None):
        SALOME_LifeCycleCORBA.__init__(self)

    def FindOrLoadComponent(self, containerName, componentName):
        return SALOME_LifeCycleCORBA.FindOrLoad_Component(self,
                                                          containerName,
                                                          componentName)

class MachineParameters (Engines.MachineParameters):
          def __init__(self, container_name='', hostname='', componentList=[], computerList=[], OS='', 
                             mem_mb=0, cpu_clock=0, nb_proc_per_node=0, nb_node=0, isMPI=False, workingdir='', 
                             mode='start', policy='altcycl', parallelLib='', nb_component_nodes=0):
            Engines.MachineParameters.__init__(self,container_name, hostname, componentList, computerList, OS, 
                                                    mem_mb, cpu_clock, nb_proc_per_node, nb_node, isMPI, workingdir, 
                                                    mode, policy, parallelLib, nb_component_nodes)

