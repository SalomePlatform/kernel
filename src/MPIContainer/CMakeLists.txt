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
  ${PYTHON_INCLUDE_DIRS}
  ${OMNIORB_INCLUDE_DIRS}
  ${CMAKE_BINARY_DIR}/salome_adm
  ${CMAKE_CURRENT_SOURCE_DIR}/../Basics
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMELocalTrace
  ${CMAKE_CURRENT_SOURCE_DIR}/../NamingService
  ${CMAKE_CURRENT_SOURCE_DIR}/../Utils
  ${CMAKE_CURRENT_SOURCE_DIR}/../Registry
  ${CMAKE_CURRENT_SOURCE_DIR}/../Notification
  ${CMAKE_CURRENT_SOURCE_DIR}/../ResourcesManager
  ${CMAKE_CURRENT_SOURCE_DIR}/../Container
  ${CMAKE_CURRENT_SOURCE_DIR}/../GenericObj
  ${CMAKE_BINARY_DIR}/idl
)

SET(COMMON_LIBS
  Registry
  SalomeNotification
  SalomeResourcesManager
  SalomeNS
  OpUtil
  SALOMELocalTrace
  SALOMEBasics
  SalomeContainer
  SalomeGenericObj
  SalomeIDLKernel
  ${CORBA_LIBS}
)

ADD_LIBRARY(SalomeMPIContainer SHARED MPIObject_i.cxx MPIContainer_i.cxx)
SET_TARGET_PROPERTIES(SalomeMPIContainer PROPERTIES COMPILE_FLAGS "${MPI_DEFINITIONS} ${OMNIORB_DEFINITIONS}")
TARGET_LINK_LIBRARIES(SalomeMPIContainer ${COMMON_LIBS})
INSTALL(TARGETS SalomeMPIContainer DESTINATION ${KERNEL_salomelib_LIBS})

ADD_EXECUTABLE(SALOME_MPIContainer SALOME_MPIContainer.cxx)
SET_TARGET_PROPERTIES(SALOME_MPIContainer PROPERTIES COMPILE_FLAGS "${MPI_DEFINITIONS} ${OMNIORB_DEFINITIONS}")
TARGET_LINK_LIBRARIES(SALOME_MPIContainer SalomeMPIContainer ${COMMON_LIBS} ${PYTHON_LIBS} ${MPI_LIBS})

ADD_EXECUTABLE(testMPI2 testMPI2.cxx)
SET_TARGET_PROPERTIES(testMPI2 PROPERTIES COMPILE_FLAGS "${MPI_DEFINITIONS}")
TARGET_LINK_LIBRARIES(testMPI2 ${MPI_LIBS})

INSTALL(TARGETS SALOME_MPIContainer testMPI2 DESTINATION ${KERNEL_salomebin_BINS})

INSTALL(FILES launch_testMPI2.csh DESTINATION ${KERNEL_salomescript_SCRIPTS})

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${KERNEL_salomeinclude_HEADERS})
