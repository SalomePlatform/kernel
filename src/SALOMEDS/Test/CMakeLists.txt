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
  ${PTHREAD_INCLUDE_DIRS}
  ${CPPUNIT_INCLUDE_DIRS}
  ${OMNIORB_INCLUDE_DIRS}
  ${BOOST_INCLUDE_DIRS}
  ${CMAKE_BINARY_DIR}/salome_adm/unix
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
  ${CMAKE_CURRENT_SOURCE_DIR}/../../SALOMEDS
  ${CMAKE_CURRENT_SOURCE_DIR}/../../SALOMEDSClient
  ${CMAKE_CURRENT_SOURCE_DIR}/../../DF
  ${CMAKE_CURRENT_SOURCE_DIR}/../../SALOMEDSImpl
  ${CMAKE_CURRENT_SOURCE_DIR}/../../SALOMEDSImpl/Test
  ${CMAKE_BINARY_DIR}/idl
)

# This local variable defines the list of dependant libraries common to all target in this package.
SET(COMMON_LIBS
 ${CPPUNIT_LIBS}
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
 SALOMEDSImplTest
 DF
 SalomeDSImpl
 SalomeDSClient
 SalomeDS
 SalomeIDLKernel
)

SET(COMMON_FLAGS "${CPPUNIT_DEFINITIONS} ${OMNIORB_DEFINITIONS} ${BOOST_DEFINITIONS} ${PLATFORM_DEFINITIONS}")
IF(WINDOWS)
  SET(COMMON_FLAGS "${COMMON_FLAGS} -DNOGDI")
ENDIF(WINDOWS)

ADD_LIBRARY(SALOMEDSTest SHARED SALOMEDSTest.cxx)
SET_TARGET_PROPERTIES(SALOMEDSTest PROPERTIES COMPILE_FLAGS "${COMMON_FLAGS}")
TARGET_LINK_LIBRARIES(SALOMEDSTest ${COMMON_LIBS})
INSTALL(TARGETS SALOMEDSTest DESTINATION ${KERNEL_salomelib_LIBS})

ADD_EXECUTABLE(TestSALOMEDS TestSALOMEDS.cxx)
SET_TARGET_PROPERTIES(TestSALOMEDS PROPERTIES COMPILE_FLAGS "${CPPUNIT_DEFINITIONS} ${OMNIORB_DEFINITIONS} ${BOOST_DEFINITIONS} ${PLATFORM_DEFINITIONS}")
TARGET_LINK_LIBRARIES(TestSALOMEDS SALOMEDSTest SALOMEBasics ${COMMON_LIBS} ${OMNIORB_LIBS})
INSTALL(TARGETS TestSALOMEDS DESTINATION ${KERNEL_salomebin_BINS})

# Executable scripts to be installed
SALOME_INSTALL_SCRIPTS(TestSALOMEDS.py ${KERNEL_salomescript_PYTHON})

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${KERNEL_salomeinclude_HEADERS})
