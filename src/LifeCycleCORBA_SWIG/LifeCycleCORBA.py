#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
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

class ContainerParameters (Engines.ContainerParameters):
  def __init__(self, container_name='', mode='start', workingdir='', nb_proc=0, isMPI=False, parallelLib='',resource_params=None):
    if resource_params is None:resource_params=ResourceParameters()
    Engines.ContainerParameters.__init__(self,container_name, mode, workingdir, nb_proc, isMPI, parallelLib,resource_params)

class ResourceParameters (Engines.ResourceParameters):
  def __init__(self, name="", hostname="", OS="", componentList=[],
                     nb_proc=0, mem_mb=0, cpu_clock=0, nb_node=0, nb_proc_per_node=0,
                     policy="", resList=[], can_launch_batch_jobs = False, can_run_containers = False):
    Engines.ResourceParameters.__init__(self, name, hostname, can_launch_batch_jobs, can_run_containers,
                                        OS, componentList, nb_proc, mem_mb, cpu_clock, nb_node,
                                        nb_proc_per_node, policy, resList)

class JobParameters (Engines.JobParameters):
  def __init__(self, job_name="", job_type="", job_file="", env_file="", in_files=[], out_files=[],
                     work_directory="", local_directory="", result_directory="", maximum_duration="",
                     resource_required=None, queue="", specific_parameters=[]):
    Engines.JobParameters.__init__(self, job_name, job_type, job_file, env_file, in_files, out_files,
                                         work_directory, local_directory, result_directory, maximum_duration,
                                         resource_required, queue, specific_parameters)

class ResourceDefinition(Engines.ResourceDefinition):
  def __init__(self, name="", hostname="", protocol="rsh", username="", applipath="", componentList=[],
               mode="interactive", OS="", mem_mb=1, cpu_clock=1, nb_node=1, nb_proc_per_node=1,
               batch="", mpiImpl="", iprotocol="rsh", type = "single_machine",
               can_launch_batch_jobs = False, can_run_containers = False, working_directory = ""):
    Engines.ResourceDefinition.__init__(self, name, hostname, type, protocol, username, applipath,
                                        componentList, OS, mem_mb, cpu_clock, nb_node, nb_proc_per_node,
                                        batch, mpiImpl, iprotocol, can_launch_batch_jobs,
                                        can_run_containers, working_directory)
