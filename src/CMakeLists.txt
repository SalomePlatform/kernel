# Copyright (C) 2012-2013  CEA/DEN, EDF R&D, OPEN CASCADE
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

SET(SUBDIR_BASE
  Basics
  SALOMELocalTrace
  HDFPersist
  KERNEL_PY
)

SET(SUBDIR_CORBA
  DF
  Logger
  SALOMETraceCollector
  Utils
  NamingService
  GenericObj
  Registry
  ModuleCatalog
  ResourcesManager
  Notification
  NOTIFICATION_SWIG
  Container
  TestContainer
  Launcher
  LifeCycleCORBA
  LifeCycleCORBA_SWIG
  SALOMEDSClient
  TOOLSDS
  SALOMEDSImpl
  SALOMEDS
  KernelHelpers
  ModuleGenerator
  Communication
  Communication_SWIG
  DSC
)

SET(SUBDIR_CPPUNIT_BASE
  Basics/Test
  SALOMELocalTrace/Test
)

SET(SUBDIR_CPPUNIT_CORBA
  Logger/Test
  SALOMETraceCollector/Test
  Utils/Test
  NamingService/Test
  LifeCycleCORBA/Test
  LifeCycleCORBA_SWIG/Test
  SALOMEDSImpl/Test
  SALOMEDS/Test
  KernelHelpers/Test
)

SET(SUBDIR_CPPUNIT_GENERAL UnitTests)
SET(SUBDIR_MPI MPIContainer TestMPIContainer)
SET(SUBDIR_PARALLEL ParallelContainer)

IF(WITHONLYLAUNCHER)
  SET(SUBDIRS Basics ResourcesManager Launcher)
ELSE(WITHONLYLAUNCHER)
  SET(SUBDIRS ${SUBDIR_BASE})
ENDIF(WITHONLYLAUNCHER)

IF(SALOME_USE_CORBA)
  SET(SUBDIRS ${SUBDIRS} ${SUBDIR_CORBA})
ENDIF(SALOME_USE_CORBA)

IF(SALOME_USE_MPI)
  SET(SUBDIRS
    ${SUBDIRS}
    ${SUBDIR_MPI}
  )
ENDIF(SALOME_USE_MPI)

IF(CPPUNIT_IS_OK)
  IF(SALOME_USE_CORBA)
    SET(SUBDIRS
      ${SUBDIRS}
      ${SUBDIR_CPPUNIT_BASE}
      ${SUBDIR_CPPUNIT_CORBA}
      ${SUBDIR_CPPUNIT_GENERAL}
    )
  ELSE(SALOME_USE_CORBA)
    SET(SUBDIRS
      ${SUBDIRS}
      ${SUBDIR_CPPUNIT_BASE}
      ${SUBDIR_CPPUNIT_GENERAL}
    )
  ENDIF(SALOME_USE_CORBA)
ENDIF(CPPUNIT_IS_OK)

IF(WITH_PACO_PARALLEL)
  SET(SUBDIRS
    ${SUBDIRS}
    ${SUBDIR_PARALLEL}
  )
ENDIF(WITH_PACO_PARALLEL)

FOREACH(DIR ${SUBDIRS})
  ADD_SUBDIRECTORY(${DIR})
ENDFOREACH(DIR ${SUBDIRS})
