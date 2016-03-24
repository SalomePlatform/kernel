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
  ${PACO_INCLUDE_DIRS}
  ${OMNIORB_INCLUDE_DIR}
  ${PROJECT_SOURCE_DIR}/src/Container
  ${PROJECT_SOURCE_DIR}/src/ParallelContainer
  ${PROJECT_SOURCE_DIR}/src/Notification
  ${PROJECT_SOURCE_DIR}/src/SALOMELocalTrace
  ${PROJECT_SOURCE_DIR}/src/Basics
  ${PROJECT_SOURCE_DIR}/src/NamingService
  ${PROJECT_SOURCE_DIR}/src/Utils
  ${PROJECT_SOURCE_DIR}/src/DSC/DSC_Basic
  ${PROJECT_SOURCE_DIR}/src/GenericObj
  ${PROJECT_BINARY_DIR}/idl
  ${PROJECT_SOURCE_DIR}/src/Registry
  ${PROJECT_SOURCE_DIR}/src/Utils
  ${PROJECT_BINARY_DIR}/salome_adm
)

ADD_DEFINITIONS(
  ${OMNIORB_DEFINITIONS}
  ${MPI_DEFINITIONS}
)
SET(SalomeParallelDSCContainer_SOURCES
    ParallelDSC_i.cxx
    Param_Double_Port_provides_i.cxx
    Param_Double_Port_uses_i.cxx
)

ADD_LIBRARY(SalomeParallelDSCContainer ${SalomeParallelDSCContainer_SOURCES})
ADD_DEPENDENCIES(SalomeParallelDSCContainer SalomeParallelIDLKernel)
INSTALL(TARGETS SalomeParallelDSCContainer EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${SALOME_INSTALL_HEADERS})
