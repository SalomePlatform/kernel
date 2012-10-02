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
  ${CMAKE_CURRENT_SOURCE_DIR}/../Basics
  ${PTHREAD_INCLUDE_DIRS}
)

SET(COMMON_HEADERS
  utilities.h
  LocalTraceBufferPool.hxx
  BaseTraceCollector.hxx
  SALOME_LocalTrace.hxx
)

SET(SALOMELocalTrace_SOURCES
  BaseTraceCollector.cxx
  BaseTraceCollector.hxx
  LocalTraceCollector.cxx
  LocalTraceCollector.hxx
  FileTraceCollector.cxx
  FileTraceCollector.hxx
  LocalTraceBufferPool.cxx
  LocalTraceBufferPool.hxx
  SALOME_LocalTrace.hxx
)

ADD_LIBRARY(SALOMELocalTrace SHARED ${SALOMELocalTrace_SOURCES})
SET_TARGET_PROPERTIES(SALOMELocalTrace PROPERTIES COMPILE_FLAGS "${PTHREAD_DEFINITIONS} ${PLATFORM_DEFINITIONS}")
TARGET_LINK_LIBRARIES(SALOMELocalTrace SALOMEBasics ${PLATFORM_LIBS} ${PTHREAD_LIBS})
INSTALL(TARGETS SALOMELocalTrace DESTINATION ${KERNEL_salomelib_LIBS})

INSTALL(FILES ${COMMON_HEADERS} DESTINATION ${KERNEL_salomeinclude_HEADERS})