# Copyright (C) 2012-2023  CEA/DEN, EDF R&D, OPEN CASCADE
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

INCLUDE(${SWIG_USE_FILE})

INCLUDE_DIRECTORIES(
  ${PYTHON_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Library
ADD_LIBRARY(ArgvKeeper ArgvKeeper.cxx)
INSTALL(TARGETS ArgvKeeper EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})
INSTALL(FILES ArgvKeeper.hxx DESTINATION ${SALOME_INSTALL_HEADERS})

# Python binding (SWIG)
SET_SOURCE_FILES_PROPERTIES(ArgvKeeper.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(ArgvKeeper.i PROPERTIES SWIG_FLAGS "-py3")
SET_SOURCE_FILES_PROPERTIES(ArgvKeeper_wrap.cpp PROPERTIES COMPILE_FLAGS "-DHAVE_CONFIG_H")
IF(${CMAKE_VERSION} VERSION_LESS "3.8.0") 
  SWIG_ADD_MODULE(ArgvKeeper python ArgvKeeper.i ArgvKeeper.hxx)
ELSE()
  SWIG_ADD_LIBRARY(ArgvKeeper LANGUAGE python SOURCES ArgvKeeper.i ArgvKeeper.hxx)
ENDIF()
IF(${MACHINE} STREQUAL WINDOWS)
  SET_TARGET_PROPERTIES(_ArgvKeeper PROPERTIES DEBUG_OUTPUT_NAME _ArgvKeeper_d)
ENDIF(${MACHINE} STREQUAL WINDOWS)
SWIG_LINK_LIBRARIES(ArgvKeeper ${PYTHON_LIBRARIES} ${PLATFORM_LIBS} ArgvKeeper)
INSTALL(TARGETS _ArgvKeeper DESTINATION ${SALOME_INSTALL_LIBS})
SALOME_INSTALL_SCRIPTS(${CMAKE_CURRENT_BINARY_DIR}/ArgvKeeper.py
                       ${SALOME_INSTALL_BINS}
                       EXTRA_DPYS "${SWIG_MODULE_ArgvKeeper_REAL_NAME}")

# Tests
IF(SALOME_BUILD_TESTS)
  ADD_SUBDIRECTORY(Test)
ENDIF(SALOME_BUILD_TESTS)
