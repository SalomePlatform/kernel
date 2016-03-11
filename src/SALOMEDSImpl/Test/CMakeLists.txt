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
  ${Boost_INCLUDE_DIR}
  ${OMNIORB_INCLUDE_DIR}
  ${PTHREAD_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../../Basics
  ${CMAKE_CURRENT_SOURCE_DIR}/../../Basics/Test
  ${CMAKE_CURRENT_SOURCE_DIR}/../../SALOMELocalTrace
  ${CMAKE_CURRENT_SOURCE_DIR}/../../SALOMELocalTrace/Test
  ${CMAKE_CURRENT_SOURCE_DIR}/../../SALOMETraceCollector
  ${CMAKE_CURRENT_SOURCE_DIR}/../../SALOMETraceCollector/Test
  ${CMAKE_CURRENT_SOURCE_DIR}/../../NamingService
  ${CMAKE_CURRENT_SOURCE_DIR}/../../NamingService/Test
  ${CMAKE_CURRENT_SOURCE_DIR}/../../Utils
  ${CMAKE_CURRENT_SOURCE_DIR}/../../Utils/Test
  ${CMAKE_CURRENT_SOURCE_DIR}/../../ResourcesManager
  ${CMAKE_CURRENT_SOURCE_DIR}/../../LifeCycleCORBA
  ${CMAKE_CURRENT_SOURCE_DIR}/../../DF
  ${CMAKE_CURRENT_SOURCE_DIR}/../../SALOMEDSImpl
  ${PROJECT_BINARY_DIR}/idl
)

# This local variable defines the list of dependant libraries common to all target in this package.
SET(COMMON_LIBS
  ${CPPUNIT_LIBRARIES}
  SALOMEBasics
  SalomeResourcesManager
  SalomeContainer
  SalomeNS
  Registry
  SalomeNotification
  UtilsTest
  OpUtil
  SALOMELocalTraceTest
  SALOMELocalTrace
  SALOMETraceCollectorTest
  DF
  SalomeDSImpl
  SalomeHDFPersist
  SalomeIDLKernel
)  

ADD_DEFINITIONS(${CPPUNIT_DEFINITIONS} ${OMNIORB_DEFINITIONS} ${BOOST_DEFINITIONS})
SET(LOCAL_TEST_DIR ${KERNEL_TEST_DIR}/SALOMEDSImpl)

ADD_LIBRARY(SALOMEDSImplTest SALOMEDSImplTest.cxx)
TARGET_LINK_LIBRARIES(SALOMEDSImplTest ${COMMON_LIBS})
INSTALL(TARGETS SALOMEDSImplTest DESTINATION ${KERNEL_TEST_LIB})

ADD_EXECUTABLE(TestSALOMEDSImpl TestSALOMEDSImpl.cxx)
TARGET_LINK_LIBRARIES(TestSALOMEDSImpl SALOMEDSImplTest SalomeDSImpl NamingServiceTest ${COMMON_LIBS})
INSTALL(TARGETS TestSALOMEDSImpl DESTINATION ${LOCAL_TEST_DIR})

# Executable scripts to be installed
INSTALL(FILES TestSALOMEDSImpl.py DESTINATION ${LOCAL_TEST_DIR})

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${LOCAL_TEST_DIR})

INSTALL(FILES CTestTestfileInstall.cmake
        DESTINATION ${LOCAL_TEST_DIR}
        RENAME CTestTestfile.cmake)