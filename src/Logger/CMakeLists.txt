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
  ${PYTHON_INCLUDE_DIRS}
  ${PROJECT_BINARY_DIR}/salome_adm
  ${CMAKE_CURRENT_SOURCE_DIR}/../ArgvKeeper
  ${CMAKE_CURRENT_SOURCE_DIR}/../Utils
  ${CMAKE_CURRENT_SOURCE_DIR}/../KernelHelpers
  ${CMAKE_CURRENT_SOURCE_DIR}/../NamingService
  ${CMAKE_CURRENT_SOURCE_DIR}/../LifeCycleCORBA
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_BINARY_DIR}/idl
  ${SALOMEBOOTSTRAP_INCLUDE_DIRS}
)
ADD_DEFINITIONS(${OMNIORB_DEFINITIONS})

ADD_LIBRARY(SalomeLoggerServer SALOME_Trace.cxx SALOME_Logger_Server.cxx)
TARGET_LINK_LIBRARIES(SalomeLoggerServer SalomeKernelHelpers  SalomeIDLKernel ${SALOMEBOOTSTRAP_SALOMEException} ${OMNIORB_LIBRARIES} ${PLATFORM_LIBS})
INSTALL(TARGETS SalomeLoggerServer EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

SET(SALOME_Logger_Server_SOURCES
  SALOME_Logger_Server_main.cxx
  SALOME_Logger_Server.hxx
)

SET(SALOME_Logger_Server_LIBS
  ArgvKeeper
  SalomeLoggerServer
  SalomeIDLKernel
  OpUtil
  SalomeKernelHelpers
  ${OMNIORB_LIBRARIES}
)

ADD_EXECUTABLE(SALOME_Logger_Server ${SALOME_Logger_Server_SOURCES})
TARGET_LINK_LIBRARIES(SALOME_Logger_Server ${SALOME_Logger_Server_LIBS})
INSTALL(TARGETS SALOME_Logger_Server EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_BINS})

SET(KernelLogger_HEADERS KernelLogger.hxx KernelLogger.i)
SET(KernelLogger_SOURCES KernelLogger.cxx ${KernelLogger_HEADERS})
SET_SOURCE_FILES_PROPERTIES(KernelLogger.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(KernelLogger.i PROPERTIES SWIG_FLAGS "-py3")
SET_SOURCE_FILES_PROPERTIES(KernelLogger_wrap.cpp PROPERTIES COMPILE_FLAGS "-DHAVE_CONFIG_H")
SET(_swig_SCRIPTS ${CMAKE_CURRENT_BINARY_DIR}/KernelLogger.py )
IF(${CMAKE_VERSION} VERSION_LESS "3.8.0") 
  SWIG_ADD_MODULE(KernelLogger python ${KernelLogger_SOURCES})
ELSE()
  SWIG_ADD_LIBRARY(KernelLogger LANGUAGE python SOURCES ${KernelLogger_SOURCES})
ENDIF()
IF(${MACHINE} STREQUAL WINDOWS)
  SET_TARGET_PROPERTIES(_KernelLogger PROPERTIES DEBUG_OUTPUT_NAME _KernelLogger_d)
ENDIF(${MACHINE} STREQUAL WINDOWS)
SWIG_LINK_LIBRARIES(KernelLogger ${PYTHON_LIBRARIES} ${PLATFORM_LIBS} SalomeLoggerServer)
install(TARGETS _KernelLogger DESTINATION ${SALOME_INSTALL_LIBS})
install(FILES ${KernelLogger_HEADERS} DESTINATION ${SALOME_INSTALL_HEADERS})
SALOME_INSTALL_SCRIPTS("${_swig_SCRIPTS}"  ${SALOME_INSTALL_BINS} EXTRA_DPYS "${SWIG_MODULE_KernelLogger_REAL_NAME}")

# Executable scripts to be installed
SALOME_INSTALL_SCRIPTS(SALOME_Trace.py ${SALOME_INSTALL_SCRIPT_PYTHON})

INSTALL(FILES SALOME_Trace.hxx DESTINATION ${SALOME_INSTALL_HEADERS})
