# Copyright (C) 2012-2021  CEA/DEN, EDF R&D, OPEN CASCADE
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

include(${SWIG_USE_FILE})

IF(SALOME_BUILD_TESTS)
  ADD_SUBDIRECTORY(Test)
ENDIF(SALOME_BUILD_TESTS)

INCLUDE_DIRECTORIES(
  ${LIBXML2_INCLUDE_DIR}
  ${OMNIORB_INCLUDE_DIR}
  ${PYTHON_INCLUDE_DIRS}
  ${PTHREAD_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${LIBBATCH_INCLUDE_DIRS}
  ${PROJECT_BINARY_DIR}/salome_adm
  ${CMAKE_CURRENT_SOURCE_DIR}/../Basics
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMELocalTrace
  ${CMAKE_CURRENT_SOURCE_DIR}/../NamingService
  ${CMAKE_CURRENT_SOURCE_DIR}/../Utils
  ${CMAKE_CURRENT_SOURCE_DIR}/../Registry
  ${CMAKE_CURRENT_SOURCE_DIR}/../Notification
  ${CMAKE_CURRENT_SOURCE_DIR}/../Container
  ${CMAKE_CURRENT_SOURCE_DIR}/../ResourcesManager
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMESDS
  ${CMAKE_CURRENT_SOURCE_DIR}/../LifeCycleCORBA
  ${CMAKE_CURRENT_SOURCE_DIR}/../KernelHelpers
  ${PROJECT_BINARY_DIR}/idl
  ${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_DEFINITIONS(${LIBXML2_DEFINITIONS} ${BOOST_DEFINITIONS} ${OMNIORB_DEFINITIONS})
IF(SALOME_USE_LIBBATCH)
  ADD_DEFINITIONS(-DWITH_LIBBATCH)
ENDIF(SALOME_USE_LIBBATCH)

# This local variable defines the list of dependent libraries common to all target in this package.
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
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${LIBBATCH_LIBRARIES}
  ${OMNIORB_LIBRARIES}
  ${LIBXML2_LIBRARIES}
  ${PYTHON_LIBRARIES}
)

SET(Launcher_SOURCES
  SALOME_Launcher_Parser.cxx
  SALOME_Launcher_Handler.cxx
  Launcher_Job.cxx
  Launcher_Job_Command.cxx
  Launcher_Job_CommandSALOME.cxx
  Launcher_Job_SALOME.cxx
  Launcher_Job_PythonSALOME.cxx
  Launcher_Job_YACSFile.cxx
  Launcher.cxx
  Launcher_XML_Persistence.cxx
)

ADD_LIBRARY(Launcher ${Launcher_SOURCES})
TARGET_LINK_LIBRARIES(Launcher ResourcesManager ${LIBBATCH_LIBRARIES} ${LIBXML2_LIBS} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
INSTALL(TARGETS Launcher EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

SET(TestLauncher_LIBS
  ${LIBXML2_LIBRARIES}
  ResourcesManager
  Launcher
  ${LIBBATCH_LIBRARIES}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
)

# TestLauncher needs only (and must need only) libxml, libbatch, ResourceManager and Launcher libraries!
ADD_EXECUTABLE(TestLauncher TestLauncher.cxx)
TARGET_LINK_LIBRARIES(TestLauncher ${TestLauncher_LIBS})
INSTALL(TARGETS TestLauncher DESTINATION ${SALOME_INSTALL_BINS})

ADD_LIBRARY(SalomeLauncher BatchTest.cxx SALOME_Launcher.cxx SALOME_ExternalServerLauncher.cxx SALOME_LauncherException.cxx SALOME_ExternalServerHandler.cxx)
TARGET_LINK_LIBRARIES(SalomeLauncher Launcher ${COMMON_LIBS})
INSTALL(TARGETS SalomeLauncher EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})
  
SET(SALOME_LauncherServer_LIBS
  Launcher
  SalomeLauncher
  SalomeSDS
  SALOMEBasics
  SALOMELocalTrace
  ${LIBBATCH_LIBRARIES}
  ${OMNIORB_LIBRARIES}
  ${LIBXML2_LIBRARIES}
  ${PYTHON_LIBRARIES}
)

ADD_EXECUTABLE(SALOME_LauncherServer SALOME_LauncherServer.cxx)
TARGET_LINK_LIBRARIES(SALOME_LauncherServer ${SALOME_LauncherServer_LIBS})
INSTALL(TARGETS SALOME_LauncherServer EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_BINS})

SET(COMMON_HEADERS_HXX
  BatchTest.hxx
  Launcher.hxx
  Launcher_Job.hxx
  Launcher_Job_Command.hxx
  Launcher_Job_CommandSALOME.hxx
  Launcher_Job_PythonSALOME.hxx
  Launcher_Job_SALOME.hxx
  Launcher_Job_YACSFile.hxx
  Launcher_Utils.hxx
  SALOME_Launcher.hxx
  SALOME_Launcher_Parser.hxx
  SALOME_Launcher_defs.hxx
  SALOME_ExternalServerLauncher.hxx
  SALOME_LauncherException.hxx
  KernelLauncher.hxx
)

SET(LAUNCHER_PYTHON_SCRIPTS
  launcher_proxy.py
)

SET(KernelLauncher_HEADERS KernelLauncher.hxx KernelLauncher.i)
SET(KernelLauncher_SOURCES KernelLauncher.cxx ${KernelLauncher_HEADERS})
SET_SOURCE_FILES_PROPERTIES(KernelLauncher.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(KernelLauncher.i PROPERTIES SWIG_FLAGS "-py3")
SET_SOURCE_FILES_PROPERTIES(KernelLauncher_wrap.cpp PROPERTIES COMPILE_FLAGS "-DHAVE_CONFIG_H")
SET(_swig_SCRIPTS ${CMAKE_CURRENT_BINARY_DIR}/KernelLauncher.py )
IF(${CMAKE_VERSION} VERSION_LESS "3.8.0") 
  SWIG_ADD_MODULE(KernelLauncher python ${KernelLauncher_SOURCES})
ELSE()
  SWIG_ADD_LIBRARY(KernelLauncher LANGUAGE python SOURCES ${KernelLauncher_SOURCES})
ENDIF()
SWIG_LINK_LIBRARIES(KernelLauncher ${PYTHON_LIBRARIES} ${PLATFORM_LIBS} SalomeDS SalomeKernelHelpers)
install(TARGETS _KernelLauncher DESTINATION ${SALOME_INSTALL_LIBS})
install(FILES ${KernelLauncher_HEADERS} DESTINATION ${SALOME_INSTALL_HEADERS})
SALOME_INSTALL_SCRIPTS("${_swig_SCRIPTS}"  ${SALOME_INSTALL_BINS} EXTRA_DPYS "${SWIG_MODULE_KernelLauncher_REAL_NAME}")


INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${SALOME_INSTALL_HEADERS})
INSTALL(FILES testLauncher.xml DESTINATION ${SALOME_KERNEL_INSTALL_RES_DATA})
SALOME_INSTALL_SCRIPTS("${LAUNCHER_PYTHON_SCRIPTS}" ${SALOME_INSTALL_PYTHON})
