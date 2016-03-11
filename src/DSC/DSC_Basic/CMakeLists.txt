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
  ${PROJECT_SOURCE_DIR}/src/Container
  ${PROJECT_SOURCE_DIR}/src/GenericObj
  ${PROJECT_SOURCE_DIR}/src/Notification
  ${PROJECT_SOURCE_DIR}/src/SALOMELocalTrace
  ${PROJECT_SOURCE_DIR}/src/Basics
  ${PROJECT_SOURCE_DIR}/src/NamingService
  ${PROJECT_SOURCE_DIR}/src/Utils
  ${PROJECT_BINARY_DIR}/idl
)

SET(SalomeDSCContainer_SOURCES
  DSC_i.cxx
  DSC_interface.cxx
  PortProperties_i.cxx
)

ADD_DEFINITIONS(${OMNIORB_DEFINITIONS})

ADD_LIBRARY(SalomeDSCContainer ${SalomeDSCContainer_SOURCES})
TARGET_LINK_LIBRARIES(SalomeDSCContainer SalomeContainer ${OMNIORB_LIBRARIES})
INSTALL(TARGETS SalomeDSCContainer EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

SET(SALOME_ConnectionManagerServer_SOURCES
  SALOME_ConnectionManagerServer.cxx
  ConnectionManager_i.cxx
)

SET(SALOME_ConnectionManagerServer_LIBS
  SalomeIDLKernel
  SalomeNS
  SALOMELocalTrace
  SalomeContainer 
  ${OMNIORB_LIBRARIES}
)

ADD_EXECUTABLE(SALOME_ConnectionManagerServer ${SALOME_ConnectionManagerServer_SOURCES})
TARGET_LINK_LIBRARIES(SALOME_ConnectionManagerServer ${SALOME_ConnectionManagerServer_LIBS})
INSTALL(TARGETS SALOME_ConnectionManagerServer EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_BINS})

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${SALOME_INSTALL_HEADERS})
