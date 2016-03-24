# Copyright (C) 2012-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

SET(SUBDIR_BASE
  AppQuickStart
  Basics
  SALOMELocalTrace
  HDFPersist
  KERNEL_PY
  Utils
)

SET(SUBDIR_CORBA
  DF
  Logger
  SALOMETraceCollector
  NamingService
  GenericObj
  Registry
  ModuleCatalog
  ResourcesManager
  Notification
  NOTIFICATION_SWIG
  Container
  SALOMESDS
  TestContainer
  Launcher
  LifeCycleCORBA
  LifeCycleCORBA_SWIG
  SALOMEDSClient
  TOOLSDS
  KernelHelpers
  SALOMEDSImpl
  SALOMEDS
  ModuleGenerator
  Communication
  Communication_SWIG
  DSC
)

SET(SUBDIR_MPI MPIContainer TestMPIContainer)
SET(SUBDIR_PARALLEL ParallelContainer)

# CppUnit directories:
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

SET(SUBDIRS ${SUBDIR_BASE})
IF(NOT SALOME_LIGHT_ONLY)
  LIST(APPEND SUBDIRS ${SUBDIR_CORBA})
ENDIF()

IF(SALOME_USE_MPI)
  LIST(APPEND SUBDIRS ${SUBDIR_MPI})
ENDIF(SALOME_USE_MPI)

IF(SALOME_BUILD_TESTS)
  LIST(APPEND SUBDIRS ${SUBDIR_CPPUNIT_BASE} ${SUBDIR_CPPUNIT_GENERAL})
  IF(NOT SALOME_LIGHT_ONLY)
    LIST(APPEND SUBDIRS ${SUBDIR_CPPUNIT_CORBA})
  ENDIF()
ENDIF()

IF(SALOME_PACO_PARALLEL)
  LIST(APPEND SUBDIRS  ${SUBDIR_PARALLEL})
ENDIF()

FOREACH(_dir ${SUBDIRS})
  ADD_SUBDIRECTORY(${_dir})
ENDFOREACH()
