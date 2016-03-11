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
  ${OMNIORB_INCLUDE_DIR}
  ${MPI_CXX_INCLUDE_PATH}
  ${PYTHON_INCLUDE_DIRS}
  ${PACO_INCLUDE_DIRS}
  ${PROJECT_BINARY_DIR}/salome_adm
  ${PROJECT_SOURCE_DIR}/src/Container
  ${PROJECT_SOURCE_DIR}/src/Notification
  ${PROJECT_SOURCE_DIR}/src/SALOMELocalTrace
  ${PROJECT_SOURCE_DIR}/src/Basics
  ${PROJECT_SOURCE_DIR}/src/NamingService
  ${PROJECT_SOURCE_DIR}/src/Registry
  ${PROJECT_SOURCE_DIR}/src/Utils
  ${PROJECT_SOURCE_DIR}/src/GenericObj
  ${CMAKE_CURRENT_SOURCE_DIR}/../HDFPersist
  ${PROJECT_BINARY_DIR}/idl
  ${PROJECT_SOURCE_DIR}/src/SALOMETraceCollector
)

# This local variable defines the list of dependant libraries common to all target in this package.
SET(COMMON_LIBS
  SalomeContainer
  SalomeNS
  SALOMELocalTrace
  SALOMEBasics
  SalomeHDFPersist
  SalomeParallelIDLKernel
  SalomeIDLKernel
  ${OMNIORB_LIBRARIES}
  ${PACO_LIBRARIES}
  ${PYTHON_LIBRARIES}
)

SET(SalomeParallelContainer_SOURCES
  SALOME_ParallelComponent_i.cxx
  SALOME_ParallelContainer_i.cxx
  SALOME_ParallelContainerProxy_i.cxx
  Parallel_Salome_file_i.cxx
  SALOME_ParallelGlobalProcessVar_i.cxx
)

ADD_DEFINITIONS(${OMNIORB_DEFINITIONS} ${MPI_DEFINITIONS})

ADD_LIBRARY(SalomeParallelContainer ${SalomeParallelContainer_SOURCES})
TARGET_LINK_LIBRARIES(SalomeParallelContainer ${COMMON_LIBS} ${MPI_CXX_LIBRARIES})
INSTALL(TARGETS SalomeParallelContainer EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

IF(SALOME_USE_MPI)
  SET(SALOME_ParallelContainerProxyMpi_LIBS
    SalomeParallelContainer
    ${MPI_CXX_LIBRARIES}
    ${COMMON_LIBS}
  )
  ADD_EXECUTABLE(SALOME_ParallelContainerProxyMpi SALOME_ParallelContainerProxyMpi.cxx)
  TARGET_LINK_LIBRARIES(SALOME_ParallelContainerProxyMpi ${SALOME_ParallelContainerProxyMpi_LIBS})

  ADD_EXECUTABLE(SALOME_ParallelContainerNodeMpi SALOME_ParallelContainerNodeMpi.cxx)
  TARGET_LINK_LIBRARIES(SALOME_ParallelContainerNodeMpi ${SALOME_ParallelContainerProxyMpi_LIBS})

  INSTALL(TARGETS SALOME_ParallelContainerProxyMpi SALOME_ParallelContainerNodeMpi
        EXPORT ${PROJECT_NAME}TargetGroup 
        DESTINATION ${SALOME_INSTALL_BINS})
ENDIF(SALOME_USE_MPI)

SET(SALOME_ParallelContainerProxyDummy_LIBS
  SalomeParallelContainer
  ${COMMON_LIBS}
)

ADD_EXECUTABLE(SALOME_ParallelContainerProxyDummy SALOME_ParallelContainerProxyDummy.cxx)
TARGET_LINK_LIBRARIES(SALOME_ParallelContainerProxyDummy ${SALOME_ParallelContainerProxyDummy_LIBS})

ADD_EXECUTABLE(SALOME_ParallelContainerNodeDummy SALOME_ParallelContainerNodeDummy.cxx)
TARGET_LINK_LIBRARIES(SALOME_ParallelContainerNodeDummy ${SALOME_ParallelContainerProxyDummy_LIBS})

INSTALL(TARGETS SALOME_ParallelContainerNodeDummy SALOME_ParallelContainerProxyDummy DESTINATION ${SALOME_INSTALL_BINS})

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${SALOME_INSTALL_HEADERS})

