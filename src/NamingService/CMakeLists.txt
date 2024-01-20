# Copyright (C) 2012-2024  CEA, EDF, OPEN CASCADE
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
  ${OMNIORB_INCLUDE_DIR}
  ${PTHREAD_INCLUDE_DIR}
  ${PROJECT_BINARY_DIR}/salome_adm
  ${CMAKE_CURRENT_SOURCE_DIR}/../ArgvKeeper
  ${CMAKE_CURRENT_SOURCE_DIR}/../Basics
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMELocalTrace
  ${CMAKE_CURRENT_SOURCE_DIR}/../Utils
  ${CMAKE_CURRENT_SOURCE_DIR}/../KernelHelpers
  ${CMAKE_CURRENT_SOURCE_DIR}/../LifeCycleCORBA
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_BINARY_DIR}/idl
  ${PYTHON_INCLUDE_DIRS}
  )

SET(SalomeNS_SOURCES
  SALOME_NamingService.cxx
  ServiceUnreachable.cxx
  NamingService_WaitForServerReadiness.cxx
  SALOME_Fake_NamingService.cxx
  SALOME_NamingService_Abstract.cxx
  SALOME_Embedded_NamingService.cxx
  SALOME_Embedded_NamingService_Common.cxx
  SALOME_Embedded_NamingService_Client.cxx
)

ADD_DEFINITIONS(${OMNIORB_DEFINITIONS})
ADD_LIBRARY(SalomeNS ${SalomeNS_SOURCES})
ADD_DEPENDENCIES(SalomeNS SalomeIDLKernel)
TARGET_LINK_LIBRARIES(SalomeNS OpUtil SalomeORB)

INSTALL(TARGETS SalomeNS EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

# Executable scripts to be installed
SALOME_INSTALL_SCRIPTS(SALOME_NamingServicePy.py ${SALOME_INSTALL_SCRIPT_PYTHON})
SALOME_INSTALL_SCRIPTS(SALOME_Embedded_NamingService_ClientPy.py ${SALOME_INSTALL_SCRIPT_PYTHON})
SALOME_INSTALL_SCRIPTS(SALOME_Embedded_NamingService.py ${SALOME_INSTALL_SCRIPT_PYTHON})

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${SALOME_INSTALL_HEADERS})

SET(NamingService_HEADERS NamingService.i)
SET(NamingService_SOURCES ${NamingService_HEADERS})
SET_SOURCE_FILES_PROPERTIES(NamingService.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(NamingService.i PROPERTIES SWIG_FLAGS "-py3")
SET_SOURCE_FILES_PROPERTIES(NamingService_wrap.cpp PROPERTIES COMPILE_FLAGS "-DHAVE_CONFIG_H")
SET(_swig_SCRIPTS ${CMAKE_CURRENT_BINARY_DIR}/NamingService.py )
IF(${CMAKE_VERSION} VERSION_LESS "3.8.0") 
  SWIG_ADD_MODULE(NamingService python ${NamingService_SOURCES})
ELSE()
  SWIG_ADD_LIBRARY(NamingService LANGUAGE python SOURCES ${NamingService_SOURCES})
ENDIF()
IF(${MACHINE} STREQUAL WINDOWS)
  SET_TARGET_PROPERTIES(_NamingService PROPERTIES DEBUG_OUTPUT_NAME _NamingService_d)
ENDIF(${MACHINE} STREQUAL WINDOWS)
SWIG_LINK_LIBRARIES(NamingService ${PYTHON_LIBRARIES} ${PLATFORM_LIBS} SalomeNS)
install(TARGETS _NamingService DESTINATION ${SALOME_INSTALL_LIBS})
install(FILES ${NamingService_HEADERS} DESTINATION ${SALOME_INSTALL_HEADERS})
SALOME_INSTALL_SCRIPTS("${_swig_SCRIPTS}"  ${SALOME_INSTALL_BINS} EXTRA_DPYS "${SWIG_MODULE_NamingService_REAL_NAME}")
