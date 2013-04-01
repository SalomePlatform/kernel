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
  ${CPPUNIT_INCLUDE_DIRS}
  ${CMAKE_BINARY_DIR}/salome_adm
  ${CMAKE_CURRENT_SOURCE_DIR}/../Basics
  ${CMAKE_CURRENT_SOURCE_DIR}/../Basics/Test
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMELocalTrace
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMELocalTrace/Test
)

# ===============================================================
# Files to be installed
# ===============================================================

# Executable scripts to be installed
SALOME_INSTALL_SCRIPTS(UnitTests.py ${KERNEL_salomescript_SCRIPTS})

# ===============================================================
# Executables targets
# ===============================================================

ADD_DEFINITIONS(${CPPUNIT_DEFINITIONS})

SET(COMMON_LIBS
  ${CPPUNIT_LIBS}
  SALOMEBasics
  SALOMELocalTrace
  SALOMELocalTraceTest
)

IF(SALOME_USE_CORBA)
  INCLUDE_DIRECTORIES(
    ${OMNIORB_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMETraceCollector
    ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMETraceCollector/Test
    ${CMAKE_CURRENT_SOURCE_DIR}/../NamingService
    ${CMAKE_CURRENT_SOURCE_DIR}/../NamingService/Test
    ${CMAKE_CURRENT_SOURCE_DIR}/../Utils
    ${CMAKE_CURRENT_SOURCE_DIR}/../Utils/Test
    ${CMAKE_CURRENT_SOURCE_DIR}/../LifeCycleCORBA
    ${CMAKE_CURRENT_SOURCE_DIR}/../LifeCycleCORBA/Test
    ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMDESImpl
    ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMEDSImpl/Test
    ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMDES
    ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMEDS/Test
    ${CMAKE_BINARY_DIR}/idl
    ${CMAKE_CURRENT_SOURCE_DIR}/../Registry
    ${CMAKE_CURRENT_SOURCE_DIR}/../Notification
    ${CMAKE_CURRENT_SOURCE_DIR}/../ResourcesManager
  )
  ADD_DEFINITIONS(-DWITH_CORBA ${OMNIORB_DEFINITIONS})

  SET(COMMON_LIBS
    ${COMMON_LIBS}
    Registry
    SalomeNotification
    SalomeResourcesManager
    SalomeNS
    NamingServiceTest
    SalomeContainer
    SALOMETraceCollectorTest
    OpUtil
    UtilsTest
    SalomeLifeCycleCORBA
    LifeCycleCORBATest
    SalomeDSImpl
    SALOMEDSImplTest
    SalomeDS
    SALOMEDSTest
    SalomeIDLKernel
    ${OMNIORB_LIBRARIES}
  )
ENDIF(SALOME_USE_CORBA)

ADD_EXECUTABLE(UnitTests UnitTests.cxx)
TARGET_LINK_LIBRARIES(UnitTests ${COMMON_LIBS})
INSTALL(TARGETS UnitTests DESTINATION ${KERNEL_salomebin_BINS})
