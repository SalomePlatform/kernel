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
  ${Boost_INCLUDE_DIRS}
  ${OMNIORB_INCLUDE_DIR}
  ${PTHREAD_INCLUDE_DIR}
  ${PROJECT_BINARY_DIR}/salome_adm
  ${PROJECT_SOURCE_DIR}/src/Container
  ${PROJECT_SOURCE_DIR}/src/GenericObj
  ${PROJECT_SOURCE_DIR}/src/Notification
  ${PROJECT_SOURCE_DIR}/src/DSC/DSC_Basic
  ${PROJECT_SOURCE_DIR}/src/SALOMELocalTrace
  ${PROJECT_SOURCE_DIR}/src/Basics
  ${PROJECT_SOURCE_DIR}/src/Utils
  ${PROJECT_BINARY_DIR}/idl
)

ADD_SUBDIRECTORY(Basic)
ADD_SUBDIRECTORY(Datastream)

ADD_DEFINITIONS(${OMNIORB_DEFINITIONS} ${BOOST_DEFINITIONS})

SET(SalomeDSCSuperv_SOURCES
  base_port.cxx
  uses_port.cxx
  provides_port.cxx
  Superv_Component_i.cxx
)

ADD_LIBRARY(SalomeDSCSuperv ${SalomeDSCSuperv_SOURCES})
TARGET_LINK_LIBRARIES(SalomeDSCSuperv SalomeDSCContainer ${PLATFORM_LIBS})
INSTALL(TARGETS SalomeDSCSuperv EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

ADD_EXECUTABLE(test_DSC_Exception test_DSC_Exception.cxx)
TARGET_LINK_LIBRARIES(test_DSC_Exception OpUtil SALOMELocalTrace 
    ${OMNIORB_LIBRARIES} ${PLATFORM_LIBS} ${PTHREAD_LIBRARIES})

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${SALOME_INSTALL_HEADERS})
