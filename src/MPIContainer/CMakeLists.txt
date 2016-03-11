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
  ${MPI_CXX_INCLUDE_PATH}
  ${PTHREAD_INCLUDE_DIR}
  ${PYTHON_INCLUDE_DIRS}
  ${OMNIORB_INCLUDE_DIR}
  ${PROJECT_BINARY_DIR}/salome_adm
  ${CMAKE_CURRENT_SOURCE_DIR}/../Basics
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMELocalTrace
  ${CMAKE_CURRENT_SOURCE_DIR}/../NamingService
  ${CMAKE_CURRENT_SOURCE_DIR}/../Utils
  ${CMAKE_CURRENT_SOURCE_DIR}/../Registry
  ${CMAKE_CURRENT_SOURCE_DIR}/../Notification
  ${CMAKE_CURRENT_SOURCE_DIR}/../ResourcesManager
  ${CMAKE_CURRENT_SOURCE_DIR}/../Container
  ${CMAKE_CURRENT_SOURCE_DIR}/../GenericObj
  ${PROJECT_BINARY_DIR}/idl
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
  ${OMNIORB_LIBRARIES}
)

ADD_DEFINITIONS(${MPI_DEFINITIONS} ${OMNIORB_DEFINITIONS})

ADD_LIBRARY(SalomeMPIContainer MPIObject_i.cxx MPIContainer_i.cxx)
TARGET_LINK_LIBRARIES(SalomeMPIContainer ${COMMON_LIBS})
INSTALL(TARGETS SalomeMPIContainer EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

ADD_EXECUTABLE(SALOME_MPIContainer SALOME_MPIContainer.cxx)
TARGET_LINK_LIBRARIES(SALOME_MPIContainer SalomeMPIContainer ${COMMON_LIBS} ${PYTHON_LIBRARIES} ${MPI_CXX_LIBRARIES})

ADD_EXECUTABLE(testMPI2 testMPI2.cxx)
TARGET_LINK_LIBRARIES(testMPI2 ${MPI_CXX_LIBRARIES})

ADD_EXECUTABLE(getMPIImplementation getMPIImplementation.cxx)
TARGET_LINK_LIBRARIES(getMPIImplementation ${MPI_CXX_LIBRARIES})

INSTALL(TARGETS SALOME_MPIContainer EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_BINS})
INSTALL(TARGETS testMPI2 DESTINATION ${SALOME_INSTALL_BINS}) 
INSTALL(TARGETS getMPIImplementation DESTINATION ${SALOME_INSTALL_BINS})

INSTALL(FILES launch_testMPI2.csh PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE DESTINATION ${SALOME_INSTALL_SCRIPT_SCRIPTS})

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${SALOME_INSTALL_HEADERS})
