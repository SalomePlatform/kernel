#  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
#
#  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
#  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
#  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
IF(WINDOWS)
  IF(CMAKE_BUILD_TYPE STREQUAL Release)
    FIND_PROGRAM(PYTHON_EXECUTABLE python)
  ELSE(CMAKE_BUILD_TYPE STREQUAL Release)
    FIND_PROGRAM(PYTHON_EXECUTABLE python_d)
  ENDIF(CMAKE_BUILD_TYPE STREQUAL Release)
ELSE(WINDOWS)
  FIND_PROGRAM(PYTHON_EXECUTABLE python)
ENDIF(WINDOWS)

EXECUTE_PROCESS(
  COMMAND ${PYTHON_EXECUTABLE} -c "import sys; sys.stdout.write(sys.version[:3])"
  OUTPUT_VARIABLE PYTHON_VERSION
  )

SET(PYTHON_ROOT ${PYTHON_EXECUTABLE})
GET_FILENAME_COMPONENT(PYTHON_ROOT ${PYTHON_ROOT} PATH)
GET_FILENAME_COMPONENT(PYTHON_ROOT ${PYTHON_ROOT} PATH)

FIND_PATH(PYTHON_INCLUDE_PATH Python.h PATHS ${PYTHON_ROOT}/include ${PYTHON_ROOT}/include/python${PYTHON_VERSION})

IF(WINDOWS)
  STRING(REPLACE "." "" PYTHON_VERSION_WITHOUT_DOT ${PYTHON_VERSION})
  IF(CMAKE_BUILD_TYPE STREQUAL Release)
    FIND_LIBRARY(PYTHON_LIBRARIES python${PYTHON_VERSION_WITHOUT_DOT} ${PYTHON_ROOT}/libs)
  ELSE(CMAKE_BUILD_TYPE STREQUAL Release)
    FIND_LIBRARY(PYTHON_LIBRARIES python${PYTHON_VERSION_WITHOUT_DOT}_d ${PYTHON_ROOT}/libs)
  ENDIF(CMAKE_BUILD_TYPE STREQUAL Release)
ELSE(WINDOWS)
  FIND_LIBRARY(PYTHON_LIBRARIES python${PYTHON_VERSION} ${PYTHON_ROOT}/lib)
ENDIF(WINDOWS)

SET(PYTHON_INCLUDES -I${PYTHON_INCLUDE_PATH})
SET(PYTHON_LIBS ${PYTHON_LIBRARIES})

IF(WINDOWS)
  IF(CMAKE_BUILD_TYPE STREQUAL Release)
  ELSE(CMAKE_BUILD_TYPE STREQUAL Release)
    SET(PYTHON_INCLUDES ${PYTHON_INCLUDES} -DHAVE_DEBUG_PYTHON)
  ENDIF(CMAKE_BUILD_TYPE STREQUAL Release)
ENDIF(WINDOWS)
