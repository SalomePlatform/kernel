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
  ${PTHREAD_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../../Basics
  ${CMAKE_CURRENT_SOURCE_DIR}/../../Basics/Test
  ${CMAKE_CURRENT_SOURCE_DIR}/../../SALOMELocalTrace
  ${CMAKE_CURRENT_SOURCE_DIR}/../../SALOMELocalTrace/Test
  ${CMAKE_CURRENT_SOURCE_DIR}/../../SALOMETraceCollector
)

SET(COMMON_LIBS
  SALOMELocalTraceTest
  SALOMELocalTrace
  SALOMEBasics
  ${PTHREAD_LIBRARIES}
  ${PLATFORM_LIBS}
  ${CPPUNIT_LIBRARIES}
)

ADD_DEFINITIONS(${CPPUNIT_DEFINITIONS})
SET(LOCAL_TEST_DIR ${KERNEL_TEST_DIR}/SALOMETraceCollector)

ADD_LIBRARY(SALOMETraceCollectorTest SALOMETraceCollectorTest.cxx)
TARGET_LINK_LIBRARIES(SALOMETraceCollectorTest ${COMMON_LIBS})
INSTALL(TARGETS SALOMETraceCollectorTest DESTINATION ${KERNEL_TEST_LIB})

ADD_EXECUTABLE(TestSALOMETraceCollector TestSALOMETraceCollector.cxx)
TARGET_LINK_LIBRARIES(TestSALOMETraceCollector SALOMETraceCollectorTest ${COMMON_LIBS})
INSTALL(TARGETS TestSALOMETraceCollector DESTINATION ${LOCAL_TEST_DIR})

# Executable scripts to be installed
INSTALL(FILES TestSALOMETraceCollector.py DESTINATION ${LOCAL_TEST_DIR})

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${LOCAL_TEST_DIR})

INSTALL(FILES CTestTestfileInstall.cmake
        DESTINATION ${LOCAL_TEST_DIR}
        RENAME CTestTestfile.cmake)