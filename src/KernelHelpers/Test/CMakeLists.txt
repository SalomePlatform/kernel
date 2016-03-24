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
  ${CPPUNIT_INCLUDE_DIRS}
  ${OMNIORB_INCLUDE_DIR}
  ${PTHREAD_INCLUDE_DIR}
  ${PROJECT_BINARY_DIR}/salome_adm
  ${PROJECT_SOURCE_DIR}/src/Basics/Test
  ${CMAKE_CURRENT_SOURCE_DIR}/..
  ${PROJECT_SOURCE_DIR}/src/NamingService
  ${PROJECT_SOURCE_DIR}/src/SALOMELocalTrace
  ${PROJECT_SOURCE_DIR}/src/Basics
  ${PROJECT_SOURCE_DIR}/src/Utils
  ${PROJECT_SOURCE_DIR}/src/LifeCycleCORBA
  ${PROJECT_SOURCE_DIR}/src/Container
  ${PROJECT_SOURCE_DIR}/src/Notification
  ${PROJECT_SOURCE_DIR}/src/GenericObj
  ${PROJECT_BINARY_DIR}/idl
)

SET(SalomeKernelHelpersTest_LIBS
  ${CPPUNIT_LIBRARIES}
  SalomeKernelHelpers
  ${OMNIORB_LIBRARIES}
  SalomeNS
  SALOMELocalTrace
  SALOMEBasics
  OpUtil
  SalomeLifeCycleCORBA
  SalomeContainer
  SalomeNotification
  SalomeGenericObj
  SalomeIDLKernel
)

ADD_DEFINITIONS(${CPPUNIT_DEFINITIONS} ${OMNIORB_DEFINITIONS})

SET(LOCAL_TEST_DIR ${KERNEL_TEST_DIR}/KernelHelpers)

ADD_LIBRARY(SalomeKernelHelpersTest KernelHelpersUnitTests.cxx)
TARGET_LINK_LIBRARIES(SalomeKernelHelpersTest ${SalomeKernelHelpersTest_LIBS})
INSTALL(TARGETS SalomeKernelHelpersTest DESTINATION ${KERNEL_TEST_LIB})

ADD_EXECUTABLE(TestKernelHelpers TestKernelHelpers.cxx)
TARGET_LINK_LIBRARIES(TestKernelHelpers SalomeKernelHelpersTest ${SalomeKernelHelpersTest_LIBS})
#ADD_TEST(TestKernelHelpers TestKernelHelpers)
INSTALL(TARGETS TestKernelHelpers DESTINATION ${LOCAL_TEST_DIR})

# Executable scripts to be installed
INSTALL(FILES TestKernelHelpers.py DESTINATION ${LOCAL_TEST_DIR})

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${LOCAL_TEST_DIR})

INSTALL(FILES CTestTestfileInstall.cmake
        DESTINATION ${LOCAL_TEST_DIR}
        RENAME CTestTestfile.cmake)
