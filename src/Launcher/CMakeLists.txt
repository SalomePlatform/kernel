# Copyright (C) 2012  CEA/DEN, EDF R&D, OPEN CASCADE
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

INCLUDE_DIRECTORIES(
  ${LIBXML_INCLUDE_DIRS}
  ${OMNIORB_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_DIRS}
  ${PTHREAD_INCLUDE_DIRS}
  ${MPI_INCLUDE_DIRS}
  ${LIBBATCH_INCLUDE_DIRS}
  ${CMAKE_BINARY_DIR}/salome_adm/unix
  ${CMAKE_CURRENT_SOURCE_DIR}/../Basics
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMELocalTrace
  ${CMAKE_CURRENT_SOURCE_DIR}/../NamingService
  ${CMAKE_CURRENT_SOURCE_DIR}/../Utils
  ${CMAKE_CURRENT_SOURCE_DIR}/../Registry
  ${CMAKE_CURRENT_SOURCE_DIR}/../Notification
  ${CMAKE_CURRENT_SOURCE_DIR}/../Container
  ${CMAKE_CURRENT_SOURCE_DIR}/../ResourcesManager
  ${CMAKE_BINARY_DIR}/idl
)
SET(WITH_LIBBATCH_FLAG)
IF(WITH_LIBBATCH)
  SET(WITH_LIBBATCH_FLAG "-DWITH_LIBBATCH")
ENDIF(WITH_LIBBATCH)

# This local variable defines the list of FLAGS common to all target in this package.
SET(COMMON_FLAGS "${MPI_DEFINITIONS} ${LIBXML_DEFINITIONS} ${LIBBATCH_DEFINITIONS} ${OMNIORB_DEFINITIONS} ${WITH_LIBBATCH_FLAG} ${PLATFORM_DEFINITIONS}")

# This local variable defines the list of dependant libraries common to all target in this package.
SET(COMMON_LIBS
  Registry
  SalomeNotification
  SalomeContainer
  SalomeResourcesManager
  SalomeNS
  OpUtil
  SALOMELocalTrace
  SALOMEBasics
  SalomeIDLKernel
  ${LIBBATCH_LIBS}
  ${MPI_LIBS}
  ${OMNIORB_LIBS}
  ${LIBXML_LIBS}
  ${PYTHON_LIBS}
)

SET(Launcher_SOURCES
  SALOME_Launcher_Parser.cxx
  SALOME_Launcher_Handler.cxx
  Launcher_Utils.hxx
  Launcher_Job.cxx
  Launcher_Job_Command.cxx
  Launcher_Job_SALOME.cxx
  Launcher_Job_PythonSALOME.cxx
  Launcher_Job_YACSFile.cxx
  Launcher.cxx
)
SET(Launcher_FLAGS "${MPI_DEFINITIONS} ${LIBXML_DEFINITIONS} ${LIBBATCH_DEFINITIONS} ${WITH_LIBBATCH_FLAG} ${PLATFORM_DEFINITIONS}")

ADD_LIBRARY(Launcher SHARED ${Launcher_SOURCES})
SET_TARGET_PROPERTIES(Launcher PROPERTIES COMPILE_FLAGS "${Launcher_FLAGS}")
TARGET_LINK_LIBRARIES(Launcher ResourcesManager ${LIBBATCH_LIBS} ${MPI_LIBS} ${LIBXML_LIBS})
INSTALL(TARGETS Launcher DESTINATION ${KERNEL_salomelib_LIBS})

SET(TestLauncher_LIBS
  ${LIBXML_LIBS}
  ResourcesManager
  Launcher
  OpUtil
  ${LIBBATCH_LIBS}
)

ADD_EXECUTABLE(TestLauncher TestLauncher.cxx)
SET_TARGET_PROPERTIES(TestLauncher PROPERTIES COMPILE_FLAGS "${LIBXML_DEFINITIONS} ${LIBBATCH_DEFINITIONS} ${WITH_LIBBATCH_FLAG} ${PLATFORM_DEFINITIONS}")
TARGET_LINK_LIBRARIES(TestLauncher ${TestLauncher_LIBS})
INSTALL(TARGETS TestLauncher DESTINATION ${KERNEL_salomebin_BINS})

IF(NOT WITHONLYLAUNCHER)
  ADD_LIBRARY(SalomeLauncher SHARED BatchTest.cxx SALOME_Launcher.cxx)
  SET_TARGET_PROPERTIES(SalomeLauncher PROPERTIES COMPILE_FLAGS "${COMMON_FLAGS}")
  TARGET_LINK_LIBRARIES(SalomeLauncher Launcher ${COMMON_LIBS})
  INSTALL(TARGETS SalomeLauncher DESTINATION ${KERNEL_salomelib_LIBS})
  
  SET(SALOME_LauncherServer_LIBS
    Launcher
    SalomeLauncher
    SALOMEBasics
    SALOMELocalTrace
    ${LIBBATCH_LIBS}
    ${MPI_LIBS}
    ${OMNIORB_LIBS}
    ${LIBXML_LIBS}
    ${PYTHON_LIBS}
  )

  

  ADD_EXECUTABLE(SALOME_LauncherServer SALOME_LauncherServer.cxx)
  SET_TARGET_PROPERTIES(SALOME_LauncherServer PROPERTIES COMPILE_FLAGS "${COMMON_FLAGS}")
  TARGET_LINK_LIBRARIES(SALOME_LauncherServer ${SALOME_LauncherServer_LIBS})
  INSTALL(TARGETS SALOME_LauncherServer DESTINATION ${KERNEL_salomebin_BINS})
ENDIF(NOT WITHONLYLAUNCHER)

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${KERNEL_salomeinclude_HEADERS})
