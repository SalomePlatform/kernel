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
  ${PTHREAD_INCLUDE_DIR}
  ${HDF5_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_DIRS}
  ${PROJECT_BINARY_DIR}/salome_adm
  ${CMAKE_CURRENT_SOURCE_DIR}/../Basics
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMELocalTrace
  ${CMAKE_CURRENT_SOURCE_DIR}/../NamingService
  ${CMAKE_CURRENT_SOURCE_DIR}/../Utils
  ${CMAKE_CURRENT_SOURCE_DIR}/../Registry
  ${CMAKE_CURRENT_SOURCE_DIR}/../Notification
  ${CMAKE_CURRENT_SOURCE_DIR}/../ResourcesManager
  ${CMAKE_CURRENT_SOURCE_DIR}/../HDFPersist
  ${CMAKE_CURRENT_SOURCE_DIR}/../GenericObj
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMETraceCollector/Test
  ${PROJECT_BINARY_DIR}/idl
)

SET(SCRIPTS
  SALOME_ComponentPy.py
  SALOME_PyNode.py
  SALOME_Container.py
)

ADD_DEFINITIONS(${HDF5_DEFINITIONS} ${OMNIORB_DEFINITIONS})
SET(COMMON_LIBS
  Registry
  SalomeNotification
  SalomeResourcesManager
  ResourcesManager
  SalomeNS
  OpUtil
  SALOMELocalTrace
  SALOMEBasics
  SalomeHDFPersist
  SalomeGenericObj
  SalomeIDLKernel
  ${OMNIORB_LIBRARIES}
  ${PYTHON_LIBRARIES}
)

IF(WITH_MPI_SEQ_CONTAINER)
  ADD_DEFINITIONS(${MPI_DEFINITIONS})
  SET(COMMON_LIBS
    ${MPI_CXX_LIBRARIES}
  )
ENDIF(WITH_MPI_SEQ_CONTAINER)

IF(SALOME_PACO_PARALLEL)
  INCLUDE_DIRECTORIES(${PACO_INCLUDE_DIRS})
  ADD_DEFINITIONS(-DWITH_PACO_PARALLEL)
  LIST(APPEND COMMON_LIBS ${PACO_LIBRARIES})
ENDIF()

SET(SalomeContainer_SOURCES
  Component_i.cxx
  Container_i.cxx
  SALOME_FileTransfer_i.cxx
  SALOME_FileRef_i.cxx
  Container_init_python.cxx
  SALOME_ContainerManager.cxx
  Salome_file_i.cxx
)

ADD_LIBRARY(SalomeContainer ${SalomeContainer_SOURCES})
TARGET_LINK_LIBRARIES(SalomeContainer ${COMMON_LIBS} ${PTHREAD_LIBRARIES})
INSTALL(TARGETS SalomeContainer EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

SET(SALOME_Container_LIBS
  SalomeContainer
  SALOMEBasics
  ${COMMON_LIBS}
  ${HDF5_LIBRARIES}
  ${MPI_LIBRARIES}
)

ADD_EXECUTABLE(SALOME_Container SALOME_Container.cxx SALOME_Container_SignalsHandler.cxx)
TARGET_LINK_LIBRARIES(SALOME_Container ${SALOME_Container_LIBS})

IF(SALOME_BUILD_TESTS)
  ADD_EXECUTABLE(TestSalome_file TestSalome_file.cxx)
  TARGET_LINK_LIBRARIES(TestSalome_file SALOMETraceCollectorTest ${SALOME_Container_LIBS})
ENDIF()
INSTALL(TARGETS SALOME_Container DESTINATION ${SALOME_INSTALL_BINS})

# Executable scripts to be installed
SALOME_INSTALL_SCRIPTS("${SCRIPTS}" ${SALOME_INSTALL_SCRIPT_PYTHON})
FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${SALOME_INSTALL_HEADERS})
