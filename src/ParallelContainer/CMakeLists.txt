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
  ${OMNIORB_INCLUDE_DIRS}
  ${MPI_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_DIRS}
  ${PACO_INCLUDE_DIRS}
  ${CMAKE_BINARY_DIR}/salome_adm/unix
  ${CMAKE_SOURCE_DIR}/src/Container
  ${CMAKE_SOURCE_DIR}/src/Notification
  ${CMAKE_SOURCE_DIR}/src/SALOMELocalTrace
  ${CMAKE_SOURCE_DIR}/src/Basics
  ${CMAKE_SOURCE_DIR}/src/NamingService
  ${CMAKE_SOURCE_DIR}/src/Registry
  ${CMAKE_SOURCE_DIR}/src/Utils
  ${CMAKE_SOURCE_DIR}/src/GenericObj
  ${CMAKE_CURRENT_SOURCE_DIR}/../HDFPersist
  ${CMAKE_BINARY_DIR}/idl
  ${CMAKE_SOURCE_DIR}/src/SALOMETraceCollector
  ${PACOPATH}/lib
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
  ${CORBA_LIBS}
  ${PACO_LIBS}
  ${PYTHON_LIBS}
)

SET(SalomeParallelContainer_SOURCES
  SALOME_ParallelComponent_i.cxx
  SALOME_ParallelContainer_i.cxx
  SALOME_ParallelContainerProxy_i.cxx
  Parallel_Salome_file_i.cxx
  SALOME_ParallelGlobalProcessVar_i.cxx
)

ADD_LIBRARY(SalomeParallelContainer SHARED ${SalomeParallelContainer_SOURCES})
SET_TARGET_PROPERTIES(SalomeParallelContainer PROPERTIES COMPILE_FLAGS "${ONMIORB_DEFINITIONS} ${MPI_DEFINITIONS} ${PACO_DEFINITIONS}")
TARGET_LINK_LIBRARIES(SalomeParallelContainer ${COMMON_LIBS} ${MPI_LIBS})
INSTALL(TARGETS Registry DESTINATION ${KERNEL_salomelib_LIBS})

IF(MPI_IS_OK)
  SET(SALOME_ParallelContainerProxyMpi_LIBS
    SalomeParallelContainer
    ${PaCO_dummy}
    ${PaCO_mpi}
    ${PaCO_omnithread}
    ${MPI_LIBS}
    ${COMMON_LIBS}
  )
  ADD_EXECUTABLE(SALOME_ParallelContainerProxyMpi SALOME_Registry_Server.cxx)
  SET_TARGET_PROPERTIES(SALOME_ParallelContainerProxyMpi PROPERTIES COMPILE_FLAGS "${ONMIORB_DEFINITIONS} ${MPI_DEFINITIONS} ${PACO_DEFINITIONS}")
  TARGET_LINK_LIBRARIES(SALOME_ParallelContainerProxyMpi ${SALOME_ParallelContainerProxyMpi_LIBS})

  ADD_EXECUTABLE(SALOME_ParallelContainerNodeMpi SALOME_ParallelContainerNodeMpi.cxx)
  SET_TARGET_PROPERTIES(SALOME_ParallelContainerNodeMpi PROPERTIES COMPILE_FLAGS "${ONMIORB_DEFINITIONS} ${MPI_DEFINITIONS} ${PACO_DEFINITIONS}")
  TARGET_LINK_LIBRARIES(SALOME_ParallelContainerNodeMpi ${SALOME_ParallelContainerProxyMpi_LIBS})

  INSTALL(TARGETS SALOME_ParallelContainerProxyMpi SALOME_ParallelContainerNodeMpi DESTINATION ${KERNEL_salomebin_BINS})
ENDIF(MPI_IS_OK)

SET(SALOME_ParallelContainerProxyDummy_LIBS
  SalomeParallelContainer
  ${PaCO_dummy}
  ${PaCO_omnithread}
  ${COMMON_LIBS}
)

ADD_EXECUTABLE(SALOME_ParallelContainerProxyDummy SALOME_ParallelContainerProxyDummy.cxx)
SET_TARGET_PROPERTIES(SALOME_ParallelContainerProxyDummy PROPERTIES COMPILE_FLAGS "${ONMIORB_DEFINITIONS} ${MPI_DEFINITIONS} ${PACO_DEFINITIONS}")
TARGET_LINK_LIBRARIES(SALOME_ParallelContainerProxyDummy ${SALOME_ParallelContainerProxyDummy_LIBS})

ADD_EXECUTABLE(SALOME_ParallelContainerNodeDummy SALOME_ParallelContainerNodeDummy.cxx)
SET_TARGET_PROPERTIES(SALOME_ParallelContainerNodeDummy PROPERTIES COMPILE_FLAGS "${ONMIORB_DEFINITIONS} ${MPI_DEFINITIONS} ${PACO_DEFINITIONS}")
TARGET_LINK_LIBRARIES(SALOME_ParallelContainerNodeDummy ${SALOME_ParallelContainerProxyDummy_LIBS})

INSTALL(TARGETS SALOME_ParallelContainerNodeDummy SALOME_ParallelContainerProxyDummy DESTINATION ${KERNEL_salomebin_BINS})

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${KERNEL_salomeinclude_HEADERS})

