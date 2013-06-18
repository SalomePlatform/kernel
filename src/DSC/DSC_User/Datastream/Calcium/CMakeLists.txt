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

INCLUDE_DIRECTORIES(
  ${Boost_INCLUDE_DIR}
  ${OMNIORB_INCLUDE_DIR}
  ${CMAKE_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/salome_adm
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

ADD_DEFINITIONS(${BOOST_DEFINITIONS} ${OMNIORB_DEFINITIONS})

ADD_LIBRARY(SalomeCalcium ${SalomeCalcium_SOURCES})
TARGET_LINK_LIBRARIES(SalomeCalcium SalomeDSCSuperv SalomeContainer ${OMNIORB_LIBRARIES} ${PLATFORM_LIBS})

INSTALL(TARGETS SalomeCalcium DESTINATION ${SALOME_INSTALL_LIBS})

SET(CalciumC_SOURCES
  CalciumC.c
  Calcium.cxx
  calciumf.c
)
ADD_LIBRARY(CalciumC ${CalciumC_SOURCES})
TARGET_LINK_LIBRARIES(CalciumC SalomeDSCSuperv SalomeCalcium)

INSTALL(TARGETS CalciumC DESTINATION ${SALOME_INSTALL_LIBS})

ADD_EXECUTABLE(test_DataIdContainer_Calcium test_DataIdContainer.cxx)
TARGET_LINK_LIBRARIES(test_DataIdContainer_Calcium SalomeDSCSuperv SalomeContainer SalomeCalcium OpUtil SALOMELocalTrace ${OMNIORB_LIBRARIES} ${PLATFORM_LIBS})

CONFIGURE_FILE(calcium_integer_port_uses.hxx.in calcium_integer_port_uses.hxx @ONLY)
CONFIGURE_FILE(CalciumProvidesPort.hxx.in CalciumProvidesPort.hxx @ONLY)
CONFIGURE_FILE(CalciumFortranInt.h.in CalciumFortranInt.h @ONLY)

SET(COMMON_HEADERS
  Calcium.hxx
  CalciumCInterface.hxx
  CalciumCouplingPolicy.hxx
  CalciumCxxInterface.hxx
  CalciumException.hxx
  CalciumGenericProvidesPort.hxx
  CalciumGenericUsesPort.hxx
  CalciumInterface.hxx
  CalciumMacroCInterface.hxx
  CalciumPortTraits.hxx
  CalciumTypes.hxx
  CalciumTypes2CorbaTypes.hxx
  Copy2CorbaSpace.hxx
  Copy2UserSpace.hxx
  CorbaTypes2CalciumTypes.hxx
  calcium_complex_port_uses.hxx
  calcium_double_port_uses.hxx
  calcium_intc_port_uses.hxx
  calcium_logical_port_uses.hxx
  calcium_long_port_uses.hxx
  calcium_port_factory.hxx
  calcium_provides_port.hxx
  calcium_real_port_uses.hxx
  calcium_string_port_uses.hxx
  calcium_uses_port.hxx
  calcium.h
  calcium.hf
  calciumE.h
  calciumP.h
  calciumf.h
  fortoc.h
  version.h
  ${CMAKE_CURRENT_BINARY_DIR}/CalciumFortranInt.h
  ${CMAKE_CURRENT_BINARY_DIR}/CalciumProvidesPort.hxx
  ${CMAKE_CURRENT_BINARY_DIR}/calcium_integer_port_uses.hxx
)
INSTALL(FILES ${COMMON_HEADERS} DESTINATION ${SALOME_INSTALL_HEADERS})
