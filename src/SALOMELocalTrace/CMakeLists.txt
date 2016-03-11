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
  ${PTHREAD_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../Basics
  ${PTHREAD_INCLUDE_DIR}
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

ADD_LIBRARY(SALOMELocalTrace ${SALOMELocalTrace_SOURCES})
TARGET_LINK_LIBRARIES(SALOMELocalTrace SALOMEBasics ${PLATFORM_LIBS} ${PTHREAD_LIBRARIES})
INSTALL(TARGETS SALOMELocalTrace EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

INSTALL(FILES ${COMMON_HEADERS} DESTINATION ${SALOME_INSTALL_HEADERS})
