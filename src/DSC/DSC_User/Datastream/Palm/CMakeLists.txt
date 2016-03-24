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
  ${PROJECT_BINARY_DIR}/salome_adm
  ${PROJECT_SOURCE_DIR}/src/DSC/DSC_User
  ${PROJECT_SOURCE_DIR}/src/DSC/DSC_User/Datastream
  ${PROJECT_SOURCE_DIR}/src/DSC/DSC_Basic
  ${PROJECT_SOURCE_DIR}/src/Container
  ${PROJECT_SOURCE_DIR}/src/GenericObj
  ${PROJECT_SOURCE_DIR}/src/Notification
  ${PROJECT_SOURCE_DIR}/src/SALOMELocalTrace
  ${PROJECT_SOURCE_DIR}/src/Basics
  ${PROJECT_SOURCE_DIR}/src/Utils
  ${PROJECT_BINARY_DIR}/idl
)


ADD_DEFINITIONS(${OMNIORB_DEFINITIONS})

ADD_LIBRARY(SalomePalm palm_port_factory.cxx)
TARGET_LINK_LIBRARIES(SalomePalm SalomeDSCSuperv ${OMNIORB_LIBRARIES})

INSTALL(TARGETS SalomePalm EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

ADD_EXECUTABLE(test_DataIdContainer_Palm test_DataIdContainer.cxx)
ADD_DEPENDENCIES(test_DataIdContainer_Palm SalomeIDLKernel)
TARGET_LINK_LIBRARIES(test_DataIdContainer_Palm ${OMNIORB_LIBRARIES})

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${SALOME_INSTALL_HEADERS})

