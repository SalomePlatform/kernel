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

INCLUDE_DIRECTORIES(
  ${PTHREAD_INCLUDE_DIR}
  ${OMNIORB_INCLUDE_DIR}
  ${PROJECT_BINARY_DIR}/salome_adm
  ${CMAKE_CURRENT_SOURCE_DIR}/../NamingService
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMELocalTrace
  ${CMAKE_CURRENT_SOURCE_DIR}/../Basics
  ${CMAKE_CURRENT_SOURCE_DIR}/../Utils
  ${CMAKE_CURRENT_SOURCE_DIR}/../LifeCycleCORBA
  ${CMAKE_CURRENT_SOURCE_DIR}/../Container
  ${CMAKE_CURRENT_SOURCE_DIR}/../Notification
  ${CMAKE_CURRENT_SOURCE_DIR}/../GenericObj
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

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${SALOME_INSTALL_HEADERS})
