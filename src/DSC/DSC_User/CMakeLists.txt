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

ADD_SUBDIRECTORY(Basic)
ADD_SUBDIRECTORY(Datastream)

INCLUDE_DIRECTORIES(
  ${Boost_INCLUDE_DIRS}
  ${OMNIORB_INCLUDE_DIR}
  ${CMAKE_BINARY_DIR}/salome_adm
  ${CMAKE_SOURCE_DIR}/src/Container
  ${CMAKE_SOURCE_DIR}/src/GenericObj
  ${CMAKE_SOURCE_DIR}/src/Notification
  ${CMAKE_SOURCE_DIR}/src/DSC/DSC_Basic
  ${CMAKE_SOURCE_DIR}/src/SALOMELocalTrace
  ${CMAKE_SOURCE_DIR}/src/Basics
  ${CMAKE_SOURCE_DIR}/src/Utils
  ${CMAKE_BINARY_DIR}/idl
)

ADD_DEFINITIONS(${OMNIORB_DEFINITIONS} ${BOOST_DEFINITIONS})

SET(SalomeDSCSuperv_SOURCES
  base_port.cxx
  uses_port.cxx
  provides_port.cxx
  Superv_Component_i.cxx
)

ADD_LIBRARY(SalomeDSCSuperv ${SalomeDSCSuperv_SOURCES})
TARGET_LINK_LIBRARIES(SalomeDSCSuperv SalomeDSCContainer ${PLATFORM_LIBS})
INSTALL(TARGETS SalomeDSCSuperv DESTINATION ${KERNEL_salomelib_LIBS})

ADD_EXECUTABLE(test_DSC_Exception test_DSC_Exception.cxx)
TARGET_LINK_LIBRARIES(test_DSC_Exception OpUtil SALOMELocalTrace ${OMNIORB_LIBRARIES} ${PLATFORM_LIBS})

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${KERNEL_salomeinclude_HEADERS})
