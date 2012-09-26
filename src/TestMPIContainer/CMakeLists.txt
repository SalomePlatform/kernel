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
  ${MPI_INCLUDE_DIRS}
  ${OMNIORB_INCLUDE_DIRS}
  ${CMAKE_BINARY_DIR}/salome_adm/unix
  ${CMAKE_CURRENT_SOURCE_DIR}/../Basics
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMELocalTrace
  ${CMAKE_CURRENT_SOURCE_DIR}/../NamingService
  ${CMAKE_CURRENT_SOURCE_DIR}/../Utils
  ${CMAKE_CURRENT_SOURCE_DIR}/../Registry
  ${CMAKE_CURRENT_SOURCE_DIR}/../Notification
  ${CMAKE_CURRENT_SOURCE_DIR}/../ResourcesManager
  ${CMAKE_CURRENT_SOURCE_DIR}/../Container
  ${CMAKE_CURRENT_SOURCE_DIR}/../MPIContainer
  ${CMAKE_CURRENT_SOURCE_DIR}/../GenericObj
  ${CMAKE_BINARY_DIR}/idl
)

SET(COMMON_FLAGS "${OMNIORB_DEFINITIONS} ${MPI_DEFINITIONS}")

SET(COMMON_LIBS
  Registry
  SalomeNotification
  SalomeResourcesManager
  SalomeNS
  OpUtil
  SALOMELocalTrace
  SALOMEBasics
  SalomeContainer
  SalomeMPIContainer
  SalomeGenericObj
  SalomeIDLKernel
  ${OMNIORB_LIBS}
)

ADD_LIBRARY(SalomeTestMPIComponentEngine SHARED TestMPIComponentEngine.cxx)
SET_TARGET_PROPERTIES(SalomeTestMPIComponentEngine PROPERTIES COMPILE_FLAGS "${COMMON_FLAGS}")
TARGET_LINK_LIBRARIES(SalomeTestMPIComponentEngine ${COMMON_LIBS})
INSTALL(TARGETS SalomeTestMPIComponentEngine DESTINATION ${KERNEL_salomelib_LIBS})

ADD_EXECUTABLE(TestMPIContainer TestMPIContainer.cxx)
SET_TARGET_PROPERTIES(TestMPIContainer PROPERTIES COMPILE_FLAGS "${COMMON_FLAGS}")
TARGET_LINK_LIBRARIES(TestMPIContainer SalomeTestMPIComponentEngine ${COMMON_LIBS} ${MPI_LIBS})
INSTALL(TARGETS TestMPIContainer DESTINATION ${KERNEL_salomebin_BINS})

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${KERNEL_salomeinclude_HEADERS})
