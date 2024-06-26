# Copyright (C) 2012-2024  CEA, EDF, OPEN CASCADE
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

include(${SWIG_USE_FILE})

INCLUDE_DIRECTORIES(
  ${PTHREAD_INCLUDE_DIR}
  ${PYTHON_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET(SALOMELog_SOURCES
  libSALOMELog.cxx
)

SET(SALOMEBasics_SOURCES
  BasicsGenericDestructor.cxx
  Basics_Utils.cxx
  Basics_DirUtils.cxx
  KernelBasis.cxx
  HeatMarcel.cxx
  Monitoring.cxx
)

ADD_LIBRARY(SALOMELog ${SALOMELog_SOURCES})
TARGET_LINK_LIBRARIES(SALOMELog ${PLATFORM_LIBS} ${PTHREAD_LIBRARIES})
INSTALL(TARGETS SALOMELog EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

ADD_LIBRARY(SALOMEBasics ${SALOMEBasics_SOURCES})
INSTALL(TARGETS SALOMEBasics EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

TARGET_LINK_LIBRARIES(SALOMEBasics SALOMELog ${PLATFORM_LIBS} ${PTHREAD_LIBRARIES})

FILE(GLOB SALOMEBasics_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${SALOMEBasics_HEADERS_HXX} DESTINATION ${SALOME_INSTALL_HEADERS})

SET(KernelBasis_HEADERS baseutilities.h KernelBasis.hxx KernelBasis.i)
SET(KernelBasis_SOURCES ${KernelBasis_HEADERS})
SET_SOURCE_FILES_PROPERTIES(KernelBasis.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(KernelBasis.i PROPERTIES SWIG_FLAGS "-py3")
SET_SOURCE_FILES_PROPERTIES(KernelBasis_wrap.cpp PROPERTIES COMPILE_FLAGS "-DHAVE_CONFIG_H")
SET(_swig_SCRIPTS ${CMAKE_CURRENT_BINARY_DIR}/KernelBasis.py )
IF(${CMAKE_VERSION} VERSION_LESS "3.8.0") 
  SWIG_ADD_MODULE(KernelBasis python ${KernelBasis_SOURCES})
ELSE()
  SWIG_ADD_LIBRARY(KernelBasis LANGUAGE python SOURCES ${KernelBasis_SOURCES})
ENDIF()
IF(${MACHINE} STREQUAL WINDOWS)
  SET_TARGET_PROPERTIES(_KernelBasis PROPERTIES DEBUG_OUTPUT_NAME _KernelBasis_d)
ENDIF(${MACHINE} STREQUAL WINDOWS)
SWIG_LINK_LIBRARIES(KernelBasis ${PYTHON_LIBRARIES} ${PLATFORM_LIBS} SALOMEBasics)
install(TARGETS _KernelBasis DESTINATION ${SALOME_INSTALL_LIBS})
install(FILES ${KernelBasis_HEADERS} DESTINATION ${SALOME_INSTALL_HEADERS})
SALOME_INSTALL_SCRIPTS("${_swig_SCRIPTS}"  ${SALOME_INSTALL_BINS} EXTRA_DPYS "${SWIG_MODULE_KernelBasis_REAL_NAME}")

SALOME_CONFIGURE_FILE(smIdType.hxx.in smIdType.hxx INSTALL ${SALOME_INSTALL_HEADERS})

