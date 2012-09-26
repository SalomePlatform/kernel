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
  ${BOOST_INCLUDE_DIRS}
  ${OMNIORB_INCLUDE_DIRS}
  ${CMAKE_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/salome_adm/unix
  ${CMAKE_SOURCE_DIR}/src/DSC/DSC_User
  ${CMAKE_SOURCE_DIR}/src/DSC/DSC_User/Datastream
  ${CMAKE_SOURCE_DIR}/src/DSC/DSC_User/Datastream/Palm
  ${CMAKE_SOURCE_DIR}/src/DSC/DSC_User/Datastream/Calcium
  ${CMAKE_SOURCE_DIR}/src/DSC/DSC_User/Basic
  ${CMAKE_SOURCE_DIR}/src/DSC/DSC_Basic
  ${CMAKE_SOURCE_DIR}/src/SALOMELocalTrace
  ${CMAKE_SOURCE_DIR}/src/Basics
  ${CMAKE_SOURCE_DIR}/src/Utils
  ${CMAKE_SOURCE_DIR}/src/Container
  ${CMAKE_SOURCE_DIR}/src/GenericObj
  ${CMAKE_SOURCE_DIR}/src/Notification
  ${CMAKE_BINARY_DIR}/idl
  ${CMAKE_BINARY_DIR}/src/DSC/DSC_User/Datastream/Calcium
)

SET(SalomeCalcium_SOURCES
  calcium_port_factory.cxx
  calcium_uses_port.cxx
  calcium_provides_port.cxx
  calcium_repository_types.cxx
  CalciumCouplingPolicy.cxx
  CorbaTypes2CalciumTypes.cxx
  CalciumCxxInterface.cxx
  CalciumTypes2CorbaTypes.cxx
  CalciumCInterface.cxx
  CalciumProvidesPort.cxx
  Calcium.cxx
  calcium_destructors_port_uses.cxx
)
ADD_LIBRARY(SalomeCalcium SHARED ${SalomeCalcium_SOURCES})
SET_TARGET_PROPERTIES(SalomeCalcium PROPERTIES COMPILE_FLAGS "${BOOST_DEFINITIONS} ${OMNIORB_DEFINITIONS} ${PLATFORM_DEFINITIONS}")
TARGET_LINK_LIBRARIES(SalomeCalcium ${PTHREAD_LIBS} ${CORBA_LIBS} ${PLATFORM_LIBS})

SET(CalciumC_SOURCES
  CalciumC.c
  Calcium.cxx
  calciumf.c
)
ADD_LIBRARY(CalciumC SHARED ${CalciumC_SOURCES})
SET_TARGET_PROPERTIES(CalciumC PROPERTIES COMPILE_FLAGS "${BOOST_DEFINITIONS} ${OMNIORB_DEFINITIONS} ${PLATFORM_DEFINITIONS}")

INSTALL(TARGETS CalciumC DESTINATION ${KERNEL_salomelib_LIBS})

ADD_EXECUTABLE(test_DataIdContainer test_DataIdContainer.cxx)
SET_TARGET_PROPERTIES(test_DataIdContainer PROPERTIES COMPILE_FLAGS "${BOOST_DEFINITIONS} ${OMNIORB_DEFINITIONS} ${PLATFORM_DEFINITIONS}")
TARGET_LINK_LIBRARIES(test_DataIdContainer SalomeDSCSuperv SalomeContainer SalomeCalcium OpUtil SALOMELocalTrace ${CORBA_LIBS} ${PLATFORM_LIBS})

CONFIGURE_FILE(calcium_integer_port_uses.hxx.in calcium_integer_port_uses.hxx @ONLY)
CONFIGURE_FILE(CalciumProvidesPort.hxx.in CalciumProvidesPort.hxx @ONLY)
CONFIGURE_FILE(CalciumFortranInt.h.in CalciumFortranInt.h @ONLY)

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
FILE(GLOB COMMON_HEADERS_H "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
SET(COMMON_HEADERS
  ${COMMON_HEADERS_HXX}
  ${COMMON_HEADERS_H}
  calcium.hf  
  ${CMAKE_CURRENT_BINARY_DIR}/calcium_integer_port_uses.hxx
  ${CMAKE_CURRENT_BINARY_DIR}/CalciumProvidesPort.hxx
  ${CMAKE_CURRENT_BINARY_DIR}/CalciumFortranInt.h
)
LIST(REMOVE_ITEM COMMON_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/fortoc.h)
LIST(REMOVE_ITEM COMMON_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/CalciumTypesManipulator.hxx)
INSTALL(FILES ${COMMON_HEADERS} DESTINATION ${KERNEL_salomeinclude_HEADERS})
