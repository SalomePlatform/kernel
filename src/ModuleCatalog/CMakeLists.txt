# Copyright (C) 2012-2023  CEA/DEN, EDF R&D, OPEN CASCADE
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

INCLUDE_DIRECTORIES(
  ${PTHREAD_INCLUDE_DIR}
  ${OMNIORB_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${PYTHON_INCLUDE_DIRS}
  ${PROJECT_BINARY_DIR}/salome_adm
  ${CMAKE_CURRENT_SOURCE_DIR}/../ArgvKeeper
  ${CMAKE_CURRENT_SOURCE_DIR}/../Basics
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMELocalTrace
  ${CMAKE_CURRENT_SOURCE_DIR}/../NamingService
  ${CMAKE_CURRENT_SOURCE_DIR}/../Utils
  ${CMAKE_CURRENT_SOURCE_DIR}/../KernelHelpers
  ${CMAKE_CURRENT_SOURCE_DIR}/../LifeCycleCORBA
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_BINARY_DIR}/idl
)

# This local variable defines the list of dependent libraries common to all target in this package.
SET(COMMON_LIBS
  ArgvKeeper
  SalomeNS
  OpUtil
  SALOMELocalTrace
  SALOMEBasics
  SalomeIDLKernel
  SalomeKernelHelpers
  ${LIBXML2_LIBRARIES}
  ${PTHREAD_LIBRARIES}
)

SET(SalomeCatalog_SOURCES
  SALOME_ModuleCatalog_Handler.cxx
  SALOME_ModuleCatalog_Parser_IO.cxx
  SALOME_ModuleCatalog_impl.cxx
  SALOME_ModuleCatalog_Acomponent_impl.cxx
)
ADD_DEFINITIONS(${LIBXML2_DEFINITIONS} ${OMNIORB_DEFINITIONS})

ADD_LIBRARY(SalomeCatalog ${SalomeCatalog_SOURCES})
TARGET_LINK_LIBRARIES(SalomeCatalog ${COMMON_LIBS})
INSTALL(TARGETS SalomeCatalog EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

ADD_EXECUTABLE(SALOME_ModuleCatalog_Server SALOME_ModuleCatalog_Server.cxx)
TARGET_LINK_LIBRARIES(SALOME_ModuleCatalog_Server SalomeCatalog ${COMMON_LIBS} ${OMNIORB_LIBRARIES})

ADD_EXECUTABLE(SALOME_ModuleCatalog_Client SALOME_ModuleCatalog_Client.cxx)
TARGET_LINK_LIBRARIES(SALOME_ModuleCatalog_Client SalomeCatalog ${COMMON_LIBS} ${OMNIORB_LIBRARIES})

#

SET(KernelModuleCatalog_HEADERS KernelModuleCatalog.hxx KernelModuleCatalog.i)
SET(KernelModuleCatalog_SOURCES KernelModuleCatalog.cxx ${KernelModuleCatalog_HEADERS})
SET_SOURCE_FILES_PROPERTIES(KernelModuleCatalog.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(KernelModuleCatalog.i PROPERTIES SWIG_FLAGS "-py3")
SET_SOURCE_FILES_PROPERTIES(KernelModuleCatalog_wrap.cpp PROPERTIES COMPILE_FLAGS "-DHAVE_CONFIG_H")
SET(_swig_SCRIPTS ${CMAKE_CURRENT_BINARY_DIR}/KernelModuleCatalog.py )
IF(${CMAKE_VERSION} VERSION_LESS "3.8.0") 
  SWIG_ADD_MODULE(KernelModuleCatalog python ${KernelModuleCatalog_SOURCES})
ELSE()
  SWIG_ADD_LIBRARY(KernelModuleCatalog LANGUAGE python SOURCES ${KernelModuleCatalog_SOURCES})
ENDIF()
IF(${MACHINE} STREQUAL WINDOWS)
  SET_TARGET_PROPERTIES(_KernelModuleCatalog PROPERTIES DEBUG_OUTPUT_NAME _KernelModuleCatalog_d)
ENDIF(${MACHINE} STREQUAL WINDOWS)
SWIG_LINK_LIBRARIES(KernelModuleCatalog ${PYTHON_LIBRARIES} ${PLATFORM_LIBS} SalomeCatalog SalomeKernelHelpers)
install(TARGETS _KernelModuleCatalog DESTINATION ${SALOME_INSTALL_LIBS})
install(FILES ${KernelModuleCatalog_HEADERS} DESTINATION ${SALOME_INSTALL_HEADERS})
SALOME_INSTALL_SCRIPTS("${_swig_SCRIPTS}"  ${SALOME_INSTALL_BINS} EXTRA_DPYS "${SWIG_MODULE_KernelModuleCatalog_REAL_NAME}")

#

INSTALL(TARGETS SALOME_ModuleCatalog_Server SALOME_ModuleCatalog_Client 
        EXPORT ${PROJECT_NAME}TargetGroup
        DESTINATION ${SALOME_INSTALL_BINS})

# Executable scripts to be installed
SALOME_INSTALL_SCRIPTS(TestModuleCatalog.py ${SALOME_INSTALL_SCRIPT_PYTHON})

SET(COMMON_HEADERS_HXX
  SALOME_ModuleCatalog.hxx
  SALOME_ModuleCatalog_Acomponent_impl.hxx
  SALOME_ModuleCatalog_impl.hxx
  )

INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${SALOME_INSTALL_HEADERS})
