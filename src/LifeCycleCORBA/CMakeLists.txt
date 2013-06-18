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

INCLUDE_DIRECTORIES(
  ${LIBXML2_INCLUDE_DIR}
  ${OMNIORB_INCLUDE_DIR}
  ${PTHREAD_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_DIRS}
  ${CMAKE_BINARY_DIR}/salome_adm
  ${CMAKE_CURRENT_SOURCE_DIR}/../Basics
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMELocalTrace
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMETraceCollector
  ${CMAKE_CURRENT_SOURCE_DIR}/../NamingService
  ${CMAKE_CURRENT_SOURCE_DIR}/../Utils
  ${CMAKE_CURRENT_SOURCE_DIR}/../GenericObj
  ${CMAKE_CURRENT_SOURCE_DIR}/../Container
  ${CMAKE_CURRENT_SOURCE_DIR}/../ResourcesManager
  ${CMAKE_CURRENT_SOURCE_DIR}/../Batch
  ${CMAKE_CURRENT_SOURCE_DIR}/../Notification
  ${CMAKE_CURRENT_SOURCE_DIR}/../Registry
  ${CMAKE_BINARY_DIR}/idl
)

SET(COMMON_LIBS
  SalomeContainer
  SalomeResourcesManager
  SalomeNS
  OpUtil
  SALOMELocalTrace
  SALOMEBasics
  SalomeGenericObj
  SalomeIDLKernel
)

ADD_DEFINITIONS(${LIBXML2_DEFINITIONS} ${OMNIORB_DEFINITIONS})

ADD_LIBRARY(SalomeLifeCycleCORBA SALOME_LifeCycleCORBA.cxx SALOME_FileTransferCORBA.cxx)
TARGET_LINK_LIBRARIES(SalomeLifeCycleCORBA ${COMMON_LIBS})
INSTALL(TARGETS SalomeLifeCycleCORBA DESTINATION ${SALOME_INSTALL_LIBS})

ADD_EXECUTABLE(Test_LifeCycleCORBA Test_LifeCycleCORBA.cxx)
TARGET_LINK_LIBRARIES(Test_LifeCycleCORBA SalomeLifeCycleCORBA Registry SalomeNotification SalomeContainer ${COMMON_LIBS} ${OMNIORB_LIBRARIES})

ADD_EXECUTABLE(TestContainerManager TestContainerManager.cxx)
TARGET_LINK_LIBRARIES(TestContainerManager SalomeLifeCycleCORBA Registry SalomeNotification SalomeContainer ${COMMON_LIBS} ${OMNIORB_LIBRARIES})

INSTALL(TARGETS Test_LifeCycleCORBA TestContainerManager DESTINATION ${SALOME_INSTALL_BINS})

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${SALOME_INSTALL_HEADERS})
