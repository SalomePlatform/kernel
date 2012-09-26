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
  ${CMAKE_BINARY_DIR}/salome_adm/unix
  ${CMAKE_SOURCE_DIR}/src/DSC/DSC_User
  ${CMAKE_SOURCE_DIR}/src/DSC/DSC_User/Datastream
  ${CMAKE_SOURCE_DIR}/src/DSC/DSC_Basic
  ${CMAKE_SOURCE_DIR}/src/Container
  ${CMAKE_SOURCE_DIR}/src/GenericObj
  ${CMAKE_SOURCE_DIR}/src/Notification
  ${CMAKE_SOURCE_DIR}/src/SALOMELocalTrace
  ${CMAKE_SOURCE_DIR}/src/Basics
  ${CMAKE_SOURCE_DIR}/src/Utils
  ${CMAKE_BINARY_DIR}/idl
)

ADD_LIBRARY(SalomePalm SHARED palm_port_factory.cxx)
SET_TARGET_PROPERTIES(SalomePalm PROPERTIES COMPILE_FLAGS "${OMNIORB_DEFINITIONS}")

ADD_EXECUTABLE(test_DataIdContainer test_DataIdContainer.cxx)
SET_TARGET_PROPERTIES(test_DataIdContainer PROPERTIES COMPILE_FLAGS "${OMNIORB_DEFINITIONS}")
TARGET_LINK_LIBRARIES(test_DataIdContainer ${OMNIORB_LIBS})

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${KERNEL_salomeinclude_HEADERS})

