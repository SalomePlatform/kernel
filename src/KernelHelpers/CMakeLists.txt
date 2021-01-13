# Copyright (C) 2012-2020  CEA/DEN, EDF R&D, OPEN CASCADE
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
  ${PYTHON_INCLUDE_DIRS}
  ${PROJECT_BINARY_DIR}/salome_adm
  ${CMAKE_CURRENT_SOURCE_DIR}/../NamingService
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMELocalTrace
  ${CMAKE_CURRENT_SOURCE_DIR}/../Basics
  ${CMAKE_CURRENT_SOURCE_DIR}/../Utils
  ${CMAKE_CURRENT_SOURCE_DIR}/../LifeCycleCORBA
  ${CMAKE_CURRENT_SOURCE_DIR}/../Container
  ${CMAKE_CURRENT_SOURCE_DIR}/../Notification
  ${CMAKE_CURRENT_SOURCE_DIR}/../GenericObj
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_BINARY_DIR}/idl
)

SET(SalomeKernelHelpers_LIBS
  ${OMNIORB_LIBRARIES}
  SalomeNS
  SALOMELocalTrace
  SALOMEBasics
  OpUtil
  SalomeLifeCycleCORBA
  SalomeContainer
  SalomeNotification
  SalomeGenericObj
  SalomeIDLKernel
)

SET(SalomeKernelHelpers_SOURCES
  SALOME_KernelServices.cxx
  SALOME_StudyEditor.cxx
  SALOMEDS_DriverDefaultImpl.cxx
)

ADD_DEFINITIONS(${OMNIORB_DEFINITIONS})

ADD_LIBRARY(SalomeKernelHelpers ${SalomeKernelHelpers_SOURCES})
TARGET_LINK_LIBRARIES(SalomeKernelHelpers ${SalomeKernelHelpers_LIBS})
INSTALL(TARGETS SalomeKernelHelpers EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

ADD_EXECUTABLE(KernelHelpersUseCases KernelHelpersUseCases.cxx)
TARGET_LINK_LIBRARIES(KernelHelpersUseCases SalomeKernelHelpers ${SalomeKernelHelpers_LIBS})
INSTALL(TARGETS KernelHelpersUseCases DESTINATION ${SALOME_INSTALL_BINS})

#

SET(KernelServices_HEADERS KernelServices.hxx KernelServices.i)
SET(KernelServices_SOURCES KernelServices.cxx ${KernelServices_HEADERS})
SET_SOURCE_FILES_PROPERTIES(KernelServices.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(KernelServices.i PROPERTIES SWIG_FLAGS "-py3")
SET_SOURCE_FILES_PROPERTIES(KernelServices_wrap.cpp PROPERTIES COMPILE_FLAGS "-DHAVE_CONFIG_H")
SET(_swig_SCRIPTS ${CMAKE_CURRENT_BINARY_DIR}/KernelServices.py )
IF(${CMAKE_VERSION} VERSION_LESS "3.8.0") 
  SWIG_ADD_MODULE(KernelServices python ${KernelServices_SOURCES})
ELSE()
  SWIG_ADD_LIBRARY(KernelServices LANGUAGE python SOURCES ${KernelServices_SOURCES})
ENDIF()
SWIG_LINK_LIBRARIES(KernelServices ${PYTHON_LIBRARIES} ${PLATFORM_LIBS} SalomeKernelHelpers)
install(TARGETS _KernelServices DESTINATION ${SALOME_INSTALL_LIBS})
install(FILES ${KernelServices_HEADERS} DESTINATION ${SALOME_INSTALL_HEADERS})
SALOME_INSTALL_SCRIPTS("${_swig_SCRIPTS}"  ${SALOME_INSTALL_BINS} EXTRA_DPYS "${SWIG_MODULE_KernelServices_REAL_NAME}")

#

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${SALOME_INSTALL_HEADERS})
