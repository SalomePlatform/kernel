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
  ${PTHREAD_INCLUDE_DIRS}
  ${CPPUNIT_INCLUDE_DIRS}
  ${OMNIORB_INCLUDE_DIR}
  ${CMAKE_BINARY_DIR}/salome_adm
  ${CMAKE_CURRENT_SOURCE_DIR}/../../NamingService
  ${CMAKE_CURRENT_SOURCE_DIR}/../../Basics
  ${CMAKE_CURRENT_SOURCE_DIR}/../../SALOMELocalTrace
  ${CMAKE_CURRENT_SOURCE_DIR}/../../Utils
  ${CMAKE_CURRENT_SOURCE_DIR}/../../LifeCycleCORBA
  ${CMAKE_BINARY_DIR}/idl
  ${CMAKE_CURRENT_SOURCE_DIR}/../../SALOMELocalTrace/Test
  ${CMAKE_CURRENT_SOURCE_DIR}/../../SALOMETraceCollector/Test
  ${CMAKE_CURRENT_SOURCE_DIR}/../../Utils/Test
  ${CMAKE_CURRENT_SOURCE_DIR}/../../Basics/Test
)

# This local variable defines the list of dependant libraries common to all target in this package.
SET(COMMON_LIBS
    ${CPPUNIT_LIBS}
    SalomeNS
    SalomeLifeCycleCORBA
    ${OMNIORB_LIBRARIES}
    ${PLATFORM_LIBS}
)

ADD_DEFINITIONS(${CPPUNIT_DEFINITIONS} ${OMNIORB_DEFINITIONS})

ADD_LIBRARY(NamingServiceTest NamingServiceTest.cxx)
TARGET_LINK_LIBRARIES(NamingServiceTest ${COMMON_LIBS})
INSTALL(TARGETS NamingServiceTest DESTINATION ${KERNEL_salomelib_LIBS})

SET(TestNamingService_LIBS
  NamingServiceTest
  SalomeNS
  SALOMELocalTraceTest
  SALOMELocalTrace
  SALOMETraceCollectorTest
  UtilsTest
  OpUtil
  SALOMEBasics
  SalomeIDLKernel
  ${COMMON_LIBS}
)

ADD_EXECUTABLE(TestNamingService TestNamingService.cxx)
TARGET_LINK_LIBRARIES(TestNamingService ${TestNamingService_LIBS})
INSTALL(TARGETS TestNamingService DESTINATION ${KERNEL_salomebin_BINS})

# Executable scripts to be installed
SALOME_INSTALL_SCRIPTS(TestNamingService.py ${KERNEL_salomescript_PYTHON})

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${KERNEL_salomeinclude_HEADERS})
