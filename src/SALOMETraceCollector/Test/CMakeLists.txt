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
  ${CMAKE_THREAD_LIBS_INIT}
  ${PLATFORM_LIBS}
)

ADD_DEFINITIONS(${CPPUNIT_DEFINITIONS} ${PLATFORM_DEFINITIONS})

ADD_LIBRARY(SALOMETraceCollectorTest SALOMETraceCollectorTest.cxx)
TARGET_LINK_LIBRARIES(SALOMETraceCollectorTest ${COMMON_LIBS})
INSTALL(TARGETS SALOMETraceCollectorTest DESTINATION ${KERNEL_salomelib_LIBS})

ADD_EXECUTABLE(TestSALOMETraceCollector TestSALOMETraceCollector.cxx)
TARGET_LINK_LIBRARIES(TestSALOMETraceCollector SALOMETraceCollectorTest ${COMMON_LIBS})
INSTALL(TARGETS TestSALOMETraceCollector DESTINATION ${KERNEL_salomebin_BINS})

# Executable scripts to be installed
SALOME_INSTALL_SCRIPTS(TestSALOMETraceCollector.py ${KERNEL_salomescript_PYTHON})

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${KERNEL_salomeinclude_HEADERS})
